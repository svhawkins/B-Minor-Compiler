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

// array declarations 
Status test_decl_codegen_array_literal_global(void);
Status test_decl_codegen_array_literal_local(void);
Status test_decl_codegen_array_string_literal_global(void);
Status test_decl_codegen_array_string_literal_local(void);
Status test_decl_codegen_array_global_uninit(void);
Status test_decl_codegen_array_local_uninit(void);
Status test_decl_codegen_array_size_mismatch_fatal(void);
Status test_decl_codegen_array_size_mismatch_nonfatal(void);
Status test_decl_codegen_array_size_infer_init(void);
Status test_decl_codegen_array_multidim(void);

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
      test_decl_codegen_array_literal_global,
      test_decl_codegen_array_literal_local,
      test_decl_codegen_array_string_literal_global,
      test_decl_codegen_array_string_literal_local,
      test_decl_codegen_array_global_uninit,
      test_decl_codegen_array_local_uninit,
      test_decl_codegen_array_size_mismatch_fatal,
      test_decl_codegen_array_size_mismatch_nonfatal,
      test_decl_codegen_array_size_infer_init,
       //test_decl_codegen_array_multidim
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

  /* tests that a global non-string array (init) is generated properly */
Status test_decl_codegen_array_literal_global(void) {
  strcpy(test_type, "Testing: test_decl_codegen_array_literal_global");
  Status status = SUCCESS;
  char* expect =
"foo:\n\
\t.quad 493\n\
\t.quad 12\n\
\t.quad -1\n";
  CODEGEN_OUT = fopen("foo.txt", "w"); if (!CODEGEN_OUT) { return file_error(test_type); }
  struct symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st); //symbol_table_scope_enter(st); // local scope
  register_codegen_init(true);
  struct type* t = type_create(TYPE_ARRAY, type_create(TYPE_INTEGER, NULL, NULL, NULL), NULL, expr_create_integer_literal(3));

  struct expr* erightright = expr_create(EXPR_COMMA, expr_create_integer_literal(12), expr_create_integer_literal(-1));
  struct expr* eright = expr_create(EXPR_COMMA, expr_create_integer_literal(493), erightright);
  struct expr* e = expr_create(EXPR_INIT, eright, NULL);
  struct decl* d = decl_create(strdup("foo"), t, e, NULL, NULL);
  error_status = decl_resolve(st, d);
  error_status = decl_typecheck(st, d);
  error_status = decl_codegen(st, d);

  decl_destroy(&d);
  symbol_table_destroy(&st);
  register_codegen_clear();

  CODEGEN_OUT = freopen("foo.txt", "r", CODEGEN_OUT); if (!CODEGEN_OUT) { return file_error(test_type); }
  fileread(CODEGEN_OUT, buffer, MAX_BUFFER); remove("foo.txt");
  if (strcmp(expect, buffer) != 0) { print_error(test_type, expect, buffer); status = FAILURE; }
  return status;
}

/* tests that a local non-string array (init) is generated properly */
Status test_decl_codegen_array_literal_local(void) {
    strcpy(test_type, "Testing: test_decl_codegen_array_string_literal_local");
  Status status = SUCCESS;
  char* expect =
"MOVQ $1, %r10\n\
MOVQ %r10, -8(%rbp)\n\
MOVQ $2, %r11\n\
MOVQ %r11, -16(%rbp)\n";
  CODEGEN_OUT = fopen("foo.txt", "w"); if (!CODEGEN_OUT) { return file_error(test_type); }
  struct symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st); symbol_table_scope_enter(st);
  register_codegen_init(true);
  struct type* t = type_create(TYPE_ARRAY, type_create(TYPE_INTEGER, NULL, NULL, NULL), NULL, expr_create_integer_literal(2));
  struct decl* d = decl_create(strdup("foo"), t,
                  expr_create(EXPR_INIT,
                  expr_create(EXPR_COMMA, expr_create_integer_literal(1), expr_create_integer_literal(2)), NULL),
                  NULL, NULL);
  error_status = decl_resolve(st, d);
  error_status = decl_typecheck(st, d);
  error_status = decl_codegen(st, d);

  decl_destroy(&d);
  symbol_table_destroy(&st);
  register_codegen_clear();

  CODEGEN_OUT = freopen("foo.txt", "r", CODEGEN_OUT); if (!CODEGEN_OUT) { return file_error(test_type); }
  fileread(CODEGEN_OUT, buffer, MAX_BUFFER); remove("foo.txt");
  if (strcmp(expect, buffer) != 0) { print_error(test_type, expect, buffer); status = FAILURE; }
  return status;
}

/* tests that a local STRING array (init) is generated properly */
Status test_decl_codegen_array_string_literal_local(void) {
  strcpy(test_type, "Testing: test_decl_codegen_array_string_literal_local");
  Status status = SUCCESS;
  char* expect =
".L1:\n\t.string \"goose\"\n.L0:\n\t.string \"duck\"\n\
LEAQ .L0, %r10\n\
MOVQ %r10, -8(%rbp)\n\
LEAQ .L1, %r11\n\
MOVQ %r11, -16(%rbp)\n";
  CODEGEN_OUT = fopen("foo.txt", "w"); if (!CODEGEN_OUT) { return file_error(test_type); }
  struct symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st); symbol_table_scope_enter(st);
  register_codegen_init(true);
  struct type* t = type_create(TYPE_ARRAY, type_create(TYPE_STRING, NULL, NULL, NULL), NULL, expr_create_integer_literal(2));
  struct decl* d = decl_create(strdup("foo"), t,
                  expr_create(EXPR_INIT,
                  expr_create(EXPR_COMMA, expr_create_string_literal("duck"), expr_create_string_literal("goose")), NULL),
                  NULL, NULL);
  error_status = decl_resolve(st, d);
  error_status = decl_typecheck(st, d);
  error_status = decl_codegen(st, d);

  decl_destroy(&d);
  symbol_table_destroy(&st);
  register_codegen_clear();

  CODEGEN_OUT = freopen("foo.txt", "r", CODEGEN_OUT); if (!CODEGEN_OUT) { return file_error(test_type); }
  fileread(CODEGEN_OUT, buffer, MAX_BUFFER); remove("foo.txt");
  if (strcmp(expect, buffer) != 0) { print_error(test_type, expect, buffer); status = FAILURE; }
  return status;
}

/* tests that a global STRING array (init) is generated properly */
Status test_decl_codegen_array_string_literal_global(void) {
  strcpy(test_type, "Testing: test_decl_codegen_array_string_literal_global");
  Status status = SUCCESS;
  char* expect =
".L0:\n\t.string \"duck\"\nfoo:\n\t.quad .L0\n";
  CODEGEN_OUT = fopen("foo.txt", "w"); if (!CODEGEN_OUT) { return file_error(test_type); }
  struct symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  register_codegen_init(true);
  struct type* t = type_create(TYPE_ARRAY, type_create(TYPE_STRING, NULL, NULL, NULL), NULL, expr_create_integer_literal(1));
  struct decl* d = decl_create(strdup("foo"), t, expr_create(EXPR_INIT, expr_create_string_literal("duck"), NULL), NULL, NULL);
  error_status = decl_resolve(st, d);
  error_status = decl_typecheck(st, d);
  error_status = decl_codegen(st, d);

  decl_destroy(&d);
  symbol_table_destroy(&st);
  register_codegen_clear();

  CODEGEN_OUT = freopen("foo.txt", "r", CODEGEN_OUT); if (!CODEGEN_OUT) { return file_error(test_type); }
  fileread(CODEGEN_OUT, buffer, MAX_BUFFER); remove("foo.txt");
  if (strcmp(expect, buffer) != 0) { print_error(test_type, expect, buffer); status = FAILURE; }
  return status;
}

