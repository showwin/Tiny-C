YACC = bison
YFLAGS = -d -v
LEX = flex
LFLAGS = 
OBJS = make_tree.o semantic_analyser.o code_creater.o parser.tab.o lex.yy.o 
all: tcc 

parser.tab.c: parser.y
	$(YACC) $(YFLAGS) parser.y
lex.yy.c: lexer.l parser.tab.h
	$(LEX) $(LFLAGS) lexer.l
parser.tab.h: parser.y
	$(YACC) $(YFLAGS) parser.y
tcc: $(OBJS)
	$(CC) -o $@ $(OBJS)

clean:
	rm *.o
	rm tcc
