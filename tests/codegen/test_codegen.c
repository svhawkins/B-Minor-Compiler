#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../src/symbol_table.h"
#define MAX_BUFFER 512 // fcall may have a lot of characters.

typedef enum { FAILURE = 0, SUCCESS = 1 } Status;
char test_type[MAX_BUFFER];
char buffer[MAX_BUFFER];
void print_error(char* test, char* expect, char* value);

extern FILE *REG_ERR_OUT, *ERR_OUT, *CODEGEN_OUT;

/* helper function tests */
Status test_scratch_alloc_success(void);
Status test_scratch_alloc_fail_ainuse(void);
Status test_scratch_free_success(void);
Status test_scratch_free_fail_invalid(void);
Status test_scratch_free_fail_ninuse(void);
Status test_scratch_name_success(void);
Status test_scratch_name_fail_invalid(void);
Status test_scratch_name_fail_nohigh(void);
Status test_label_create_success(void);
Status test_label_create_fail_max(void);
Status test_label_name(void);

/* code generation tests */

// symbols
Status test_symbol_codegen(void);

// primitive expressions
Status test_expr_codegen_literal(void);
Status test_expr_codegen_string(void);
Status test_expr_codegen_name_literal(void);

// simple arithmetic + logical expressions (arithmetic + logical)
Status test_expr_codegen_binary(void);
Status test_expr_codegen_unary(void);
Status test_expr_codegen_muldivmod(void);


// codegen tests that emit errors/warnings
Status test_expr_codegen_divmod_zero(void);
Status test_expr_codegen_overflow(void);
Status test_expr_codegen_underflow(void);
Status test_expr_codegen_mult_underflow_overflow(void);

// relational expressions
Status test_expr_codegen_relate(void);

// array expressions
// Status test_expr_codegen_array_literal(void); // TODO --> expr inint
// Status test_expr_codegen_array_string(void); // TODO --> expr init
// subscript stuff here

// fcall

// declarations
Status test_decl_codegen_literal_global(void); // covers init and uninit
Status test_decl_codegen_literal_local(void);
Status test_decl_codegen_string_literal_global(void);
Status test_decl_codegen_string_literal_local(void);
// Status test_decl_codegen_array_literal_global(void);
// Status test_decl_codegen_array_literal_local(void);
// Status test_decl_codegen_array_string_literal_global(void);
// Status test_decl_codegen_array_string_literal_local(void);
// Status test_decl_codegen_array_size_mismatch(void);
// Status test_decl_codegen_array_size_infer_uninit(void);
// Status test_decl_codegen_array_size_infer_init(void);

/*
  test function preamables:
    strcpy(test_type, "Testing: <function name>");
    Status status = FAILURE;
*/

/*
tmp files
  CODEGEN_OUT = fopen("foo.txt", "w"); if (!CODEGEN_OUT) { return file_error(test_type); }
  <struct>_codegen(struct);
  CODEGEN_OUT = freopen("foo.txt", "r", CODEGEN_OUT); if (!CODEGEN_OUT) { return file_error(test_type); }
  fileread(CODEGEN_OUT, output, MAX_BUFFER); remove("foo.txt");
*/

// helper functions

// reads contents of file into string s
void fileread(FILE* fp, char* s, int n) {
  int i; for (i = 0; i < n && !feof(fp); i++) s[i] = fgetc(fp);
  s[i-1] = '\0'; fclose(fp); fp = NULL;
}

void compare_expect_output(char* expect, char* output) {
 long int n = strlen(expect), m = strlen(output); int i;
  printf("expect length: %ld\toutput length: %ld\n", n, m);
  printf("expect == output\n");
  for (i = 0; i < (n < m) ? n : m; i++) {
    if (expect[i] != output[i]) break;
    printf("[%c] == [%c]\n", expect[i], output[i]);
  }
  printf("%d: expect: [%c]\toutput: [%c]\n", i, expect[i], output[i]);
}