/* tests that a global uninit array is generated properly */
Status test_decl_codegen_array_global_uninit(void) {
  strcpy(test_type, "Testing: test_decl_codegen_array_global_uninit");
  Status status = SUCCESS;
  char* expect =
"foo:\n\
\t.zero 8\n\
\t.zero 8\n\
\t.zero 8\n";
  CODEGEN_OUT = fopen("foo.txt", "w"); if (!CODEGEN_OUT) { return file_error(test_type); }
  struct symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  register_codegen_init(true);
  struct type* t = type_create(TYPE_ARRAY, type_create(TYPE_INTEGER, NULL, NULL, NULL), NULL, expr_create_integer_literal(3));
  struct decl* d = decl_create(strdup("foo"), t, NULL, NULL, NULL);
  error_status = decl_resolve(st, d);
  error_status = decl_typecheck(st, d);
  error_status = decl_codegen(st, d);

  decl_destroy(&d);
  symbol_table_destroy(&st);
  register_codegen_clear();

  CODEGEN_OUT = freopen("foo.txt", "r", CODEGEN_OUT); if (!CODEGEN_OUT) { return file_error(test_type); }
  fileread(CODEGEN_OUT, buffer, MAX_BUFFER); remove("foo.txt");
  if (strcmp(expect, buffer) != 0) { print_error(test_type, expect, buffer); status = FAILURE; }
  return status;
}

/* tests that a local uninit array is generated properly */
Status test_decl_codegen_array_local_uninit(void) {
  strcpy(test_type, "Testing: test_decl_codegen_array_local_uninit");
  Status status = SUCCESS;
  char* expect =
"MOVQ $0, -8(%rbp)\n\
MOVQ $0, -16(%rbp)\n";
  CODEGEN_OUT = fopen("foo.txt", "w"); if (!CODEGEN_OUT) { return file_error(test_type); }
  struct symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st); symbol_table_scope_enter(st);
  register_codegen_init(true);
  struct type* t = type_create(TYPE_ARRAY, type_create(TYPE_INTEGER, NULL, NULL, NULL), NULL, expr_create_integer_literal(2));
  struct decl* d = decl_create(strdup("foo"), t, NULL, NULL, NULL);
  error_status = decl_resolve(st, d);
  error_status = decl_typecheck(st, d);
  error_status = decl_codegen(st, d);

  decl_destroy(&d);
  symbol_table_destroy(&st);
  register_codegen_clear();

  CODEGEN_OUT = freopen("foo.txt", "r", CODEGEN_OUT); if (!CODEGEN_OUT) { return file_error(test_type); }
  fileread(CODEGEN_OUT, buffer, MAX_BUFFER); remove("foo.txt");
  if (strcmp(expect, buffer) != 0) { print_error(test_type, expect, buffer); status = FAILURE; }
  return status;
}

/* tests that array fatal size mismatches are handled properly (negative) */
Status test_decl_codegen_array_size_mismatch_fatal(void) {
  strcpy(test_type, "Testing: test_decl_codegen_array_size_mismatch_fatal");
  Status status = SUCCESS;

  struct symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st); symbol_table_scope_enter(st);
  register_codegen_init(true);

  struct type* bad_type= type_create(TYPE_ARRAY,
                                     type_create(TYPE_INTEGER, NULL, NULL, NULL),
                                     NULL,
                                     expr_create_integer_literal(-1));
  struct decl* d = decl_create(strdup("foo"), bad_type, NULL, NULL, NULL);
  error_status = decl_resolve(st, d);
  error_status = decl_typecheck(st, d);
  error_status = decl_codegen(st, d);

  if (!global_error_count) { print_error(test_type, "int global_error_count = 1", "0"); status = FAILURE; }
  if (error_status != DECL_NEGSIZE) { print_error(test_type, "DECL_NEGSIZE", "int error_status"); status = FAILURE; }

  decl_destroy(&d);
  symbol_table_destroy(&st);
  register_codegen_clear();
  return status; 
}

