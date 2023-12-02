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

extern FILE *REG_ERR_OUT, *ERR_OUT;

/* helper function tests */
Status test_scratch_alloc_success(void);
Status test_scratch_alloc_fail_ainuse(void);
Status test_scratch_free_success(void);
Status test_scratch_free_fail_invalid(void);
Status test_scratch_free_fail_ninuse(void);
Status test_scratch_name_success(void);
Status test_scratch_name_fail_invalid(void);
Status test_label_create_success(void);
Status test_label_create_fail_max(void);
Status test_label_name(void);

/* code generation tests */
Status test_symbol_codegen(void);

int main(int argc, const char* argv[]) {
  ERR_OUT = REG_ERR_OUT = fopen("error_output_codegen.txt", "w");
  if (!ERR_OUT || !REG_ERR_OUT) { ERR_OUT = REG_ERR_OUT = stderr; }
  Status (*tests[])(void) = {
       test_scratch_alloc_success,
       test_scratch_alloc_fail_ainuse,
       test_scratch_free_success,
       test_scratch_free_fail_invalid,
       test_scratch_free_fail_ninuse,
       test_scratch_name_success,
       test_scratch_name_fail_invalid,
       test_label_create_success,
       test_label_create_fail_max,
       test_label_name,
       test_symbol_codegen,
  };
  int n_tests = sizeof(tests)/sizeof(tests[0]);
  int n_pass = 0;

  printf("Running %d tests...\n", n_tests);
  for (int i = 0; i < n_tests; i++) { if (tests[i]()) { n_pass++; }}

  printf("Passed: %d/%d\n", n_pass, n_tests);
  printf("Failed: %d/%d\n", (n_tests - n_pass), n_tests);
  fclose(REG_ERR_OUT); // no ERR_OUT as it just points to REG_ERR_OUT
  return 0;
}

void print_error(char* test, char* expect, char* value) {
  printf("Error:\t[%s]:\n\tExpected a value of %s for value: [%s]\n", test, expect, value);
}

Status test_scratch_alloc_success(void) {
  strcpy(test_type, "Testing: test_scratch_alloc_success");
  Status status = SUCCESS; bool test = true;
  register_codegen_init(test);
  int r = register_scratch_alloc();
  if (r != 0) { print_error(test_type, "0", "int r: register index"); status = FAILURE; }
  if (!scratch_register[r].inuse) { print_error(test_type, "true", "bool scratch_register[0].inuse"); status = FAILURE; }
  if (register_error_count) { print_error(test_type, "0", "int register_error_count"); status = FAILURE; }
  if (register_error_status) { print_error(test_type, "0", "int register_error_status"); status = FAILURE; }
  register_codegen_clear();
  return status;
}

Status test_scratch_alloc_fail_ainuse(void) {
  strcpy(test_type, "Testing: test_scratch_alloc_fail_ainuse");
  Status status = SUCCESS; bool test = true;
  register_codegen_init(test);
  int r = 9;
  for (int i = 0; i < NSCRATCH; i++) { r = register_scratch_alloc(); }
  r = register_scratch_alloc();
  if (r != -1) { print_error(test_type, "-1", "int r: register index"); status = FAILURE; }
  if (!register_error_count) { print_error(test_type, "1", "int register_error_count"); status = FAILURE; }
  if (!register_error_status) { print_error(test_type, "REG_AINUSE", "int register_error_status"); status = FAILURE; }
  register_codegen_clear();
  return status;
}

Status test_scratch_free_success(void){
  strcpy(test_type, "Testing: test_scratch_free_success");
  Status status = SUCCESS; bool test = true;
  register_codegen_init(test);
  int r = register_scratch_alloc(); register_scratch_free(r);
  if (scratch_register[r].inuse) { print_error(test_type, "false", "bool scratch_register[0].inuse"); status = FAILURE; }
  if (register_error_count) { print_error(test_type, "0", "int register_error_count"); status = FAILURE; }
  if (register_error_status) { print_error(test_type, "0", "int register_error_status"); status = FAILURE; }
  register_codegen_clear();
  return status;
}

Status test_scratch_free_fail_invalid(void) {
  strcpy(test_type, "Testing: test_scratch_alloc_fail_invalid");
  Status status = SUCCESS; bool test = true;
  register_codegen_init(test);
  int r = 9; register_scratch_free(r);
  if (!register_error_count) { print_error(test_type, "1", "int register_error_count"); status = FAILURE; }
  if (!register_error_status) { print_error(test_type, "REG_INVALID", "int register_error_status"); status = FAILURE; }
  register_codegen_clear();
  return status;
}

Status test_scratch_free_fail_ninuse(void) {
  strcpy(test_type, "Testing: test_scratch_alloc_fail_ninuse");
  Status status = SUCCESS; bool test = true;
  register_codegen_init(test);
  int r = register_scratch_alloc(); register_scratch_free(r); register_scratch_free(r);
  if (!register_error_count) { print_error(test_type, "1", "int register_error_count"); status = FAILURE; }
  if (!register_error_status) { print_error(test_type, "REG_NINUSE", "int register_error_status"); status = FAILURE; }
  register_codegen_clear();
  return status;
}

Status test_scratch_name_success(void) {
  strcpy(test_type, "Testing: test_scratch_name_fail_invalid");
  Status status = SUCCESS; bool test = true;
  register_codegen_init(test);

  for (int i = 0; i < NSCRATCH; i++) {
    if (strcmp(scratch_register[i].name, register_scratch_name(i))) {
	print_error(test_type, scratch_register[i].name, "const char* register_scratch_name(r)");
	status = FAILURE;
    }
    if (register_error_count) { print_error(test_type, "0", "int register_error_count"); status = FAILURE; }
    if (register_error_status) { print_error(test_type, "0", "int register_error_status"); status = FAILURE; }
  }
  register_codegen_clear();
  return status;
}

