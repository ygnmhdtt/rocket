#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>

#include "assembla.h"

#define BUFLEN 256

void error(char *fmt, ...) {
  // multiple arguments
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  fprintf(stderr, "\n");
  va_end(args);
  exit(1);
}

void number(int n) {
  int c;
  while ((c = getc(stdin)) != EOF) {
    if (isspace(c))
      break;
    if (!isdigit(c))
      error("Invalid character in number: '%c'", c);
    n = n * 10 + (c - '0');
  }
  print_num(n);
}

void string(void) {
  char buf[BUFLEN];
  int i = 0;
  for (;;) {
    int c = getc(stdin);
    if (c == EOF)
      error("Unterminated string");
    if (c == '"')
      break;
    if (c == '\\') {
      c = getc(stdin);
      if (c == EOF) error("Unterminated \\");
    }
    buf[i++] = c;
    if (i == BUFLEN - 1)
      error("String too long");
  }
  buf[i] = '\0';
  print_string(buf);
}

void compile(void) {
  int c = getc(stdin);
  if (isdigit(c))
    return number(c - '0'); // convert to int
  if (c == '"')
    return string();
  error("Don't know how to handle '%c'", c);
}


int main(int argc, char **argv) {
  compile();
  return 0;
}
