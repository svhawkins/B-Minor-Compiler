#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

// for debugging purposes
int yydebug = 0;

extern int yyparse();
extern unsigned char eof;
extern void print_error_message();

// parser outputs
extern struct stmt* test_parser_result;
extern struct decl* parser_result;

int main(int argc, const char* argv[]) {
  if (argc > 1 && !strcmp(argv[1], "-t")) { yydebug = 1; }
/*  for (int i = 0; !eof; i++) {
    printf("%d: ", i);
    if (yyparse() == 0) {
      if (test_parser_result) { stmt_print(test_parser_result, 0); test_parser_result = NULL; }
    } else { print_error_message(); }
  }
  if (parser_result) { decl_print(parser_result, 0); printf("\n"); } */

  Symbol_table* st = symbol_table_create();
  for(int i = 0; i < 3; i++) symbol_table_scope_enter(st);
  symbol_table_scope_enter(st);
  symbol_table_scope_bind(st, "foo", symbol_create(SYMBOL_GLOBAL, NULL, strdup("bar")));

  symbol_table_scope_enter(st);
  symbol_table_scope_bind(st, "duck", symbol_create(SYMBOL_GLOBAL, NULL, strdup("goose")));

  struct symbol* ret = NULL;
  ret = symbol_table_scope_lookup(st, "foo");
  if (ret) printf("[%s]\n", ret->name);
  ret = symbol_table_scope_lookup(st, "duck");
  if (ret) printf("[%s]\n", ret->name);
  symbol_table_print(st);

  symbol_table_destroy(&st);
  //printf("Hello world! :)\n");
  return 0;
}

