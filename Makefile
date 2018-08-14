PROGRAM	= lavirint
CC	= gcc
CFLAGS = -g -Wall
LDFLAGS = -lGL -lGLU -lglut

$(PROGRAM): 1.o
	$(CC) -o $(PROGRAM) 1.o $(LDFLAGS)

.PHONY: clean dist

clean:
	-rm *.o $(PROGRAM) *core

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)

