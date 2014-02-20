#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "make_tree.h"
#include "parser.tab.h"
#include "code_creater.h"
#include "semantic_analyser.h"

struct code *code_list;
struct code *first_code;
int label_num = 0;
int return_label_num = 0;
char *return_label_j;
int f_flag = 0;

void print_code(){
	struct code *looking = first_code;

	while(looking->next){
		printf("%s",looking->cd);
		looking = looking->next;
	}
	printf("%s",looking->cd);
}

struct code *emit(char *inst, char *op1, char *op2, char *label){
	char buf[80];
	struct code *c = (struct code *)malloc(sizeof(struct code));

	if (label == NULL){
		if (inst == NULL)
			buf[0] = '\0';
		else if (op1 == NULL)
			sprintf(buf, "\t%s\n", inst);
		else if (op2 == NULL)
			sprintf(buf, "\t%s\t%s\n", inst, op1);
		else
			sprintf(buf, "\t%s\t%s, %s\n", inst, op1, op2);
	}else{
		if (inst == NULL)
			sprintf(buf, "%s\n", label);
		else if (op1 == NULL)
			sprintf(buf, "%s\t%s\n", label, inst);
		else if (op2 == NULL)
			sprintf(buf, "%s\t%s\t%s\n", label, inst, op1);
		else
			sprintf(buf, "%s\t%s\t%s, %s\n", label, inst, op1, op2);
	}
	
	c->cd = strdup(buf);
	if (code_list){
		code_list->next = c;
		code_list = c;
	} else {
		code_list = c;
		first_code = code_list;
	}

	return c;
}

char *make_label(){
	char n[64];
	char _label[64] = "L";
	char *label;

	label_num++;
	sprintf(n, "%d", label_num);
	strcat(_label, n);
	label = strdup(_label);
	
	return label;
}

char *make_return_label(){
	char n[64];
	char _label[64] = "L";
	char *label;

	return_label_num++;
	sprintf(n, "%d", return_label_num);
	strcat(_label, n);
	strcat(_label, "ret");
	label = strdup(_label);

	return label;
}

int emit_program(tree p){
	if (p->tp.op != CONS){
		emit_external_decralation(p);
	} else {
		emit_program(p->tp.a[0]);
		emit_external_decralation(p->tp.a[1]);
	}
	return 0; }

