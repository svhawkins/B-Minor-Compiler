#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../src/symbol_table.h"
#define MAX_BUFFER 256

typedef enum { FAILURE = 0, SUCCESS = 1 } Status;
char test_type[MAX_BUFFER];
char output[MAX_BUFFER];
extern FILE* ERR_OUT;

// helper functions
void print_error(char* test, char* expect, char* value);

// name resolution tests (which uses the symbol table)

// resolution for expressions
Status test_expr_resolve_name(void);
Status test_expr_resolve_binary_op(void);
Status test_expr_resolve_sym_undef(void);
Status test_expr_resolve_hidden(void);

// resolution for declarations
Status test_decl_resolve_atomic_uninit(void);
Status test_decl_resolve_atomic_init(void);
Status test_decl_resolve_function_uninit_no_param(void);
Status test_decl_resolve_function_uninit_one_param(void);
Status test_decl_resolve_function_uninit_many_param(void);
Status test_decl_resolve_sym_redef(void);
Status test_decl_resolve_function_sym_redef_undef_undef(void);
Status test_decl_resolve_function_sym_redef_undef_def(void);
Status test_decl_resolve_function_sym_redef_def_undef(void);
Status test_decl_resolve_function_sym_redef_def_def(void);
Status test_decl_resolve_function_param_redef(void);
Status test_decl_resolve_function_param_mismatch(void);
Status test_decl_resolve_var_sym_redef_def_undef(void);
Status test_decl_resolve_var_sym_redef_def_def(void);
Status test_decl_resolve_var_sym_redef_undef_undef(void);
Status test_decl_resolve_nonconst_array_size(void);
Status test_decl_resolve_nonconst_global(void);
Status test_decl_resolve_nonconst_global_array(void);

// resolution for statements
Status test_stmt_resolve_expr(void);
Status test_stmt_resolve_print(void);
Status test_stmt_resolve_return(void);
Status test_stmt_resolve_decl(void);
Status test_stmt_resolve_block_empty(void);
Status test_stmt_resolve_block(void);
Status test_stmt_resolve_while(void);
Status test_stmt_resolve_if_else_null(void);
Status test_stmt_resolve_if_else(void);
Status test_stmt_resolve_for_expr(void);
Status test_stmt_resolve_for_decl(void);
Status test_stmt_resolve_shadow_no_redef(void);
Status test_decl_resolve_program(void);
Status test_decl_resolve_which_scope_enter(void);
Status test_stmt_resolve_which_scope_exit(void);

