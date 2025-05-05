#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SUBSCRIBER 100
#define MAX_LENGHT 10 


typedef struct {
  char name[MAX_LENGHT];
  char second_name[MAX_LENGHT];
  char tel[MAX_LENGHT];
} abonent;

extern int quantity_subscriber;
extern abonent *subscribers;


void search_by_name();
void print_all();
void print_menu();
void delete_a_subscriber();
void conclusion();
void add_a_subscriber();