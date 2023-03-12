#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_BUFFER 256

extern FILE* yyin;
extern void yyrestart();
extern int yylineno;
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
Status test_print_statements(void);
Status test_jump_statements(void);
Status test_iteration_statements(void);
Status test_selection_statements(void);
Status test_declarations(void);
Status test_initializations(void);
Status test_functions(void);
Status test_code(void);
Status test_empty(void);

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

void print_error(char* test_type, char* line, int lineno, int expect, int actual) {
  char* expect_str = (expect == PARSE_SUCCESS) ? "PARSE_SUCCESS" : "PARSE_FAILURE";
  char* actual_str = (actual == PARSE_SUCCESS) ? "PARSE_SUCCESS" : "PARSE_FAILURE";

  printf("ERROR %s\n", test_type);
  printf("%d : %s", lineno, line);
  printf("Expected result: %s, recieved result: %s\n\n", expect_str, actual_str);
}
Status test_parse(int expect, int actual) { return (expect == actual) ? SUCCESS : FAILURE; }

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


  // have an array to hold expected values due to yyparse() continuing even after a failed parse.
  // there are more expected values than there are lines in the file.
  // since n_parse starts at 1, [0] index is skipped.
  unsigned char expected[100] = { 1, 1, 1, 0, 1, 0, 0, 0, 1, 1,
				  1, 1, 1, 0, 0, 1, 1, 1, 0, 1,
				  1, 1, 0, 0, 0, 1, 1, 1, 0, 1,

				  1, 1, 0, 1, 1, 1, 0, 1, 1, 1,
				  0, 1, 1, 1, 0, 1, 1, 1, 0, 1,
				  1, 1, 1, 0, 1, 0, 1, 1, 1, 0,

				  1, 0, 1, 1, 1, 1, 0, 0, 0, 1,
				  1, 1, 0, 1, 1, 0, 1, 0, 1, 1,
				  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

				  1, 1, 1, 1, 1, 1, 1, 1, 1, 1
		            	 };
  yylineno = 0;
  char line[MAX_BUFFER];
  for(int i = 0; fgets(line, MAX_BUFFER, ifp); i++) {
    actual = yyparse();
    expect = (!expected[i]) ? PARSE_SUCCESS : PARSE_FAILURE;
    status = test_parse(expect, actual);
    if (status == FAILURE)  {
      print_error(test_type, line, yylineno, expect, actual);
      overall_status = FAILURE;
    }
  }
  fclose(yyin); fclose(ifp);
  return overall_status;
}

Status test_print_statements(void) {
  strcpy(test_type, "Testing: Print statements");
  char* filename = "./tests/parser/print.bminor";
  // apparently tokens from prev file(s) were still in buffer
  // messing up parses
  yyrestart(yyin); //yyrewind()
  yyin = fopen(filename, "r"); ifp = fopen(filename, "r");
  if (!(yyin && ifp)) { return file_error(test_type, filename); }
  Status status, overall_status = SUCCESS;

  unsigned char expected[30] = { 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
				 1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
				 0, 1, 1, 1, 1, 1, 1, 1, 0, 1
			       };

  int expect, actual; yylineno = 0;
  char line[MAX_BUFFER];
  for(int i = 0; fgets(line, MAX_BUFFER, ifp); i++) {
    actual = yyparse();
    expect = (!expected[i]) ? PARSE_SUCCESS : PARSE_FAILURE;
    status = test_parse(expect, actual);
    if (status == FAILURE) {
      print_error(test_type, line, yylineno, expect, actual);
      overall_status = FAILURE;
    }
  }
  fclose(yyin); fclose(ifp);
  return overall_status;
}


Status test_jump_statements(void) {
  strcpy(test_type, "Testing: Return statements");
  char* filename = "./tests/parser/jump.bminor";
  yyrestart(yyin);
  yyin = fopen(filename, "r"); ifp = fopen(filename, "r");
  if (!(yyin && ifp)) { return file_error(test_type, filename); }
  Status status, overall_status = SUCCESS;

  unsigned char expected[30] = { 0, 0, 0, 1, 1, 0, 1, 1, 1, 1,
                                 1, 1, 0, 0, 1, 1, 1, 1, 1, 1,
                                 1, 1, 1, 0, 1, 1, 1, 1, 1, 1
                               };

  int expect, actual; yylineno = 0;
  char line[MAX_BUFFER];
  for(int i = 0; fgets(line, MAX_BUFFER, ifp); i++) {
    actual = yyparse();
    expect = (!expected[i]) ? PARSE_SUCCESS : PARSE_FAILURE;
    status = test_parse(expect, actual);
    if (status == FAILURE) {
      print_error(test_type, line, yylineno, expect, actual);
      overall_status = FAILURE;
    }
  }
  fclose(yyin); fclose(ifp);
  return overall_status;
}


// TO DO: add tests for declarations within a for-loop
Status test_iteration_statements(void) {
  strcpy(test_type, "Testing: For and While statements");
  char* filename = "./tests/parser/iteration.bminor";
  yyrestart(yyin);
  yyin = fopen(filename, "r"); ifp = fopen(filename, "r");
  if (!(yyin && ifp)) { return file_error(test_type, filename); }
  Status status, overall_status = SUCCESS;

  unsigned char expected[50] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
				 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
				 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				 0, 1, 1, 1, 1, 1, 1, 0, 1, 1,
				 0, 1, 1, 1, 1, 1, 1, 1, 1, 1
                               };

  int expect, actual; yylineno = 0;
  char line[MAX_BUFFER];
  for(int i = 0; fgets(line, MAX_BUFFER, ifp); i++) {
    actual = yyparse();
    expect = (!expected[i]) ? PARSE_SUCCESS : PARSE_FAILURE;
    status = test_parse(expect, actual);
    if (status == FAILURE) {
      print_error(test_type, line, yylineno, expect, actual);
      overall_status = FAILURE;
    }
  }
  fclose(yyin); fclose(ifp);
  return overall_status;
}

// TO DO: test lone if statements WITHOUT else
// due to a shift-reduce conflict
Status test_selection_statements(void) {
  strcpy(test_type, "Testing: If and Else statements");
  char* filename = "./tests/parser/selection.bminor";
  yyrestart(yyin);
  yyin = fopen(filename, "r"); ifp = fopen(filename, "r");
  if (!(yyin && ifp)) { return file_error(test_type, filename); }
  Status status, overall_status = SUCCESS;

  unsigned char expected[50] = { 0, 1, 0, 0, 0, 1, 0, 0, 1, 0,
                                 1, 0, 0, 0, 0, 0, 0, 0, 1, 0,
                                 0, 1, 0, 1, 1, 0, 1, 1, 1, 1,
                                 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
				 0, 1, 1, 1, 1, 1, 1, 1, 1, 1
                               };

  int expect, actual; yylineno = 0;
  char line[MAX_BUFFER];
  for(int i = 0; fgets(line, MAX_BUFFER, ifp); i++) {
    actual = yyparse();
    expect = (!expected[i]) ? PARSE_SUCCESS : PARSE_FAILURE;
    status = test_parse(expect, actual);
    if (status == FAILURE) {
      print_error(test_type, line, yylineno, expect, actual);
      overall_status = FAILURE;
    }
  }
  fclose(yyin); fclose(ifp);
  return overall_status;
}


// declarations without initializations only
Status test_declarations(void) {
  strcpy(test_type, "Testing: Declarations");
  char* filename = "./tests/parser/declaration.bminor";
  yyrestart(yyin);
  yyin = fopen(filename, "r"); ifp = fopen(filename, "r");
  if (!(yyin && ifp)) { return file_error(test_type, filename); }
  Status status, overall_status = SUCCESS;

  unsigned char expected[70] = { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
				 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				 1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
				 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
				 1, 0, 1, 1, 1, 1, 1, 1, 1, 1,
				 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
                               };

  int expect, actual; yylineno = 0;
  char line[MAX_BUFFER];
  for(int i = 0; fgets(line, MAX_BUFFER, ifp); i++) {
    actual = yyparse();
    expect = (!expected[i]) ? PARSE_SUCCESS : PARSE_FAILURE;
    status = test_parse(expect, actual);
    if (status == FAILURE) {
      print_error(test_type, line, yylineno, expect, actual);
      overall_status = FAILURE;
    }
  }
  fclose(yyin); fclose(ifp);
  return overall_status;
}


// includes declaration + initialization combos
Status test_initializations(void) {
  strcpy(test_type, "Testing: Initializations");
  char* filename = "./tests/parser/initialization.bminor";
  yyrestart(yyin);
  yyin = fopen(filename, "r"); ifp = fopen(filename, "r");
  if (!(yyin && ifp)) { return file_error(test_type, filename); }
  Status status, overall_status = SUCCESS;

  unsigned char expected[30] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
				 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
                               };

  int expect, actual; yylineno = 0;
  char line[MAX_BUFFER];
  for(int i = 0; fgets(line, MAX_BUFFER, ifp); i++) {
    actual = yyparse();
    expect = (!expected[i]) ? PARSE_SUCCESS : PARSE_FAILURE;
    status = test_parse(expect, actual);
    if (status == FAILURE) {
      print_error(test_type, line, yylineno, expect, actual);
      overall_status = FAILURE;
    }
  }
  fclose(yyin); fclose(ifp);
  return overall_status;
}


// all about functions!
Status test_functions(void) {
  strcpy(test_type, "Testing: Function declarations/definitions");
  char* filename = "./tests/parser/function.bminor";
  yyrestart(yyin);
  yyin = fopen(filename, "r"); ifp = fopen(filename, "r");
  if (!(yyin && ifp)) { return file_error(test_type, filename); }
  Status status, overall_status = SUCCESS;

  unsigned char expected[30] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
                                 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
                               };

  int expect, actual; yylineno = 0;
  char line[MAX_BUFFER];
  for(int i = 0; fgets(line, MAX_BUFFER, ifp); i++) {
    actual = yyparse();
    expect = (!expected[i]) ? PARSE_SUCCESS : PARSE_FAILURE;
    status = test_parse(expect, actual);
    if (status == FAILURE) {
      print_error(test_type, line, yylineno, expect, actual);
      overall_status = FAILURE;
    }
  }
  fclose(yyin); fclose(ifp);
  return overall_status;
}

// tests valid code
// there should be NO parse errors
Status test_code(void) {
  strcpy(test_type, "Testing: Programs");
  char* filename = "./tests/parser/code.bminor";
  yyrestart(yyin);
  yyin = fopen(filename, "r"); ifp = fopen(filename, "r");
  if (!(yyin && ifp)) { return file_error(test_type, filename); }
  Status status, overall_status = SUCCESS;
  int expect, actual; yylineno = 0;
  char line[MAX_BUFFER];
  for(int i = 0; fgets(line, MAX_BUFFER, ifp); i++) {
    actual = yyparse();
    expect = PARSE_SUCCESS;
    status = test_parse(expect, actual);
    if (status == FAILURE) {
      print_error(test_type, line, yylineno, expect, actual);
      overall_status = FAILURE;
    }
  }
  fclose(yyin); fclose(ifp);
  return overall_status;
}

// tests empty program
Status test_empty(void) {
  strcpy(test_type, "Testing: Empty Program");
  char* filename = "./tests/parser/empty.bminor";
  yyrestart(yyin);
  yyin = fopen(filename, "r"); ifp = fopen(filename, "r");
  if (!(yyin && ifp)) { return file_error(test_type, filename); }
  Status status, overall_status = SUCCESS;
  int expect, actual; yylineno = 0;
  char line[MAX_BUFFER];
  actual = yyparse();
  expect = PARSE_SUCCESS;
  status = test_parse(expect, actual);
  if (status == FAILURE) {
    print_error(test_type, line, yylineno, expect, actual);
    overall_status = FAILURE;
  }
  fclose(yyin); fclose(ifp);
  return overall_status;
}
