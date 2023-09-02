#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

extern int yyparse();
extern unsigned char eof;
extern void print_error_message();
extern FILE *REG_ERR_OUT, *ERR_OUT;

// parser outputs
extern struct stmt* test_parser_result;
extern struct decl* parser_result;

// other outputs
extern struct decl* decl_hidden_list = NULL;
extern struct decl* decl_hidden_list_tail = NULL;

int main(int argc, const char** argv) {
  ERR_OUT = REG_ERR_OUT = stderr;
  printf("Hello world!\n");
  return 0;
}
