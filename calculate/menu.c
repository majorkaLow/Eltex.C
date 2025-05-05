#include <menu.h>

void menu() {
printf("Выберите операцию: 1.Сумма 2.Разность 3.Умножение 4.Деление\n");

int check;
if(scanf("%d", &check) != 1) {
    printf("Ошибка. Введите целое число.\n");
    return;
}

float a,b;
printf("Введите 2 числа:\n");
if (scanf("%f %f", &a, &b) !=2 ) {
    printf("Ошибка. Введите два числа.\n");
    return;
}

float result;

switch (check) {
    case 1:
    result = addition(a, b);
    printf("Ответ: %f\n", result);
    break;

    case 2:
    result =subtraction(a, b);
    printf("Ответ: %f\n", result);
    break;

    case 3:
    result= multiplication(a, b);
    printf("Ответ: %f\n", result);
    break;

    case 4:
    result = division(a, b);
    printf("Ответ: %f\n", result);
    break;

    default:
    printf("Неверный выбор операции. \n");
    break;
}
}