Status file_error(char* test_type) {
  printf("Failed to open temporary I/O file for %s.\n Test failure.\n", test_type);
  return FAILURE;
}

void print_error(char* test, char* expect, char* value) { printf("Error:\t[%s]:\n\tExpected:\n[%s]\n\tReceived:\n[%s]\n", test, expect, value); }

int main(void) {
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
       test_scratch_name_fail_nohigh,
       test_label_create_success,
       test_label_create_fail_max,
       test_label_name,
       test_symbol_codegen,
       test_expr_codegen_literal,
       test_expr_codegen_string,
       test_expr_codegen_name_literal,
       test_expr_codegen_binary,
       test_expr_codegen_unary,
       test_expr_codegen_muldivmod,
       test_expr_codegen_relate,
       test_expr_codegen_divmod_zero,
       test_expr_codegen_overflow,
       test_expr_codegen_mult_underflow_overflow,
       test_expr_codegen_underflow,
       test_decl_codegen_literal_global,
       test_decl_codegen_literal_local,
       test_decl_codegen_string_literal_global,
       test_decl_codegen_string_literal_local
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
  if (!register_error_status || register_error_status != REG_AINUSE) {
    print_error(test_type, "REG_AINUSE", "int register_error_status");
    status = FAILURE;
  }
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
  strcpy(test_type, "Testing: test_scratch_free_fail_invalid");
  Status status = SUCCESS; bool test = true;
  register_codegen_init(test);
  int r = 9; register_scratch_free(r);
  if (!register_error_count) { print_error(test_type, "1", "int register_error_count"); status = FAILURE; }
  if (!register_error_status || register_error_status != REG_INVALID) {
    print_error(test_type, "REG_INVALID", "int register_error_status");
    status = FAILURE;
  }
  register_codegen_clear();
  return status;
}

Status test_scratch_free_fail_ninuse(void) {
  strcpy(test_type, "Testing: test_scratch_alloc_fail_ninuse");
  Status status = SUCCESS; bool test = true;
  register_codegen_init(test);
  int r = register_scratch_alloc(); register_scratch_free(r); register_scratch_free(r);
  if (!register_error_count) { print_error(test_type, "1", "int register_error_count"); status = FAILURE; }
  if (!register_error_status || register_error_status != REG_NINUSE) {
    print_error(test_type, "REG_NINUSE", "int register_error_status");
    status = FAILURE;
  }
  register_codegen_clear();
  return status;
}

Status test_scratch_name_success(void) {
  strcpy(test_type, "Testing: test_scratch_name_success");
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
  if (!register_error_status || register_error_status != REG_INVALID) {
    print_error(test_type, "REG_INVALID", "int register_error_status");
    status = FAILURE;
  }
  register_codegen_clear();
  return status;
}

