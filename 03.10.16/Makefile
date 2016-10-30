CC=gcc
CFLAGS=-std=c99 -Wall

all: test help

test:
	$(CC) $(CFLAGS) ./tests/test_stack.c -o ./tests/TEST__stack
	./tests/TEST__stack

help:
	doxygen

clean:
	rm -rf *.o help tests/TEST__* 
