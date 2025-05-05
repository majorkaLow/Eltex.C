#include <struct.h>
void delete_a_subscriber() {

    if (quantity_subscriber == 0 ) {
        printf("Справочник пуст. \n");
        return;
    }

    char name_delete[MAX_LENGHT];
    int check = 0;

    printf("Введите имя абонента для удаления: ");
    scanf("%s", name_delete);

    for (int i = 0; i <= quantity_subscriber; i++)
    {
        if (strcmp(subscribers[i].name, name_delete) == 0)
        {
            for (int j = i; j < quantity_subscriber - 1; j++)
            {
                subscribers[j] = subscribers[j + 1];
            }

            quantity_subscriber--;
            subscribers = realloc(subscribers, quantity_subscriber * sizeof(abonent));
            if (subscribers == NULL && quantity_subscriber > 0)
            {
                printf("Ошибка выделения памяти.\n");
                exit(1);
            }

            check = 1;
            printf("Абонент %s удален из справочника.\n", name_delete);
            break;
        }
    }

    if (!check)
    {
        printf("Абонент с именем %s не найден в справочнике.\n", name_delete);
    }
}