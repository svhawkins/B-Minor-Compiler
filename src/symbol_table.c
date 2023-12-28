#include "symbol_table.h"


/* implementation file for symbol table (stack of void pointers) */

char buffer[16];

// helper functions

// handles symbol table errors and recovery
char* symbol_table_strerror(symbol_error_t kind) {
  switch(kind) {
    case SYM_UNDEF: strcpy(buffer, "EUNDEF"); break;
    case SYM_REDEF: strcpy(buffer, "EREDEF"); break;
    case SYM_TYPE: strcpy(buffer, "ESYMTYPE"); break;
    case SYM_PARAM: strcpy(buffer, "ESYMPARAM"); break;
  }
  return buffer;
}
int symbol_table_error_handle(symbol_error_t kind, void* ctx1, void* ctx2) {
  fprintf(ERR_OUT, "ERROR %s (%d):\n", symbol_table_strerror(kind), kind);
  switch(kind) {
    case SYM_UNDEF: /* undefined symbol used. recover by adding it to table with default type integer */
    fprintf(ERR_OUT, "Undefined symbol by name %s.\n Adding symbol as INTEGER.", ((struct expr*)ctx2)->name);
    int scope = (symbol_table_scope_level((struct symbol_table*)ctx1) == 1) ? SYMBOL_GLOBAL : SYMBOL_LOCAL;
    struct symbol* s = symbol_create(scope, type_create(TYPE_INTEGER, NULL, NULL, NULL), strdup(((struct expr*)ctx2)->name));
    symbol_table_scope_bind((struct symbol_table*)ctx1, ((struct expr*)ctx2)->name, s);
    ((struct expr*)ctx2)->symbol = s;
    fprintf(ERR_OUT, "\n");
    break;
    case SYM_REDEF: /* symbol name is already being used in current scope */
    fprintf(ERR_OUT, "Symbol %s already defined in current scope as: ", ((struct symbol*)ctx2)->name);
    symbol_fprint(ERR_OUT, (struct symbol*)ctx2);
    fprintf(ERR_OUT, ".Failed to bind to new symbol "); symbol_fprint(ERR_OUT, (struct symbol*)ctx1);
    break;
    case SYM_TYPE: /* symbol type does not match its value/return type */
    if (((struct symbol*)ctx1)->type->kind == TYPE_FUNCTION) {
      fprintf(ERR_OUT, "Return type: "); type_fprint(ERR_OUT, (struct type*)ctx2);
      fprintf(ERR_OUT, " does not match declared type: "); type_fprint(ERR_OUT, ((struct symbol*)ctx1)->type->subtype);
      fprintf(ERR_OUT, "\nin symbol: "); symbol_fprint(ERR_OUT, (struct symbol*)ctx1);
    } else {
      fprintf(ERR_OUT, "Value type: "); type_fprint(ERR_OUT, (struct type*)ctx2);
      fprintf(ERR_OUT, " does not match declared type in symbol: "); symbol_fprint(ERR_OUT, (struct symbol*)ctx1);
    }
    break;
    case SYM_PARAM: /* parameter types do not match definiton */
    fprintf(ERR_OUT, "Parameter list declared by symbol: "); symbol_fprint(ERR_OUT, (struct symbol*)ctx1);
    fprintf(ERR_OUT, " does not match definition: "); type_fprint(ERR_OUT, (struct type*)ctx2);
    fprintf(ERR_OUT, "\n");
    break;
  }
  fprintf(ERR_OUT, "\n");
  global_error_count++;
  return kind;
}

// prints out the key value pairs within a hash table
void hash_table_fprint(FILE* fp, struct hash_table* ht) {
  if (!ht) { fprintf(fp, "\t\t[null table]\n\n"); return; }
  if (!hash_table_size(ht)) { fprintf(fp, "\t\t[empty table]\n\n"); return; }
  char* key; void* value;
  hash_table_firstkey(ht); fprintf(fp, "\n");
  while (hash_table_nextkey(ht, &key, &value)) {
    fprintf(fp, "%s --> ", key);
    symbol_fprint(fp, value);
  }
  fprintf(fp, "\n"); for (int i = 0; i < 50; i++) { fprintf(fp, "-"); }
}

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

