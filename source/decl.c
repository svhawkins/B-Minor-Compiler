#include <stdio.h>
#include "decl.h"

struct decl * decl_create( char *name,
			   struct type *type,
			   struct expr *value,
			   struct stmt *code,
			   struct decl *next ) {

// FOR NOW STUB RETURN NULL
return NULL;

}
void decl_print( struct decl *d, int indent ) {}
