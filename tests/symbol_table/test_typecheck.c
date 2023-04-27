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
enum { INT = 0, BOOL, CHAR, STRING, VOID, AUTO, N_TYPES };

// expr_typecheck tests
Status test_expr_typecheck_primitive(void);


// param_list_typecheck -> param_list_equals, param_list_fcall_compare tests
Status test_param_list_equals_both_null(void);
Status test_param_list_equals_left_null(void);
Status test_param_list_equals_right_null(void);
Status test_param_list_equals_diff(void);
Status test_param_list_equals_same(void);

// decl_typecheck tests
Status test_decl_typecheck(void);
Status test_decl_typecheck_name(void);
Status test_decl_typecheck_auto_primitive(void);

// stmt_typecheck tests

int main(void) {
  Status (*tests[])(void) = {
    test_expr_typecheck_primitive,
    test_param_list_equals_both_null,
    test_param_list_equals_left_null,
    test_param_list_equals_right_null,
    test_param_list_equals_diff,
    test_param_list_equals_same,
    test_decl_typecheck,
    test_decl_typecheck_name,
    test_decl_typecheck_auto_primitive
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

  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct type* boolean = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
  struct type* character = type_create(TYPE_CHARACTER, NULL, NULL, NULL);
  struct type* string = type_create(TYPE_STRING, NULL, NULL, NULL);
  struct type* tvoid = type_create(TYPE_VOID, NULL, NULL, NULL);
  struct type* tauto = type_create(TYPE_AUTO, NULL, NULL, NULL);
  struct type* types[N_TYPES] = { integer, boolean, character, string, tvoid, tauto };

  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  struct expr* exprs[5] = { expr_create_integer_literal(2),
			    expr_create_boolean_literal(true),
			    expr_create_char_literal('a'),
			    expr_create_string_literal("foo"),
			    expr_create_name(strdup("foo"))
			  };
  struct type* t = NULL; struct symbol* s = NULL;
  for (int i = 0; i < 5; i++) {
    // testing name types will be void.
    if (i == VOID) { s = symbol_create(SYMBOL_GLOBAL, type_copy(types[i]), strdup("foo")); symbol_table_scope_bind(st, "foo", s); }
    expr_resolve(st, exprs[i]); t = expr_typecheck(st, exprs[i]);
    if (!exprs[i]) { print_error(test_type, "NOT NULL", "struct expr* e"); return FAILURE; }
    if (!t) { print_error(test_type, "NOT NULL", "struct type* t"); return FAILURE; }
    if (!type_equals(t, types[i])) { print_error(test_type, "true", "bool type_equals(t, types[i])"); status = FAILURE; }
    type_destroy(&t);
  }
  symbol_table_destroy(&st); type_destroy(&t); for (int i = 0; i < 5; i++) { expr_destroy(&exprs[i]); }
  for (int i = 0; i < N_TYPES; i++) { type_destroy(&types[i]); }
  return status;
}

Status test_param_list_equals_both_null(void) {
  strcpy(test_type, "Testing: test_param_list_equals_both_null");
  Status status = SUCCESS;
  struct param_list* left = NULL;
  struct param_list* right = NULL;
  if (!param_list_equals(left, right)) {
    print_error(test_type, "true", "bool param_list_equals(struct param_list* left, struct param_list* right)");
    status = FAILURE;
  }
  param_list_destroy(&left); param_list_destroy(&right);
  return status;
}

Status test_param_list_equals_left_null(void) {
  strcpy(test_type, "Testing: test_param_list_equals_left_null");
  Status status = SUCCESS;
  struct type* tvoid = type_create(TYPE_VOID, NULL, NULL, NULL);
  struct param_list* left = NULL;
  struct param_list* right = param_list_create(strdup("x"), type_copy(tvoid), NULL);
  if (param_list_equals(left, right)) {
    print_error(test_type, "false", "bool param_list_equals(struct param_list* left, struct param_list* right)");
    status = FAILURE;
  }
  param_list_destroy(&left); param_list_destroy(&right); type_destroy(&tvoid);
  return status;
}

Status test_param_list_equals_right_null(void) {
  strcpy(test_type, "Testing: test_param_list_equals_right_null");
  Status status = SUCCESS;
  struct type* tvoid = type_create(TYPE_VOID, NULL, NULL, NULL);
  struct param_list* left = param_list_create(strdup("y"), type_copy(tvoid), NULL);
  struct param_list* right = NULL;
  if (param_list_equals(left, right)) {
    print_error(test_type, "false", "bool param_list_equals(struct param_list* left, struct param_list* right)");
    status = FAILURE;
  }
  param_list_destroy(&left); param_list_destroy(&right); type_destroy(&tvoid);
  return status;
}

Status test_param_list_equals_diff(void) {
  strcpy(test_type, "Testing: test_param_list_equals_diff");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct type* string = type_create(TYPE_STRING, NULL, NULL, NULL);
  struct param_list* left = param_list_create(strdup("x"), type_copy(integer), NULL);
  struct param_list* right = param_list_create(strdup("y"), type_copy(string), NULL);
  if (param_list_equals(left, right)) {
    print_error(test_type, "false", "bool param_list_equals(struct param_list* left, struct param_list* right)");
    status = FAILURE;
  }
  param_list_destroy(&left); param_list_destroy(&right); type_destroy(&integer); type_destroy(&string);
  return status;
}

Status test_param_list_equals_same(void) {
  strcpy(test_type, "Testing: test_param_list_equals_same");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct type* string = type_create(TYPE_STRING, NULL, NULL, NULL);
  struct param_list* left = param_list_create(strdup("x"), type_copy(integer), param_list_create(strdup("y"), type_copy(string), NULL));
  struct param_list* right = param_list_copy(left);
  if (!param_list_equals(left, right)) {
    print_error(test_type, "true", "bool param_list_equals(struct param_list* left, struct param_list* right)");
    status = FAILURE;
  }
  param_list_destroy(&left); param_list_destroy(&right); type_destroy(&integer); type_destroy(&string);
  return status;
}

Status test_decl_typecheck(void) {
  strcpy(test_type, "Testing: test_decl_typecheck");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct type* boolean = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
  struct type* character = type_create(TYPE_CHARACTER, NULL, NULL, NULL);
  struct type* string = type_create(TYPE_STRING, NULL, NULL, NULL);
  struct type* tvoid = type_create(TYPE_VOID, NULL, NULL, NULL);
  struct type* tauto = type_create(TYPE_AUTO, NULL, NULL, NULL);
  struct type* types[N_TYPES] = { integer, boolean, character, string, tvoid, tauto };
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);

  struct decl* decls[4] = { decl_create(strdup("foo"), type_copy(integer), expr_create_integer_literal(2), NULL, NULL),
			    decl_create(strdup("bar"), type_copy(boolean), expr_create_boolean_literal(true), NULL, NULL),
                            decl_create(strdup("baz"), type_copy(character), expr_create_char_literal('a'), NULL, NULL),
                            decl_create(strdup("qux"), type_copy(string), expr_create_string_literal("bar"), NULL, NULL),
                          };

  for (int i = 0; i < 4; i++) {
    if (!decls[i]) { print_error(test_type, "NOT NULL", "struct decl* d"); return FAILURE; }
    decl_resolve(st, decls[i]); decl_typecheck(st, decls[i]);
    if (!(type_equals(decls[i]->type, types[i]))) {
      print_error(test_type, "true", "bool type_equals(decls[i]->type, types[i])");
      status = FAILURE;
    }
  }
  symbol_table_destroy(&st);
  for (int i = 0; i < 4; i++) { decl_destroy(&decls[i]); }
  for (int i = 0; i < N_TYPES; i++) { type_destroy(&types[i]); }
  return status;
}

