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
  int i = 0;
  do {
    printf("%d: ", i + 1);
    if (yyparse() == 0) {
      printf("[Successful parse!]\n");
    } else {
      if (eof) { printf("EOF detected. Parsing complete.\n"); break; }
      else { printf("[Parse failure.]\n"); print_error_message(); }
    }
    i++;
  } while (i < 100);
  return 0;
}
