COMP 4060 201
Assignment 4: Symbol Table and Type Checking


added things:

- symbol now has a symbol_fprint function. used only as a helper function for symbol_table_fprint
- symbol_table has symbol_table_fprint and symbol_table_print functions to print out table contents and
  to indicate scope

BRANCHES:

stack:
(with manually created AST trees)
  DONE- implements basic stack for symbol table
  - implements name resolution functions for each of the AST structures
  DONE- symbol table stack unit testing (null void pointers)
 DONE - symbol table stack + hash table intgration testing
  - name resolution function unit + integration testing
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


symbol_table
(with automatically created AST trees from parser)
  - implement main function
  - stack + typecheck functions full integration testing



difficulties
- not exactly difficult, just reallt tedious was implementing the destroy functions for the ast structures.
  implementing the functions themselves was very simple, it was just that i had to change my test code.
  not exactly in behavior of the tests, but how i arranged the structures, ESPECIALLY in the manually created ones.
  there was a lot of debugging done.

- figuring out why 'test_symbol_table_scope_lookup_multi_table' was failing only to realize that
  i forgot to put a BREAK STATEMENT in the loop once the key was found

  replicating the bug in main helped me realize this by showing that all of the other
  stack, symbol_table, and hash_table functions were working fine since 'symbol_table_print'
  printed everything out correctly.


caveats
- my 'delete' functions are called 'destroy' functions but they do the same thing.
  just difference in name

- symbol table api functions have a 'symbol_table_' prefix added to them to have consistent naming with
  other functions in all the other files.


- symbol_table itself is just a wrapper for the void* stack, better handling/manager for hashtables and symbols.
  ('all under one roof' so to speak)



- BUGS:
