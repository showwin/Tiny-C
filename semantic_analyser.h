#ifndef SEMANTIC_ANALYSER_H
#define SEMANTIC_ANALYSER_H

extern int semnerrs;
extern int yylineno;
extern signed int last_alloc;
extern signed int top_alloc;
extern int parm_alloc;
extern int parm_num;

int allocate_loc();
void release_loc();
void release_one_loc();
int allocate_parm_loc();
void erroe(char *, ...);
void warn(char *, ...);
tree make_decl(tree);
tree make_parm_decl(tree);
tree make_fun_def(tree);
tree ref_var(tree);
tree ref_fun(tree);

#endif
