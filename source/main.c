#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

extern int yyparse();
extern unsigned char eof;
extern void print_error_message();

// parser outputs
extern struct stmt* test_parser_result;
extern struct decl* parser_result;

int main(int argc, const char** argv) {
  printf("Hello world!\n");
  return 0;
}
