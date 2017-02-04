CC=gcc
CFLAGS=-std=c99 -Wall -lasan -O2 -g -Wall -Wextra -Waggressive-loop-optimizations -Wcast-align -Wcast-qual -Wchar-subscripts -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlarger-than=8192 -Wlogical-op -Wmissing-declarations -Wopenmp-simd -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-conversion -Wstack-usage=8192 -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-varargs -fcheck-new -fstack-check -fstack-protector -fstrict-overflow -fchkp-first-field-has-own-bounds -fchkp-narrow-to-innermost-array -flto-odr-type-merging -fno-omit-frame-pointer -fsanitize=address -fsanitize=alignment -fsanitize=bool -fsanitize=bounds -fsanitize=enum -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=integer-divide-by-zero -fsanitize=leak -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=object-size -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=shift -fsanitize=signed-integer-overflow -fsanitize=undefined -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr -fPIE -pie
LDFLAGS=-lm

all: treetex.o treemath.o main.c tree.o io.o rdp.o diff.o common.o article.o
	$(CC) $(LDFLAGS) $(CFLAGS) common.o treetex.o treemath.o diff.o rdp.o article.o tree.o io.o main.c -o ./bin/differentiator

article.o: article.h article.c
	$(CC) $(CFLAGS) -c article.c

common.o: common.h common.c
	$(CC) $(CFLAGS) -c common.c

tree.o: tree.h tree.c
	$(CC) $(CFLAGS) -c tree.c

treemath.o: tree.o treemath.h treemath.c
	$(CC) $(CFLAGS) -c treemath.c

treetex.o: treemath.o treetex.h treetex.c
	$(CC) $(CFLAGS) -c treetex.c

diff.o: tree.o treemath.o diff.h diff.c
	$(CC) $(CFLAGS) -c diff.c

io.o: io.h io.c
	$(CC) $(CLFAGS) -c io.c

rdp.o: rdp.h rdp.c tree.o treemath.o
	$(CC) $(CFLAGS) -c rdp.c

tests: tree.o io.o
	$(CC) $(CFLAGS) io.o Tests/tests.c tree.o -o ./bin/tests
	./bin/tests

clean:
	rm -rf *.o ./bin/*

