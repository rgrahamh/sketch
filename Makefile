CC=gcc
CFLAGS=-Wall -g

CFILES=shared.c sketch.c tracer.c tracee.c

sketch: $(CFILES)
	$(CC) $(CFILES) -o sketch $(CFLAGS)
