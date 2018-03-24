#include <stdio.h>
#include "assembla.h"

void print_num(int n) {
  printf(".text\n\t"
         ".global intfn\n"
         "intfn:\n\t"
         "mov $%d, %%rax\n\t"
         "ret\n", n);
}

void print_string(char buf[]) {
  printf("\t.data\n"
         ".mydata:\n\t"
         ".string \"%s\"\n\t"
         ".text\n\t"
         ".global stringfn\n"
         "stringfn:\n\t"
         "lea .mydata(%%rip), %%rax\n\t"
         "ret\n", buf);
}
