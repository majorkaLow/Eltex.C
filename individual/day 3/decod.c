#include <stdio.h>

// Декодер/энкодер - запрещено создавать массивы и использовать динамическую память
void encode();  // кодирование
void decode();  // декодирование

int main(int argc, char* argv[]) {
    // printf("%d\n", argc);

    if (argc == 2) {
        // printf("%d\n", argv[1][0]);
        if (argv[1][0] == 48) {  // 0
            encode();
            printf("\n");
        }
        if (argv[1][0] == 49) {  // 1
            decode();
            printf("\n");
        }

    } else
        printf("n/a\n");

    return 0;
}

/* Функции */
void encode() {
    char bukva;
    int cod;
    // printf("%d\n",scanf("%c", &bukva));
    while (1) {
        if (scanf("%c", &bukva) == 1) {
            cod = bukva;
            printf("%X ", cod);
            if (getchar() == 10) break;
        } else {
            printf("n/a");
            break;
        }
    }
}
void decode() {
    char bukva2;
    int cod2;
    while (1) {
        if (scanf("%x", &cod2)) {
            bukva2 = cod2;
            printf("%C ", cod2);
            if (getchar() == 10) break;
        } else {
            printf("n/a");
            break;
        }
    }
}