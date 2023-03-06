#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "../../source/parser.h"
#define MAX_BUFFER 256

extern FILE* yyin;
extern int yyparse();
extern char error_text[MAX_BUFFER];
typedef enum {FAILURE = 0, SUCCESS = 1} Status;
enum {PARSE_SUCCESS = 0, PARSE_FAILURE = 1};
char test_type[MAX_BUFFER];
FILE* ifp;

Status file_error(char* test_type, char* filename);
void print_error(char* test_type, char* line, int lineno, int expect, int actual);
Status test_parse(int expect, int actual);

Status test_hello(void);
Status test_goodbye(void);
Status test_expressions(void);

int main(int argc, const char* argv[]) {
  Status (*tests[])(void) = {
    test_hello,
    test_goodbye,
    test_expressions
  };
  int n_tests = sizeof(tests)/sizeof(tests[0]);
  int n_pass = 0;

  printf("Running %d tests...\n", 3);
  for (int i = 0; i < n_tests; i++) { if (tests[i]()) { n_pass++; }}

  printf("Passed: %d/%d\n", n_pass, n_tests);
  printf("Failed: %d/%d\n", (n_tests - n_pass), n_tests);
  return 0;
}

void print_error(char* test_type, char* line, int lineno, int expect, int actual) {
  char* expect_str = (expect == PARSE_SUCCESS) ? "PARSE_SUCCESS" : "PARSE_FAILURE";
  char* actual_str = (actual == PARSE_SUCCESS) ? "PARSE_SUCCESS" : "PARSE_FAILURE";

  printf("Error:\n");
  printf("%d : %s", lineno, line);
  printf("Expected result: %s, recieved result: %s\n\n", expect_str, actual_str);
}
Status test_parse(int expect, int actual) { return (expect == actual || strcmp(error_text, "syntax error") != 0) ? SUCCESS : FAILURE; }

Status file_error(char* test_type, char* filename) {
  printf("Failed to open %s for %s.\n Test failure.\n", filename, test_type);
  return FAILURE;
}
Status test_hello(void) {
  printf("Hello world! :)\n");
  return SUCCESS;
}

Status test_goodbye(void) {
  printf("Goodbye world! :(\n");
  return SUCCESS;
}

Status test_expressions(void) {
  strcpy(test_type, "Testing: Expressions");
  char* filename = "./tests/parser/expression.bminor";
  Status status, overall_status = SUCCESS;
  yyin = fopen(filename, "r"); ifp = fopen(filename, "r");
  if (!(yyin && ifp)) { return file_error(test_type, filename); }
  int expect, actual = -1;

  int lineno = 1;
  char line[MAX_BUFFER];
  while (fgets(line, MAX_BUFFER, ifp)) {
    actual = yyparse();
    expect = PARSE_SUCCESS;
    status = test_parse(expect, actual);
    if (status == FAILURE)  {
      print_error(test_type, line, lineno, expect, actual);
      overall_status = FAILURE;
    }
    lineno++;
  }
  fclose(yyin);
  return overall_status;
}
