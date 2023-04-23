#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../source/symbol_table.h"
#define MAX_BUFFER 256

typedef enum { FAILURE = 0, SUCCESS = 1 } Status;
char test_type[MAX_BUFFER];
char output[MAX_BUFFER];

Status test_hello(void);
Status test_goodbye(void);

int main(void) {
  Status (*tests[])(void) = {
    test_hello,
    test_goodbye
  };

  int n_tests = sizeof(tests)/sizeof(tests[0]);
  int n_pass = 0;
  printf("Running %d tests...\n", n_tests);
  for (int i = 0; i < n_tests; i++) { if (tests[i]()) { n_pass++; }}

  printf("Passed: %d/%d\n", n_pass, n_tests);
  printf("Failed: %d/%d\n", (n_tests - n_pass), n_tests);
  return 0;
}

Status test_hello(void) { printf("Hello world!:)\n"); return SUCCESS; }
Status test_goodbye(void) { printf("Goodbye world!:(\n"); return FAILURE; }