// searches through a symbol table from top to bottom
struct symbol* symbol_table_search(struct symbol_table* st, const char* name, int top) {
  if (!st || !st->stack->items || !st->stack->size) return NULL;
  struct symbol* found = NULL;
  for (int i = top; i >= 0; i--) {
    found = symbol_table_scope_lookup_at(st, name, i); if (found) break;
  }
  return found;
}

/*
Creates a hidden symbol table.
Returns a NULL pointer upon any memory allocation failures.
*/
Hidden_table* symbol_table_hidden_create() {
  Hidden_table* hst = hash_table_create(0, 0);
  return hst;
}


/*
Creates a symbol table.
Returns a NULL pointer upon any memory allocation failures.
*/
struct symbol_table* symbol_table_create() {
  struct symbol_table* st = malloc(sizeof(struct symbol_table));
  if (st) {
    st->stack = stack_create();
    st->verbose = false;
    st->top = -1;
    st->show_hidden = false; // true iff from command line option
    st->hidden_table = symbol_table_hidden_create();
  }
  global_error_count = 0;
  error_status = 0;
  return st;
}

/*
Creates a symbol table, setting the verbose field to true.
Returns a NULL pointer upon any memory allocation failures.
*/
struct symbol_table* symbol_table_verbose_create() {
  struct symbol_table* st = symbol_table_create();
  if (st) st->verbose = true;
  return st;
}

/*
Destroys a hidden symbol table.
Sets hst to NULL upon success.
Does nothing if NULL hidden symbol table.
*/
void symbol_table_hidden_destroy(Hidden_table** hst) {
  if (!hst || !(*hst)) { return; }

  // delete all of the individual keys (strdupped)
  const char* key; void* value;
  hash_table_firstkey(*hst);
  while (hash_table_nextkey(*hst, &key, &value)) {
    const char* label = (const char*)hash_table_lookup(*hst, key);
    free(label);
  }

  // delete the table
  hash_table_delete((struct hash_table*)*hst);
  *hst = NULL;
}

/*
Destroys a symbol table.
Sets st to NULL upon success.
Does nothing if NULL symbol table.
*/
void symbol_table_destroy(struct symbol_table** st) {
  /*
  for each hash table:
	for all elements (iterate over):
		retrieve the symbol
		delete the symbol
	delete hash table
   delete stack
   */
  if (!(*st)) return;
  int top = (*st)->stack->size;
  for (int i = top - 1; i >= 0; i--) {
    struct hash_table* ht = stack_pop((*st)->stack);
    hash_table_destroy(&ht);
  }
  stack_destroy(&((*st)->stack));

  symbol_table_hidden_destroy(&((*st)->hidden_table));

  free(*st); *st = NULL;
  global_error_count = 0;
}

/*
Destroys then creates a new symbol table
*/
struct symbol_table* symbol_table_clear(struct symbol_table* st) {
  bool is_verbose = st->verbose, show_hidden = st->show_hidden;
  symbol_table_destroy(&st);
  st = (is_verbose) ? symbol_table_verbose_create() : symbol_table_create();
  symbol_table_scope_enter(st); // global scope
  st->show_hidden = show_hidden;
  return st;
}

/*
Pushes a new hashtable to the stack
Does nothing if:
        - NULL symbol table
        - NULL symbol table items array
        - hash table fails to be created
*/
void symbol_table_scope_enter(struct symbol_table* st) {
  st->top++;
  if ((st->top) >= stack_size(st->stack)) { stack_push(st->stack, (void*)hash_table_create(0, 0)); }
  // start which count
  if (st->top == 1) { which_count = -1; }
}


