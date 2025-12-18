FLAGS = -Wall -Wextra -Werror -std=c11
BUILD_DIR = build

all: door_struct list_test stack_test print_module documentation_module bst_create_test bst_insert_test bst_traverse_test

clean:
	rm -f *.o
	rm -rf $(BUILD_DIR)
	clear

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

door_struct: $(BUILD_DIR)
	rm -f *.o dmanager_module.o
	gcc -c $(FLAGS) dmanager_module.c
	gcc dmanager_module.o -o $(BUILD_DIR)/Quest_1
	rm -f dmanager_module.o
	valgrind -- $(BUILD_DIR)/Quest_1

list_test: $(BUILD_DIR)
	rm -f *.o list.o
	gcc -c $(FLAGS) list.c
	gcc list.o -o $(BUILD_DIR)/Quest_2
	rm -f list.o
	valgrind -- $(BUILD_DIR)/Quest_2

stack_test: $(BUILD_DIR)
	rm -f *.o stack.o stack_test.o
	gcc -c $(FLAGS) stack.c
	gcc -c $(FLAGS) stack_test.c
	gcc stack.o stack_test.o -o $(BUILD_DIR)/Quest_3
	rm -f stack.o stack_test.o
	valgrind -- $(BUILD_DIR)/Quest_3

rebuild:
	$(BUILD_DIR)/Quest_4
	$(BUILD_DIR)/Quest_5
	$(BUILD_DIR)/Quest_6
	$(BUILD_DIR)/Quest_7
	$(BUILD_DIR)/Quest_8

print_module:
	gcc -c $(FLAGS) main_module_entry_point.c print_module.c -D Q_1
	gcc -o $(BUILD_DIR)/Quest_4 *.o
	rm *.o

documentation_module:
	gcc -c $(FLAGS) main_module_entry_point.c documentation_module.c -D Q_2
	gcc -o $(BUILD_DIR)/Quest_5 *.o
	rm *.o

bst_create_test:
	gcc -c $(FLAGS) bst.c bst_create_test.c -D Q_3
	gcc -o $(BUILD_DIR)/Quest_6 *.o
	rm *.o

bst_insert_test:
	gcc -c $(FLAGS) bst.c bst_insert_test.c -D Q_4
	gcc -o $(BUILD_DIR)/Quest_7 *.o
	rm *.o

bst_traverse_test:
	gcc -c $(FLAGS) bst.c bst_traverse_test.c -D Q_5
	gcc -o $(BUILD_DIR)/Quest_8 *.o
	rm *.o