CC = gcc
CFLAGS = -Wall -pthread 
TARGETS = task1 

all: $(TARGETS)

1: task1.o
	$(CC) $(CFLAGS) task1.o -o task1

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) *.o
	rm -f task1
