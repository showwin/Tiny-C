#include <stdio.h>
#include <stdlib.h>
#include "make_tree.h"
#include "parser.tab.h"
#include "print_tree.h"

int print_program(tree p){
	if (p->tp.op != CONS){
		print_external_decralation(p);
	} else {
		print_program(p->tp.a[0]);
		print_external_decralation(p->tp.a[1]);
	}
	return 0;
}

int print_parm(tree p){
	printf("int %s:%d:%d", p->tk.name, p->tk.lev, p->tk.offset);
	return 0;
}

int print_external_decralation(tree p){
	if (p->tk.op == IDENTIFIER){
		printf(" %s:%d", p->tk.name, p->tk.lev);
	}else if(p->c.op == CONSTANT){
		printf(" %d", p->c.v);
	}else{
		printf(" (");
		switch(p->tp.op){
			case PSTFIX:
				printf("FCALL");
				print_program(p->tp.a[0]);
				if (p->tp.a[1] != NULL) print_program(p->tp.a[1]);
				break;
			case FUNDEF:
				printf("\nint");
				print_program(p->tp.a[0]);
				printf(" (");
				if (p->tp.a[1] != NULL) print_program(p->tp.a[1]);
				printf(") \n");
				print_program(p->tp.a[2]);
				break;
			case INT:
				print_parm(p->tp.a[0]);
				break;
			case RETURN:
				printf("RETURN");
				if (p->tp.a[0] != NULL) print_program(p->tp.a[0]);
				break;
			case IF:
				printf("IF");
				print_program(p->tp.a[0]);
				printf(" ");
				print_program(p->tp.a[1]);
				printf("\n");
				if (p->tp.a[2] != NULL){
					printf("(ELSE");
					print_program(p->tp.a[2]);
					printf(")");
				}
				break;
			case WHILE:
				printf("WHILE");
				print_program(p->tp.a[0]);
				printf("\n");
				print_program(p->tp.a[1]);
				break;
			case ASS:
				printf("=");
				print_program(p->tp.a[0]);
				print_program(p->tp.a[1]);
				break;
			case OR:
				printf("||");
				print_program(p->tp.a[0]);
				print_program(p->tp.a[1]);
				break;
			case AND:
				printf("&&");
				print_program(p->tp.a[0]);
				print_program(p->tp.a[1]);
				break;
			case EQU:
				printf("==");
				print_program(p->tp.a[0]);
				print_program(p->tp.a[1]);
				break;
			case NEQU:
				printf("!=");
				print_program(p->tp.a[0]);
				print_program(p->tp.a[1]);
				break;
			case LT:
				printf("<");
				print_program(p->tp.a[0]);
				print_program(p->tp.a[1]);
				break;
			case MT:
				printf(">");
				print_program(p->tp.a[0]);
				print_program(p->tp.a[1]);
				break;
			case ELT:
				printf("=<");
				print_program(p->tp.a[0]);
				print_program(p->tp.a[1]);
				break;
			case EMT:
				printf("=>");
				print_program(p->tp.a[0]);
				print_program(p->tp.a[1]);
				break;
			case PLUS:
				printf("+");
				print_program(p->tp.a[0]);
				print_program(p->tp.a[1]);
				break;
			case MINUS:
				printf("-");
				if(p->tp.a[0] != NULL ) print_program(p->tp.a[0]);
				print_program(p->tp.a[1]);
				break;
			case MUL:
				printf("*");
				print_program(p->tp.a[0]);
				print_program(p->tp.a[1]);
				break;
			case DIV:
				printf("/");
				print_program(p->tp.a[0]);
				print_program(p->tp.a[1]);
				break;
			case CMPD:
				if(p->tp.a[0] != NULL) 
					print_program(p->tp.a[0]);
				printf("\n");
				if(p->tp.a[1] != NULL) 
					print_program(p->tp.a[1]);
				break;
			case CONS:
				print_program(p->tp.a[0]);
				print_program(p->tp.a[1]);
				break;
			}		
		printf(")");
	}
	return 0;
}