Status test_scratch_name_fail_nohigh(void) {
  strcpy(test_type, "Testing: test_scratch_name_fail_nohigh");
  Status status = SUCCESS; bool test = true;
  register_codegen_init(test);

  for (int i = 1; i < NSCRATCH; i++) {
    register_error_count = 0;
    const char* s = register_scratch_name_high(i);
    if (!register_error_count) { print_error(test_type, "1", "int register_error_count"); status = FAILURE; }
    if (!register_error_status || register_error_status != REG_NOHIGH) {
      print_error(test_type, "REG_NOHIGH", "int register_error_status");
      status = FAILURE;
    }
  }
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

Status test_expr_codegen_literal(void) {
  strcpy(test_type, "Testing: test_expr_codegen_literal");
  Status status = SUCCESS;
  struct expr* exprs[3] = { expr_create_integer_literal(42),
                            expr_create_char_literal('A'),
                            expr_create_boolean_literal(false) 
                          };
char expect[MAX_BUFFER >> 1];
strcpy(expect, "MOVQ $42, %rbx\nMOVQ $65, %rbx\nMOVQ $0, %rbx\n");
CODEGEN_OUT = fopen("foo.txt", "w"); if (!CODEGEN_OUT) { return file_error(test_type); }
  for (int i = 0; i < 3; i++) {
    struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
    register_codegen_init(true);
    error_status = expr_resolve(st, exprs[i]);
    struct type* t; t = expr_typecheck(st, exprs[i]); type_destroy(&t);
    error_status = expr_codegen(st, exprs[i]);
    if (exprs[i]->reg != 0) { print_error(test_type, "0", "int exprs[i]->reg" ); return FAILURE; }
    if (!scratch_register[exprs[i]->reg].inuse) {
      print_error(test_type, "true", "bool scratch_register[0].inuse");
      status = FAILURE;
    }
    expr_destroy(&exprs[i]);
    symbol_table_destroy(&st);
    register_codegen_clear();
  }
  CODEGEN_OUT = freopen("foo.txt", "r", CODEGEN_OUT); if (!CODEGEN_OUT) { return file_error(test_type); }
  fileread(CODEGEN_OUT, buffer, MAX_BUFFER); remove("foo.txt");
  if (strcmp(expect, buffer) != 0) { print_error(test_type, expect, buffer); status = FAILURE; }
  return status;
}


Status test_expr_codegen_string(void) {
  strcpy(test_type, "Testing: test_expr_codegen_string");
  Status status = SUCCESS;
  char expect[MAX_BUFFER >> 1];
  strcpy(expect, "LEAQ .L0, %rbx\n");
  register_codegen_init(true);
  CODEGEN_OUT = fopen("foo.txt", "w"); if (!CODEGEN_OUT) { return file_error(test_type); }

  struct expr* e = expr_create_string_literal("foo"); struct type* t;
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
  error_status = expr_resolve(st, e);
  t = expr_typecheck(st, e); type_destroy(&t);
  error_status = expr_codegen(st, e);

  if (e->reg != 0) { print_error(test_type, "0", "int e->reg" ); return FAILURE; }
  if (!scratch_register[e->reg].inuse) {
    print_error(test_type, "true", "bool scratch_register[0].inuse");
    status = FAILURE;
  }

  register_codegen_clear();
  expr_destroy(&e);
  symbol_table_destroy(&st);

  CODEGEN_OUT = freopen("foo.txt", "r", CODEGEN_OUT); if (!CODEGEN_OUT) { return file_error(test_type); }
  fileread(CODEGEN_OUT, buffer, MAX_BUFFER); remove("foo.txt");
  if (strcmp(expect, buffer) != 0) { print_error(test_type, expect, buffer); status = FAILURE; }
  return status;
}

Status test_expr_codegen_name_literal(void) {
  strcpy(test_type, "Testing: test_expr_codegen_name_literal");
  Status status = SUCCESS;
  char expect[MAX_BUFFER >> 1];
  strcpy(expect, "MOVQ foo, %rbx\nMOVQ -8(%rbp), %r10\n");
  register_codegen_init(true);
  CODEGEN_OUT = fopen("foo.txt", "w"); if (!CODEGEN_OUT) { return file_error(test_type); }

  register_codegen_init(true);
  struct expr* global_expr = expr_create_name(strdup("foo"));
  struct expr* local_expr = expr_create_name(strdup("bar"));
  struct type* t;

  // add in symbols (here as not testing decl stuff yet)
  struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);

  // global
  error_status = expr_resolve(st, global_expr);
  t = expr_typecheck(st, global_expr); type_destroy(&t);
  error_status = expr_codegen(st, global_expr);

  // local
  symbol_table_scope_enter(st);
  error_status = expr_resolve(st, local_expr);
  t = expr_typecheck(st, local_expr); type_destroy(&t);
  error_status = expr_codegen(st, local_expr);

  if (global_expr->reg != 0) { print_error(test_type, "0", "int global_expr->reg" ); return FAILURE; }
  if (!scratch_register[global_expr->reg].inuse) {
    print_error(test_type, "true", "bool scratch_register[0].inuse");
    status = FAILURE;
  }
  if (local_expr->reg != 1) { print_error(test_type, "0", "int global_expr->reg" ); return FAILURE; }
  if (!scratch_register[local_expr->reg].inuse) {
    print_error(test_type, "true", "bool scratch_register[1].inuse");
    status = FAILURE;
  }

  register_codegen_clear();
  expr_destroy(&global_expr); expr_destroy(&local_expr);
  symbol_table_destroy(&st);

  CODEGEN_OUT = freopen("foo.txt", "r", CODEGEN_OUT); if (!CODEGEN_OUT) { return file_error(test_type); }
  fileread(CODEGEN_OUT, buffer, MAX_BUFFER); remove("foo.txt");
  if (strcmp(expect, buffer) != 0) { print_error(test_type, expect, buffer); status = FAILURE; }
  return status;
}

