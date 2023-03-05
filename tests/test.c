#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/* this just runs all the test executables */

// TO DO: add pipes to keep track of TOTAL tests run and TOTAL tests passed
int main(int argc, const char* argv[]) {

  // executables list
  int n_exec = 2;
  char* path[] = {
    "tests/test_scan",
    "tests/test_parse"
  };
  char* name[] = { "test_scan", "test_parse"};

  // what to print per pre-execution
  char* header = "TESTING: ";
  char* tests[] = {"SCANNER", "PARSER"};

  // executions
  int pid = 0;
  for (int i = 0; i < n_exec; i++) {
    if (i > 0) waitpid(pid, NULL, 0);
    printf("\n");
    pid = fork();
    //printf("[%d] [%d] i=%d\n", getppid(), getpid(), i);
    if (pid < 0) { fprintf(stderr, "Fork failure. Cannot continue\n"); return -1; }
    if (pid == 0) {
      printf("%s%s\n", header, tests[i]);
      execlp(path[i], name[i], NULL);
    }
  }
   // debugging
  //printf("[%d] [%d] out of loop\n", getppid(), getpid());
  waitpid(0, NULL, 0);
  return 0;
}
