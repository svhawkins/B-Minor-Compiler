#include <stdio.h>
#include <string.h>
#include "../../src/parser.h"
#define MAX_BUFFER 256

// TOKEN_EOF NOW HAS OPTIONAL STATE TO BE ALSO VALUE 0
// CONDITIONS WERE MODIFIED FOR THIS
// PARSER.H REPLACES TOKEN.H AND ADDITIONAL CODE MOVED ELSEWHERE

extern FILE* yyin;
extern int yylex();
extern char* yytext;
extern int yylineno;
extern int n_col;

typedef enum { FAILURE = 0, SUCCESS } Status;
char test_type[MAX_BUFFER];


/* used in all tests */
Status file_error(char* test_type, char* filename);
void print_error(char* buffer, token_t expect, token_t actual);
Status test_token(token_t expect, token_t actual);

/* individual tests */
Status test_keywords(void);
Status test_integers(void);
Status test_identifiers(void);
Status test_characters(void);
Status test_strings(void);
Status test_string_format(void);
Status test_code(void);

int main(void) {
  /* holding all the tests */
  Status (*tests[])(void) = {
    test_keywords,
    test_integers,
    test_identifiers,
    test_characters,
    test_strings,
    test_string_format,
    test_code
  };
  int n_tests = sizeof(tests)/sizeof(tests[0]);
  int n_pass = 0;

  printf("Running %d tests...\n", n_tests);
  for (int i = 0; i < n_tests; i++) { if (tests[i]()) { n_pass++; } }
  printf("Passed: %d/%d\n", n_pass, n_tests);
  printf("Failed: %d/%d\n", n_tests - n_pass, n_tests);
  return 0;
}

Status file_error(char* test_type, char* filename) {
  printf("Failed to open %s for %s.\n Test failure.\n", filename, test_type);
  return FAILURE;
}

void print_error(char* test_type, token_t expect, token_t actual) {
  printf("Error: %s\n", test_type);
  printf("\tToken %s\n", yytext);
  if (expect != actual) {
  printf("\tExpected type %d, Recieved type %d\n", expect, actual);
  } else { printf("Unexpected type %d\n", actual); }
}

Status test_token(token_t expect, token_t actual) { return (expect == actual) ? SUCCESS : FAILURE; }

Status test_eof(token_t t) {
  strcpy(test_type,"EOF Token Test");
  Status status = test_token(TOKEN_EOF, t) || test_token(0, t);
  if (!status) print_error(test_type, TOKEN_EOF, t);
  return status;
}

/* start of keywords at TOKEN_ARRAY (1), ends at TOKEN_COMMA (46) */
/* do all keywords return the correct token type? (excluding datatypes) */
Status test_keywords(void) {
  strcpy(test_type, "Testing: keywords, operators, and special symbols");
  char* filename = "./tests/scanner/keywords.bminor";
  Status status, overall_status = SUCCESS;
  yyin = fopen(filename, "r");
  if (!yyin) { return file_error(test_type, filename); }

  token_t t, expect; int i;
  for (i = TOKEN_ARRAY; i <= TOKEN_COMMA; i++) {
    if (i >= TOKEN_IDENT && i <= TOKEN_CH) continue;
    t = yylex(); status = test_token(i, t);
    if (status == FAILURE) { overall_status = FAILURE; print_error(test_type, i, t);}
  }
  while(1) {
    t = yylex();
    if (!t || t == TOKEN_EOF) break;
    expect = TOKEN_ERROR;
    if (strcmp(yytext, "true") == 0 || strcmp(yytext, "false") == 0) { expect = TOKEN_BOOL; }
    status = test_token(expect, t);
    if (status == FAILURE) { overall_status = FAILURE; print_error(test_type, expect, t); }
  }
  //status = test_eof(yylex()); fclose(yyin);
  return (overall_status && status);
}

