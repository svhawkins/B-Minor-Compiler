CFLAGS = -pedantic -Og -Wall -Wextra
AST = tests/test_factory tests/test_print tests/test_ast
SYM = tests/test_data_structures tests/test_resolve tests/test_typecheck
CODEGEN = tests/test_sym_codegen  tests/test_expr_codegen tests/test_decl_codegen tests/test_decl_array_codegen tests/test_codegen
TESTS = tests/test_scan tests/test_parse $(AST) $(SYM) $(CODEGEN) test
COMPILER = scan parse print typecheck codegen
EXEC = $(COMPILER) $(TESTS)
OBJECTS = src/*.o tests/scanner/*.o tests/parser/*.o tests/ast/*.o tests/symbol_table/*.o tests/codegen/*.o
GEN = src/scanner.c src/parser.c
INCLUDES = src/decl.o src/expr.o src/param_list.o src/stmt.o src/symbol.o src/type.o src/hash_table.o src/symbol_table.o src/vector.o src/register.o

## compiles all targets
all: $(EXEC)

## compiles all source (compiler) targets
compiler: $(COMPILER)

## compiles all test targets
tests: $(TESTS)

## test targets
test: tests/test.c
	gcc -o $@ $^
tests/test_scan: tests/scanner/test_scanner.o src/scanner.o src/parser.o $(INCLUDES)
	gcc -o $@ $^
tests/test_parse: tests/parser/test_parser.o src/scanner.o src/parser.o $(INCLUDES)
	gcc -o $@ $^
tests/test_factory: tests/ast/test_factory.o src/scanner.o src/parser.o $(INCLUDES)
	gcc -o $@ $^
tests/test_print: tests/ast/test_print.o src/scanner.o src/parser.o $(INCLUDES)
	gcc -o $@ $^
tests/test_ast: tests/ast/test_ast.o src/scanner.o src/parser.o $(INCLUDES)
	gcc -o $@ $^
tests/test_data_structures: tests/symbol_table/test_data_structures.o src/scanner.o src/parser.o $(INCLUDES)
	gcc -o $@ $^
tests/test_resolve: tests/symbol_table/test_resolve.o src/scanner.o src/parser.o $(INCLUDES)
	gcc -o $@ $^
tests/test_typecheck: tests/symbol_table/test_typecheck.o src/scanner.o src/parser.o $(INCLUDES)
	gcc -o $@ $^
tests/test_codegen: tests/codegen/test_codegen.o src/scanner.o src/parser.o $(INCLUDES)
	gcc -o $@ $^
tests/test_sym_codegen: tests/codegen/test_sym_codegen.o src/scanner.o src/parser.o $(INCLUDES)
	gcc -o $@ $^
tests/test_expr_codegen: tests/codegen/test_expr_codegen.o src/scanner.o src/parser.o $(INCLUDES)
	gcc -o $@ $^
tests/test_decl_codegen: tests/codegen/test_decl_codegen.o src/scanner.o src/parser.o $(INCLUDES)
	gcc -o $@ $^
tests/test_decl_array_codegen: tests/codegen/test_decl_array_codegen.o src/scanner.o src/parser.o $(INCLUDES)
	gcc -o $@ $^

## root targets
scan: src/scan.o src/scanner.o src/parser.o $(INCLUDES)
	gcc -o $@ $^
parse: src/parse.o src/scanner.o src/parser.o $(INCLUDES)
	gcc -o $@ $^
print: src/print.o src/scanner.o src/parser.o $(INCLUDES)
	gcc -o $@ $^
typecheck: src/typecheck.o src/scanner.o src/parser.o $(INCLUDES)
	gcc -o $@ $^
codegen: src/codegen.o src/scanner.o src/parser.o $(INCLUDES)
	gcc -o $@ $^

## generated code
src/scanner.c: src/scanner.flex
	flex -o$@ $^
src/parser.c: src/grammar.bison
	bison --output=$@ -t -W -k -v --feature=caret --report-file=src/grammar.txt $<

## directory tests code
%tests/scanner/.o: %tests/scanner/.c src/parser.h src/symbol_table.h
	gcc $(CFLAGS) -c -g $< -o $@
%tests/parser/.o: %tests/parser/.c src/parser.h src/symbol_table.h
	gcc $(CFLAGS) -c -g $< -o $@
%tests/ast/.o: %tests/ast/.c src/parser.h src/symbol_table.h
	gcc $(CFLAGS) -c -g $< $@
%tests/symbol_table/.o: %tests/symbol_table/.c src/parser.h src/symbol_table.h
	gcc $(CFLAGS) -c -g $< $@
%tests/codegen/.o: %tests/codegen/.c src/parser.h src/symbol_table.h
	gcc $(CFLAGS) -c -g $< $@

## directory src code
%src/.o: %src/.c
	gcc $(CFLAGS) -c -g $^ $@

## removal
clean:
	rm -f $(GEN) $(OBJECTS) $(EXEC)
