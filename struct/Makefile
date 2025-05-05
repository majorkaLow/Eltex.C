CC = gcc -g 
CFLAGS = -Wall -Wextra -I . -I ../struct

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
LIB = libstructure.a

.PHONY: all clean


EXECUTABLE03 = struct

all: $(LIB) $(EXECUTABLE03)

$(LIB): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXECUTABLE03): struct.o
	gcc struct.o  -L../struct -lstructure -o struct

struct.o: structure.c
	gcc structure.c -I../struct -c -o struct.o 

clean:
	rm -f *.o $(LIB)