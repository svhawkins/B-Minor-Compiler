#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
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
char output[MAX_BUFFER];

// error functions
void print_error(char* test, char* expect, char* value);
Status file_error(char* test_type); // error message when getting temporary file

Status test_expr_print_name(void);
Status test_expr_print_str(void);
Status test_expr_print_int(void);
Status test_expr_print_ch(void);
Status test_expr_print_bool_true(void);
Status test_expr_print_bool_false(void);
Status test_expr_print_op(void);
Status test_type_print_atomic(void);
Status test_type_print_array(void);
Status test_type_print_array_nest(void);
Status test_type_print_function(void);
Status test_type_print_function_nest(void);
Status test_param_list_print_single(void);
Status test_param_list_print_multiple(void);
Status test_param_list_print_single_nest(void);

/*
  test function preamables:
    strcpy(test_type, "Testing: <function name>");
    Status status = SUCCESS;
*/

/*
tmp files
  FILE* tmp; tmp = fopen("foo.bminor", "w"); if (!tmp) { return file_error(test_type); }
  <struct>_fprint(tmp, struct);
  tmp = freopen("foo.bminor", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("foo.bminor");

*/

// reads contents of file into string s
void fileread(FILE* fp, char* s, int n) {
  int i; for (i = 0; i < n && !feof(fp); i++) s[i] = fgetc(fp);
  s[i-1] = '\0'; fclose(fp);
}

int main(int argc, const char* argv[]) {
  Status (*tests[])(void) = {
    test_expr_print_name,
    test_expr_print_str,
    test_expr_print_int,
    test_expr_print_ch,
    test_expr_print_bool_true,
    test_expr_print_bool_false,
    test_expr_print_op,
    test_type_print_atomic,
    test_type_print_array,
    test_type_print_array_nest,
    test_type_print_function,
    test_type_print_function_nest,
    test_param_list_print_single,
    test_param_list_print_multiple,
    test_param_list_print_single_nest,
  };
  int n_tests = sizeof(tests)/sizeof(tests[0]);
  int n_pass = 0;

  printf("Running %d tests...\n", n_tests);
  for (int i = 0; i < n_tests; i++) { if (tests[i]()) { n_pass++; }}

  printf("Passed: %d/%d\n", n_pass, n_tests);
  printf("Failed: %d/%d\n", (n_tests - n_pass), n_tests);
  return 0;
}
Status file_error(char* test_type) {
  printf("Failed to open temporary I/O file for %s.\n Test failure.\n", test_type);
  return FAILURE;
}
void print_error(char* test, char* expect, char* value) { printf("Error:\t[%s]:\n\tExpected: [%s]\n\tReceived: [%s]\n", test, expect, value); }

