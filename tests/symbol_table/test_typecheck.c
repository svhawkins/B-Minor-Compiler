#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../src/symbol_table.h"
#define MAX_BUFFER 256

typedef enum { FAILURE = 0, SUCCESS = 1 } Status;
char test_type[MAX_BUFFER];
char output[MAX_BUFFER];
void print_error(char* test, char* expect, char* value);
enum { INT = 0, BOOL, CHAR, STRING, VOID, AUTO, N_TYPES };

extern FILE* ERR_OUT;

// expr_typecheck tests
Status test_expr_typecheck_primitive(void);
Status test_expr_typecheck_logical_bad(void);
Status test_expr_typecheck_logical_good(void);
Status test_expr_typecheck_arithmetic_bad(void);
Status test_expr_typecheck_arithmetic_good(void);
Status test_expr_typecheck_relational_bad(void);
Status test_expr_typecheck_relational_good(void);
Status test_expr_typecheck_equality_bad_type_mismatch(void);
Status test_expr_typecheck_equality_bad_type_invalid(void);
Status test_expr_typecheck_equality_good(void);
Status test_expr_typecheck_subscript_bad_array(void);
Status test_expr_typecheck_subscript_bad_integer(void);
Status test_expr_typecheck_subscript_good(void);
Status test_expr_typecheck_comma_good(void);
Status test_expr_typecheck_assign_bad(void);
Status test_expr_typecheck_assign_good(void);
Status test_expr_typecheck_assign_bad_lval(void);
Status test_expr_typecheck_assign_good_subscript(void);
Status test_expr_typecheck_assign_bad_string(void);
Status test_expr_typecheck_fcall_bad_function(void);
Status test_expr_typecheck_fcall_bad_param(void);
Status test_expr_typecheck_fcall_good_no_param(void);
Status test_expr_typecheck_fcall_good_one_param(void);
Status test_expr_typecheck_fcall_good_many_param(void);
Status test_expr_typecheck_init_good_one(void);
Status test_expr_typecheck_init_bad(void);
Status test_expr_typecheck_init_good_many(void);
Status test_expr_typecheck_init_good_multidim(void);


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
Status test_decl_typecheck_auto_array(void);
Status test_decl_typecheck_value_type_mismatch(void);
Status test_decl_typecheck_function_ret_type_integer(void);
Status test_decl_typecheck_function_ret_type_null(void);
Status test_decl_typecheck_function_ret_type_mismatch(void);
Status test_decl_typecheck_array_null_size(void);
Status test_decl_typecheck_array_null_size_nonnull_body(void);
Status test_decl_typecheck_array_nint_size(void);

// stmt_typecheck tests
Status test_stmt_typecheck_print_good(void);
Status test_stmt_typecheck_print_bad(void);
Status test_stmt_typecheck_expr_good(void);
Status test_stmt_typecheck_expr_bad(void);
Status test_stmt_typecheck_expr_bad_comma(void);
Status test_stmt_typecheck_expr_bad_assign(void);

int main(void) {
  ERR_OUT = fopen("error_output_typecheck.txt", "w");
  if (!ERR_OUT) { ERR_OUT = stderr; }

  Status (*tests[])(void) = {
    test_expr_typecheck_primitive,
    test_expr_typecheck_logical_bad,
    test_expr_typecheck_logical_good,
    test_expr_typecheck_arithmetic_bad,
    test_expr_typecheck_arithmetic_good,
    test_expr_typecheck_relational_bad,
    test_expr_typecheck_relational_good,
    test_expr_typecheck_equality_bad_type_mismatch,
    test_expr_typecheck_equality_bad_type_invalid,
    test_expr_typecheck_equality_good,
    test_expr_typecheck_subscript_bad_array,
    test_expr_typecheck_subscript_bad_integer,
    test_expr_typecheck_subscript_good,
    test_expr_typecheck_comma_good,
    test_expr_typecheck_assign_bad,
    test_expr_typecheck_assign_good,
    test_expr_typecheck_assign_bad_lval,
    test_expr_typecheck_assign_good_subscript,
    test_expr_typecheck_assign_bad_string,
    test_expr_typecheck_fcall_bad_function,
    test_expr_typecheck_fcall_bad_param,
    test_expr_typecheck_fcall_good_no_param,
    test_expr_typecheck_fcall_good_one_param,
    test_expr_typecheck_fcall_good_many_param,
    test_expr_typecheck_init_good_one,
    test_expr_typecheck_init_bad,
    test_expr_typecheck_init_good_many,
    test_expr_typecheck_init_good_multidim,
    test_param_list_equals_both_null,
    test_param_list_equals_left_null,
    test_param_list_equals_right_null,
    test_param_list_equals_diff,
    test_param_list_equals_same,
    test_decl_typecheck,
    test_decl_typecheck_name,
    test_decl_typecheck_auto_primitive,
    test_decl_typecheck_auto_array,
    test_decl_typecheck_value_type_mismatch,
    test_decl_typecheck_function_ret_type_integer,
    test_decl_typecheck_function_ret_type_null,
    test_decl_typecheck_function_ret_type_mismatch,

    test_decl_typecheck_array_null_size,
    test_decl_typecheck_array_null_size_nonnull_body,
    test_decl_typecheck_array_nint_size,

    test_stmt_typecheck_print_good,
    test_stmt_typecheck_print_bad,
    test_stmt_typecheck_expr_good,
    test_stmt_typecheck_expr_bad,
    test_stmt_typecheck_expr_bad_comma,
    test_stmt_typecheck_expr_bad_assign
  };

  int n_tests = sizeof(tests)/sizeof(tests[0]);
  int n_pass = 0;
  printf("Running %d tests...\n", n_tests);
  for (int i = 0; i < n_tests; i++) { if (tests[i]()) { n_pass++; }}

  printf("Passed: %d/%d\n", n_pass, n_tests);
  printf("Failed: %d/%d\n", (n_tests - n_pass), n_tests);

  fclose(ERR_OUT);
  return 0;
}

void print_error(char* test, char* expect, char* value) {
  printf("Error:\t[%s]:\n\tExpected a value of %s for value: [%s]\n", test, expect, value);
}

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
    decl_resolve(st, decls[i]);
    decl_typecheck(st, decls[i]);
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
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st); symbol_table_scope_enter(st);
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
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st); symbol_table_scope_enter(st);
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

