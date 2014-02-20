%{
#include <stdio.h>
#include "make_tree.h"
#include "print_tree.h"
#include "semantic_analyser.h"
%}
%error_verbose
%union {
		int i;
		char *str;
		tree n;
}
%token <str> ASS PLUS MINUS DIV MUL CONS FUNDEF OPE MINS PSTFIX INT RETURN IF ELSE WHILE EQU NEQU ELT LT EMT MT OR AND CMPD 
%token <i> CONSTANT
%token <n> IDENTIFIER 
%type <n> main program external_declaration declaration declarator_list function_definition parameter_type_list parameter_declaration statement compound_statement declaration_list statement_list expression assign_expr logical_or_expr logical_and_expr equality_expr relational_expr add_expr mult_expr unary_expr postfix_expr primary_expr argument_expression_list opt_declaration_list opt_statement_list pst_identifier ass_identifier 
%%
main:
					{ //変数の初期化
						symtab = NULL; cur_lev = 0; parm_num = 0; 
					}
				program 
					{ //構文木の表示	
						last_alloc = 0; top_alloc = 0;
						if (yynerrs == 0 && semnerrs == 0) {
							emit_program($2);
							print_code();
						}	
					}
				;

program:
				external_declaration
					{ $$ = $1; }
				| program external_declaration
					{ $$ = make_tuple(CONS, $1, $2, NULL, NULL); }
				;

external_declaration:
				declaration
					{ $$ = $1; }
				| function_definition
					{ last_alloc = 4; top_alloc = 4; $$ = $1; }
				;

declaration:
				INT declarator_list ';'
					{ $$ = $2; }
				;

declarator_list:
				IDENTIFIER	
					{ 
						tree t = make_decl($1);
						$$ = make_tuple(INT, t, NULL, NULL, NULL);
					}
				| declarator_list ',' IDENTIFIER 
					{ 
						tree t =  make_decl($3);
						t = make_tuple(INT, t, NULL, NULL, NULL);
						$$ = make_tuple(CONS, $1, t, NULL, NULL); 
					}
				;

function_definition:
				INT IDENTIFIER '(' {cur_lev++; last_alloc = 0; top_alloc = 0;} ')' compound_statement
					{ 
						$$ = make_tuple(FUNDEF, make_fun_def($2), NULL, $6, NULL);
						cur_lev--;
						release_loc();
						delete_sym();
					}
				| INT IDENTIFIER '(' {cur_lev++; parm_alloc = 4;} parameter_type_list {last_alloc = 0; top_alloc = 0;} ')' compound_statement
					{ 
						$$ = make_tuple(FUNDEF, make_fun_def($2), $5, $8, NULL);
						cur_lev--;
						release_loc();
						delete_sym();
					}
				;

parameter_type_list:
				parameter_declaration
					{
						parm_num++;
						$$ = $1;
					}
				| parameter_type_list ',' parameter_declaration
					{ 
						parm_num++;
						$$ = make_tuple(CONS, $1, $3, NULL, NULL);
					}
				;

parameter_declaration:
				INT IDENTIFIER 
					{ 
						tree t;
						t = make_parm_decl($2);
						$$ = make_tuple(INT, t, NULL, NULL, NULL);
					}
				;

statement:
				';' {}
				| expression ';'
					{ $$ = $1; }
				| compound_statement
					{ $$ = $1; }
				| IF '(' expression ')' statement
					{ $$ = make_tuple(IF, $3, $5, NULL, NULL); }
				| IF '(' expression ')' statement ELSE statement
					{ $$ = make_tuple(IF, $3, $5, $7, NULL); }
				| WHILE '(' expression ')' statement
					{ $$ = make_tuple(WHILE, $3, $5, NULL, NULL); }
				| RETURN ';'
					{ $$ = make_tuple(RETURN, NULL, NULL, NULL, NULL); }
				| RETURN expression ';'
					{ $$ = make_tuple(RETURN, $2, NULL, NULL, NULL); }
				;

compound_statement:
				'{' '}' {}
				| '{'
						{ cur_lev++; }
					opt_declaration_list opt_statement_list '}'
						{ 
							$$ = make_tuple(CMPD, $3, $4, NULL, NULL); 
							cur_lev--;
							release_loc();
							delete_sym();
						}
				;

