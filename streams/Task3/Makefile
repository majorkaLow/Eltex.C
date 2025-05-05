CC = gcc -g 
CFLAGS = -Wall -Wextra -I. -I../Task3

MAIN_SRC = task3.c
LIB_SRC = $(wildcard *.c)

MAIN_OBJ = $(MAIN_SRC:.c=.o)
CHECK_OBJ = $(CHECK_SRC:.c=.o)
LIB_OBJ = $(LIB_SRC:.c=.o)

LIB = libstreams.a

.PHONY: all clean check

MAIN_EXEC = task3

all: $(LIB) $(MAIN_EXEC)

$(LIB): $(LIB_OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(MAIN_EXEC): $(MAIN_OBJ) $(LIB)
	$(CC) $< -L. -L../Task3 -lstreams -o $@

$(CHECK_EXEC): $(CHECK_OBJ)
	$(CC) $< -o $@

task3.o: task3.c
	$(CC) $(CFLAGS) -c -o $@ $<

check: $(CHECK_EXEC)

clean:
	rm -f *.o $(LIB) $(MAIN_EXEC) $(CHECK_EXEC)