CC = gcc
CFLAGS = -Wall -Wextra -I ../FileManager

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
LIB = libFManager.a

EXECUTABLE04 = FM

.PHONY: all clean

all: $(LIB) $(EXECUTABLE04)

$(LIB): $(OBJ)
	ar rcs -lncursesw $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXECUTABLE04): fileManager.o
	gcc fileManager.o  -L../FileManager -lFManager -lncursesw -o FManager

fileManager.o: fileManager.c
	gcc fileManager.c -I../FileManager -c -o fileManager.o 

clean:
	rm -f *.o $(LIB)

r: clean all