int main(void) {
  ERR_OUT = fopen("error_output_st.txt", "w");
  if (!ERR_OUT) { ERR_OUT = stderr; }

  Status (*tests[])(void) = {
    test_expr_resolve_name,
    test_expr_resolve_binary_op,
    test_expr_resolve_sym_undef,
    test_expr_resolve_hidden,
    test_decl_resolve_atomic_uninit,
    test_decl_resolve_atomic_init,
    test_decl_resolve_function_uninit_no_param,
    test_decl_resolve_function_uninit_one_param,
    test_decl_resolve_function_uninit_many_param,
    test_decl_resolve_sym_redef,
    test_decl_resolve_function_param_redef,
    test_decl_resolve_function_sym_redef_undef_undef,
    test_decl_resolve_function_sym_redef_undef_def,
    test_decl_resolve_function_sym_redef_def_undef,
    test_decl_resolve_function_sym_redef_def_def,
    test_decl_resolve_function_param_mismatch,
    test_decl_resolve_var_sym_redef_def_undef,
    test_decl_resolve_var_sym_redef_def_def,
    test_decl_resolve_var_sym_redef_undef_undef,
    test_stmt_resolve_expr,
    test_stmt_resolve_print,
    test_stmt_resolve_return,
    test_stmt_resolve_decl,
    test_stmt_resolve_block_empty,
    test_stmt_resolve_block,
    test_stmt_resolve_while,
    test_stmt_resolve_if_else_null,
    test_stmt_resolve_if_else,
    test_stmt_resolve_for_expr,
    test_stmt_resolve_for_decl,
    test_stmt_resolve_shadow_no_redef,
    test_decl_resolve_program,
    test_decl_resolve_which_scope_enter,
    test_stmt_resolve_which_scope_exit,
    test_decl_resolve_nonconst_array_size,
    test_decl_resolve_nonconst_global,
    test_decl_resolve_nonconst_global_array
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
// pointers so use strings for expected types
void print_error(char* test, char* expect, char* value) { printf("Error:\t[%s]:\n\tExpected a value of %s for value: [%s]\n", test, expect, value); }

Status test_expr_resolve_name(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct expr* e = expr_create_name(strdup("x"));
  struct symbol* s = symbol_create(SYMBOL_GLOBAL, type_copy(integer), strdup("x"));

  Symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  symbol_table_scope_bind(st, "x", s);
  expr_resolve(st, e);

  symbol_table_scope_lookup(st, "x");
  if (!e->symbol) { print_error(test_type, "NOT NULL", "symbol_table_scope_lookup(Symbol_table* st)"); return FAILURE; }
  if (strcmp(e->symbol->name, e->name)) { print_error(test_type, "0", "strcmp(ret->name, e->name)"); status = FAILURE; }
  if (!type_equals(integer, e->symbol->type)) { print_error(test_type, "true", "type_equals(integer, ret->type)"); status = FAILURE; }
  if (e->symbol->kind != SYMBOL_GLOBAL) { print_error(test_type, "SYMBOL_GLOBAL", "int ret->kind"); status = FAILURE; }
  symbol_table_destroy(&st); expr_destroy(&e); type_destroy(&integer);
  return status;
}

Status test_expr_resolve_binary_op(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct expr* x = expr_create_name(strdup("x"));
  struct symbol* sx = symbol_create(SYMBOL_GLOBAL, type_copy(integer), strdup("x"));
  struct expr* y = expr_create_name(strdup("y"));
  struct symbol* sy = symbol_create(SYMBOL_GLOBAL, type_copy(integer), strdup("y"));
  struct expr* e = expr_create(EXPR_ADD, x, y);

  Symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  symbol_table_scope_bind(st, "x", sx);
  symbol_table_scope_bind(st, "y", sy);
  expr_resolve(st, e);

  symbol_table_scope_lookup(st, "x");
  if (!x->symbol) { print_error(test_type, "NOT NULL", "symbol_table_scope_lookup(Symbol_table* st)"); return FAILURE; }
  if (strcmp(x->symbol->name, x->name)) { print_error(test_type, "0", "strcmp(ret->name, e->name) [x]"); status = FAILURE; }
  if (!type_equals(integer, x->symbol->type)) { print_error(test_type, "true", "type_equals(integer, ret->type)"); status = FAILURE; }
  if (x->symbol->kind != SYMBOL_GLOBAL) { print_error(test_type, "SYMBOL_GLOBAL", "int ret->kind"); status = FAILURE; }

  symbol_table_scope_lookup(st, "y");
  if (!y->symbol) { print_error(test_type, "NOT NULL", "symbol_table_scope_lookup(Symbol_table* st)"); return FAILURE; }
  if (strcmp(y->symbol->name, y->name)) { print_error(test_type, "0", "strcmp(ret->name, e->name) [y]"); status = FAILURE; }
  if (!type_equals(integer, y->symbol->type)) { print_error(test_type, "true", "type_equals(integer, ret->type)"); status = FAILURE; }
  if (y->symbol->kind != SYMBOL_GLOBAL) { print_error(test_type, "SYMBOL_GLOBAL", "int ret->kind"); status = FAILURE; }

  symbol_table_destroy(&st); expr_destroy(&e); type_destroy(&integer);
  return status;
}

Status test_decl_resolve_atomic_uninit(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct decl* d = decl_create(strdup("x"), type_copy(integer), NULL, NULL, NULL);

  Symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  decl_resolve(st, d);

  if (!symbol_table_scope_lookup(st, "x")) { print_error(test_type, "NOT NULL", "symbol_table_scope_lookup(Symbol_table* st)"); return FAILURE; }
  if (!d->symbol) { print_error(test_type, "NOT NULL", "symbol_table_scope_lookup(Symbol_table* st)"); return FAILURE; }
  if (strcmp(d->symbol->name, d->name)) { print_error(test_type, "0", "strcmp(ret->name, e->name) [x]"); status = FAILURE; }
  if (!type_equals(integer, d->symbol->type)) { print_error(test_type, "true", "type_equals(integer, ret->type)"); status = FAILURE; }
  if (d->symbol->kind != SYMBOL_GLOBAL) { print_error(test_type, "SYMBOL_GLOBAL", "int ret->kind"); status = FAILURE; }

  symbol_table_destroy(&st); decl_destroy(&d); type_destroy(&integer);
  return status;
}

Status test_decl_resolve_atomic_init(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct expr* y = expr_create_name(strdup("y"));
  struct decl* d = decl_create(strdup("x"), type_copy(integer), y, NULL, NULL);
  struct symbol* sy = symbol_create(SYMBOL_GLOBAL, type_copy(integer), strdup("y"));

  char* keyname = strdup("y");
  Symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  symbol_table_scope_bind(st, keyname, sy);
  decl_resolve(st, d);

  symbol_table_scope_lookup(st, "x");
  if (!d->symbol) { print_error(test_type, "NOT NULL", "symbol_table_scope_lookup(Symbol_table* st)"); return FAILURE; }
  if (strcmp(d->symbol->name, d->name)) { print_error(test_type, "0", "strcmp(ret->name, e->name) [x]"); status = FAILURE; }
  if (!type_equals(integer, d->symbol->type)) { print_error(test_type, "true", "type_equals(integer, ret->type)"); status = FAILURE; }
  if (d->symbol->kind != SYMBOL_GLOBAL) { print_error(test_type, "SYMBOL_GLOBAL", "int ret->kind"); status = FAILURE; }

  symbol_table_scope_lookup(st, "y");
  if (!y->symbol) { print_error(test_type, "NOT NULL", "symbol_table_scope_lookup(Symbol_table* st)"); return FAILURE; }
  if (strcmp(y->symbol->name, y->name)) { print_error(test_type, "0", "strcmp(ret->name, e->name) [y]"); status = FAILURE; }
  if (!type_equals(integer, y->symbol->type)) { print_error(test_type, "true", "type_equals(integer, ret->type)"); status = FAILURE; }
  if (y->symbol->kind != SYMBOL_GLOBAL) { print_error(test_type, "SYMBOL_GLOBAL", "int ret->kind"); status = FAILURE; }

  symbol_table_destroy(&st); decl_destroy(&d); type_destroy(&integer);
  free(keyname);
  return status;
}

Status test_decl_resolve_function_uninit_no_param(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* fvoid = type_create(TYPE_FUNCTION, type_create(TYPE_VOID, NULL, NULL, NULL), NULL, NULL);
  struct decl* d = decl_create(strdup("foo"), type_copy(fvoid), NULL, NULL, NULL);

  Symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  decl_resolve(st, d);

  if (!symbol_table_scope_lookup(st, "foo")) { print_error(test_type, "NOT NULL", "symbol_table_scope_lookup(Symbol_table* st)"); return FAILURE; }
  if (!d->symbol) { print_error(test_type, "NOT NULL", "symbol_table_scope_lookup(Symbol_table* st)"); return FAILURE; }
  if (strcmp(d->symbol->name, d->name)) { print_error(test_type, "0", "strcmp(d->symbol->name, d->name) [foo]"); status = FAILURE; }
  if (!type_equals(fvoid, d->symbol->type)) { print_error(test_type, "true", "type_equals(fvoid, d->symbol->type)"); status = FAILURE; }
  if (d->symbol->kind != SYMBOL_GLOBAL) { print_error(test_type, "SYMBOL_GLOBAL", "int d->symbol->kind"); status = FAILURE; }

  symbol_table_destroy(&st); decl_destroy(&d); type_destroy(&fvoid);
  return status;
}

Status test_decl_resolve_function_uninit_one_param(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct param_list* p = param_list_create(strdup("x"), type_create(TYPE_INTEGER, NULL, NULL, NULL), NULL);
  struct type* fvoid_p = type_create(TYPE_FUNCTION, type_create(TYPE_VOID, NULL, NULL, NULL), p, NULL);
  struct decl* d = decl_create(strdup("foo"), type_copy(fvoid_p), NULL, NULL, NULL);

  Symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  decl_resolve(st, d);

  if (!symbol_table_scope_lookup(st, "foo")) { print_error(test_type, "NOT NULL", "symbol_table_scope_lookup(Symbol_table* st)"); return FAILURE; }
  if (!d->symbol) { print_error(test_type, "NOT NULL", "symbol_table_scope_lookup(Symbol_table* st)"); return FAILURE; }
  if (strcmp(d->symbol->name, d->name)) { print_error(test_type, "0", "strcmp(d->symbol->name, d->name) [foo]"); status = FAILURE; }
  if (!type_equals(fvoid_p, d->symbol->type)) { print_error(test_type, "true", "type_equals(fvoid, d->symbol->type)"); status = FAILURE; }
  if (d->symbol->kind != SYMBOL_GLOBAL) { print_error(test_type, "SYMBOL_GLOBAL", "int d->symbol->kind"); status = FAILURE; }

  symbol_table_destroy(&st); decl_destroy(&d); type_destroy(&fvoid_p);
  return status;
}

Status test_decl_resolve_function_uninit_many_param(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;

  struct type* targv = type_create(TYPE_ARRAY, type_create(TYPE_STRING, NULL, NULL, NULL), NULL, NULL);
  struct param_list* pend = param_list_create(strdup("argv"), targv, NULL);
  struct param_list* p = param_list_create(strdup("argc"), type_create(TYPE_INTEGER, NULL, NULL, NULL), pend);
  struct type* t = type_create(TYPE_FUNCTION, type_create(TYPE_INTEGER, NULL, NULL, NULL), p, NULL);
  struct decl* d = decl_create(strdup("main"), type_copy(t), NULL, NULL, NULL);

  Symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  decl_resolve(st, d);

  if (!symbol_table_scope_lookup(st, "main")) { print_error(test_type, "NOT NULL", "symbol_table_scope_lookup(Symbol_table* st)"); return FAILURE; }
  if (!d->symbol) { print_error(test_type, "NOT NULL", "symbol_table_scope_lookup(Symbol_table* st)"); return FAILURE; }
  if (strcmp(d->symbol->name, d->name)) { print_error(test_type, "0", "strcmp(d->symbol->name, d->name) [main]"); status = FAILURE; }
  if (!type_equals(t, d->symbol->type)) { print_error(test_type, "true", "type_equals(fvoid, d->symbol->type)"); status = FAILURE; }
  if (d->symbol->kind != SYMBOL_GLOBAL) { print_error(test_type, "SYMBOL_GLOBAL", "int d->symbol->kind"); status = FAILURE; }

  symbol_table_destroy(&st); decl_destroy(&d); type_destroy(&t);
  return status;
}

Status test_stmt_resolve_expr(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* tvoid = type_create(TYPE_VOID, NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_EXPR, NULL, NULL, expr_create_name(strdup("duck")), NULL, NULL, NULL, NULL);
  struct symbol* sym = symbol_create(SYMBOL_GLOBAL, type_copy(tvoid), strdup("duck"));

  Symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  symbol_table_scope_bind(st, "duck", sym);
  stmt_resolve(st, s);

  symbol_table_scope_lookup(st, "duck");
  if (!s->expr->symbol) { print_error(test_type, "NOT NULL", "symbol_table_scope_lookup(Symbol_table* st)"); return FAILURE; }
  if (strcmp(s->expr->symbol->name, s->expr->name)) { print_error(test_type, "0", "strcmp(expr->name, symbol->name)"); status = FAILURE; }
  if (!type_equals(tvoid, s->expr->symbol->type)) { print_error(test_type, "true", "type_equals(tvoid, symbol->type)"); status = FAILURE; }
  if (s->expr->symbol->kind != SYMBOL_GLOBAL) { print_error(test_type, "SYMBOL_GLOBAL", "symbol->kind"); status = FAILURE; }
  symbol_table_destroy(&st); stmt_destroy(&s); type_destroy(&tvoid);
  return status;
}

Status test_stmt_resolve_print(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* tvoid = type_create(TYPE_VOID, NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_PRINT, NULL, NULL, expr_create_name(strdup("duck")), NULL, NULL, NULL, NULL);
  struct symbol* sym = symbol_create(SYMBOL_GLOBAL, type_copy(tvoid), strdup("duck"));

  Symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  symbol_table_scope_bind(st, "duck", sym);
  stmt_resolve(st, s);

  symbol_table_scope_lookup(st, "duck");
  if (!s->expr->symbol) { print_error(test_type, "NOT NULL", "symbol_table_scope_lookup(Symbol_table* st)"); return FAILURE; }
  if (strcmp(s->expr->symbol->name, s->expr->name)) { print_error(test_type, "0", "strcmp(expr->name, symbol->name)"); status = FAILURE; }
  if (!type_equals(tvoid, s->expr->symbol->type)) { print_error(test_type, "true", "type_equals(tvoid, symbol->type)"); status = FAILURE; }
  if (s->expr->symbol->kind != SYMBOL_GLOBAL) { print_error(test_type, "SYMBOL_GLOBAL", "symbol->kind"); status = FAILURE; }
  symbol_table_destroy(&st); stmt_destroy(&s); type_destroy(&tvoid);
  return status;
}

Status test_stmt_resolve_return(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* tvoid = type_create(TYPE_VOID, NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_RETURN, NULL, NULL, expr_create_name(strdup("duck")), NULL, NULL, NULL, NULL);
  struct symbol* sym = symbol_create(SYMBOL_GLOBAL, type_copy(tvoid), strdup("duck"));

  Symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  symbol_table_scope_bind(st, "duck", sym);
  stmt_resolve(st, s);

  symbol_table_scope_lookup(st, "duck");
  if (!s->expr->symbol) { print_error(test_type, "NOT NULL", "symbol_table_scope_lookup(Symbol_table* st)"); return FAILURE; }
  if (strcmp(s->expr->symbol->name, s->expr->name)) { print_error(test_type, "0", "strcmp(expr->name, symbol->name)"); status = FAILURE; }
  if (!type_equals(tvoid, s->expr->symbol->type)) { print_error(test_type, "true", "type_equals(tvoid, symbol->type)"); status = FAILURE; }
  if (s->expr->symbol->kind != SYMBOL_GLOBAL) { print_error(test_type, "SYMBOL_GLOBAL", "symbol->kind"); status = FAILURE; }
  symbol_table_destroy(&st); stmt_destroy(&s); type_destroy(&tvoid);
  return status;
}

Status test_stmt_resolve_decl(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* tvoid = type_create(TYPE_VOID, NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_DECL, decl_create(strdup("duck"), type_copy(tvoid), NULL, NULL, NULL), NULL, NULL, NULL, NULL, NULL, NULL);

  Symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  stmt_resolve(st, s);

  symbol_table_scope_lookup(st, "duck");
  if (!s->decl->symbol) { print_error(test_type, "NOT NULL", "symbol_table_scope_lookup(Symbol_table* st)"); return FAILURE; }
  if (strcmp(s->decl->symbol->name, s->decl->name)) { print_error(test_type, "0", "strcmp(decl->name, symbol->name)"); status = FAILURE; }
  if (!type_equals(tvoid, s->decl->symbol->type)) { print_error(test_type, "true", "type_equals(tvoid, symbol->type)"); status = FAILURE; }
  if (s->decl->symbol->kind != SYMBOL_GLOBAL) { print_error(test_type, "SYMBOL_GLOBAL", "symbol->kind"); status = FAILURE; }
  symbol_table_destroy(&st); stmt_destroy(&s); type_destroy(&tvoid);
  return status;
}

Status test_stmt_resolve_block_empty(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct stmt* s = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  Symbol_table* st = symbol_table_verbose_create();
  symbol_table_scope_enter(st); int old_level = vector_size(st->vector);
  stmt_resolve(st, s);

  if ((vector_size(st->vector) - old_level) != 1) { print_error(test_type, "2", "scope level"); status = FAILURE; }
  if (hash_table_size(st->vector->items[vector_size(st->vector) - 1])) { print_error(test_type, "0", "topmost hashtable size"); status = FAILURE; }
  symbol_table_destroy(&st); stmt_destroy(&s);
  return status;
}

Status test_stmt_resolve_block(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* tvoid = type_create(TYPE_VOID, NULL, NULL, NULL);
  struct stmt* body = stmt_create(STMT_DECL, decl_create(strdup("duck"), type_copy(tvoid), NULL, NULL, NULL), NULL, NULL, NULL, NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, body, NULL, NULL);
  Symbol_table* st = symbol_table_verbose_create();
  symbol_table_scope_enter(st); int old_level = vector_size(st->vector);
  stmt_resolve(st, s);
  if ((vector_size(st->vector) - old_level) != 1) { print_error(test_type, "2", "scope level"); status = FAILURE; }
  if (!hash_table_size(st->vector->items[vector_size(st->vector) - 1])) { print_error(test_type, "1", "topmost hashtable size"); status = FAILURE; }
  if (!symbol_table_scope_lookup_at(st, "duck", old_level)) { print_error(test_type, "NOT NULL", "current scope lookup [duck]"); status = FAILURE; }
  if (s->body->decl->symbol->kind != SYMBOL_LOCAL) { print_error(test_type, "SYMBOL_LOCAL", "symbol kind [duck]"); status = FAILURE; }
  symbol_table_destroy(&st); stmt_destroy(&s); type_destroy(&tvoid);
  return status;
}

Status test_stmt_resolve_while(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct expr* e = expr_create_name(strdup("duck"));
  struct stmt* body = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_WHILE, NULL, NULL, e, NULL, body, NULL, NULL);
  struct symbol* sym = symbol_create(SYMBOL_GLOBAL, type_create(TYPE_VOID, NULL, NULL, NULL), strdup("duck"));

  Symbol_table* st = symbol_table_verbose_create();
  symbol_table_scope_enter(st); int old_level = vector_size(st->vector);
  symbol_table_scope_bind(st, "duck", sym);
  stmt_resolve(st, s);

  if ((vector_size(st->vector) - old_level) != 1) { print_error(test_type, "2", "scope level"); status = FAILURE; }
  if (hash_table_size(st->vector->items[vector_size(st->vector) - 1])) { print_error(test_type, "0", "topmost hashtable size"); status = FAILURE; }
  if (!symbol_table_scope_lookup(st, "duck")) { print_error(test_type, "NOT NULL", "scope lookup [duck]"); status = FAILURE; }
  if (symbol_table_scope_lookup_at(st, "duck", old_level)) { print_error(test_type, "NULL", "current scope lookup [duck]"); status = FAILURE; }
  symbol_table_destroy(&st); stmt_destroy(&s);
  return status;
}

Status test_stmt_resolve_if_else_null(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct expr* e = expr_create_name(strdup("duck"));
  struct stmt* body = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_IF_ELSE, NULL, NULL, e, NULL, body, NULL, NULL);
  struct symbol* sym = symbol_create(SYMBOL_GLOBAL, type_create(TYPE_VOID, NULL, NULL, NULL), strdup("duck"));

  Symbol_table* st = symbol_table_verbose_create();
  symbol_table_scope_enter(st); int old_level = vector_size(st->vector);
  symbol_table_scope_bind(st, "duck", sym);
  stmt_resolve(st, s);

  if ((vector_size(st->vector) - old_level) != 1) { print_error(test_type, "2", "scope level"); status = FAILURE; }
  if (hash_table_size(st->vector->items[vector_size(st->vector) - 1])) { print_error(test_type, "0", "topmost hashtable size"); status = FAILURE; }
  if (!symbol_table_scope_lookup(st, "duck")) { print_error(test_type, "NOT NULL", "scope lookup [duck]"); status = FAILURE; }
  if (symbol_table_scope_lookup_at(st, "duck", old_level)) { print_error(test_type, "NULL", "current scope lookup [duck]"); status = FAILURE; }
  symbol_table_destroy(&st); stmt_destroy(&s);
  return status;
}

