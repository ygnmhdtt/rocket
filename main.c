#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>

#define BUFLEN 256

#include "ast.h"
#include "error.h"

Ast *read_string(void);
Ast *read_expr(void);

Ast *ast_op(char type, Ast *left, Ast *right) {
  Ast *r = malloc(sizeof(Ast));
  r->type = type;
  r->left = left;
  r->right = right;
  return r;
}

Ast *ast_int(int val) {
  Ast *r = malloc(sizeof(Ast));
  r->type = AST_INT;
  r->ival = val;
  return r;
}

Ast *ast_str(char *str) {
  Ast *r = malloc(sizeof(Ast));
  r->type = AST_STR;
  r->sval = str;
  return r;
}

void skip_space(void) {
  int c;
  while ((c = getc(stdin)) != EOF) {
    if (isspace(c))
      continue;
    ungetc(c, stdin);
    return;
  }
}

int priority(char op) {
  switch (op) {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    default:
      error("Unknown binary operator: %c", op);
  }
}

Ast *read_number(int n) {
  for (;;) {
    int c = getc(stdin);
    if (!isdigit(c)) {
      ungetc(c, stdin);
      return ast_int(n);
    }
    n = n * 10 + (c - '0');
  }
}

Ast *read_string(void) {
  char *buf = malloc(BUFLEN);
  int i = 0;
  for (;;) {
    int c = getc(stdin);
    if (c == EOF)
      error("Unterminated string");
    if (c == '"') break;
    buf[i++] = c;
    if (i == BUFLEN - 1)
      error("String too long");
  }
  buf[i] = '\0';
  return ast_str(buf);
}

Ast *read_prim(void) {
  int c = getc(stdin);
  if (isdigit(c))
    return read_number(c - '0');
  else if (c == '"')
    return read_string();
  else if (c == EOF)
    error("Unexpected EOF");
  error("Don't know how to handle '%c'", c);
}

Ast *new_ast(int prec) {
  Ast *ast = read_prim();
  for (;;) {
    skip_space();
    int c = getc(stdin);
    if (c == EOF) return ast; // if string, will return now
    int prec2 = priority(c);
    if (prec2 < prec) {
      ungetc(c, stdin);
      return ast;
    }
    skip_space();
    ast = ast_op(c, ast, new_ast(prec2 + 1));
  }
  return ast;
}

void compile(Ast *ast) {
  if (ast->type == AST_STR) {
    print_string(ast);
  } else {
    print_int(ast);
  }
}

int main(int argc, char **argv) {
  Ast *ast = new_ast(0);
  compile(ast);
  return 0;
}
