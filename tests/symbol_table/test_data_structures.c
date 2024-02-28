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

// unit tests for the vector
Status test_vector_create(void);
Status test_vector_destroy(void);
Status test_vector_size_null_vector(void);
Status test_vector_size_null_items(void);
Status test_vector_size_empty(void);
Status test_vector_push_null_vector(void);
Status test_vector_push_null_items(void);
Status test_vector_push(void);
Status test_vector_pop(void);
Status test_vector_pop_null_vector(void);
Status test_vector_pop_null_items(void);
Status test_vector_pop_empty(void);
Status test_vector_push_refit(void);
Status test_vector_pop_refit(void);
Status test_vector_item_null_vector(void);
Status test_vector_item_null_items(void);
Status test_vector_item_out_of_bounds_low(void);
Status test_vector_item_out_of_bounds_high(void);
Status test_vector_item(void);

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
Status test_symbol_table_scope_lookup_null_vector(void);
Status test_symbol_table_scope_lookup_null_items(void);
Status test_symbol_table_scope_lookup_empty_vector(void);
Status test_symbol_table_scope_lookup_null_table(void);
Status test_symbol_table_scope_lookup_empty_table(void);
Status test_symbol_table_scope_lookup_bad_key(void);
Status test_symbol_table_scope_lookup_good_key(void);
Status test_symbol_table_scope_lookup_multi_table(void);
Status test_symbol_table_scope_lookup_current_null_vector(void);
Status test_symbol_table_scope_lookup_current_null_items(void);
Status test_symbol_table_scope_lookup_current_empty_vector(void);
Status test_symbol_table_scope_lookup_current_null_table(void);
Status test_symbol_table_scope_lookup_current_empty_table(void);
Status test_symbol_table_scope_lookup_current_bad_key(void);
Status test_symbol_table_scope_lookup_current_good_key(void);
Status test_symbol_table_scope_lookup_current_multi_key(void);

// tests for the hidden symbol table
Status test_symbol_table_hidden_create();
Status test_symbol_table_hidden_destroy();
Status test_symbol_table_hidden_bind();
Status test_symbol_table_hidden_lookup_empty();
Status test_symbol_table_hidden_lookup_bad_key();
Status test_symbol_table_hidden_lookup_good_key();