Status test_stmt_resolve_if_else(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct expr* e = expr_create_name(strdup("duck"));
  struct stmt* body = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  struct stmt* else_body = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_IF_ELSE, NULL, NULL, e, NULL, body, else_body, NULL);
  struct symbol* sym = symbol_create(SYMBOL_GLOBAL, type_create(TYPE_VOID, NULL, NULL, NULL), strdup("duck"));

  Symbol_table* st = symbol_table_verbose_create();
  symbol_table_scope_enter(st); int old_level = vector_size(st->vector);
  symbol_table_scope_bind(st, "duck", sym);
  stmt_resolve(st, s);
  if ((vector_size(st->vector) - old_level) != 2) { print_error(test_type, "3", "scope level"); status = FAILURE; }
  if (hash_table_size(st->vector->items[vector_size(st->vector) - 1])) { print_error(test_type, "0", "topmost hashtable size"); status = FAILURE; }
  if (!symbol_table_scope_lookup(st, "duck")) { print_error(test_type, "NOT NULL", "scope lookup [duck]"); status = FAILURE; }
  if (symbol_table_scope_lookup_at(st, "duck", old_level)) { print_error(test_type, "NULL", "current scope lookup [duck]"); status = FAILURE; }
  symbol_table_destroy(&st); stmt_destroy(&s);
  return status;
}

