#include <struct.h>
void conclusion() {
    int choice;
  
    do {
      print_menu();
      scanf("%d", &choice);
  
      switch (choice) {
        case 1:
          add_a_subscriber();
          break;
        case 2:
          delete_a_subscriber();
          break;
        case 3:
          search_by_name();
          break;
        case 4:
          print_all();
          break;
        case 5:
          printf("Выход.\n");
          break;
  
        default:
          printf("Ошибка! Неверное значение пункта меню.\n");
          break;
      }
    } while (choice != 5);
  }