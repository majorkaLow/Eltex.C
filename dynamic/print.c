#include <struct.h>
void print_all() {
    int printed = 0;
      for (int i = 0; i < quantity_subscriber ; i++) {
              printed = 1;
              printf("Имя: %s, Фамилия: %s, Телефон: %s\n",
                subscribers[i].name, subscribers[i].second_name, subscribers[i].tel);
      }
  
      if (!printed) {
          printf("Список пуст\n");
      }
  } 