Status test_expr_codegen_binary(void) {
  strcpy(test_type, "Testing: test_expr_codegen_binary");
  Status status = SUCCESS;
  expr_t exprs[5] = { EXPR_ASSIGN, EXPR_ADD, EXPR_SUB, EXPR_AND, EXPR_OR };
  char* expect =
"MOVQ $493, %rbx\nMOVQ $42, %r10\nMOVQ %rbx, %r10\n\
MOVQ $493, %rbx\nMOVQ $42, %r10\nADDQ %rbx, %r10\n\
MOVQ $493, %rbx\nMOVQ $42, %r10\nSUBQ %rbx, %r10\n\
MOVQ $493, %rbx\nMOVQ $42, %r10\nANDQ %rbx, %r10\n\
MOVQ $493, %rbx\nMOVQ $42, %r10\nORQ %rbx, %r10\n";
  CODEGEN_OUT = fopen("foo.txt", "w"); if (!CODEGEN_OUT) { return file_error(test_type); }
  for (int i = 0; i < 5; i++) {
    struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
    register_codegen_init(true);
    struct expr* e = expr_create(exprs[i], expr_create_integer_literal(493), expr_create_integer_literal(42));
    error_status = expr_resolve(st, e);
    struct type* t; t = expr_typecheck(st, e); type_destroy(&t);
    error_status = expr_codegen(st, e);

    if (e->left->reg != 0) { print_error(test_type, "0", "int e->left->reg"); return FAILURE; }
    if (e->right->reg != 1) { print_error(test_type, "1", "int e->right->reg"); return FAILURE; }
    if (e->reg != 1) { print_error(test_type, "1", "int e->reg"); status = FAILURE; }
    if (e->reg != e->right->reg) { print_error(test_type, "int e->reg (1)", "int e->right->reg (1)"); status = FAILURE; }
    if (scratch_register[e->left->reg].inuse) {
      print_error(test_type, "false", "bool scratch_register[e->left->reg].inuse");
      status = FAILURE;
    }
    if (!scratch_register[e->right->reg].inuse) {
      print_error(test_type, "true", "scratch_register[e->right->reg].inuse");
      status = FAILURE;
    }
    if (!scratch_register[e->reg].inuse) {
      print_error(test_type, "true", "scratch_register[e->reg].inuse");
      status = FAILURE;
    }

    expr_destroy(&e);
    symbol_table_destroy(&st);
    register_codegen_clear();
  }
  CODEGEN_OUT = freopen("foo.txt", "r", CODEGEN_OUT); if (!CODEGEN_OUT) { return file_error(test_type); }
  fileread(CODEGEN_OUT, buffer, MAX_BUFFER); remove("foo.txt");
  if (strcmp(expect, buffer) != 0) { print_error(test_type, expect, buffer); status = FAILURE; }
  return status;
}

