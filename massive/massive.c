#include <stdio.h>
#include <stdlib.h>
int main()
{
    int N;
    printf("Введите размер квадратной матрицы: ");
    scanf("%d", &N);
    printf("Квадратная матрица:\n");
    Square_Matrix(N);
    printf("Массив в обратном порядке:\n");
    Reverse_Massive(N);
    printf("\nВывод верхнего треугольника\n");
    Triangle_Matrix(N);
    printf("\nЗаполнение матрицы числами от 1 до N спиралью:\n");
    Spiral_Matrix(N);
}