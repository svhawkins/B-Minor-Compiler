Sandra Hawkins
ID: 01843958
E-Mail: Sandra_Hawkins@student.uml.edu

COMP 4060 201
Assignment 2: B-Minor Grammar Validator

*******************************************************************************************
*******************************************************************************************
Completion rate: 97% (unresolved shift-reduce conflicts)

*******************************************************************************************
FILES (there are a lot of files, half of them are just .bminor test files)
4 directories, 37 files (after a [make] or [make all], it grows to 42)

Directories:
	source: holds the source code for the validator and scanner
	tests: holds ALL the files for running tests
	parser: subdirectory in tests to hold parser specific test files
	scanner: subdirectory in tests to hold scanner specific test files

*ALL .bminor files are test files*
.
├── Makefile : builds everything
├── README.txt
├── parse : validator executable
├── scan : scanner executable
├── source
│   ├── grammar.bison : grammar specification file
│   ├── grammar.txt : description of the grammar (generated)
│   ├── main.c : runs the validator
│   ├── parser.c : grammar automaton code (generated)
│   ├── parser.h : header file for tokens (generated)
│   ├── scan.c : runs the scanner
│   ├── scanner.c : scanner automaton code (generated)
│   └── scanner.flex : scanner specification file
├── test : executable to run all tests
└── tests
    ├── parser
    │   ├── code.bminor
    │   ├── declaration.bminor
    │   ├── empty.bminor
    │   ├── expression.bminor
    │   ├── function.bminor
    │   ├── initialization.bminor
    │   ├── iteration.bminor
    │   ├── jump.bminor
    │   ├── print.bminor
    │   ├── selection.bminor
    │   └── test_parser.c : runs parser tests
    ├── scanner
    │   ├── character.bminor
    │   ├── code.bminor
    │   ├── foo.bminor
    │   ├── identifier.bminor
    │   ├── integer.bminor
    │   ├── keywords.bminor
    │   ├── string.bminor
    │   ├── string_format.bminor
    │   └── test_scanner.c : runs scanner tests
    ├── test.c : runs the test_ executables
    ├── test_parse 
    └── test_scan

The reason that the former assignment code is here in assignment 2 is to make sure that it is still able to
run and pass all the tests after the validator has been made. Since then, these are the following changes to the
existing assignment 1 files/directory:

- Makefile now uses variables
- [parser.h] replaces [token.h]
        values OTHER than the enumeration values either from [token.h] or [token.c] were put into [main.c]
- [token.c] has been removed
- [lexer] executable has been renamed to [scan]
- the [main.c] program for [scan] has been renamed to [scan.c]
- [test.c] from assignment 1 has been renamed to [test_scanner.c]
- inclusion of directories for better code organization

*****************************************************************************************************************
EXECUTABLES:

[parse] : Runs the validator
	  Compiled by: [make], [make all], or [make parse]
	  
	  [parse] has 1 optional command line argument: -t
	  If [parse] is run with the -t option, it makes the value in yydebug be 1, showing all the
	  states and transitions the automaton goes through as output.

	  [parse] stops running the validator once EOF is reached if given a filename through input redirection.
	  If the input is stdin, use ^D (or ^C to stop at once).

	  ex) entering ^D as the only input:
	
	  [./parse]
	  0: [Successful parse!]

	  [./parse -t]
	  Starting parse
	  Entering state 0
	  Reading a token: 0: Next token is token TOKEN_EOF ()
	  Shifting token TOKEN_EOF ()
	  Entering state 1
	  Reducing stack by rule 4 (line 72):
	     $1 = token TOKEN_EOF ()
	  [Successful parse!]

[scan] : runs the scanner (from assignment 1)
	 Compiled by [make], [make all], or [make scan]

[test_parse] : runs parser test code
	       Compiled by [make], [make all], [make test_all],
	       or [make tests/test_parse] (this executuable is in the tests directory)

	       Has no command line arguments.

[test_scan] :  runs scanner  test code
               Compiled by [make], [make all], [make test_all],
               or [make tests/test_scan] (this executuable is in the tests directory)

	       Has no command line arguments.

