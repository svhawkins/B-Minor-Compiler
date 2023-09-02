#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

// for debugging purposes
int yydebug = 0;

extern int yyparse();
extern unsigned char eof;
extern void print_error_message();

void get_options(int argc, const char* argv[]);
int main(int argc, const char* argv[]) {
  get_options(argc, argv);
  for (int i = 0; !eof; i++) {
    printf("%d: ", i);
    if (yyparse() == 0) { printf("[Successful parse!]\n"); }
    else { printf("[Parse failure.]\n"); print_error_message(); }
  }
  return 0;
}

void get_options(int argc, const char* argv[]) {
  struct option long_opts [] = {
    {"toggle", no_argument, NULL, 't'},
    {"help", no_argument, NULL, 'h'},
    {0, 0, 0, 0}
  };
  int c, option_index;
  while(1) {
    c = getopt_long(argc, argv, "th", long_opts, &option_index);
    if (c == -1) { break; }
    switch(c) {
      case 't': yydebug = 1; break;
      case 'h': exit(0); // TO DO: print help stuff, return instead.
      case '?': default: exit(-1); // TO DO: invalid
    }
  }
}

