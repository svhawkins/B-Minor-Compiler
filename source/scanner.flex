%{ 
 #include <stdio.h>
 #include <stdlib.h>
 #include <stdbool.h>
 #include <string.h>
 #include <limits.h>
 #include "parser.h"
 #define YYLMAX 256

 typedef enum {OVERFLOW, TOO_LONG, MULTICHAR, MISS_TERM, STRAY, EMPTY} error_t;

 void add_col(bool is_newline);				/* adds and/or resets n_col */
 token_t check_token(token_t kind);			/* checks for invalid pattern-matched tokens */
 token_t format_token(token_t kind);			/* properly formats string and character tokens */
 void error_message(error_t error, token_t kind);	/* formats an error message */

 char* lmax = "9223372036854775807"; /* LONG_MIN, LONG_MAX as strings */
 char* lmin = "-9223372036854775808";

 int n_col; /* to keep track of column numbers */
 bool has_match = true; /* to keep track of multiline comment matching */
 int stop_line, stop_col; /* to indicate exactly where the error ocurred scanning */
 
 char filename[YYLMAX];
 char error_text[2 * YYLMAX]; /* stores error messages */

%}
DIGIT	[[:digit:]]
FLOAT	[[:digit:]]+\.[[:digit:]]+|[[:digit:]]+\.|\.[[:digit:]]+
IDENT	[_[:alpha:]][_[:alnum:]]*
CH_CHAR	(\\[^'\n\0]|[^\n\0'\\])
CH_STR  (\\[^\n\0]|[^\n\0\"\\])
%x c_comment
%option yylineno noinput nounput nodefault
%%
"/*"			{ add_col(false); 
			  has_match = false;  stop_line = yylineno; stop_col = n_col;
                          BEGIN(c_comment);
			}
<c_comment>[^*\n]*	add_col(false);
<c_comment>"*"+[^*/\n]	add_col(false);
<c_comment>\n		add_col(true);
<c_comment>"*"+"/"	{ add_col(0); has_match = true; BEGIN(INITIAL); }

"//"[^\n]*		add_col(false);

(" "|\t)		add_col(false); BEGIN(INITIAL); /* skip whitespace */
\n			add_col(true); BEGIN(INITIAL);

array			{ return check_token(TOKEN_ARRAY); }
auto			{ return check_token(TOKEN_AUTO); }
boolean			{ return check_token(TOKEN_BOOLEAN); }
char			{ return check_token(TOKEN_CHAR); }
else			{ return check_token(TOKEN_ELSE); }
for			{ return check_token(TOKEN_FOR); }
function		{ return check_token(TOKEN_FUNCTION); }
if			{ return check_token(TOKEN_IF); }
integer			{ return check_token(TOKEN_INTEGER); }
print			{ return check_token(TOKEN_PRINT); }
return			{ return check_token(TOKEN_RETURN); }
string			{ return check_token(TOKEN_STRING); }
void			{ return check_token(TOKEN_VOID); }
while			{ return check_token(TOKEN_WHILE); }

true|false		{ return check_token(TOKEN_BOOL); }
[+-]?{DIGIT}+		{ return check_token(TOKEN_NUMBER); }

"'"{CH_CHAR}"'"		{ return format_token(check_token(TOKEN_CH)); }
"'"{CH_CHAR}[\"]?       { /* missing terminating ' error */
			  error_message(MISS_TERM, TOKEN_CH);
			  return TOKEN_ERROR;
			}
"'"{CH_CHAR}{2,}"'"	{ /* multicharacter character error */
			  error_message(MULTICHAR, TOKEN_CH);
			  return TOKEN_ERROR;
			}
"''"			{ /* empty character error */
			  error_message(EMPTY, TOKEN_CH);
			  return TOKEN_ERROR;
			}
"\""{CH_STR}*"\""	{ return format_token(check_token(TOKEN_STR)); }
"\""{CH_STR}*[\"']?	{ /* misssing terminating " error */
			  error_message(MISS_TERM, TOKEN_STR);
			  return TOKEN_ERROR;
			}

\+{2}			{ return check_token(TOKEN_INC); }
-{2}			{ return check_token(TOKEN_DEC); }
-			{ return check_token(TOKEN_SUB); }
\^			{ return check_token(TOKEN_EXP); }
!			{ return check_token(TOKEN_NOT); }
\*			{ return check_token(TOKEN_MULT); }
\/			{ return check_token(TOKEN_DIV); }
"\%"			{ return check_token(TOKEN_MOD); }
\+			{ return check_token(TOKEN_ADD); }
"<="			{ return check_token(TOKEN_LEQ); }
"<"			{ return check_token(TOKEN_LESS); }
">="			{ return check_token(TOKEN_GEQ); }
">"			{ return check_token(TOKEN_GREAT); }
==			{ return check_token(TOKEN_EQ); }
!=			{ return check_token(TOKEN_NEQ); }
&&			{ return check_token(TOKEN_AND); }
"||"			{ return check_token(TOKEN_OR); }
=			{ return check_token(TOKEN_ASSIGN); }

{IDENT}			{ return check_token(TOKEN_IDENT); }

\[			{ return check_token(TOKEN_LBRACK); }
\]			{ return check_token(TOKEN_RBRACK); }
\(			{ return check_token(TOKEN_LPAR); }
\)			{ return check_token(TOKEN_RPAR); }
"{"			{ return check_token(TOKEN_LCURL); }
"}"			{ return check_token(TOKEN_RCURL); }
:			{ return check_token(TOKEN_COLON); }
;			{ return check_token(TOKEN_SEMI); }
,			{ return check_token(TOKEN_COMMA); }
<<EOF>>			{ return TOKEN_EOF; }
.			{ 
			  error_message(STRAY, TOKEN_ERROR);
			  return TOKEN_ERROR;
			}
%%

int yywrap() { 
  if (!has_match) error_message(MISS_TERM, TOKEN_EOF);
  return 1;
}
void add_col(bool is_newline) { n_col += yyleng; if (is_newline) n_col = 1; }

/* formats error messages into a single string */
void error_message(error_t error, token_t kind) {
  /* save where the error occurred: file, line, column */
  sprintf(error_text, "\t%s:%d:%d: error\n",
          filename, (kind != TOKEN_EOF) ? yylineno : stop_line, (kind != TOKEN_EOF) ? n_col : stop_col);
  char msg[2 * YYLMAX];
  char term[3]; strcpy(term, "*/");
  switch(error) {
    case OVERFLOW: sprintf(msg, "\tInteger overflow with token: %s", yytext); break;
    case MULTICHAR: sprintf(msg, "\tMultiple characters within character: %s", yytext); break;
    case TOO_LONG: sprintf(msg, "\tToken %s too long. Must be less than %d characters", yytext, YYLMAX); break;
    case STRAY:	sprintf(msg, "\tStray %s token", yytext); break;
    case EMPTY: sprintf(msg, "\tEmpty character %s. Characters cannot be empty", yytext); break;
    case MISS_TERM: 
      switch(kind) { case TOKEN_STR: strcpy(term, "\""); break; case TOKEN_CH: strcpy(term, "'"); break; }
      if (kind == TOKEN_EOF) { sprintf(msg, "\tMissing terminating %s for comment", term); }
      else { sprintf(msg, "\tMissing terminating %s for token %s", term, yytext); }
      break;
  }
  strcat(error_text, msg);
}
token_t check_token(token_t kind) {
  /* checks if a token is too long */
  if (yyleng > YYLMAX) {
    kind = TOKEN_ERROR;
    error_message(TOO_LONG, TOKEN_ERROR);
  }

  /* checks if a numeric token has too large a value */
  if (kind == TOKEN_NUMBER) {
    char buffer[21]; /* 1 for -, 19 for LONG_MAX digits, 1 for \0 */
    long l = atol(yytext); sprintf(buffer, "%ld", l);

    /* if a value was truncated, it was out of range */
    if (strcmp(buffer, yytext) != 0 &&
       (l == LONG_MIN || l == LONG_MAX)) {
      kind = TOKEN_ERROR;
      error_message(OVERFLOW, TOKEN_NUMBER);
    }
  }
  return kind;
}
token_t format_token(token_t kind) {
  /* 
   * copies all contents of yytext within quotation marks,
   * making all escaped characters be their proper forms, appends \0 if need be 
   */
  if (kind == TOKEN_STR || kind == TOKEN_CH) {
    char buffer[yyleng];
    bool has_null = false, is_escape = false; int i, j; char ch;
    for (i = 1, j = 0; !has_null && i < yyleng - 1; i++) {
      if (!is_escape && yytext[i] == '\\') { is_escape = true; continue; }
      ch = yytext[i];
      if (is_escape) {
        switch(yytext[i]) {
          case '0': ch = '\0'; has_null = true; break;
          case 'n': ch = '\n'; break;
        }
        buffer[j] = ch; is_escape = false; j++;
      } else { buffer[j] = ch; j++; }
    }
    if (!has_null) { buffer[j] = '\0'; }
    strcpy(yytext, buffer); yyleng = j + 1;
  }
  return kind;
}
