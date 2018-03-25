#include <stdio.h>

#include "ast.h"
#include "error.h"

void emit_intexpr(Ast *ast);

void print_quote(char *p) {
  while (*p) {
    if (*p == '\"' || *p == '\\')
      printf("\\");
    printf("%c", *p);
    p++;
  }
}

void print_string(Ast *ast) {
  printf("\t.data\n"
         ".mydata:\n\t"
         ".string \"");
  print_quote(ast->sval);
  printf("\"\n\t"
         ".text\n\t"
         ".global stringfn\n"
         "stringfn:\n\t"
         "lea .mydata(%%rip), %%rax\n\t"
         "ret\n");
  return;
}

void emit_binop(Ast *ast) {
  char *op;
  switch (ast->type) {
    case '+': op = "add"; break;
    case '-': op = "sub"; break;
    case '*': op = "imul"; break;
    case '/': break;
    default: error("invalid operator '%c'", ast->type);
  }
  emit_intexpr(ast->left);
  printf("push %%rax\n\t");
  emit_intexpr(ast->right);
  if (ast->type == '/') {
    printf("mov %%eax, %%ebx\n\t");
    printf("pop %%rax\n\t");
    printf("mov $0, %%edx\n\t");
    printf("idiv %%ebx\n\t");
  } else {
    printf("pop %%rbx\n\t");
    printf("%s %%ebx, %%eax\n\t", op);
  }
}

void emit_intexpr(Ast *ast) {
  if (ast->type == AST_INT)
    printf("mov $%d, %%eax\n\t", ast->ival);
  else
    emit_binop(ast);
}

void print_int(Ast *ast) {
  printf(".text\n\t"
         ".global intfn\n"
         "intfn:\n\t");
  emit_intexpr(ast);
  printf("ret\n");
}