int main(void) {
  ERR_OUT = fopen("error_output_st.txt", "w");
  if (!ERR_OUT) { ERR_OUT = stderr; }

  Status (*tests[])(void) = {
    test_vector_create,
    test_vector_destroy,
    test_vector_size_null_vector,
    test_vector_size_null_items,
    test_vector_size_empty,
    test_vector_push_null_vector,
    test_vector_push_null_items,
    test_vector_push,
    test_vector_pop,
    test_vector_pop_null_vector,
    test_vector_pop_null_items,
    test_vector_pop_empty,
    test_vector_push_refit,
    test_vector_pop_refit,
    test_vector_item_null_vector,
    test_vector_item_null_items,
    test_vector_item_out_of_bounds_low,
    test_vector_item_out_of_bounds_high,
    test_vector_item,
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
    test_symbol_table_scope_lookup_null_vector,
    test_symbol_table_scope_lookup_null_items,
    test_symbol_table_scope_lookup_empty_vector,
    test_symbol_table_scope_lookup_null_table,
    test_symbol_table_scope_lookup_empty_table,
    test_symbol_table_scope_lookup_bad_key,
    test_symbol_table_scope_lookup_good_key,
    test_symbol_table_scope_lookup_multi_table,
    test_symbol_table_scope_lookup_current_null_vector,
    test_symbol_table_scope_lookup_current_null_items,
    test_symbol_table_scope_lookup_current_empty_vector,
    test_symbol_table_scope_lookup_current_null_table,
    test_symbol_table_scope_lookup_current_empty_table,
    test_symbol_table_scope_lookup_current_bad_key,
    test_symbol_table_scope_lookup_current_good_key,
    test_symbol_table_scope_lookup_current_multi_key,
    test_symbol_table_hidden_create,
    test_symbol_table_hidden_destroy,
    test_symbol_table_hidden_bind,
    test_symbol_table_hidden_lookup_empty,
    test_symbol_table_hidden_lookup_bad_key,
    test_symbol_table_hidden_lookup_good_key
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

Status test_vector_create(void) {
strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Vector* s = vector_create();
  if (!s) { print_error(test_type, "NOT NULL", "Vector* s"); return FAILURE; }
  if (!s->items) { print_error(test_type, "NOT NULL", "void** s->items"); status = FAILURE; }
  if (s->size) { print_error(test_type, "ZERO", "int s->size"); status= FAILURE; }
  if (s->capacity != (1<<3)) { print_error(test_type, "EIGHT (1<<3)", "int s->capacity"); status = FAILURE; }
  if (s) { if (s->items) { free(s->items); } free(s); }
  return status;
}

Status test_vector_destroy(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Vector* s = vector_create();
  if (!s) { print_error(test_type, "NOT NULL", "Vector* s"); return FAILURE; }
  vector_destroy(&s);
  if (s) { print_error(test_type, "NULL", "Vector* s"); status = FAILURE; }
  return status;
}


Status test_vector_size_null_vector(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Vector* s = NULL;
  if (vector_size(s) != -1) { print_error(test_type, "-1", "return value vector_size(Vector* s)"); status = FAILURE; }
  vector_destroy(&s);
  return status;
}

Status test_vector_size_null_items(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Vector* s = vector_create(); free(s->items); s->items = NULL;
  if (vector_size(s) != -1) { print_error(test_type, "-1", "return value vector_size(Vector* s)"); status = FAILURE; }
  vector_destroy(&s);
  return status;
}

Status test_vector_size_empty(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Vector* s = vector_create();
  if (vector_size(s)) { print_error(test_type, "0", "return value vector_size(Vector* s)"); status = FAILURE; }
  vector_destroy(&s);
  return status;
}

Status test_vector_push_null_vector(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Vector* s = NULL; void* item = NULL;
  vector_push(s, item);
  if (vector_size(s) != -1) { print_error(test_type, "-1", "vector_push(Vector* s, void* item)"); status = FAILURE; }
  vector_destroy(&s);
  return status;
}

Status test_vector_push_null_items(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Vector* s = vector_create(); free(s->items); s->items = NULL; void* item = NULL;
  vector_push(s, item);
  if (vector_size(s) != -1) { print_error(test_type, "-1", "vector_push(Vector* s, void* item)"); status = FAILURE; }
  vector_destroy(&s);
  return status;
}

Status test_vector_push(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Vector* s = vector_create(); void* item = (void*)"duck";
  vector_push(s, item); int top = vector_size(s) - 1;
  if (!vector_size(s)) { print_error(test_type, "1", "return value vector_size(Vector* s) after push"); status = FAILURE; }
  if (strcmp(s->items[top], "duck")) { print_error(test_type, "\"duck\"", "(char*)s->items[top]"); status = FAILURE; }
  vector_destroy(&s);
  return status;
}

Status test_vector_pop(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Vector* s = vector_create(); void* item = (void*)"duck";
  vector_push(s, item); void* ret = vector_pop(s);
  if (vector_size(s)) { print_error(test_type, "0", "return value vector_size(Vector* s)"); status = FAILURE; }
  if (strcmp((char*)ret, "duck")) { print_error(test_type, "\"duck\"", "return value of vector_pop(Vector* s)"); status = FAILURE; }
  vector_destroy(&s);
  return status;
}

Status test_vector_pop_null_vector(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Vector* s = NULL;
  vector_pop(s);
  if (vector_size(s) != -1) { print_error(test_type, "-1", "vector size vector_pop(Vector* s)"); status = FAILURE; }
  vector_destroy(&s);
  return status;
}

Status test_vector_pop_null_items(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Vector* s = vector_create(); free(s->items); s->items = NULL;
  vector_pop(s);
  if (vector_size(s) != -1) { print_error(test_type, "-1", "vector size after vector_pop(Vector* s)"); status = FAILURE; }
  vector_destroy(&s);
  return status;
}

Status test_vector_pop_empty(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Vector* s = vector_create(); vector_pop(s);
  if (vector_size(s)) { print_error(test_type, "0", "vector size after vector_pop(Vector* s)"); status = FAILURE; }
  vector_destroy(&s);
  return status;
}

Status test_vector_push_refit(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Vector* s = vector_create(); void* item = (void*)"duck"; int cap = s->capacity + 1;
  for (int i = 0; i < cap; i++) vector_push(s, item);
  if (s->capacity != (1<<4)) { print_error(test_type, "16 (1<<4)", "refitting items in vector_push(Vector* s, void* item)"); status = FAILURE; }
  if (s->size != cap) { print_error(test_type, "9", "vector size after push"); status = FAILURE; }
  vector_destroy(&s);
  return status;
}

Status test_vector_pop_refit(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Vector* s = vector_create(); void* item = (void*)"duck"; int cap = (s->capacity >> 1);
  for (int i = 0; i < cap; i++) { vector_push(s, item); } vector_pop(s);
  if (s->capacity != (1<<2)) { print_error(test_type, "4 (1<<2)", "refitting items in vector_pop(Vector* s)"); status = FAILURE; }
  if (s->size != (cap - 1)) { print_error(test_type, "3", "vector size after pop"); status = FAILURE; }
  vector_destroy(&s);
  return status;
}

Status test_vector_item_null_vector(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Vector* s = NULL;
  void* item = vector_item(s, 0);
  if (item) { print_error(test_type, "NULL", "return value of vector_item(Vector* s, int position)"); status = FAILURE; }
  vector_destroy(&s);
  return status;
}

Status test_vector_item_null_items(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Vector* s = vector_create(); free(s->items); s->items = NULL;
  void* item = vector_item(s, 0);
  if (item) { print_error(test_type, "NULL", "return value of vector_item(Vector* s, int position)"); status = FAILURE; }
  vector_destroy(&s);
  return status;
}

Status test_vector_item_out_of_bounds_low(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Vector* s = vector_create(); void* item = vector_item(s, -1);
  if (item) { print_error(test_type, "NULL", "return value of vector_item(Vector* s, int position)"); status = FAILURE; }
  vector_destroy(&s);
  return status;
}

Status test_vector_item_out_of_bounds_high(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Vector* s = vector_create(); void* item = vector_item(s, 1);
  if (item) { print_error(test_type, "NULL", "return value of vector_item(Vector* s, int position)"); status = FAILURE; }
  vector_destroy(&s);
  return status;
}

Status test_vector_item(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Vector* s = vector_create();
  vector_push(s, (void*)"duck"); int top = vector_size(s) - 1; void* item = vector_item(s, top);
  if (!item) { print_error(test_type, "NOT NULL", "return value of vector_item(Vector* s, int position)"); status = FAILURE; }
  if (strcmp((char*)item, "duck")) { print_error(test_type, "\"duck\"", "(char*)item"); status = FAILURE; }
  vector_destroy(&s);
  return status;
}

Status test_hash_symbol_insert(void) {
  strcpy(test_type, __FUNCTION__);
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
  strcpy(test_type, __FUNCTION__);
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
  strcpy(test_type, __FUNCTION__);
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
  strcpy(test_type, __FUNCTION__);
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
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create();
  struct hash_table* ht = hash_table_create(0, 0);
  struct symbol* s = symbol_create(SYMBOL_GLOBAL, NULL, strdup("bar"));
  if (!st) { print_error(test_type, "NOT NULL", "Symbol_table* st"); status = FAILURE; }
  if (!ht) { print_error(test_type, "NOT NULL", "struct hash_table* ht"); status = FAILURE; }
  if (!s) { print_error(test_type, "NOT NULL", "struct symbol* s"); status = FAILURE; }

  status = hash_table_insert(ht, "foo", (void*)s);
  vector_push(st->vector, ht);
  symbol_table_destroy(&st); ht = NULL; s = NULL; // tests keep failing here.
  if (st) { print_error(test_type, "NULL", "Symbol_table* st"); status = FAILURE; }
  if (ht) { print_error(test_type, "NULL", "struct hash_table* ht"); status = FAILURE; }
  if (s) { print_error(test_type, "NULL", "struct symbol* s"); status = FAILURE; }
  return status;
}

Status test_symbol_table_scope_level(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create();
  struct hash_table* ht = hash_table_create(0, 0);
  if (!st) { print_error(test_type, "NOT NULL", "Symbol_table* st"); status = FAILURE; }
  if (!ht) { print_error(test_type, "NOT NULL", "struct hash_table* ht"); status = FAILURE; }

  vector_push(st->vector, ht); st->top++;
  if (symbol_table_scope_level(st) != 1) { print_error(test_type, "1", "int symbol_table_scope_level(Symbol_table* st)"); status = FAILURE; }
  if (hash_table_size(ht)) { print_error(test_type, "0", "int hash_table_size(struct hash_table* ht)"); status = FAILURE; }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_enter(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create();
  if (!st) { print_error(test_type, "NOT NULL", "Symbol_table* st"); status = FAILURE; }
  symbol_table_scope_enter(st);
  if (symbol_table_scope_level(st) != 1) { print_error(test_type, "1", "int symbol_table_scope_level(Symbol_table* st)"); status = FAILURE; }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_enter_multi(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create();
  if (!st) { print_error(test_type, "NOT NULL", "Symbol_table* st"); status = FAILURE; }
  symbol_table_scope_enter(st); symbol_table_scope_enter(st); symbol_table_scope_enter(st);
  if (symbol_table_scope_level(st) != 3) { print_error(test_type, "3", "int symbol_table_scope_level(Symbol_table* st)"); status = FAILURE; }
  for (int i = st->vector->size - 1; i >= 0; i--) {
    if (!st->vector->items[i]) { print_error(test_type, "NOT NULL", "hash tables after scope entry"); status = FAILURE; }
  }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_exit(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create();
  if (!st) { print_error(test_type, "NOT NULL", "Symbol_table* st"); status = FAILURE; }
  symbol_table_scope_enter(st); symbol_table_scope_exit(st);
  if (symbol_table_scope_level(st)) { print_error(test_type, "0", "int symbol_table_scope_level(Symbol_table* st)"); status = FAILURE; }
  if (st->vector->items[vector_size(st->vector)]) { print_error(test_type, "NULL", "pointer value of topmost scope"); status = FAILURE; }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_bind(void) {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create();
  struct symbol* s = symbol_create(SYMBOL_GLOBAL, NULL, strdup("bar"));
  if (!st) { print_error(test_type, "NOT NULL", "Symbol_table* st"); status = FAILURE; }
  if (!s) { print_error(test_type, "NOT NULL", "struct symbol* s"); status = FAILURE; }

  symbol_table_scope_enter(st); int top = vector_size(st->vector) - 1;
  symbol_table_scope_bind(st, "foo", s);
  if (!hash_table_size(st->vector->items[top])) { print_error(test_type, "1", "int hash_table_size(struct hash_table* ht)"); status = FAILURE; }
  if (!hash_table_lookup(st->vector->items[top], "foo")) {
    print_error(test_type, "NOT NULL", "void* hash_table_lookup(struct hash_table* h, const char* key, void* value)");
    status = FAILURE;
  }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_bind_null(void)  {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create();
  struct hash_table* ht = NULL;
  struct symbol* s = symbol_create(SYMBOL_GLOBAL, NULL, strdup("bar"));
  if (!st) { print_error(test_type, "NOT NULL", "Symbol_table* st"); status = FAILURE; }
  if (ht) { print_error(test_type, "NULL", "struct hash_table* ht"); status = FAILURE; }
  if (!s) { print_error(test_type, "NOT NULL", "struct symbol* s"); status = FAILURE; }

  vector_push(st->vector, ht); int top = vector_size(st->vector) - 1;
  symbol_table_scope_bind(st, "foo", s);
  if (st->vector->items[top]) {
    print_error(test_type, "NULL", "struct hash_table* st->items[top]");
    status = FAILURE;
  }
  symbol_table_destroy(&st); symbol_destroy(&s);
  return status;
}

Status test_symbol_table_scope_lookup_null_vector(void) {
  strcpy(test_type, __FUNCTION__);
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
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create(); free(st->vector->items); st->vector->items = NULL;
  if (symbol_table_scope_lookup(st, "foo")) {
    print_error(test_type, "NULL", "struct symbol* symbol_table_scope_lookup(Symbol_table* st, const char* name)");
    status = FAILURE;
  }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_lookup_empty_vector(void) {
  strcpy(test_type, __FUNCTION__);
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
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create();
  struct hash_table* ht = NULL;
  vector_push(st->vector, ht);
  if (symbol_table_scope_lookup(st, "foo")) {
    print_error(test_type, "NULL", "struct symbol* symbol_table_scope_lookup(Symbol_table* st, const char* name)");
    status = FAILURE;
  }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_lookup_empty_table(void) {
  strcpy(test_type, __FUNCTION__);
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
  strcpy(test_type, __FUNCTION__);
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
  strcpy(test_type, __FUNCTION__);
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
  strcpy(test_type, __FUNCTION__);
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

Status test_symbol_table_scope_lookup_current_null_vector(void) {
  strcpy(test_type, __FUNCTION__);
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
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create(); free(st->vector->items); st->vector->items = NULL;
  if (symbol_table_scope_lookup_current(st, "foo")) {
    print_error(test_type, "NULL", "struct symbol* symbol_table_scope_lookup_current(Symbol_table* st, const char* name)");
    status = FAILURE;
  }
  symbol_table_destroy(&st);
  return status;
}

Status test_symbol_table_scope_lookup_current_empty_vector(void) {
  strcpy(test_type, __FUNCTION__);
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
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Symbol_table* st = symbol_table_create();
  struct hash_table* ht = NULL;
  vector_push(st->vector, ht);
  if (symbol_table_scope_lookup_current(st, "foo")) {
    print_error(test_type, "NULL", "struct symbol* symbol_table_scope_lookup_current(Symbol_table* st, const char* name)");
    status = FAILURE;
  }
  symbol_table_destroy(&st);
  return status;
}
Status test_symbol_table_scope_lookup_current_empty_table(void) {
  strcpy(test_type, __FUNCTION__);
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
  strcpy(test_type, __FUNCTION__);
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
  strcpy(test_type, __FUNCTION__);
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
  strcpy(test_type, __FUNCTION__);
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

Status test_symbol_table_hidden_create() {
strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Hidden_table* hst = symbol_table_hidden_create();
  if (!hst) { print_error(test_type, "NOT NULL", "Hidden_table* s"); return FAILURE; }
  if (hash_table_size(hst) != 0) {
    print_error(test_type, "0", "hash_table_size(Hidden_table* hst");
    status = FAILURE;
  }
  hash_table_delete(hst);
  return status;
}
Status test_symbol_table_hidden_destroy() {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Hidden_table* hst = symbol_table_hidden_create();
  if (!hst) { print_error(test_type, "NOT NULL", "Hidden_table* s"); return FAILURE; }
  symbol_table_hidden_destroy(&hst);
  if (hst) { print_error(test_type, "NULL", "Hidden_table* s"); status = FAILURE; }
  return status;
}

Status test_symbol_table_hidden_bind() {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Hidden_table* hst = symbol_table_hidden_create();
  const char* label = ".L0";
  if (!hst) { print_error(test_type, "NOT NULL", "Hidden_table* st"); status = FAILURE; }
  symbol_table_hidden_bind(hst, "foo", strdup(label));
  if (hash_table_size(hst) != 1) {
    print_error(test_type, "1", "int hash_table_size(struct hash_table* ht)"); status = FAILURE;
  }
  if (!hash_table_lookup(hst, "foo")) {
    print_error(test_type, "NOT NULL", "void* hash_table_lookup(struct hash_table* h, const char* key, void* value)");
    status = FAILURE;
  }
  symbol_table_hidden_destroy(&hst);
  return status;
}

Status test_symbol_table_hidden_lookup_empty() {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  Hidden_table* hst = symbol_table_hidden_create();
  if (symbol_table_hidden_lookup(hst, "foo")) {
    print_error(test_type, "NULL", "const char* symbol_table_hidden_lookup(Hidden_table* st, const char* literal)");
    status = FAILURE;
  }
  symbol_table_hidden_destroy(&hst);
  return status;
}

Status test_symbol_table_hidden_lookup_bad_key() {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  const char* label = ".L0";
  Hidden_table* hst = symbol_table_hidden_create();
  symbol_table_hidden_bind(hst, "foo", strdup(label));
  if (symbol_table_hidden_lookup(hst, "bar")) {
    print_error(test_type, "NULL", "const char* symbol_table_hidden_lookup(Hidden_table* st, const char* literal)");
    status = FAILURE;
  }
  symbol_table_hidden_destroy(&hst);
  return status;
}

Status test_symbol_table_hidden_lookup_good_key() {
  strcpy(test_type, __FUNCTION__);
  Status status = SUCCESS;
  const char* label = ".L0";
  Hidden_table* hst = symbol_table_hidden_create();
  symbol_table_hidden_bind(hst, "foo", strdup(label));
  const char* found_label = symbol_table_hidden_lookup(hst, "foo");
  if (!found_label) {
    print_error(test_type, "NOT NULL", "const char* symbol_table_hidden_lookup(Hidden_table* st, const char* literal)");
    status = FAILURE;
  }
  if (strcmp(found_label, label) != 0) {
    print_error(test_type, "0 (equal)", "const char* found_label, .L0");
    status = FAILURE;
  }
  symbol_table_hidden_destroy(&hst);
  return status;
}