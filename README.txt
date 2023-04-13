
COMP 4060 201
Assignment 2: B-Minor Grammar Validator

*******************************************************************************************
Completion rate:

Does it meet the requirements?
*******************************************************************************************
FILES:


show tree prior to make
	filename (or directory) : brief description


EXECUTABLES:
	exec name: brief description
		how to compile
		how to run

TESTS:
	what tests are in test_parser.c?
	what do they test for?

The reason that the former assignment code is here in assignment 2 is to make sure that it is still able to
run and pass all the tests after the validator has been made. Both need to work accurately. This also means that
the code for this assignment is just an extension of my assignment 1 code, though there have been some (obvious)
changes that are not new features/additions:

- Makefile is new and improved, now with variables!
- [parser.h] replaces [token.h]
	values OTHER than the enumeration values either from [token.h] or [token.c] were put into [main.c]
- [token.c] has been removed
- inclusion of directories for better code organization
- [lexer] executable has been renamed to [scan]
- the [main.c] program for [scan] has been renamed to [scan.c]
- [test.c] from assignment 1 has been renamed to [test_scanner.c]


************************************************************************************
ERROR MESSAGES:


************************************************************************************
CAVEATS:


*************************************************************************************
DIFFICULTIES:

- integrating flex and bison
	spent way too much unnecessary time (WAY TOO MUCH) trying to figure out 
	how they are supposed to work together
	solution: read the flex and bison manuals more carefully

- not realizing that yyparse() by default tries to read from stdin
	I for too long thought it was an endless loop somewhere!
	
