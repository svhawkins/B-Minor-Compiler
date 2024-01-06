#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "vector.h"
#include "hash_table.h"
#include "decl.h"
#include "expr.h"
#include "param_list.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"

/* header file for the symbol table : a vector of hash tables (void pointers cast to hash tables) */


// for error handling for everything related
int global_error_count;
int error_status;
typedef enum { SYM_UNDEF = 1, SYM_REDEF, SYM_TYPE, SYM_PARAM } symbol_error_t;
int symbol_table_error_handle(symbol_error_t kind, void* ctx1, void* ctx2);

// code generation flags
bool generate_hidden; /* indicator if hidden symbols have been generated or not */
bool generate_expr; /* indicator if declaration is to have its entire expression generated
                         global variables must consist only of constant expressions.
                         global variables use the result of the intermediate values instead.
                      */

// file pointer for error output
FILE* ERR_OUT;

// for which assignment
int which_count;

/*
Symbol table itself is a vector wrapper handling
the casts and allocations of its hash table elements,
and symbol elements of its element hash tables.

It also has the additional field of verbose, used to indicate if it should exit scopes.
This is used when printing out (complete) symbol tables (and for testing!)

The top field is also used to indicate current scope via its index.
This ensures for proper scope lookups even with verbose enabled.

The show_hidden field is for printing purposes. It prints the hidden symbol table, a seperate structure.
*/
typedef struct hash_table Hidden_table;
struct symbol_table {
  Vector* vector;
  bool verbose;
  int top;
  bool show_hidden;
  Hidden_table* hidden_table;
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
Creates a hidden symbol table.
Returns a NULL pointer upon any memory allocation failures.
*/
Hidden_table* symbol_table_hidden_create();

/*
Destroys a symbol table.
Sets st to NULL upon success.
Does nothing if NULL symbol table.
*/
void symbol_table_destroy(struct symbol_table** st);

/*
Destroys a hidden symbol table.
Sets hst to NULL upon success.
Does nothing if NULL hidden symbol table.
*/
void symbol_table_hidden_destroy(Hidden_table** hst);


/*
Destroys then creates a new symbol table
*/
struct symbol_table* symbol_table_clear(struct symbol_table* st);

/*
Pushes a new hashtable to the vector
Does nothing if:
	- NULL symbol table
	- NULL symbol table items array
	- hash table fails to be created
*/
void symbol_table_scope_enter(struct symbol_table* st);

/*
Removes topmost hash table from the vector
Does nothing if:
	- NULL symbol table
	- NULL symbol table items array
*/
void symbol_table_scope_exit(struct symbol_table* st);

/*
Returns number of hashtables (scopes) currently on the vector
Returns -1 for:
	- NULL symbol table
	- NULL items array in symbol table
*/
int symbol_table_scope_level(struct symbol_table* st);

/*
Adds <name, sym> as a key-value pair to the topmost hash table in the vector
Returns 1 upon success, 0 upon failure.
Failure if:
	- NULL symbol table
	- NULL symbol table items array
	- NULL hash table within symbol table
	- empty symbol table
*/
int symbol_table_scope_bind(struct symbol_table* st, const char* name, struct symbol* sym);

/*
Adds <string literal, label> as a key-value to the hidden symbol table.
Returns 1 upon success, 0 upon failure.
Failure if:
        - NULL hash table
*/
int symbol_table_hidden_bind(Hidden_table* hst, const char* literal, const char* label);

/*
Searches for <name> in indicated scope from index regardless of value in st->top
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
Searches through the vector from top to bottom for <name> of all tables within scope.
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
Searches for <literal> in hidden symbol table's, returning the associated label.
Returns a NULL pointer in the following cases:
        - NULL hash table
        - empty hash table
        - invalid key
*/
const char* symbol_table_hidden_lookup(Hidden_table* hst, const char* literal);

/*
Prints out all of the hash tables in the vector.
Each hash table prints out the all of their key value pairs.

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

/*
Prints out all of the key-value (string literal to label) pairs
in the hidden symbol hash table.

example)

LITERAL --> LABEL
--------------------------------------------------
foo --> .L0
--------------------------------------------------
*/
void symbol_table_hidden_fprint(FILE* fp, Hidden_table* hst);
void symbol_table_hidden_print(Hidden_table* hst);

/*
Generates the necessary assembly labels and values from the hidden symbol table.

Example with a string literal, "foo", being associated the label ".L0":

.L0:
    .string "foo"

This is done for every entry in the table.
*/
void symbol_table_hidden_codegen(Hidden_table* hst);


#endif /* SYMBOL_TABLE_H */
