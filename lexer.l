%option noyywrap
%option yylineno
%{
#include "make_tree.h"
#include "parser.tab.h"
%}
%%
int {
		return INT;
	}

if	{
		return IF;
	}

else	{
		return ELSE;
	}

while	{
		return WHILE;
	}

return	{
		return RETURN;
	}

[A-Za-z][A-Za-z0-9\_]*	{
	//ここでIDENTIFIERをtree型にして返す
		if (!(yylval.n = lookup_sym(yytext))){
			yylval.n = make_token_node(yytext);
			//printf("symtab.name = %s in lexer\n",symtab->name);
			}
		return IDENTIFIER;
	}

[0-9]+ 	{
		yylval.i = atoi(yytext);
  	return CONSTANT;
	}

"=""="	{
		yylval.str = strdup(yytext);
		return EQU;			
	}

"!""="	{
		yylval.str = strdup(yytext);
		return NEQU;
	}

"|""|"	{
		yylval.str = strdup(yytext);
		return OR;
	}

"&""&"	{
		yylval.str = strdup(yytext);
		return AND;
	}

"<""="	{
		yylval.str = strdup(yytext);
		return ELT;
	}

">""="	{
		yylval.str = strdup(yytext);
		return EMT;
	}

"<"	{
		yylval.str = strdup(yytext);
		return LT;
	}

">"	{
		yylval.str = strdup(yytext);
		return MT;
	}

"=" {
		yylval.str = strdup(yytext);
		return ASS;
	}

"+" {
		yylval.str = strdup(yytext);	
		return PLUS;
	}

"-" {
		yylval.str = strdup(yytext);
		return MINUS;
	}

"/" {
		yylval.str = strdup(yytext);
		return DIV;
	}

"*" {
		yylval.str = strdup(yytext);
		return MUL;
	}

"("|")"|"{"|"}"|","|";"	 {
		yylval.str = strdup(yytext);
		return *yytext;
	}

[ \t]+|\n 	 //スペース、タブ、改行は無視

.					yyerror("Error: invalid character");

%%
