#include <mas.h>
void Square_Matrix(int N)
{
    int count=1;
 for(int i = 0; i < N; i++)
  {
    for(int j = 0; j < N; j++)
     {
        printf("%d ", count++);
     }
     printf("\n");
  }
}