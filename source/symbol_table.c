#include "symbol_table.h"


/* implementation file for symbol table (stack of void pointers) */


// helper functions

// destroys a hash table and its contained symbols.
void hash_table_destroy(struct hash_table** ht) {
    if (!(*ht)) return;
    char* key; void* value;
    hash_table_firstkey(*ht);
    while (hash_table_nextkey(*ht, &key, &value)) {
      struct symbol* s = (struct symbol*)hash_table_lookup(*ht, key);
      symbol_destroy(&s); s = NULL;
    }
    hash_table_delete(*ht); *ht = NULL;
}

/*
Creates a symbol table.

Returns a NULL pointer upon any memory allocation failures.
*/
Symbol_table* symbol_table_create() { return (Symbol_table*)stack_create(); }

/*
Destroys a symbol table.
Sets st to NULL upon success.
Does nothing if NULL symbol table.
*/
void symbol_table_destroy(Symbol_table** st) {
  /*
  for each hash table:
	for all elements (iterate over):
		retrieve the symbol
		delete the symbol
	delete hash table
   delete stack
   */
  if (!(*st)) return;
  int top = (*st)->size;
  for (int i = top - 1; i >= 0; i--) {
    hash_table_destroy((struct hash_table**)&((*st)->items[i]));
    (*st)->size--; (*st)->items[i] = NULL;
  }
  stack_destroy(st); *st = NULL;
}

/*
Pushes a new hashtable to the stack
Does nothing if:
        - NULL symbol table
        - NULL symbol table items array
        - hash table fails to be created
*/
void symbol_table_scope_enter(Symbol_table* st) {
  stack_push(st, (void*)hash_table_create(0, 0));
}


/*
Removes topmost hash table from the stack
Also removes it from memory.
Does nothing if:
        - NULL symbol table
        - NULL symbol table items array
*/
void symbol_table_scope_exit(Symbol_table* st) {
  struct hash_table* ht = (struct hash_table*)stack_pop(st);
  hash_table_destroy(&(ht));
}

/*
Returns number of hashtables (scopes) currently on the stack
Returns -1 for:
        - NULL symbol table
        - NULL items array in symbol table
*/
int symbol_table_scope_level(Symbol_table* st) { return stack_size(st); }

/*
Adds <name, sym> as a key-value pair to the topmost hash table in the stack
Does nothing if:
        - NULL symbol table
        - NULL symbol table items array
        - NULL hash table within symbol table
        - empty symbol table
*/
void symbol_table_scope_bind(Symbol_table* st, const char* name, struct symbol* sym) {
  if (!st || !st->items || !st->size || !(st->items[st->size - 1])) return;
  int status = hash_table_insert((struct hash_table*)st->items[st->size - 1], name, (void*)sym);
  if (status != 1) printf("binding failure...\n");
}

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
struct symbol* symbol_table_scope_lookup(Symbol_table* st, const char* name) {
  if (!st || !st->items || !st->size) return NULL;
  struct symbol* found = NULL;
  for (int i = st->size - 1; i >= 0; i--) {
    if (!st->items[i] || !hash_table_size(st->items[i])) return NULL;
    found = (struct symbol*)hash_table_lookup(st->items[i], name);
  }
  return found;
}


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
struct symbol* symbol_table_scope_lookup_current(Symbol_table* st, const char* name) {
  return
 (st && st->items && st->size && st->items[st->size - 1] && hash_table_size(st->items[st->size - 1]))
 ? hash_table_lookup(st->items[st->size - 1], name) : NULL;
}

/*
prints out the key value pairs of the names associated with the symbols for all hash tables within
*/
//void symbol_table_fprint(FILE* fp, Symbol_table* st);
//void symbol_table_print(Symbol_table* st) { symbol_table_fprint(stdout, st); }
