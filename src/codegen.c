#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "symbol_table.h"

extern int yyparse();
extern unsigned char eof;
extern void print_error_message();
extern FILE *REG_ERR_OUT, *ERR_OUT, *CODEGEN_OUT;

// parser outputs
extern struct stmt* test_parser_result;
extern struct decl* parser_result;

// other outputs
extern struct decl* decl_hidden_list = NULL;
extern struct decl* decl_hidden_list_tail = NULL;

char* out_filename = "output.s";

void get_options(int argc, const char* argv[]);
bool setup_genfile(void);
int main(int argc, const char** argv) {
  ERR_OUT = REG_ERR_OUT = stderr;
  get_options(argc, argv);
  bool infile = setup_genfile();

  // set up symbol table
  Symbol_table* st = symbol_table_create(); symbol_table_scope_enter(st);

  // what is generated is dependent upon parser result

  printf("Hello world!\n");
  symbol_table_destroy(&st);
  if (infile) { fclose(CODEGEN_OUT); CODEGEN_OUT = NULL; }
  return 0;
}

void get_options(int argc, const char* argv[]) {
  struct option long_opts [] = {
    {"help", no_argument, NULL, 'h'},
    {"output_file", no_argument, NULL, 'o'},
    {0, 0, 0, 0}
  };
  int c, option_index;
  while(1) {
    c = getopt_long(argc, argv, "ho:", long_opts, &option_index);
    if (c == -1) { break; }
    switch(c) {
      case 'o': strcpy(out_filename, optarg); break;
      case 'h': exit(0); // TO DO: print help stuff, return instead.
      case '?': default: exit(-1); // TO DO: invalid
    }
  }
}

bool setup_genfile(void) {
  CODEGEN_OUT = stdout;
  bool infile = false;
  if (strcmp(out_filename, "stdout") != 0) {
    CODEGEN_OUT = fopen(out_filename, "w");
    if (!CODEGEN_OUT) { printf("Failed to open file. Exiting...\n"); exit(-1); }
    infile = true;
  }
  return infile;
}