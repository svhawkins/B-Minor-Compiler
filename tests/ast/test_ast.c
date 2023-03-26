#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../source/decl.h"
#include "../../source/expr.h"
#include "../../source/param_list.h"
#include "../../source/stmt.h"
#include "../../source/symbol.h"
#include "../../source/type.h"
#define MAX_BUFFER 256

extern FILE* yyin;
extern void yyrestart();
extern int yyparse();
typedef enum {FAILURE = 0, SUCCESS = 1} Status;
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

int main(int argc, const char* argv[]) {
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
    test_expr_create_str
  };
  int n_tests = sizeof(tests)/sizeof(tests[0]);
  int n_pass = 0;

  printf("Running %d tests...\n", n_tests);
  for (int i = 0; i < n_tests; i++) { if (tests[i]()) { n_pass++; }}

  printf("Passed: %d/%d\n", n_pass, n_tests);
  printf("Failed: %d/%d\n", (n_tests - n_pass), n_tests);
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
  Status overall_status = SUCCESS, status;
  struct decl* d = decl_create(NULL, NULL, NULL, NULL, NULL);
  if (!d) { print_error(test_type, "NOT NULL", "decl d"); overall_status = FAILURE; }
  else {
    if (d->name) { print_error(test_type, "NULL", "char* d->name"); overall_status = FAILURE; }
    if (d->type) { print_error(test_type, "NULL", "type* d->type"); overall_status = FAILURE; }
    if (d->value) { print_error(test_type, "NULL", "expr* d->value"); overall_status = FAILURE; }
    if (d->code) { print_error(test_type, "NULL", "stmt* d->code"); overall_status = FAILURE; }
    if (d->next) { print_error(test_type, "NULL", "decl* d->next"); overall_status = FAILURE; }
  }
  return overall_status;
}

Status test_decl_create_name(void) {
  strcpy(test_type, "Testing: decl_create, non-NULL name");
  Status overall_status = SUCCESS, status;
  struct decl* d = decl_create("foo", NULL, NULL, NULL, NULL);
  if (!d) { print_error(test_type, "NOT NULL", "decl d"); overall_status = FAILURE; }
  else {
    if (strcmp(d->name, "foo")) { print_error(test_type, "foo", "char* d->name"); overall_status = FAILURE; }
    if (d->type) { print_error(test_type, "NULL", "type* d->type"); overall_status = FAILURE; }
    if (d->value) { print_error(test_type, "NULL", "expr* d->value"); overall_status = FAILURE; }
    if (d->code) { print_error(test_type, "NULL", "stmt* d->code"); overall_status = FAILURE; }
    if (d->next) { print_error(test_type, "NULL", "decl* d->next"); overall_status = FAILURE; }
  }
  return overall_status;
}


Status test_param_list_create_null(void) {
  strcpy(test_type, "Testing: param_list_create, all NULL");
  Status overall_status = SUCCESS, status;
  struct param_list* p = param_list_create(NULL, NULL, NULL);
  if (!p) { print_error(test_type, "NOT NULL", "param_list p"); overall_status = FAILURE; }
  else {
    if (p->name) { print_error(test_type, "NULL", "char* p->name"); overall_status = FAILURE; }
    if (p->type) { print_error(test_type, "NULL", "type* p->type"); overall_status = FAILURE; }
    if (p->next) { print_error(test_type, "NULL", "param_list* p->next"); overall_status = FAILURE; }
  }
  return overall_status;
}

Status test_param_list_create_name(void) {
  strcpy(test_type, "Testing: param_list_create_name, non-NULL name");
  Status overall_status = SUCCESS, status;
  struct param_list* p = param_list_create("foo", NULL, NULL);
  if (!p) { print_error(test_type, "NOT NULL", "param_list p"); overall_status = FAILURE; }
  else {
    if (strcmp(p->name, "foo")) { print_error(test_type, "foo", "char* p->name"); overall_status = FAILURE; }
    if (p->type) { print_error(test_type, "NULL", "type* p->type"); overall_status = FAILURE; }
    if (p->next) { print_error(test_type, "NULL", "param_list* p->next"); overall_status = FAILURE; }
  }
  return overall_status;
}

