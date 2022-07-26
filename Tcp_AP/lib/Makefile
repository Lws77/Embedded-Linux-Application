# Build the library from source files

OBJ1 = serFun.o
OBJ2 = cltFun.o

LIBS = serLib.a cltLib.a

CC = gcc
LD = gcc
AR = ar rv
CFLAGS = -c -g
INCLUDE = -I ./../include/

all: $(LIBS)

$(LIBS): $(OBJ1) $(OBJ2)
	$(AR) -o $@ $(OBJ1) 
	$(AR) -o $@ $(OBJ2)

.c.o:
	$(CC) $(CFLAGS) $< $(INCLUDE)

clean:
	rm -fr *.o *.a