Status test_stmt_resolve_for_expr(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct expr* duck = expr_create_name(strdup("duck"));
  struct expr* duck2 = expr_create_name(strdup("duck"));
  struct expr* goose = expr_create_name(strdup("goose"));
  struct stmt* body = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_FOR, NULL, duck, duck2, goose, body, NULL, NULL);
  struct symbol* sym_duck = symbol_create(SYMBOL_GLOBAL, type_create(TYPE_VOID, NULL, NULL, NULL), strdup("duck"));
  struct symbol* sym_goose = symbol_create(SYMBOL_GLOBAL, type_create(TYPE_VOID, NULL, NULL, NULL), strdup("goose"));

  Symbol_table* st = symbol_table_verbose_create();
  symbol_table_scope_enter(st); int old_level = vector_size(st->vector);
  symbol_table_scope_bind(st, "duck", sym_duck);
  symbol_table_scope_bind(st, "goose", sym_goose);
  stmt_resolve(st, s);

  if ((vector_size(st->vector) - old_level) != 1) { print_error(test_type, "2", "scope level"); status = FAILURE; }
  if (hash_table_size(st->vector->items[vector_size(st->vector) - 1])) { print_error(test_type, "0", "topmost hashtable size"); status = FAILURE; }
  if (!symbol_table_scope_lookup(st, "duck")) { print_error(test_type, "NOT NULL", "scope lookup [duck]"); status = FAILURE; }
  if (symbol_table_scope_lookup_at(st, "duck", old_level)) { print_error(test_type, "NULL", "current scope lookup [duck]"); status = FAILURE; }
  if (!symbol_table_scope_lookup(st, "goose")) { print_error(test_type, "NOT NULL", "scope lookup [goose]"); status = FAILURE; }
  if (symbol_table_scope_lookup_at(st, "goose", old_level)) { print_error(test_type, "NULL", "current scope lookup [goose]"); status = FAILURE; }
  symbol_table_destroy(&st);
  stmt_destroy(&s);
  return status;
}

