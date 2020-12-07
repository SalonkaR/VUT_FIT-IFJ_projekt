# /*
# // IFJ20 - projekt(varianta I.)
# // Makefile
# // Matej Hornik, xhorni20
# */


CC=gcc
CFLAGS= -std=c11 -pedantic -Wall -Wextra

#preklad hlavnych casti
#--------------------------------------
all: ifj20

#testy
#--------------------------------------
test_lex: test-lex
test_symtable: test-symtable
test_parser: test-parser
test_sem: test-sem

#preklad hlavneho compilatoru
ifj20: ifj20proj.o parser.o str.o scanner.o expression.o symtable.o stack.o bt_stack.o id_queue.o gen_code.o
	$(CC) $(CFLAGS) -o $@ $^


#preklad testovacich programov
test-lex: test-lex.o str.o scanner.o
	$(CC) $(CFLAGS) -g -o $@ $^

test-symtable: test-symtable.o symtable.o str.o scanner.o id_queue.o
	$(CC) $(CFLAGS) -g -o $@ $^

test-parser: test-parser.o parser.o str.o scanner.o expression.o symtable.o stack.o bt_stack.o id_queue.o gen_code.o
	$(CC) $(CFLAGS) -g -o $@ $^

test-sem: test-sem.o parser.o str.o scanner.o expression.o symtable.o stack.o bt_stack.o id_queue.o gen_code.o
	$(CC) $(CFLAGS) -g -o $@ $^
#--------------------------------------
#objektove subory

ifj20proj.o: ifj20proj.c parser.h scanner.h
	$(CC) $(CFLAGS) -o $@ -c $<

expression.o: expression.c parser.h expression.h error.h scanner.h stack.h symtable.h bt_stack.h
	$(CC) $(CFLAGS) -o $@ -c $<

stack.o: stack.c stack.h expression.h symtable.h
	$(CC) $(CFLAGS) -o $@ -c $<

scanner.o: scanner.c scanner.h str.h error.h
	$(CC) $(CFLAGS) -o $@ -c $<

str.o: str.c str.h
	$(CC) $(CFLAGS) -o $@ -c $<

test-lex.o: tests/test-lex.c scanner.h str.h error.h
	$(CC) $(CFLAGS) -o $@ -c $<

symtable.o: symtable.c symtable.h str.h id_queue.h
	$(CC) $(CFLAGS) -o $@ -c $<

bt_stack.o: bt_stack.c bt_stack.h symtable.h
	$(CC) $(CFLAGS) -o $@ -c $<

id_queue.o: id_queue.c id_queue.h str.h
	$(CC) $(CFLAGS) -o $@ -c $<

gen_code.o: gen_code.c symtable.h expression.h scanner.h str.h
	$(CC) $(CFLAGS) -o $@ -c $<

test-symtable.o: tests/test-symtable.c symtable.h str.h error.h scanner.h
	$(CC) $(CFLAGS) -o $@ -c $<

parser.o: parser.c parser.h expression.h symtable.h str.h error.h scanner.h bt_stack.h id_queue.h gen_code.h
	$(CC) $(CFLAGS) -o $@ -c $<

test-parser.o: tests/test-parser.c parser.h symtable.h str.h error.h scanner.h
	$(CC) $(CFLAGS) -o $@ -c $<

test-sem.o: tests/test-sem.c parser.h symtable.h str.h error.h scanner.h
	$(CC) $(CFLAGS) -o $@ -c $<

#ostatne prikazy
.PHONY: clean clean-all

zip:
	zip xhorni20.zip *.c *.h Makefile

clean:
	rm -f *.o

clean-all:
	rm -f *.o test-lex test-symtable test-parser test-sem ifj20