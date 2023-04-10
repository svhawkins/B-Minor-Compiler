#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../source/decl.h"
#include "../../source/expr.h"
#include "../../source/param_list.h"
#include "../../source/stmt.h"
#include "../../source/symbol.h"
#include "../../source/type.h"
#define MAX_BUFFER 256

extern FILE* yyin;
extern void yyrestart();
extern int yylineno;
extern int yyparse();
extern char error_text[MAX_BUFFER];
extern unsigned char eof;
typedef enum {FAILURE = 0, SUCCESS = 1} Status;
enum {PARSE_SUCCESS = 0, PARSE_FAILURE = 1};
FILE* ifp;
char test_type[MAX_BUFFER];
char lines[MAX_BUFFER][MAX_BUFFER];

int get_lines(char v[][MAX_BUFFER], char* filename); // stores file contents
Status file_error(char* test_type, char* filename); // error messsage when opening file
void print_error(char* test_type, int case_no, int expect, int actual); // error message for test failures
Status test_parse(int expect, int actual); // tests parse status

Status test_expressions(void); // tests B-Minor operators and expressions
Status test_print_statements(void); // tests print statements
Status test_jump_statements(void); // tests return statements
Status test_iteration_statements(void); // tests for and while loops
Status test_selection_statements(void); // tests if and if-else statements
Status test_declarations(void); // tests variable declarations
Status test_initializations(void); // tests variable declarations with initializations
Status test_functions(void); // tests function declarations and definitions
Status test_code(void); // tests a valid program
Status test_empty(void); // tests an empty program

int main(int argc, const char* argv[]) {
  Status (*tests[])(void) = {
    test_expressions,
    test_print_statements,
    test_jump_statements,
    test_iteration_statements,
    test_selection_statements,
    test_declarations,
    test_initializations,
    test_functions,
    test_code,
    test_empty
  };
  int n_tests = sizeof(tests)/sizeof(tests[0]);
  int n_pass = 0;

  printf("Running %d tests...\n", n_tests);
  for (int i = 0; i < n_tests; i++) { if (tests[i]()) { n_pass++; }}

  printf("Passed: %d/%d\n", n_pass, n_tests);
  printf("Failed: %d/%d\n", (n_tests - n_pass), n_tests);
  return 0;
}

int get_lines(char v[][MAX_BUFFER], char* filename) {
  ifp = fopen(filename, "r"); if (!ifp) { return 1; }
  char line[MAX_BUFFER]; int i;
  for (i = 0; fgets(line, MAX_BUFFER, ifp); i++) {
    strcpy(v[i], line);
  }
  fclose(ifp);
  return i + 1;
}
void print_error(char* test_type, int case_no, int expect, int actual) {
  char* expect_str = (expect == PARSE_SUCCESS) ? "PARSE_SUCCESS" : "PARSE_FAILURE";
  char* actual_str = (actual == PARSE_SUCCESS) ? "PARSE_SUCCESS" : "PARSE_FAILURE";

  printf("ERROR %s\n", test_type);
  printf("line %d: case %d\n", yylineno, case_no);
  printf("Expected result: %s, recieved result: %s\n\n", expect_str, actual_str);
}
Status test_parse(int expect, int actual) { return (expect == actual) ? SUCCESS : FAILURE; }

Status file_error(char* test_type, char* filename) {
  printf("Failed to open %s for %s.\n Test failure.\n", filename, test_type);
  return FAILURE;
}

Status test_expressions(void) {
  strcpy(test_type, "Testing: Expressions");
  char* filename = "./tests/parser/expression.bminor";
  int n = get_lines(lines, filename);
  Status status, overall_status = SUCCESS;
  yyin = fopen(filename, "r");
  if (!yyin) { return file_error(test_type, filename); }


  // have an array to hold expected values due to yyparse() continuing even after a failed parse.
  // there are more expected values than there are lines in the file.
  unsigned char expected[80] = { 1, 1, 1, 0, 1, 0, 0, 0, 1, 1,
				 1, 1, 1, 0, 0, 1, 1, 1, 0, 1,
				 1, 1, 0, 0, 0, 1, 1, 1, 0, 1,
				 1, 1, 0, 1, 1, 1, 0, 1, 1, 1,
				 0, 1, 1, 1, 0, 1, 1, 1, 0, 1,
				 0, 1, 1, 1, 0, 1, 0, 1, 1, 1,
				 1, 0, 0, 0, 1, 1, 1, 1, 1, 0,
				 1, 0, 0, 0, 0, 0, 1, 0, 2, 2
			       };
  int expect, actual; eof = 0;
  for(int i = 0; !eof; i++) {
    actual = yyparse();
    expect = (!expected[i]) ? PARSE_SUCCESS : PARSE_FAILURE;
    status = test_parse(expect, actual);
    if (status == FAILURE)  {
      print_error(test_type, i, expect, actual);
      overall_status = FAILURE;
    }
  }
  fclose(yyin);
  return overall_status;
}