Status test_decl_typecheck_auto_array(void) {
  strcpy(test_type, "Testing: test_decl_typecheck_auto_array");
  Status status = SUCCESS;
  struct type* tauto = type_create(TYPE_AUTO, NULL, NULL, NULL);
  struct type* array_integer = type_create(TYPE_ARRAY, type_create(TYPE_INTEGER, NULL, NULL, NULL), NULL, NULL);
  struct decl* d = decl_create(strdup("foo"), type_copy(tauto), expr_create(EXPR_INIT, expr_create_integer_literal(493), NULL), NULL, NULL);
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st); symbol_table_scope_enter(st);
  decl_resolve(st, d); decl_typecheck(st, d);
  if (!d) { print_error(test_type, "NOT NULL", "struct decl* d"); return FAILURE; }
  if (!(type_equals(d->type, array_integer))) {
    print_error(test_type, "true", "bool type_equals(d->type, array_integer)");
    status = FAILURE;
  }
  if (!(type_equals(d->symbol->type, array_integer))) {
    print_error(test_type, "true", "bool type_equals(d->symbol->type, array_integer)");
    status = FAILURE;
  }
  symbol_table_destroy(&st); decl_destroy(&d); type_destroy(&array_integer); type_destroy(&tauto);
  return status;
}

Status test_expr_typecheck_logical_bad(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_logical_bad");
  Status status = SUCCESS;
  struct type* boolean = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  int operators[3] = { EXPR_AND, EXPR_OR, EXPR_NOT };
  enum { TYPE_MISMATCH = 0, INCOMPATIBLE, N_ERROR };

  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < N_ERROR; j++) {
      struct expr* left = NULL; struct expr* right = NULL; struct expr* e = NULL;
      switch (j) {
        case TYPE_MISMATCH: left = expr_create_integer_literal(493); right = expr_create_boolean_literal(true); break;
        case INCOMPATIBLE: left = expr_create_integer_literal(493); right = (i < 2) ? expr_copy(left) : NULL; break;
      }
      e = expr_create(operators[i], left, right); expr_resolve(st, e);
      struct type* t = expr_typecheck(st, e);
      if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
      if (error_status != LOGIC) { print_error(test_type, "LOGIC", "int error_status"); status = FAILURE; }
      expr_destroy(&e); type_destroy(&t);
    }
  }
  symbol_table_destroy(&st); type_destroy(&boolean); type_destroy(&integer);
  return status;
}

Status test_expr_typecheck_logical_good(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_logical_good");
  Status status = SUCCESS;
  struct type* boolean = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
  int operators[3] = { EXPR_AND, EXPR_OR, EXPR_NOT };
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  for (int i = 0; i < 3; i++) {
    struct expr* left = expr_create_boolean_literal(true);
    struct expr* right = (i < 2) ? expr_create_boolean_literal(true) : NULL;
    struct expr* e = expr_create(operators[i], left, right); expr_resolve(st, e);
    struct type* t = expr_typecheck(st, e);
    if (!type_equals(t, boolean)) { print_error(test_type, "true", "bool type_equals(t, boolean)"); status = FAILURE; }
    if (global_error_count) { print_error(test_type, "0", "int global_error_count"); status = FAILURE; }
    if (error_status) { print_error(test_type, "0", "int error_status"); status = FAILURE; }
    expr_destroy(&e); type_destroy(&t);
  }
  symbol_table_destroy(&st); type_destroy(&boolean);
  return status;
}

Status test_expr_typecheck_arithmetic_bad(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_arithmetic_bad");
  Status status = SUCCESS;
  struct type* boolean = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  int operators[10] = { EXPR_ADD, EXPR_SUB, EXPR_MULT, EXPR_DIV, EXPR_MOD, EXPR_EXP, EXPR_INC, EXPR_DEC, EXPR_POS, EXPR_NEG };
  enum { TYPE_MISMATCH = 0, INCOMPATIBLE, N_ERROR };

  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < N_ERROR; j++) {
      struct expr* left = NULL; struct expr* right = NULL; struct expr* e = NULL;
      switch (j) {
        case TYPE_MISMATCH: left = expr_create_boolean_literal(true); right = (i < 6) ? expr_create_integer_literal(493) : NULL; break;
        case INCOMPATIBLE: left = expr_create_boolean_literal(true); right = (i < 6) ? expr_copy(left) : NULL; break;
      }
      e = expr_create(operators[i], left, right); expr_resolve(st, e);
      struct type* t = expr_typecheck(st, e);
      if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
      if (error_status != MATH) { print_error(test_type, "MATH", "int error_status"); status = FAILURE; }
      expr_destroy(&e); type_destroy(&t);
    }
  }
  symbol_table_destroy(&st); type_destroy(&boolean); type_destroy(&integer);
  return status;
}

Status test_expr_typecheck_arithmetic_good(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_arithmetic_good");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  int operators[10] = { EXPR_ADD, EXPR_SUB, EXPR_MULT, EXPR_DIV, EXPR_MOD, EXPR_EXP, EXPR_INC, EXPR_DEC, EXPR_POS, EXPR_NEG };
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  for (int i = 0; i < 10; i++) {
    struct expr* left = expr_create_integer_literal(493);
    struct expr* right = expr_create_integer_literal(493);
    struct expr* e = expr_create(operators[i], left, right); expr_resolve(st, e);
    struct type* t = expr_typecheck(st, e);
    if (!type_equals(t, integer)) { print_error(test_type, "true", "bool type_equals(t, integer)"); status = FAILURE; }
    if (global_error_count) { print_error(test_type, "0", "int global_error_count"); status = FAILURE; }
    if (error_status) { print_error(test_type, "0", "int error_status"); status = FAILURE; }
    expr_destroy(&e); type_destroy(&t);
  }
  symbol_table_destroy(&st); type_destroy(&integer);
  return status;
}