Status test_stmt_resolve_for_decl(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct stmt* s = stmt_create(STMT_FOR, decl_create(strdup("duck"), type_create(TYPE_VOID, NULL, NULL, NULL), NULL, NULL, NULL),
				NULL, NULL, NULL,
 				stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
 				NULL, NULL);
  Symbol_table* st = symbol_table_verbose_create();
  symbol_table_scope_enter(st); int old_level = vector_size(st->vector);
  stmt_resolve(st, s);

  if ((vector_size(st->vector) - old_level) != 1) { print_error(test_type, "2", "scope level"); status = FAILURE; }
  if (hash_table_size(st->vector->items[vector_size(st->vector) - 1])) { print_error(test_type, "0", "topmost hashtable size"); status = FAILURE; }
  if (!symbol_table_scope_lookup(st, "duck")) { print_error(test_type, "NOT NULL", "scope lookup [duck]"); status = FAILURE; }
  if (symbol_table_scope_lookup_at(st, "duck", old_level)) { print_error(test_type, "NULL", "current scope lookup [duck]"); status = FAILURE; }
  symbol_table_destroy(&st); stmt_destroy(&s);
  return status;
}

Status test_decl_resolve_program(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  // components of statements for better readability
  struct param_list* pend = param_list_create(strdup("argv"), type_create(TYPE_ARRAY, type_create(TYPE_STRING, NULL, NULL, NULL), NULL, NULL), NULL);
  struct param_list* p = param_list_create(strdup("argc"), type_create(TYPE_INTEGER, NULL, NULL, NULL), pend);
  struct decl* i = decl_create(strdup("i"), type_create(TYPE_INTEGER, NULL, NULL, NULL), expr_create_integer_literal(0), NULL, NULL);

  // structs that make up stmts in code
  struct stmt* return_stmt = stmt_create(STMT_RETURN, NULL, NULL, expr_create_name(strdup("i")), NULL, NULL, NULL, NULL);
  struct stmt* i_init = stmt_create(STMT_DECL, i, NULL, NULL, NULL, NULL, NULL, return_stmt);
  struct stmt* s = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, i_init, NULL, NULL);
  struct decl* d = decl_create(strdup("main"), type_create(TYPE_FUNCTION, type_create(TYPE_INTEGER, NULL, NULL, NULL), p, NULL), NULL, s, NULL);


  Symbol_table* st = symbol_table_verbose_create();
  symbol_table_scope_enter(st);
  decl_resolve(st, d);

  if (vector_size(st->vector) != 3) { print_error(test_type, "3", "total tables"); status = FAILURE; }
  if (!symbol_table_scope_lookup_all(st, "main")) { print_error(test_type, "NOT NULL", "scope lookup [main]"); status = FAILURE; }
  if (!symbol_table_scope_lookup_all(st, "argc")) { print_error(test_type, "NOT NULL", "scope lookup [argc]"); status = FAILURE; }
  if (!symbol_table_scope_lookup_all(st, "argv")) { print_error(test_type, "NOT NULL", "scope lookup [argv]"); status = FAILURE; }
  if (!symbol_table_scope_lookup_all(st, "i")) { print_error(test_type, "NOT NULL", "scope lookup [i]"); status = FAILURE; }
  symbol_table_destroy(&st); decl_destroy(&d);
  return status;
}

Status test_expr_resolve_sym_undef(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct expr* e = expr_create_name(strdup("x"));

  Symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st); symbol_table_scope_enter(st);
  if (e->symbol) { print_error(test_type, "NULL", "e->symbol"); status = FAILURE; }
  if (symbol_table_scope_lookup(st, "x")) { print_error(test_type, "NULL", "struct symbol* symbol_table_scope_lookup(st, \"x\""); status = FAILURE; }

  error_status = expr_resolve(st, e);
  symbol_table_scope_lookup_current(st, "x");

  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != SYM_UNDEF) { print_error(test_type, "SYM_UNDEF", "int error_status"); status = FAILURE; }
  if (!e->symbol) { print_error(test_type, "NOT NULL", "symbol_table_scope_lookup(Symbol_table* st)"); return FAILURE; }
  if (strcmp(e->symbol->name, e->name)) { print_error(test_type, "0", "strcmp(e->symbol->name, e->name)"); status = FAILURE; }
  if (!type_equals(integer, e->symbol->type)) { print_error(test_type, "true", "type_equals(integer, ret->type)"); status = FAILURE; }
  if (e->symbol->kind != SYMBOL_LOCAL) { print_error(test_type, "SYMBOL_GLOBAL", "int ret->kind"); status = FAILURE; }
  symbol_table_destroy(&st); expr_destroy(&e); type_destroy(&integer);
  return status;
}

