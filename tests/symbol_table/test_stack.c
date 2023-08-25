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

// test functions

// unit tests for the stack
Status test_stack_create(void);
Status test_stack_destroy(void);
Status test_stack_size_null_stack(void);
Status test_stack_size_null_items(void);
Status test_stack_size_empty(void);
Status test_stack_push_null_stack(void);
Status test_stack_push_null_items(void);
Status test_stack_push(void);
Status test_stack_pop(void);
Status test_stack_pop_null_stack(void);
Status test_stack_pop_null_items(void);
Status test_stack_pop_empty(void);
Status test_stack_push_refit(void);
Status test_stack_pop_refit(void);
Status test_stack_item_null_stack(void);
Status test_stack_item_null_items(void);
Status test_stack_item_out_of_bounds_low(void);
Status test_stack_item_out_of_bounds_high(void);
Status test_stack_item(void);

// hashtable + symbol integration tests
Status test_hash_symbol_insert(void);
Status test_hash_symbol_remove(void);
Status test_hash_symbol_lookup_null(void);
Status test_hash_symbol_lookup(void);

// symbol table tests
Status test_symbol_table_destroy(void);
Status test_symbol_table_scope_level(void);
Status test_symbol_table_scope_enter(void);
Status test_symbol_table_scope_enter_multi(void);
Status test_symbol_table_scope_exit(void);
Status test_symbol_table_scope_bind(void);
Status test_symbol_table_scope_bind_null(void);
Status test_symbol_table_scope_lookup_null_stack(void);
Status test_symbol_table_scope_lookup_null_items(void);
Status test_symbol_table_scope_lookup_empty_stack(void);
Status test_symbol_table_scope_lookup_null_table(void);
Status test_symbol_table_scope_lookup_empty_table(void);
Status test_symbol_table_scope_lookup_bad_key(void);
Status test_symbol_table_scope_lookup_good_key(void);
Status test_symbol_table_scope_lookup_multi_table(void);
Status test_symbol_table_scope_lookup_current_null_stack(void);
Status test_symbol_table_scope_lookup_current_null_items(void);
Status test_symbol_table_scope_lookup_current_empty_stack(void);
Status test_symbol_table_scope_lookup_current_null_table(void);
Status test_symbol_table_scope_lookup_current_empty_table(void);
Status test_symbol_table_scope_lookup_current_bad_key(void);
Status test_symbol_table_scope_lookup_current_good_key(void);
Status test_symbol_table_scope_lookup_current_multi_key(void);

// name resolution tests (which uses the symbol table)

// resolution for expressions
Status test_expr_resolve_name(void);
Status test_expr_resolve_binary_op(void);
Status test_expr_resolve_sym_undef(void);

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

// hidden symbol name resolution tests
Status test_symbol_table_hidden_global(void);
Status test_symbol_table_hidden_local(void);

