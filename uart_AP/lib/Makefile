# Build the library from source files

OBJS = ApFunc1.o
LIB1 = lib.a

CC = gcc
AR = ar rv
CFLAGS = -c

INCLUDE = -I ./../include/

all: $(OBJS)
	$(AR) $(LIB1) $(OBJS) 

.c.o:
	$(CC) $(CFLAGS) $< -o $@ $(INCLUDE)

clean:
	rm -fr *.o *.a

