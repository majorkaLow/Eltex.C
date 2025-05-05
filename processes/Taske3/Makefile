CC = gcc -g 
CFLAGS = -Wall -Wextra -I. -I../Taske3

MAIN_SRC = task3.c
CHECK_SRC = check.c
LIB_SRC = $(filter-out $(MAIN_SRC) $(CHECK_SRC), $(wildcard *.c))

MAIN_OBJ = $(MAIN_SRC:.c=.o)
CHECK_OBJ = $(CHECK_SRC:.c=.o)
LIB_OBJ = $(LIB_SRC:.c=.o)

LIB = libshell.a

.PHONY: all clean check

MAIN_EXEC = task3
CHECK_EXEC = check

all: $(LIB) $(MAIN_EXEC)

$(LIB): $(LIB_OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(MAIN_EXEC): $(MAIN_OBJ) $(LIB)
	$(CC) $< -L. -L../Taske3 -lshell -o $@

$(CHECK_EXEC): $(CHECK_OBJ)
	$(CC) $< -o $@

task3.o: task3.c
	$(CC) $(CFLAGS) -c -o $@ $<

check: $(CHECK_EXEC)

clean:
	rm -f *.o $(LIB) $(MAIN_EXEC) $(CHECK_EXEC)