Status test_scratch_name_fail_invalid(void) {
  strcpy(test_type, "Testing: test_scratch_name_fail_invalid");
  Status status = SUCCESS; bool test = true;
  register_codegen_init(test);
  int r = 9; const char* s = register_scratch_name(r);
  if (!register_error_count) { print_error(test_type, "1", "int register_error_count"); status = FAILURE; }
  if (!register_error_status) { print_error(test_type, "REG_INVALID", "int register_error_status"); status = FAILURE; }
  register_codegen_clear();
  return status;
}

Status test_label_create_success(void) {
  strcpy(test_type, "Testing: test_label_create_success");
  Status status = SUCCESS; bool test = true;
  register_codegen_init(test);
  if (label_count > 0) { print_error(test_type, "-1", "int label_count"); return FAILURE; }
  int label = label_create();
  if (label != 0) { print_error(test_type, "0", "int label"); status = FAILURE; }
  if (label != label_count) { print_error(test_type, "0", "int label && int label_count"); status = FAILURE; }
  if (register_error_count) { print_error(test_type, "0", "int register_error_count"); status = FAILURE; }
  if (register_error_status) { print_error(test_type, "0", "int register_error_status"); status = FAILURE; }
  register_codegen_clear();
  return status;
}

Status test_label_create_fail_max(void) {
  strcpy(test_type, "Testing: test_label_create_fail_max");
  Status status = SUCCESS; bool test = true;
  register_codegen_init(test);
  label_count = INT_MAX;
  int label = label_create();
  if (label != INT_MIN) { print_error(test_type, "INT_MIN", "int label"); status = FAILURE; }
  if (label != label_count) { print_error(test_type, "INT_MIN", "int label && int label_count"); status = FAILURE; }
  if (!register_error_count) { print_error(test_type, "1", "int register_error_count"); status = FAILURE; }
  if (!register_error_status) { print_error(test_type, "LABEL_MAX", "int register_error_status"); status = FAILURE; }
  register_codegen_clear();
  return status;
}

Status test_label_name(void) {
  strcpy(test_type, "Testing: test_label_name");
  Status status = SUCCESS; bool test = true;
  register_codegen_init(test);
  int label_num = label_create(); const char* label = label_name(label_num);
  if (strcmp(label, ".L0")) { print_error(test_type, ".L0", "const char* label_name"); status = FAILURE; }
  if (register_error_count) { print_error(test_type, "0", "int register_error_count"); status = FAILURE; }
  if (register_error_status) { print_error(test_type, "0", "int register_error_status"); status = FAILURE; }
  register_codegen_clear();
  return status;
}

Status test_symbol_codegen(void) {
  strcpy(test_type, "Testing: test_symbol_codegen");
  Status status = SUCCESS;
  struct type* tvoid = type_create(TYPE_VOID, NULL, NULL, NULL);
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct param_list* p = param_list_create(strdup("x"), type_copy(integer), NULL);
  struct type* tfuncv = type_create(TYPE_FUNCTION, type_copy(tvoid), p, NULL);
  struct stmt* s = stmt_create(STMT_DECL, decl_create(strdup("y"), type_copy(integer), NULL, NULL, NULL), NULL, NULL, NULL, NULL, NULL, NULL);
  struct stmt* function_body = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, s, NULL, NULL);
  struct decl* d = decl_create(strdup("foo"), type_copy(tfuncv), NULL, function_body, NULL);

  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  error_status = decl_resolve(st, d); error_status = decl_typecheck(st, d);
  struct symbol* sym_foo = symbol_table_scope_lookup_all(st, "foo");
  struct symbol* sym_x = symbol_table_scope_lookup_all(st, "x");
  struct symbol* sym_y = symbol_table_scope_lookup_all(st, "y");
  if (!sym_foo) { print_error(test_type, "NOT NULL", "struct symbol* sym_foo"); return FAILURE; }
  if (!sym_x) { print_error(test_type, "NOT NULL", "struct symbol* sym_x"); return FAILURE; }
  if (!sym_y) { print_error(test_type, "NOT NULL", "struct symbol* sym_y"); return FAILURE; }
  if (sym_x->which != 0) { print_error(test_type, "0", "int sym_x->which"); return FAILURE; }
  if (sym_y->which != 1) { print_error(test_type, "1", "int sym_y->which"); return FAILURE; }

  const char* foo_addr = symbol_codegen(sym_foo);
  const char* x_addr = symbol_codegen(sym_x);
  const char* y_addr = symbol_codegen(sym_y);
  if (strcmp("foo", sym_foo->address)) {
    print_error(test_type, "foo", "const char* symbol_codegen(struct symbol* sym_foo)");
    status = FAILURE;
  }
  if (strcmp("-8(%rbp)", sym_x->address)) {
    print_error(test_type, "-8(%rbp)", "const char* symbol_codegen(struct symbol* sym_x)");
    status = FAILURE;
  }
  if (strcmp("-16(%rbp)", sym_y->address)) {
    print_error(test_type, "-16(%rbp)", "const char* symbol_codegen(struct symbol* sym_y)");
    status = FAILURE;
  }
  symbol_table_destroy(&st); type_destroy(&tfuncv); type_destroy(&tvoid); type_destroy(&integer); decl_destroy(&d);
  return status;
}
