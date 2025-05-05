CC = gcc
CFLAGS = -Wall -Wextra -I ../massive

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
LIB = libmas.a

.PHONY: all clean


EXECUTABLE02 = massive


all: $(LIB) $(EXECUTABLE02)

$(LIB): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXECUTABLE02): massive.o
	gcc massive.o  -L../massive -lmas -o massive

massive.o: massive.c 
	gcc massive.c -I../massive -c -o massive.o 

clean:
	rm -f *.o $(LIB)