/*
Removes topmost hash table from the stack
Does nothing if:
        - NULL symbol table
        - NULL symbol table items array
*/
void symbol_table_scope_exit(struct symbol_table* st) {
  st->top--;
  // reset which count to previous count (or set to 0 if global scope)
  which_count = (st->top > 0) ? hash_table_size((struct hash_table*)st->stack->items[st->top]) - 1: 0;
}

/*
Returns number of hashtables (scopes) currently on the stack
Returns -1 for:
        - NULL symbol table
        - NULL items array in symbol table
*/
int symbol_table_scope_level(struct symbol_table* st) {
  return st->top + 1;
}

/*
Adds <name, sym> as a key-value pair to the topmost hash table in the stack
Returns 1 upon success, 0 upon failure.
Failure if:
        - NULL symbol table
        - NULL symbol table items array
        - NULL hash table within symbol table
        - empty symbol table
*/
int symbol_table_scope_bind(struct symbol_table* st, const char* name, struct symbol* sym) {
  if (!st || !st->stack->items || !(st->top + 1) || !(st->stack->items[st->top])) return 0;
  int status = (hash_table_insert((struct hash_table*)st->stack->items[st->top], name, (void*)sym) == 1);
  // update which count for successful binding of non-globals
  if (status && (stack_size(st->stack) > 1) && sym && !(sym->kind == SYMBOL_GLOBAL)) {
    which_count++; sym->which = which_count;
  }
  return status;
}

/*
Adds <string literal, label> as a key-value to the hidden symbol table.
Returns 1 upon success, 0 upon failure.
Failure if:
        - NULL hash table
*/
int symbol_table_hidden_bind(Hidden_table* hst, const char* literal, const char* label) {
  return (hst) ? hash_table_insert(hst, literal, (void*)label) : 0;
}

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
struct symbol* symbol_table_scope_lookup_at(struct symbol_table* st, const char* name, int index) {
  if (!st || !st->stack || !st->stack->items || !st->stack->size) { return NULL; }
  if (index < 0 || index >= stack_size(st->stack)) { return NULL; }
  if (!st->stack->items[index] || !hash_table_size(st->stack->items[index])) { return NULL; }
  else { return hash_table_lookup(st->stack->items[index], name); }
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
struct symbol* symbol_table_scope_lookup(struct symbol_table* st, const char* name) {
  if (!st || !st->stack->items || !st->stack->size) return NULL;
  return symbol_table_search(st, name, st->top);
}

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
struct symbol* symbol_table_scope_lookup_all(struct symbol_table* st, const char* name) {
  if (!st || !st->stack->items || !st->stack->size) return NULL;
  return symbol_table_search(st, name, stack_size(st->stack) - 1);
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
struct symbol* symbol_table_scope_lookup_current(struct symbol_table* st, const char* name) {
  return (st) ? symbol_table_scope_lookup_at(st, name, st->top) : NULL;
}

/*
Searches for <literal> in hidden symbol table's, returning the associated label.
Returns a NULL pointer in the following cases:
        - NULL hash table
        - empty hash table
        - invalid key
*/
const char* symbol_table_hidden_lookup(Hidden_table* hst, const char* literal) {
  if (!hst || (hash_table_size(hst) == 0)) { return NULL; }
  else { return hash_table_lookup(hst, literal); }
}

void symbol_table_fprint(FILE* fp, struct symbol_table* st) {
  int top = (st->verbose) ? stack_size(st->stack) - 1 : st->top;
  for (int i = top; i >= 0; i--) {
    // hash table header
    fprintf(fp, "\nSCOPE [%d]:", i);
    if (i == st->top) fprintf(fp, " CURRENT (TOP)");
    if (!i) fprintf(fp, " GLOBAL (BOTTOM)");
    fprintf(fp, "\n"); for (int j = 0; j < 50; j++) fprintf(fp, "-"); fprintf(fp, "\n");

    // print out hash table
    hash_table_fprint(fp, st->stack->items[i]);
  }
  fprintf(fp, "\n");
}

void symbol_table_print(struct symbol_table* st) { symbol_table_fprint(stdout, st); }
