CC=gcc
CFLAGS=-Wall -c
LDFLAGS=-lm

all: zcpu zasm dizasm

zcpu: cpu_arch.o zcpu.o double_stack.o io.o
	$(CC) $(LDFLAGS) cpu_arch.o zcpu.o double_stack.o io.o -o ./bin/zcpu

zasm: cpu_arch.o zasm.o io.o
	$(CC) $(LDFLAGS) cpu_arch.o zasm.o io.o -o ./bin/zasm

dizasm: dizasm.o cpu_arch.o io.o  
	$(CC) $(LDFLAGS) dizasm.o cpu_arch.o io.o  -o ./bin/dizasm

dizasm.o: dizasm.c
	$(CC) $(CFLAGS) dizasm.c  

zcpu.o: zcpu.c
	$(CC) $(CFLAGS) zcpu.c

zasm.o: zasm.c
	$(CC) $(CFLAGS) zasm.c

double_stack.o: double_stack.c
	$(CC) $(CFLAGS) double_stack.c

cpu_arch.o: scripts cpu_arch.c
	$(CC) $(CFLAGS) cpu_arch.c

io.o: io.c
	$(CC) $(CFLAGS) io.c

scripts:
	./cpu_header_gen.py

help:
	doxygen

clean:
	rm -rf *.o ./bin/*