Status test_expr_typecheck_relational_bad(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_relational_bad");
  Status status = SUCCESS;
  struct type* boolean = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  int operators[4] = { EXPR_GREAT, EXPR_GEQ, EXPR_LESS, EXPR_LEQ };
  enum { TYPE_MISMATCH = 0, INCOMPATIBLE, N_ERROR };

  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < N_ERROR; j++) {
      struct expr* left = NULL; struct expr* right = NULL; struct expr* e = NULL;
      switch (j) {
        case TYPE_MISMATCH: left = expr_create_boolean_literal(true); right = expr_create_integer_literal(493); break;
        case INCOMPATIBLE: left = expr_create_boolean_literal(true); right = expr_copy(left); break;
      }
      e = expr_create(operators[i], left, right); expr_resolve(st, e);
      struct type* t = expr_typecheck(st, e);
      if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
      if (error_status != RELATE) { print_error(test_type, "RELATE", "int error_status"); status = FAILURE; }
      expr_destroy(&e); type_destroy(&t);
    }
  }
  symbol_table_destroy(&st); type_destroy(&boolean); type_destroy(&integer);
  return status;
}

Status test_expr_typecheck_relational_good(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_relational_good");
  Status status = SUCCESS;
  struct type* boolean = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  int operators[4] = { EXPR_GREAT, EXPR_GEQ, EXPR_LESS, EXPR_LEQ };
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  for (int i = 0; i < 4; i++) {
    struct expr* left = expr_create_integer_literal(493);
    struct expr* right = expr_create_integer_literal(493);
    struct expr* e = expr_create(operators[i], left, right); expr_resolve(st, e);
    struct type* t = expr_typecheck(st, e);
    if (!type_equals(t, boolean)) { print_error(test_type, "true", "bool type_equals(t, boolean)"); status = FAILURE; }
    if (global_error_count) { print_error(test_type, "0", "int global_error_count"); status = FAILURE; }
    if (error_status) { print_error(test_type, "0", "int error_status"); status = FAILURE; }
    expr_destroy(&e); type_destroy(&t);
  }
  symbol_table_destroy(&st); type_destroy(&boolean); type_destroy(&integer);
  return status;
}

Status test_expr_typecheck_equality_bad_type_mismatch(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_equality_bad_type_mismatch");
  Status status = SUCCESS;
  struct type* boolean = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);

  int operators[2] = { EXPR_EQ, EXPR_NEQ };
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);

  // add in symbols
  symbol_table_scope_bind(st, "foo", symbol_create(SYMBOL_GLOBAL, type_copy(integer), strdup("foo")));
  symbol_table_scope_bind(st, "bar", symbol_create(SYMBOL_GLOBAL, type_copy(boolean), strdup("bar")));

  for (int i = 0; i < 2; i++) {
      symbol_table_scope_enter(st); 
      struct expr* left = expr_create_name(strdup("foo"));
      struct expr* right = expr_create_name(strdup("bar"));
      struct expr* e = expr_create(operators[i], left, right); 
      expr_resolve(st, e);
      struct type* t = expr_typecheck(st, e);
      if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
      if (error_status != EQUAL) { print_error(test_type, "EQUAL", "int error_status"); status = FAILURE; }
      expr_destroy(&e);
      type_destroy(&t);
      symbol_table_scope_exit(st);
  }
  symbol_table_scope_exit(st);
  symbol_table_destroy(&st);
  type_destroy(&boolean); type_destroy(&integer);
  return status;
}

Status test_expr_typecheck_equality_bad_type_invalid(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_equality_bad_type_invalid");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct type* tfunction = type_create(TYPE_FUNCTION, type_copy(integer), NULL, NULL);
  struct type* tarray = type_create(TYPE_ARRAY, type_copy(integer), NULL, NULL);
  struct type* tvoid = type_create(TYPE_VOID, NULL, NULL, NULL);

  int operators[2] = { EXPR_EQ, EXPR_NEQ };
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);

  // void == void
  for (int i = 0; i < 2; i++) {
      symbol_table_scope_enter(st); 
      // add in symbols
      symbol_table_scope_bind(st, "foo", symbol_create(SYMBOL_LOCAL, type_copy(tvoid), strdup("foo")));
      symbol_table_scope_bind(st, "bar", symbol_create(SYMBOL_LOCAL, type_copy(tvoid), strdup("bar")));
      struct expr* left = expr_create_name(strdup("foo"));
      struct expr* right = expr_create_name(strdup("bar"));
      struct expr* e = expr_create(operators[i], left, right); 
      expr_resolve(st, e);
      struct type* t = expr_typecheck(st, e);
      if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
      if (error_status != EQUAL) { print_error(test_type, "EQUAL", "int error_status"); status = FAILURE; }
      expr_destroy(&e);
      type_destroy(&t);
  }

  // function == function
    for (int i = 0; i < 2; i++) {
      symbol_table_scope_enter(st); 
      // add in symbols
      symbol_table_scope_bind(st, "foo", symbol_create(SYMBOL_LOCAL, type_copy(tfunction), strdup("foo")));
      symbol_table_scope_bind(st, "bar", symbol_create(SYMBOL_LOCAL, type_copy(tfunction), strdup("bar")));
      struct expr* left = expr_create_name(strdup("foo"));
      struct expr* right = expr_create_name(strdup("bar"));
      struct expr* e = expr_create(operators[i], left, right); 
      expr_resolve(st, e);
      struct type* t = expr_typecheck(st, e);
      if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
      if (error_status != EQUAL) { print_error(test_type, "EQUAL", "int error_status"); status = FAILURE; }
      expr_destroy(&e);
      type_destroy(&t);
  }

    for (int i = 0; i < 2; i++) {
      symbol_table_scope_enter(st); 
      // add in symbols
      symbol_table_scope_bind(st, "foo", symbol_create(SYMBOL_LOCAL, type_copy(tarray), strdup("foo")));
      symbol_table_scope_bind(st, "bar", symbol_create(SYMBOL_LOCAL, type_copy(tarray), strdup("bar")));
      struct expr* left = expr_create_name(strdup("foo"));
      struct expr* right = expr_create_name(strdup("bar"));
      struct expr* e = expr_create(operators[i], left, right); 
      expr_resolve(st, e);
      struct type* t = expr_typecheck(st, e);
      if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
      if (error_status != EQUAL) { print_error(test_type, "EQUAL", "int error_status"); status = FAILURE; }
      expr_destroy(&e);
      type_destroy(&t);
  }
  int level = symbol_table_scope_level(st);
  for (int i= 0; i < level; i++) { symbol_table_scope_exit(st); }
  symbol_table_destroy(&st);
  type_destroy(&integer);
  type_destroy(&tfunction); type_destroy(&tarray); type_destroy(&tvoid);
  return status;
}