Status test_print_statements(void) {
  strcpy(test_type, "Testing: Print statements");
  char* filename = "./tests/parser/print.bminor";
  int n = get_lines(lines, filename);
  // apparently tokens from prev file(s) were still in buffer
  // messing up parses
  yyrestart(yyin); //yyrewind()
  yyin = fopen(filename, "r");
  if (!yyin) { return file_error(test_type, filename); }
  Status status, overall_status = SUCCESS;

  unsigned char expected[30] = { 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
				 1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
				 0, 1, 1, 1, 1, 1, 1, 1, 0, 0
			       };

  int expect, actual; yylineno = 0; eof = 0;
  for(int i = 0; !eof; i++) {
    actual = yyparse();
    expect = (!expected[i]) ? PARSE_SUCCESS : PARSE_FAILURE;
    status = test_parse(expect, actual);
    if (status == FAILURE) {
      print_error(test_type, i, expect, actual);
      overall_status = FAILURE;
    }
  }
  fclose(yyin);
  return overall_status;
}


Status test_jump_statements(void) {
  strcpy(test_type, "Testing: Return statements");
  char* filename = "./tests/parser/jump.bminor";
  int n = get_lines(lines, filename);
  yyrestart(yyin);
  yyin = fopen(filename, "r");
  if (!yyin) { return file_error(test_type, filename); }
  Status status, overall_status = SUCCESS;

  unsigned char expected[30] = { 0, 0, 0, 1, 1, 0, 1, 1, 1, 1,
                                 1, 1, 0, 0, 1, 1, 1, 1, 1, 1,
                                 1, 1, 0, 0, 2, 2, 2, 2, 2, 2
                               };

  int expect, actual; yylineno = 0; eof = 0;
  for(int i = 0; !eof; i++) {
    actual = yyparse();
    expect = (!expected[i]) ? PARSE_SUCCESS : PARSE_FAILURE;
    status = test_parse(expect, actual);
    if (status == FAILURE) {
      print_error(test_type, i, expect, actual);
      overall_status = FAILURE;
    }
  }
  fclose(yyin);
  return overall_status;
}

Status test_iteration_statements(void) {
  strcpy(test_type, "Testing: For and While statements");
  char* filename = "./tests/parser/iteration.bminor";
  int n = get_lines(lines, filename);
  yyrestart(yyin);
  yyin = fopen(filename, "r");
  if (!yyin) { return file_error(test_type, filename); }
  Status status, overall_status = SUCCESS;

  unsigned char expected[50] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
				 1, 1, 1, 1, 1, 1, 1, 0, 0, 1,
				 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
				 1, 1, 1, 1, 1, 0, 1, 1, 0, 0
                               };

  int expect, actual; yylineno = 0; eof = 0;
  for(int i = 0; !eof; i++) {
    actual = yyparse();
    expect = (!expected[i]) ? PARSE_SUCCESS : PARSE_FAILURE;
    status = test_parse(expect, actual);
    if (status == FAILURE) {
      print_error(test_type, i, expect, actual);
      overall_status = FAILURE;
    }
  }
  fclose(yyin);
  return overall_status;
}

// Lone if statements are not tested
Status test_selection_statements(void) {
  strcpy(test_type, "Testing: If and Else statements");
  char* filename = "./tests/parser/selection.bminor";
  int n = get_lines(lines, filename);
  yyrestart(yyin);
  yyin = fopen(filename, "r");
  if (!yyin) { return file_error(test_type, filename); }
  Status status, overall_status = SUCCESS;

  unsigned char expected[60] = { 0, 1, 1, 1, 1, 0, 0, 1, 1, 1,
                                 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
                                 0, 1, 1, 1, 1, 0, 1, 1, 1, 1,
                                 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
				 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                               };

  int expect, actual; yylineno = 0;
  for(int i = 0; !eof; i++) {
    actual = yyparse();
    expect = (!expected[i]) ? PARSE_SUCCESS : PARSE_FAILURE;
    status = test_parse(expect, actual);
    if (status == FAILURE) {
      print_error(test_type, i, expect, actual);
      overall_status = FAILURE;
    }
  }
  fclose(yyin);
  return overall_status;
}


