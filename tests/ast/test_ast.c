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
#define MAX_BUFFER 512

extern FILE* yyin;
extern void yyrestart();
extern int yyparse();
extern char error_text[MAX_BUFFER]; // print_error sprintf error messages
extern unsigned char eof;
char output[MAX_BUFFER];

// parser outputs
extern struct stmt* test_parser_result;
extern struct decl* parser_result;

typedef enum { FAILURE = 0, SUCCESS = 1 } Status;
char test_type[MAX_BUFFER];

// helper functions
void fileread(FILE* fp, char* s, int n);

// error functions
void print_error(char* test, char* expect, char* value);
Status file_error(char* test_type, char* filename);

// expression testing
Status test_expr_vanilla(void);
Status test_expr_associativity(void);
Status test_expr_precedence(void);
Status test_expr_postfix_binary(void);

int main(int argc, const char* argv[]) {
  Status (*tests[])(void) = {
    test_expr_vanilla,
    test_expr_associativity,
    test_expr_precedence,
    test_expr_postfix_binary
  };

  int n_tests = sizeof(tests)/sizeof(tests[0]);
  int n_pass = 0;

  printf("Running %d tests...\n", n_tests);
  for (int i = 0; i < n_tests; i++) { if (tests[i]()) { n_pass++; }}

  printf("Passed: %d/%d\n", n_pass, n_tests);
  printf("Failed: %d/%d\n", (n_tests - n_pass), n_tests);
  return 0;
}

// reads contents of file into string s, closes file
void fileread(FILE* fp, char* s, int n) {
  int i; for (i = 0; i < n && !feof(fp); i++) s[i] = fgetc(fp);
  s[i-1] = '\0'; fclose(fp);
}

Status file_error(char* test_type, char* filename) {
  printf("Failed to open %s for %s.\n Test failure.\n", filename, test_type);
  return FAILURE;
}

void print_error(char* test, char* expect, char* value) { printf("Error:\t[%s]:\n\tExpected:\n[%s]\n\tReceived:\n[%s]\n", test, expect, value); }