Status test_expr_typecheck_equality_good(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_equality_good");
  Status status = SUCCESS;
  struct type* boolean = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
  int operators[2] = { EXPR_EQ, EXPR_NEQ };
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);

  // add in symbols
  symbol_table_scope_bind(st, "foo", symbol_create(SYMBOL_GLOBAL, type_copy(boolean), strdup("foo")));
  symbol_table_scope_bind(st, "bar", symbol_create(SYMBOL_GLOBAL, type_copy(boolean), strdup("bar")));

  for (int i = 0; i < 2; i++) {
    struct expr* left = expr_create_name(strdup("foo"));
    struct expr* right = expr_create_name(strdup("bar"));
    struct expr* e = expr_create(operators[i], left, right);
    expr_resolve(st, e);
    struct type* t = expr_typecheck(st, e);
    if (!type_equals(t, boolean)) { print_error(test_type, "true", "bool type_equals(t, boolean)"); status = FAILURE; }
    if (global_error_count) { print_error(test_type, "0", "int global_error_count"); status = FAILURE; }
    if (error_status) { print_error(test_type, "0", "int error_status"); status = FAILURE; }
    type_destroy(&t);
    expr_destroy(&e);
  }
  symbol_table_scope_exit(st);
  symbol_table_destroy(&st);
  type_destroy(&boolean);
  return status;
}

Status test_expr_typecheck_subscript_bad_array(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_subscript_bad_array");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  struct expr* left = expr_create_name(strdup("foo"));
  symbol_table_scope_bind(st, left->name, symbol_create(SYMBOL_GLOBAL, type_copy(integer), strdup(left->name)));
  struct expr* right = expr_create_integer_literal(493);
  struct expr* e = expr_create(EXPR_SUBSCRIPT, left, right); expr_resolve(st, e);
  struct type* t = expr_typecheck(st, e);
  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != SUBSCRIPT) { print_error(test_type, "SUBSCRIPT", "int error_status"); status = FAILURE; }
  expr_destroy(&e); type_destroy(&t);
  symbol_table_destroy(&st); type_destroy(&integer);
  return status;
}

Status test_expr_typecheck_subscript_bad_integer(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_subscript_bad_integer");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct type* array_integer = type_create(TYPE_ARRAY, type_copy(integer), NULL, NULL);
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  struct expr* left = expr_create_name(strdup("foo"));
  symbol_table_scope_bind(st, left->name, symbol_create(SYMBOL_GLOBAL, type_copy(array_integer), strdup(left->name)));
  struct expr* right = expr_create_char_literal('a');
  struct expr* e = expr_create(EXPR_SUBSCRIPT, left, right); expr_resolve(st, e);
  struct type* t = expr_typecheck(st, e);
  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != SUBSCRIPT) { print_error(test_type, "SUBSCRIPT", "int error_status"); status = FAILURE; }
  expr_destroy(&e); type_destroy(&t);
  symbol_table_destroy(&st); type_destroy(&array_integer); type_destroy(&integer);
  return status;
}

Status test_expr_typecheck_subscript_good(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_subscript_good");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct type* array_integer = type_create(TYPE_ARRAY, type_copy(integer), NULL, NULL);

  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  struct expr* left = expr_create_name(strdup("foo"));
  symbol_table_scope_bind(st, left->name, symbol_create(SYMBOL_GLOBAL, type_copy(array_integer), strdup(left->name)));

  struct expr* right = expr_create_integer_literal(0);
  struct expr* e = expr_create(EXPR_SUBSCRIPT, left, right); expr_resolve(st, e);
  struct type* t = expr_typecheck(st, e);
  if (!type_equals(t, integer)) { print_error(test_type, "true", "bool type_equals(t, integer)"); status = FAILURE; }
  if (global_error_count) { print_error(test_type, "0", "int global_error_count"); status = FAILURE; }
  if (error_status) { print_error(test_type, "0", "int error_status"); status = FAILURE; }
  expr_destroy(&e); type_destroy(&t);
  symbol_table_destroy(&st); type_destroy(&array_integer); type_destroy(&integer);
  return status;
}

Status test_expr_typecheck_comma_good(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_comma_good");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct type* boolean = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  struct expr* left = expr_create_boolean_literal(false);
  struct expr* right = expr_create_integer_literal(0);
  struct expr* e = expr_create(EXPR_COMMA, left, right); expr_resolve(st, e);
  struct type* t = expr_typecheck(st, e);
  if (!type_equals(t, boolean)) { print_error(test_type, "true", "bool type_equals(t, boolean)"); status = FAILURE; }
  if (global_error_count) { print_error(test_type, "0", "int global_error_count"); status = FAILURE; }
  if (error_status) { print_error(test_type, "0", "int error_status"); status = FAILURE; }
  expr_destroy(&e); type_destroy(&t);
  symbol_table_destroy(&st); type_destroy(&boolean); type_destroy(&integer);
  return status;
}

Status test_expr_typecheck_assign_bad(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_assign_bad");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct expr* left = expr_create_name(strdup("foo"));
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  symbol_table_scope_bind(st, left->name, symbol_create(SYMBOL_GLOBAL, type_copy(integer), strdup(left->name)));
  struct expr* right = expr_create_boolean_literal(true);
  struct expr* e = expr_create(EXPR_ASSIGN, left, right); expr_resolve(st, e);
  struct type* t = expr_typecheck(st, e);
  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != ASSIGN) { print_error(test_type, "ASSIGN", "int error_status"); status = FAILURE; }
  expr_destroy(&e); type_destroy(&t);
  symbol_table_destroy(&st); type_destroy(&integer);
  return status;
}

