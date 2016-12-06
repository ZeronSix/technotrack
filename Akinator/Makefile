CC=gcc
CFLAGS=-std=c99 -Wall -lasan -O2 -g -Wall -Wextra -Waggressive-loop-optimizations -Wcast-align -Wcast-qual -Wchar-subscripts -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlarger-than=8192 -Wlogical-op -Wmissing-declarations -Wopenmp-simd -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-conversion -Wstack-usage=8192 -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-varargs -fcheck-new -fstack-check -fstack-protector -fstrict-overflow -fchkp-first-field-has-own-bounds -fchkp-narrow-to-innermost-array -flto-odr-type-merging -fno-omit-frame-pointer -fsanitize=address -fsanitize=alignment -fsanitize=bool -fsanitize=bounds -fsanitize=enum -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=integer-divide-by-zero -fsanitize=leak -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=object-size -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=shift -fsanitize=signed-integer-overflow -fsanitize=undefined -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr -fPIE -pie

all: main.c tree.o io.o
	$(CC) $(CFLAGS) tree.o io.o main.c -o ./bin/akinator

tree.o: tree.h tree.c
	$(CC) $(CFLAGS) -c tree.c

io.o: io.h io.c
	$(CC) $(CLFAGS) -c io.c

tests: main.c tree.o io.o
	$(CC) $(CFLAGS) io.o Tests/tests.c tree.o -o ./bin/tests
	./bin/tests

clean:
	rm -rf *.o ./bin/*

