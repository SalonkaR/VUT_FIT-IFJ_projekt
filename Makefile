# // Makefile 

CC=gcc
CFLAGS= -std=c11 -pedantic -Wall -Wextra

#preklad hlavnych casti
#--------------------------------------
all:

#test lexikalnej analyzy
#--------------------------------------
test_lex: test-lex

#preklad
test-lex: test-lex.o str.o scanner.o
	$(CC) $(CFLAGS) -o $@ $^

#--------------------------------------
#objektove subory


scanner.o: scanner.c scanner.h str.h error.h
	$(CC) $(CFLAGS) -o $@ -c $<

str.o: str.c str.h
	$(CC) $(CFLAGS) -o $@ -c $<

test-lex.o: tests/test-lex.c scanner.h str.h error.h
	$(CC) $(CFLAGS) -o $@ -c $<

#ostatne prikazy
.PHONY: clean clean-all

clean:
	rm -f *.o

clean-all:
	rm -f *.o test-lex