Status test_expr_codegen_unary(void) {
  strcpy(test_type, "Testing: test_expr_codegen_unary");
  Status status = SUCCESS;
  expr_t exprs[5] = { EXPR_POS, EXPR_NEG, EXPR_INC, EXPR_DEC, EXPR_NOT };
  char* expect =
"MOVQ $493, %rbx\n\
MOVQ $493, %rbx\nNEGQ %rbx\n\
MOVQ $493, %rbx\nINCQ %rbx\n\
MOVQ $493, %rbx\nDECQ %rbx\n\
MOVQ $493, %rbx\nNOTQ %rbx\n";
  CODEGEN_OUT = fopen("foo.txt", "w"); if (!CODEGEN_OUT) { return file_error(test_type); }
  for (int i = 0; i < 5; i++) {
    struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
    register_codegen_init(true);
    struct expr* e = expr_create(exprs[i], expr_create_integer_literal(493), NULL);
    error_status = expr_resolve(st, e);
    struct type* t; t = expr_typecheck(st, e); type_destroy(&t);
    error_status = expr_codegen(st, e);

    if (e->left->reg != 0) { print_error(test_type, "0", "int e->left->reg"); return FAILURE; }
    if (e->reg != 0) { print_error(test_type, "0", "int e->reg"); status = FAILURE; }
    if (e->reg != e->left->reg) { print_error(test_type, "int e->reg (0)", "int e->right->reg (0)"); status = FAILURE; }
    if (!scratch_register[e->left->reg].inuse) {
      print_error(test_type, "true", "bool scratch_register[e->left->reg].inuse");
      status = FAILURE;
    }
    if (!scratch_register[e->reg].inuse) {
      print_error(test_type, "true", "scratch_register[e->reg].inuse");
      status = FAILURE;
    }
    expr_destroy(&e);
    symbol_table_destroy(&st);
    register_codegen_clear();
  }
  CODEGEN_OUT = freopen("foo.txt", "r", CODEGEN_OUT); if (!CODEGEN_OUT) { return file_error(test_type); }
  fileread(CODEGEN_OUT, buffer, MAX_BUFFER); remove("foo.txt");
  if (strcmp(expect, buffer) != 0) { print_error(test_type, expect, buffer); status = FAILURE; }
  return status;
}

Status test_expr_codegen_muldivmod(void) {
  strcpy(test_type, "Testing: test_expr_codegen_muldivmod");
  Status status = SUCCESS;
  expr_t exprs[3] = { EXPR_MULT, EXPR_DIV, EXPR_MOD };
  char* expect =
"MOVQ $4, %rbx\nMOVQ $5, %r10\nMOVQ %rbx, %rax\nIMULQ %r10, %rax\nMOVQ %rax, %r11\n\
MOVQ $4, %rbx\nMOVQ $5, %r10\nMOVQ %rbx, %rax\nCQTO\nIDIVQ %r10\nMOVQ %rax, %r11\n\
MOVQ $4, %rbx\nMOVQ $5, %r10\nMOVQ %rbx, %rax\nCQTO\nIDIVQ %r10\nMOVQ %rdx, %r11\n";
  CODEGEN_OUT = fopen("foo.txt", "w"); if (!CODEGEN_OUT) { return file_error(test_type); }
  for (int i = 0; i < 3; i++) {
    struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
    register_codegen_init(true);
    struct expr* e = expr_create(exprs[i], expr_create_integer_literal(4), expr_create_integer_literal(5));
    error_status = expr_resolve(st, e);
    struct type* t; t = expr_typecheck(st, e); type_destroy(&t);
    error_status = expr_codegen(st, e);

    if (e->left->reg != 0) { print_error(test_type, "0", "int e->left->reg"); return FAILURE; }
    if (e->right->reg != 1) { print_error(test_type, "1", "int e->right->reg"); return FAILURE; }
    if (e->reg != 2) { print_error(test_type, "1", "int e->reg"); status = FAILURE; }
    if (e->reg == e->left->reg) { print_error(test_type, "int e->reg (2)", "int e->left->reg (0)"); status = FAILURE; }
    if (e->reg == e->right->reg) { print_error(test_type, "int e->reg (2)", "int e->right->reg (1)"); status = FAILURE; }
    if (scratch_register[e->left->reg].inuse) {
      print_error(test_type, "false", "bool scratch_register[e->left->reg].inuse");
      status = FAILURE;
    }
    if (scratch_register[e->right->reg].inuse) {
      print_error(test_type, "false", "scratch_register[e->right->reg].inuse");
      status = FAILURE;
    }
    if (!scratch_register[e->reg].inuse) {
      print_error(test_type, "true", "scratch_register[e->reg].inuse");
      status = FAILURE;
    }

    expr_destroy(&e);
    symbol_table_destroy(&st);
    register_codegen_clear();
  }
  CODEGEN_OUT = freopen("foo.txt", "r", CODEGEN_OUT); if (!CODEGEN_OUT) { return file_error(test_type); }
  fileread(CODEGEN_OUT, buffer, MAX_BUFFER); remove("foo.txt");
  if (strcmp(expect, buffer) != 0) { print_error(test_type, expect, buffer); status = FAILURE; }
  return status;
}


