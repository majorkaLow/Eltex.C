#include <stdio.h>

void printBinary(int n) 
{
    int size = sizeof(int) *8;
    
    int mask = 1 << (size - 1);
    
    for (int i = 0; i < size; i++) 
    {
        printf("%d", (n & mask) ? 1 : 0);
        mask >>= 1;
    }
    printf("\n");
}

int countOnes(int n) 
{
    int count = 0;
    while (n != 0) 
    {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

int main() 
{
    int number;
  
    printf("1. Введите целое положительное число: ");
    scanf("%d", &number);
    printf("   Двоичное представление: ");
    printBinary(number);
    
    printf("2. Введите целое отрицательное число: ");
    scanf("%d", &number);
    printf("   Двоичное представление: ");
    printBinary(number);
   
    printf("3. Введите целое положительное число: ");
    scanf("%d", &number);
    printf("   Количество единиц в двоичном представлении: %d\n", countOnes(number));

    int originalNumber, newByte;

    printf("4. Введите целое положительное число (изначальное): ");
    scanf("%d", &originalNumber);
    printf("   Введите целое положительное число (новое значение третьего байта): ");
    scanf("%d", &newByte);
  
    int mask = 0xFF << 16;
    int result = originalNumber & ~mask;
    result |= (newByte & 0xFF) << 16;
    
    printf("   Результат после замены: %d\n", result);
    return 0;
}