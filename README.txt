Sandra Hawkins
ID: 01843958
E-Mail: Sandra_Hawkins@student.uml.edu

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


- Currently my tests do not provide coverage for:
   1. iteration or selection statements that do not have an associated body (without curly braces)
      Like in C, it just uses the next line to be parsed as its body.
   2. iteration or selection statements containing only a { for its body.
      Also like in C, this will read all lines up to the first occurence of } (and not {, causes a different error)
      and use that for the body.
      
      THOUGH YOU STILL NEED TO TEST FOR MISMTACH

   3. Lone if statements without an associated else.
      Probably due to a dangling else. But not sure. I just always have to add MORE input (even just a simple ;) after it to
      Parse correctly. Maybe trying to make sure it's NOT else?

- This is considered a "Successful parse!": if (true) {}} even though there is an uneven number of curly brackets.

- I added a grammar rule called test_program that uses statements (instead of just declarations or function definitions) since
  I made all of my tests really before doing that (and I didn't want to undo all my work...)
*************************************************************************************
DIFFICULTIES:

- integrating flex and bison
	spent way too much unnecessary time (WAY TOO MUCH) trying to figure out 
	how they are supposed to work together
	solution: read the flex and bison manuals more carefully

- not realizing that yyparse() by default tries to read from stdin
	I for too long thought it was an endless loop somewhere!


THIS MIGHT CHANGE AFTER GETTING EVERYTHING AND TESTS BEGIN TO FAIL.
- I was having difficulty writing and running tests for yyparse() and the grammar because I did not quite yet realize
  that yyparse() will continue parsing all of its input even in sight of errors.
  It just 'discards' the erroneous production, marks it as a syntax error, and continues the parse starting anew. 
  I didn't realize this until I enabled the trace/-t option (this is why there is an available -t flag for [parse]).

  For example this gives 2 values from yyparse():
  x = %"493";

  // SHOW EXAMPLE

  It reads up the modulus symbol, realizes that the current input parsed ([x = %]) does not match any productions and returns failure.
  It continues with ["493";], which is a valid statement (and also in C!).
  There are more examples of this in foo.bminor.


- Didn't realize that [yyrestart(yyin);] is necessary to 'flush' the input token buffer when changing file pointers.
  Otherwise it will incorrectly parse everything and make all of my tests fail after I changed yyin's file pointer.

  

  
	