/*
TEST FUNCTION TEMPLATE
Status <function_name>(void) {
  strcpy(test_type, "<function_name>");
  char* filename = "./tests/ast/<filename>.bminor";
  Status status = SUCCESS;

  // file stuff
  yyin = fopen(filename, "r");
  if (!yyin) { return file_error(test_type, filename); }

  FILE* ifp = fopen("temp.txt", "w"); if (!ifp) { return file_error(test_type); }
  // write to temporary file
  char* expect = "<expected result>";
  eof = 0;
  for(int i = 0; !eof; i++) { if (yyparse() == 0) stmt_fprint(ifp, test_parser_result, 0); test_parser_result = NULL; }


  // get resulting print output
  fclose(yyin); ifp = freopen("temp.txt", "r", ifp); if (!ifp) { return file_error(test_type); }
  fileread(ifp, output, MAX_BUFFER); remove("temp.txt");

  // compare output
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

*/
// expression testing
Status test_expr_vanilla(void) {
  strcpy(test_type, "test_expr_vanilla");
  char* filename = "./tests/ast/expr_vanilla.bminor";
  Status status = SUCCESS;
  yyin = fopen(filename, "r"); if (!yyin) { return file_error(test_type, filename); }
  FILE* ifp = fopen("temp.txt", "w"); if (!ifp) { return file_error(test_type, "temp.txt"); }
  char* expect = "493;\n'q';\n\"duck\";\ntrue;\nfoo;\nf++;\nf--;\nf(0);\nf[0];\n1;\n-1;\n!true;\n0 ^ 1;\n0 * 1;\n0 / 1;\n0 % 1;\n0 + 1;\n0 - 1;\n0 < 1;\n0 <= 1;\n0 > 1;\n0 >= 1;\n0 == 1;\n0 != 1;\ntrue && false;\ntrue || false;\nf = 0;\n0, 1;\n";
  eof = 0;
  for(int i = 0; !eof; i++) if (yyparse() == 0) stmt_fprint(ifp, test_parser_result, 0);
  fclose(yyin); ifp = freopen("temp.txt", "r", ifp); if (!ifp) { return file_error(test_type, "temp.txt"); }
  fileread(ifp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_expr_associativity(void) {
  strcpy(test_type, "test_expr_associativity");
  char* filename = "./tests/ast/expr_associativity.bminor";
  Status status = SUCCESS;
  yyrestart(yyin); yyin = fopen(filename, "r"); if (!yyin) { return file_error(test_type, filename); }
  FILE* ifp = fopen("temp.txt", "w"); if (!ifp) { return file_error(test_type, "temp.txt"); }
  char* expect = "(f++)++;\n(f--)--;\nf(f(0));\nf[f[0]];\nf;\n-(-f);\n!(!true);\n0 ^ (1 ^ 2);\n(0 * 1) * 2;\n(0 / 1) / 2;\n(0 % 1) % 2;\n(0 + 1) + 2;\n(0 - 1) - 2;\n(0 < 1) < 2;\n(0 <= 1) <= 2;\n(0 > 1) > 2;\n(0 >= 1) >= 2;\n(0 == 1) == 2;\n(0 != 1) != 2;\n(true && false) && true;\n(true || false) || true;\nf = foo = 0;\n0, 1, 2;\n";
  eof = 0;
  for(int i = 0; !eof; i++) if (yyparse() == 0) { stmt_fprint(ifp, test_parser_result, 0); test_parser_result = NULL; }
  fclose(yyin); ifp = freopen("temp.txt", "r", ifp); if (!ifp) { return file_error(test_type, "temp.txt"); }
  fileread(ifp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

// foo = +a--^b%c+d<e==f&&g||h,bar=-a++^b*c+d,baz=!a[b]<=c!=d&&e||f, qux=-a(b)--^c/d+e==f&&g||h;
Status test_expr_precedence(void) {
  strcpy(test_type, "test_expr_precedence");
  char* filename = "./tests/ast/expr_precedence.bminor";
  Status status = SUCCESS;
  yyrestart(yyin); yyin = fopen(filename, "r"); if (!yyin) { return file_error(test_type, filename); }
  FILE* ifp = fopen("temp.txt", "w"); if (!ifp) { return file_error(test_type, "temp.txt"); } eof = 0;
  char* expect = "foo = (((((((a--) ^ b) % c) + d) < e) == f) && g) || h, bar = (((-(a++)) ^ b) * c) + d, baz = ((((!(a[b])) <= c) != d) && e) || f, qux = ((((((-((a(b))--)) ^ c) / d) + e) == f) && g) || h;\n";
  for(int i = 0; !eof; i++) if (yyparse() == 0) { stmt_fprint(ifp, test_parser_result, 0); test_parser_result = NULL; }
  fclose(yyin); ifp = freopen("temp.txt", "r", ifp); if (!ifp) { return file_error(test_type, "temp.txt"); }
  fileread(ifp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}

Status test_expr_postfix_binary(void) {
  strcpy(test_type, "test_expr_postfix_binary");
  char* filename = "./tests/ast/expr_postfix.bminor";
  Status status = SUCCESS;
  yyrestart(yyin); yyin = fopen(filename, "r"); if (!yyin) { return file_error(test_type, filename); }
  FILE* ifp = fopen("temp.txt", "w"); if (!ifp) { return file_error(test_type, "temp.txt"); } eof = 0;
  char* expect = "x[i][j];\nx[i][j][k];\nf(x)(y);\nf(duck)(duck)(goose);\nf(a, b, c)(x, y, z)[i][j][k];\n";
  for(int i = 0; !eof; i++) if (yyparse() == 0) { stmt_fprint(ifp, test_parser_result, 0); test_parser_result = NULL; }
  fclose(yyin); ifp = freopen("temp.txt", "r", ifp); if (!ifp) { return file_error(test_type, "temp.txt"); }
  fileread(ifp, output, MAX_BUFFER); remove("temp.txt");
  if (strcmp(output, expect)) { print_error(test_type, expect, output); status = FAILURE; }
  return status;
}
