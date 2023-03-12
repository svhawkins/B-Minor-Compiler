#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// for debugging purposes
int yydebug = 0;

extern int yyparse();
extern unsigned char eof;
extern void print_error_message();

int main(int argc, const char* argv[]) {
  printf("%s\n", "Hello world! :(\n");
  if (argc > 1 && !strcmp(argv[1], "-t")) { yydebug = 1; }
  for (int i = 0; !eof; i++) {
    printf("%d: ", i);
    if (yyparse() == 0) { printf("[Successful parse!]\n"); }
    else { printf("[Parse failure.]\n"); print_error_message(); }
  }
  return 0;
}
