[09:53-10:05] - make test_print, test_factory execs, update makefile + test
[11:00-
	update readme
	added driver code for assignment
        renamed main.c

current problems:
1. ast structures do not have proper destroy() functions

*********************************************
main.c for parse was renamed to parse.c

there are 3 test executables for ast testing

test_factory tests factory functions
	i got lost in pointer hell.
test_print tests print functions
test_ast tests the ast from bison, which integrates the factory and print functions


the factory and print function tests had the structures manually created per test.

new exec: ast? pretty_print? print? -> PRINT
uses new main.c
*************

driver function has -t option (eventually)



*** 
any problems/difficulties:

1. nothing so far. some might arise with the bison ast integration.