[test] : runs the test_parse and test_scan executables (and soon to be others)
	 Compiled by [make], [make all], or [make test_all]
	 or [make test] (if only [make test], [./test] won't run properly).

	 Has no command line arguments.


******************************************************************************************************************
TESTS:
	There are 10 test 'groups' each with many test cases for the parser. Each 'group' has their own
	.bminor file:

     1. expression.bminor : tests B-Minor operators in expressions
     2. print.bminor : tests print statements
     3. jump.bminor : tests return statements
     4. iteration.bminor : tests for and while loops
     5. selection.bminor : tests if and if-else statements
     6. declaration.bminor : tests variable declarations
     7. initialization.bminor : tests variable declarations with initializations
     8. function.bminor : tests function declarations and definitions
     9. code.bminor : tests for a valid program
    10. empty.bminor : tests for an empty program

The test cases themselves consist of my own tests as well as (modified) segments of the test
code from the github.

************************************************************************************
CAVEATS:


- Currently my tests do not provide coverage for:
   1. Iteration or selection statements that do not have an associated body (without curly braces)
      Like in C, it just uses the next line to be parsed as its body.
   2. Iteration or selection statements  or function definitions without matching }
      Also like in C, this will read all lines up to the first occurence of } (and not {, causes a different error)
      and use that for the body.
   3. Lone if statements without an associated else.
      This is one of the shift-reduce conflicts. 

- This is considered a "Successful parse!": [if (true) {}}] even though there is an uneven number of curly brackets.
  This is due to a shift-reduce conflict I believe.

- I added a grammar rule called test_program that allows a program to consist
  of only statement(s) and external declarations. I made almost all of my test cases BEFORE
  writing out the rules for the 'true' start symbol (and I did not want to undo a lot of my work...)
  Test programs ALWAYS start with 2 modulus operators: [%%]:

  %% print "This is a test program"; // without the %%, this would be invalid unless part of a block statment

*****************************************************************************************
SHIFT-REDUCE CONFLICTS

My grammar has 9. As of now, I am not sure how to resolve them/there have been a few
attempts but it didn't change anything. The full detail of what they are can be read
in grammar.txt.


// STATE NUMBERS WILL CHANGE AFTER USELESS RULE REMOVAL
State 47 conflicts: 1 shift/reduce
State 62 conflicts: 1 shift/reduce
State 92 conflicts: 2 shift/reduce
State 95 conflicts: 1 shift/reduce
State 96 conflicts: 1 shift/reduce
State 162 conflicts: 1 shift/reduce
State 212 conflicts: 1 shift/reduce
State 214 conflicts: 1 shift/reduce


*************************************************************************************
DIFFICULTIES:

- integrating flex and bison
	spent way too much unnecessary time (WAY TOO MUCH) trying to figure out 
	how they are supposed to work together
	solution: read the flex and bison manuals more carefully

- not realizing that yyparse() by default tries to read from stdin

- debugging (I designed my tests poorly and they always broke after every change)

- Didn't realize that [yyrestart(yyin);] is necessary to 'flush' the input token buffer when changing file pointers.
  Otherwise it will incorrectly parse everything and make all of my tests fail after I changed yyin's file pointer.
  (Saying there is an 'unexpected TOKEN_whatever' even though that TOKEN_whatever is
   not at all whatsoever present in the current file)

- I was having difficulty writing and running tests for yyparse() and the grammar because I did not quite yet realize
  that yyparse() will continue parsing all of its input even in sight of errors.
  It just 'discards' the erroneous production, marks it as a syntax error, and continues the parse starting anew. 
  I didn't realize this until I enabled the trace/-t option (this is why there is an available -t flag for [parse]).

  For example this gives 2 values from yyparse():


  0: b);
  [Parse failure.]
  syntax error, unexpected TOKEN_RPAR, expecting TOKEN_COLON
  1: [Parse failure.]
  syntax error, unexpected TOKEN_SEMI, expecting TOKEN_EOF or TOKEN_IDENT or TOKEN_MOD or TOKEN_LPAR
  2:

  It reads up to ), realizes that the current input parsed ([b)]) does not match any productions and returns failure.
  It continues with [;], which is a valid statement in C, but not in B-Minor.

  Because of this, many test cases from the github were modified to be shorter
  in order to reduce the number of 'double counts'.
