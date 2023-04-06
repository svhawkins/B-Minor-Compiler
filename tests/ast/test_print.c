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

typedef enum {FAILURE = 0, SUCCESS = 1} Status;
char test_type[MAX_BUFFER];
char output[MAX_BUFFER];

// error functions
void print_error(char* test, char* expect, char* value);
Status file_error(char* test_type); // error message when getting temporary file


/* expression printing tests:
   atomic types and operators
*/
Status test_expr_print_name(void);
Status test_expr_print_str(void);
Status test_expr_print_int(void);
Status test_expr_print_ch(void);
Status test_expr_print_bool_true(void);
Status test_expr_print_bool_false(void);
Status test_expr_print_op(void);

/*
 type printing tests:
  atomic types, array types, function types (no parameter lists)
*/
Status test_type_print_atomic(void);
Status test_type_print_array(void);
Status test_type_print_array_nest(void);
Status test_type_print_function(void);
Status test_type_print_function_nest(void);

/*
param_list printing tests
single parameter, multiparameter, composite parameter type
*/
Status test_param_list_print_single(void);
Status test_param_list_print_multiple(void);
Status test_param_list_print_single_nest(void);

/*
type print integration tests for:
- function parameter lists
- array sizes within the [] operator
*/
Status test_type_print_function_param_list(void);
Status test_type_print_array_expr(void);

/*
declaration print testing
uninitialized values, initialized values of all types
printing declaration lists
*/
Status test_decl_print_uninit_atomic(void);
Status test_decl_print_uninit_array(void);
Status test_decl_print_uninit_array_nest(void);
Status test_decl_print_uninit_function(void);
Status test_decl_print_init_atomic(void);
Status test_decl_print_init_array(void);
Status test_decl_print_init_array_nest(void);
Status test_decl_print_init_function(void);
Status test_decl_print_multiple(void);

/*
statement printing tests
based on kind
*/
Status test_stmt_print_decl(void);
Status test_stmt_print_expr(void);

// print statement
Status test_stmt_print_print_null(void);
Status test_stmt_print_print_expr(void);
Status test_stmt_print_print_list(void);

// return statement
Status test_stmt_print_return_null(void);
Status test_stmt_print_return_expr(void);

// block statement
Status test_stmt_print_block_null(void);
Status test_stmt_print_block_single(void);
Status test_stm_print_block_list(void);

// if-else statement
Status test_stmt_print_if_null(void);
Status test_stmt_print_if(void);
Status test_stmt_print_if_else(void);
Status test_stmt_print_if_else_nest(void);

// while statement
Status test_stmt_print_while_null(void);
Status test_stmt_print_while(void);

// for statement with an expression, declaration, and initalization
Status test_stmt_print_for_expr(void);
Status test_stmt_print_for_decl(void);
Status test_stmt_print_for_init(void);

// tests a valid program
Status test_print_program(void);

/*
  test function preamables:
    strcpy(test_type, "Testing: <function name>");
    Status status = FAILURE;
*/

