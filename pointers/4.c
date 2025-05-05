#include <stdio.h>
#include <string.h>
#define MAX_LENGTH 100


char* searchStr(char* str, char* substr){
    if (*substr == '\0') {
    return (char*)str;
    }
    const char *j, *i, *l;
    for(i = str; *i != '\0'; i++) {
        for(j = i, l = substr; *l != '\0' && *j == *l; j++, l++) {
        }
        if (*l == '\0') {
            return (char*)i;
        }
    }
    return NULL;
}

int main() 
{
char str[MAX_LENGTH];
char substr[MAX_LENGTH];

    printf("Введите строку: ");
    fgets(str, MAX_LENGTH, stdin);
    str[strcspn(str, "\n")] = '\0';

    printf("Введите подстроку: ");
    fgets(substr, MAX_LENGTH, stdin);
    substr[strcspn(substr, "\n")] = '\0';

    char* result = searchStr(str, substr);

    if (result) {
        printf("Подстрока '%s' найдена, начиная с %ld (%p)\n", substr, result - str, (void*)result);
    } else {
        printf("Подстрока не найдена\n");
    }

    return 0;
}