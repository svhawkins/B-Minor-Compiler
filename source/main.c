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
  struct symbol_table* st = NULL;
  bool refresh = false;
  switch (argc) {
    case 1: st = symbol_table_create(); break;
    case 2:
        st =  (!strcmp(argv[1], "-v")) ? symbol_table_verbose_create() : symbol_table_create();
        if (!strcmp(argv[1], "-c")) refresh = true;
        break;
    case 3:
      st = (!strcmp(argv[1], "-v") || !strcmp(argv[2], "-v")) ? symbol_table_verbose_create() : symbol_table_create();
      if (!strcmp(argv[1], "-c") || !strcmp(argv[2], "-c")) { refresh = true; }
    break;
  }

  symbol_table_scope_enter(st); // global scope
  for (int i = 0; !eof; i++) {
    printf("%d: ", i);
    if (yyparse() == 0) {
      if (test_parser_result) {
        stmt_print(test_parser_result, 0);
 	stmt_resolve(st, test_parser_result);
	stmt_typecheck(st, test_parser_result, NULL);
	symbol_table_print(st);
        printf("Total errors: %d\n", global_error_count);
	stmt_destroy(&test_parser_result);
        if (refresh) { st = symbol_table_clear(st); }
      }
    } else { print_error_message(); }
  }
  if (parser_result) {
    decl_print(parser_result, 0); printf("\n");
    decl_resolve(st, parser_result);
    decl_typecheck(st, parser_result);
    symbol_table_print(st);
    decl_destroy(&parser_result);
    printf("Total errors: %d\n", global_error_count);
  }
  symbol_table_destroy(&st);
  return 0;
}

