#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "stack.h"
#include "hash_table.h"
#include "decl.h"
#include "expr.h"
#include "param_list.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"

/* header file for the symbol table : a stack of hash tables (void pointers actually but casting can be done) */


/*
Symbol table itself is a stack wrapper handling
the casts and allocations of its hash table elements,
and symbol elements of its element hash tables.

With that being said, the same kind of values will be returned under the same
conditions, such as failure cases.
*/
typedef Stack Symbol_table;


/*
Creates a symbol table.

Returns a NULL pointer upon any memory allocation failures.
*/
Symbol_table* symbol_table_create();


/*
Destroys a symbol table.
Sets st to NULL upon success.
Does nothing if NULL symbol table.
*/
void symbol_table_destroy(Symbol_table** st);

/*
Pushes a new hashtable to the stack
Does nothing if:
	- NULL symbol table
	- NULL symbol table items array
	- hash table fails to be created
*/
void symbol_table_scope_enter(Symbol_table* st);

/*
Removes topmost hash table from the stack
Also removes it from memory.
Does nothing if:
	- NULL symbol table
	- NULL symbol table items array
*/
void symbol_table_scope_exit(Symbol_table* st);

/*
Returns number of hashtables (scopes) currently on the stack
Returns -1 for:
	- NULL symbol table
	- NULL items array in symbol table
*/
int symbol_table_scope_level(Symbol_table* st);

/*
Adds <name, sym> as a key-value pair to the topmost hash table in the stack
Does nothing if:
	- NULL symbol table
	- NULL symbol table items array
	- NULL hash table within symbol table
	- empty symbol table
*/
void symbol_table_scope_bind(Symbol_table* st, const char* name, struct symbol* sym);

/*
Searches through the stack from top to bottom for <name>
Returns associated pointer value to key upon success, otherwise NULL.

Returns a NULL pointer in the following cases:
	- NULL symbol table
	- NULL symbol table items
	- NULL hash table within symbol table
	- empty symbol table
	- empty hash table
	- invalid key
*/
struct symbol* symbol_table_scope_lookup(Symbol_table* st, const char* name);

/*
Searches for <name> only in topmost scope
Returns a NULL pointer in the following cases:
        - NULL symbol table
        - NULL symbol table items
        - NULL hash table within symbol table
        - empty symbol table
        - empty hash table
        - invalid key
*/
struct symbol* symbol_table_scope_lookup_current(Symbol_table* st, const char* name);


/*
prints out the key value pairs of the names associated with the symbols for all hash tables within
*/
void symbol_table_fprint(FILE* fp, Symbol_table* st);
void symbol_table_print(Symbol_table* st);
