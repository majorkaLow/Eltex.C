CC = gcc
CFLAGS = -Wall -Wextra -pthread -I.
TARGET = task2
OBJ = task2.o create_process.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean