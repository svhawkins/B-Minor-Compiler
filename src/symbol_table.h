#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

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

/* header file for the symbol table : a stack of hash tables (void pointers cast to hash tables) */


// for error handling for everything related
int global_error_count;
int error_status;
typedef enum { SYM_UNDEF = 1, SYM_REDEF, SYM_TYPE, SYM_PARAM } symbol_error_t;
int symbol_table_error_handle(symbol_error_t kind, void* ctx1, void* ctx2);

// file pointer for error output
FILE* ERR_OUT;

// for which assignment
int which_count;

/*
Symbol table itself is a stack wrapper handling
the casts and allocations of its hash table elements,
and symbol elements of its element hash tables.

It also has the additional field of verbose, used to indicate if it should exit scopes.
This is used when printing out (complete) symbol tables (and for testing!)

The top field is also used to indicate current scope via its index.
This ensures for proper scope lookups even with verbose enabled.

The show_hidden field is for printing purposes.
*/

struct symbol_table {
  Stack* stack;
  bool verbose;
  int top;
  bool show_hidden;
}; typedef struct symbol_table Symbol_table;


/*
Creates a symbol table, setting the verbose field to false.
Returns a NULL pointer upon any memory allocation failures.
*/
struct symbol_table* symbol_table_create();

/*
Creates a symbol table, setting the verbose field to true.
Returns a NULL pointer upon any memory allocation failures.
*/
struct symbol_table* symbol_table_verbose_create();

/*
Destroys a symbol table.
Sets st to NULL upon success.
Does nothing if NULL symbol table.
*/
void symbol_table_destroy(struct symbol_table** st);


/*
Destroys then creates a new symbol table
*/
struct symbol_table* symbol_table_clear(struct symbol_table* st);

/*
Pushes a new hashtable to the stack
Does nothing if:
	- NULL symbol table
	- NULL symbol table items array
	- hash table fails to be created
*/
void symbol_table_scope_enter(struct symbol_table* st);

/*
Removes topmost hash table from the stack
Does nothing if:
	- NULL symbol table
	- NULL symbol table items array
*/
void symbol_table_scope_exit(struct symbol_table* st);

/*
Returns number of hashtables (scopes) currently on the stack
Returns -1 for:
	- NULL symbol table
	- NULL items array in symbol table
*/
int symbol_table_scope_level(struct symbol_table* st);

/*
Adds <name, sym> as a key-value pair to the topmost hash table in the stack
*Exception: hidden symbols are always global
Returns 1 upon success, 0 upon failure.
Failure if:
	- NULL symbol table
	- NULL symbol table items array
	- NULL hash table within symbol table
	- empty symbol table
*/
int symbol_table_scope_bind(struct symbol_table* st, const char* name, struct symbol* sym);

/*
Searches for <name> only in topmost scope regardless of value in st->top
Returns a NULL pointer in the following cases:
        - out of bounds index value
        - NULL symbol table
        - NULL symbol table items
        - NULL hash table within symbol table
        - empty symbol table
        - empty hash table
        - invalid key
*/
struct symbol* symbol_table_scope_lookup_at(struct symbol_table* st, const char* name, int index);

/*
Searches through the stack from top to bottom for <name> of all tables within scope.
Returns associated pointer value to key upon success, otherwise NULL.

Returns a NULL pointer in the following cases:
	- NULL symbol table
	- NULL symbol table items
	- NULL hash table within symbol table
	- empty symbol table
	- empty hash table
	- invalid key
*/
struct symbol* symbol_table_scope_lookup(struct symbol_table* st, const char* name);

/*
Searches through the entire table for <name>, regardless of scope.
Returns associated pointer value to key upon success, otherwise NULL.

Returns a NULL pointer in the following cases:
        - NULL symbol table
        - NULL symbol table items
        - NULL hash table within symbol table
        - empty symbol table
        - empty hash table
        - invalid key
*/
struct symbol* symbol_table_scope_lookup_all(struct symbol_table* st, const char* name);

/*
Searches for <name> only in topmost scope, indicated by st->top
Returns a NULL pointer in the following cases:
        - NULL symbol table
        - NULL symbol table items
        - NULL hash table within symbol table
        - empty symbol table
        - empty hash table
        - invalid key
Note this is a special case of symbol_table_scope_lookup_at()
*/
struct symbol* symbol_table_scope_lookup_current(struct symbol_table* st, const char* name);

/*
Prints out all of the hash tables in the stack.
Each hash table prints out the all of their key value pairs.

If the show_hidden flag is set to false, key, hidden symbols are NOT printed.

example using function definition:
foo: function void(x: integer, z: boolean, y: char) = {
  s: string;
}

SCOPE [2]:
--------------------------------------------------
s --> (kind: local, name: s, type: string)
--------------------------------------------------

SCOPE [1]:
--------------------------------------------------
x --> (kind: parameter, name: x, type: integer)

z --> (kind: parameter, name: z, type: boolean)

y --> (kind: parameter, name: y, type: char)
--------------------------------------------------

SCOPE [0]: CURRENT (TOP) GLOBAL (BOTTOM)
--------------------------------------------------
foo --> (kind: global, name: foo, type: function void (x: integer, y: char, z: boolean))
--------------------------------------------------

*/
void symbol_table_fprint(FILE* fp, struct symbol_table* st);
void symbol_table_print(struct symbol_table* st);

#endif /* SYMBOL_TABLE_H */
