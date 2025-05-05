#include <struct.h>
void add_a_subscriber() {
    if (quantity_subscriber >= MAX_SUBSCRIBER) {
        printf("Достигнуто максимальное количество абонентов\n");
        return;
    }
    quantity_subscriber++;
    subscribers = realloc(subscribers, quantity_subscriber * sizeof(abonent));
    if(subscribers == NULL){
        printf("Ошибка выделения памяти. \n");
        exit(1);
    }

    printf("Введите Фамилию: ");
    scanf("%s", subscribers[quantity_subscriber-1].second_name);
    
    printf("Введите Имя: ");
    scanf("%s", subscribers[quantity_subscriber-1].name);
    
    printf("Введите телефон: ");
    scanf("%s", subscribers[quantity_subscriber-1].tel);
    
    printf("Абонент добавлен\n");
    }