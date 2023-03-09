#include <stdio.h>
#include <stdlib.h>


// for debugging purposes
// TO DO: have a command line option to toggle this
int yydebug = 0;
extern int yyparse();
int main(int argc, const char* argv[]) {
  printf("%s\n", "Hello world! :(\n");


  int i = 0;
  do {
    printf("%d: ", i + 1);
    if (yyparse() == 0) { printf("[Successful parse!]\n"); }
    else { printf("[Parse failure]\n"); }
    i++;
  } while (i < 10);
  return 0;
}
