struct code{
	char *cd;
	struct code *next;
};

extern struct code *code_list;
extern struct code *first_code;

struct code *emit(char*, char*, char*, char*);
void print_code();
char *make_label();
int emit_program(tree);
int emit_external_decralation(tree);
