CC=gcc
CFLAGS=-std=c99 -Wall

all:
	$(CC) $(CFLAGS) input.c -lm -o input
	$(CC) $(CFLAGS) string_funcs.c -lm -o string_funcs

help:
	doxygen

clean:
	rm -rf *.o input string_funcs