int emit_external_decralation(tree p){
	tree t;
	//IDENTIFIERかCONSTANTの場合
	if (p->tk.op == IDENTIFIER){
		if (p->tk.lev == 0){
		//大域変数の場合
			if(f_flag){
				char name[64] = "[";
				strcat(name, p->tk.name);
				strcat(name,"]");
				emit("mov", "eax", name, NULL);
			//大域変数の呼び出し
			}else{
			//大域変数の定義
				char name[64];
				strcpy(name, p->tk.name);
				printf("\tCOMMON\t%s\t4\n", name);
			}
		}else{
		//局所変数の場合
		char loc[256] = "[ebp", offset[64];
		if (p->tk.offset < 0){
			sprintf(offset, "%d", p->tk.offset);
		} else 
			sprintf(offset, "+%d", p->tk.offset);
		strcat(loc, offset);
		strcat(loc, "]");
			
		//コード生成
		emit("mov", "eax", loc, NULL);
		}
	}else if(p->c.op == CONSTANT){
		char value[64];
		sprintf(value, "%d", p->c.v);
		emit("mov", "eax", value, NULL);
	}else{
	//IDENTIFIERとCONSTANT以外の場合
		switch(p->tp.op){
			case PSTFIX:{
				int parm_num = 0;
				char address[64];
				if(p->tp.a[0]->tk.kind == UNDEFFUN)
					emit("EXTERN", p->tp.a[0]->tk.name, NULL, NULL);
				//引数がある場合、引数の中身を計算してpushする
				if (p->tp.a[1]){
					tree t = p->tp.a[1];
					while(t->tp.op == CONS){
						emit_program(t->tp.a[1]);
						emit("push", "eax", NULL, NULL);
						parm_num++;
						t = t->tp.a[0];
					}
					emit_program(t);
					emit("push", "eax", NULL, NULL);
					parm_num++;
				}
				//関数呼び出し
				emit("call", p->tp.a[0]->tk.name, NULL, NULL);
				sprintf(address, "%d", parm_num * 4);
				emit("add", "esp", address, NULL);
				break;
			}
			case FUNDEF:{
				//準備(関数名の取得)
				char *fun_name;
				struct code *c;
				char fun_label[64];
				char return_label[64];
				fun_name = strdup(p->tp.a[0]->tk.name);
				return_label_j = make_return_label();
				strcpy(return_label, return_label_j);
				strcat(return_label, ":");
				strcpy(fun_label, fun_name);
				strcat(fun_label, ":");

				//関数定義に入ったことを示すフラグ
				f_flag = 1;

				//コード生成
				emit("GLOBAL", fun_name, NULL, NULL);
				emit("push", "ebp", NULL, fun_label);
				emit("mov", "ebp", "esp", NULL);
				c = emit(NULL, NULL, NULL, NULL);
				//本体のコード生成
				emit_program(p->tp.a[2]);
				//Nlocalの部分のコード生成
				if (top_alloc){
					char buf[80];
					sprintf(buf, "\tsub\tesp, %d\n", -top_alloc);
					c->cd = strdup(buf);
					emit("mov", "esp", "ebp", return_label);
					emit("pop", "ebp", NULL, NULL);
					emit("ret", NULL, NULL, NULL);
				} else {
					emit("pop", "ebp", NULL, return_label);
					emit("ret", NULL, NULL, NULL);
				}
				f_flag = 0;
				break;
			}
			case INT:{
				emit_program(p->tp.a[0]);
				break;
			}
			case RETURN:{
					if (p->tp.a[0] != NULL) 
						emit_program(p->tp.a[0]);
					//FUNDEFで定義したreturnラベルに飛ぶ
					emit("jmp", return_label_j, NULL, NULL);
					}
					break;
			case IF:{
					//ラベル生成
					char *label1_j = make_label(), *label2_j = make_label();			
					char label1[64], label2[64];
					strcpy(label1, label1_j);
					strcpy(label2, label2_j);
					strcat(label1, ":");
					strcat(label2, ":");
					//コード生成
						//条件判定
					emit_program(p->tp.a[0]);
					emit("cmp", "eax", "0", NULL);
					emit("je", label1_j, NULL, NULL);
						//trueだった場合の処理
					emit_program(p->tp.a[1]);
						//elseの有無による場合分け
					if (p->tp.a[2] != NULL){
						emit("jmp", label2_j, NULL, NULL);
						emit(NULL, NULL, NULL, label1);
						emit_program(p->tp.a[2]);
						emit(NULL, NULL, NULL, label2);
					} else {
						emit(NULL, NULL, NULL, label1);
					}
				}
				break;
			case WHILE:{
					//ラベル生成
					char *label1_j = make_label(), *label2_j = make_label();
					char label1[64], label2[64];
					strcpy(label1, label1_j);
					strcpy(label2, label2_j);
					strcat(label1, ":");
					strcat(label2, ":");
					//コード生成
					emit(NULL, NULL, NULL, label1);
						//条件判定
					emit_program(p->tp.a[0]);
					emit("cmp", "eax", "0", NULL);
					emit("je", label2_j, NULL, NULL);
						//trueだった場合の処理
					emit_program(p->tp.a[1]);
					emit("jmp", label1_j, NULL, NULL);
						//falseだった時の移動先
					emit(NULL, NULL, NULL, label2);
				}
				break;
			case ASS:{
				//局所変数のaddressの文字列をつくる
					char loc[64] = "[", offset[64];
					tree t0 = p->tp.a[0];
					if(t0->tk.lev == 0){
						//大域変数の場合
						strcpy(offset, t0->tk.name);	
					}else if(t0->tk.offset < 0){
						//局所変数の場合
						sprintf(offset, "%d", t0->tk.offset);
						strcat(loc, "ebp");
					} else {
						sprintf(offset, "+%d", t0->tk.offset);
						strcat(loc, "ebp");
					}
					strcat(loc, offset);
					strcat(loc, "]");
					
				//コード作成
					emit_program(p->tp.a[1]);
					emit("mov", loc, "eax", NULL);
				}
				break;
			case OR:{
				//準備(一時変数の確保、ラベル生成)
					char temp[64] = "[ebp", address[64];
					char d_temp[64] = "dword ";
					char *label_j = make_label();
					char label[64];
					strcpy(label, label_j);
					strcat(label, ":");
					sprintf(address, "%d", allocate_loc());
					strcat(temp, address);
					strcat(temp, "]");
					strcat(d_temp, temp);
				
				//コード生成
					emit("mov", d_temp, "1", NULL);
					emit_program(p->tp.a[0]);
					emit("cmp", "eax", "1", NULL);
					emit("je", label_j, NULL, NULL);
					emit_program(p->tp.a[1]);
					emit("cmp", "eax", "1", NULL);
					emit("je", label_j, NULL, NULL);
					emit("mov", d_temp, "0", NULL);
					emit("mov", "eax", temp, label);
				
					release_one_loc();
				}
				break;
			case AND:{
				//準備(一時変数の確保、ラベル生成)
					char temp[64] = "[ebp", address[64];
					char d_temp[64] = "dword ";
					char *label_j = make_label();
					char label[64];
					strcpy(label, label_j);
					strcat(label, ":");
					sprintf(address, "%d", allocate_loc());
					strcat(temp, address);
					strcat(temp, "]");
					strcat(d_temp, temp);
				
				//コード生成
					emit("mov", d_temp, "0", NULL);
					emit_program(p->tp.a[0]);
					emit("cmp", "eax", "0", NULL);
					emit("je", label_j, NULL, NULL);
					emit_program(p->tp.a[1]);
					emit("cmp", "eax", "0", NULL);
					emit("je", label_j, NULL, NULL);
					emit("mov", d_temp, "1", NULL);
					emit("mov", "eax", temp, label);
				
					release_one_loc();
				}
				break;
			case EQU:{
				//一時変数の確保
					char temp[64] = "[ebp", address[64];
					sprintf(address, "%d", allocate_loc());
					strcat(temp, address);
					strcat(temp, "]");
				
				//コード生成
					emit_program(p->tp.a[1]);
					emit("mov", temp, "eax", NULL);
					emit_program(p->tp.a[0]);
					emit("cmp", "eax", temp, NULL);
					emit("sete", "al", NULL, NULL);
					emit("movzx", "eax", "al", NULL);
					
					release_one_loc();
				}
				break;
			case NEQU:{
				//一時変数の確保
					char temp[64] = "[ebp", address[64];
					sprintf(address, "%d", allocate_loc());
					strcat(temp, address);
					strcat(temp, "]");
				
				//コード生成
					emit_program(p->tp.a[1]);
					emit("mov", temp, "eax", NULL);
					emit_program(p->tp.a[0]);
					emit("cmp", "eax", temp, NULL);
					emit("setne", "al", NULL, NULL);
					emit("movzx", "eax", "al", NULL);
				
					release_one_loc();
				}
				break;
			case LT:{
				//一時変数の確保
					char temp[64] = "[ebp", address[64];
					sprintf(address, "%d", allocate_loc());
					strcat(temp, address);
					strcat(temp, "]");
				
				//コード生成
					emit_program(p->tp.a[1]);
					emit("mov", temp, "eax", NULL);
					emit_program(p->tp.a[0]);
					emit("cmp", "eax", temp, NULL);
					emit("setl", "al", NULL, NULL);
					emit("movzx", "eax", "al", NULL);
				
					release_one_loc();
				}
				break;
			case MT:{
				//一時変数の確保
					char temp[64] = "[ebp", address[64];
					sprintf(address, "%d", allocate_loc());
					strcat(temp, address);
					strcat(temp, "]");
				
				//コード生成
					emit_program(p->tp.a[1]);
					emit("mov", temp, "eax", NULL);
					emit_program(p->tp.a[0]);
					emit("cmp", "eax", temp, NULL);
					emit("setg", "al", NULL, NULL);
					emit("movzx", "eax", "al", NULL);
				
					release_one_loc();
				}
				break;
			case ELT:{
				//一時変数の確保
					char temp[64] = "[ebp", address[64];
					sprintf(address, "%d", allocate_loc());
					strcat(temp, address);
					strcat(temp, "]");
				
				//コード生成
					emit_program(p->tp.a[1]);
					emit("mov", temp, "eax", NULL);
					emit_program(p->tp.a[0]);
					emit("cmp", "eax", temp, NULL);
					emit("setle", "al", NULL, NULL);
					emit("movzx", "eax", "al", NULL);
				
					release_one_loc();
				}
				break;
			case EMT:{
				//一時変数の確保
					char temp[64] = "[ebp", address[64];
					sprintf(address, "%d", allocate_loc());
					strcat(temp, address);
					strcat(temp, "]");
				
				//コード生成
					emit_program(p->tp.a[1]);
					emit("mov", temp, "eax", NULL);
					emit_program(p->tp.a[0]);
					emit("cmp", "eax", temp, NULL);
					emit("setge", "al", NULL, NULL);
					emit("movzx", "eax", "al", NULL);
				
					release_one_loc();
				}
				break;
			case PLUS:{
				//一時変数の確保
					char temp[64] = "[ebp", address[64];
					sprintf(address, "%d", allocate_loc());
					strcat(temp, address);
					strcat(temp, "]");
					
				//コード生成
					emit_program(p->tp.a[0]);
					emit("mov", temp, "eax", NULL);
					emit_program(p->tp.a[1]);
					emit("add", "eax", temp, NULL);

					release_one_loc();
				}
				break;
			case MINUS:{
				//引き算のマイナス
				//一時変数の確保
					char temp[64] = "[ebp", address[64];
					sprintf(address, "%d", allocate_loc());
					strcat(temp, address);
					strcat(temp, "]");
					
				//コード生成
					emit_program(p->tp.a[1]);
					emit("mov", temp, "eax", NULL);
					if (p->tp.a[0]){
						//引き算
						emit_program(p->tp.a[0]);
					} else {
						//マイナス
						emit("mov", "eax", "0", NULL);
					}
					emit("sub", "eax", temp, NULL);
					release_one_loc();
				}
				break;
			case MUL:{
				//一時変数の確保
					char temp[64] = "[ebp", address[64];
					sprintf(address, "%d", allocate_loc());
					strcat(temp, address);
					strcat(temp, "]");
				
				//コード生成
					emit_program(p->tp.a[0]);
					emit("mov", temp, "eax", NULL);
					emit_program(p->tp.a[1]);
					emit("imul", "eax", temp, NULL);
					
					release_one_loc();
				}
				break;
			case DIV:{
				//一時変数の確保
					char temp[64] = "[ebp", address[64];
					char d_temp[64] = "dword ";
					sprintf(address, "%d", allocate_loc());
					strcat(temp, address);
					strcat(temp, "]");
					strcat(d_temp, temp);
				
				//コード生成
					emit_program(p->tp.a[1]);
					emit("mov", temp, "eax", NULL);
					emit_program(p->tp.a[0]);
					emit("cdq", NULL, NULL, NULL);
					emit("idiv", d_temp, NULL, NULL);

					release_one_loc();
				}
				break;
			case CMPD:{
				int current_loc = last_alloc;
				if(p->tp.a[0] != NULL){
					//局所変数の数を数える
					tree t = p->tp.a[0];
					while(t->tp.op == CONS){
						allocate_loc();
						t = t->tp.a[0];
					}
					allocate_loc();
				}
				if(p->tp.a[1] != NULL) 
					emit_program(p->tp.a[1]);
				//局所変数の分をリリースする
				last_alloc = current_loc;
				break;
			}
			case CONS:
				emit_program(p->tp.a[0]);
				emit_program(p->tp.a[1]);
				break;
			}		
	}
	return 0;
}
