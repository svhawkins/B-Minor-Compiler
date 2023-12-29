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

char* out_filename[MAX_LENGTH];
char* in_filename[MAX_LENGTH];
bool verbose = false;

void get_options(int argc, const char* argv[]);
void help(void);
bool setup_genfile(void);
int main(int argc, const char** argv) {

  // set up file I/O defaults
  strcpy(in_filename, "stdin");
  strcpy(out_filename, "stdout"); // TO DO: change to output.s eventually.
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

void help(void) {
  printf("codegen [options]: Code generator for B-Minor\n");
  printf("Options:\n");
  printf("--help (-h): print this help\n");
  printf("--output_file (-o <outfile>): specify the output file to be <outfile>\n");
  printf("--input_file (-i) <infile>: specify the input file to be <infile>\n");
  printf("--interactive (-I): input and output file descriptors are STDIN and STDOUT\n");
  printf("--verbose (-v): generated assembly code will have brief explanatory comments.\n");
}

void get_options(int argc, const char* argv[]) {
  struct option long_opts [] = {
    {"help", no_argument, NULL, 'h'},
    {"output_file", optional_argument, NULL, 'o'},
    {"input_file", optional_argument, NULL, 'i'},
    {"interactive", no_argument, NULL, 'I'},
    {"verbose", no_argument, NULL, '-v'},
    {0, 0, 0, 0}
  };
  int c, option_index;
  while(1) {
    c = getopt_long(argc, argv, "ho:i:Iv", long_opts, &option_index);
    if (c == -1) { break; }
    switch(c) {
      case 'o': strcpy(out_filename, optarg); break;
      case 'i': strcpy(in_filename, optarg); break;
      case 'I': strcpy(in_filename, "stdin"); strcpy(out_filename, "stdout"); break;
      case 'v': verbose = true; break;
      case 'h': help(); exit(0);
      case '?': default: fprintf(stderr, "invalid argument\n"); exit(-1);
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