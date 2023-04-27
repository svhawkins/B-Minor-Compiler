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

int main(int argc, const char* argv[]) {
  struct symbol_table* st = (argc > 1 && !strcmp(argv[1], "-v")) ? symbol_table_verbose_create() : symbol_table_create();
  symbol_table_scope_enter(st); // global scope

  for (int i = 0; !eof; i++) {
    printf("%d: ", i);
    if (yyparse() == 0) {
      if (test_parser_result) {
        stmt_print(test_parser_result, 0);
 	stmt_resolve(st, test_parser_result);
	symbol_table_print(st);
	stmt_destroy(&test_parser_result);
      }
    } else { print_error_message(); }
  }
  if (parser_result) {
    decl_print(parser_result, 0); printf("\n");
    decl_resolve(st, parser_result);
    symbol_table_print(st);
    decl_destroy(&parser_result);
  }
  symbol_table_destroy(&st);
  return 0;
}

