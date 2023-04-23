COMP 4060 201
Assignment 4: Symbol Table and Type Checking

BRANCHES:

stack:
(with manually created AST trees)
  DONE- implements basic stack for symbol table
  - implements name resolution functions for each of the AST structures
  DONE- symbol table stack unit testing (null void pointers)
  - symbol table stack + hash table intgration testing
  - name resolution function unit + integration testing


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



