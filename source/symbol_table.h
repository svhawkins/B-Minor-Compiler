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

It also has the additional field of verbose, used to indicate if it should exit scopes.
This is used when printing out (complete) symbol tables (and for testing!)
*/

struct symbol_table {
  Stack* stack;
  bool verbose;
}; typedef struct symbol_table Symbol_table;


/*
Creates a symbol table, setting the verbose field to false.

Returns a NULL pointer upon any memory allocation failures.
*/
Symbol_table* symbol_table_create();

/*
Creates a symbol table, setting the verbose field to true.
Returns a NULL pointer upon any memory allocation failures.
*/
Symbol_table* symbol_table_verbose_create();

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
Returns 1 upon success, 0 upon failure.
Failure if:
	- NULL symbol table
	- NULL symbol table items array
	- NULL hash table within symbol table
	- empty symbol table
*/
int symbol_table_scope_bind(Symbol_table* st, const char* name, struct symbol* sym);

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

// show example output
*/
void symbol_table_fprint(FILE* fp, Symbol_table* st);
void symbol_table_print(Symbol_table* st);


/* name resolution functions */

/*
Adds symbols to the symbol table
Error messages:
*/
void symbol_table_decl_resolve(Symbol_table* st, struct decl* d);

/*
Looks up symbols in the symbol table.
Error messages:
*/
void symbol_table_expr_resolve(Symbol_table* st, struct expr* e);

/*
Looks up and adds symbols to table based on the statement kind given.
*/
void symbol_table_stmt_resolve(Symbol_table* st, struct stmt* s);

/*
Adds symbols with parameter scopes
Error messages:
*/
void symbol_table_type_resolve(Symbol_table* st, struct type* t);
void symbol_table_param_list_resolve(Symbol_table* st, struct param_list* p);
