CC = gcc
CFLAGS = -I$(IDIR)

IDIR = ./include
SRCDIR = ./src

SOURCES = $(SRCDIR)/solver.c

all: clean solver run 

solver:
	$(CC) $(SOURCES) $(CFLAGS) -o $@

run:
	./solver

clean:
	rm -f ./solver

