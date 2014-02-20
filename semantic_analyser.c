#include <stdio.h>
#include <stdarg.h>
#include "make_tree.h"
#include "semantic_analyser.h"
#include "parser.tab.h"

int semnerrs;
int yylineno;
int ass_num;
signed int last_alloc;
signed int top_alloc;
int parm_alloc;
int parm_num;

int allocate_loc(){
	last_alloc += -4;
	if (last_alloc < top_alloc)
		top_alloc = last_alloc;
	return last_alloc;
}

void release_loc(){
	token looking = symtab;

	while(looking->lev > cur_lev){
		last_alloc += 4;
		looking = looking->next;
	}
}

void release_one_loc(){
	last_alloc += 4;
}

int allocate_parm_loc(){
	parm_alloc += 4;
	return parm_alloc;
}

void error(char *fmt, ...){
	va_list argp;
	va_start(argp, fmt);
	semnerrs++;
	fprintf(stderr, "%d: error: ", yylineno);
	vfprintf(stderr, fmt, argp);
	fprintf(stderr, "\n");
	va_end(argp);
}

void warn(char *fmt, ...){
	va_list argp;
	va_start(argp, fmt);
	fprintf(stderr,"%d: warning: ", yylineno);
	vfprintf(stderr, fmt, argp);
	fprintf(stderr, "\n");
	va_end(argp);
}

tree make_decl(tree n){
	switch (n->tk.kind) {
	case VAR: //すでに変数として宣言されている
		if (n->tk.lev == cur_lev)
			//同一レベルであれば二重宣言である
			error("redeclacarion of '%s'", n->tk.name);
		n = make_token_node(n->tk.name);
		break;
	case FUN: //すでに関数として定義されている
	case UNDEFFUN: //すでに未定義関数として使用されている
		if (n->tk.lev == cur_lev)
			//同一レベルであれば二重宣言である
			error("'%s' redeclared as different kind of symbol", n->tk.name);
		n = make_token_node(n->tk.name);
		break;
	case PARM: //すでにパラメータとして名前が宣言されている
		warn("declaration of '%s' shadows a parameter", n->tk.name);
		n = make_token_node(n->tk.name);
		break;
	case FRESH:
		break;
	}	
	n->tk.kind = VAR;
	return n;
}

tree make_parm_decl(tree n){
//引数の数をoffsetに足していく
/*	token looking = symtab;
	while(looking->kind != FUN){
		looking = looking->next;
	}
	looking->offset += 1;
*/
	switch (n->tk.kind){
		case VAR:
		case FUN:
		case UNDEFFUN:
			n = make_token_node(n->tk.name);
			break;
		case PARM:
			error("redeclaration of '%s'", n->tk.name);
			return n;
		case FRESH:
			break;
	}
	n->tk.kind = PARM;
	n->tk.offset = allocate_parm_loc();
	return n;
}

tree make_fun_def(tree n){
	switch (n->tk.kind){
		case VAR:
			error("'%s' redeclared as different kind of symbol", n->tk.kind);
			break;
		case FUN:
			error("redefinition of '%s'", n->tk.name);
			break;
		case UNDEFFUN:
			if(n->tk.offset != parm_num)
				error("function '%s'; the number of parameter is wrong", n->tk.name);
		case FRESH:
			n->tk.kind = FUN;
			break;
		case PARM:
			break;
			//PARMは自分で定義したから合っているか分からない。
	}
	n->tk.offset = parm_num;
	parm_num = 0;
	return n;
}

tree ref_var(tree n){
	switch (n->tk.kind){
		case VAR:
		case PARM:
			break;
		case FUN:
		case UNDEFFUN:
			error("function '%s' is used as variable", n->tk.name);
			break;
		case FRESH:
			error("'%s' undeclared variable", n->tk.name);
			n->tk.kind = VAR; //エラーリカバリ
			break;
	}
	return n;
}

tree ref_fun(tree n){
	switch (n->tk.kind){
		case VAR:
		case PARM:
			error("variable '%s' is used as function", n->tk.name);
			break;
		case FUN:
		case UNDEFFUN:
			break;
		case FRESH:
			warn("'%s' undeclared function", n->tk.name);
			n->tk.kind = UNDEFFUN;
			n->tk.offset = 0;
			if(n->tk.lev > 0) 
				globalize_sym(n);
			break;
	}
	return n;
}

void check_parm_num(tree t){
	tree looking_tree = t->tp.a[0];
	token looking = &looking_tree->tk;
	tree parm_tree = t->tp.a[1];
	int ass_num = 0;

	//引数の数を数える
	if (parm_tree == NULL)
		ass_num = 0;
	else{
		while (parm_tree->tp.op == CONS){
			ass_num += 1;
			parm_tree = parm_tree->tp.a[0];
		}
		ass_num += 1;
	}

	if (looking->kind == UNDEFFUN && looking->offset == 0)
		looking->offset = ass_num;
	else if (looking->offset != ass_num)
		error("the number of parameter is wrong");
}
