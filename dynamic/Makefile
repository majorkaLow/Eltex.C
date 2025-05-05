CC = gcc -g 
CFLAGS = -Wall -Wextra -I . -I ../dynamic

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
LIB = libdynamic.a

.PHONY: all clean


EXECUTABLE03 = dinamic

all: $(LIB) $(EXECUTABLE03)

$(LIB): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXECUTABLE03): struct.o
	gcc struct.o  -L../dynamic -ldynamic -o struct

struct.o: structure.c
	gcc structure.c -I../dynamic -c -o struct.o 

clean:
	rm -f *.o $(LIB)