Status test_expr_codegen_relate(void) {
  strcpy(test_type, "Testing: test_expr_codegen_relate");
  Status status = SUCCESS;
  expr_t exprs[6] = { EXPR_EQ, EXPR_NEQ, EXPR_LESS, EXPR_LEQ, EXPR_GREAT, EXPR_GEQ };
  char* expect =
"MOVQ $4, %rbx\nMOVQ $5, %r10\nCMP %rbx, %r10\nSETE %bl\nMOVZBQ %bl, %rbx\n\
MOVQ $4, %rbx\nMOVQ $5, %r10\nCMP %rbx, %r10\nSETNE %bl\nMOVZBQ %bl, %rbx\n\
MOVQ $4, %rbx\nMOVQ $5, %r10\nCMP %rbx, %r10\nSETL %bl\nMOVZBQ %bl, %rbx\n\
MOVQ $4, %rbx\nMOVQ $5, %r10\nCMP %rbx, %r10\nSETLE %bl\nMOVZBQ %bl, %rbx\n\
MOVQ $4, %rbx\nMOVQ $5, %r10\nCMP %rbx, %r10\nSETG %bl\nMOVZBQ %bl, %rbx\n\
MOVQ $4, %rbx\nMOVQ $5, %r10\nCMP %rbx, %r10\nSETGE %bl\nMOVZBQ %bl, %rbx\n";
  CODEGEN_OUT = fopen("foo.txt", "w"); if (!CODEGEN_OUT) { return file_error(test_type); }
  for (int i = 0; i < 6; i++) {
    struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
    register_codegen_init(true);
    struct expr* e = expr_create(exprs[i], expr_create_integer_literal(4), expr_create_integer_literal(5));
    error_status = expr_resolve(st, e);
    struct type* t; t = expr_typecheck(st, e); type_destroy(&t);
    error_status = expr_codegen(st, e);

    if (e->left->reg != 0) { print_error(test_type, "0", "int e->left->reg"); return FAILURE; }
    if (e->right->reg != 1) { print_error(test_type, "1", "int e->right->reg"); return FAILURE; }
    if (e->reg != 0) { print_error(test_type, "0", "int e->reg"); status = FAILURE; }
    if (e->reg != e->left->reg) { print_error(test_type, "int e->reg (0)", "int e->left->reg (0)"); status = FAILURE; }
    if (!scratch_register[e->left->reg].inuse) {
      print_error(test_type, "true", "bool scratch_register[e->left->reg].inuse");
      status = FAILURE;
    }
    if (scratch_register[e->right->reg].inuse) {
      print_error(test_type, "false", "scratch_register[e->right->reg].inuse");
      status = FAILURE;
    }
    if (!scratch_register[e->reg].inuse) {
      print_error(test_type, "true", "scratch_register[e->reg].inuse");
      status = FAILURE;
    }

    expr_destroy(&e);
    symbol_table_destroy(&st);
    register_codegen_clear();
  }
  CODEGEN_OUT = freopen("foo.txt", "r", CODEGEN_OUT); if (!CODEGEN_OUT) { return file_error(test_type); }
  fileread(CODEGEN_OUT, buffer, MAX_BUFFER); remove("foo.txt");
  if (strcmp(expect, buffer) != 0) { print_error(test_type, expect, buffer); status = FAILURE; }
  return status;
}

