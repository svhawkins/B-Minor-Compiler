#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

/* this just runs all the test executables */

void parse_line(char* line, int start, int end, int* total_pass, int* total_test);
enum { READ = 0, WRITE, NUM_ENDS, MAX_BUFFER = 16384}; // absurd buffer amount i know
int main(void) {
  // keep track of total passes and tests
  int total_pass = 0, total_tests = 0;

  // executables list
  int n_exec = 9;
  char* path[] = {
    "tests/test_scan",
    "tests/test_parse",
    "tests/test_factory",
    "tests/test_print",
    "tests/test_ast",
    "tests/test_stack",
    "tests/test_symbol_table",
    "tests/test_typecheck",
    "tests/test_codegen"
  };
  char* name[] = {
       "test_scan",
		   "test_parse",
		   "test_factory",
		   "test_print",
		   "test_ast",
		   "test_stack",
       "test_symbol_table",
		   "test_typecheck",
		   "test_codegen"
		  };

  // what to print per pre-execution
  char* header = "TESTING: ";
  char* tests[] = {
        "SCANNER",
		    "PARSER",
  		  "FACTORY FUNCTIONS",
		    "PRINT FUNCTIONS",
		    "ABSTRACT SYNTAX TREE",
        "SYMBOL TABLE DATA STRUCTURES",
		    "SYMBOL TABLE",
		    "TYPECHECKING",
		    "CODE GENERATION"
		   };

  // executions, output is written to pipe
  int pid = 0; int fd[NUM_ENDS];
  if (pipe(fd) < 0) { return -493; }
  printf("\n");
  for (int i = 0; i < n_exec; i++) {
    if (i > 0) waitpid(pid, NULL, 0);
    pid = fork();
    if (pid < 0) { fprintf(stderr, "Fork failure. Cannot continue\n"); return -1; }
    if (pid == 0) {
      // child writes, parent reads
      dup2(fd[WRITE], WRITE); close(fd[READ]);
      printf("%s%s\n", header, tests[i]);
      execlp(path[i], name[i], NULL);
    }
  }

  waitpid(0, NULL, 0); char output[MAX_BUFFER];
  read(fd[READ], output, MAX_BUFFER);
  close(fd[READ]); close(fd[WRITE]);

  // print + process output
  char* line = strtok(output, "\n"); char* passed = "Passed: ";
  int n = strlen(passed);
  while(line) {
    if (!strncmp(header, line, strlen(header))) printf("\n");
    if (!strncmp(passed, line, n)) parse_line(line, n, strlen(line), &total_pass, &total_tests);
    printf("%s\n", line); line = strtok(NULL, "\n");
  }
  printf("\nTOTAL PASSED: %3d/%.d\n", total_pass, total_tests);
  printf("TOTAL FAILED: %3d/%.d\n", total_tests - total_pass, total_tests);
  return 0;
}

void parse_line(char* line, int start, int end, int* total_pass, int* total_test) {
  char number[3]; int i, j, n;
  for (i = start; i < end; i++) {
    for (j = 0; isdigit(line[i + j]); j++) number[j] = line[i + j];
    i += j; n = atoi(number);
    if (line[i] == '/') { *(total_pass) += n; } else { *(total_test) += n; }
  }
}