Status test_expr_typecheck_assign_good(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_assign_good");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct expr* left = expr_create_name(strdup("foo"));
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  symbol_table_scope_bind(st, left->name, symbol_create(SYMBOL_GLOBAL, type_copy(integer), strdup(left->name)));
  struct expr* right = expr_create_integer_literal(493);
  struct expr* e = expr_create(EXPR_ASSIGN, left, right); expr_resolve(st, e);
  struct type* t = expr_typecheck(st, e);
  if (!type_equals(t, integer)) { print_error(test_type, "true", "bool type_equals(t, integer)"); status = FAILURE; }
  if (global_error_count) { print_error(test_type, "0", "int global_error_count"); status = FAILURE; }
  if (error_status) { print_error(test_type, "0", "int error_status"); status = FAILURE; }
  expr_destroy(&e); type_destroy(&t);
  symbol_table_destroy(&st); type_destroy(&integer);
  return status;
}

Status test_expr_typecheck_assign_bad_lval(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_assign_bad_lval");
  Status status = SUCCESS;
  struct expr* left = expr_create_integer_literal(493);
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  struct expr* right = expr_create_integer_literal(2);
  struct expr* e = expr_create(EXPR_ASSIGN, left, right); expr_resolve(st, e);
  struct type* t = expr_typecheck(st, e);
  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != LVAL) { print_error(test_type, "LVAL", "int error_status"); status = FAILURE; }
  expr_destroy(&e); type_destroy(&t);
  symbol_table_destroy(&st);
  return status;
}
Status test_expr_typecheck_assign_good_subscript(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_assign_good_subscript");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct type* iarray = type_create(TYPE_ARRAY, type_copy(integer), NULL, NULL);
  struct expr* arr = expr_create_name(strdup("foo"));
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  symbol_table_scope_bind(st, arr->name, symbol_create(SYMBOL_GLOBAL, type_copy(iarray), strdup(arr->name)));
  struct expr* left = expr_create(EXPR_SUBSCRIPT, arr, expr_create_integer_literal(0));
  struct expr* right = expr_create_integer_literal(493);
  struct expr* e = expr_create(EXPR_ASSIGN, left, right); expr_resolve(st, e);
  struct type* t = expr_typecheck(st, e);
  if (!type_equals(t, integer)) { print_error(test_type, "true", "bool type_equals(t, integer)"); status = FAILURE; }
  if (global_error_count) { print_error(test_type, "0", "int global_error_count"); status = FAILURE; }
  if (error_status) { print_error(test_type, "0", "int error_status"); status = FAILURE; }
  expr_destroy(&e); type_destroy(&t);
  symbol_table_destroy(&st); type_destroy(&integer); type_destroy(&iarray);
  return status;
}

Status test_expr_typecheck_fcall_bad_function(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_fcall_bad_function");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct type* function_integer = type_create(TYPE_FUNCTION, type_copy(integer), NULL, NULL);

  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  struct expr* left = expr_create_name(strdup("foo"));
  symbol_table_scope_bind(st, left->name, symbol_create(SYMBOL_GLOBAL, type_copy(integer), strdup(left->name)));

  struct expr* right = expr_create_integer_literal(493);
  struct expr* e = expr_create(EXPR_FCALL, left, right); expr_resolve(st, e);
  struct type* t = expr_typecheck(st, e);
  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != FCALL) { print_error(test_type, "FCALL", "int error_status"); status = FAILURE; }
  expr_destroy(&e); type_destroy(&t);
  symbol_table_destroy(&st); type_destroy(&function_integer); type_destroy(&integer);
  return status;
}

Status test_expr_typecheck_fcall_bad_param(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_fcall_bad_param");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct type* boolean = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
  struct param_list* p = param_list_create(strdup("x"), type_copy(boolean), NULL);
  struct type* function_integer = type_create(TYPE_FUNCTION, type_copy(integer), p, NULL);

  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  struct expr* left = expr_create_name(strdup("foo"));
  symbol_table_scope_bind(st, left->name, symbol_create(SYMBOL_GLOBAL, type_copy(function_integer), strdup(left->name)));

  struct expr* e = expr_create(EXPR_FCALL, left, expr_create_integer_literal(493)); expr_resolve(st, e);
  struct type* t = expr_typecheck(st, e);
  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != PARAM) { print_error(test_type, "PARAM", "int error_status"); status = FAILURE; }
  expr_destroy(&e); type_destroy(&t);
  symbol_table_destroy(&st); type_destroy(&function_integer); type_destroy(&integer); type_destroy(&boolean);
  return status;
}

Status test_expr_typecheck_fcall_good_no_param(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_fcall_good_no_param");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct type* function_integer = type_create(TYPE_FUNCTION, type_copy(integer), NULL, NULL);
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  struct expr* left = expr_create_name(strdup("foo"));
  symbol_table_scope_bind(st, left->name, symbol_create(SYMBOL_GLOBAL, type_copy(function_integer), strdup(left->name)));
  struct expr* e = expr_create(EXPR_FCALL, left, NULL); expr_resolve(st, e);
  struct type* t = expr_typecheck(st, e);
  if (!type_equals(t, integer)) { print_error(test_type, "true", "bool type_equals(t, integer)"); status = FAILURE; }
  if (global_error_count) { print_error(test_type, "0", "int global_error_count"); status = FAILURE; }
  if (error_status) { print_error(test_type, "0", "int error_status"); status = FAILURE; }
  expr_destroy(&e); type_destroy(&t);
  symbol_table_destroy(&st); type_destroy(&function_integer); type_destroy(&integer);
  return status;
}

Status test_expr_typecheck_fcall_good_one_param(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_fcall_good_one_param");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct type* boolean = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
  struct param_list* p = param_list_create(strdup("x"), type_copy(boolean), NULL);
  struct type* function_integer = type_create(TYPE_FUNCTION, type_copy(integer), p, NULL);

  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  struct expr* left = expr_create_name(strdup("foo"));
  symbol_table_scope_bind(st, left->name, symbol_create(SYMBOL_GLOBAL, type_copy(function_integer), strdup(left->name)));

  struct expr* right = expr_create_boolean_literal(false);
  struct expr* e = expr_create(EXPR_FCALL, left, right); expr_resolve(st, e);
  struct type* t = expr_typecheck(st, e);
  if (!type_equals(t, integer)) { print_error(test_type, "true", "bool type_equals(t, integer)"); status = FAILURE; }
  if (global_error_count) { print_error(test_type, "0", "int global_error_count"); status = FAILURE; }
  if (error_status) { print_error(test_type, "0", "int error_status"); status = FAILURE; }
  expr_destroy(&e); type_destroy(&t);
  symbol_table_destroy(&st); type_destroy(&function_integer); type_destroy(&integer); type_destroy(&boolean);
  return status;
}

