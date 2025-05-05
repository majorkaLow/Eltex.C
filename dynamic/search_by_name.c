#include <struct.h>
void search_by_name() {
    char name_search[MAX_LENGHT];
  int check = 0;
  
  printf("Введите имя абонента которого хотите найти: ");
  scanf("%s", name_search);
  
  for(int i = 0; i < quantity_subscriber; i++) {
      if (strcmp(subscribers[i].name, name_search) == 0 ) {
          printf("Имя: %s, Фамилия: %s, Телефон: %s\n",
          subscribers[i].name, subscribers[i].second_name, subscribers[i].tel);
          check = 1;
      }
  }
      if (!check) {
          printf("Абонент не найден\n");
      }
  }
  