Status test_decl_resolve_sym_redef(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct type* character = type_create(TYPE_CHARACTER, NULL, NULL, NULL);
  struct decl* d = decl_create(strdup("x"), type_copy(integer), NULL, NULL, NULL);
  struct decl* d2 = decl_create(strdup("x"), type_copy(character), NULL, NULL, d);

  Symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  error_status = decl_resolve(st, d2);

  if (!symbol_table_scope_lookup(st, "x")) { print_error(test_type, "NOT NULL", "symbol_table_scope_lookup(Symbol_table* st)"); return FAILURE; }
  if (!d2->symbol) { print_error(test_type, "NOT NULL", "symbol_table_scope_lookup(Symbol_table* st)"); return FAILURE; }
  if (strcmp(d2->symbol->name, d2->name)) { print_error(test_type, "0", "strcmp(ret->name, e->name) [x]"); status = FAILURE; }
  if (!type_equals(character, d2->symbol->type)) { print_error(test_type, "true", "type_equals(character, ret->type)"); status = FAILURE; }
  if (d2->symbol->kind != SYMBOL_GLOBAL) { print_error(test_type, "SYMBOL_GLOBAL", "int ret->kind"); status = FAILURE; }

  if (d->symbol) { print_error(test_type, "NULL", "d->symbol"); status = FAILURE; }
  if (global_error_count != 1) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != SYM_REDEF) { print_error(test_type, "SYM_REDEF", "int error_status"); status = FAILURE; }
  symbol_table_destroy(&st); decl_destroy(&d2); type_destroy(&integer); type_destroy(&character);
  return status;
}

Status test_decl_resolve_function_param_redef(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct type* character = type_create(TYPE_CHARACTER, NULL, NULL, NULL);
  struct param_list* pend = param_list_create(strdup("x"), type_copy(integer), NULL);
  struct param_list* p = param_list_create(strdup("x"), type_copy(character), pend);
  struct type* tfuncv = type_create(TYPE_FUNCTION, type_create(TYPE_VOID, NULL, NULL, NULL), p, NULL);
  struct decl* d = decl_create(strdup("foo"), tfuncv, NULL, NULL, NULL);

  Symbol_table* st = symbol_table_verbose_create();
  symbol_table_scope_enter(st);
  error_status = decl_resolve(st, d);

  if (!symbol_table_scope_lookup(st, "foo")) { print_error(test_type, "NOT NULL", "symbol_table_scope_lookup(Symbol_table* st)"); return FAILURE; }
  if (!d->symbol) { print_error(test_type, "NOT NULL", "d->symbol"); return FAILURE; }
  if (strcmp(d->symbol->name, d->name)) { print_error(test_type, "0", "strcmp(ret->name, e->name) [x]"); status = FAILURE; }
  if (!type_equals(tfuncv, d->symbol->type)) { print_error(test_type, "true", "type_equals(character, ret->type)"); status = FAILURE; }
  if (d->symbol->kind != SYMBOL_GLOBAL) { print_error(test_type, "SYMBOL_GLOBAL", "int ret->kind"); status = FAILURE; }
  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status!= SYM_REDEF) { print_error(test_type, "SYM_REDEF", "int error_status"); status = FAILURE; }
  symbol_table_destroy(&st); decl_destroy(&d); type_destroy(&integer); type_destroy(&character);
  return status;
}

Status test_decl_resolve_function_sym_redef_undef_undef(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* tvoid = type_create(TYPE_VOID, NULL, NULL, NULL);
  struct type* tfuncv = type_create(TYPE_FUNCTION, type_copy(tvoid), NULL, NULL);
  struct decl* dend = decl_create(strdup("foo"), type_copy(tfuncv), NULL, NULL, NULL);
  struct decl* d = decl_create(strdup("foo"), type_copy(tfuncv), NULL, NULL, dend);

  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  error_status = decl_resolve(st, d);
  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != SYM_REDEF) { print_error(test_type, "SYM_REDEF", "int error_status"); status = FAILURE; }
  symbol_table_destroy(&st); type_destroy(&tfuncv); type_destroy(&tvoid); decl_destroy(&d);
  return status;
}

Status test_decl_resolve_function_sym_redef_undef_def(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* tvoid = type_create(TYPE_VOID, NULL, NULL, NULL);
  struct type* tfuncv = type_create(TYPE_FUNCTION, type_copy(tvoid), NULL, NULL);
  struct stmt* function_body = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  struct decl* dend = decl_create(strdup("foo"), type_copy(tfuncv), NULL, function_body, NULL);
  struct decl* d = decl_create(strdup("foo"), type_copy(tfuncv), NULL, NULL, dend);

  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  error_status = decl_resolve(st, d);

  if (global_error_count) { print_error(test_type, "0", "int global_error_count"); status = FAILURE; }
  if (error_status) { print_error(test_type, "0", "int error_status"); status = FAILURE; }
  symbol_table_destroy(&st);
  type_destroy(&tfuncv); type_destroy(&tvoid);
  decl_destroy(&d);
  return status;
}

Status test_decl_resolve_function_sym_redef_def_undef(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* tvoid = type_create(TYPE_VOID, NULL, NULL, NULL);
  struct type* tfuncv = type_create(TYPE_FUNCTION, type_copy(tvoid), NULL, NULL);
  struct stmt* function_body = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  struct decl* dend = decl_create(strdup("foo"), type_copy(tfuncv), NULL, NULL, NULL);
  struct decl* d = decl_create(strdup("foo"), type_copy(tfuncv), NULL, function_body, dend);

  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  error_status = decl_resolve(st, d);
  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != SYM_REDEF) { print_error(test_type, "SYM_REDEF", "int error_status"); status = FAILURE; }
  symbol_table_destroy(&st); type_destroy(&tfuncv); decl_destroy(&d); type_destroy(&tvoid);

  return status;
}