Status test_stmt_create_kind(void) {
  strcpy(test_type, "Testing: test_stmt_create_kind");
  Status overall_status = SUCCESS, status;
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
  }
  return overall_status;
}


Status test_type_create_kind(void) {
  strcpy(test_type, "Testing: test_type_create_kind");
  Status overall_status = SUCCESS, status;
  char kind_expect[3]; char kind_actual[3];
  for (type_t kind = TYPE_VOID; kind <= TYPE_FUNCTION; kind++) {
    struct type* t = type_create(kind, NULL, NULL);
    if (!t) { print_error(test_type, "NOT NULL", "type t"); overall_status = FAILURE; }
    else {
      if (t->kind != kind) {
        sprintf(kind_expect, "%d", kind); sprintf(kind_actual, "%d", t->kind);
        print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
      }
      if (t->subtype) { print_error(test_type, "NULL", "type* t->subtype"); overall_status = FAILURE; }
      if (t->params) { print_error(test_type, "NULL", "param_list* params"); overall_status = FAILURE; }
    }
  }
  return overall_status;
}


Status test_expr_create_kind(void) {
  strcpy(test_type, "Testing: test_expr_create_kind");
  Status overall_status = SUCCESS, status;
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
        sprintf(kind_expect, "%d", 0); sprintf(kind_actual, "%d", e->literal_value);
        print_error(test_type, kind_expect, kind_actual); overall_status = FAILURE;
      }
      if (e->string_literal) { print_error(test_type, "NULL", "char* e->string_literal"); overall_status = FAILURE; }
      if (e->symbol) { print_error(test_type, "NULL", "symbol* e->symbol"); overall_status = FAILURE; }
    }
  }
  return overall_status;
}

Status test_expr_create_name(void) {
  strcpy(test_type, "Testing: test_expr_create_name");
  Status overall_status = SUCCESS;
  char expect[3], actual[3];
  struct expr* e = expr_create_name("foo");
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
      sprintf(expect, "%d", 0); sprintf(actual, "%d", e->literal_value);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->string_literal) { print_error(test_type, "NULL", "char* e->string_literal"); overall_status = FAILURE; }
    if (e->symbol) { print_error(test_type, "NULL", "symbol* e->symbol"); overall_status = FAILURE; }
  }
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
      sprintf(expect, "%d", -493); sprintf(actual, "%d", e->literal_value);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->string_literal) { print_error(test_type, "NULL", "char* e->string_literal"); overall_status = FAILURE; }
    if (e->symbol) { print_error(test_type, "NULL", "symbol* e->symbol"); overall_status = FAILURE; }
  }
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
      sprintf(expect, "%d", 1); sprintf(actual, "%d", e->literal_value);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->string_literal) { print_error(test_type, "NULL", "char* e->string_literal"); overall_status = FAILURE; }
    if (e->symbol) { print_error(test_type, "NULL", "symbol* e->symbol"); overall_status = FAILURE; }
  }
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
    if (e->literal_value != (*(int*)&val)) {
      sprintf(expect, "%c", val); sprintf(actual, "%c", e->literal_value);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (e->string_literal) { print_error(test_type, "NULL", "char* e->string_literal"); overall_status = FAILURE; }
    if (e->symbol) { print_error(test_type, "NULL", "symbol* e->symbol"); overall_status = FAILURE; }
  }
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
      sprintf(expect, "%d", 0); sprintf(actual, "%d", e->literal_value);
      print_error(test_type, expect, actual); overall_status = FAILURE;
    }
    if (strcmp("foo", e->string_literal)) { print_error(test_type, "foo", "char* e->string_literal"); overall_status = FAILURE; }
    if (e->symbol) { print_error(test_type, "NULL", "symbol* e->symbol"); overall_status = FAILURE; }
  }
  return overall_status;
}
