# Build the executable program named tcp_clinet
PROG1 = tcp_client
PROGOBJ = tcp_client.o

CC = gcc
LD = gcc
CFLAGS = -c -g
INCLUDE = -I ./../../include/
LIBS = ./../../lib/cltLib.a

all: $(PROG1) 

$(PROG1): $(PROGOBJ)
	$(LD) -o $(PROG1) $(PROGOBJ) $(LIBS)

.c.o: $(LIBS)
	$(CC) $(CFLAGS) -o $@ $*.c  $(INCLUDE)

clean:
	rm -fr $(PROG1) $(PROGOBJ) $(LIB1) *.o