Status test_decl_resolve_function_sym_redef_def_def(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* tvoid = type_create(TYPE_VOID, NULL, NULL, NULL);
  struct type* tfuncv = type_create(TYPE_FUNCTION, type_copy(tvoid), NULL, NULL);
  struct stmt* function_body = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  struct stmt* function_body2 = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  struct decl* dend = decl_create(strdup("foo"), type_copy(tfuncv), NULL, function_body, NULL);
  struct decl* d = decl_create(strdup("foo"), type_copy(tfuncv), NULL, function_body2, dend);

  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  error_status = decl_resolve(st, d);
  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != SYM_REDEF) { print_error(test_type, "SYM_REDEF", "int error_status"); status = FAILURE; }
  symbol_table_destroy(&st); type_destroy(&tfuncv); decl_destroy(&d); type_destroy(&tvoid);
  return status;
}

Status test_stmt_resolve_shadow_no_redef(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* tvoid = type_create(TYPE_VOID, NULL, NULL, NULL);
  struct stmt* inner_decl = stmt_create(STMT_DECL, decl_create(strdup("duck"), type_copy(tvoid), NULL, NULL, NULL), NULL, NULL, NULL, NULL, NULL, NULL);
  struct stmt* inner_body = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, inner_decl, NULL, NULL);
  struct stmt* body = stmt_create(STMT_DECL, decl_create(strdup("duck"), type_copy(tvoid), NULL, NULL, NULL), NULL, NULL, NULL, NULL, NULL, inner_body);
  struct stmt* s = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, body, NULL, NULL);

  Symbol_table* st = symbol_table_verbose_create();
  symbol_table_scope_enter(st);
  error_status = stmt_resolve(st, s);
  if (global_error_count) { print_error(test_type,"0", "int global_error_count"); status = FAILURE; }
  if (!symbol_table_scope_lookup_at(st, "duck", 2)) { print_error(test_type, "NOT NULL", "topmost (2) scope lookup [duck]"); status = FAILURE; }
  if (!symbol_table_scope_lookup_at(st, "duck", 1)) { print_error(test_type, "NOT NULL", "previous (1) scope lookup [duck]"); status = FAILURE; }
  symbol_table_destroy(&st); stmt_destroy(&s); type_destroy(&tvoid);
  return status;
}

Status test_decl_resolve_function_param_mismatch(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* tvoid = type_create(TYPE_VOID, NULL, NULL, NULL);
  struct type* tfuncv = type_create(TYPE_FUNCTION, type_copy(tvoid), NULL, NULL);
  struct type* tfuncv2 = type_create(TYPE_FUNCTION, type_copy(tvoid), param_list_create(strdup("x"), type_copy(tvoid), NULL), NULL);
  struct stmt* function_body = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  struct decl* dend = decl_create(strdup("foo"), type_copy(tfuncv), NULL, function_body, NULL);
  struct decl* d = decl_create(strdup("foo"), type_copy(tfuncv2), NULL, NULL, dend);

  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  error_status = decl_resolve(st, d);
  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != SYM_PARAM) { print_error(test_type, "SYM_PARAM", "int error_status"); status = FAILURE; }
  symbol_table_destroy(&st); type_destroy(&tfuncv); type_destroy(&tvoid); type_destroy(&tfuncv2); decl_destroy(&d);
  return status;
}


Status test_decl_resolve_which_scope_enter(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* tvoid = type_create(TYPE_VOID, NULL, NULL, NULL);
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct param_list* p = param_list_create(strdup("x"), type_copy(integer), param_list_create(strdup("y"), type_copy(integer), NULL));
  struct type* tfuncv = type_create(TYPE_FUNCTION, type_copy(tvoid), p, NULL);
  struct stmt* s = stmt_create(STMT_DECL, decl_create(strdup("z"), type_copy(integer), NULL, NULL, NULL), NULL, NULL, NULL, NULL, NULL, NULL);
  struct stmt* function_body = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, s, NULL, NULL);
  struct decl* d = decl_create(strdup("foo"), type_copy(tfuncv), NULL, function_body, NULL);

  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  error_status = decl_resolve(st, d);

  struct symbol* sym_x = symbol_table_scope_lookup_all(st, "x");
  struct symbol* sym_y = symbol_table_scope_lookup_all(st, "y");
  struct symbol* sym_z = symbol_table_scope_lookup_all(st, "z");
  if (!sym_x) { print_error(test_type, "NOT NULL", "struct symbol* sym_x"); return FAILURE; }
  if (!sym_y) { print_error(test_type, "NOT NULL", "struct symbol* sym_y"); return FAILURE; }
  if (!sym_z) { print_error(test_type, "NOT NULL", "struct symbol* sym_z"); return FAILURE; }
  if (sym_x->which != 0) { print_error(test_type, "0", "int sym_x->which"); status = FAILURE; }
  if (sym_y->which != 1) { print_error(test_type, "1", "int sym_y->which"); status = FAILURE; }
  if (sym_z->which != 2) { print_error(test_type, "2", "int sym_z->which"); status = FAILURE; }

  symbol_table_destroy(&st); type_destroy(&tfuncv); type_destroy(&tvoid); type_destroy(&integer); decl_destroy(&d);
  return status;
}

Status test_stmt_resolve_which_scope_exit(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* tvoid = type_create(TYPE_VOID, NULL, NULL, NULL);
  struct stmt* stend = stmt_create(STMT_DECL, decl_create(strdup("goose"), type_copy(tvoid), NULL, NULL, NULL), NULL, NULL, NULL, NULL, NULL, NULL);
  struct stmt* inner_decl = stmt_create(STMT_DECL, decl_create(strdup("duck"), type_copy(tvoid), NULL, NULL, NULL), NULL, NULL, NULL, NULL, NULL, NULL);
  struct stmt* inner_body = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, inner_decl, NULL, stend);
  struct stmt* body = stmt_create(STMT_DECL, decl_create(strdup("duck"), type_copy(tvoid), NULL, NULL, NULL), NULL, NULL, NULL, NULL, NULL, inner_body);
  struct stmt* s = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, body, NULL, NULL);

  Symbol_table* st = symbol_table_verbose_create();
  symbol_table_scope_enter(st);
  error_status = stmt_resolve(st, s);

  struct symbol* sym_duck_in = symbol_table_scope_lookup_at(st, "duck", 2); // innermost scope
  struct symbol* sym_duck_out = symbol_table_scope_lookup_at(st, "duck", 1);
  struct symbol* sym_goose = symbol_table_scope_lookup_at(st, "goose", 1);

  if (!sym_duck_out) { print_error(test_type, "NOT NULL", "struct symbol* sym_duck_out"); return FAILURE; }
  if (!sym_duck_in) { print_error(test_type, "NOT NULL", "struct symbol* sym_duck_in"); return FAILURE; }
  if (!sym_goose) { print_error(test_type, "NOT NULL", "struct symbol* sym_goose"); return FAILURE; }
  if (sym_duck_out->which != 0) { print_error(test_type, "0", "int sym_duck_out->which"); status = FAILURE; }
  if (sym_duck_in->which != 1) { print_error(test_type, "1", "int sym_duck_in->which"); status = FAILURE; }
  if (sym_goose->which != 1) { print_error(test_type, "1", "int sym_goose->which"); status = FAILURE; }
  if (which_count) { print_error(test_type, "0", "int which_count (global reset)"); status = FAILURE; }
  symbol_table_destroy(&st); stmt_destroy(&s); type_destroy(&tvoid);
  return status;
}


