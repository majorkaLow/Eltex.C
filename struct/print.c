#include <struct.h>
void print_all() {
    int printed = 0;
  
      for (int i = 0; i <= quantity_subscriber; i++) { 
          if (subscribers[i].name[0] != '\0' && subscribers[i].second_name[0] != '\0') {
              printf("Фамилия: %s, Имя: %s, Телефон: %s\n", subscribers[i].second_name, subscribers[i].name, subscribers[i].tel);
              printed = 1;
          }
      }
  
      if (!printed) {
          printf("Список пуст\n");
      }
  } 