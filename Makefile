CC=gcc
CFLAGS=-Wall -g

sketch: sketch.c tracer.c tracee.c
	$(CC) sketch.c tracer.c tracee.c -o sketch $(CFLAGS)
