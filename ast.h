#ifndef _AST_H_
#define _AST_H_

enum {
  AST_INT,
  AST_STR,
};

typedef struct Ast {
  char type; //
  union {
    int ival;
    char *sval;
    struct {
      struct Ast *left;
      struct Ast *right;
    };
  };
} Ast;

void print_int(Ast *ast);
void print_string(Ast *ast);

#endif