Status test_expr_codegen_divmod_zero(void) {
  strcpy(test_type, "Testing: test_expr_codegen_divmod_zero");
  Status status = SUCCESS;
  expr_t exprs[3] = { EXPR_DIV, EXPR_MOD };
  CODEGEN_OUT = fopen("foo.txt", "w"); if (!CODEGEN_OUT) { return file_error(test_type); }
  for (int i = 0; i < 2; i++) {
    struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
    register_codegen_init(true);
    struct expr* e = expr_create(exprs[i], expr_create_integer_literal(1), expr_create_integer_literal(0));
    error_status = expr_resolve(st, e);
    struct type* t; t = expr_typecheck(st, e); type_destroy(&t);
    error_status = expr_codegen(st, e);

    if (!global_error_count) { print_error(test_type, "NOT 0", "int error_status"); status = FAILURE; }
    if (error_status != EXPR_BYZERO) { print_error(test_type, "EBYZERO", "int error_status"); status = FAILURE; }
    expr_destroy(&e);
    symbol_table_destroy(&st);
    register_codegen_clear();
  }
  fclose(CODEGEN_OUT); remove("foo.txt");
  return status;
}

Status test_expr_codegen_overflow(void) {
  strcpy(test_type, "Testing: test_expr_codegen_overflow");
  Status status = SUCCESS;
  expr_t exprs[5] = { EXPR_ADD, EXPR_INC, EXPR_SUB, EXPR_DIV, EXPR_MOD };
  CODEGEN_OUT = fopen("foo.txt", "w"); if (!CODEGEN_OUT) { return file_error(test_type); }
  for (int i = 0; i < 5; i++) {
    struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
    register_codegen_init(true);
    struct expr* right = NULL;
    struct expr* left = NULL;
    switch(exprs[i]) {
      case EXPR_ADD:
          left = expr_create_integer_literal(INT64_MAX);
          right = expr_create_integer_literal(1);
      break;
      case EXPR_INC:
          left = expr_create_integer_literal(INT64_MAX);
          right = NULL;
      break;
      case EXPR_SUB:
          left = expr_create_integer_literal(INT64_MAX);
          right = expr_create_integer_literal(-1);
      break;
      case EXPR_DIV: case EXPR_MOD:
          left = expr_create_integer_literal(INT64_MIN);
          right = expr_create_integer_literal(-1);
      break;
      default: break;
    }
    struct expr* e = expr_create(exprs[i], left, right);
    error_status = expr_resolve(st, e);
    struct type* t; t = expr_typecheck(st, e); type_destroy(&t);
    error_status = expr_codegen(st, e);

    if (!global_error_count) { print_error(test_type, "NOT 0", "int error_status"); status = FAILURE; }
    if (error_status != EXPR_OVERFLOW) { print_error(test_type, "EOVERFLOW", "int error_status"); status = FAILURE; }
    expr_destroy(&e);
    symbol_table_destroy(&st);
    register_codegen_clear();
  }
  fclose(CODEGEN_OUT); remove("foo.txt");
  return status;
}

