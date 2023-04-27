#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../source/symbol_table.h"
#define MAX_BUFFER 256

typedef enum { FAILURE = 0, SUCCESS = 1 } Status;
char test_type[MAX_BUFFER];
char output[MAX_BUFFER];
void print_error(char* test, char* expect, char* value);

// expr_typecheck tests
Status test_expr_typecheck_primitive(void);

int main(void) {
  Status (*tests[])(void) = {
    test_expr_typecheck_primitive,
  };

  int n_tests = sizeof(tests)/sizeof(tests[0]);
  int n_pass = 0;
  printf("Running %d tests...\n", n_tests);
  for (int i = 0; i < n_tests; i++) { if (tests[i]()) { n_pass++; }}

  printf("Passed: %d/%d\n", n_pass, n_tests);
  printf("Failed: %d/%d\n", (n_tests - n_pass), n_tests);
  return 0;
}

void print_error(char* test, char* expect, char* value) { printf("Error:\t[%s]:\n\tExpected a value of %s for value: [%s]\n", test, expect, value); }

Status test_expr_typecheck_primitive(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_primitve");
  Status status = SUCCESS;
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  struct type* types[5] = { type_create(TYPE_INTEGER, NULL, NULL, NULL),
			    type_create(TYPE_BOOLEAN, NULL, NULL, NULL),
			    type_create(TYPE_CHARACTER, NULL, NULL, NULL),
			    type_create(TYPE_STRING, NULL, NULL, NULL),
			    type_create(TYPE_VOID, NULL, NULL, NULL)
			   };

  struct expr* exprs[5] = { expr_create_integer_literal(2),
			    expr_create_boolean_literal(true),
			    expr_create_char_literal('a'),
			    expr_create_string_literal("foo"),
			    expr_create_name(strdup("foo"))
			  };
  struct type* t = NULL; struct symbol* s = NULL;
  for (int i = 0; i < 5; i++) {
    if (i == 4) { s = symbol_create(SYMBOL_GLOBAL, type_copy(types[i]), strdup("foo")); symbol_table_scope_bind(st, "foo", s); }
    expr_resolve(st, exprs[i]); t = expr_typecheck(st, exprs[i]);
    if (!exprs[i]) { print_error(test_type, "NOT NULL", "struct expr* e"); return FAILURE; }
    if (!t) { print_error(test_type, "NOT NULL", "struct type* t"); return FAILURE; }
    if (!type_equals(t, types[i])) { print_error(test_type, "true", "bool type_equals(t, types[i])"); status = FAILURE; }
    type_destroy(&t);
  }
  symbol_table_destroy(&st); type_destroy(&t); for (int i = 0; i < 5; i++) { type_destroy(&types[i]); expr_destroy(&exprs[i]); }
  return status;
}
