#include <stdio.h>
extern int yyparse();
int main(int argc, const char* argv[]) {
  printf("%s\n", "Hello world! :(\n");
  if (yyparse() == 0) { printf("Successful parse!\n"); }
  else { printf("Parse failure\n"); }
  return 0;
}