Status test_decl_codegen_array_size_mismatch_nonfatal(void) {
/* tests that array size mismatches are handled properly
  differing non-negative sizes of size or actual_size gives warning
      - if list size < declared size, remaining elements are 'uninit' padded. (true size is declared size)
      - if list size > declared size, true size is list size
*/
strcpy(test_type, "Testing: test_decl_codegen_array_size_mismatch_nonfatal");
Status status = SUCCESS;

char* expect =
"bar:\n\
\t.quad 1\n\
\t.quad 2\n\
foo:\n\
\t.quad 1\n\
\t.zero 8\n";

// declared > list
struct expr* edecl = expr_create(EXPR_INIT, expr_create_integer_literal(1), NULL);
struct type* tdecl = type_create(TYPE_ARRAY,
                                type_create(TYPE_INTEGER, NULL, NULL, NULL),
                                NULL,
                                expr_create_integer_literal(2));
struct decl* ddecl = decl_create(strdup("foo"), tdecl, edecl, NULL, NULL);

// list > declared
struct expr* elist = expr_create(EXPR_INIT,
                                 expr_create(EXPR_COMMA, expr_create_integer_literal(1), expr_create_integer_literal(2)), NULL);
struct type* tlist = type_create(TYPE_ARRAY,
                                type_create(TYPE_INTEGER, NULL, NULL, NULL),
                                NULL,
                                expr_create_integer_literal(1));
struct decl* dlist = decl_create(strdup("bar"), tlist, elist, NULL, ddecl);

  CODEGEN_OUT = fopen("foo.txt", "w"); if (!CODEGEN_OUT) { return file_error(test_type); }
  struct symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  register_codegen_init(true);

  error_status = decl_resolve(st, dlist);
  error_status = decl_typecheck(st, dlist);
  error_status = decl_codegen(st, dlist);

  decl_destroy(&dlist);
  symbol_table_destroy(&st);
  register_codegen_clear();

  CODEGEN_OUT = freopen("foo.txt", "r", CODEGEN_OUT); if (!CODEGEN_OUT) { return file_error(test_type); }
  fileread(CODEGEN_OUT, buffer, MAX_BUFFER); remove("foo.txt");
  if (strcmp(expect, buffer) != 0) { print_error(test_type, expect, buffer); status = FAILURE; }

  //for (int i = 0; i < strlen(expect); i++) { printf("[%c], [%c]\n", expect[i], buffer[i]); }
  return status;
}

 /* tests that array code is generated of for length of initializer list size properly */
Status test_decl_codegen_array_size_infer_init(void) {
  strcpy(test_type, "Testing: test_decl_codegen_array_size_infer_init");
  Status status = SUCCESS;
  char* expect =
"foo:\n\
\t.quad 493\n\
\t.quad 12\n\
\t.quad -1\n";
  CODEGEN_OUT = fopen("foo.txt", "w"); if (!CODEGEN_OUT) { return file_error(test_type); }
  struct symbol_table* st = symbol_table_create();
  symbol_table_scope_enter(st);
  register_codegen_init(true);
  struct type* t = type_create(TYPE_ARRAY, type_create(TYPE_INTEGER, NULL, NULL, NULL), NULL, NULL);

  struct expr* erightright = expr_create(EXPR_COMMA, expr_create_integer_literal(12), expr_create_integer_literal(-1));
  struct expr* eright = expr_create(EXPR_COMMA, expr_create_integer_literal(493), erightright);
  struct expr* e = expr_create(EXPR_INIT, eright, NULL);
  struct decl* d = decl_create(strdup("foo"), t, e, NULL, NULL);

  error_status = decl_resolve(st, d);
  error_status = decl_typecheck(st, d);
  error_status = decl_codegen(st, d);

  decl_destroy(&d);
  symbol_table_destroy(&st);
  register_codegen_clear();

  CODEGEN_OUT = freopen("foo.txt", "r", CODEGEN_OUT); if (!CODEGEN_OUT) { return file_error(test_type); }
  fileread(CODEGEN_OUT, buffer, MAX_BUFFER); remove("foo.txt");
  if (strcmp(expect, buffer) != 0) { print_error(test_type, expect, buffer); status = FAILURE; }
  return status;
}

Status test_decl_codegen_array_multidim(void) {
  /* tests that a multidimensional array is generated properly
  
  1. 2-dimensional
  3. 3-dimensional
  */

  return FAILURE;
}