opt_declaration_list:
				declaration_list
					{ $$ = $1; }
				| /*NULL*/ 
					{ $$ = NULL; }
				;

opt_statement_list:
				statement_list
					{ $$ = $1; }
				| /*NULL*/ 
					{ $$ = NULL; }
				;

declaration_list:
				declaration
					{ $$ = $1; }
				| declaration_list declaration
					{ $$ = make_tuple(CONS, $1, $2, NULL, NULL); }
				;

statement_list:
				statement
					{ $$ = $1; }
				| statement_list statement
					{ $$ = make_tuple(CONS, $1, $2, NULL, NULL); }
				;

expression:
				assign_expr
					{ $$ = $1; }
				| expression ',' assign_expr
					{ $$ = make_tuple(CONS, $1, $3, NULL, NULL); }
				;

assign_expr:
				logical_or_expr
					{ $$ = $1; }
				| ass_identifier ASS assign_expr
					{ $$ = make_tuple(ASS, $1, $3, NULL, NULL); }
				;

ass_identifier:
				IDENTIFIER
					{ $$ = ref_var($1); }
				;

logical_or_expr:
				logical_and_expr
					{ $$ = $1; }
				| logical_or_expr OR logical_and_expr
					{ $$ = make_tuple(OR, $1, $3, NULL, NULL); }
				;

logical_and_expr:
				equality_expr
					{ $$ = $1; }
				| logical_and_expr AND equality_expr
					{ $$ = make_tuple(AND, $1, $3, NULL, NULL); }
				;

equality_expr:
				relational_expr
					{ $$ = $1; }
				| equality_expr EQU relational_expr
					{ $$ = make_tuple(EQU, $1, $3, NULL, NULL); }
				| equality_expr NEQU relational_expr
					{ $$ = make_tuple(NEQU, $1, $3, NULL, NULL); }
				;

relational_expr:
				add_expr
					{ $$ = $1; }
				| relational_expr LT add_expr
					{ $$ = make_tuple(LT, $1, $3, NULL, NULL); }
				| relational_expr MT add_expr
					{ $$ = make_tuple(MT, $1, $3, NULL, NULL); }
				| relational_expr ELT add_expr
					{ $$ = make_tuple(ELT, $1, $3, NULL, NULL); }
				| relational_expr EMT add_expr
					{ $$ = make_tuple(EMT, $1, $3, NULL, NULL); }
				;

add_expr:
				mult_expr
					{ $$ = $1; }
				| add_expr PLUS mult_expr
					{ $$ = make_tuple(PLUS, $1, $3, NULL, NULL); }
				|	add_expr MINUS mult_expr
					{ $$ = make_tuple(MINUS, $1, $3, NULL, NULL); }
				;

mult_expr:
				unary_expr
					{ $$ = $1; }
				| mult_expr MUL unary_expr
					{ $$ = make_tuple(MUL, $1, $3, NULL, NULL); }
				| mult_expr DIV unary_expr
					{ $$ = make_tuple(DIV, $1, $3, NULL, NULL); }
				;

unary_expr:
				postfix_expr
					{ $$ = $1; }
				| MINUS unary_expr
					{ $$ = make_tuple(MINUS, NULL, $2, NULL, NULL); }
				;

postfix_expr: /*関数呼び出し*/
				primary_expr
					{ $$ = $1; }
				| pst_identifier '(' ')'
					{	$$ = make_tuple(PSTFIX, $1, NULL, NULL, NULL);
					check_parm_num($$);}
				| pst_identifier '(' argument_expression_list ')'
					{	$$ = make_tuple(PSTFIX, $1, $3, NULL, NULL);
					check_parm_num($$);}
				;

pst_identifier:
				IDENTIFIER
					{ $$ = ref_fun($1); }
				;

primary_expr:
				IDENTIFIER
					{	$$ = ref_var($1); }
				| CONSTANT
					{ $$ = make_constant_node($1); }
				| '(' expression ')'
					{ $$ = $2; }
				;

argument_expression_list:
				assign_expr
					{	
						$$ = $1;
					}	
				|argument_expression_list ',' assign_expr
					{	
						$$ = make_tuple(CONS, $1, $3, NULL, NULL);
					}
				;

%%
extern int yylineno;

int yyerror(char *s){
	fprintf(stderr, "%d: %s\n", yylineno, s);
	return 0;
}

int main(){
	yyparse();
	return 0;
}
