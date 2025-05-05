CC = gcc
CFLAGS = -Wall -Werror
TARGETS = task1 client server

all: $(TARGETS)

1: task1.o
	$(CC) $(CFLAGS) task1.o -o task1

server: server.o
	$(CC) $(CFLAGS) server.o -o server

client: client.o
	$(CC) $(CFLAGS) client.o -o client

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) *.o
	rm -f task1
	rm -f client
	rm -f server
