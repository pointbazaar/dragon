#Manual:
#https://westes.github.io/flex/manual/

FLAGS := -g -march=native -Wall --std=c11 

INCLUDE_DIR := include/

C_HEADERS := $(INCLUDE_DIR)/driver.h $(INCLUDE_DIR)/test.h ../token/token/token.h ../token/reader/token_reader.h $(INCLUDE_DIR)/lexer_flags.h

C_SOURCES := src/driver.c test/lexer_test_utils.c test/test.c

SOURCES := smalldragon.flex Makefile $(C_SOURCES)

INCLUDE := -I$(INCLUDE_DIR)

all: dragon-lexer test

#-L/usr/lib/x86_64-linux-gnu
LINKARGS :=  -lfl -lsd-token -L../token/build/

dragon-lexer: $(SOURCES) $(C_HEADERS)
	flex smalldragon.flex
	gcc $(FLAGS) -o dragon-lexer $(INCLUDE) $(C_SOURCES) lex.yy.c $(LINKARGS)

clean:
	rm -f dragon-lexer
	rm -f lex.yy.c

test: dragon-lexer
	./dragon-lexer -test

VALGRIND_OPS := --leak-check=full --show-reachable=yes --error-exitcode=1 

valgrind: dragon-lexer
	valgrind $(VALGRIND_OPS) ./dragon-lexer ../examples/other/Everything.dg