Status test_decl_typecheck_name(void) {
  strcpy(test_type, "Testing: test_decl_typecheck_name");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct decl* d = decl_create(strdup("foo"), type_copy(integer), expr_create_name(strdup("x")), NULL, NULL);
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  struct symbol* s = symbol_create(SYMBOL_GLOBAL, type_copy(integer), strdup("x")); symbol_table_scope_bind(st, "x", s);
  decl_resolve(st, d); decl_typecheck(st, d);
  if (!d) { print_error(test_type, "NOT NULL", "struct decl* d"); return FAILURE; }
  if (!(type_equals(d->type, integer))) {
    print_error(test_type, "true", "bool type_equals(d->type, integer)");
    status = FAILURE;
  }
  symbol_table_destroy(&st); decl_destroy(&d); type_destroy(&integer);
  return status;
}

Status test_decl_typecheck_auto_primitive(void) {
  strcpy(test_type, "Testing: test_decl_typecheck_auto_primtive");
  Status status = SUCCESS;
  struct type* tauto = type_create(TYPE_AUTO, NULL, NULL, NULL);
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct decl* d = decl_create(strdup("foo"), type_copy(tauto), expr_create_integer_literal(493), NULL, NULL);
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  decl_resolve(st, d); decl_typecheck(st, d);
  if (!d) { print_error(test_type, "NOT NULL", "struct decl* d"); return FAILURE; }
  if (!(type_equals(d->type, integer))) {
    print_error(test_type, "true", "bool type_equals(d->type, integer)");
    status = FAILURE;
  }
  if (!(type_equals(d->symbol->type, integer))) {
    print_error(test_type, "true", "bool type_equals(d->symbol->type, integer)");
    status = FAILURE;
  }
  symbol_table_destroy(&st); decl_destroy(&d); type_destroy(&integer); type_destroy(&tauto);
  return status;
}
