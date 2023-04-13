
## -Wall errrors from generated code.
CFLAGS = -pedantic -Og -Wall -Wextra
TESTS = tests/test_scan tests/test_parse tests/test_factory tests/test_print tests/test_ast test
COMPILER = scan parse print
EXEC = $(COMPILER) $(TESTS)
OBJECTS = source/*.o tests/scanner/*.o tests/parser/*.o tests/ast/*.o
GEN = source/scanner.c source/parser.c
INCLUDES = source/decl.o source/expr.o source/param_list.o source/stmt.o source/symbol.o source/type.o

## compiles all targets
all: $(EXEC)

## compiles all source (compiler) targets
compiler: $(COMPILER)

## compiles all test targets
tests: $(TESTS)

## test targets
test: tests/test.c
	gcc -o $@ $^
tests/test_scan: tests/scanner/test_scanner.o source/scanner.o
	gcc -o $@ $^
tests/test_parse: tests/parser/test_parser.o source/scanner.o source/parser.o $(INCLUDES)
	gcc -o $@ $^
tests/test_factory: tests/ast/test_factory.o source/scanner.o source/parser.o $(INCLUDES)
	gcc -o $@ $^
tests/test_print: tests/ast/test_print.o source/scanner.o source/parser.o $(INCLUDES)
	gcc -o $@ $^
tests/test_ast: tests/ast/test_ast.o source/scanner.o source/parser.o $(INCLUDES)
	gcc -o $@ $^

## root targets
scan: source/scan.o source/scanner.o
	gcc -o $@ $^
parse: source/parse.o source/scanner.o source/parser.o $(INCLUDES)
	gcc -o $@ $^
print: source/main.o source/scanner.o source/parser.o $(INCLUDES)
	gcc -o $@ $^

## generated code
source/scanner.c: source/scanner.flex
	flex -o$@ $^
source/parser.c: source/grammar.bison
	bison --output=$@ -t -W -k -v --feature=caret --report-file=source/grammar.txt $<

## directory tests code
tests/scanner/test_scanner.o: tests/scanner/test_scanner.c source/parser.h
	gcc $(CFLAGS) -c -g $< -o $@
tests/parser/test_parser.o: tests/parser/test_parser.c source/parser.h
	gcc $(CFLAGS) -c -g $< -o $@
tests/ast/test_factory.o: tests/ast/test_factory.c source/parser.h
	gcc $(CFLAGS) -c -g $< -o $@
tests/ast/test_print.o: tests/ast/test_print.c source/parser.h
	gcc $(CFLAGS) -c -g $< -o $@
tests/ast/test_ast.o: tests/ast/test_ast.c source/parser.h
	gcc $(CFLAGS) -c -g $< -o $@

## directory source code
%source/.o: %source/.c
	gcc $(CFLAGS) -c -g $^ $@

## removal
clean:
	rm -f $(GEN) $(OBJECTS) $(EXEC)
