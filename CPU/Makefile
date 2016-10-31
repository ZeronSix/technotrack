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

double_stack.o: scripts double_stack.c
	$(CC) $(CFLAGS) double_stack.c
	
int_stack.o: scripts int_stack.c
	$(CC) $(CFLAGS) int_stack.c

cpu_arch.o: scripts cpu_arch.c
	$(CC) $(CFLAGS) cpu_arch.c

io.o: io.c
	$(CC) $(CFLAGS) io.c

scripts:
	./cpu_header_gen.py
	./stack_gen.py int_stack.h int_stack.c intstack_ IntStack int
	./stack_gen.py double_stack.h double_stack.c doublestack_ DoubleStack double

help:
	doxygen

clean:
	rm -rf *.o ./bin/*
