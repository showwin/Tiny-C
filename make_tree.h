#ifndef MAKE_TREE_H
#define MAKE_TREE_H

#define type(t) = t->n.op

#define first(t) t->tp.a[0]
#define second(t) t->tp.a[1]
#define third(t) t->tp.a[2]
#define fourth(t) t->tp.a[3]

typedef struct c {
	int op;
	int v;
}	*constant;

typedef struct tk	{
	int op;
	struct tk *next;
	char *name;
	int lev;
	enum {FRESH, VAR, FUN, PARM, UNDEFFUN} kind;
	int offset;
} *token;

typedef struct tp {
	int op;
	union nd *a[4];
} *tuple;

typedef union nd {
	struct {
		int op;
	} n;
	struct tp tp;
	struct tk tk;
	struct c c;
} *tree;


//以下、グローバル変数と定義と関数のプロトタイプ
extern int cur_lev;
extern token symtab;

tree make_tuple(int, tree, tree, tree, tree);
tree make_token_node(char *);
tree make_constant_node(int);
tree lookup_sym(char *);
void delete_sym();
void globalize_sym(tree n);

#endif
