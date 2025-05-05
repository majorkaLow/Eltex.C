#include <struct.h>
void add_a_subscriber() {
    if (quantity_subscriber >= MAX_SUBSCRIBER) {
        printf("Достигнуто максимальное количество абонентов\n");
        return;
    }
    
    printf("Введите Фамилию: ");
    scanf("%s", subscribers[quantity_subscriber].second_name);
    
    printf("Введите Имя: ");
    scanf("%s", subscribers[quantity_subscriber].name);
    
    printf("Введите телефон: ");
    scanf("%s", subscribers[quantity_subscriber].tel);
    
    quantity_subscriber++;
    printf("Абонент добавлен\n");
    }