Status test_expr_codegen_underflow(void) {
  strcpy(test_type, "Testing: test_expr_codegen_underflow");
  Status status = SUCCESS;
  expr_t exprs[3] = { EXPR_ADD, EXPR_DEC, EXPR_SUB };
  CODEGEN_OUT = fopen("foo.txt", "w"); if (!CODEGEN_OUT) { return file_error(test_type); }
  for (int i = 0; i < 3; i++) {
    struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
    register_codegen_init(true);
    struct expr* right = NULL;
    switch(exprs[i]) {
      case EXPR_ADD: right = expr_create_integer_literal(-1); break;
      case EXPR_DEC: right = NULL; break;
      case EXPR_SUB: right = expr_create_integer_literal(1); break;
      default: break;
    }
    struct expr* e = expr_create(exprs[i], expr_create_integer_literal(INT64_MIN), right);
    error_status = expr_resolve(st, e);
    struct type* t; t = expr_typecheck(st, e); type_destroy(&t);
    error_status = expr_codegen(st, e);

    if (!global_error_count) { print_error(test_type, "NOT 0", "int error_status"); status = FAILURE; }
    if (error_status != EXPR_UNDERFLOW) { print_error(test_type, "EUNDERFLOW", "int error_status"); status = FAILURE; }
    expr_destroy(&e);
    symbol_table_destroy(&st);
    register_codegen_clear();
  }
  fclose(CODEGEN_OUT); remove("foo.txt");
  return status;
}

Status test_expr_codegen_mult_underflow_overflow(void) {
  strcpy(test_type, "Testing: test_expr_codegen_mult_underflow_verflow");
  Status status = SUCCESS;
  CODEGEN_OUT = fopen("foo.txt", "w"); if (!CODEGEN_OUT) { return file_error(test_type); }
  codegen_error_t expect;
  for (int i = 0; i < 4; i++) {
    struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
    register_codegen_init(true);
    struct expr* right = NULL;
    struct expr* left = NULL;
    switch(i) {
      case 0: /* overflow positive operands */
          expect = EXPR_OVERFLOW;
          left = expr_create_integer_literal(INT64_MAX);
          right = expr_create_integer_literal(2);
      break;
      case 1: /* overflow negative operands */
          expect = EXPR_OVERFLOW;
          left = expr_create_integer_literal(INT64_MIN);
          right = expr_create_integer_literal(-1);
      break;
      case 2: /* underflow left positive right negative */
          expect = EXPR_UNDERFLOW;
          left = expr_create_integer_literal(INT64_MAX);
          right = expr_create_integer_literal(-2);
      break;
      case 3: /* underflow left negative right positive */
          expect = EXPR_UNDERFLOW;
          left = expr_create_integer_literal(-2);
          right = expr_create_integer_literal(INT64_MAX);
      break;
      default: break;
    }
    struct expr* e = expr_create(EXPR_MULT, left, right);
    error_status = expr_resolve(st, e);
    struct type* t; t = expr_typecheck(st, e); type_destroy(&t);
    error_status = expr_codegen(st, e);

    if (!global_error_count) { print_error(test_type, "NOT 0", "int error_status"); status = FAILURE; }
    if (error_status != expect) {
      print_error(test_type, (expect == EXPR_OVERFLOW) ?  "EOVERFLOW" : "EUNDERFLOW", "int error_status");
      status = FAILURE;
    }
    expr_destroy(&e);
    symbol_table_destroy(&st);
    register_codegen_clear();
  }
  fclose(CODEGEN_OUT); remove("foo.txt");
  return status;
}

// covers both init and uninit!!!
Status test_decl_codegen_literal_global(void) { return FAILURE; }
Status test_decl_codegen_literal_local(void) { return FAILURE; }
Status test_decl_codegen_string_literal_global(void) { return FAILURE; }
Status test_decl_codegen_string_literal_local(void) { return FAILURE; }