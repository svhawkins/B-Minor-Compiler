#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../source/symbol_table.h"
#define MAX_BUFFER 256

typedef enum { FAILURE = 0, SUCCESS = 1 } Status;
char test_type[MAX_BUFFER];
char output[MAX_BUFFER];

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

int main(void) {
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
    test_stack_item
  };

  int n_tests = sizeof(tests)/sizeof(tests[0]);
  int n_pass = 0;

  printf("Running %d tests...\n", n_tests);
  for (int i = 0; i < n_tests; i++) { if (tests[i]()) { n_pass++; }}

  printf("Passed: %d/%d\n", n_pass, n_tests);
  printf("Failed: %d/%d\n", (n_tests - n_pass), n_tests);

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
  Stack* s = stack_create(); void* item = (void*)"duck"; int top = 0;
  stack_push(s, item);
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
  Stack* s = stack_create(); int top = 0;
  stack_push(s, (void*)"duck"); void* item = stack_item(s, top);
  if (!item) { print_error(test_type, "NOT NULL", "return value of stack_item(Stack* s, int position)"); status = FAILURE; }
  if (strcmp((char*)item, "duck")) { print_error(test_type, "\"duck\"", "(char*)item"); status = FAILURE; }
  stack_destroy(&s);
  return status;
}