// declarations without initializations only
Status test_declarations(void) {
  strcpy(test_type, "Testing: Declarations");
  char* filename = "./tests/parser/declaration.bminor";
  int n = get_lines(lines, filename);
  yyrestart(yyin);
  yyin = fopen(filename, "r");
  if (!yyin) { return file_error(test_type, filename); }
  Status status, overall_status = SUCCESS;

  unsigned char expected[70] = { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
				 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				 1, 1, 1, 1, 1, 0, 1, 1, 1, 1,
				 1, 1, 1, 1, 1, 0, 0, 1, 1, 0,
				 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				 1, 1, 1, 0, 1, 1, 1, 1, 1, 1,
				 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
                               };
  int expect, actual; yylineno = 0; eof = 0;
  for(int i = 0; !eof; i++) {
    actual = yyparse();
    expect = (!expected[i]) ? PARSE_SUCCESS : PARSE_FAILURE;
    status = test_parse(expect, actual);
    if (status == FAILURE) {
      print_error(test_type, i, expect, actual);
      overall_status = FAILURE;
    }
  }
  fclose(yyin);
  return overall_status;
}


// includes declaration + initialization combos
Status test_initializations(void) {
  strcpy(test_type, "Testing: Initializations");
  char* filename = "./tests/parser/initialization.bminor";
  int n = get_lines(lines, filename);
  yyrestart(yyin);
  yyin = fopen(filename, "r");
  if (!yyin) { return file_error(test_type, filename); }
  Status status, overall_status = SUCCESS;

  unsigned char expected[30] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
				 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
                               };

  int expect, actual; yylineno = 0; eof = 0;
  for(int i = 0; !eof; i++) {
    actual = yyparse();
    expect = (!expected[i]) ? PARSE_SUCCESS : PARSE_FAILURE;
    status = test_parse(expect, actual);
    if (status == FAILURE) {
      print_error(test_type, i, expect, actual);
      overall_status = FAILURE;
    }
  }
  fclose(yyin);
  return overall_status;
}


// all about functions!
Status test_functions(void) {
  strcpy(test_type, "Testing: Function declarations/definitions");
  char* filename = "./tests/parser/function.bminor";
  int n = get_lines(lines, filename);
  yyrestart(yyin);
  yyin = fopen(filename, "r");
  if (!yyin) { return file_error(test_type, filename); }
  Status status, overall_status = SUCCESS;

  unsigned char expected[40] = { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
                                 0, 0, 0, 0, 0, 0, 1, 1, 1, 0,
                                 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
				 1, 0, 1, 1, 1, 1, 1, 1, 0, 0
                               };

  int expect, actual; yylineno = 0; eof = 0;
  for(int i = 0; !eof; i++) {
    actual = yyparse();
    expect = (!expected[i]) ? PARSE_SUCCESS : PARSE_FAILURE;
    status = test_parse(expect, actual);
    if (status == FAILURE) {
      print_error(test_type, i, expect, actual);
      overall_status = FAILURE;
    }
  }
  fclose(yyin);
  return overall_status;
}

// tests valid code
// there should be NO parse errors
Status test_code(void) {
  strcpy(test_type, "Testing: Programs");
  char* filename = "./tests/parser/code.bminor";
  int n = get_lines(lines, filename);
  yyrestart(yyin);
  yyin = fopen(filename, "r");
  if (!yyin) { return file_error(test_type, filename); }
  Status status, overall_status = SUCCESS;

  int expect, actual; yylineno = 0; eof = 0;
  eof = 0;
  for (int i = 0; !eof; i++) {
    actual = yyparse();
    expect = PARSE_SUCCESS;
    status = test_parse(expect, actual);
    if (status == FAILURE) {
      print_error(test_type, i, expect, actual);
      overall_status = FAILURE;
    }
  }
  fclose(yyin);
  return overall_status;
}

// tests empty program
Status test_empty(void) {
  strcpy(test_type, "Testing: Empty Program");
  char* filename = "./tests/parser/empty.bminor";
  int n = get_lines(lines, filename);
  yyrestart(yyin);
  yyin = fopen(filename, "r");
  if (!yyin) { return file_error(test_type, filename); }
  Status status, overall_status = SUCCESS;

  int expect, actual; yylineno = 0; eof = 0;
  for (int i = 0; !eof; i++) {
    actual = yyparse();
    expect = PARSE_SUCCESS;
    status = test_parse(expect, actual);
    if (status == FAILURE) {
      print_error(test_type, i, expect, actual);
      overall_status = FAILURE;
    }
  }
    fclose(yyin);
    return overall_status;
}

