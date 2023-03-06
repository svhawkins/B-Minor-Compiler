#include <stdio.h>
#include <stdlib.h>


// for debugging purposes
// TO DO: have a command line option to toggle this
int yydebug = 0;
extern int yyparse();
extern char* text;
int main(int argc, const char* argv[]) {
  printf("%s\n", "Hello world! :(\n");

  int ret, i = 0;
  do {
    printf("%d: ", i);
    if (yyparse() == 0) { printf("[Successful parse!]\n"); }
    else { printf("[Parse failure]\n"); }
    i++;
  } while (i < 60);
  return 0;
}