// testing expressions of the primitives, also the base case for expr_print
Status test_expr_print_name(void) {
  strcpy(test_type, "Testing: test_expr_print_name");
  Status status = SUCCESS;
  struct expr* e = expr_create_name("foo");
  char* expect = "foo";

  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  expr_fprint(tmp, e);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");

  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_expr_print_str(void) {
  strcpy(test_type, "Testing: test_expr_print_str");
  Status status = SUCCESS;
  struct expr* e = expr_create_string_literal("hello world!:)\n");
  char* expect = "hello world!:)\n";


  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  expr_fprint(tmp, e);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");

  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_expr_print_int(void) {
  strcpy(test_type, "Testing: test_expr_print_int");
  Status status = SUCCESS;

  struct expr* e = expr_create_integer_literal(493);
  char* expect = "493";


  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  expr_fprint(tmp, e);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");

  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_expr_print_ch(void) {
  strcpy(test_type, "Testing: test_expr_print_ch");
  Status status = SUCCESS;

  struct expr* e = expr_create_char_literal('a');
  char* expect = "a";


  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  expr_fprint(tmp, e);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");

  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_expr_print_bool_true(void) {
  strcpy(test_type, "Testing: test_expr_print_bool_true");
  Status status = SUCCESS;

  struct expr* e = expr_create_boolean_literal(true);
  char* expect = "true";


  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  expr_fprint(tmp, e);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");

  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}
Status test_expr_print_bool_false(void) {
  strcpy(test_type, "Testing: test_expr_print_bool_false");
  Status status = SUCCESS;

  struct expr* e = expr_create_boolean_literal(false);
  char* expect = "false";


  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  expr_fprint(tmp, e);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");

  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}


// TO DO: may have to update since associativity and parentheses not yet been imped
Status test_expr_print_op(void) {
  strcpy(test_type, "Testing: test_expr_print_op");
  Status status = SUCCESS;

  struct expr* l = expr_create_name("x");
  struct expr* r = expr_create_name("y");

  char* expect = "x++\nx--\n+x\n-x\n!x\nx^y\nx * y\nx / y\nx % y\nx + y\nx - y\nx <= y\nx < y\nx >= y\nx > y\nx == y\nx != y\nx && y\nx || y\nx = y\nx[y]\nx(y)\n";
  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  for (expr_t kind = EXPR_INC; kind <= EXPR_FCALL; kind++) {
    struct expr* e = (kind < EXPR_EXP) ? expr_create(kind, l, NULL) : expr_create(kind, l, r);
    expr_fprint(tmp, e); fprintf(tmp, "\n");
  }
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_type_print_atomic(void) {
  strcpy(test_type, "Testing: test_type_print_atomic");
  Status status = SUCCESS;
  char* expect ="voidbooleancharintegerstring";
  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  for (type_t kind = TYPE_VOID; kind < TYPE_ARRAY; kind++) {
    struct type* t = type_create(kind, NULL, NULL, NULL);
    type_fprint(tmp, t);
  }
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_type_print_array(void) {
  strcpy(test_type, "Testing: test_type_print_array");
  Status status = SUCCESS;
  struct type* subtype = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct type* t = type_create(TYPE_ARRAY, subtype, NULL, NULL);
  char* expect = "array [] integer";
  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  type_fprint(tmp, t);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_type_print_array_nest(void) {
  strcpy(test_type, "Testing: test_type_print_array_nest");
  Status status = SUCCESS;
  struct type* subtype = type_create(TYPE_ARRAY, type_create(TYPE_INTEGER, NULL, NULL, NULL), NULL, NULL);
  struct type* t = type_create(TYPE_ARRAY, subtype, NULL, NULL);
  char* expect = "array [] array [] integer";
  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  type_fprint(tmp, t);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;

}

Status test_type_print_function(void) {
  strcpy(test_type, "Testing: test_type_print_function");
  Status status = SUCCESS;
  struct type* subtype = type_create(TYPE_VOID, NULL, NULL, NULL);
  struct type* t = type_create(TYPE_FUNCTION, subtype, NULL, NULL);
  char* expect = "function void ()";
  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  type_fprint(tmp, t);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_type_print_function_nest(void) {
  strcpy(test_type, "Testing: test_type_print_function_nest");
  Status status = SUCCESS;
  struct type* subtype = type_create(TYPE_ARRAY, type_create(TYPE_INTEGER, NULL, NULL, NULL), NULL, NULL);
  struct type* t = type_create(TYPE_FUNCTION, subtype, NULL, NULL);
  char* expect = "function array [] integer ()";
  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  type_fprint(tmp, t);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}


Status test_param_list_print_single(void) {
  strcpy(test_type, "Testing: test_param_list_print_single");
  Status status = SUCCESS;
  struct type* t = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct param_list* p = param_list_create("x", t, NULL);
  char* expect = "x: integer";
  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  param_list_fprint(tmp, p);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}
Status test_param_list_print_multiple(void) {
  strcpy(test_type, "Testing: test_param_list_print_multiple");
  Status status = SUCCESS;
  struct type* t = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct param_list* pend = param_list_create("y", t, NULL);
  struct param_list* p = param_list_create("x", t, pend);
  char* expect = "x: integer, y: integer";
  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  param_list_fprint(tmp, p);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}
Status test_param_list_print_single_nest(void) {
  strcpy(test_type, "Testing: test_param_list_print_single_nest");
  Status status = SUCCESS;
  struct type* t = type_create(TYPE_ARRAY, type_create(TYPE_INTEGER, NULL, NULL, NULL), NULL, NULL);
  struct param_list* p = param_list_create("x", t, NULL);
  char* expect = "x: array [] integer";
  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  param_list_fprint(tmp, p);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}