Status test_expr_typecheck_fcall_good_many_param(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_fcall_good_many_param");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct type* boolean = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
  struct param_list* p = param_list_create(strdup("x"), type_copy(boolean), param_list_create(strdup("y"), type_copy(integer), NULL));
  struct type* function_integer = type_create(TYPE_FUNCTION, type_copy(integer), p, NULL);

  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  struct expr* left = expr_create_name(strdup("foo"));
  symbol_table_scope_bind(st, left->name, symbol_create(SYMBOL_GLOBAL, type_copy(function_integer), strdup(left->name)));

  struct expr* right_left = expr_create_boolean_literal(false);
  struct expr* right_right = expr_create_integer_literal(493);
  struct expr* e = expr_create(EXPR_FCALL, left, expr_create(EXPR_COMMA, right_left, right_right)); expr_resolve(st, e);
  struct type* t = expr_typecheck(st, e);

  if (!type_equals(t, integer)) { print_error(test_type, "true", "bool type_equals(t, integer)"); status = FAILURE; }
  if (global_error_count) { print_error(test_type, "0", "int global_error_count"); status = FAILURE; }
  if (error_status) { print_error(test_type, "0", "int error_status"); status = FAILURE; }
  expr_destroy(&e); type_destroy(&t);
  symbol_table_destroy(&st); type_destroy(&function_integer); type_destroy(&integer); type_destroy(&boolean);
  return status;
}

Status test_expr_typecheck_init_good_one(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_init_good_one");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct type* array_integer = type_create(TYPE_ARRAY, type_copy(integer), NULL, NULL);

  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  struct expr* left = expr_create_integer_literal(493);

  struct expr* e = expr_create(EXPR_INIT, left, NULL); expr_resolve(st, e);
  struct type* t = expr_typecheck(st, e);
  if (!type_equals(t, array_integer)) { print_error(test_type, "true", "bool type_equals(t, array_integer)"); status = FAILURE; }
  if (global_error_count) { print_error(test_type, "0", "int global_error_count"); status = FAILURE; }
  if (error_status) { print_error(test_type, "0", "int error_status"); status = FAILURE; }
  expr_destroy(&e); type_destroy(&t);
  symbol_table_destroy(&st); type_destroy(&array_integer); type_destroy(&integer);
  return status;
}

Status test_expr_typecheck_init_bad(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_init_bad");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct type* array_integer = type_create(TYPE_ARRAY, type_copy(integer), NULL, NULL);

  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  struct expr* right_left = expr_create_boolean_literal(true);
  struct expr* right_right = expr_create_integer_literal(493);

  struct expr* e = expr_create(EXPR_INIT, expr_create(EXPR_COMMA, right_left, right_right), NULL); expr_resolve(st, e);
  struct type* t = expr_typecheck(st, e);
  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != INIT) { print_error(test_type, "INIT", "int error_status"); status = FAILURE; }
  expr_destroy(&e); type_destroy(&t);
  symbol_table_destroy(&st); type_destroy(&array_integer); type_destroy(&integer);
  return status;
}

Status test_expr_typecheck_init_good_many(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_init_good_many");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct type* array_integer = type_create(TYPE_ARRAY, type_copy(integer), NULL, NULL);

  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  struct expr* right_left = expr_create_integer_literal(493);
  struct expr* right_right = expr_create_integer_literal(-493);

  struct expr* e = expr_create(EXPR_INIT, expr_create(EXPR_COMMA, right_left, right_right), NULL); expr_resolve(st, e);
  struct type* t = expr_typecheck(st, e);
  if (!type_equals(t, array_integer)) { print_error(test_type, "true", "bool type_equals(t, array_integer)"); status = FAILURE; }
  if (global_error_count) { print_error(test_type, "0", "int global_error_count"); status = FAILURE; }
  if (error_status) { print_error(test_type, "0", "int error_status"); status = FAILURE; }
  expr_destroy(&e); type_destroy(&t);
  symbol_table_destroy(&st); type_destroy(&array_integer); type_destroy(&integer);
  return status;
}

Status test_expr_typecheck_init_good_multidim(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_init_good_multidim");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct type* array_integer = type_create(TYPE_ARRAY, type_copy(integer), NULL, NULL);
  struct type* array_array_integer = type_create(TYPE_ARRAY, type_copy(array_integer), NULL, NULL);

  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  struct expr* right_left = expr_create_integer_literal(493);
  struct expr* right_right = expr_create_integer_literal(-493);

  struct expr* body = expr_create(EXPR_INIT, expr_create(EXPR_COMMA, right_left, right_right), NULL);
  struct expr* e = expr_create(EXPR_INIT, body, NULL);
  expr_resolve(st, e); struct type* t = expr_typecheck(st, e);
  if (!type_equals(t, array_array_integer)) { print_error(test_type, "true", "bool type_equals(t, array_array_integer)"); status = FAILURE; }
  if (global_error_count) { print_error(test_type, "0", "int global_error_count"); status = FAILURE; }
  if (error_status) { print_error(test_type, "0", "int error_status"); status = FAILURE; }
  expr_destroy(&e); type_destroy(&t);
  symbol_table_destroy(&st); type_destroy(&array_array_integer); type_destroy(&array_integer); type_destroy(&integer);
  return status;
}

Status test_decl_typecheck_value_type_mismatch(void) {
  strcpy(test_type, "Testing: test_decl_typecheck_value_type_mismatch");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct decl* d = decl_create(strdup("foo"), type_copy(integer), expr_create_string_literal("hello"), NULL, NULL);
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st); symbol_table_scope_enter(st);
  decl_resolve(st, d);
  decl_typecheck(st, d);
  if (!d) { print_error(test_type, "NOT NULL", "struct decl* d"); return FAILURE; }
  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != SYM_TYPE) { print_error(test_type, "SYM_TYPE", "int error_status"); status = FAILURE; }
  symbol_table_destroy(&st); decl_destroy(&d); type_destroy(&integer);
  return status;
}

