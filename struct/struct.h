#include <stdio.h>
#include <string.h>

#define MAX_SUBSCRIBER 100
#define MAX_LENGHT 10 

extern int quantity_subscriber;

typedef struct {
  char name[MAX_LENGHT];
  char second_name[MAX_LENGHT];
  char tel[MAX_LENGHT];
} abonent;

extern abonent subscribers[MAX_SUBSCRIBER];


void search_by_name();
void print_all();
void print_menu();
void delete_a_subscriber();
void conclusion();
void add_a_subscriber();