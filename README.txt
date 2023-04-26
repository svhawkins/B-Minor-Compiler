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
  - expr_resolve fully implemented for non-name expression types
  - type_equals, type_copy, type_delete functions implemented
  - expr_resolve unit testing
  - type_equals, type_copy, type_delete unit testing
  - <struct>_typecheck functions implemented
  - <struct>_typecheck unit testing
  - <struct>_typecheck with + <struct>_resolve integration testing



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
  this includes the <struct>_resolve functions.


- symbol_table itself is just a wrapper for the void* stack, better handling/manager for hashtables and symbols.
  ('all under one roof' so to speak)

- symbol_table_scope_bind returns an int instead of being void. used to indicate bind success/failure.
  this is used for error messages.



- BUGS:
