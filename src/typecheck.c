#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "symbol_table.h"

extern int yyparse();
extern unsigned char eof;
extern void print_error_message();
extern FILE* ERR_OUT;

// parser outputs
extern struct stmt* test_parser_result;
extern struct decl* parser_result;

// command line stuff
bool refresh = false, show_hidden = false;
struct symbol_table* st = NULL;
void help(void);
void get_options(int argc, const char* argv[]);
int main(int argc, const char* argv[]) {
  ERR_OUT = stderr;
  get_options(argc, argv);

  register_codegen_init(false); // for labels
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
    decl_resolve(st, parser_result);
    decl_typecheck(st, parser_result);
    decl_print(parser_result, 0); printf("\n");
    symbol_table_print(st);
    if (show_hidden) { symbol_table_hidden_print(st->hidden_table); }
    printf("Total errors: %d\n", global_error_count);
  }
  symbol_table_destroy(&st);
  return 0;
}

void help(void) {
    printf("typecheck [options]: Typechecker for B-Minor (and symbol table printer)\n");
    printf("Options:\n");
    printf("--refresh (-r): clear the symbol table and start anew after EOF.\n"); // lk deprecated.
    printf("--verbose (-v): print all of the stored hash tables in the symbol table, even if out of scope.\n");
    printf("--help (-h): show this help\n");
    printf("--show_hidden (-s): print the 'hidden symbols' (labels) associated with string literals\n");
}


void get_options(int argc, const char* argv[]) {
  struct option long_opts [] = {
    {"refresh", no_argument, NULL, 'r'},
    {"verbose", no_argument, NULL, 'v'},
    {"help", no_argument, NULL, 'h'},
    {"show_hidden", no_argument, 's'},
    {0, 0, 0, 0}
  };

  int c, option_index;
  while(1) {
    c = getopt_long(argc, argv, "rhvs", long_opts, &option_index);
    if (c == -1) { break; }
    switch(c) {
      case 'r': refresh = true; break;
      case 'v': st = symbol_table_verbose_create(); break;
      case 's': show_hidden = true; break;
      case 'h': help(); exit(0);
      case '?': default: fprintf(stderr, "invalid argument\n"); exit(-1);
    }
  }
  if (!st) { st = symbol_table_create(); }
  st->show_hidden = show_hidden;
}
