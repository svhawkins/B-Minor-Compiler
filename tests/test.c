#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

/* this just runs all the test executables */

// TO DO: add pipes to keep track of TOTAL tests run and TOTAL tests passed

void parse_line(char* line, int start, int end, int* total_pass, int* total_test);
enum { READ = 0, WRITE, NUM_ENDS, MAX_BUFFER = 1024};
char output[MAX_BUFFER];
int main(int argc, const char* argv[]) {
  // keep track of total passes and tests
  int total_pass = 0, total_tests = 0;

  // executables list
  int n_exec = 3;
  char* path[] = {
    "tests/test_scan",
    "tests/test_parse",
    "tests/test_ast"
  };
  char* name[] = { "test_scan", "test_parse", "test_ast"};

  // what to print per pre-execution
  char* header = "TESTING: ";
  char* tests[] = {"SCANNER", "PARSER", "ABSTRACT SYNTAX TREE"};

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
  waitpid(0, NULL, 0);
  read(fd[READ], output, MAX_BUFFER);
  close(fd[READ]); close(fd[WRITE]);

  // print + process output
  char* line = strtok(output, "\n"); char* passed = "Passed: ";
  int n = strlen(passed);
  while(line) {
    // process output...
    if (!strncmp(header, line, strlen(header))) printf("\n");
    if (!strncmp(passed, line, n)) parse_line(line, n, strlen(line), &total_pass, &total_tests);
    printf("%s\n", line);
    line = strtok(NULL, "\n");
  }
  printf("\nTOTAL TESTS: %d\nTOTAL PASSED: %d/%d\n", total_tests, total_pass, total_tests);
  return 0;
}
void parse_line(char* line, int start, int end, int* total_pass, int* total_test) {
  char number[3]; int i, j; int n;
  for (i = start; i < end; i++) {
    for (j = 0; isdigit(line[i + j]); j++) number[j] = line[i + j];
    i += j; n = atoi(number);
    if (line[i] == '/') { *(total_pass) += n; } else { *(total_test) += n; }
  }
}
