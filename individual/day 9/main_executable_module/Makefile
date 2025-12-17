CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

SRCS = ../data_libs/data_io.c \
       ../data_libs/data_stat.c \
       ../yet_another_decision_module/decision.c \
       ../data_module/data_process.c \
       main_executable_module.c

OBJS = $(SRCS:.c=.o)

BUILD_DIR = ../../build
TARGET = $(BUILD_DIR)/Quest_3
MACRO_TARGET = $(BUILD_DIR)/Quest_4
DYNAMIC_TARGET = $(BUILD_DIR)/Quest_5

PROCESS_LIB = data_process.so
PROCESS_SRC = ../data_module/data_process.c
PROCESS_OBJ = $(PROCESS_SRC:.c=.o)

.PHONY: all clean rebuild build_with_macro build_with_dynamic $(PROCESS_LIB)

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS) $(PROCESS_OBJ)
	@rm -f $(TARGET) $(MACRO_TARGET) $(DYNAMIC_TARGET)
	@rm -f $(PROCESS_LIB)
	@rm -rf $(BUILD_DIR)

rebuild: clean all

build_with_macro: CFLAGS += -DUSE_MACRO_IO
build_with_macro: clean $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRCS) -o $(MACRO_TARGET)

$(PROCESS_LIB): $(PROCESS_SRC)
	$(CC) -c -fPIC $(CFLAGS) $(PROCESS_SRC) -o $(PROCESS_OBJ)
	$(CC) -shared -o $(PROCESS_LIB) $(PROCESS_OBJ)

build_with_dynamic: CFLAGS += -DUSE_DYNAMIC
build_with_dynamic: clean $(BUILD_DIR) $(PROCESS_LIB)
	$(CC) $(CFLAGS) main_executable_module.c ../data_libs/data_io.c ../data_libs/data_stat.c \
		../yet_another_decision_module/decision.c -o $(DYNAMIC_TARGET) -ldl

build_with_static:
	gcc -c $(FLAGS) $(LIBS)
	ar r data_stat.a data_stat.o
	rm data_stat.o
	gcc -o ../../build/Quest_5 *.o data_stat.a
	clear
	leaks -atExit -- ../../build/Quest_5
	rm *.o
	rm *.a

	