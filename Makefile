CC=gcc
CFLAGS=-Wall -g

CFILES=./shared/shared.c ./sketch/sketch.c ./tracer/tracer.c ./tracee/tracee.c

debugger: $(CFILES)
	$(CC) $(CFILES) -o sktch $(CFLAGS)