Status test_decl_typecheck_function_ret_type_integer(void) {
  strcpy(test_type, "Testing: test_decl_typecheck_function_ret_type_integer");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct stmt* function_body = stmt_create(STMT_RETURN, NULL, NULL, expr_create_integer_literal(0), NULL, NULL, NULL, NULL);
  struct decl* d = decl_create(strdup("foo"), type_create(TYPE_FUNCTION, type_copy(integer), NULL, NULL), NULL, function_body, NULL);
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  decl_resolve(st, d);
  error_status = decl_typecheck(st, d);
  if (global_error_count) { print_error(test_type, "0", "int global_error_count"); status = FAILURE; }
  symbol_table_destroy(&st); decl_destroy(&d); type_destroy(&integer);
  return status;
}

Status test_decl_typecheck_function_ret_type_null(void) {
  strcpy(test_type, "Testing: test_decl_typecheck_function_ret_type_null");
  Status status = SUCCESS;
  struct type* tvoid = type_create(TYPE_VOID, NULL, NULL, NULL);
  struct stmt* function_body = stmt_create(STMT_RETURN, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  struct decl* d = decl_create(strdup("foo"), type_create(TYPE_FUNCTION, type_copy(tvoid), NULL, NULL), NULL, function_body, NULL);
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  decl_resolve(st, d);
  error_status = decl_typecheck(st, d);
  if (global_error_count) { print_error(test_type, "0", "int global_error_count"); status = FAILURE; }
  symbol_table_destroy(&st); decl_destroy(&d); type_destroy(&tvoid);
  return status;
}

Status test_decl_typecheck_function_ret_type_mismatch(void) {
  strcpy(test_type, "Testing: test_decl_typecheck_function_ret_type_mismatch");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct stmt* function_body = stmt_create(STMT_RETURN, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  struct decl* d = decl_create(strdup("foo"), type_create(TYPE_FUNCTION, type_copy(integer), NULL, NULL), NULL, function_body, NULL);
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  decl_resolve(st, d);
  error_status = decl_typecheck(st, d);

  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != SYM_TYPE) { print_error(test_type, "SYM_TYPE", "int error_status"); status = FAILURE; }
  symbol_table_destroy(&st); decl_destroy(&d); type_destroy(&integer);
  return status;
}

Status test_decl_typecheck_array_null_size(void) {
  strcpy(test_type, "Testing: test_decl_typecheck_array_null_size");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct decl* d = decl_create(strdup("foo"), type_create(TYPE_ARRAY, type_copy(integer), NULL, NULL), NULL,NULL, NULL);
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  decl_resolve(st, d);
  error_status = decl_typecheck(st, d);

  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != DECL_NULL) { print_error(test_type, "DECL_NULL", "int error_status"); status = FAILURE; }
  symbol_table_destroy(&st); decl_destroy(&d); type_destroy(&integer);
  return status;
}

Status test_decl_typecheck_array_null_size_nonnull_body(void) {
  strcpy(test_type, "Testing: test_decl_typecheck_array_null_size_nonnull_body");
  Status status = SUCCESS;
  struct expr* e = expr_create(EXPR_INIT, expr_create_integer_literal(493), NULL);
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct decl* d = decl_create(strdup("foo"), type_create(TYPE_ARRAY, type_copy(integer), NULL, NULL), e,NULL, NULL);
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st); symbol_table_scope_enter(st);
  decl_resolve(st, d);
  error_status = decl_typecheck(st, d);

  if (global_error_count) { print_error(test_type, "0", "int global_error_count"); status = FAILURE; }
  if (error_status == DECL_NULL) { print_error(test_type, "NO_ERROR", "int error_status"); status = FAILURE; }
  symbol_table_destroy(&st); decl_destroy(&d); type_destroy(&integer);
  return status;
}

Status test_decl_typecheck_array_nint_size(void) {
  strcpy(test_type, "Testing: test_decl_typecheck_array_nint_size");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct expr* size = expr_create_char_literal('z');
  struct decl* d = decl_create(strdup("foo"), type_create(TYPE_ARRAY, type_copy(integer), NULL, size), NULL,NULL, NULL);
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st); symbol_table_scope_enter(st);
  decl_resolve(st, d);
  error_status = decl_typecheck(st, d);

  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != DECL_NINT) { print_error(test_type, "DECL_NINT", "int error_status"); status = FAILURE; }
  symbol_table_destroy(&st); decl_destroy(&d); type_destroy(&integer);
  return status;
}

Status test_stmt_typecheck_print_good(void) {
  strcpy(test_type, "Testing: test_stmt_typecheck_print_good");
  Status status = SUCCESS;

  // literals
  struct stmt* stmts[4] =  { stmt_create(STMT_PRINT, NULL, NULL, expr_create_integer_literal(493), NULL, NULL, NULL, NULL),
                             stmt_create(STMT_PRINT, NULL, NULL, expr_create_boolean_literal(true), NULL, NULL, NULL, NULL),
                             stmt_create(STMT_PRINT, NULL, NULL, expr_create_char_literal('q'), NULL, NULL, NULL, NULL),
                             stmt_create(STMT_PRINT, NULL, NULL, expr_create_string_literal("duck"), NULL, NULL, NULL, NULL),
                           };
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  for (int i = 0; i < 4; i++) {
    error_status = stmt_resolve(st, stmts[i]);
    error_status = stmt_typecheck(st, stmts[i], NULL);
    if (global_error_count) { print_error(test_type, "0", "int global_error_count"); status = FAILURE; }
    if (error_status) { print_error(test_type, "0", "int error_status"); status = FAILURE; }
    error_status = 0; global_error_count = 0;
    stmt_destroy(&stmts[i]);
  }
  symbol_table_destroy(&st);
  return status;
}

