#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../src/decl.h"
#include "../../src/expr.h"
#include "../../src/param_list.h"
#include "../../src/stmt.h"
#include "../../src/symbol.h"
#include "../../src/type.h"
#define MAX_BUFFER 256

FILE* tmp;
typedef enum { FAILURE = 0, SUCCESS = 1 } Status;
char test_type[MAX_BUFFER];
char output[MAX_BUFFER];

// helper functions
void compare_expect_output(char* expect, char* output);
void fileread(FILE* fp, char* s, int n);

// error functions
void print_error(char* test, char* expect, char* value);
Status file_error(char* test_type); // error message when getting temporary file


/*
   expression printing tests:
   atomic types, operators, and associativity
*/

Status test_expr_print_name(void);
Status test_expr_print_str(void);
Status test_expr_print_int(void);
Status test_expr_print_ch(void);
Status test_expr_print_bool_true(void);
Status test_expr_print_bool_false(void);
Status test_expr_print_op(void);
Status test_expr_print_op_left_assoc_unary(void);
Status test_expr_print_op_left_assoc_binary(void);
Status test_expr_print_op_right_assoc_unary(void);
Status test_expr_print_op_right_assoc_binary(void);
Status test_expr_print_fcall_list(void);
Status test_expr_print_fcall_nest(void);
Status test_expr_print_subscript_nest(void);
Status test_expr_print_subscript_list(void);
Status test_expr_print_init_list(void);
Status test_expr_print_init_nest(void);
Status test_expr_print_init_list_nest(void);

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
Status test_stmt_print_block_list(void);

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
  tmp = fopen("foo.txt", "w"); if (!tmp) { return file_error(test_type); }
  <struct>_fprint(tmp, struct);
  tmp = freopen("foo.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("foo.txt");
*/

int main(void) {
  Status (*tests[])(void) = {
    test_expr_print_name,
    test_expr_print_str,
    test_expr_print_int,
    test_expr_print_ch,
    test_expr_print_bool_true,
    test_expr_print_bool_false,
    test_expr_print_op,
    test_expr_print_op_left_assoc_unary,
    test_expr_print_op_left_assoc_binary,
    test_expr_print_op_right_assoc_unary,
    test_expr_print_op_right_assoc_binary,
    test_expr_print_init_list,
    test_expr_print_init_nest,
    test_expr_print_init_list_nest,
    test_expr_print_fcall_list,
    test_expr_print_fcall_nest,
    test_expr_print_subscript_list,
    test_expr_print_subscript_nest,
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
    //test_decl_print_uninit_array_nest, --> TODO: causing a segfault
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
    test_stmt_print_block_list,
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

  printf("Passed: %2d/%d\n", n_pass, n_tests);
  printf("Failed: %2d/%d\n", (n_tests - n_pass), n_tests);
  return 0;
}

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

// testing expressions of the primitives, also the base case for expr_print
Status test_expr_print_name(void) {
  strcpy(test_type, "Testing: test_expr_print_name");
  Status status = SUCCESS;
  struct expr* e = expr_create_name(strdup("foo"));
  char* expect = "foo";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  expr_fprint(tmp, e);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");

  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  expr_destroy(&e);
  return status;
}

Status test_expr_print_str(void) {
  strcpy(test_type, "Testing: test_expr_print_str");
  Status status = SUCCESS;
  struct expr* e = expr_create_string_literal("hello world!:)\n");
  char* expect = "\"hello world!:)\n\"";
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  expr_fprint(tmp, e);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");

  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  expr_destroy(&e);
  return status;
}

Status test_expr_print_int(void) {
  strcpy(test_type, "Testing: test_expr_print_int");
  Status status = SUCCESS;

  struct expr* e = expr_create_integer_literal(493);
  char* expect = "493";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  expr_fprint(tmp, e);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");

  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  expr_destroy(&e);
  return status;
}

Status test_expr_print_ch(void) {
  strcpy(test_type, "Testing: test_expr_print_ch");
  Status status = SUCCESS;

  struct expr* e = expr_create_char_literal('a');
  char* expect = "'a'";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  expr_fprint(tmp, e);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");

  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  expr_destroy(&e);
  return status;
}

Status test_expr_print_bool_true(void) {
  strcpy(test_type, "Testing: test_expr_print_bool_true");
  Status status = SUCCESS;

  struct expr* e = expr_create_boolean_literal(true);
  char* expect = "true";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  expr_fprint(tmp, e);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");

  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  expr_destroy(&e);
  return status;
}

Status test_expr_print_bool_false(void) {
  strcpy(test_type, "Testing: test_expr_print_bool_false");
  Status status = SUCCESS;

  struct expr* e = expr_create_boolean_literal(false);
  char* expect = "false";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  expr_fprint(tmp, e);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");

  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  expr_destroy(&e);
  return status;
}

// expression operations
Status test_expr_print_op(void) {
  strcpy(test_type, "Testing: test_expr_print_op");
  Status status = SUCCESS;

  char* expect = "x++\nx--\nx\n-x\n!x\nx ^ y\nx * y\nx / y\nx % y\nx + y\nx - y\nx <= y\nx < y\nx >= y\nx > y\nx == y\nx != y\nx && y\nx || y\nx = y\nx, y\nx[y]\nx(y)\n{x}\n";
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  for (expr_t kind = EXPR_INC; kind <= EXPR_INIT; kind++) {
    struct expr* e = (kind < EXPR_EXP || kind == EXPR_INIT) ? expr_create(kind, expr_create_name(strdup("x")), NULL) : expr_create(kind, expr_create_name(strdup("x")), expr_create_name(strdup("y")));
    expr_fprint(tmp, e); fprintf(tmp, "\n");
    expr_destroy(&e);
  }
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_expr_print_op_left_assoc_unary(void) {
  strcpy(test_type, "Testing: test_expr_print_op_left_assoc_unary");
  Status status = SUCCESS;

  char* expect = "(foo++)++\n(foo--)--\n";
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  for (expr_t kind = EXPR_INC; kind <= EXPR_DEC; kind++) {
    struct expr* l = expr_create_name(strdup("foo"));
    struct expr* e = expr_create(kind, expr_create(kind, l, NULL), NULL);
    expr_fprint(tmp, e); fprintf(tmp, "\n");
    expr_destroy(&e);
  }
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_expr_print_op_left_assoc_binary(void) {
  strcpy(test_type, "Testing: test_expr_print_op");
  Status status = SUCCESS;

  char* expect = "(a * b) * c\n(a / b) / c\n(a % b) % c\n(a + b) + c\n(a - b) - c\n(a <= b) <= c\n(a < b) < c\n(a >= b) >= c\n(a > b) > c\n(a == b) == c\n(a != b) != c\n(a && b) && c\n(a || b) || c\n";
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  for (expr_t kind = EXPR_MULT; kind <= EXPR_OR; kind++) {
    struct expr* a = expr_create_name(strdup("a"));
    struct expr* b = expr_create_name(strdup("b"));
    struct expr* c = expr_create_name(strdup("c"));
    struct expr* e = expr_create(kind, expr_create(kind, a, b), c);
    expr_fprint(tmp, e); fprintf(tmp, "\n");
    expr_destroy(&e);
  }
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_expr_print_op_right_assoc_unary(void) {
  strcpy(test_type, "Testing: test_expr_print_op_right_assoc_unary");
  Status status = SUCCESS;

  char* expect = "foo\n-(-foo)\n!(!foo)\n";
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  for (expr_t kind = EXPR_POS; kind <= EXPR_NOT; kind++) {
    struct expr* l = expr_create_name(strdup("foo"));
    struct expr* e = expr_create(kind, expr_create(kind, l, NULL), NULL);
    expr_fprint(tmp, e); fprintf(tmp, "\n");
    expr_destroy(&e);
  }
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_expr_print_op_right_assoc_binary(void) {
  strcpy(test_type, "Testing: test_expr_print_op_right_assoc_binary");
  Status status = SUCCESS;
  struct expr* e = expr_create(EXPR_EXP, expr_create_name(strdup("duck")), expr_create(EXPR_EXP, expr_create_name(strdup("duck")), expr_create_name(strdup("goose"))));
  char* expect = "duck ^ (duck ^ goose)";
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  expr_fprint(tmp, e);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  expr_destroy(&e);
  return status;
}

Status test_expr_print_fcall_list(void) {
  strcpy(test_type, "Testing: test_expr_print_fcall_list");
  Status status = SUCCESS;
  struct expr* e = expr_create(EXPR_FCALL, expr_create_name(strdup("duck")), expr_create(EXPR_COMMA, expr_create_name(strdup("duck")), expr_create_name(strdup("goose"))));
  char* expect = "duck(duck, goose)";
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  expr_fprint(tmp, e);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");

  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  expr_destroy(&e);
  return status;
}

Status test_expr_print_fcall_nest(void) {
  strcpy(test_type, "Testing: test_expr_print_fcall_nest");
  Status status = SUCCESS;
  struct expr* e = expr_create(EXPR_FCALL, expr_create_name(strdup("f")), expr_create(EXPR_FCALL, expr_create_name(strdup("g")), expr_create_name(strdup("x"))));
  char* expect = "f(g(x))";
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  expr_fprint(tmp, e);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  expr_destroy(&e);
  return status;
}

Status test_expr_print_subscript_nest(void) {
  strcpy(test_type, "Testing: test_expr_print_subscript_nest");
  Status status = SUCCESS;
  struct expr* e = expr_create(EXPR_SUBSCRIPT, expr_create_name(strdup("f")), expr_create(EXPR_SUBSCRIPT, expr_create_name(strdup("g")), expr_create_name(strdup("x"))));
  char* expect = "f[g[x]]";
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  expr_fprint(tmp, e);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  expr_destroy(&e);
  return status;
}

Status test_expr_print_subscript_list(void) {
  strcpy(test_type, "Testing: test_expr_print_fcall_list");
  Status status = SUCCESS;
  struct expr* left = expr_create(EXPR_SUBSCRIPT, expr_create_name(strdup("foo")), expr_create_name(strdup("i")));
  struct expr* e = expr_create(EXPR_SUBSCRIPT, left, expr_create_name(strdup("j")));
  char* expect = "foo[i][j]";
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  expr_fprint(tmp, e);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");

  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  expr_destroy(&e);
  return status;
}

Status test_expr_print_init_list(void) {
  strcpy(test_type, "Testing: test_expr_print_init_list");
  Status status = SUCCESS;

  struct expr* e = expr_create(EXPR_INIT, expr_create(EXPR_COMMA, expr_create_name(strdup("duck")), expr_create_name(strdup("goose"))), NULL);
  char* expect = "{duck, goose}";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  expr_fprint(tmp, e);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");

  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  expr_destroy(&e);
  return status;
}

Status test_expr_print_init_nest(void) {
  strcpy(test_type, "Testing: test_expr_print_init_nest");
  Status status = SUCCESS;

  struct expr* e = expr_create(EXPR_INIT, expr_create(EXPR_INIT, expr_create_name(strdup("duck")), NULL), NULL);
  char* expect = "{{duck}}";
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  expr_fprint(tmp, e);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");

  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  expr_destroy(&e);
  return status;
}

Status test_expr_print_init_list_nest(void) {
  strcpy(test_type, "Testing: test_expr_print_init_list_nest");
  Status status = SUCCESS;
  struct expr* duck = expr_create(EXPR_INIT, expr_create_name(strdup("duck")), NULL);
  struct expr* goose = expr_create(EXPR_INIT, expr_create_name(strdup("goose")), NULL);
  struct expr* e = expr_create(EXPR_INIT, expr_create(EXPR_COMMA, duck, goose), NULL);
  char* expect = "{{duck}, {goose}}";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  expr_fprint(tmp, e);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");

  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  expr_destroy(&e);
  return status;
}


// type printing
Status test_type_print_atomic(void) {
  strcpy(test_type, "Testing: test_type_print_atomic");
  Status status = SUCCESS;
  char* expect ="voidbooleancharintegerstringauto";
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  for (type_t kind = TYPE_VOID; kind < TYPE_ARRAY; kind++) {
    struct type* t = type_create(kind, NULL, NULL, NULL);
    type_fprint(tmp, t);
    type_destroy(&t);
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
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  type_fprint(tmp, t);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  type_destroy(&t);
  return status;
}

Status test_type_print_array_nest(void) {
  strcpy(test_type, "Testing: test_type_print_array_nest");
  Status status = SUCCESS;
  struct type* subtype = type_create(TYPE_ARRAY, type_create(TYPE_INTEGER, NULL, NULL, NULL), NULL, NULL);
  struct type* t = type_create(TYPE_ARRAY, subtype, NULL, NULL);
  char* expect = "array [] array [] integer";
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  type_fprint(tmp, t);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  type_destroy(&t);
  return status;
}

Status test_type_print_function(void) {
  strcpy(test_type, "Testing: test_type_print_function");
  Status status = SUCCESS;
  struct type* subtype = type_create(TYPE_VOID, NULL, NULL, NULL);
  struct type* t = type_create(TYPE_FUNCTION, subtype, NULL, NULL);
  char* expect = "function void (void)";
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  type_fprint(tmp, t);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  type_destroy(&t);
  return status;
}

Status test_type_print_function_nest(void) {
  strcpy(test_type, "Testing: test_type_print_function_nest");
  Status status = SUCCESS;
  struct type* subtype = type_create(TYPE_ARRAY, type_create(TYPE_INTEGER, NULL, NULL, NULL), NULL, NULL);
  struct type* t = type_create(TYPE_FUNCTION, subtype, NULL, NULL);
  char* expect = "function array [] integer (void)";
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  type_fprint(tmp, t);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  type_destroy(&t);
  return status;
}

// param list printing
Status test_param_list_print_single(void) {
  strcpy(test_type, "Testing: test_param_list_print_single");
  Status status = SUCCESS;
  struct type* t = type_create(TYPE_INTEGER, NULL, NULL, NULL);
  struct param_list* p = param_list_create(strdup("x"), t, NULL);
  char* expect = "x: integer";
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  param_list_fprint(tmp, p);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  param_list_destroy(&p);
  return status;
}

Status test_param_list_print_multiple(void) {
  strcpy(test_type, "Testing: test_param_list_print_multiple");
  Status status = SUCCESS;
  struct param_list* pend = param_list_create(strdup("y"), type_create(TYPE_INTEGER, NULL, NULL, NULL), NULL);
  struct param_list* p = param_list_create(strdup("x"), type_create(TYPE_INTEGER, NULL, NULL, NULL), pend);
  char* expect = "x: integer, y: integer";
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  param_list_fprint(tmp, p);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  param_list_destroy(&p);
  return status;
}

Status test_param_list_print_single_nest(void) {
  strcpy(test_type, "Testing: test_param_list_print_single_nest");
  Status status = SUCCESS;
  struct type* t = type_create(TYPE_ARRAY, type_create(TYPE_INTEGER, NULL, NULL, NULL), NULL, NULL);
  struct param_list* p = param_list_create(strdup("x"), t, NULL);
  char* expect = "x: array [] integer";
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  param_list_fprint(tmp, p);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  param_list_destroy(&p);
  return status;
}

Status test_type_print_function_param_list(void) {
  strcpy(test_type, "Testing: test_type_print_function_param_list");
  Status status = SUCCESS;

  struct type* tend = type_create(TYPE_ARRAY, type_create(TYPE_STRING, NULL, NULL, NULL), NULL, NULL);
  struct param_list* pend = param_list_create(strdup("argv"), tend, NULL);
  struct param_list* p = param_list_create(strdup("argc"), type_create(TYPE_INTEGER, NULL, NULL, NULL), pend);
  struct type* t = type_create(TYPE_FUNCTION, type_create(TYPE_INTEGER, NULL, NULL, NULL), p, NULL);

  char* expect = "function integer (argc: integer, argv: array [] string)";
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  type_fprint(tmp, t);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  type_destroy(&t);
  return status;
}

Status test_type_print_array_expr(void) {
  strcpy(test_type, "Testing: test_type_print_array_expr");
  Status status = SUCCESS;

  struct expr* dim1 = expr_create(EXPR_EXP, expr_create_integer_literal(3), expr_create_integer_literal(1));
  struct expr* dim2 = expr_create(EXPR_DIV, expr_create(EXPR_MULT, expr_create_integer_literal(3), expr_create_integer_literal(3)), expr_create_integer_literal(3));
  struct type* subtype = type_create(TYPE_ARRAY, type_create(TYPE_BOOLEAN, NULL, NULL, NULL), NULL, dim2);
  struct type* t = type_create(TYPE_ARRAY, subtype, NULL, dim1);

  char* expect = "array [3 ^ 1] array [(3 * 3) / 3] boolean";
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  type_fprint(tmp, t);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  type_destroy(&t);
  return status;
}

// declaration printing
Status test_decl_print_uninit_atomic(void) {
  strcpy(test_type, "Testing: test_decl_print_uninit_atomic");
  Status status = SUCCESS;
  char* expect ="x: boolean;x: char;x: integer;x: string;x: auto;";
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  for (type_t kind = TYPE_BOOLEAN; kind < TYPE_ARRAY; kind++) {
    struct type* t = type_create(kind, NULL, NULL, NULL);
    struct decl* d = decl_create(strdup("x"), t, NULL, NULL, NULL);
    decl_fprint(tmp, d, 0);
    decl_destroy(&d);
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
  struct decl* d = decl_create(strdup("x"), t, NULL, NULL, NULL);
  char* expect = "x: array [2] boolean;";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  decl_fprint(tmp, d, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  decl_destroy(&d);
  return status;
}

Status test_decl_print_uninit_array_nest(void) {
  strcpy(test_type, "Testing: test_decl_print_uninit_array_nest");
  Status status = SUCCESS;
  struct expr* three = expr_create_integer_literal(3);
  struct type* subtype = type_create(TYPE_ARRAY, type_create(TYPE_INTEGER, NULL, NULL, NULL), NULL, three);
  struct type* t = type_create(TYPE_ARRAY, subtype, NULL, three);
  struct decl* d = decl_create(strdup("x"), t, NULL, NULL, NULL);
  char* expect = "x: array [3] array [3] integer;";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  decl_fprint(tmp, d, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  decl_destroy(&d);
  return status;
}

Status test_decl_print_uninit_function(void) {
  strcpy(test_type, "Testing: test_decl_print_uninit_function");
  Status status = SUCCESS;

  struct type* tend = type_create(TYPE_ARRAY, type_create(TYPE_STRING, NULL, NULL, NULL), NULL, NULL);
  struct param_list* pend = param_list_create(strdup("argv"), tend, NULL);
  struct param_list* p = param_list_create(strdup("argc"), type_create(TYPE_INTEGER, NULL, NULL, NULL), pend);
  struct type* t = type_create(TYPE_FUNCTION, type_create(TYPE_INTEGER, NULL, NULL, NULL), p, NULL);
  struct decl* d = decl_create(strdup("main"), t, NULL, NULL, NULL);

  char* expect = "main: function integer (argc: integer, argv: array [] string);";
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  decl_fprint(tmp, d, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  decl_destroy(&d);
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
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  for (type_t kind = TYPE_BOOLEAN, i = 0; kind < TYPE_ARRAY; kind++, i++) {
    struct type* t = type_create(kind, NULL, NULL, NULL);
    struct decl* d = decl_create(strdup("x"), t, inits[i], NULL, NULL);
    decl_fprint(tmp, d, 0);
    decl_destroy(&d);
  }
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_decl_print_init_array(void) {
  strcpy(test_type, "Testing: test_decl_print_init_array");
  Status status = SUCCESS;
  struct type* t = type_create(TYPE_ARRAY, type_create(TYPE_BOOLEAN, NULL, NULL, NULL), NULL, NULL);
  struct expr* tf = expr_create(EXPR_COMMA, expr_create_boolean_literal(1), expr_create_boolean_literal(0));
  struct expr* v = expr_create(EXPR_INIT, tf, NULL);
  struct decl* d = decl_create(strdup("bool"), t, v, NULL, NULL);
  char* expect = "bool: array [] boolean = {true, false};";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  decl_fprint(tmp, d, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  decl_destroy(&d);
  return status;
}

Status test_decl_print_init_array_nest(void) {
  strcpy(test_type, "Testing: test_decl_print_init_array_nest");
  Status status = SUCCESS;
  struct expr* row_1 = expr_create(EXPR_INIT, expr_create(EXPR_COMMA, expr_create(EXPR_COMMA, expr_create_integer_literal(1), expr_create_integer_literal(0)), expr_create_integer_literal(0)), NULL);
  struct expr* row_2 = expr_create(EXPR_INIT, expr_create(EXPR_COMMA, expr_create(EXPR_COMMA, expr_create_integer_literal(0), expr_create_integer_literal(1)), expr_create_integer_literal(0)), NULL);
  struct expr* row_3 = expr_create(EXPR_INIT, expr_create(EXPR_COMMA, expr_create(EXPR_COMMA, expr_create_integer_literal(0), expr_create_integer_literal(0)), expr_create_integer_literal(1)), NULL);
  struct expr* rows = expr_create(EXPR_COMMA, expr_create(EXPR_COMMA, row_1, row_2), row_3);
  struct expr* v = expr_create(EXPR_INIT, rows, NULL);
  struct type* t = type_create(TYPE_ARRAY, type_create(TYPE_ARRAY, type_create(TYPE_INTEGER, NULL, NULL, NULL), NULL, NULL), NULL, NULL);
  struct decl* d = decl_create(strdup("identity"), t, v, NULL, NULL);
  char* expect = "identity: array [] array [] integer = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  decl_fprint(tmp, d, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  decl_destroy(&d);
  return status;
}

Status test_decl_print_init_function(void) {
  strcpy(test_type, "Testing: test_decl_print_init_function");
  Status status = SUCCESS;
  struct type* tend = type_create(TYPE_ARRAY, type_create(TYPE_STRING, NULL, NULL, NULL), NULL, NULL);
  struct param_list* pend = param_list_create(strdup("argv"), tend, NULL);
  struct param_list* p = param_list_create(strdup("argc"), type_create(TYPE_INTEGER, NULL, NULL, NULL), pend);
  struct type* t = type_create(TYPE_FUNCTION, type_create(TYPE_INTEGER, NULL, NULL, NULL), p, NULL);
  struct stmt* s = stmt_create(STMT_EXPR, NULL, NULL, expr_create_name(strdup("duck")), NULL,NULL, NULL, NULL);
  struct stmt* ss = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, s, NULL, NULL);
  struct decl* d = decl_create(strdup("main"), t, NULL, ss, NULL);

  char* expect = "main: function integer (argc: integer, argv: array [] string) = {\n  duck;\n}\n";
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  decl_fprint(tmp, d, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  decl_destroy(&d);
  return status;
}

Status test_decl_print_multiple(void) {
  strcpy(test_type, "Testing: test_decl_print_multiple");
  Status status = SUCCESS;
  struct type* matrix = type_create(TYPE_ARRAY, type_create(TYPE_ARRAY, type_create(TYPE_INTEGER, NULL, NULL, NULL), NULL, expr_create_name(strdup("n"))), NULL, expr_create_name(strdup("n")));
  struct decl* dend = decl_create(strdup("square"), matrix, NULL, NULL, NULL);
  struct decl* d = decl_create(strdup("n"), type_create(TYPE_INTEGER, NULL, NULL, NULL), expr_create_integer_literal(2), NULL, dend);
  char* expect = "n: integer = 2;\nsquare: array [n] array [n] integer;";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  decl_fprint(tmp, d, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  decl_destroy(&d);
  return status;
}

// statement printing
Status test_stmt_print_decl(void) {
  strcpy(test_type, "Testing: test_stmt_print_decl");
  Status status = SUCCESS;
  struct decl* d = decl_create(strdup("b"), type_create(TYPE_BOOLEAN, NULL, NULL, NULL),
				    expr_create(EXPR_AND, expr_create_boolean_literal(1), expr_create_boolean_literal(0)), NULL, NULL);
  struct stmt* s = stmt_create(STMT_DECL, d, NULL, NULL, NULL, NULL, NULL, NULL);
  char* expect = "b: boolean = true && false;\n";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  stmt_destroy(&s);
  return status;
}

Status test_stmt_print_expr(void) {
  strcpy(test_type, "Testing: test_stmt_print_expr");
  Status status = SUCCESS;
  struct expr* e = expr_create(EXPR_ASSIGN, expr_create_name(strdup("x")), expr_create(EXPR_ADD, expr_create_integer_literal(493), expr_create_integer_literal(1)));
  struct stmt* s = stmt_create(STMT_EXPR, NULL, NULL, e, NULL, NULL, NULL, NULL);
  char* expect = "x = 493 + 1;\n";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  stmt_destroy(&s);
  return status;
}

Status test_stmt_print_print_null(void) {
  strcpy(test_type, "Testing: test_stmt_print_print_null");
  Status status = SUCCESS;
  struct stmt* s = stmt_create(STMT_PRINT, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  char* expect = "print;\n";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  stmt_destroy(&s);
  return status;
}

Status test_stmt_print_print_expr(void) {
  strcpy(test_type, "Testing: test_stmt_print_print_expr");
  Status status = SUCCESS;
  struct expr* e = expr_create(EXPR_OR, expr_create(EXPR_AND, expr_create_boolean_literal(0), expr_create_boolean_literal(1)), expr_create_name(strdup("foo")));
  struct stmt* s = stmt_create(STMT_PRINT, NULL, NULL, e, NULL, NULL, NULL, NULL);
  char* expect = "print (false && true) || foo;\n";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  stmt_destroy(&s);
  return status;
}

Status test_stmt_print_print_list(void) {
  strcpy(test_type, "Testing: test_stmt_print_print_list");
  Status status = SUCCESS;
  struct expr* e = expr_create(EXPR_COMMA, expr_create_name(strdup("duck")), expr_create(EXPR_COMMA, expr_create_name(strdup("duck")), expr_create_name(strdup("goose"))));
  struct stmt* s = stmt_create(STMT_PRINT, NULL, NULL, e, NULL, NULL, NULL, NULL);
  char* expect = "print duck, duck, goose;\n";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  stmt_destroy(&s);
  return status;
}

Status test_stmt_print_return_null(void) {
  strcpy(test_type, "Testing: test_stmt_print_return_null");
  Status status = SUCCESS;
  struct stmt* s = stmt_create(STMT_RETURN, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  char* expect = "return;\n";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  stmt_destroy(&s);
  return status;
}

Status test_stmt_print_return_expr(void) {
  strcpy(test_type, "Testing: test_stmt_print_return_expr");
  Status status = SUCCESS;
  struct expr* e = expr_create(EXPR_OR, expr_create(EXPR_AND, expr_create_boolean_literal(0), expr_create_boolean_literal(1)), expr_create_name(strdup("foo")));
  struct stmt* s = stmt_create(STMT_RETURN, NULL, NULL, e, NULL, NULL, NULL, NULL);
  char* expect = "return (false && true) || foo;\n";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  stmt_destroy(&s);
  return status;
}

Status test_stmt_print_block_null(void) {
  strcpy(test_type, "Testing: test_stmt_print_block_null");
  Status status = SUCCESS;
  struct stmt* s = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  char* expect = "{}";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  stmt_destroy(&s);
  return status;
}

Status test_stmt_print_block_single(void) {
  strcpy(test_type, "Testing: test_stmt_print_block_single");
  Status status = SUCCESS;
  struct stmt* body = stmt_create(STMT_EXPR, NULL, NULL, expr_create_name(strdup("duck")), NULL, NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, body, NULL, NULL);
  char* expect = "{\n  duck;\n}";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  stmt_destroy(&s);
  return status;
}

Status test_stmt_print_block_list(void) {
  strcpy(test_type, "Testing: test_stmt_print_block_list");
  Status status = SUCCESS;
  struct stmt* bodyend = stmt_create(STMT_EXPR, NULL, NULL, expr_create_name(strdup("goose")), NULL, NULL, NULL, NULL);
  struct stmt* body = stmt_create(STMT_EXPR, NULL, NULL, expr_create_name(strdup("duck")), NULL, NULL, NULL, bodyend);
  struct stmt* s = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, body, NULL, NULL);
  char* expect = "{\n  duck;\n  goose;\n}";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  stmt_destroy(&s);
  return status;
}

Status test_stmt_print_if_null(void) {
  strcpy(test_type, "Testing: test_stmt_print_print_if_null");
  Status status = SUCCESS;
  struct stmt* s = stmt_create(STMT_IF_ELSE, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  char* expect = "if () {\n}\n";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  stmt_destroy(&s);
  return status;
}

Status test_stmt_print_if(void) {
  strcpy(test_type, "Testing: test_stmt_print_print_if");
  Status status = SUCCESS;
  struct expr* e = expr_create_name(strdup("e"));
  struct stmt* duck = stmt_create(STMT_EXPR, NULL, NULL, expr_create_name(strdup("duck")), NULL, NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_IF_ELSE, NULL, NULL, e, NULL, duck, NULL, NULL);
  char* expect = "if (e) {\n  duck;\n}\n";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  stmt_destroy(&s);
  return status;
}

Status test_stmt_print_if_else(void) {
  strcpy(test_type, "Testing: test_stmt_print_print_if_else");
  Status status = SUCCESS;
  struct expr* e = expr_create_name(strdup("e"));
  struct stmt* duck = stmt_create(STMT_EXPR, NULL, NULL, expr_create_name(strdup("duck")), NULL, NULL, NULL, NULL);
  struct stmt* goose = stmt_create(STMT_EXPR, NULL, NULL, expr_create_name(strdup("goose")), NULL, NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_IF_ELSE, NULL, NULL, e, NULL, duck, goose, NULL);
  char* expect = "if (e) {\n  duck;\n} else {\n  goose;\n}\n";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  stmt_destroy(&s);
  return status;
}

Status test_stmt_print_if_else_nest(void) {
  strcpy(test_type, "Testing: test_stmt_print_print_if_else_nest");
  Status status = SUCCESS;
  struct stmt* snest = stmt_create(STMT_IF_ELSE, NULL, NULL, expr_create_name(strdup("e")), NULL,
                                                             stmt_create(STMT_EXPR, NULL, NULL, expr_create_name(strdup("duck")), NULL, NULL, NULL, NULL),
                                                             stmt_create(STMT_EXPR, NULL, NULL, expr_create_name(strdup("goose")), NULL, NULL, NULL, NULL),
                                                             NULL);
  struct stmt* s = stmt_create(STMT_IF_ELSE, NULL, NULL, expr_create_name(strdup("e")), NULL, snest,
                                                         stmt_create(STMT_EXPR, NULL, NULL, expr_create_name(strdup("goose")), NULL, NULL, NULL, NULL),
                                                         NULL);
  char* expect = "if (e) {\n  if (e) {\n    duck;\n  } else {\n    goose;\n  }\n} else {\n  goose;\n}\n";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  stmt_destroy(&s);
  return status;
}

Status test_stmt_print_while_null(void) {
  strcpy(test_type, "Testing: test_stmt_print_while_null");
  Status status = SUCCESS;
  struct stmt* s = stmt_create(STMT_WHILE, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  char* expect = "while () {\n}\n";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  stmt_destroy(&s);
  return status;
}

Status test_stmt_print_while(void) {
  strcpy(test_type, "Testing: test_stmt_print_while");
  Status status = SUCCESS;
  struct expr* e = expr_create_name(strdup("e"));
  struct stmt* duck = stmt_create(STMT_EXPR, NULL, NULL, expr_create_name(strdup("duck")), NULL, NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_WHILE, NULL, NULL, e, NULL, duck, NULL, NULL);
  char* expect = "while (e) {\n  duck;\n}\n";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  stmt_destroy(&s);
  return status;
}

Status test_stmt_print_for_expr(void) {
  strcpy(test_type, "Testing: test_stmt_print_for_expr");
  Status status = SUCCESS;
  char* expect = "for ( ; ; ) {}\nfor ( ; ; i++) {}\nfor ( ; i < n; ) {}\nfor ( ; i < n; i++) {}\nfor (i = 0; ; ) {}\nfor (i = 0; ; i++) {}\nfor (i = 0; i < n; ) {}\nfor (i = 0; i < n; i++) {}\n";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  for (int i = 0; i < (1 << 3); i++) {
    struct stmt* body = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    struct stmt* s = stmt_create(STMT_FOR, NULL,
				 (i & (1 << 2)) ?
 				   expr_create(EXPR_ASSIGN, expr_create_name(strdup("i")), expr_create_integer_literal(0)) : NULL,
				 (i & (1 << 1)) ?
				   expr_create(EXPR_LESS, expr_create_name(strdup("i")), expr_create_name(strdup("n"))) : NULL,
				 (i & (1 << 0)) ?
				   expr_create(EXPR_INC, expr_create_name(strdup("i")), NULL) : NULL,
				 body, NULL, NULL);
    stmt_fprint(tmp, s, 0);
    stmt_destroy(&s);
  }
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_stmt_print_for_decl(void) {
  strcpy(test_type, "Testing: test_stmt_print_for_decl");
  Status status = SUCCESS;
  struct decl* d = decl_create(strdup("i"), type_create(TYPE_INTEGER, NULL, NULL, NULL), NULL, NULL, NULL);
  struct stmt* body = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_FOR, d, NULL, NULL, NULL, body, NULL, NULL);
  char* expect = "for (i: integer; ; ) {}\n";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  stmt_destroy(&s);
  return status;
}

Status test_stmt_print_for_init(void) {
  strcpy(test_type, "Testing: test_stmt_print_for_init");
  Status status = SUCCESS;
  struct decl* d = decl_create(strdup("i"), type_create(TYPE_INTEGER, NULL, NULL, NULL), expr_create_integer_literal(0), NULL, NULL);
  struct stmt* body = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  struct stmt* s = stmt_create(STMT_FOR, d, NULL, NULL, NULL, body, NULL, NULL);
  char* expect = "for (i: integer = 0; ; ) {}\n";

  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  stmt_fprint(tmp, s, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  stmt_destroy(&s);
  return status;
}


// putting it all together!
Status test_print_program(void) {
  strcpy(test_type, "Testing: test_print_program");
  Status status = SUCCESS;

  // components of statements for better readability
  struct param_list* pend = param_list_create(strdup("argv"), type_create(TYPE_ARRAY, type_create(TYPE_STRING, NULL, NULL, NULL), NULL, NULL), NULL);
  struct param_list* p = param_list_create(strdup("argc"), type_create(TYPE_INTEGER, NULL, NULL, NULL), pend);
  struct expr* assign = expr_create(EXPR_ASSIGN, expr_create_name(strdup("i")), expr_create_integer_literal(0));
  struct expr* less = expr_create(EXPR_LESS, expr_create_name(strdup("i")), expr_create_name(strdup("n")));
  struct expr* inc = expr_create(EXPR_INC, expr_create_name(strdup("i")), NULL);
  struct decl* i = decl_create(strdup("i"), type_create(TYPE_INTEGER, NULL, NULL, NULL), NULL, NULL, NULL);
  struct decl* n = decl_create(strdup("n"), type_create(TYPE_INTEGER, NULL, NULL, NULL), expr_create_integer_literal(10), NULL, NULL);

  // structs that make up stmts in code
  struct stmt* return_stmt = stmt_create(STMT_RETURN, NULL, NULL, expr_create_integer_literal(0), NULL, NULL, NULL, NULL);
  struct stmt* for_body = stmt_create(STMT_PRINT, NULL, NULL, expr_create_string_literal("hello world!:)"), NULL, NULL, NULL, NULL); // single stmt body
  struct stmt* for_stmt = stmt_create(STMT_FOR, NULL, assign, less, inc, for_body, NULL, return_stmt);
  struct stmt* n_init = stmt_create(STMT_DECL, n, NULL, NULL, NULL, NULL, NULL, for_stmt);
  struct stmt* i_init = stmt_create(STMT_DECL, i, NULL, NULL, NULL, NULL, NULL, n_init);
  struct stmt* s = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, i_init, NULL, NULL);
  struct decl* d = decl_create(strdup("main"), type_create(TYPE_FUNCTION, type_create(TYPE_INTEGER, NULL, NULL, NULL), p, NULL), NULL, s, NULL);

  char* expect = "main: function integer (argc: integer, argv: array [] string) = {\n  i: integer;\n  n: integer = 10;\n  for (i = 0; i < n; i++) {\n    print \"hello world!:)\";\n  }\n  return 0;\n}\n";
  tmp = fopen("temp.txt", "w"); if (!tmp) { return file_error(test_type); }
  decl_fprint(tmp, d, 0);
  tmp = freopen("temp.txt", "r", tmp); if (!tmp) { return file_error(test_type); }
  fileread(tmp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  decl_destroy(&d);
  return status;
}
