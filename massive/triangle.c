#include <mas.h>

void Triangle_Matrix(int N)
{
     int **matrix =(int **)malloc(N*sizeof(int *));
for (int i = 0; i < N; i++)
  {
    matrix[i] = (int *)malloc(N*sizeof(int));
  }
 for (int i = 0; i < N; ++i) 
 {
    for (int j = 0; j < N; ++j) 
    {
      if ((i + (j + 1)) >= N) 
      {
        matrix[i][j] = 1;
      } else 
      {
        matrix[i][j] = 0;
      }
    }
  }
  for (int i = 0; i < N; i++) 
  {
        for (int j = 0; j < N; j++) 
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}