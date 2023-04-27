COMP 4060 201
Assignment 4: Symbol Table and Type Checking


added things:

- symbol now has a symbol_fprint function. used only as a helper function for symbol_table_fprint
- symbol_table has symbol_table_fprint and symbol_table_print functions to print out table contents and
  to indicate scope
- symbol_table now has an additional feature: verbose
  it is by default false, but a symbol table can be created with it set to true
  by running ./typecheck with -v option

 verbose flag makes it so the symbol table wont exit scopes, so ALL symbols seen throughout
 the ast remain until destruciton

- symbol table now has additional field: top
  top keeps track of the topmost table IN SCOPE regardless of verbose status.
  also makes the code easier to read/shorter.

  this makes for correct current scope lookups when verbose

- symbol table has the following functions added:
  symbol_table_scope_lookup_at(Symbol_table* st, const char* name, int index)
     looks up a name in the symbol table given the index of indicated table.
  symbol_table_scope_lookup_all(Symbol_table* st, const char* name)
     looks up a name by going through ALL hash tables (not just the ones in scope!)
     symbol_table_scope_lookup only goes through the in-scope tables, as indicated by st->top.

BRANCHES:

DONEstack:
(with manually created AST trees)
  DONE- implements basic stack for symbol table
  DONE- implements name resolution functions for each of the AST structures
  DONE- symbol table stack unit testing (null void pointers)
 DONE - symbol table stack + hash table intgration testing
 DONE - name resolution function unit + integration testing
 DONE - hashtable + symbol integration testing (brief)

-overdue task:
  DONEdestroy functions for structure types

typecheck:
(with manually created AST trees)

  DONE- expr_typecheck fully implemented for primitve  expression types
  - DONEtype_equals, type_copy, type_delete functions implemented
  DONE- expr_typcheck unit testing for primitive expressions
DONE - expr_typecheck unit testing for logical, arithmetic, relational, and equality expressions
  DONE- <struct>_typecheck functions implemented
  DONE- <struct>_typecheck unit testing (excluding error cases)
  DONE- expr_typecheck fully implemented
	to do: EXPR_FCALL, EXPR_SUBSCRIPT, EXPR_INIT, EXPR_COMMA
 - expr_typecheck testing for EXPR_FCALL,EXPR_SUBSCRIPT, EXPR_INIT, EXPR_COMMA
  - error message handler



difficulties
- not exactly difficult, just really tedious was implementing the destroy functions for the ast structures.
  implementing the functions themselves was very simple, it was just that i had to change my test code.
  not exactly in behavior of the tests, but how i arranged the structures, ESPECIALLY in the manually created ones.
  there was a lot of debugging done.

- figuring out why 'test_symbol_table_scope_lookup_multi_table' was failing only to realize that
  i forgot to put a BREAK STATEMENT in the loop once the key was found

  replicating the bug in main helped me realize this by showing that all of the other
  stack, symbol_table, and hash_table functions were working fine since 'symbol_table_print'
  printed everything out correctly.

- figuring out why i was getting segfaults with hash_table_insert() in scope_bind in decl_resolve
  only to realize that after running just the failing test in valgrind was due to a double free.
  symbols in the symbol table are not deep copies of the symbols in the ast structures. the pointers in the table and
  the structures point to the same object.
  so when the <struct>_destroy() functions were called, the symbol field was being destroyed in both the ast object and the table,
  when it should be just the table (or just the object. whatever, just not both)


caveats
- my 'delete' functions are called 'destroy' functions but they do the same thing.
  just difference in name

- symbol table api functions have a 'symbol_table_' prefix added to them to have consistent naming with
  other functions in all the other files.

- symbol_table itself is just a wrapper for the void* stack, better handling/manager for hashtables and symbols.
  ('all under one roof' so to speak)

- symbol_table_scope_bind returns an int instead of being void. used to indicate bind success/failure.
  this is used for error messages.

- param_list_typecheck is NOT IMPLEMENTED.
  for determining if parameter lists are the same, param_list_equals is called
  (type_equals with function types works too, type_equals calls param_list_equals)

  for determining if argument lists are the proper type a comparision function is called.

- the following functions were changed from a void return type to an int return type:
      symbol_table:
	* symbol_table_scope_bind
      typechecking:
	* decl_typecheck
	* stmt_typecheck
   their return types were changed to int so that their error status able to be returned.
   this error status is the error code that is used by the error message handler.



- BUGS:
