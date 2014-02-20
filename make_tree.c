#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "make_tree.h"
#include "parser.tab.h"

int cur_lev;
token symtab;

tree make_tuple(int a, tree t1, tree t2, tree t3, tree t4){
	tree tr;
	tr = (tree)malloc(sizeof(union nd));
	
	tr->tp.a[0] = t1;
	tr->tp.a[1] = t2;
	tr->tp.a[2] = t3;
	tr->tp.a[3] = t4;
	tr->tp.op = a;

	return tr;
}


//offsetに何の値を入れるのか？
//cur_levの値をどこからもってくるのか
tree make_token_node(char *name){
	
	tree tr;
	tr = (tree)malloc(sizeof(union nd));

	tr->tk.op = IDENTIFIER;
	tr->tk.next = symtab;
	tr->tk.name = strdup(name);
	tr->tk.lev = cur_lev;
	tr->tk.kind = FRESH;
	tr->tk.offset = allocate_loc();
	
	symtab = (token)tr;

	return tr;
}

tree make_constant_node(int i){
	tree tr;	
	tr = (tree)malloc(sizeof(union nd));

	tr->c.op = CONSTANT;
	tr->c.v = i;

	return tr;
}

tree lookup_sym(char *name){
	token looking = symtab;

	if(looking == NULL) return NULL;
	while(strcmp(name,looking->name)){
		if(looking->next == NULL) return NULL;
		looking = looking->next;
	}

	return (tree)looking;
}

void delete_sym(){
	token looking;

	while(symtab->lev > cur_lev){
		symtab = symtab->next;
	}

	looking = symtab;
	while(looking){
		looking = looking->next;
	}
}


void globalize_sym(tree t){
	token undef = symtab;
	token undef_before = NULL;
	token looking = symtab;
	
	//undeffunのトークンを探す
	while(undef != &t->tk){
		undef_before = undef;
		undef = undef->next;
	}
	
	//undefを抜く分の修正
	if(undef_before)
		undef_before->next = undef->next;
	else{
		symtab = undef->next;
		looking = symtab;
	}

	//undefの情報を適切にする
	undef->lev = 0;
	undef->next = NULL;
	
	//symtabのトークンを探す。
	while(looking->next){
		looking = looking->next;
	}
	//挿入する
	looking->next = undef;
}

