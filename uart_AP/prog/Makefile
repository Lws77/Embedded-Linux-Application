# Build the executable program named uart_AP

PROG1 = uart_AP
PROGOBJ = main.o
LIB1 = ./../lib/lib.a

CC = gcc
LD = gcc
CFLAGS = -c -g
INCLUDE = -I ./../include/

all: $(PROG1) 

$(PROG1): $(PROGOBJ)
	$(LD) -o $@ $(PROGOBJ) $(LIB1)

.c.o: $(LIB1)
	$(CC) $(CFLAGS) $*.c  $(INCLUDE)

clean:
	rm -fr $(PROG1) $(PROGOBJ) $(LIB1) *.o


