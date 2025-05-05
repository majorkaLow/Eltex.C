#include <struct.h>
void delete_a_subscriber() {
    char name_delete[MAX_LENGHT], surname_delete[MAX_LENGHT];
    int check = 0;
    
    printf("Введите фамилию абонента которого хотите удалить: ");
    scanf("%s", surname_delete);
    
    printf("Введите имя абонента которого хотите удалить: ");
    scanf("%s", name_delete);
    
    for(int i = 0; i < quantity_subscriber; i++) {
        if (strcmp(subscribers[i].name, name_delete) == 0 && strcmp(subscribers[i].second_name, surname_delete) == 0 ) {
            subscribers[i].name[0]='\0';
            subscribers[i].second_name[0]='\0';
            subscribers[i].tel[0]='\0';
            check = 1;
            printf("Абонент удален успешно.\n");
        }
    }
        if (!check) {
            printf("Абонент не найден\n");
        }
    }