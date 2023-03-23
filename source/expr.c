#include <stdio.h>
#include <stdlib.h>
#include "expr.h"


struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right ) { return NULL; }

struct expr * expr_create_name( const char *n ) { return NULL; }
struct expr * expr_create_integer_literal( int c ) { return NULL; }
struct expr * expr_create_boolean_literal( int c ) { return NULL; }
struct expr * expr_create_char_literal( char c ) { return NULL; }
struct expr * expr_create_string_literal( const char *str ) { return NULL; }

void expr_print( struct expr *e ) {}
