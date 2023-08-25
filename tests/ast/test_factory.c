#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../src/decl.h"
#include "../../src/expr.h"
#include "../../src/param_list.h"
#include "../../src/stmt.h"
#include "../../src/symbol.h"
#include "../../src/type.h"
#define MAX_BUFFER 256

typedef enum { FAILURE = 0, SUCCESS = 1 } Status;
char test_type[MAX_BUFFER];

void print_error(char* test, char* expect, char* value);
Status file_error(char* test_type, char* filename); // error messsage when opening file
Status test_decl_create_null(void);
Status test_decl_create_name(void);
Status test_param_list_create_null(void);
Status test_param_list_create_name(void);
Status test_stmt_create_kind(void);
Status test_type_create_kind(void);
Status test_expr_create_kind(void);
Status test_expr_create_name(void);
Status test_expr_create_int(void);
Status test_expr_create_bool(void);
Status test_expr_create_char(void);
Status test_expr_create_str(void);
Status test_expr_create_unary(void);
Status test_expr_create_binary_2_op(void);
Status test_expr_create_binary_3_op(void);
Status test_stmt_create_print(void);
Status test_decl_create_atomic_uninit(void);
Status test_decl_create_atomic_init(void);
Status test_decl_create_composite_array(void);
Status test_decl_create_composite_function(void);
Status test_decl_create_program(void);

int main(void) {
  Status (*tests[])(void) = {
    test_decl_create_null,
    test_decl_create_name,
    test_param_list_create_null,
    test_param_list_create_name,
    test_stmt_create_kind,
    test_type_create_kind,
    test_expr_create_kind,
    test_expr_create_name,
    test_expr_create_int,
    test_expr_create_bool,
    test_expr_create_char,
    test_expr_create_str,
    test_expr_create_unary,
    test_expr_create_binary_2_op,
    test_expr_create_binary_3_op,
    test_stmt_create_print,
    test_decl_create_atomic_uninit,
    test_decl_create_atomic_init,
    test_decl_create_composite_array,
    test_decl_create_composite_function,
    test_decl_create_program
  };
  int n_tests = sizeof(tests)/sizeof(tests[0]);
  int n_pass = 0;

  printf("Running %d tests...\n", n_tests);
  for (int i = 0; i < n_tests; i++) { if (tests[i]()) { n_pass++; }}

  printf("Passed: %2d/%d\n", n_pass, n_tests);
  printf("Failed: %2d/%d\n", (n_tests - n_pass), n_tests);
  return 0;
}

Status file_error(char* test_type, char* filename) {
  printf("Failed to open %s for %s.\n Test failure.\n", filename, test_type);
  return FAILURE;
}

// pointers so use strings for expected types
void print_error(char* test, char* expect, char* value) { printf("Error:\t[%s]:\n\tExpected a value of %s for value: [%s]\n", test, expect, value); }

Status test_decl_create_null(void) {
  strcpy(test_type, "Testing: decl_create, all NULL");
  Status overall_status = SUCCESS;
  struct decl* d = decl_create(NULL, NULL, NULL, NULL, NULL);
  if (!d) { print_error(test_type, "NOT NULL", "decl d"); overall_status = FAILURE; }
  else {
    if (d->name) { print_error(test_type, "NULL", "char* d->name"); overall_status = FAILURE; }
    if (d->type) { print_error(test_type, "NULL", "type* d->type"); overall_status = FAILURE; }
    if (d->value) { print_error(test_type, "NULL", "expr* d->value"); overall_status = FAILURE; }
    if (d->code) { print_error(test_type, "NULL", "stmt* d->code"); overall_status = FAILURE; }
    if (d->next) { print_error(test_type, "NULL", "decl* d->next"); overall_status = FAILURE; }
  }
  decl_destroy(&d);
  return overall_status;
}

Status test_decl_create_name(void) {
  strcpy(test_type, "Testing: decl_create, non-NULL name");
  Status overall_status = SUCCESS;
  struct decl* d = decl_create(strdup("foo"), NULL, NULL, NULL, NULL);
  if (!d) { print_error(test_type, "NOT NULL", "decl d"); overall_status = FAILURE; }
  else {
    if (strcmp(d->name, "foo")) { print_error(test_type, "foo", "char* d->name"); overall_status = FAILURE; }
    if (d->type) { print_error(test_type, "NULL", "type* d->type"); overall_status = FAILURE; }
    if (d->value) { print_error(test_type, "NULL", "expr* d->value"); overall_status = FAILURE; }
    if (d->code) { print_error(test_type, "NULL", "stmt* d->code"); overall_status = FAILURE; }
    if (d->next) { print_error(test_type, "NULL", "decl* d->next"); overall_status = FAILURE; }
  }
  decl_destroy(&d);
  return overall_status;
}

Status test_param_list_create_null(void) {
  strcpy(test_type, "Testing: param_list_create, all NULL");
  Status overall_status = SUCCESS;
  struct param_list* p = param_list_create(NULL, NULL, NULL);
  if (!p) { print_error(test_type, "NOT NULL", "param_list p"); overall_status = FAILURE; }
  else {
    if (p->name) { print_error(test_type, "NULL", "char* p->name"); overall_status = FAILURE; }
    if (p->type) { print_error(test_type, "NULL", "type* p->type"); overall_status = FAILURE; }
    if (p->next) { print_error(test_type, "NULL", "param_list* p->next"); overall_status = FAILURE; }
  }
  param_list_destroy(&p);
  return overall_status;
}

Status test_param_list_create_name(void) {
  strcpy(test_type, "Testing: param_list_create_name, non-NULL name");
  Status overall_status = SUCCESS;
  struct param_list* p = param_list_create(strdup("foo"), NULL, NULL);
  if (!p) { print_error(test_type, "NOT NULL", "param_list p"); overall_status = FAILURE; }
  else {
    if (strcmp(p->name, "foo")) { print_error(test_type, "foo", "char* p->name"); overall_status = FAILURE; }
    if (p->type) { print_error(test_type, "NULL", "type* p->type"); overall_status = FAILURE; }
    if (p->next) { print_error(test_type, "NULL", "param_list* p->next"); overall_status = FAILURE; }
  }
  param_list_destroy(&p);
  return overall_status;
}