Status test_stmt_typecheck_print_bad(void) {
  strcpy(test_type, "Testing: test_stmt_typecheck_print_bad");
  Status status = SUCCESS;

  // non-literals (array, function, void): as names
  struct stmt* stmts[3] = { stmt_create(STMT_PRINT, NULL, NULL, expr_create_name(strdup("foo")), NULL, NULL, NULL, NULL),
                            stmt_create(STMT_PRINT, NULL, NULL, expr_create_name(strdup("bar")), NULL, NULL, NULL, NULL),
                            stmt_create(STMT_PRINT, NULL, NULL, expr_create_name(strdup("baz")), NULL, NULL, NULL, NULL)
                          };
  
  // associate names to symbols of invalid types
  struct type* tvoid = type_create(TYPE_VOID, NULL, NULL, NULL);
  struct type* tarray = type_create(TYPE_ARRAY, NULL, NULL, NULL);
  struct type* tfunction = type_create(TYPE_FUNCTION, NULL, NULL, NULL);
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  symbol_table_scope_bind(st, "foo", symbol_create(SYMBOL_GLOBAL, type_copy(tvoid), strdup("foo")));
  symbol_table_scope_bind(st, "bar", symbol_create(SYMBOL_GLOBAL, type_copy(tarray), strdup("bar")));
  symbol_table_scope_bind(st, "baz", symbol_create(SYMBOL_GLOBAL, type_copy(tfunction), strdup("baz")));
  for (int i = 0; i < 3; i++) {
    error_status = stmt_resolve(st, stmts[i]);
    error_status = stmt_typecheck(st, stmts[i], NULL);
    if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
    if (error_status != STMT_TYPE) { print_error(test_type, "STMT_TYPE", "int error_status"); status = FAILURE; }
    error_status = 0; global_error_count = 0;
    stmt_destroy(&stmts[i]);
  }
  symbol_table_destroy(&st);
  type_destroy(&tvoid); type_destroy(&tarray); type_destroy(&tfunction);
  return status;
}

Status test_stmt_typecheck_expr_good(void) {
  strcpy(test_type, "Testing: test_stmt_typecheck_expr_good");
  Status status = SUCCESS;
  const int stmts[3] = { STMT_FOR, STMT_WHILE, STMT_IF_ELSE };
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  for (int i = 0; i < 1; i++) {
    struct stmt* s = stmt_create(stmts[i], NULL, NULL, expr_create_boolean_literal(false), NULL, NULL, NULL, NULL);
    error_status = stmt_resolve(st, s);
    error_status = stmt_typecheck(st, s, NULL);
    if (global_error_count) { print_error(test_type, "0", "int global_error_count"); status = FAILURE; }
    if (error_status) { print_error(test_type, "0", "int error_status"); status = FAILURE; }
    stmt_destroy(&s);
  }
  symbol_table_destroy(&st);
  return status;
}

Status test_stmt_typecheck_expr_bad(void) {
  strcpy(test_type, "Testing: test_stmt_typecheck_expr_bad");
  Status status = SUCCESS;
  const int stmts[3] = { STMT_FOR, STMT_WHILE, STMT_IF_ELSE };
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  for (int i = 0; i < 3; i++) {
    struct stmt* s = stmt_create(stmts[i], NULL, NULL, expr_create_integer_literal(493), NULL, NULL, NULL, NULL);
    error_status = stmt_resolve(st, s);
    error_status = stmt_typecheck(st, s, NULL);
    if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
    if (error_status != STMT_BOOLEAN) { print_error(test_type, "STMT_BOOLEAN", "int error_status"); status = FAILURE; }
    stmt_destroy(&s);
    global_error_count = 0; error_status = 0;
  }
  symbol_table_destroy(&st);
  return status;
}

Status test_stmt_typecheck_expr_bad_comma(void) {
  strcpy(test_type, "Testing: test_stmt_typecheck_expr_bad_comma");
  Status status = SUCCESS;
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  struct expr* e = expr_create(EXPR_COMMA, expr_create_boolean_literal(true), expr_create_integer_literal(493));
  struct stmt* s = stmt_create(STMT_IF_ELSE, NULL, NULL, e, NULL, NULL, NULL, NULL);
  error_status = stmt_resolve(st, s);
  error_status = stmt_typecheck(st, s, NULL);
  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != STMT_BOOLEAN) { print_error(test_type, "STMT_BOOLEAN", "int error_status"); status = FAILURE; }
  stmt_destroy(&s);
  global_error_count = 0; error_status = 0;
  symbol_table_destroy(&st);
  return status;
}

Status test_stmt_typecheck_expr_bad_assign(void) {
  strcpy(test_type, "Testing: test_stmt_typecheck_expr_bad_assign");
  Status status = SUCCESS;
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  struct expr* e = expr_create(EXPR_ASSIGN, expr_create_name(strdup("x")), expr_create_boolean_literal(true));
  struct stmt* s = stmt_create(STMT_IF_ELSE, NULL, NULL, e, NULL, NULL, NULL, NULL);
  struct symbol* sym = symbol_create(SYMBOL_GLOBAL, type_create(TYPE_BOOLEAN, NULL, NULL, NULL), strdup("x"));
  symbol_table_scope_bind(st, "x", sym);
  error_status = stmt_resolve(st, s);
  error_status = stmt_typecheck(st, s, NULL);
  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != STMT_BOOLEAN) { print_error(test_type, "STMT_BOOLEAN", "int error_status"); status = FAILURE; }
  stmt_destroy(&s);
  global_error_count = 0; error_status = 0;
  symbol_table_destroy(&st);
  return status;
}

Status test_expr_typecheck_assign_bad_string(void) {
  strcpy(test_type, "Testing: test_expr_typecheck_assign_bad_string");
  Status status = SUCCESS;
  struct type* string = type_create(TYPE_STRING, NULL, NULL, NULL);
  struct expr* left = expr_create_name(strdup("x"));
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  symbol_table_scope_bind(st, left->name, symbol_create(SYMBOL_GLOBAL, type_copy(string), strdup(left->name)));

  struct expr* right = expr_create_string_literal("duck");
  struct expr* e = expr_create(EXPR_ASSIGN, left, right);
  expr_resolve(st, e);
  struct type* t = expr_typecheck(st, e);

  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != IMMUTABLE) { print_error(test_type, "IMMUTABLE", "int error_status"); status = FAILURE; }

  expr_destroy(&e);
  type_destroy(&t);
  type_destroy(&string);
  symbol_table_destroy(&st);
  return status;
}
