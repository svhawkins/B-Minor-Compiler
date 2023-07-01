CFLAGS = -pedantic -Og -Wall -Wextra
AST = tests/test_factory tests/test_print tests/test_ast
SYM = tests/test_stack tests/test_typecheck
TESTS = tests/test_scan tests/test_parse $(AST) $(SYM) tests/test_codegen test
COMPILER = scan parse print typecheck codegen
EXEC = $(COMPILER) $(TESTS)
OBJECTS = source/*.o tests/scanner/*.o tests/parser/*.o tests/ast/*.o tests/symbol_table/*.o tests/codegen/*.o
GEN = source/scanner.c source/parser.c
INCLUDES = source/decl.o source/expr.o source/param_list.o source/stmt.o source/symbol.o source/type.o source/hash_table.o source/symbol_table.o source/stack.o source/register.o

## compiles all targets
all: $(EXEC)

## compiles all source (compiler) targets
compiler: $(COMPILER)

## compiles all test targets
tests: $(TESTS)

## test targets
test: tests/test.c
	gcc -o $@ $^; ./test;
tests/test_scan: tests/scanner/test_scanner.o source/scanner.o source/parser.o $(INCLUDES)
	gcc -o $@ $^
tests/test_parse: tests/parser/test_parser.o source/scanner.o source/parser.o $(INCLUDES)
	gcc -o $@ $^
tests/test_factory: tests/ast/test_factory.o source/scanner.o source/parser.o $(INCLUDES)
	gcc -o $@ $^
tests/test_print: tests/ast/test_print.o source/scanner.o source/parser.o $(INCLUDES)
	gcc -o $@ $^
tests/test_ast: tests/ast/test_ast.o source/scanner.o source/parser.o $(INCLUDES)
	gcc -o $@ $^
tests/test_stack: tests/symbol_table/test_stack.o source/scanner.o source/parser.o $(INCLUDES)
	gcc -o $@ $^
tests/test_typecheck: tests/symbol_table/test_typecheck.o source/scanner.o source/parser.o $(INCLUDES)
	gcc -o $@ $^
tests/test_codegen: tests/codegen/test_codegen.o source/scanner.o source/parser.o $(INCLUDES)
	gcc -o $@ $^

## root targets
scan: source/scan.o source/scanner.o source/parser.o $(INCLUDES)
	gcc -o $@ $^
parse: source/parse.o source/scanner.o source/parser.o $(INCLUDES)
	gcc -o $@ $^
print: source/print.o source/scanner.o source/parser.o $(INCLUDES)
	gcc -o $@ $^
typecheck: source/typecheck.o source/scanner.o source/parser.o $(INCLUDES)
	gcc -o $@ $^
codegen: source/main.o source/scanner.o source/parser.o $(INCLUDES)
	gcc -o $@ $^

## generated code
source/scanner.c: source/scanner.flex
	flex -o$@ $^
source/parser.c: source/grammar.bison
	bison --output=$@ -t -W -k -v --feature=caret --report-file=source/grammar.txt $<

## directory tests code
%tests/scanner/.o: %tests/scanner/.c source/parser.h source/symbol_table.h
	gcc $(CFLAGS) -c -g $< -o $@
%tests/parser/.o: %tests/parser/.c source/parser.h source/symbol_table.h
	gcc $(CFLAGS) -c -g $< -o $@
%tests/ast/.o: %tests/ast/.c source/parser.h source/symbol_table.h
	gcc $(CFLAGS) -c -g $< $@
%tests/symbol_table/.o: %tests/symbol_table/.c source/parser.h source/symbol_table.h
	gcc $(CFLAGS) -c -g $< $@
%tests/codegen/.o: %tests/codegen/.c source/parser.h source/symbol_table.h
	gcc $(CFLAGS) -c -g $< $@

## directory source code
%source/.o: %source/.c
	gcc $(CFLAGS) -c -g $^ $@

## removal
clean:
	rm -f $(GEN) $(OBJECTS) $(EXEC)
