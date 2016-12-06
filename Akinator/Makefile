CC=gcc
CFLAGS=-std=c99 -Wall

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