int main(void) {
  ERR_OUT = fopen("error_output_st.txt", "w");
  if (!ERR_OUT) { ERR_OUT = stderr; }

  Status (*tests[])(void) = {
    test_stack_create,
    test_stack_destroy,
    test_stack_size_null_stack,
    test_stack_size_null_items,
    test_stack_size_empty,
    test_stack_push_null_stack,
    test_stack_push_null_items,
    test_stack_push,
    test_stack_pop,
    test_stack_pop_null_stack,
    test_stack_pop_null_items,
    test_stack_pop_empty,
    test_stack_push_refit,
    test_stack_pop_refit,
    test_stack_item_null_stack,
    test_stack_item_null_items,
    test_stack_item_out_of_bounds_low,
    test_stack_item_out_of_bounds_high,
    test_stack_item,
    test_hash_symbol_insert,
    test_hash_symbol_remove,
    test_hash_symbol_lookup_null,
    test_hash_symbol_lookup,
    test_symbol_table_destroy,
    test_symbol_table_scope_level,
    test_symbol_table_scope_enter,
    test_symbol_table_scope_enter_multi,
    test_symbol_table_scope_exit,
    test_symbol_table_scope_bind,
    test_symbol_table_scope_bind_null,
    test_symbol_table_scope_lookup_null_stack,
    test_symbol_table_scope_lookup_null_items,
    test_symbol_table_scope_lookup_empty_stack,
    test_symbol_table_scope_lookup_null_table,
    test_symbol_table_scope_lookup_empty_table,
    test_symbol_table_scope_lookup_bad_key,
    test_symbol_table_scope_lookup_good_key,
    test_symbol_table_scope_lookup_multi_table,
    test_symbol_table_scope_lookup_current_null_stack,
    test_symbol_table_scope_lookup_current_null_items,
    test_symbol_table_scope_lookup_current_empty_stack,
    test_symbol_table_scope_lookup_current_null_table,
    test_symbol_table_scope_lookup_current_empty_table,
    test_symbol_table_scope_lookup_current_bad_key,
    test_symbol_table_scope_lookup_current_good_key,
    test_symbol_table_scope_lookup_current_multi_key,
    test_expr_resolve_name,
    test_expr_resolve_binary_op,
    test_expr_resolve_sym_undef,
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
    test_symbol_table_hidden_global,
    test_symbol_table_hidden_local
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

Status test_stack_create(void) {
  strcpy(test_type, "Testing: test_stack_create");
  Status status = SUCCESS;
  Stack* s = stack_create();
  if (!s) { print_error(test_type, "NOT NULL", "Stack* s"); return FAILURE; }
  if (!s->items) { print_error(test_type, "NOT NULL", "void** s->items"); status = FAILURE; }
  if (s->size) { print_error(test_type, "ZERO", "int s->size"); status= FAILURE; }
  if (s->capacity != (1<<3)) { print_error(test_type, "EIGHT (1<<3)", "int s->capacity"); status = FAILURE; }
  if (s) { if (s->items) { free(s->items); } free(s); }
  return status;
}

Status test_stack_destroy(void) {
  strcpy(test_type, "Testing: test_stack_destroy");
  Status status = SUCCESS;
  Stack* s = stack_create();
  if (!s) { print_error(test_type, "NOT NULL", "Stack* s"); return FAILURE; }
  stack_destroy(&s);
  if (s) { print_error(test_type, "NULL", "Stack* s"); status = FAILURE; }
  return status;
}


Status test_stack_size_null_stack(void) {
  strcpy(test_type, "Testing: test_stack_size_null_stack");
  Status status = SUCCESS;
  Stack* s = NULL;
  if (stack_size(s) != -1) { print_error(test_type, "-1", "return value stack_size(Stack* s)"); status = FAILURE; }
  stack_destroy(&s);
  return status;
}

Status test_stack_size_null_items(void) {
  strcpy(test_type, "Testing: test_stack_size_null_items");
  Status status = SUCCESS;
  Stack* s = stack_create(); free(s->items); s->items = NULL;
  if (stack_size(s) != -1) { print_error(test_type, "-1", "return value stack_size(Stack* s)"); status = FAILURE; }
  stack_destroy(&s);
  return status;
}

Status test_stack_size_empty(void) {
  strcpy(test_type, "Testing: test_stack_size_empty");
  Status status = SUCCESS;
  Stack* s = stack_create();
  if (stack_size(s)) { print_error(test_type, "0", "return value stack_size(Stack* s)"); status = FAILURE; }
  stack_destroy(&s);
  return status;
}

Status test_stack_push_null_stack(void) {
  strcpy(test_type, "Testing: test_stack_push_null_stack");
  Status status = SUCCESS;
  Stack* s = NULL; void* item = NULL;
  stack_push(s, item);
  if (stack_size(s) != -1) { print_error(test_type, "-1", "stack_push(Stack* s, void* item)"); status = FAILURE; }
  stack_destroy(&s);
  return status;
}

Status test_stack_push_null_items(void) {
  strcpy(test_type, "Testing: test_stack_push_null_items");
  Status status = SUCCESS;
  Stack* s = stack_create(); free(s->items); s->items = NULL; void* item = NULL;
  stack_push(s, item);
  if (stack_size(s) != -1) { print_error(test_type, "-1", "stack_push(Stack* s, void* item)"); status = FAILURE; }
  stack_destroy(&s);
  return status;
}

Status test_stack_push(void) {
  strcpy(test_type, "Testing: test_stack_push_empty");
  Status status = SUCCESS;
  Stack* s = stack_create(); void* item = (void*)"duck";
  stack_push(s, item); int top = stack_size(s) - 1;
  if (!stack_size(s)) { print_error(test_type, "1", "return value stack_size(Stack* s) after push"); status = FAILURE; }
  if (strcmp(s->items[top], "duck")) { print_error(test_type, "\"duck\"", "(char*)s->items[top]"); status = FAILURE; }
  stack_destroy(&s);
  return status;
}

Status test_stack_pop(void) {
  strcpy(test_type, "Testing: test_stack_pop");
  Status status = SUCCESS;
  Stack* s = stack_create(); void* item = (void*)"duck";
  stack_push(s, item); void* ret = stack_pop(s);
  if (stack_size(s)) { print_error(test_type, "0", "return value stack_size(Stack* s)"); status = FAILURE; }
  if (strcmp((char*)ret, "duck")) { print_error(test_type, "\"duck\"", "return value of stack_pop(Stack* s)"); status = FAILURE; }
  stack_destroy(&s);
  return status;
}

Status test_stack_pop_null_stack(void) {
  strcpy(test_type, "Testing: test_stack_pop_null_stack");
  Status status = SUCCESS;
  Stack* s = NULL;
  stack_pop(s);
  if (stack_size(s) != -1) { print_error(test_type, "-1", "stack size stack_pop(Stack* s)"); status = FAILURE; }
  stack_destroy(&s);
  return status;
}

Status test_stack_pop_null_items(void) {
  strcpy(test_type, "Testing: test_stack_pop_null_items");
  Status status = SUCCESS;
  Stack* s = stack_create(); free(s->items); s->items = NULL;
  stack_pop(s);
  if (stack_size(s) != -1) { print_error(test_type, "-1", "stack size after stack_pop(Stack* s)"); status = FAILURE; }
  stack_destroy(&s);
  return status;
}

Status test_stack_pop_empty(void) {
  strcpy(test_type, "Testing: test_stack_pop_empty");
  Status status = SUCCESS;
  Stack* s = stack_create(); stack_pop(s);
  if (stack_size(s)) { print_error(test_type, "0", "stack size after stack_pop(Stack* s)"); status = FAILURE; }
  stack_destroy(&s);
  return status;
}

Status test_stack_push_refit(void) {
  strcpy(test_type, "Testing: test_stack_push_refit");
  Status status = SUCCESS;
  Stack* s = stack_create(); void* item = (void*)"duck"; int cap = s->capacity + 1;
  for (int i = 0; i < cap; i++) stack_push(s, item);
  if (s->capacity != (1<<4)) { print_error(test_type, "16 (1<<4)", "refitting items in stack_push(Stack* s, void* item)"); status = FAILURE; }
  if (s->size != cap) { print_error(test_type, "9", "stack size after push"); status = FAILURE; }
  stack_destroy(&s);
  return status;
}

Status test_stack_pop_refit(void) {
  strcpy(test_type, "Testing: test_stack_pop_refit");
  Status status = SUCCESS;
  Stack* s = stack_create(); void* item = (void*)"duck"; int cap = (s->capacity >> 1);
  for (int i = 0; i < cap; i++) { stack_push(s, item); } stack_pop(s);
  if (s->capacity != (1<<2)) { print_error(test_type, "4 (1<<2)", "refitting items in stack_pop(Stack* s)"); status = FAILURE; }
  if (s->size != (cap - 1)) { print_error(test_type, "3", "stack size after pop"); status = FAILURE; }
  stack_destroy(&s);
  return status;
}

Status test_stack_item_null_stack(void) {
  strcpy(test_type, "Testing: test_stack_item_null_stack");
  Status status = SUCCESS;
  Stack* s = NULL;
  void* item = stack_item(s, 0);
  if (item) { print_error(test_type, "NULL", "return value of stack_item(Stack* s, int position)"); status = FAILURE; }
  stack_destroy(&s);
  return status;
}

Status test_stack_item_null_items(void) {
  strcpy(test_type, "Testing: test_stack_item_null_items");
  Status status = SUCCESS;
  Stack* s = stack_create(); free(s->items); s->items = NULL;
  void* item = stack_item(s, 0);
  if (item) { print_error(test_type, "NULL", "return value of stack_item(Stack* s, int position)"); status = FAILURE; }
  stack_destroy(&s);
  return status;
}

Status test_stack_item_out_of_bounds_low(void) {
  strcpy(test_type, "Testing: test_stack_item_out_of_bounds_low");
  Status status = SUCCESS;
  Stack* s = stack_create(); void* item = stack_item(s, -1);
  if (item) { print_error(test_type, "NULL", "return value of stack_item(Stack* s, int position)"); status = FAILURE; }
  stack_destroy(&s);
  return status;
}

Status test_stack_item_out_of_bounds_high(void) {
  strcpy(test_type, "Testing: test_stack_item_out_of_bounds_high");
  Status status = SUCCESS;
  Stack* s = stack_create(); void* item = stack_item(s, 1);
  if (item) { print_error(test_type, "NULL", "return value of stack_item(Stack* s, int position)"); status = FAILURE; }
  stack_destroy(&s);
  return status;
}

Status test_stack_item(void) {
  strcpy(test_type, "Testing: test_stack_item");
  Status status = SUCCESS;
  Stack* s = stack_create();
  stack_push(s, (void*)"duck"); int top = stack_size(s) - 1; void* item = stack_item(s, top);
  if (!item) { print_error(test_type, "NOT NULL", "return value of stack_item(Stack* s, int position)"); status = FAILURE; }
  if (strcmp((char*)item, "duck")) { print_error(test_type, "\"duck\"", "(char*)item"); status = FAILURE; }
  stack_destroy(&s);
  return status;
}

Status test_hash_symbol_insert(void) {
  strcpy(test_type, "Testing: test_hash_symbol_insert");
  Status status = SUCCESS;
  struct hash_table* ht = hash_table_create(0, 0);
  if (!ht) { print_error(test_type, "NOT NULL", "struct hash_table* ht"); status = FAILURE; }
  struct symbol* s = symbol_create(SYMBOL_GLOBAL, NULL, NULL);
  if (!s) { print_error(test_type, "NOT NULL", "struct symbol* s"); status = FAILURE; }
  if (!(hash_table_insert(ht, "foo", s) == 1 && hash_table_size(ht) == 1)) {
    print_error(test_type, "1", "int hash_table_size(struct hash_table* ht)");
    status = FAILURE;
  }
  hash_table_delete(ht);
  symbol_destroy(&s);
  return status;
}

Status test_hash_symbol_remove(void) {
  strcpy(test_type, "Testing: test_hash_symbol_remove");
  Status status = SUCCESS;
  struct hash_table* ht = hash_table_create(0, 0);
  struct symbol* s = symbol_create(SYMBOL_GLOBAL, NULL, strdup("bar"));
  if (!ht) { print_error(test_type, "NOT NULL", "struct hash_table* ht"); status = FAILURE; }
  if (!s) { print_error(test_type, "NOT NULL", "struct symbol* s"); status = FAILURE; }

  status = hash_table_insert(ht, "foo", (void*)s);
  struct symbol* ret = hash_table_remove(ht, "foo");
  if (!ret) { print_error(test_type, "NOT NULL", "void* hash_table_remove(struct hash_table* ht, const char* key)"); return FAILURE; }

  if (hash_table_size(ht)) { print_error(test_type, "0", "int hash_table_size(struct hash_table* ht)"); status = FAILURE; }
  if (strcmp(ret->name, s->name)) { print_error(test_type, "bar", "char* ret->name"); status = FAILURE; }
  hash_table_delete(ht);
  symbol_destroy(&s);
  return status;
}

Status test_hash_symbol_lookup_null(void) {
  strcpy(test_type, "Testing: test_hash_symbol_lookup_null");
  Status status = SUCCESS;
  struct hash_table* ht = hash_table_create(0, 0);
  struct symbol* s = symbol_create(SYMBOL_GLOBAL, NULL, strdup("duck"));
  if (!ht) { print_error(test_type, "NOT NULL", "struct hash_table* ht"); status = FAILURE; }
  if (!s) { print_error(test_type, "NOT NULL", "struct symbol* s"); status = FAILURE; }

  status = hash_table_insert(ht, "foo", (void*)s);
  if (hash_table_lookup(ht, "bar")) { print_error(test_type, "NULL", "void* hash_table_lookup(struct hash_table* ht, const char* key)"); status = FAILURE; }
  hash_table_delete(ht);
  symbol_destroy(&s);
  return status;
}

Status test_hash_symbol_lookup(void) {
  strcpy(test_type, "Testing: test_hash_symbol_lookup");
  Status status = SUCCESS;
  struct hash_table* ht = hash_table_create(0, 0);
  struct symbol* s = symbol_create(SYMBOL_GLOBAL, NULL, strdup("duck"));
  if (!ht) { print_error(test_type, "NOT NULL", "struct hash_table* ht"); status = FAILURE; }
  if (!s) { print_error(test_type, "NOT NULL", "struct symbol* s"); status = FAILURE; }

  status = hash_table_insert(ht, "foo", (void*)s);
  struct symbol* ret = (struct symbol*)hash_table_lookup(ht, "foo");
  if (!ret) { print_error(test_type, "NOT NULL", "void* hash_table_lookup(struct hash_table* ht, const char* key)"); status = FAILURE; }
  if (strcmp(ret->name, s->name)) { print_error(test_type, "duck", "char* ret->name"); status = FAILURE; }
  hash_table_delete(ht);
  symbol_destroy(&s);
  return status;
}

Status test_symbol_table_destroy(void) {
  strcpy(test_type, "Testing: test_symbol_table_destroy");
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create();
  struct hash_table* ht = hash_table_create(0, 0);
  struct symbol* s = symbol_create(SYMBOL_GLOBAL, NULL, strdup("bar"));
  if (!st) { print_error(test_type, "NOT NULL", "Symbol_table* st"); status = FAILURE; }
  if (!ht) { print_error(test_type, "NOT NULL", "struct hash_table* ht"); status = FAILURE; }
  if (!s) { print_error(test_type, "NOT NULL", "struct symbol* s"); status = FAILURE; }

  status = hash_table_insert(ht, "foo", (void*)s);
  stack_push(st->stack, ht);
  symbol_table_destroy(&st); ht = NULL; s = NULL; // tests keep failing here.
  if (st) { print_error(test_type, "NULL", "Symbol_table* st"); status = FAILURE; }
  if (ht) { print_error(test_type, "NULL", "struct hash_table* ht"); status = FAILURE; }
  if (s) { print_error(test_type, "NULL", "struct symbol* s"); status = FAILURE; }
  return status;
}

Status test_symbol_table_scope_level(void) {
  strcpy(test_type, "Testing: test_symbol_table_scope_level");
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create();
  struct hash_table* ht = hash_table_create(0, 0);
  if (!st) { print_error(test_type, "NOT NULL", "Symbol_table* st"); status = FAILURE; }
  if (!ht) { print_error(test_type, "NOT NULL", "struct hash_table* ht"); status = FAILURE; }

  stack_push(st->stack, ht); st->top++;
  if (symbol_table_scope_level(st) != 1) { print_error(test_type, "1", "int symbol_table_scope_level(Symbol_table* st)"); status = FAILURE; }
  if (hash_table_size(ht)) { print_error(test_type, "0", "int hash_table_size(struct hash_table* ht)"); status = FAILURE; }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_enter(void) {
  strcpy(test_type, "Testing: test_symbol_table_scope_enter");
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create();
  if (!st) { print_error(test_type, "NOT NULL", "Symbol_table* st"); status = FAILURE; }
  symbol_table_scope_enter(st);
  if (symbol_table_scope_level(st) != 1) { print_error(test_type, "1", "int symbol_table_scope_level(Symbol_table* st)"); status = FAILURE; }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_enter_multi(void) {
  strcpy(test_type, "Testing: test_symbol_table_scope_enter_multi");
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create();
  if (!st) { print_error(test_type, "NOT NULL", "Symbol_table* st"); status = FAILURE; }
  symbol_table_scope_enter(st); symbol_table_scope_enter(st); symbol_table_scope_enter(st);
  if (symbol_table_scope_level(st) != 3) { print_error(test_type, "3", "int symbol_table_scope_level(Symbol_table* st)"); status = FAILURE; }
  for (int i = st->stack->size - 1; i >= 0; i--) {
    if (!st->stack->items[i]) { print_error(test_type, "NOT NULL", "hash tables after scope entry"); status = FAILURE; }
  }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_exit(void) {
  strcpy(test_type, "Testing: test_symbol_table_scope_exit");
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create();
  if (!st) { print_error(test_type, "NOT NULL", "Symbol_table* st"); status = FAILURE; }
  symbol_table_scope_enter(st); symbol_table_scope_exit(st);
  if (symbol_table_scope_level(st)) { print_error(test_type, "0", "int symbol_table_scope_level(Symbol_table* st)"); status = FAILURE; }
  if (st->stack->items[stack_size(st->stack)]) { print_error(test_type, "NULL", "pointer value of topmost scope"); status = FAILURE; }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_bind(void) {
  strcpy(test_type, "Testing: test_symbol_table_scope_bind");
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create();
  struct symbol* s = symbol_create(SYMBOL_GLOBAL, NULL, strdup("bar"));
  if (!st) { print_error(test_type, "NOT NULL", "Symbol_table* st"); status = FAILURE; }
  if (!s) { print_error(test_type, "NOT NULL", "struct symbol* s"); status = FAILURE; }

  symbol_table_scope_enter(st); int top = stack_size(st->stack) - 1;
  symbol_table_scope_bind(st, "foo", s);
  if (!hash_table_size(st->stack->items[top])) { print_error(test_type, "1", "int hash_table_size(struct hash_table* ht)"); status = FAILURE; }
  if (!hash_table_lookup(st->stack->items[top], "foo")) {
    print_error(test_type, "NOT NULL", "void* hash_table_lookup(struct hash_table* h, const char* key, void* value)");
    status = FAILURE;
  }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_bind_null(void)  {
  strcpy(test_type, "Testing: test_symbol_table_scope_bind_null");
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create();
  struct hash_table* ht = NULL;
  struct symbol* s = symbol_create(SYMBOL_GLOBAL, NULL, strdup("bar"));
  if (!st) { print_error(test_type, "NOT NULL", "Symbol_table* st"); status = FAILURE; }
  if (ht) { print_error(test_type, "NULL", "struct hash_table* ht"); status = FAILURE; }
  if (!s) { print_error(test_type, "NOT NULL", "struct symbol* s"); status = FAILURE; }

  stack_push(st->stack, ht); int top = stack_size(st->stack) - 1;
  symbol_table_scope_bind(st, "foo", s);
  if (st->stack->items[top]) {
    print_error(test_type, "NULL", "struct hash_table* st->items[top]");
    status = FAILURE;
  }
  symbol_table_destroy(&st); symbol_destroy(&s);
  return status;
}

Status test_symbol_table_scope_lookup_null_stack(void) {
  strcpy(test_type, "Testing: test_symbol_table_scope_lookup_null_stack");
  Status status = SUCCESS;
  Symbol_table* st = NULL;
  if (symbol_table_scope_lookup(st, "foo")) {
    print_error(test_type, "NULL", "struct symbol* symbol_table_scope_lookup(Symbol_table* st, const char* name)");
    status = FAILURE;
  }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_lookup_null_items(void) {
  strcpy(test_type, "Testing: test_symbol_table_scope_lookup_null_items");
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create(); free(st->stack->items); st->stack->items = NULL;
  if (symbol_table_scope_lookup(st, "foo")) {
    print_error(test_type, "NULL", "struct symbol* symbol_table_scope_lookup(Symbol_table* st, const char* name)");
    status = FAILURE;
  }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_lookup_empty_stack(void) {
  strcpy(test_type, "Testing: test_symbol_table_scope_lookup_empty_stack");
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create();
  if (symbol_table_scope_lookup(st, "foo")) {
    print_error(test_type, "NULL", "struct symbol* symbol_table_scope_lookup(Symbol_table* st, const char* name)");
    status = FAILURE;
  }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_lookup_null_table(void) {
  strcpy(test_type, "Testing: test_symbol_table_scope_lookup_null_table");
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create();
  struct hash_table* ht = NULL;
  stack_push(st->stack, ht);
  if (symbol_table_scope_lookup(st, "foo")) {
    print_error(test_type, "NULL", "struct symbol* symbol_table_scope_lookup(Symbol_table* st, const char* name)");
    status = FAILURE;
  }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_lookup_empty_table(void) {
  strcpy(test_type, "Testing: test_symbol_table_scope_lookup_empty_table");
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  if (symbol_table_scope_lookup(st, "foo")) {
    print_error(test_type, "NULL", "struct symbol* symbol_table_scope_lookup(Symbol_table* st, const char* name)");
    status = FAILURE;
  }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_lookup_bad_key(void) {
  strcpy(test_type, "Testing: test_symbol_table_scope_lookup_bad_key");
  Status status = SUCCESS;
  struct symbol* s = symbol_create(SYMBOL_GLOBAL, NULL, strdup("bar"));
  Symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  symbol_table_scope_bind(st, "bar", s);
  if (symbol_table_scope_lookup(st, "foo")) {
    print_error(test_type, "NULL", "struct symbol* symbol_table_scope_lookup(Symbol_table* st, const char* name)");
    status = FAILURE;
  }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_lookup_good_key(void) {
  strcpy(test_type, "Testing: test_symbol_table_scope_lookup_good_key");
  Status status = SUCCESS;
  struct symbol* s = symbol_create(SYMBOL_GLOBAL, NULL, strdup("bar"));
  Symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  symbol_table_scope_bind(st, "foo", s);
  if (!symbol_table_scope_lookup(st, "foo")) {
    print_error(test_type, "NOT NULL", "struct symbol* symbol_table_scope_lookup(Symbol_table* st, const char* name)");
    status = FAILURE;
  }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_lookup_multi_table(void) {
  strcpy(test_type, "Testing: test_symbol_table_scope_lookup_multi_table");
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  symbol_table_scope_bind(st, "foo", symbol_create(SYMBOL_GLOBAL, NULL, strdup("bar")));
  symbol_table_scope_enter(st);
  symbol_table_scope_bind(st, "duck", symbol_create(SYMBOL_GLOBAL, NULL, strdup("goose")));

  struct symbol* ret = symbol_table_scope_lookup(st, "foo");
  if (!ret) {
    print_error(test_type, "NOT NULL", "struct symbol* symbol_table_scope_lookup(Symbol_table* st, const char* name) [foo]");
    status = FAILURE;
  }
  ret = symbol_table_scope_lookup(st, "duck");
  if (!ret) {
    print_error(test_type, "NOT NULL", "struct symbol* symbol_table_scope_lookup(Symbol_table* st, const char* name) [duck]");
    status = FAILURE;
  }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_lookup_current_null_stack(void) {
  strcpy(test_type, "Testing: test_symbol_table_scope_lookup_current_null_stack");
  Status status = SUCCESS;
  Symbol_table* st = NULL;
  if (symbol_table_scope_lookup_current(st, "foo")) {
    print_error(test_type, "NULL", "struct symbol* symbol_table_scope_lookup_current(Symbol_table* st, const char* name)");
    status = FAILURE;
  }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_lookup_current_null_items(void) {
  strcpy(test_type, "Testing: test_symbol_table_scope_lookup_current_null_items");
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create(); free(st->stack->items); st->stack->items = NULL;
  if (symbol_table_scope_lookup_current(st, "foo")) {
    print_error(test_type, "NULL", "struct symbol* symbol_table_scope_lookup_current(Symbol_table* st, const char* name)");
    status = FAILURE;
  }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_lookup_current_empty_stack(void) {
  strcpy(test_type, "Testing: test_symbol_table_scope_lookup_current_empty_stack");
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create();
  if (symbol_table_scope_lookup_current(st, "foo")) {
    print_error(test_type, "NULL", "struct symbol* symbol_table_scope_lookup_current(Symbol_table* st, const char* name)");
    status = FAILURE;
  }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_lookup_current_null_table(void) {
  strcpy(test_type, "Testing: test_symbol_table_scope_lookup_current_null_table");
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create();
  struct hash_table* ht = NULL;
  stack_push(st->stack, ht);
  if (symbol_table_scope_lookup_current(st, "foo")) {
    print_error(test_type, "NULL", "struct symbol* symbol_table_scope_lookup_current(Symbol_table* st, const char* name)");
    status = FAILURE;
  }
  symbol_table_destroy(&st);
  return status;
}
Status test_symbol_table_scope_lookup_current_empty_table(void) {
  strcpy(test_type, "Testing: test_symbol_table_scope_lookup_current_empty_table");
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  if (symbol_table_scope_lookup_current(st, "foo")) {
    print_error(test_type, "NULL", "struct symbol* symbol_table_scope_lookup_current(Symbol_table* st, const char* name)");
    status = FAILURE;
  }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_lookup_current_bad_key(void) {
  strcpy(test_type, "Testing: test_symbol_table_scope_lookup_current_bad_key");
  Status status = SUCCESS;
  struct symbol* s = symbol_create(SYMBOL_GLOBAL, NULL, strdup("bar"));
  Symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  symbol_table_scope_bind(st, "bar", s);
  if (symbol_table_scope_lookup_current(st, "foo")) {
    print_error(test_type, "NULL", "struct symbol* symbol_table_scope_lookup_current(Symbol_table* st, const char* name)");
    status = FAILURE;
  }
  symbol_table_destroy(&st);
  return status;
}
Status test_symbol_table_scope_lookup_current_good_key(void) {
  strcpy(test_type, "Testing: test_symbol_table_scope_lookup_current_good_key");
  Status status = SUCCESS;
  struct symbol* s = symbol_create(SYMBOL_GLOBAL, NULL, strdup("bar"));
  Symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  symbol_table_scope_bind(st, "foo", s);
  if (!symbol_table_scope_lookup_current(st, "foo")) {
    print_error(test_type, "NOT NULL", "struct symbol* symbol_table_scope_lookup_current(Symbol_table* st, const char* name)");
    status = FAILURE;
  }
  symbol_table_destroy(&st);
  return status;
}
Status test_symbol_table_scope_lookup_current_multi_key(void) {
  strcpy(test_type, "Testing: test_symbol_table_scope_lookup_current_multi_key");
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  symbol_table_scope_bind(st, "foo", symbol_create(SYMBOL_GLOBAL, NULL, strdup("bar")));
  symbol_table_scope_bind(st, "duck", symbol_create(SYMBOL_GLOBAL, NULL, strdup("goose")));

  struct symbol* ret = symbol_table_scope_lookup_current(st, "foo");
  if (!ret) {
    print_error(test_type, "NOT NULL", "struct symbol* symbol_table_scope_lookup(Symbol_table* st, const char* name) [foo]");
    status = FAILURE;
  }
  ret = symbol_table_scope_lookup_current(st, "duck");
  if (!ret) {
    print_error(test_type, "NOT NULL", "struct symbol* symbol_table_scope_lookup(Symbol_table* st, const char* name) [duck]");
    status = FAILURE;
  }
  symbol_table_destroy(&st);
  return status;
}

Status test_expr_resolve_name(void) {
  strcpy(test_type, "Testing: test_expr_resolve_name");
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
  strcpy(test_type, "Testing: test_expr_resolve_binary_op");
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
  strcpy(test_type, "Testing: test_decl_resolve_atomic_uninit");
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
  strcpy(test_type, "Testing: test_decl_resolve_atomic_init");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct expr* y = expr_create_name(strdup("y"));
  struct decl* d = decl_create(strdup("x"), type_copy(integer), y, NULL, NULL);
  struct symbol* sy = symbol_create(SYMBOL_GLOBAL, type_copy(integer), strdup("y"));

  Symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  symbol_table_scope_bind(st, "y", sy);
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
  return status;
}

Status test_decl_resolve_function_uninit_no_param(void) {
  strcpy(test_type, "Testing: test_decl_resolve_function_uninit_no_param");
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
  strcpy(test_type, "Testing: test_decl_resolve_function_uninit_one_param");
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
  strcpy(test_type, "Testing: test_decl_resolve_function_uninit_many_param");
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
  strcpy(test_type, "Testing: test_stmt_resolve_expr");
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
  strcpy(test_type, "Testing: test_stmt_resolve_print");
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
  strcpy(test_type, "Testing: test_stmt_resolve_return");
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
  strcpy(test_type, "Testing: test_stmt_resolve_decl");
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
  strcpy(test_type, "Testing: test_stmt_resolve_block_empty");
  Status status = SUCCESS;
  struct stmt* s = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  Symbol_table* st = symbol_table_verbose_create();
  symbol_table_scope_enter(st); int old_level = stack_size(st->stack);
  stmt_resolve(st, s);

  if ((stack_size(st->stack) - old_level) != 1) { print_error(test_type, "2", "scope level"); status = FAILURE; }
  if (hash_table_size(st->stack->items[stack_size(st->stack) - 1])) { print_error(test_type, "0", "topmost hashtable size"); status = FAILURE; }
  symbol_table_destroy(&st); stmt_destroy(&s);
  return status;
}

Status test_stmt_resolve_block(void) {
  strcpy(test_type, "Testing: test_stmt_resolve_block");
  Status status = SUCCESS;
  struct type* tvoid = type_create(TYPE_VOID, NULL, NULL, NULL);
  struct stmt* body = stmt_create(STMT_DECL, decl_create(strdup("duck"), type_copy(tvoid), NULL, NULL, NULL), NULL, NULL, NULL, NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, body, NULL, NULL);
  Symbol_table* st = symbol_table_verbose_create();
  symbol_table_scope_enter(st); int old_level = stack_size(st->stack);
  stmt_resolve(st, s);
  if ((stack_size(st->stack) - old_level) != 1) { print_error(test_type, "2", "scope level"); status = FAILURE; }
  if (!hash_table_size(st->stack->items[stack_size(st->stack) - 1])) { print_error(test_type, "1", "topmost hashtable size"); status = FAILURE; }
  if (!symbol_table_scope_lookup_at(st, "duck", old_level)) { print_error(test_type, "NOT NULL", "current scope lookup [duck]"); status = FAILURE; }
  if (s->body->decl->symbol->kind != SYMBOL_LOCAL) { print_error(test_type, "SYMBOL_LOCAL", "symbol kind [duck]"); status = FAILURE; }
  symbol_table_destroy(&st); stmt_destroy(&s); type_destroy(&tvoid);
  return status;
}

Status test_stmt_resolve_while(void) {
  strcpy(test_type, "Testing: test_stmt_resolve_while");
  Status status = SUCCESS;
  struct expr* e = expr_create_name(strdup("duck"));
  struct stmt* body = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_WHILE, NULL, NULL, e, NULL, body, NULL, NULL);
  struct symbol* sym = symbol_create(SYMBOL_GLOBAL, type_create(TYPE_VOID, NULL, NULL, NULL), strdup("duck"));

  Symbol_table* st = symbol_table_verbose_create();
  symbol_table_scope_enter(st); int old_level = stack_size(st->stack);
  symbol_table_scope_bind(st, "duck", sym);
  stmt_resolve(st, s);

  if ((stack_size(st->stack) - old_level) != 1) { print_error(test_type, "2", "scope level"); status = FAILURE; }
  if (hash_table_size(st->stack->items[stack_size(st->stack) - 1])) { print_error(test_type, "0", "topmost hashtable size"); status = FAILURE; }
  if (!symbol_table_scope_lookup(st, "duck")) { print_error(test_type, "NOT NULL", "scope lookup [duck]"); status = FAILURE; }
  if (symbol_table_scope_lookup_at(st, "duck", old_level)) { print_error(test_type, "NULL", "current scope lookup [duck]"); status = FAILURE; }
  symbol_table_destroy(&st); stmt_destroy(&s);
  return status;
}

Status test_stmt_resolve_if_else_null(void) {
  strcpy(test_type, "Testing: test_stmt_resolve_if_else_null");
  Status status = SUCCESS;
  struct expr* e = expr_create_name(strdup("duck"));
  struct stmt* body = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_IF_ELSE, NULL, NULL, e, NULL, body, NULL, NULL);
  struct symbol* sym = symbol_create(SYMBOL_GLOBAL, type_create(TYPE_VOID, NULL, NULL, NULL), strdup("duck"));

  Symbol_table* st = symbol_table_verbose_create();
  symbol_table_scope_enter(st); int old_level = stack_size(st->stack);
  symbol_table_scope_bind(st, "duck", sym);
  stmt_resolve(st, s);

  if ((stack_size(st->stack) - old_level) != 1) { print_error(test_type, "2", "scope level"); status = FAILURE; }
  if (hash_table_size(st->stack->items[stack_size(st->stack) - 1])) { print_error(test_type, "0", "topmost hashtable size"); status = FAILURE; }
  if (!symbol_table_scope_lookup(st, "duck")) { print_error(test_type, "NOT NULL", "scope lookup [duck]"); status = FAILURE; }
  if (symbol_table_scope_lookup_at(st, "duck", old_level)) { print_error(test_type, "NULL", "current scope lookup [duck]"); status = FAILURE; }
  symbol_table_destroy(&st); stmt_destroy(&s);
  return status;
}

Status test_stmt_resolve_if_else(void) {
  strcpy(test_type, "Testing: test_stmt_resolve_if_else");
  Status status = SUCCESS;
  struct expr* e = expr_create_name(strdup("duck"));
  struct stmt* body = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  struct stmt* else_body = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_IF_ELSE, NULL, NULL, e, NULL, body, else_body, NULL);
  struct symbol* sym = symbol_create(SYMBOL_GLOBAL, type_create(TYPE_VOID, NULL, NULL, NULL), strdup("duck"));

  Symbol_table* st = symbol_table_verbose_create();
  symbol_table_scope_enter(st); int old_level = stack_size(st->stack);
  symbol_table_scope_bind(st, "duck", sym);
  stmt_resolve(st, s);
  if ((stack_size(st->stack) - old_level) != 2) { print_error(test_type, "3", "scope level"); status = FAILURE; }
  if (hash_table_size(st->stack->items[stack_size(st->stack) - 1])) { print_error(test_type, "0", "topmost hashtable size"); status = FAILURE; }
  if (!symbol_table_scope_lookup(st, "duck")) { print_error(test_type, "NOT NULL", "scope lookup [duck]"); status = FAILURE; }
  if (symbol_table_scope_lookup_at(st, "duck", old_level)) { print_error(test_type, "NULL", "current scope lookup [duck]"); status = FAILURE; }
  symbol_table_destroy(&st); stmt_destroy(&s);
  return status;
}

Status test_stmt_resolve_for_expr(void) {
  strcpy(test_type, "Testing: test_stmt_resolve_for_expr");
  Status status = SUCCESS;
  struct expr* duck = expr_create_name(strdup("duck"));
  struct expr* duck2 = expr_create_name(strdup("duck"));
  struct expr* goose = expr_create_name(strdup("goose"));
  struct stmt* body = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_FOR, NULL, duck, duck2, goose, body, NULL, NULL);
  struct symbol* sym_duck = symbol_create(SYMBOL_GLOBAL, type_create(TYPE_VOID, NULL, NULL, NULL), strdup("duck"));
  struct symbol* sym_goose = symbol_create(SYMBOL_GLOBAL, type_create(TYPE_VOID, NULL, NULL, NULL), strdup("goose"));

  Symbol_table* st = symbol_table_verbose_create();
  symbol_table_scope_enter(st); int old_level = stack_size(st->stack);
  symbol_table_scope_bind(st, "duck", sym_duck);
  symbol_table_scope_bind(st, "goose", sym_goose);
  stmt_resolve(st, s);

  if ((stack_size(st->stack) - old_level) != 1) { print_error(test_type, "2", "scope level"); status = FAILURE; }
  if (hash_table_size(st->stack->items[stack_size(st->stack) - 1])) { print_error(test_type, "0", "topmost hashtable size"); status = FAILURE; }
  if (!symbol_table_scope_lookup(st, "duck")) { print_error(test_type, "NOT NULL", "scope lookup [duck]"); status = FAILURE; }
  if (symbol_table_scope_lookup_at(st, "duck", old_level)) { print_error(test_type, "NULL", "current scope lookup [duck]"); status = FAILURE; }
  if (!symbol_table_scope_lookup(st, "goose")) { print_error(test_type, "NOT NULL", "scope lookup [goose]"); status = FAILURE; }
  if (symbol_table_scope_lookup_at(st, "goose", old_level)) { print_error(test_type, "NULL", "current scope lookup [goose]"); status = FAILURE; }
  symbol_table_destroy(&st);
  stmt_destroy(&s);
  return status;
}

Status test_stmt_resolve_for_decl(void) {
  strcpy(test_type, "Testing: test_stmt_resolve_for_decl");
  Status status = SUCCESS;
  struct stmt* s = stmt_create(STMT_FOR, decl_create(strdup("duck"), type_create(TYPE_VOID, NULL, NULL, NULL), NULL, NULL, NULL),
				NULL, NULL, NULL,
 				stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
 				NULL, NULL);
  Symbol_table* st = symbol_table_verbose_create();
  symbol_table_scope_enter(st); int old_level = stack_size(st->stack);
  stmt_resolve(st, s);

  if ((stack_size(st->stack) - old_level) != 1) { print_error(test_type, "2", "scope level"); status = FAILURE; }
  if (hash_table_size(st->stack->items[stack_size(st->stack) - 1])) { print_error(test_type, "0", "topmost hashtable size"); status = FAILURE; }
  if (!symbol_table_scope_lookup(st, "duck")) { print_error(test_type, "NOT NULL", "scope lookup [duck]"); status = FAILURE; }
  if (symbol_table_scope_lookup_at(st, "duck", old_level)) { print_error(test_type, "NULL", "current scope lookup [duck]"); status = FAILURE; }
  symbol_table_destroy(&st); stmt_destroy(&s);
  return status;
}

Status test_decl_resolve_program(void) {
  strcpy(test_type, "Testing: test_decl_resolve_program");
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

  if (stack_size(st->stack) != 3) { print_error(test_type, "3", "total tables"); status = FAILURE; }
  if (!symbol_table_scope_lookup_all(st, "main")) { print_error(test_type, "NOT NULL", "scope lookup [main]"); status = FAILURE; }
  if (!symbol_table_scope_lookup_all(st, "argc")) { print_error(test_type, "NOT NULL", "scope lookup [argc]"); status = FAILURE; }
  if (!symbol_table_scope_lookup_all(st, "argv")) { print_error(test_type, "NOT NULL", "scope lookup [argv]"); status = FAILURE; }
  if (!symbol_table_scope_lookup_all(st, "i")) { print_error(test_type, "NOT NULL", "scope lookup [i]"); status = FAILURE; }
  symbol_table_destroy(&st); decl_destroy(&d);
  return status;
}

Status test_expr_resolve_sym_undef(void) {
  strcpy(test_type, "Testing: test_expr_resolve_sym_undef");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct expr* e = expr_create_name(strdup("x"));

  Symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  if (e->symbol) { print_error(test_type, "NULL", "e->symbol"); status = FAILURE; }
  if (symbol_table_scope_lookup(st, "x")) { print_error(test_type, "NULL", "struct symbol* symbol_table_scope_lookup(st, \"x\""); status = FAILURE; }

  error_status = expr_resolve(st, e);
  symbol_table_scope_lookup_current(st, "x");

  if (!global_error_count) { print_error(test_type, "1", "int global_error_count"); status = FAILURE; }
  if (error_status != SYM_UNDEF) { print_error(test_type, "SYM_UNDEF", "int error_status"); status = FAILURE; }
  if (!e->symbol) { print_error(test_type, "NOT NULL", "symbol_table_scope_lookup(Symbol_table* st)"); return FAILURE; }
  if (strcmp(e->symbol->name, e->name)) { print_error(test_type, "0", "strcmp(e->symbol->name, e->name)"); status = FAILURE; }
  if (!type_equals(integer, e->symbol->type)) { print_error(test_type, "true", "type_equals(integer, ret->type)"); status = FAILURE; }
  if (e->symbol->kind != SYMBOL_GLOBAL) { print_error(test_type, "SYMBOL_GLOBAL", "int ret->kind"); status = FAILURE; }
  symbol_table_destroy(&st); expr_destroy(&e); type_destroy(&integer);
  return status;
}

Status test_decl_resolve_sym_redef(void) {
  strcpy(test_type, "Testing: test_decl_resolve_sym_redef");
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
  strcpy(test_type, "Testing: test_decl_resolve_function_param_redef");
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
  strcpy(test_type, "Testing: test_decl_resolve_function_sym_redef_undef_undef");
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
  strcpy(test_type, "Testing: test_decl_resolve_function_sym_redef_undef_def");
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
  strcpy(test_type, "Testing: test_decl_resolve_function_sym_redef_def_undef");
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
  strcpy(test_type, "Testing: test_decl_resolve_function_sym_redef_def_def");
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
  strcpy(test_type, "Testing: test_stmt_resolve_shadow_no_redef");
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
  strcpy(test_type, "Testing: test_decl_resolve_function_param_mismatch");
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
  strcpy(test_type, "Testing: test_decl_resolve_which_scope_enter");
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
  strcpy(test_type, "Testing: test_stmt_resolve_which_scope_exit");
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


Status test_symbol_table_hidden_global(void) {
  strcpy(test_type, "Testing: test_symbol_table_hidden_global");
  Status status = SUCCESS;

  struct decl* d = decl_create(strdup("x"), type_create(TYPE_STRING, NULL, NULL, NULL), expr_create_string_literal("493"), NULL, NULL);

  // not using register codegen init here
  label_count = -1;
  Symbol_table* st = symbol_table_verbose_create(); st->show_hidden = true;
  symbol_table_scope_enter(st);
  error_status = decl_resolve(st, d);

 struct symbol* hidden_symbol = symbol_table_scope_lookup(st, ".L0");
 if (!hidden_symbol) { print_error(test_type, "NOT NULL", "struct symbol* hidden_symbol"); return FAILURE; }
 if (hidden_symbol->which != -1) { print_error(test_type, "-1", "int hidden_symbol->which"); status= FAILURE; }
 if (which_count) { print_error(test_type, "0", "int which_count"); status = FAILURE; }
 symbol_table_destroy(&st); decl_destroy(&d);
 return status;
}

Status test_symbol_table_hidden_local(void) {
  strcpy(test_type, "Testing: test_symbol_table_hidden_local");
  Status status = SUCCESS;

  struct decl* d = decl_create(strdup("x"), type_create(TYPE_STRING, NULL, NULL, NULL), expr_create_string_literal("493"), NULL, NULL);

  // not using register codegen init here
  label_count = -1;
  Symbol_table* st = symbol_table_verbose_create(); st->show_hidden = true;
  symbol_table_scope_enter(st); symbol_table_scope_enter(st);
  error_status = decl_resolve(st, d);

 struct symbol* hidden_symbol_local = symbol_table_scope_lookup_at(st, ".L0", 1);
 struct symbol* hidden_symbol_global = symbol_table_scope_lookup_at(st, ".L0", 0);

 if (hidden_symbol_local) { print_error(test_type, "NULL", "struct symbol* hidden_symbol_local"); return FAILURE; }
 if (!hidden_symbol_global) { print_error(test_type, "NOT NULL", "struct symbol* hidden_symbol_global"); return FAILURE; }
 if (hidden_symbol_global->which != -1) { print_error(test_type, "-1", "int hidden_symbol->which"); status= FAILURE; }
 if (which_count) { print_error(test_type, "0", "int which_count"); status = FAILURE; }


 symbol_table_destroy(&st); decl_destroy(&d);
 return status;
}


Status test_decl_resolve_var_sym_redef_def_undef(void) {
  strcpy(test_type, "Testing: test_decl_resolve_var_sym_redef_def_undef");
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
  strcpy(test_type, "Testing: test_decl_resolve_var_sym_redef_def_def");
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
  strcpy(test_type, "Testing: test_decl_resolve_var_sym_redef_undef_undef");
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