/* tests integers, integer limits, leading zeroes, etc. */
Status test_integers(void) {
  strcpy(test_type, "Testing: Integers and their accepted values");
  char* filename = "./tests/scanner/integer.bminor";
  Status status, overall_status = SUCCESS;
  yyin = fopen(filename, "r");
  if (!yyin) { return file_error(test_type, filename); }

  token_t expect, t;
  while(1) {
    t = yylex(); if (!t || t == TOKEN_EOF) break;
    expect = TOKEN_NUMBER;
    if (strcmp(yytext, "-9223372036854775809") == 0 ||
        strcmp(yytext, "9223372036854775808") == 0 ||
	strcmp(yytext, ".") == 0) { expect = TOKEN_ERROR; }
    status = test_token(expect, t);
    if (!status) { overall_status = FAILURE; print_error(test_type, expect, t); }
  }
  status = test_eof(yylex()); fclose(yyin);
  return (overall_status && status);
}


/* tests for valid identifiers */
/* identifiers must consist only of alphanumeric characters or underscores with a length <= 256 */
Status test_identifiers(void) {
  strcpy(test_type, "Testing: Valid identifiers");
  char* filename = "./tests/scanner/identifier.bminor";
  Status status, overall_status = SUCCESS;
  yyin = fopen(filename, "r");
  if (!yyin) { return file_error(test_type, filename); }

  token_t expect, t;
  while(1) {
    t = yylex(); if (!t || t == TOKEN_EOF) break;
    expect = TOKEN_IDENT;
    if (strcmp(yytext, "493") == 0 ||
        strcmp(yytext, "12345") == 0 ||
	strcmp(yytext, "2") == 0 ||
	strcmp(yytext, "24") == 0 ||
	strcmp(yytext, "44") == 0 ||
	strcmp(yytext, "0") == 0 ||
        strcmp(yytext, "3") == 0) { expect = TOKEN_NUMBER; }
    else if (strcmp(yytext, "$") == 0 ||
             strcmp(yytext, "#") == 0 ||
             strcmp(yytext, "\\") == 0) { expect = TOKEN_ERROR; }
    else if (strcmp(yytext, "true") == 0 ||
             strcmp(yytext, "false") == 0) { expect = TOKEN_BOOL; }
    else if (strcmp(yytext, "while") == 0) { expect = TOKEN_WHILE; }
    status = test_token(expect, t);
    if (!status) { overall_status = FAILURE; print_error(test_type, expect, t); }
  }
  status = test_eof(yylex()); fclose(yyin);
  return (overall_status && status);
}

/* tests for valid characters */
/* characters must be of length 1 and cannot have invalid characters or literals such as newline or null terminator */
Status test_characters(void) {
  strcpy(test_type, "Testing: Accepting values of characters");
  char* filename = "./tests/scanner/character.bminor";
  Status status, overall_status = SUCCESS;
  yyin = fopen(filename, "r");
  if (!yyin) { return file_error(test_type, filename); }

  token_t expect, t;
  while(1) {
    t = yylex(); if (!t || t == TOKEN_EOF) break;
    expect = TOKEN_ERROR;
    if (strcmp(yytext, "x") == 0 ||
        strcmp(yytext, "\n") == 0 ||
	strcmp(yytext, "\0") == 0 ||
	strcmp(yytext, "%") == 0 ||
	strcmp(yytext, "A") == 0) { expect = TOKEN_CH; }
    else if (strcmp(yytext, "y") == 0 ||
	     strcmp(yytext, "z") == 0) { expect = TOKEN_IDENT; }
    status = test_token(expect, t);
    if (!status) { overall_status = FAILURE; print_error(test_type, expect, t); }
  }
  status = test_eof(yylex()); fclose(yyin);
  return (overall_status & status);
}