Status test_stmt_create_kind(void) {
  strcpy(test_type, "Testing: test_stmt_create_kind");
  Status overall_status = SUCCESS;
  char kind_expect[3]; char kind_actual[3];
  for (stmt_t kind = STMT_DECL; kind <= STMT_BLOCK; kind++) {
    struct stmt* s = stmt_create(kind, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    if (!s) { print_error(test_type, "NOT NULL", "stmt s"); overall_status = FAILURE; }
    else {
      if (s->kind != kind) {
        sprintf(kind_expect, "%d", kind); sprintf(kind_actual, "%d", s->kind);
        print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
      }
      if (s->decl) { print_error(test_type, "NULL", "decl* s->decl"); overall_status = FAILURE; }
      if (s->init_expr) { print_error(test_type, "NULL", "expr* s->init_expr"); overall_status = FAILURE; }
      if (s->expr) { print_error(test_type, "NULL", "expr* s->expr"); overall_status = FAILURE; }
      if (s->next_expr) { print_error(test_type, "NULL", "expr* s->next_expr"); overall_status = FAILURE; }
      if (s->body) { print_error(test_type, "NULL", "stmt* s->body"); overall_status = FAILURE; }
      if (s->else_body) { print_error(test_type, "NULL", "stmt* s->else_body"); overall_status = FAILURE; }
      if (s->next) { print_error(test_type, "NULL", "stmt* s->next"); overall_status = FAILURE; }
    }
    stmt_destroy(&s);
  }
  return overall_status;
}


Status test_type_create_kind(void) {
  strcpy(test_type, "Testing: test_type_create_kind");
  Status overall_status = SUCCESS;
  char kind_expect[3]; char kind_actual[3];
  for (type_t kind = TYPE_VOID; kind <= TYPE_FUNCTION; kind++) {
    struct type* t = type_create(kind, NULL, NULL, NULL);
    if (!t) { print_error(test_type, "NOT NULL", "type t"); overall_status = FAILURE; }
    else {
      if (t->kind != kind) {
        sprintf(kind_expect, "%d", kind); sprintf(kind_actual, "%d", t->kind);
        print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
      }
      if (t->subtype) { print_error(test_type, "NULL", "type* t->subtype"); overall_status = FAILURE; }
      if (t->params) { print_error(test_type, "NULL", "param_list* params"); overall_status = FAILURE; }
    }
    type_destroy(&t);
  }
  return overall_status;
}


Status test_expr_create_kind(void) {
  strcpy(test_type, "Testing: test_expr_create_kind");
  Status overall_status = SUCCESS;
  char kind_expect[3]; char kind_actual[3];
  for (expr_t kind = EXPR_INC; kind <= EXPR_FCALL; kind++) {
    struct expr* e = expr_create(kind, NULL, NULL);
    if (!e) { print_error(test_type, "NOT NULL", "expr e"); overall_status = FAILURE; }
    else {
      if (e->kind != kind) {
        sprintf(kind_expect, "%d", kind); sprintf(kind_actual, "%d", e->kind);
        print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
      }
      if (e->left) { print_error(test_type, "NULL", "expr* e->left"); overall_status = FAILURE; }
      if (e->right) { print_error(test_type, "NULL", "expr* e->right"); overall_status = FAILURE; }
      if (e->name) { print_error(test_type, "NULL", "char* e->name"); overall_status = FAILURE; }
      if (e->literal_value) {
        sprintf(kind_expect, "%d", 0); sprintf(kind_actual, "%ld", e->literal_value);
        print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
      }
      if (e->string_literal) { print_error(test_type, "NULL", "char* e->string_literal"); overall_status = FAILURE; }
    }
    expr_destroy(&e);
  }
  return overall_status;
}

Status test_expr_create_name(void) {
  strcpy(test_type, "Testing: test_expr_create_name");
  Status overall_status = SUCCESS;
  char expect[3], actual[3];
  struct expr* e = expr_create_name(strdup("foo"));
  if (!e) { print_error(test_type, "NOT NULL", "expr e"); overall_status = FAILURE; }
  else {
    if (e->kind != EXPR_NAME) {
      sprintf(expect, "%d", EXPR_NAME); sprintf(actual, "%d", e->kind);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->left) { print_error(test_type, "NULL", "expr* e->left"); overall_status = FAILURE; }
    if (e->right) { print_error(test_type, "NULL", "expr* e->right"); overall_status = FAILURE; }
    if (strcmp("foo", e->name)) { print_error(test_type, "foo", "char* e->name"); overall_status = FAILURE; }
    if (e->literal_value) {
      sprintf(expect, "%d", 0); sprintf(actual, "%ld", e->literal_value);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->string_literal) { print_error(test_type, "NULL", "char* e->string_literal"); overall_status = FAILURE; }
  }
  expr_destroy(&e);
  return overall_status;
}

Status test_expr_create_int(void) {
  strcpy(test_type, "Testing: test_expr_create_int");
  Status overall_status = SUCCESS;
  char expect[5], actual[5];
  struct expr* e = expr_create_integer_literal(-493);
  if (!e) { print_error(test_type, "NOT NULL", "expr e"); overall_status = FAILURE; }
  else {
    if (e->kind != EXPR_INT) {
      sprintf(expect, "%d", EXPR_INT); sprintf(actual, "%d", e->kind);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->left) { print_error(test_type, "NULL", "expr* e->left"); overall_status = FAILURE; }
    if (e->right) { print_error(test_type, "NULL", "expr* e->right"); overall_status = FAILURE; }
    if (e->name) { print_error(test_type, "NULL", "char* e->name"); overall_status = FAILURE; }
    if (e->literal_value != -493) {
      sprintf(expect, "%d", -493); sprintf(actual, "%ld", e->literal_value);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->string_literal) { print_error(test_type, "NULL", "char* e->string_literal"); overall_status = FAILURE; }
  }
  expr_destroy(&e);
  return overall_status;
}

Status test_expr_create_bool(void) {
  strcpy(test_type, "Testing: test_expr_create_bool");
  Status overall_status = SUCCESS;
  char expect[3], actual[3];
  struct expr* e = expr_create_boolean_literal(1);
  if (!e) { print_error(test_type, "NOT NULL", "expr e"); overall_status = FAILURE; }
  else {
    if (e->kind != EXPR_BOOL) {
      sprintf(expect, "%d", EXPR_BOOL); sprintf(actual, "%d", e->kind);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->left) { print_error(test_type, "NULL", "expr* e->left"); overall_status = FAILURE; }
    if (e->right) { print_error(test_type, "NULL", "expr* e->right"); overall_status = FAILURE; }
    if (e->name) { print_error(test_type, "NULL", "char* e->name"); overall_status = FAILURE; }
    if (e->literal_value != 1) {
      sprintf(expect, "%d", 1); sprintf(actual, "%ld", e->literal_value);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->string_literal) { print_error(test_type, "NULL", "char* e->string_literal"); overall_status = FAILURE; }
  }
  expr_destroy(&e);
  return overall_status;
}

Status test_expr_create_char(void) {
  strcpy(test_type, "Testing: test_expr_create_char");
  Status overall_status = SUCCESS;
  char expect[3], actual[3]; char val = 'A';
  struct expr* e = expr_create_char_literal(val);
  if (!e) { print_error(test_type, "NOT NULL", "expr e"); overall_status = FAILURE; }
  else {
    if (e->kind != EXPR_CH) {
      sprintf(expect, "%d", EXPR_CH); sprintf(actual, "%d", e->kind);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->left) { print_error(test_type, "NULL", "expr* e->left"); overall_status = FAILURE; }
    if (e->right) { print_error(test_type, "NULL", "expr* e->right"); overall_status = FAILURE; }
    if (e->name) { print_error(test_type, "NULL", "char* e->name"); overall_status = FAILURE; }
    if (e->literal_value != 'A') {
      sprintf(expect, "%c", val); sprintf(actual, "%c", (int)e->literal_value);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->string_literal) { print_error(test_type, "NULL", "char* e->string_literal"); overall_status = FAILURE; }
  }
  expr_destroy(&e);
  return overall_status;
}

Status test_expr_create_str(void) {
  strcpy(test_type, "Testing: test_expr_create_str");
  Status overall_status = SUCCESS;
  char expect[3], actual[3];
  struct expr* e = expr_create_string_literal("foo");
  if (!e) { print_error(test_type, "NOT NULL", "expr e"); overall_status = FAILURE; }
  else {
    if (e->kind != EXPR_STR) {
      sprintf(expect, "%d", EXPR_STR); sprintf(actual, "%d", e->kind);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->left) { print_error(test_type, "NULL", "expr* e->left"); overall_status = FAILURE; }
    if (e->right) { print_error(test_type, "NULL", "expr* e->right"); overall_status = FAILURE; }
    if (e->name) { print_error(test_type, "NULL", "char* e->name"); overall_status = FAILURE; }
    if (e->literal_value) {
      sprintf(expect, "%d", 0); sprintf(actual, "%ld", e->literal_value);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (strcmp("foo", e->string_literal)) { print_error(test_type, "foo", "char* e->string_literal"); overall_status = FAILURE; }
  }
  expr_destroy(&e);
  return overall_status;
}


Status test_expr_create_unary(void) {
  strcpy(test_type, "Testing: test_expr_create_unary");
  Status overall_status = SUCCESS;
  char expect[3], actual[3];
  struct expr* e = expr_create(EXPR_NOT, expr_create_name(strdup("x")), NULL);
  if (!e) { print_error(test_type, "NOT NULL", "expr e"); overall_status = FAILURE; }
  else {
    if (e->kind != EXPR_NOT) {
      sprintf(expect, "%d", EXPR_NOT); sprintf(actual, "%d", e->kind);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (!e->left) { print_error(test_type, "NOT NULL", "expr* e->left"); overall_status = FAILURE; }
    if (e->right) { print_error(test_type, "NULL", "expr* e->right"); overall_status = FAILURE; }
    if (e->left->kind != EXPR_NAME) { sprintf(expect, "%d", EXPR_NAME); sprintf(actual, "%d", e->left->kind);
      				      print_error(test_type, expect, actual); overall_status = FAILURE;
 				    }
    if (strcmp("x", e->left->name)) { print_error(test_type, "x", "char* e->left->name"); overall_status = FAILURE; }
    if (e->literal_value) {
      sprintf(expect, "%d", 0); sprintf(actual, "%ld", e->literal_value);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->string_literal) { print_error(test_type, "NULL", "char* e->string_literal"); overall_status = FAILURE; }
    if (e->left->left) { print_error(test_type, "NULL", "expr* e->left->left"); overall_status = FAILURE; }
    if (e->left->right) { print_error(test_type, "NULL", "expr* e->left->right"); overall_status = FAILURE; }
  }
  expr_destroy(&e);
  return overall_status;
}

Status test_expr_create_binary_2_op(void) {
  strcpy(test_type, "Testing: test_expr_create_binary_2_op");
  Status overall_status = SUCCESS;
  char expect[3], actual[3];
  struct expr* e = expr_create(EXPR_ADD, expr_create_integer_literal(1), expr_create_integer_literal(493));
  if (!e) { print_error(test_type, "NOT NULL", "expr e"); overall_status = FAILURE; }
  else {
    if (e->kind != EXPR_ADD) {
      sprintf(expect, "%d", EXPR_ADD); sprintf(actual, "%d", e->kind);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->literal_value) {
      sprintf(expect, "%d", 0); sprintf(actual, "%ld", e->literal_value);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->string_literal) { print_error(test_type, "NULL", "char* e->string_literal"); overall_status = FAILURE; }

    // left subtree
    if (!e->left) { print_error(test_type, "NOT NULL", "expr* e->left"); overall_status = FAILURE; }
    if (e->left->name) { print_error(test_type, "NULL", "char* e->left->name"); overall_status = FAILURE; }
    if (e->left->string_literal)  { print_error(test_type, "NULL", "char* e->left->string_literal"); overall_status = FAILURE; }
    if (e->left->literal_value != 1) {
      sprintf(expect, "%d", 0); sprintf(actual, "%ld", e->left->literal_value);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->left->kind != EXPR_INT) {
      sprintf(expect, "%d", EXPR_INT); sprintf(actual, "%d", e->left->kind);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->left->left) { print_error(test_type, "NULL", "expr* e->left->left"); overall_status = FAILURE; }
    if (e->left->right) { print_error(test_type, "NULL", "expr* e->left->right"); overall_status = FAILURE; }

    // right subtree
    if (!e->right) { print_error(test_type, "NOT NULL", "expr* e->right"); overall_status = FAILURE; }
    if (e->right->name) { print_error(test_type, "NULL", "char* e->right->name"); overall_status = FAILURE; }
    if (e->right->string_literal)  { print_error(test_type, "NULL", "char* e->right->string_literal"); overall_status = FAILURE; }
    if (e->right->literal_value != 493) {
      sprintf(expect, "%d", 0); sprintf(actual, "%ld", e->right->literal_value);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->right->kind != EXPR_INT) {
        sprintf(expect, "%d", EXPR_INT); sprintf(actual, "%d", e->right->kind);
        print_error(test_type, expect, actual); overall_status = FAILURE;
      }
    if (e->right->left) { print_error(test_type, "NULL", "expr* e->right->left"); overall_status = FAILURE; }
    if (e->right->right) { print_error(test_type, "NULL", "expr* e->right->right"); overall_status = FAILURE; }
  }
  expr_destroy(&e);
  return overall_status;
}

Status test_expr_create_binary_3_op(void) {
  strcpy(test_type, "Testing: test_expr_create_binary_3_op");
  Status overall_status = SUCCESS;
  char expect[3], actual[3];
  struct expr* e = expr_create(EXPR_MULT, expr_create(EXPR_ADD, expr_create_integer_literal(10), expr_create_integer_literal(20)), expr_create_integer_literal(30));
  if (!e) { print_error(test_type, "NOT NULL", "expr e"); overall_status = FAILURE; }
  else {
    if (e->kind != EXPR_MULT) {
      sprintf(expect, "%d", EXPR_MULT); sprintf(actual, "%d", e->kind);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->literal_value) {
      sprintf(expect, "%d", 0); sprintf(actual, "%ld", e->literal_value);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->string_literal) { print_error(test_type, "NULL", "char* e->string_literal"); overall_status = FAILURE; }

    // left subtree
    if (!e->left) { print_error(test_type, "NOT NULL", "expr* e->left"); overall_status = FAILURE; }
    if (e->left->name) { print_error(test_type, "NULL", "char* e->left->name"); overall_status = FAILURE; }
    if (e->left->string_literal)  { print_error(test_type, "NULL", "char* e->left->string_literal"); overall_status = FAILURE; }
    if (e->left->literal_value)  { print_error(test_type, "0", "int e->left->literal_value"); overall_status = FAILURE; }
    if (e->left->kind != EXPR_ADD) {
      sprintf(expect, "%d", EXPR_ADD); sprintf(actual, "%d", e->left->kind);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }

    // left left subtree
    if (!e->left->left) { print_error(test_type, "NOT NULL", "expr* e->left->left"); overall_status = FAILURE; }
    if (e->left->left->name) { print_error(test_type, "NULL", "char* e->left->left->name"); overall_status = FAILURE; }
    if (e->left->left->string_literal)  { print_error(test_type, "NULL", "char* e->left->left->string_literal"); overall_status = FAILURE; }
    if (e->left->left->literal_value != 10) {
      sprintf(expect, "%d", 0); sprintf(actual, "%ld", e->left->left->literal_value);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->left->left->kind != EXPR_INT) {
      sprintf(expect, "%d", EXPR_INT); sprintf(actual, "%d", e->left->left->kind);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->left->left->left) { print_error(test_type, "NULL", "expr* e->left->left->left"); overall_status = FAILURE; }
    if (e->left->left->right) { print_error(test_type, "NULL", "expr* e->left->left->right"); overall_status = FAILURE; }

    // left right subtree
    if (!e->left->right) { print_error(test_type, "NOT NULL", "expr* e->left->right"); overall_status = FAILURE; }
    if (e->left->right->name) { print_error(test_type, "NULL", "char* e->left->right->name"); overall_status = FAILURE; }
    if (e->left->right->string_literal)  { print_error(test_type, "NULL", "char* e->left->right->string_literal"); overall_status = FAILURE; }
    if (e->left->right->literal_value != 20) {
      sprintf(expect, "%d", 0); sprintf(actual, "%ld", e->left->right->literal_value);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->left->right->kind != EXPR_INT) {
      sprintf(expect, "%d", EXPR_INT); sprintf(actual, "%d", e->left->right->kind);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->left->right->left) { print_error(test_type, "NULL", "expr* e->left->right->left"); overall_status = FAILURE; }
    if (e->left->right->right) { print_error(test_type, "NULL", "expr* e->left->right->right"); overall_status = FAILURE; }

    // right subtree
    if (!e->right) { print_error(test_type, "NOT NULL", "expr* e->right"); overall_status = FAILURE; }
    if (e->right->name) { print_error(test_type, "NULL", "char* e->right->name"); overall_status = FAILURE; }
    if (e->right->string_literal)  { print_error(test_type, "NULL", "char* e->right->string_literal"); overall_status = FAILURE; }
    if (e->right->literal_value != 30) {
      sprintf(expect, "%d", 0); sprintf(actual, "%ld", e->right->literal_value);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->right->kind != EXPR_INT) {
      sprintf(expect, "%d", EXPR_INT); sprintf(actual, "%d", e->right->kind);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
  }
  if (e->right->left) { print_error(test_type, "NULL", "expr* e->right->left"); overall_status = FAILURE; }
  if (e->right->right) { print_error(test_type, "NULL", "expr* e->right->right"); overall_status = FAILURE; }
  expr_destroy(&e);
  return overall_status;
}


Status test_stmt_create_print(void) {
  strcpy(test_type, "Testing: test_stmt_create_print");
  Status overall_status = SUCCESS;
  char kind_expect[3]; char kind_actual[3];
  struct stmt* s = stmt_create(STMT_PRINT, NULL, NULL, expr_create_string_literal("hello world!:)\n") , NULL, NULL, NULL, NULL);
  if (!s) { print_error(test_type, "NOT NULL", "stmt s"); overall_status = FAILURE; }
  else {
    if (s->kind != STMT_PRINT) {
      sprintf(kind_expect, "%d", STMT_PRINT); sprintf(kind_actual, "%d", s->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (s->decl) { print_error(test_type, "NULL", "decl* s->decl"); overall_status = FAILURE; }
    if (s->init_expr) { print_error(test_type, "NULL", "expr* s->init_expr"); overall_status = FAILURE; }
    if (!s->expr) { print_error(test_type, "NOT NULL", "expr* s->expr"); overall_status = FAILURE; }
    if (s->next_expr) { print_error(test_type, "NULL", "expr* s->next_expr"); overall_status = FAILURE; }
    if (s->body) { print_error(test_type, "NULL", "stmt* s->body"); overall_status = FAILURE; }
    if (s->else_body) { print_error(test_type, "NULL", "stmt* s->else_body"); overall_status = FAILURE; }
    if (s->next) { print_error(test_type, "NULL", "stmt* s->next"); overall_status = FAILURE; }

    // check for expr subtree
    if (s->expr->kind != EXPR_STR) {
      sprintf(kind_expect, "%d", EXPR_STR); sprintf(kind_actual, "%d", s->expr->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (s->expr->left) { print_error(test_type, "NULL", "expr* s->expr->left"); overall_status = FAILURE; }
    if (s->expr->right) { print_error(test_type, "NULL", "expr* s->expr->right"); overall_status = FAILURE; }
    if (s->expr->name) { print_error(test_type, "NULL", "char* s->expr->name"); overall_status = FAILURE; }
    if (s->expr->literal_value) { print_error(test_type, "NULL", "int* s->expr->literal_value"); overall_status = FAILURE; }
    if (strcmp("hello world!:)\n", s->expr->string_literal)) { print_error(test_type, "NULL", "char* s->expr->string_literal"); overall_status = FAILURE; }
  }
  stmt_destroy(&s);
  return overall_status;
}

Status test_decl_create_atomic_uninit(void) {
  strcpy(test_type, "Testing: decl_create_atomic_uninit");
  Status overall_status = SUCCESS;
  char kind_expect[3]; char kind_actual[3];
  struct decl* d = decl_create(strdup("foo"), type_create(TYPE_INTEGER, NULL, NULL, NULL), NULL, NULL, NULL);
  if (!d) { print_error(test_type, "NOT NULL", "decl d"); overall_status = FAILURE; }
  else {
    if (strcmp(d->name, "foo")) { print_error(test_type, "foo", "char* d->name"); overall_status = FAILURE; }
    if (!d->type) { print_error(test_type, "NOT NULL", "type* d->type"); overall_status = FAILURE; }
    if (d->value) { print_error(test_type, "NULL", "expr* d->value"); overall_status = FAILURE; }
    if (d->code) { print_error(test_type, "NULL", "stmt* d->code"); overall_status = FAILURE; }
    if (d->next) { print_error(test_type, "NULL", "decl* d->next"); overall_status = FAILURE; }

    // check for type subtree
    if (d->type->kind != TYPE_INTEGER) {
      sprintf(kind_expect, "%d", TYPE_INTEGER); sprintf(kind_actual, "%d", d->type->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->type->subtype) { print_error(test_type, "NULL", "type* d->type->subtype"); overall_status = FAILURE; }
    if (d->type->params) { print_error(test_type, "NULL", "param_list* d->type->params"); overall_status = FAILURE; }
  }
  decl_destroy(&d);
  return overall_status;
}

Status test_decl_create_atomic_init(void) {
  strcpy(test_type, "Testing: decl_create_atomic_init");
  Status overall_status = SUCCESS;
  char kind_expect[3]; char kind_actual[3];
  struct decl* d = decl_create(strdup("bar"), type_create(TYPE_STRING, NULL, NULL, NULL), expr_create_string_literal("hello world!:)\n"), NULL, NULL);
  if (!d) { print_error(test_type, "NOT NULL", "decl d"); overall_status = FAILURE; }
  else {
    if (strcmp(d->name, "bar")) { print_error(test_type, "bar", "char* d->name"); overall_status = FAILURE; }
    if (!d->type) { print_error(test_type, "NOT NULL", "type* d->type"); overall_status = FAILURE; }
    if (!d->value) { print_error(test_type, "NOT NULL", "expr* d->value"); overall_status = FAILURE; }
    if (d->code) { print_error(test_type, "NULL", "stmt* d->code"); overall_status = FAILURE; }
    if (d->next) { print_error(test_type, "NULL", "decl* d->next"); overall_status = FAILURE; }

    // check for type subtree
    if (d->type->kind != TYPE_STRING) {
      sprintf(kind_expect, "%d", TYPE_STRING); sprintf(kind_actual, "%d", d->type->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->type->subtype) { print_error(test_type, "NULL", "type* d->type->subtype"); overall_status = FAILURE; }
    if (d->type->params) { print_error(test_type, "NULL", "param_list* d->type->params"); overall_status = FAILURE; }

    // check for expr subtree
    if (d->value->kind != EXPR_STR) {
      sprintf(kind_expect, "%d", EXPR_STR); sprintf(kind_actual, "%d", d->value->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->value->left) { print_error(test_type, "NULL", "expr* d->value->left"); overall_status = FAILURE; }
    if (d->value->right) { print_error(test_type, "NULL", "expr* d->value->right"); overall_status = FAILURE; }
    if (d->value->name) { print_error(test_type, "NULL", "char* d->value->name"); overall_status = FAILURE; }
    if (d->value->literal_value) { print_error(test_type, "NULL", "int d->value->literal_value"); overall_status = FAILURE; }
    if (strcmp("hello world!:)\n", d->value->string_literal)) { print_error(test_type, "NULL", "char* d->value->string_literal"); overall_status = FAILURE; }
  }
  decl_destroy(&d);
  return overall_status;
}

Status test_decl_create_composite_array(void) {
  strcpy(test_type, "Testing: decl_create_composite_array");
  Status overall_status = SUCCESS;
  char kind_expect[3]; char kind_actual[3];
  struct decl* d = decl_create(strdup("foo"), type_create(TYPE_ARRAY, type_create(TYPE_INTEGER, NULL, NULL, NULL), NULL, NULL), NULL, NULL, NULL);
  if (!d) { print_error(test_type, "NOT NULL", "decl d"); overall_status = FAILURE; }
  else {
    if (strcmp(d->name, "foo")) { print_error(test_type, "foo", "char* d->name"); overall_status = FAILURE; }
    if (!d->type) { print_error(test_type, "NOT NULL", "type* d->type"); overall_status = FAILURE; }
    if (d->value) { print_error(test_type, "NULL", "expr* d->value"); overall_status = FAILURE; }
    if (d->code) { print_error(test_type, "NULL", "stmt* d->code"); overall_status = FAILURE; }
    if (d->next) { print_error(test_type, "NULL", "decl* d->next"); overall_status = FAILURE; }

    // check for type subtree
    if (d->type->kind != TYPE_ARRAY) {
      sprintf(kind_expect, "%d", TYPE_ARRAY); sprintf(kind_actual, "%d", d->type->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (!d->type->subtype) { print_error(test_type, "NOT NULL", "type* d->type->subtype"); overall_status = FAILURE; }
    if (d->type->params) { print_error(test_type, "NULL", "param_list* d->type->params"); overall_status = FAILURE; }

    // check for subtype subtree
    if (d->type->subtype->kind != TYPE_INTEGER) {
      sprintf(kind_expect, "%d", TYPE_INTEGER); sprintf(kind_actual, "%d", d->type->subtype->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->type->subtype->subtype) { print_error(test_type, "NULL", "type* d->type->subtype->subtype"); overall_status = FAILURE; }
    if (d->type->subtype->params) { print_error(test_type, "NULL", "param_list* d->type->subtype->params"); overall_status = FAILURE; }
  }
  decl_destroy(&d);
  return overall_status;
}


Status test_decl_create_composite_function(void) {
  strcpy(test_type, "Testing: decl_create_composite_function");
  Status overall_status = SUCCESS;
  char kind_expect[3]; char kind_actual[3];
  struct decl* d = decl_create(strdup("foo"), type_create(TYPE_FUNCTION,
				      type_create(TYPE_VOID, NULL, NULL, NULL),
				      param_list_create(strdup("x"), type_create(TYPE_INTEGER, NULL, NULL,NULL), NULL), NULL),
				      NULL, NULL, NULL);
  if (!d) { print_error(test_type, "NOT NULL", "decl d"); overall_status = FAILURE; }
  else {
    if (strcmp(d->name, "foo")) { print_error(test_type, "foo", "char* d->name"); overall_status = FAILURE; }
    if (!d->type) { print_error(test_type, "NOT NULL", "type* d->type"); overall_status = FAILURE; }
    if (d->value) { print_error(test_type, "NULL", "expr* d->value"); overall_status = FAILURE; }
    if (d->code) { print_error(test_type, "NULL", "stmt* d->code"); overall_status = FAILURE; }
    if (d->next) { print_error(test_type, "NULL", "decl* d->next"); overall_status = FAILURE; }

    // check for type subtree
    if (d->type->kind != TYPE_FUNCTION) {
      sprintf(kind_expect, "%d", TYPE_FUNCTION); sprintf(kind_actual, "%d", d->type->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (!d->type->subtype) { print_error(test_type, "NOT NULL", "type* d->type->subtype"); overall_status = FAILURE; }
    if (!d->type->params) { print_error(test_type, "NOT NULL", "param_list* d->type->params"); overall_status = FAILURE; }

    // check for subtype subtree
    if (d->type->subtype->kind != TYPE_VOID) {
      sprintf(kind_expect, "%d", TYPE_VOID); sprintf(kind_actual, "%d", d->type->subtype->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->type->subtype->subtype) { print_error(test_type, "NULL", "type* d->type->subtype->subtype"); overall_status = FAILURE; }
    if (d->type->subtype->params) { print_error(test_type, "NULL", "param_list* d->type->subtype->params"); overall_status = FAILURE; }

    // check for params subtree
    if (strcmp(d->type->params->name, "x")) { print_error(test_type, "x", "char* d->type->params->name"); overall_status = FAILURE; }
    if (d->type->params->type->kind != TYPE_INTEGER) {
      sprintf(kind_expect, "%d", TYPE_INTEGER); sprintf(kind_actual, "%d", d->type->params->type->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->type->params->type->subtype) { print_error(test_type, "NULL", "type* d->type->params->subtype->subtype"); overall_status = FAILURE; }
    if (d->type->params->type->params) { print_error(test_type, "NULL", "param_list* d->type->params->subtype->params"); overall_status = FAILURE; }
    if (d->type->params->next) { print_error(test_type, "NULL", "decl* d->type->params->next"); overall_status = FAILURE; }
  }
  decl_destroy(&d);
  return overall_status;
}


// this function is so big i am so fucking sorry
Status test_decl_create_program(void) {
  strcpy(test_type, "Testing: decl_create_program");

  /*
  main: function integer(argc: integer, argv: array [] string) = {
    i: integer;
    n: integer = 10;
    for (i = 0; i < n; i++) print "hello world!:)\n";
    return 0;
  }
  */

  Status overall_status = SUCCESS;
  char kind_expect[3]; char kind_actual[3];
  // structs that make up param list
  struct param_list* p = param_list_create(strdup("argc"), type_create(TYPE_INTEGER, NULL, NULL,NULL),
					   param_list_create(strdup("argv"), type_create(TYPE_ARRAY, type_create(TYPE_STRING, NULL, NULL, NULL), NULL, NULL), NULL)
					  );
  // structs that make up stmts in code
  struct stmt* return_stmt = stmt_create(STMT_RETURN, NULL, NULL, expr_create_integer_literal(0), NULL, NULL, NULL, NULL);
  struct stmt* for_body = stmt_create(STMT_PRINT, NULL, NULL, expr_create_string_literal("hello world!:)\n"), NULL, NULL, NULL, NULL);
  struct stmt* for_stmt = stmt_create(STMT_FOR, NULL, expr_create(EXPR_ASSIGN, expr_create_name(strdup("i")), expr_create_integer_literal(0)),
						      expr_create(EXPR_LESS, expr_create_name(strdup("i")), expr_create_name(strdup("n"))),
						      expr_create(EXPR_INC, expr_create_name(strdup("i")), NULL),
						      for_body, NULL, return_stmt);
  struct stmt* n_init = stmt_create(STMT_DECL, decl_create(strdup("n"), type_create(TYPE_INTEGER, NULL, NULL, NULL), expr_create_integer_literal(10), NULL, NULL),
					       NULL, NULL, NULL, NULL, NULL, for_stmt);

  struct stmt* i_init = stmt_create(STMT_DECL, decl_create(strdup("i"), type_create(TYPE_INTEGER, NULL, NULL, NULL), NULL, NULL, NULL), NULL, NULL, NULL, NULL, NULL, n_init);



  // the actual declaration (oh dear lord...)
  struct decl* d = decl_create(strdup("main"), type_create(TYPE_FUNCTION,
                                      type_create(TYPE_INTEGER, NULL, NULL, NULL), p, NULL), NULL, i_init, NULL);

  // main: function integer(argc: integer, argv: array [] string)
  if (!d) { print_error(test_type, "NOT NULL", "decl d"); overall_status = FAILURE; }
  else {
    if (strcmp(d->name, "main")) { print_error(test_type, "main", "char* d->name"); overall_status = FAILURE; }
    if (!d->type) { print_error(test_type, "NOT NULL", "type* d->type"); overall_status = FAILURE; }
    if (d->value) { print_error(test_type, "NULL", "expr* d->value"); overall_status = FAILURE; }
    if (!d->code) { print_error(test_type, "NOT NULL", "stmt* d->code"); overall_status = FAILURE; }
    if (d->next) { print_error(test_type, "NULL", "decl* d->next"); overall_status = FAILURE; }

    // check for type subtree
    if (d->type->kind != TYPE_FUNCTION) {
      sprintf(kind_expect, "%d", TYPE_FUNCTION); sprintf(kind_actual, "%d", d->type->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (!d->type->subtype) { print_error(test_type, "NOT NULL", "type* d->type->subtype"); overall_status = FAILURE; }
    if (!d->type->params) { print_error(test_type, "NOT NULL", "param_list* d->type->params"); overall_status = FAILURE; }

    // check for subtype subtree
    if (d->type->subtype->kind != TYPE_INTEGER) {
      sprintf(kind_expect, "%d", TYPE_INTEGER); sprintf(kind_actual, "%d", d->type->subtype->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->type->subtype->subtype) { print_error(test_type, "NULL", "type* d->type->subtype->subtype"); overall_status = FAILURE; }
    if (d->type->subtype->params) { print_error(test_type, "NULL", "param_list* d->type->subtype->params"); overall_status = FAILURE; }

    // check for params subtree

    // argument 1: argc: integer
    if (strcmp(d->type->params->name, "argc")) { print_error(test_type, "argc", "char* d->type->params->name"); overall_status = FAILURE; }
    if (d->type->params->type->kind != TYPE_INTEGER) {
      sprintf(kind_expect, "%d", TYPE_INTEGER); sprintf(kind_actual, "%d", d->type->params->type->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->type->params->type->subtype) { print_error(test_type, "NULL", "type* d->type->params->type->subtype"); overall_status = FAILURE; }
    if (d->type->params->type->params) { print_error(test_type, "NULL", "param_list* d->type->params->type->params"); overall_status = FAILURE; }
    if (!d->type->params->next) { print_error(test_type, "NOT NULL", "decl* d->type->params->next"); overall_status = FAILURE; }

    // argument 2 : argv: array [] string
    if (strcmp(d->type->params->next->name, "argv")) { print_error(test_type, "argv", "char* d->type->params->next->name"); overall_status = FAILURE; }
    if (d->type->params->next->type->kind != TYPE_ARRAY) {
      sprintf(kind_expect, "%d", TYPE_ARRAY); sprintf(kind_actual, "%d", d->type->params->next->type->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    // has subtype
    if (!d->type->params->next->type->subtype) { print_error(test_type, "NOT NULL", "type* d->type->params->next->type->subtype"); overall_status = FAILURE; }
    if (d->type->params->next->type->subtype->kind != TYPE_STRING) {
      sprintf(kind_expect, "%d", TYPE_STRING); sprintf(kind_actual, "%d", d->type->params->next->type->subtype->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->type->params->next->type->params) { print_error(test_type, "NULL", "param_list* d->type->params->next->type->params"); overall_status = FAILURE; }
    if (d->type->params->next->next) { print_error(test_type, "NULL", "decl* d->type->params->next->next"); overall_status = FAILURE; }

    // check for code subtree (this will consist of A LOT, this was a pain to write...

    // i: integer, d->code->decl...
    if (d->code->kind != STMT_DECL) {
      sprintf(kind_expect, "%d", STMT_DECL); sprintf(kind_actual, "%d", d->code->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (strcmp(d->code->decl->name, "i")) { print_error(test_type, "i", "char* d->code->decl->name"); overall_status = FAILURE; }
    if (!d->code->decl->type) { print_error(test_type, "NOT NULL", "type* d->code->decl->type"); overall_status = FAILURE; }
    if (d->code->decl->value) { print_error(test_type, "NULL", "expr* d->code->decl->value"); overall_status = FAILURE; }
    if (d->code->decl->code) { print_error(test_type, "NULL", "stmt* d->code->decl->code"); overall_status = FAILURE; }
    if (!d->code->next) { print_error(test_type, "NOT NULL", "decl* d->code->next"); overall_status = FAILURE; }

    // check for type subtree
    if (d->code->decl->type->kind != TYPE_INTEGER) {
      sprintf(kind_expect, "%d", TYPE_INTEGER); sprintf(kind_actual, "%d", d->code->decl->type->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->code->decl->type->subtype) { print_error(test_type, "NULL", "type* d->code->decl->type->subtype"); overall_status = FAILURE; }
    if (d->code->decl->type->params) { print_error(test_type, "NULL", "param_list* d->code->decl->type->params"); overall_status = FAILURE; }

    // n: integer = 10, d->code->next->decl...
    if (d->code->next->kind != STMT_DECL) {
      sprintf(kind_expect, "%d", STMT_DECL); sprintf(kind_actual, "%d", d->code->next->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (strcmp(d->code->next->decl->name, "n")) { print_error(test_type, "n", "char* d->code->next->decl->name"); overall_status = FAILURE; }
    if (!d->code->next->decl->type) { print_error(test_type, "NOT NULL", "type* d->code->next->decl->type"); overall_status = FAILURE; }
    if (!d->code->next->decl->value) { print_error(test_type, "NOT NULL", "expr* d->code->next->decl->value"); overall_status = FAILURE; }
    if (d->code->next->decl->code) { print_error(test_type, "NULL", "stmt* d->code->next->decl->code"); overall_status = FAILURE; }
    if (!d->code->next->next) { print_error(test_type, "NOT NULL", "decl* d->code->next->next"); overall_status = FAILURE; }

    // check for type subtree
    if (d->code->next->decl->type->kind != TYPE_INTEGER) {
      sprintf(kind_expect, "%d", TYPE_INTEGER); sprintf(kind_actual, "%d", d->code->next->decl->type->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->code->next->decl->type->subtype) { print_error(test_type, "NULL", "type* d->code->next->decl->type->subtype"); overall_status = FAILURE; }
    if (d->code->next->decl->type->params) { print_error(test_type, "NULL", "param_list* d->code->next->decl->type->params"); overall_status = FAILURE; }

    // check for value (expr) subtree
    if (d->code->next->decl->value->kind != EXPR_INT) {
      sprintf(kind_expect, "%d", EXPR_INT); sprintf(kind_actual, "%d", d->code->next->decl->value->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->code->next->decl->value->left) { print_error(test_type, "NULL", "expr* d->code->next->decl->value->left"); overall_status = FAILURE; }
    if (d->code->next->decl->value->right) { print_error(test_type, "NULL", "expr* d->code->next->decl->value->right"); overall_status = FAILURE; }
    if (d->code->next->decl->value->name) { print_error(test_type, "NULL", "char* d->code->next->decl->value->name"); overall_status = FAILURE; }
    if (d->code->next->decl->value->literal_value != 10) {
      sprintf(kind_expect, "%d", 10); sprintf(kind_actual, "%ld", d->code->next->decl->value->literal_value);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->code->next->decl->value->string_literal) { print_error(test_type, "NULL", "char* d->code->next->decl->value->string_literal"); overall_status = FAILURE; }

    // for (i = 0; i < n; i++) print "hello world!:)\n"; d->code->next->next...
    if (d->code->next->next->kind != STMT_FOR) {
      sprintf(kind_expect, "%d", STMT_FOR); sprintf(kind_actual, "%d", d->code->next->next->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->code->next->next->decl) { print_error(test_type, "NULL", "decl* d->code->next->next->decl"); overall_status = FAILURE; }
    if (!d->code->next->next->init_expr) { print_error(test_type, "NOT NULL", "expr* d->code->next->next->init_expr"); overall_status = FAILURE; }
    if (!d->code->next->next->expr) { print_error(test_type, "NOT NULL", "expr* d->code->next->next->expr"); overall_status = FAILURE; }
    if (!d->code->next->next->next_expr) { print_error(test_type, "NOT NULL", "expr* d->code->next->next->next_expr"); overall_status = FAILURE; }
    if (!d->code->next->next->body) { print_error(test_type, "NOT NULL", "stmt* d->code->next->next->body"); overall_status = FAILURE; }
    if (d->code->next->next->else_body) { print_error(test_type, "NULL", "stmt* d->code->next->next->else_body"); overall_status = FAILURE; }
    if (!d->code->next->next->next) { print_error(test_type, "NOT NULL", "stmt* d->code->next->next->next"); overall_status = FAILURE; }

    // check for init_expr subtree
    if (d->code->next->next->init_expr->kind != EXPR_ASSIGN) {
      sprintf(kind_expect, "%d", EXPR_ASSIGN); sprintf(kind_actual, "%d", d->code->next->next->init_expr->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (!d->code->next->next->init_expr->left) { print_error(test_type, "NOT NULL", "expr* d->code->next->next->init_expr->left"); overall_status = FAILURE; }
    if (!d->code->next->next->init_expr->right) { print_error(test_type, "NOT NULL", "expr* d->code->next->next->init_expr->right"); overall_status = FAILURE; }
    if (d->code->next->next->init_expr->name) { print_error(test_type, "NULL", "char* d->code->next->next->init_expr->name"); overall_status = FAILURE; }
    if (d->code->next->next->init_expr->literal_value) { print_error(test_type, "NULL", "int* d->code->next->next->init_expr->literal_value"); overall_status = FAILURE; }
    if (d->code->next->next->init_expr->string_literal) { print_error(test_type, "NULL", "char* d->code->next->next->init_expr->string_literal"); overall_status = FAILURE; }

    // check for init_expr left subtree
    if (d->code->next->next->init_expr->left->kind != EXPR_NAME) {
      sprintf(kind_expect, "%d", EXPR_NAME); sprintf(kind_actual, "%d", d->code->next->next->init_expr->left->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->code->next->next->init_expr->left->left) { print_error(test_type, "NULL", "expr* d->code->next->next->init_expr->left->left"); overall_status = FAILURE; }
    if (d->code->next->next->init_expr->left->right) { print_error(test_type, "NULL", "expr* d->code->next->next->init_expr->left->right"); overall_status = FAILURE; }
    if (strcmp("i", d->code->next->next->init_expr->left->name)) { print_error(test_type, "i", "char* d->code->next->next->init_expr->left->name"); overall_status = FAILURE; }
    if (d->code->next->next->init_expr->left->literal_value) { print_error(test_type, "NULL", "int d->code->next->next->init_expr->left->literal_value"); overall_status = FAILURE; }
    if (d->code->next->next->init_expr->left->string_literal) { print_error(test_type, "NULL", "char* d->code->next->next->init_expr->left->string_literal"); overall_status = FAILURE; }

    // check for init_expr right subtree
    if (d->code->next->next->init_expr->right->kind != EXPR_INT) {
      sprintf(kind_expect, "%d", EXPR_INT); sprintf(kind_actual, "%d", d->code->next->next->init_expr->right->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->code->next->next->init_expr->right->left) { print_error(test_type, "NULL", "expr* d->code->next->next->init_expr->right->left"); overall_status = FAILURE; }
    if (d->code->next->next->init_expr->right->right) { print_error(test_type, "NULL", "expr* d->code->next->next->init_expr->right->right"); overall_status = FAILURE; }
    if (d->code->next->next->init_expr->right->name) { print_error(test_type, "NULL", "char* d->code->next->next->init_expr->right->name"); overall_status = FAILURE; }
    if (d->code->next->next->init_expr->literal_value != 0) {
      sprintf(kind_expect, "%d", EXPR_INT); sprintf(kind_actual, "%ld", d->code->next->next->init_expr->literal_value);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->code->next->next->init_expr->left->string_literal) { print_error(test_type, "NULL", "char* d->code->next->next->init_expr->right->string_literal"); overall_status = FAILURE; }

    // check for expr subtree
    if (d->code->next->next->expr->kind != EXPR_LESS) {
      sprintf(kind_expect, "%d", EXPR_LESS); sprintf(kind_actual, "%d", d->code->next->next->expr->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (!d->code->next->next->expr->left) { print_error(test_type, "NOT NULL", "expr* d->code->next->next->expr->left"); overall_status = FAILURE; }
    if (!d->code->next->next->expr->right) { print_error(test_type, "NOTNULL", "expr* d->code->next->next->expr->right"); overall_status = FAILURE; }
    if (d->code->next->next->expr->name) { print_error(test_type, "NULL", "char* d->code->next->next->expr->name"); overall_status = FAILURE; }
    if (d->code->next->next->expr->literal_value) { print_error(test_type, "NULL", "int d->code->next->next->expr->literal_value"); overall_status = FAILURE; }
    if (d->code->next->next->expr->string_literal) { print_error(test_type, "NULL", "char* d->code->next->next->expr->string_literal"); overall_status = FAILURE; }

    // check for expr left subtree
    if (d->code->next->next->expr->left->kind != EXPR_NAME) {
      sprintf(kind_expect, "%d", EXPR_NAME); sprintf(kind_actual, "%d", d->code->next->next->expr->left->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->code->next->next->expr->left->left) { print_error(test_type, "NULL", "expr* d->code->next->next->expr->left->left"); overall_status = FAILURE; }
    if (d->code->next->next->expr->left->right) { print_error(test_type, "NULL", "expr* d->code->next->next->expr->left->right"); overall_status = FAILURE; }
    if (strcmp("i", d->code->next->next->expr->left->name)) { print_error(test_type, "i", "char* d->code->next->next->expr->left->name"); overall_status = FAILURE; }
    if (d->code->next->next->expr->left->literal_value) {  print_error(test_type, "NULL", "int d->code->next->next->expr->left->literal_value"); overall_status = FAILURE; }
    if (d->code->next->next->expr->left->string_literal) { print_error(test_type, "NULL", "char* d->code->next->next->expr->right->string_literal"); overall_status = FAILURE; }

    // check for expr right subtree
    if (d->code->next->next->expr->right->kind != EXPR_NAME) {
      sprintf(kind_expect, "%d", EXPR_NAME); sprintf(kind_actual, "%d", d->code->next->next->expr->right->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->code->next->next->expr->right->left) { print_error(test_type, "NULL", "expr* d->code->next->next->expr->right->left"); overall_status = FAILURE; }
    if (d->code->next->next->expr->right->right) { print_error(test_type, "NULL", "expr* d->code->next->next->expr->right->right"); overall_status = FAILURE; }
    if (strcmp("n", d->code->next->next->expr->right->name)) { print_error(test_type, "i", "char* d->code->next->next->expr->right->name"); overall_status = FAILURE; }
    if (d->code->next->next->expr->right->literal_value) {  print_error(test_type, "NULL", "int d->code->next->next->expr->right->literal_value"); overall_status = FAILURE; }
    if (d->code->next->next->expr->right->string_literal) { print_error(test_type, "NULL", "char* d->code->next->next->expr->right->string_literal"); overall_status = FAILURE; }

    // check for next_expr subtree
    if (d->code->next->next->next_expr->kind != EXPR_INC) {
      sprintf(kind_expect, "%d", EXPR_INC); sprintf(kind_actual, "%d", d->code->next->next->next_expr->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (!d->code->next->next->next_expr->left) { print_error(test_type, "NOT NULL", "expr* d->code->next->next->next_expr->left"); overall_status = FAILURE; }
    if (d->code->next->next->next_expr->right) { print_error(test_type, "NULL", "expr* d->code->next->next->next_expr->right"); overall_status = FAILURE; }
    if (d->code->next->next->next_expr->name) { print_error(test_type, "NULL", "char* d->code->next->next->next_expr->name"); overall_status = FAILURE; }
    if (d->code->next->next->next_expr->literal_value) { print_error(test_type, "NULL", "int d->code->next->next->next_expr->literal_value"); overall_status = FAILURE; }
    if (d->code->next->next->next_expr->string_literal) { print_error(test_type, "NULL", "char* d->code->next->next->next_expr->string_literal"); overall_status = FAILURE; }

    // check for next_expr left subtree
    if (d->code->next->next->next_expr->left->kind != EXPR_NAME) {
      sprintf(kind_expect, "%d", EXPR_NAME); sprintf(kind_actual, "%d", d->code->next->next->next_expr->left->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->code->next->next->next_expr->left->left) { print_error(test_type, "NULL", "expr* d->code->next->next->next_expr->left->left"); overall_status = FAILURE; }
    if (d->code->next->next->next_expr->left->right) { print_error(test_type, "NULL", "expr* d->code->next->next->next_expr->left->right"); overall_status = FAILURE; }
    if (strcmp("i", d->code->next->next->next_expr->left->name)) { print_error(test_type, "i", "char* d->code->next->next->next_expr->left->name"); overall_status = FAILURE; }
    if (d->code->next->next->next_expr->left->literal_value) { print_error(test_type, "NULL", "int d->code->next->next->next_expr->left->literal_value"); overall_status = FAILURE; }
    if (d->code->next->next->expr->left->string_literal) { print_error(test_type, "NULL", "char* d->code->next->next->expr->left->string_literal"); overall_status = FAILURE; }

    // check for body subtree
    if (d->code->next->next->body->kind != STMT_PRINT) {
      sprintf(kind_expect, "%d", STMT_PRINT); sprintf(kind_actual, "%d", d->code->next->next->body->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->code->next->next->body->decl) { print_error(test_type, "NULL", "decl* d->code->next->next->body->decl"); overall_status = FAILURE; }
    if (d->code->next->next->body->init_expr) { print_error(test_type, "NULL", "expr* d->code->next->next->body->init_expr"); overall_status = FAILURE; }
    if (!d->code->next->next->body->expr) { print_error(test_type, "NOT NULL", "expr* d->code->next->next->body->expr"); overall_status = FAILURE; }
    if (d->code->next->next->body->next_expr) { print_error(test_type, "NULL", "expr* d->code->next->next->body->next_expr"); overall_status = FAILURE; }
    if (d->code->next->next->body->body) { print_error(test_type, "NULL", "stmt* d->code->next->next->body->body"); overall_status = FAILURE; }
    if (d->code->next->next->body->else_body) { print_error(test_type, "NULL", "stmt* d->code->next->next->body->else_body"); overall_status = FAILURE; }
    if (d->code->next->next->body->next) { print_error(test_type, "NULL", "stmt* d->code->next->next->body->next"); overall_status = FAILURE; }

    // check for body expr subtree
    if (d->code->next->next->body->expr->kind != EXPR_STR) {
      sprintf(kind_expect, "%d", EXPR_STR); sprintf(kind_actual, "%d", d->code->next->next->body->expr->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->code->next->next->body->expr->left) { print_error(test_type, "NULL", "expr* d->code->next->next->next_expr->left->left"); overall_status = FAILURE; }
    if (d->code->next->next->body->expr->right) { print_error(test_type, "NULL", "expr* d->code->next->next->next_expr->left->right"); overall_status = FAILURE; }
    if (d->code->next->next->body->expr->name) { print_error(test_type, "NULL", "char* d->code->next->next->next_expr->left->name"); overall_status = FAILURE; }
    if (d->code->next->next->body->expr->literal_value) { print_error(test_type, "NULL", "int d->code->next->next->next_expr->left->literal_value"); overall_status = FAILURE; }
    if (strcmp(d->code->next->next->body->expr->string_literal, "hello world!:)\n")) { print_error(test_type, "NULL", " char* d->code->next->next->body->string_literal"); overall_status = FAILURE; }

    // return 0; d->code->next->next->next
    if (d->code->next->next->next->kind != STMT_RETURN) {
      sprintf(kind_expect, "%d", STMT_RETURN); sprintf(kind_actual, "%d", d->code->next->next->next->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->code->next->next->next->decl) { print_error(test_type, "NULL", "decl* d->code->next->next->next->decl"); overall_status = FAILURE; }
    if (d->code->next->next->next->init_expr) { print_error(test_type, "NULL", "expr* d->code->next->next->next->init_expr"); overall_status = FAILURE; }
    if (!d->code->next->next->next->expr) { print_error(test_type, "NOT NULL", "expr* d->code->next->next->next->expr"); overall_status = FAILURE; }
    if (d->code->next->next->next->next_expr) { print_error(test_type, "NULL", "expr* d->code->next->next->next->next_expr"); overall_status = FAILURE; }
    if (d->code->next->next->next->body) { print_error(test_type, "NULL", "stmt* d->code->next->next->next->body"); overall_status = FAILURE; }
    if (d->code->next->next->next->else_body) { print_error(test_type, "NULL", "stmt* d->code->next->next->next->else_body"); overall_status = FAILURE; }
    if (d->code->next->next->next->next) { print_error(test_type, "NULL", "stmt* d->code->next->next->next->next"); overall_status = FAILURE; }

    // check for expr subtree
    if (d->code->next->next->next->expr->kind != EXPR_INT) {
      sprintf(kind_expect, "%d", EXPR_INT); sprintf(kind_actual, "%d", d->code->next->next->next->expr->kind);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->code->next->next->next->expr->left) { print_error(test_type, "NULL", "expr* d->code->next->next->next->expr->left"); overall_status = FAILURE; }
    if (d->code->next->next->next->expr->right) { print_error(test_type, "NULL", "expr* d->code->next->next->next->expr->right"); overall_status = FAILURE; }
    if (d->code->next->next->next->expr->name) { print_error(test_type, "NULL", "char* d->code->next->next->next->expr->name"); overall_status = FAILURE; }
    if (d->code->next->next->next->expr->literal_value != 0) {
      sprintf(kind_expect, "%d", 0); sprintf(kind_actual, "%ld", d->code->next->next->next->expr->literal_value);
      print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
    }
    if (d->code->next->next->next->expr->string_literal) { print_error(test_type, "NULL", "char* d->code->next->next->next->expr->string_literal"); overall_status = FAILURE; }
  }
  decl_destroy(&d);
  return overall_status;
}
