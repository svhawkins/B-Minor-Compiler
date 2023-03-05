#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "parser.h" // TO DO: move to_str code elsewhere (perhaps just here), n_tokens to here, remove token.c
#define YYLMAX 256

extern FILE* yyin;
extern int yylex();
extern char* yytext;
extern int yylineno;
extern int n_col;
extern char error_text[2 * YYLMAX];
extern char filename[YYLMAX];
extern bool has_match;

extern void add_col(bool is_newline);
void print_header(void);
token_t print_token(token_t kind);

/* values from token.h */
/* function to get string form of a token */
char token_name[64];
void token_to_str(token_t t);
int n_tokens;

typedef enum { DEFAULT = 0, COLUMN, ALL, VERBOSE } mode_t;
mode_t mode;
mode_t determine_mode(const char* flag);
int main(int argc, const char* argv[]) {
  switch(argc) {
    case 1: yyin = stdin; break;
    case 2:
      mode = determine_mode(argv[1]); yyin = stdin; strcpy(filename, "stdin");
      if (mode == DEFAULT) { yyin = fopen(argv[1], "r"); strcpy(filename, argv[1]); }
      break;
    case 3:
      mode = determine_mode(argv[1]); yyin = fopen(argv[2], "r"); strcpy(filename, argv[2]);
      break;
  } if (!yyin) { fprintf(stderr, "Failed to open source file. Unable to proceed.\n"); return -1; }

  n_tokens = 0;
  n_col = 1;

  print_header();
  while (1) {
    token_t t = yylex();
    if (!t || t == TOKEN_EOF) break;
    print_token(t);
  } fclose(yyin);
  if (!has_match) { printf("%s\n", error_text); }
  printf("Total tokens: %d\n", n_tokens);
  return 0;
}
mode_t determine_mode(const char* flag) {
  mode_t m = DEFAULT;
  if (strcmp(flag, "-c") == 0) m = COLUMN;
  else if (strcmp(flag, "-a") == 0) m = ALL;
  else if (strcmp(flag, "-v") == 0) m = VERBOSE;
  return m;
}
void print_header(void) {
  char str [128];

  switch(mode) {
    case DEFAULT: case ALL:
      sprintf(str, "%-5s%15s%20s%8s\n", "TOKEN", "KIND", "LINE_NO", "NAME");
      break;
    case COLUMN: case VERBOSE:
      sprintf(str, "%-5s%15s%20s%10s%8s\n", "TOKEN", "KIND", "LINE_NO", "COL_NO", "NAME");
      break;
  }
  printf("%s", str); for (int i = 0; str[i] != '\0'; i++) { printf("-"); } printf("\n");
}
token_t print_token(token_t kind) {
  ++n_tokens;

  /* same regardless of mode */
  token_to_str(kind);
  printf("%-5.d%20s%10.d", n_tokens, token_name, yylineno);
  switch(mode) {
    case DEFAULT:
      if (kind >= TOKEN_IDENT && kind <= TOKEN_CH && kind != TOKEN_BOOL) {
        printf("%10c%-48s", ' ', yytext);
      } break;
    case COLUMN:
      printf("%10.d", n_col);
      if (kind >= TOKEN_IDENT && kind <= TOKEN_CH && kind != TOKEN_BOOL) {
        printf("%10c%-48s", ' ', yytext);
      } break;
    case ALL:
      printf("%10c%-48s", ' ', yytext);
      break;
    case VERBOSE:
      printf("%10.d%10c%-48s", n_col, ' ', yytext);
  } printf("\n");
  if (kind == TOKEN_ERROR) printf("%s\n", error_text);
  add_col(false);
  return kind;
}

void token_to_str(token_t t) {
  char prefix[64] = "TOKEN_";
  switch(t) {
    case TOKEN_ERROR: strcpy(token_name, "ERROR"); break;
    case TOKEN_EOF: strcpy(token_name, "EOF"); break;
    case TOKEN_ARRAY: strcpy(token_name, "ARRAY"); break;
    case TOKEN_AUTO: strcpy(token_name, "AUTO"); break;
    case TOKEN_BOOLEAN: strcpy(token_name,"BOOLEAN"); break;
    case TOKEN_CHAR: strcpy(token_name,"CHAR"); break;
    case TOKEN_ELSE: strcpy(token_name,"ELSE"); break;
    case TOKEN_FOR: strcpy(token_name,"FOR"); break;
    case TOKEN_FUNCTION: strcpy(token_name,"FUNCTION"); break;
    case TOKEN_IF: strcpy(token_name,"IF"); break;
    case TOKEN_INTEGER: strcpy(token_name,"INTEGER"); break;
    case TOKEN_PRINT: strcpy(token_name,"PRINT"); break;
    case TOKEN_RETURN: strcpy(token_name,"RETURN"); break;
    case TOKEN_STRING: strcpy(token_name,"STRING"); break;
    case TOKEN_VOID: strcpy(token_name,"VOID"); break;
    case TOKEN_WHILE: strcpy(token_name,"WHILE"); break;
    case TOKEN_IDENT: strcpy(token_name,"IDENT"); break;
    case TOKEN_BOOL: strcpy(token_name,"BOOL"); break;
    case TOKEN_NUMBER: strcpy(token_name,"NUMBER"); break;
    case TOKEN_STR: strcpy(token_name,"STR"); break;
    case TOKEN_CH: strcpy(token_name,"CH"); break;
    case TOKEN_INC: strcpy(token_name,"INC"); break;
    case TOKEN_DEC: strcpy(token_name,"DEC"); break;
    case TOKEN_SUB: strcpy(token_name,"SUB"); break;
    case TOKEN_EXP: strcpy(token_name,"EXP"); break;
    case TOKEN_NOT: strcpy(token_name,"NOT"); break;
    case TOKEN_MULT: strcpy(token_name,"MULT"); break;
    case TOKEN_DIV: strcpy(token_name,"DIV"); break;
    case TOKEN_MOD: strcpy(token_name,"MOD"); break;
    case TOKEN_ADD: strcpy(token_name,"ADD"); break;
    case TOKEN_LEQ: strcpy(token_name,"LEQ"); break;
    case TOKEN_LESS: strcpy(token_name,"LESS"); break;
    case TOKEN_GEQ: strcpy(token_name,"GEQ"); break;
    case TOKEN_GREAT: strcpy(token_name,"GREQ"); break;
    case TOKEN_EQ: strcpy(token_name,"EQ"); break;
    case TOKEN_NEQ: strcpy(token_name,"NEQ"); break;
    case TOKEN_AND: strcpy(token_name, "AND"); break;
    case TOKEN_OR: strcpy(token_name, "OR"); break;
    case TOKEN_ASSIGN: strcpy(token_name,"ASSIGN"); break;
    case TOKEN_LBRACK: strcpy(token_name,"LBRACK"); break;
    case TOKEN_RBRACK: strcpy(token_name,"RBRACK"); break;
    case TOKEN_LPAR: strcpy(token_name,"LPAR"); break;
    case TOKEN_RPAR: strcpy(token_name,"RPAR"); break;
    case TOKEN_LCURL: strcpy(token_name,"LCURL"); break;
    case TOKEN_RCURL: strcpy(token_name,"RCURL"); break;
    case TOKEN_COLON: strcpy(token_name,"COLON"); break;
    case TOKEN_SEMI: strcpy(token_name,"SEMI"); break;
    case TOKEN_COMMA: strcpy(token_name,"COMMA"); break;
  }
  strcat(prefix, token_name);
  strcpy(token_name, prefix);
}
