CC=gcc
CFLAGS=-std=gnu99 -Wall -lasan -O2 -g -Wall -Wextra -Waggressive-loop-optimizations -Wcast-align -Wcast-qual -Wchar-subscripts -Wempty-body -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlarger-than=8192 -Wlogical-op -Wmissing-declarations -Wopenmp-simd -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-conversion -Wstack-usage=8192 -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-varargs -fcheck-new -fstack-check -fstack-protector -fstrict-overflow -fchkp-first-field-has-own-bounds -fchkp-narrow-to-innermost-array -flto-odr-type-merging -fno-omit-frame-pointer -fsanitize=address -fsanitize=bool -fsanitize=bounds -fsanitize=enum -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=integer-divide-by-zero -fsanitize=leak -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=object-size -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=shift -fsanitize=signed-integer-overflow -fsanitize=undefined -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr -fPIE -pie 
LDFLAGS=-lm

all: zcc zasm zvm
	
zcc: io.o lexer.o tree.o rdp.o simplifier.o hashmap.o translator.o zcc.c ztdlib.o
	$(CC) $(LDFLAGS) $(CFLAGS) ztdlib.o rdp.o io.o lexer.o simplifier.o hashmap.o tree.o translator.o zcc.c -o ./bin/zcc

ztdlib.o: common.h ztdlib.h ztdlib.c
	$(CC) $(CFLAGS) -c ztdlib.c

io.o: io.h io.c
	$(CC) $(CFLAGS) -c io.c

stack.o: stack.h stack.c
	$(CC) $(CFLAGS) -c stack.c

tree.o: tree.h tree.c lexer.h
	$(CC) $(CFLAGS) -c tree.c

lexer.o: lexer.h lexer.c
	$(CC) $(CFLAGS) -c lexer.c

rdp.o: lexer.o tree.o rdp.h rdp.c
	$(CC) $(CFLAGS) -c rdp.c

simplifier.o: simplifier.h simplifier.c
	$(CC) $(CFLAGS) -c simplifier.c

hashmap.o: hashmap.h hashmap.c 
	$(CC) $(CFLAGS) -c hashmap.c

translator.o: translator.h translator.c
	$(CC) $(CFLAGS) -c translator.c

zasm: io.o asm.c stack.o hashmap.o 
	$(CC) $(LDFLAGS) $(CFLAGS) asm.c hashmap.o io.o stack.o -o ./bin/zasm

zvm: io.o stack.o zvm.c
	$(CC) $(LDFLAGS) $(CFLAGS) zvm.c stack.o io.o -o ./bin/zvm

Tests: io.o
	$(CC) $(CFLAGS) $(LDFLAGS) io.o tests/tests.c -o ./bin/tests
	./bin/tests

clean:
	rm -rf *.o ./bin/*
