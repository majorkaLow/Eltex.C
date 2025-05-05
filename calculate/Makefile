STATIC_LIB = libcalculate.a
DYNAMIC_LIB = libmenu.so

CC = gcc
CFLAGS = -Wall -fPIC

EXECUTABLE05=calculate

STATIC_SRC = addition.c division.c multiplication.c subtraction.c
STATIC_OBJ = $(STATIC_SRC:.c=.o)

DYNAMIC_SRC = menu
DYNAMIC_OBJ = $(DYNAMIC_SRC:.o)

all: $(STATIC_LIB) menu.o $(DYNAMIC_LIB) $(EXECUTABLE05)

$(STATIC_LIB): $(STATIC_OBJ)
	ar rcs $@ $^

$(DYNAMIC_LIB): menu.o
	$(CC) -shared -o $@ $^ $(STATIC_LIB)

menu.o: menu.c 
	$(CC) $(CFLAGS) -I../calculate -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -I../calculate -c $< -o $@

$(EXECUTABLE05): calculate.o
	gcc calculate.o -L../calculate -lmenu -Wl,-rpath,../calculate -o calculate

calculate.o: calculate.c
	gcc calculate.c -I../calculate  -L../calculate -lmenu -c -o calculate.o 

clean:
	rm -f *.o 
	rm -f *.a
	rm -f *.so