/* tests for valid strings */
/* strings must have matching quotation marks and cannot contain literal newline or null terminators */
Status test_strings(void) {
  strcpy(test_type, "Testing: Accepting values of strings");
  char* filename = "./tests/scanner/string.bminor";
  Status status, overall_status = SUCCESS;
  yyin = fopen(filename, "r");
  if (!yyin) { return file_error(test_type, filename); }

  token_t expect, t;
  while(1) {
    t = yylex(); if (!t || t == TOKEN_EOF) break;
    expect = TOKEN_STR;
    if (strcmp(yytext, "\"invalid") == 0 ||
        strcmp(yytext, "\"foo") == 0 ||
        strcmp(yytext, "\"foo'") == 0 ||
        strcmp(yytext, "\"\\\"") == 0 ||
        strcmp(yytext, "\"") == 0 ||
        strcmp(yytext, "\"x'") == 0 ||
        strcmp(yytext, "\"\\x'") == 0 ||
        strcmp(yytext, "\"012345678901234567890101234567890123456789010123456789012345678901012345678901234567890101234567890123456789010123456789012345678901012345678901234567890101234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\"") == 0 ||
        strcmp(yytext, "\"\\0'") == 0) { expect = TOKEN_ERROR; }
    else if (strcmp(yytext, "string") == 0) { expect = TOKEN_STRING; }
    status = test_token(expect, t);
    if (!status) { overall_status = FAILURE; print_error(test_type, expect, t); }
  }
  status = test_eof(yylex()); fclose(yyin);
  return(overall_status && status);
}

/* tests if strings and characters are being formatted correctly */
/* escape characters (sans n or 0) are converted into the character immediately following \ */
/* escape characters n, 0 are converted into literal newline and null terminator */
/* quotation marks are removed */
/* a null terminator is appended if not yet there */
Status test_string_format(void) {
  strcpy(test_type, "Testing: String and character formatting");
  char* filename = "./tests/scanner/string_format.bminor";
  Status status, overall_status = SUCCESS;
  yyin = fopen(filename, "r");
  if (!yyin) { return file_error(test_type, filename); }

  int copy_5 = 0, copy_groovy = 0, copy_p = 0;
  int expect_5 = 4, expect_groovy = 5, expect_p = 2;
  token_t t;
  while(1) {
    t = yylex(); if (!t || t == TOKEN_EOF) break;
    if (strcmp(yytext, "5") == 0) { copy_5++; }
    else if (strcmp(yytext, "groovy") == 0) { copy_groovy++; }
    else if (strcmp(yytext, "p") == 0) { copy_p++; }
    else if (strcmp(yytext, "n") == 0 || strcmp(yytext, "0") == 0) {
      overall_status = FAILURE; printf("Error: expected newline or null terminator, recieved: %s\n", yytext);
    }
  }
  status = test_eof(yylex()); fclose(yyin);
  if (copy_5 != expect_5) {
    overall_status = FAILURE;
    printf("Error: Expected %d of string %s, recieved %d\n", expect_5, "5", copy_5);
  }
  if (copy_groovy != expect_groovy) {
    overall_status = FAILURE;
    printf("Error: Expected %d of string %s, recieved %d\n", expect_groovy, "groovy", copy_groovy);
  }
  if (copy_p != expect_p) {
    overall_status = FAILURE;
    printf("Error: Expected %d of string %s, recieved %d\n", expect_p, "p", copy_p);
  }
  return (overall_status && status);
}

/* tests proper code. there are ONLY TWO instances where there should be
   an error. otherwise, there should be NO OTHER ERRORS WHATSOEVER */
Status test_code(void) {
  strcpy(test_type, "Testing: Code and many types of tokens at once");
  char* filename = "./tests/scanner/code.bminor";
  Status status, overall_status = SUCCESS;
  yyin = fopen(filename, "r");
  if (!yyin) { return file_error(test_type, filename); }

  token_t expect, t;
  while(1) {
    t = yylex(); if (!t || t == TOKEN_EOF) break;
    if (strcmp(yytext, "|") == 0 || strcmp(yytext, "&") == 0) {
      expect = TOKEN_ERROR;
      status = test_token(expect, t);
    } else if (t == TOKEN_ERROR) {
      overall_status = FAILURE;
      print_error(test_type, t, t);
    }
  }
  status = test_eof(yylex()); fclose(yyin);
  return (overall_status && status);
}
