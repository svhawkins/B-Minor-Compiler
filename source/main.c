#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expr.h"
#include "type.h"
#include "param_list.h"
#include "stmt.h"
#include "decl.h"

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
  for (int i = 0; !eof; i++) {
    printf("%d: ", i);
    if (yyparse() == 0 && !eof) {
      if (test_parser_result) { stmt_print(test_parser_result, 0); test_parser_result = NULL; }
      //else if (parser_result) { decl_print(parser_result, 0); printf("\n"); }
    }
    else { /*printf("[Parse failure.]\n");*/ print_error_message(); }
  }
  if (parser_result) { decl_print(parser_result, 0); printf("\n"); }
  return 0;
}
