CC = gcc
CFLAGS = -Wall -Wextra -pthread -I.
TARGET = task1
OBJ = task1.o print_index.o create_streams.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean