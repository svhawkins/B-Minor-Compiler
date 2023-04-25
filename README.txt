COMP 4060 201
Assignment 4: Symbol Table and Type Checking

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


caveats
- my 'delete' functions are called 'destroy' functions but they do the same thing.
  just difference in name

- symbol table api functions have a 'symbol_table_' prefix added to them to have consistent naming with
  other functions in all the other files.


- symbol_table itself is just a wrapper for the void* stack, better handling/manager for hashtables and symbols.
  ('all under one roof' so to speak)




- BUGS:

1. adding more tables to the stack after binding makes the top one NULL
   fails test: scope lookup multiple tables
   if both keys are in the same table it works fine.
   if the keys are in seperate tables, after scope entry binding makes it null unable to be accessed.

   this is going to cause a lot of problems later...
