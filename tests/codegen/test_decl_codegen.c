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

/* code generation tests */

// declarations
Status test_decl_codegen_global(void); // covers init and uninit
Status test_decl_codegen_local(void);
Status test_decl_codegen_global_uninit(void);
Status test_decl_codegen_local_uninit(void);

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
      test_decl_codegen_global,
      test_decl_codegen_global_uninit,
      test_decl_codegen_local,
      test_decl_codegen_local_uninit,
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

Status test_decl_codegen_global(void) {
  strcpy(test_type, "Testing: test_decl_codegen_global");
  Status status = SUCCESS;
  expr_t exprs[4] = { EXPR_INT, EXPR_CH, EXPR_BOOL, EXPR_STR };
  char* expect =
"foo:\n\t.quad 493\n\
foo:\n\t.quad 65\n\
foo:\n\t.quad 1\n\
.L0:\n\t.string \"duck\"\nfoo:\n\t.quad .L0\n";
  CODEGEN_OUT = fopen("foo.txt", "w"); if (!CODEGEN_OUT) { return file_error(test_type); }
  for (int i = 0; i < 4; i++) {
    struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
    register_codegen_init(true);
    struct decl* d = NULL; struct type* t = NULL; struct expr* e = NULL;
    switch(exprs[i]) {
      case EXPR_INT:
        t = type_create(TYPE_INTEGER, NULL, NULL, NULL);
        e = expr_create_integer_literal(493);
      break;
      case EXPR_CH:
        t = type_create(TYPE_CHARACTER, NULL, NULL, NULL);
        e = expr_create_char_literal('A');
      break;
      case EXPR_BOOL:
        t = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
        e = expr_create_boolean_literal(true);
      break;
      case EXPR_STR:
        t = type_create(TYPE_STRING, NULL, NULL, NULL);
        e = expr_create_string_literal("duck");
      break;
      default: break;
    }
    d = decl_create(strdup("foo"), t, e, NULL, NULL);
    error_status = decl_resolve(st, d);
    error_status = decl_typecheck(st, d);
    error_status = decl_codegen(st, d);

    if (d->value->reg != 0) { print_error(test_type, "0", "int d->value->reg"); return FAILURE; }
    if (!scratch_register[d->value->reg].inuse) {
      print_error(test_type, "true", "bool scratch_register[d->value->reg].inuse");
      status = FAILURE;
    }
    decl_destroy(&d);
    symbol_table_destroy(&st);
    register_codegen_clear();
  }
  CODEGEN_OUT = freopen("foo.txt", "r", CODEGEN_OUT); if (!CODEGEN_OUT) { return file_error(test_type); }
  fileread(CODEGEN_OUT, buffer, MAX_BUFFER); remove("foo.txt");
  if (strcmp(expect, buffer) != 0) { print_error(test_type, expect, buffer); status = FAILURE; }
  return status;
}

Status test_decl_codegen_global_uninit(void) {
  strcpy(test_type, "Testing: test_decl_codegen_global_uninit");
  Status status = SUCCESS;
  expr_t exprs[4] = { EXPR_INT, EXPR_CH, EXPR_BOOL, EXPR_STR };
  char* expect =
"foo:\n\t.zero 8\n\
foo:\n\t.zero 8\n\
foo:\n\t.zero 8\n\
foo:\n\t.zero 8\n";
  CODEGEN_OUT = fopen("foo.txt", "w"); if (!CODEGEN_OUT) { return file_error(test_type); }
  for (int i = 0; i < 4; i++) {
    struct symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);
    register_codegen_init(true);
    struct decl* d = NULL; struct type* t = NULL;
    switch(exprs[i]) {
      case EXPR_INT:
        t = type_create(TYPE_INTEGER, NULL, NULL, NULL);
      break;
      case EXPR_CH:
        t = type_create(TYPE_CHARACTER, NULL, NULL, NULL);
      break;
      case EXPR_BOOL:
        t = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
      break;
      case EXPR_STR:
        t = type_create(TYPE_STRING, NULL, NULL, NULL);
      break;
      default: break;
    }
    d = decl_create(strdup("foo"), t, NULL, NULL, NULL);
    error_status = decl_resolve(st, d);
    error_status = decl_typecheck(st, d);
    error_status = decl_codegen(st, d);

    decl_destroy(&d);
    symbol_table_destroy(&st);
    register_codegen_clear();
  }
  CODEGEN_OUT = freopen("foo.txt", "r", CODEGEN_OUT); if (!CODEGEN_OUT) { return file_error(test_type); }
  fileread(CODEGEN_OUT, buffer, MAX_BUFFER); remove("foo.txt");
  if (strcmp(expect, buffer) != 0) { print_error(test_type, expect, buffer); status = FAILURE; }
  return status;
}

Status test_decl_codegen_local(void)  {
  strcpy(test_type, "Testing: test_decl_codegen_local");
  Status status = SUCCESS;
  expr_t exprs[4] = { EXPR_INT, EXPR_CH, EXPR_BOOL, EXPR_STR };
  char* expect =
"MOVQ $493, %rbx\nMOVQ %rbx, -8(%rbp)\n\
MOVQ $65, %rbx\nMOVQ %rbx, -8(%rbp)\n\
MOVQ $1, %rbx\nMOVQ %rbx, -8(%rbp)\n\
.L0:\n\t.string \"duck\"\nLEAQ .L0, %rbx\nMOVQ %rbx, -8(%rbp)\n";
  CODEGEN_OUT = fopen("foo.txt", "w"); if (!CODEGEN_OUT) { return file_error(test_type); }
  for (int i = 0; i < 4; i++) {
    struct symbol_table* st = symbol_table_create();
    symbol_table_scope_enter(st); symbol_table_scope_enter(st); // local scope
    register_codegen_init(true);
    struct decl* d = NULL; struct type* t = NULL; struct expr* e = NULL;
    switch(exprs[i]) {
      case EXPR_INT:
        t = type_create(TYPE_INTEGER, NULL, NULL, NULL);
        e = expr_create_integer_literal(493);
      break;
      case EXPR_CH:
        t = type_create(TYPE_CHARACTER, NULL, NULL, NULL);
        e = expr_create_char_literal('A');
      break;
      case EXPR_BOOL:
        t = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
        e = expr_create_boolean_literal(true);
      break;
      case EXPR_STR:
        t = type_create(TYPE_STRING, NULL, NULL, NULL);
        e = expr_create_string_literal("duck");
      break;
      default: break;
    }
    d = decl_create(strdup("foo"), t, e, NULL, NULL);
    error_status = decl_resolve(st, d);
    error_status = decl_typecheck(st, d);
    error_status = decl_codegen(st, d);

    if (d->value->reg != 0) { print_error(test_type, "0", "int d->value->reg"); return FAILURE; }
    if (!scratch_register[d->value->reg].inuse) {
      print_error(test_type, "true", "bool scratch_register[d->value->reg].inuse");
      status = FAILURE;
    }
    if (d->symbol->which != 0) { print_error(test_type, "0", "int d->symbol->which"); return FAILURE; }
    decl_destroy(&d);
    symbol_table_destroy(&st);
    register_codegen_clear();
  }
  CODEGEN_OUT = freopen("foo.txt", "r", CODEGEN_OUT); if (!CODEGEN_OUT) { return file_error(test_type); }
  fileread(CODEGEN_OUT, buffer, MAX_BUFFER); remove("foo.txt");
  if (strcmp(expect, buffer) != 0) { print_error(test_type, expect, buffer); status = FAILURE; }
  return status;
}

Status test_decl_codegen_local_uninit(void)  {
  strcpy(test_type, "Testing: test_decl_codegen_local_uninit");
  Status status = SUCCESS;
  expr_t exprs[4] = { EXPR_INT, EXPR_CH, EXPR_BOOL, EXPR_STR };
  char* expect =
"MOVQ $0, -8(%rbp)\n\
MOVQ $0, -8(%rbp)\n\
MOVQ $0, -8(%rbp)\n\
MOVQ $0, -8(%rbp)\n";
  CODEGEN_OUT = fopen("foo.txt", "w"); if (!CODEGEN_OUT) { return file_error(test_type); }
  for (int i = 0; i < 4; i++) {
    struct symbol_table* st = symbol_table_create();
    symbol_table_scope_enter(st); symbol_table_scope_enter(st); // local scope
    register_codegen_init(true);
    struct decl* d = NULL; struct type* t = NULL;
    switch(exprs[i]) {
      case EXPR_INT:
        t = type_create(TYPE_INTEGER, NULL, NULL, NULL);
      break;
      case EXPR_CH:
        t = type_create(TYPE_CHARACTER, NULL, NULL, NULL);
      break;
      case EXPR_BOOL:
        t = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
      break;
      case EXPR_STR:
        t = type_create(TYPE_STRING, NULL, NULL, NULL);
      break;
      default: break;
    }
    d = decl_create(strdup("foo"), t, NULL, NULL, NULL);
    error_status = decl_resolve(st, d);
    error_status = decl_typecheck(st, d);
    error_status = decl_codegen(st, d);

    if (d->symbol->which != 0) { print_error(test_type, "0", "int d->symbol->which"); return FAILURE; }

    decl_destroy(&d);
    symbol_table_destroy(&st);
    register_codegen_clear();
  }
  CODEGEN_OUT = freopen("foo.txt", "r", CODEGEN_OUT); if (!CODEGEN_OUT) { return file_error(test_type); }
  fileread(CODEGEN_OUT, buffer, MAX_BUFFER); remove("foo.txt");
  if (strcmp(expect, buffer) != 0) { print_error(test_type, expect, buffer); status = FAILURE; }
  return status;
}