/*
tmp files
  FILE* tmp; tmp = fopen("foo.txt", "w"); if (!tmp) { return file_error(test_type); }
  <struct>_fprint(tmp, struct);
  tmp = freopen("foo.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
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
    test_type_print_function_param_list,
    test_type_print_array_expr,
    test_decl_print_uninit_atomic,
    test_decl_print_uninit_array,
    test_decl_print_uninit_array_nest,
    test_decl_print_uninit_function,
    test_decl_print_init_atomic,
    test_decl_print_init_array,
    test_decl_print_init_array_nest,
    test_decl_print_init_function,
    test_decl_print_multiple,
    test_stmt_print_decl,
    test_stmt_print_expr,
    test_stmt_print_print_null,
    test_stmt_print_print_expr,
    test_stmt_print_print_list,
    test_stmt_print_return_null,
    test_stmt_print_return_expr,
    test_stmt_print_block_null,
    test_stmt_print_block_single,
    test_stm_print_block_list,
    test_stmt_print_if_null,
    test_stmt_print_if,
    test_stmt_print_if_else,
    test_stmt_print_if_else_nest,
    test_stmt_print_while_null,
    test_stmt_print_while,
    test_stmt_print_for_expr,
    test_stmt_print_for_decl,
    test_stmt_print_for_init,
    test_print_program
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
void print_error(char* test, char* expect, char* value) { printf("Error:\t[%s]:\n\tExpected:\n[%s]\n\tReceived:\n[%s]\n", test, expect, value); }

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
  char* expect = "\"hello world!:)\n\"";


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
  char* expect = "'a'";


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
  char* expect ="voidbooleancharintegerstringauto";
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

Status test_type_print_function_param_list(void) {
  strcpy(test_type, "Testing: test_type_print_function_param_list");
  Status status = SUCCESS;

  struct type* tend = type_create(TYPE_ARRAY, type_create(TYPE_STRING, NULL, NULL, NULL), NULL, NULL);
  struct param_list* pend = param_list_create("argv", tend, NULL);
  struct param_list* p = param_list_create("argc", type_create(TYPE_INTEGER, NULL, NULL, NULL), pend);
  struct type* t = type_create(TYPE_FUNCTION, type_create(TYPE_INTEGER, NULL, NULL, NULL), p, NULL);

  char* expect = "function integer (argc: integer, argv: array [] string)";
  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  type_fprint(tmp, t);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}


// TO DO: may have to update expected output to handle associativity
Status test_type_print_array_expr(void) {
  strcpy(test_type, "Testing: test_type_print_array_expr");
  Status status = SUCCESS;

  struct expr* three = expr_create_integer_literal(3);
  struct expr* dim1 = expr_create(EXPR_EXP, three, expr_create_integer_literal(1));
  struct expr* dim2 = expr_create(EXPR_DIV, expr_create(EXPR_MULT, three, three), three);
  struct type* subtype = type_create(TYPE_ARRAY, type_create(TYPE_BOOLEAN, NULL, NULL, NULL), NULL, dim2);
  struct type* t = type_create(TYPE_ARRAY, subtype, NULL, dim1);

  char* expect = "array [3^1] array [3 * 3 / 3] boolean";
  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  type_fprint(tmp, t);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_decl_print_uninit_atomic(void) {
  strcpy(test_type, "Testing: test_decl_print_uninit_atomic");
  Status status = SUCCESS;
  char* expect ="x: boolean;x: char;x: integer;x: string;x: auto;";
  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  for (type_t kind = TYPE_BOOLEAN; kind < TYPE_ARRAY; kind++) {
    struct type* t = type_create(kind, NULL, NULL, NULL);
    struct decl* d = decl_create("x", t, NULL, NULL, NULL);
    decl_fprint(tmp, d, 0);
  }
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_decl_print_uninit_array(void) {
  strcpy(test_type, "Testing: test_decl_print_uninit_array");
  Status status = SUCCESS;

  struct expr* two = expr_create_integer_literal(2);
  struct type* t = type_create(TYPE_ARRAY, type_create(TYPE_BOOLEAN, NULL, NULL, NULL), NULL, two);
  struct decl* d = decl_create("x", t, NULL, NULL, NULL);
  char* expect = "x: array [2] boolean;";

  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  decl_fprint(tmp, d, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_decl_print_uninit_array_nest(void) {
  strcpy(test_type, "Testing: test_decl_print_uninit_array_nest");
  Status status = SUCCESS;
  struct expr* three = expr_create_integer_literal(3);
  struct type* subtype = type_create(TYPE_ARRAY, type_create(TYPE_INTEGER, NULL, NULL, NULL), NULL, three);
  struct type* t = type_create(TYPE_ARRAY, subtype, NULL, three);
  struct decl* d = decl_create("x", t, NULL, NULL, NULL);
  char* expect = "x: array [3] array [3] integer;";

  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  decl_fprint(tmp, d, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_decl_print_uninit_function(void) {
  strcpy(test_type, "Testing: test_decl_print_uninit_function");
  Status status = SUCCESS;

  struct type* tend = type_create(TYPE_ARRAY, type_create(TYPE_STRING, NULL, NULL, NULL), NULL, NULL);
  struct param_list* pend = param_list_create("argv", tend, NULL);
  struct param_list* p = param_list_create("argc", type_create(TYPE_INTEGER, NULL, NULL, NULL), pend);
  struct type* t = type_create(TYPE_FUNCTION, type_create(TYPE_INTEGER, NULL, NULL, NULL), p, NULL);
  struct decl* d = decl_create("main", t, NULL, NULL, NULL);

  char* expect = "main: function integer (argc: integer, argv: array [] string);";
  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  decl_fprint(tmp, d, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_decl_print_init_atomic(void) {
  strcpy(test_type, "Testing: test_decl_print_init_atomic");
  Status status = SUCCESS;

  struct expr* inits[5] = { expr_create_boolean_literal(1),
			    expr_create_char_literal('a'),
			    expr_create_integer_literal(493),
			    expr_create_string_literal("string"),
			    expr_create_integer_literal(493) // how would auto be done? it nullifies dummy value and makes it again?
			   };

  char* expect ="x: boolean = true;x: char = 'a';x: integer = 493;x: string = \"string\";x: auto = 493;";
  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  for (type_t kind = TYPE_BOOLEAN, i = 0; kind < TYPE_ARRAY; kind++, i++) {
    struct type* t = type_create(kind, NULL, NULL, NULL);
    struct decl* d = decl_create("x", t, inits[i], NULL, NULL);
    decl_fprint(tmp, d, 0);
  }
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}


// TO DO: implement
Status test_decl_print_init_array(void) {
  strcpy(test_type, "Testing: test_decl_print_init_array");
  Status status = FAILURE;
  return status;
}

// TO DO: implement
Status test_decl_print_init_array_nest(void) {
  strcpy(test_type, "Testing: test_decl_print_init_array_nest");
  Status status = FAILURE;
  return status;
}

Status test_decl_print_init_function(void) {
  strcpy(test_type, "Testing: test_decl_print_init_function");
  Status status = SUCCESS;
  struct type* tend = type_create(TYPE_ARRAY, type_create(TYPE_STRING, NULL, NULL, NULL), NULL, NULL);
  struct param_list* pend = param_list_create("argv", tend, NULL);
  struct param_list* p = param_list_create("argc", type_create(TYPE_INTEGER, NULL, NULL, NULL), pend);
  struct type* t = type_create(TYPE_FUNCTION, type_create(TYPE_INTEGER, NULL, NULL, NULL), p, NULL);
  struct stmt* s = stmt_create(STMT_EXPR, NULL, NULL, expr_create_name("duck"), NULL,NULL, NULL, NULL);
  struct decl* d = decl_create("main", t, NULL, s, NULL);

  char* expect = "main: function integer (argc: integer, argv: array [] string) = {\n  duck;\n}\n";
  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  decl_fprint(tmp, d, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_decl_print_multiple(void) {
  strcpy(test_type, "Testing: test_decl_print_multiple");
  Status status = SUCCESS;
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct type* matrix = type_create(TYPE_ARRAY, type_create(TYPE_ARRAY, integer, NULL, expr_create_name("n")), NULL, expr_create_name("n"));
  struct decl* dend = decl_create("square", matrix, NULL, NULL, NULL);
  struct decl* d = decl_create("n", integer, expr_create_integer_literal(2), NULL, dend);
  char* expect = "n: integer = 2;\nsquare: array [n] array [n] integer;";

  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  decl_fprint(tmp, d, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_stmt_print_decl(void) {
  strcpy(test_type, "Testing: test_stmt_print_decl");
  Status status = SUCCESS;
  struct decl* d = decl_create("b", type_create(TYPE_BOOLEAN, NULL, NULL, NULL),
				    expr_create(EXPR_AND, expr_create_boolean_literal(1), expr_create_boolean_literal(0)), NULL, NULL);
  struct stmt* s = stmt_create(STMT_DECL, d, NULL, NULL, NULL, NULL, NULL, NULL);
  char* expect = "b: boolean = true && false;\n";

  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_stmt_print_expr(void) {
  strcpy(test_type, "Testing: test_stmt_print_expr");
  Status status = SUCCESS;
  struct expr* e = expr_create(EXPR_ASSIGN, expr_create_name("x"), expr_create(EXPR_ADD, expr_create_integer_literal(493), expr_create_integer_literal(1)));
  struct stmt* s = stmt_create(STMT_EXPR, NULL, NULL, e, NULL, NULL, NULL, NULL);
  char* expect = "x = 493 + 1;\n";

  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_stmt_print_print_null(void) {
  strcpy(test_type, "Testing: test_stmt_print_print_null");
  Status status = SUCCESS;
  struct stmt* s = stmt_create(STMT_PRINT, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  char* expect = "print;\n";

  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}


// TO DO: update expect for parentheses
Status test_stmt_print_print_expr(void) {
  strcpy(test_type, "Testing: test_stmt_print_print_expr");
  Status status = SUCCESS;
  struct expr* e = expr_create(EXPR_AND, expr_create_boolean_literal(0), expr_create(EXPR_OR, expr_create_boolean_literal(1), expr_create_name("foo")));
  struct stmt* s = stmt_create(STMT_PRINT, NULL, NULL, e, NULL, NULL, NULL, NULL);
  char* expect = "print false && true || foo;\n";

  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

// TO DO: implement expression lists to test this
Status test_stmt_print_print_list(void) {
  strcpy(test_type, "Testing: test_stmt_print_print_list");
  Status status = FAILURE;
  return status;
}

Status test_stmt_print_return_null(void) {
  strcpy(test_type, "Testing: test_stmt_print_return_null");
  Status status = SUCCESS;
  struct stmt* s = stmt_create(STMT_RETURN, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  char* expect = "return;\n";

  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_stmt_print_return_expr(void) {
  strcpy(test_type, "Testing: test_stmt_print_return_expr");
  Status status = SUCCESS;
  struct expr* e = expr_create(EXPR_AND, expr_create_boolean_literal(0), expr_create(EXPR_OR, expr_create_boolean_literal(1), expr_create_name("foo")));
  struct stmt* s = stmt_create(STMT_RETURN, NULL, NULL, e, NULL, NULL, NULL, NULL);
  char* expect = "return false && true || foo;\n";

  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_stmt_print_block_null(void) {
  strcpy(test_type, "Testing: test_stmt_print_block_null");
  Status status = SUCCESS;
  struct stmt* s = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  char* expect = "{}\n";

  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_stmt_print_block_single(void) {
  strcpy(test_type, "Testing: test_stmt_print_block_single");
  Status status = SUCCESS;
  struct stmt* body = stmt_create(STMT_EXPR, NULL, NULL, expr_create_name("duck"), NULL, NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, body, NULL, NULL);
  char* expect = "{\n  duck;\n}\n";

  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}
Status test_stm_print_block_list(void) {
  strcpy(test_type, "Testing: test_stmt_print_block_list");
  Status status = SUCCESS;
  struct stmt* bodyend = stmt_create(STMT_EXPR, NULL, NULL, expr_create_name("goose"), NULL, NULL, NULL, NULL);
  struct stmt* body = stmt_create(STMT_EXPR, NULL, NULL, expr_create_name("duck"), NULL, NULL, NULL, bodyend);
  struct stmt* s = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, body, NULL, NULL);
  char* expect = "{\n  duck;\n  goose;\n}\n";

  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_stmt_print_if_null(void) {
  strcpy(test_type, "Testing: test_stmt_print_print_if_null");
  Status status = SUCCESS;
  struct stmt* s = stmt_create(STMT_IF_ELSE, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  char* expect = "if () {\n}\n";

  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_stmt_print_if(void) {
  strcpy(test_type, "Testing: test_stmt_print_print_if");
  Status status = SUCCESS;
  struct expr* e = expr_create_name("e");
  struct stmt* duck = stmt_create(STMT_EXPR, NULL, NULL, expr_create_name("duck"), NULL, NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_IF_ELSE, NULL, NULL, e, NULL, duck, NULL, NULL);
  char* expect = "if (e) {\n  duck;\n}\n";

  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_stmt_print_if_else(void) {
  strcpy(test_type, "Testing: test_stmt_print_print_if_else");
  Status status = SUCCESS;
  struct expr* e = expr_create_name("e");
  struct stmt* duck = stmt_create(STMT_EXPR, NULL, NULL, expr_create_name("duck"), NULL, NULL, NULL, NULL);
  struct stmt* goose = stmt_create(STMT_EXPR, NULL, NULL, expr_create_name("goose"), NULL, NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_IF_ELSE, NULL, NULL, e, NULL, duck, goose, NULL);
  char* expect = "if (e) {\n  duck;\n} else {\n  goose;\n}\n";

  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_stmt_print_if_else_nest(void) {
  strcpy(test_type, "Testing: test_stmt_print_print_if_else_nest");
  Status status = SUCCESS;
  struct expr* e = expr_create_name("e");
  struct stmt* duck = stmt_create(STMT_EXPR, NULL, NULL, expr_create_name("duck"), NULL, NULL, NULL, NULL);
  struct stmt* goose = stmt_create(STMT_EXPR, NULL, NULL, expr_create_name("goose"), NULL, NULL, NULL, NULL);
  struct stmt* snest = stmt_create(STMT_IF_ELSE, NULL, NULL, e, NULL, duck, goose, NULL);
  struct stmt* s = stmt_create(STMT_IF_ELSE, NULL, NULL, e, NULL, snest, goose, NULL);
  char* expect = "if (e) {\n  if (e) {\n    duck;\n  } else {\n    goose;\n  }\n} else {\n  goose;\n}\n";

  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_stmt_print_while_null(void) {
  strcpy(test_type, "Testing: test_stmt_print_while_null");
  Status status = SUCCESS;
  struct stmt* s = stmt_create(STMT_WHILE, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  char* expect = "while () {\n}\n";

  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}
Status test_stmt_print_while(void) {
  strcpy(test_type, "Testing: test_stmt_print_while");
  Status status = SUCCESS;
  struct expr* e = expr_create_name("e");
  struct stmt* duck = stmt_create(STMT_EXPR, NULL, NULL, expr_create_name("duck"), NULL, NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_WHILE, NULL, NULL, e, NULL, duck, NULL, NULL);
  char* expect = "while (e) {\n  duck;\n}\n";

  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_stmt_print_for_expr(void) {
  strcpy(test_type, "Testing: test_stmt_print_for_expr");
  Status status = SUCCESS;
  struct expr* exprs[3] = { expr_create(EXPR_ASSIGN, expr_create_name("i"), expr_create_integer_literal(0)),
			    expr_create(EXPR_LESS, expr_create_name("i"), expr_create_name("n")),
			    expr_create(EXPR_INC, expr_create_name("i"), NULL)
			  };
  char* expect = "for ( ; ; ) {\n}\nfor ( ; ; i++) {\n}\nfor ( ; i < n; ) {\n}\nfor ( ; i < n; i++) {\n}\nfor (i = 0; ; ) {\n}\nfor (i = 0; ; i++) {\n}\nfor (i = 0; i < n; ) {\n}\nfor (i = 0; i < n; i++) {\n}\n";

  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  for (int i = 0; i < (1 << 3); i++) {
    struct stmt* s = stmt_create(STMT_FOR, NULL,
				 (i & (1 << 2)) ? exprs[0] : NULL, (i & (1 << 1)) ? exprs[1] : NULL, (i & (1 << 0)) ? exprs[2] : NULL,
				 NULL, NULL, NULL);
    stmt_fprint(tmp, s, 0);
  }
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}
Status test_stmt_print_for_decl(void) {
  strcpy(test_type, "Testing: test_stmt_print_for_decl");
  Status status = SUCCESS;
  struct decl* d = decl_create("i", type_create(TYPE_INTEGER, NULL, NULL, NULL), NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_FOR, d, NULL, NULL, NULL, NULL, NULL, NULL);
  char* expect = "for (i: integer; ; ) {\n}\n";

  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}
Status test_stmt_print_for_init(void) {
  strcpy(test_type, "Testing: test_stmt_print_for_init");
  Status status = SUCCESS;
  struct decl* d = decl_create("i", type_create(TYPE_INTEGER, NULL, NULL, NULL), expr_create_integer_literal(0), NULL, NULL);
  struct stmt* s = stmt_create(STMT_FOR, d, NULL, NULL, NULL, NULL, NULL, NULL);
  char* expect = "for (i: integer = 0; ; ) {\n}\n";

  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_print_program(void) {
  strcpy(test_type, "Testing: test_print_program");
  Status status = SUCCESS;

  // components of statements for better readability
  struct type* integer = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct param_list* pend = param_list_create("argv", type_create(TYPE_ARRAY, type_create(TYPE_STRING, NULL, NULL, NULL), NULL, NULL), NULL);
  struct param_list* p = param_list_create("argc", integer, pend);
  struct expr* assign = expr_create(EXPR_ASSIGN, expr_create_name("i"), expr_create_integer_literal(0));
  struct expr* less = expr_create(EXPR_LESS, expr_create_name("i"), expr_create_name("n"));
  struct expr* inc = expr_create(EXPR_INC, expr_create_name("i"), NULL);
  struct decl* i = decl_create("i", integer, NULL, NULL, NULL);
  struct decl* n = decl_create("n", integer, expr_create_integer_literal(10), NULL, NULL);

  // structs that make up stmts in code
  struct stmt* return_stmt = stmt_create(STMT_RETURN, NULL, NULL, expr_create_integer_literal(0), NULL, NULL, NULL, NULL);
  struct stmt* for_body = stmt_create(STMT_PRINT, NULL, NULL, expr_create_string_literal("hello world!:)"), NULL, NULL, NULL, NULL);
  struct stmt* for_stmt = stmt_create(STMT_FOR, NULL, assign, less, inc, for_body, NULL, return_stmt);
  struct stmt* n_init = stmt_create(STMT_DECL, n, NULL, NULL, NULL, NULL, NULL, for_stmt);
  struct stmt* i_init = stmt_create(STMT_DECL, i, NULL, NULL, NULL, NULL, NULL, n_init);
  struct decl* d = decl_create("main", type_create(TYPE_FUNCTION, integer, p, NULL), NULL, i_init, NULL);

  char* expect = "main: function integer (argc: integer, argv: array [] string) = {\n  i: integer;\n  n: integer = 10;\n  for (i = 0; i < n; i++) {\n    print \"hello world!:)\";\n  }\n  return 0;\n}\n";
  FILE* tmp; tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  decl_fprint(tmp, d, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}