Status test_decl_resolve_var_sym_redef_def_undef(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* tinteger = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct expr* e1 = expr_create_integer_literal(493);
  struct decl* dend = decl_create(strdup("foo"), type_copy(tinteger), NULL, NULL, NULL);
  struct decl* d = decl_create(strdup("foo"), type_copy(tinteger), e1, NULL, dend);

  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  error_status = decl_resolve(st, d);
  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != SYM_REDEF) { print_error(test_type, "SYM_REDEF", "int error_status"); status = FAILURE; }
  symbol_table_destroy(&st); decl_destroy(&d); type_destroy(&tinteger);
  return status;
}

Status test_decl_resolve_var_sym_redef_def_def(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* tinteger = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct expr* e1 = expr_create_integer_literal(493);
  struct expr* e2 = expr_create_integer_literal(12);
  struct decl* dend = decl_create(strdup("foo"), type_copy(tinteger), e2, NULL, NULL);
  struct decl* d = decl_create(strdup("foo"), type_copy(tinteger), e1, NULL, dend);

  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  error_status = decl_resolve(st, d);
  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != SYM_REDEF) { print_error(test_type, "SYM_REDEF", "int error_status"); status = FAILURE; }
  symbol_table_destroy(&st); decl_destroy(&d); type_destroy(&tinteger);
  return status;
}

Status test_decl_resolve_var_sym_redef_undef_undef(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* tinteger = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct decl* dend = decl_create(strdup("foo"), type_copy(tinteger), NULL, NULL, NULL);
  struct decl* d = decl_create(strdup("foo"), type_copy(tinteger), NULL, NULL, dend);

  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  error_status = decl_resolve(st, d);
  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != SYM_REDEF) { print_error(test_type, "SYM_REDEF", "int error_status"); status = FAILURE; }
  symbol_table_destroy(&st); decl_destroy(&d); type_destroy(&tinteger);
  return status;
}

Status test_decl_resolve_nonconst_array_size(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* tinteger = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct decl* arrdecl = decl_create(strdup("foo"), type_create(TYPE_ARRAY, tinteger, NULL, expr_create_name(strdup("x"))), NULL, NULL, NULL);
  struct decl* vardecl = decl_create(strdup("x"), type_copy(tinteger), expr_create_integer_literal(1), NULL, arrdecl);
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st); symbol_table_scope_enter(st);
  error_status = decl_resolve(st, vardecl);
  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != DECL_CONST) { print_error(test_type, "DECL_CONST", "int error_status"); status = FAILURE;}
  symbol_table_destroy(&st); decl_destroy(&vardecl);
  return status;
}

Status test_decl_resolve_nonconst_global(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct type* tinteger = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct decl* dend = decl_create(strdup("foo"), type_copy(tinteger), expr_create_name(strdup("duck")), NULL, NULL);
  struct decl* d = decl_create(strdup("duck"), tinteger, expr_create_integer_literal(-493), NULL, dend);
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  error_status = decl_resolve(st, d);
  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (decl_error != DECL_CONST) { print_error(test_type, "DECL_CONST", "int error_status"); status = FAILURE; }
  symbol_table_destroy(&st); decl_destroy(&d);
  return status;
}

Status test_decl_resolve_nonconst_global_array(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct expr* x = expr_create_name(strdup("x"));
  struct symbol* s = symbol_create(SYMBOL_GLOBAL, NULL, strdup("x"));
  struct expr* e = expr_create(EXPR_INIT, x, NULL);
  struct type* tinteger = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct type* tarray = type_create(TYPE_ARRAY, tinteger, NULL, expr_copy(x));
  struct decl* arrdecl = decl_create(strdup("foo"), tarray, e, NULL, NULL);

  // was causing a segfault, tho manually binding fixed the issue???
  //struct decl* vardecl = decl_create(strdup("x"), type_copy(tinteger), expr_create_integer_literal(1), NULL, arrdecl);
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);   symbol_table_scope_bind(st, "x", s);

  error_status = decl_resolve(st, arrdecl);
  // this error is triggered twice.
  if (global_error_count != 2) { print_error(test_type, "2", "int global_error_count"); status = FAILURE; }
  if (decl_error != DECL_CONST) { print_error(test_type, "DECL_CONST", "int error_status"); status = FAILURE;}
  symbol_table_destroy(&st);
  decl_destroy(&arrdecl);
  return status;
}

Status test_expr_resolve_hidden(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  struct expr* e = expr_create_string_literal(strdup("foo"));

  Symbol_table* st = symbol_table_create();
  label_count = -1;
  symbol_table_scope_enter(st); symbol_table_scope_enter(st);
  if (e->symbol) { print_error(test_type, "NULL", "e->symbol"); status = FAILURE; }
  if (symbol_table_hidden_lookup(st->hidden_table, "foo")) {
    print_error(test_type, "NULL", "struct symbol* symbol_table_scope_lookup(st, \"x\""); status = FAILURE;
  }

  error_status = expr_resolve(st, e);
  const char* found_label = symbol_table_hidden_lookup(st->hidden_table, "foo");
  if (global_error_count) { print_error(test_type, "0", "int global_error_count"); status = FAILURE; }
  if (e->symbol) { print_error(test_type, "NULL", "struct symbol* e->symbol"); status = FAILURE; }
  if (!found_label) {
    print_error(test_type, "NOT NULL", "const char* symbol_table_hidden_lookup(Hidden_table* hst, \"foo\")");
    status = FAILURE;
  }
  if (strcmp(found_label, ".L0")) { print_error(test_type, "0", "strcmp(found_label, \".L0\")"); status = FAILURE; }
  symbol_table_destroy(&st); expr_destroy(&e);
  return status;
}