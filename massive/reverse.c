#include <mas.h>

void Reverse_Massive(int N)
{
 int temp, count=1, arr[N];

   for(int i = 0; i < N; i++)
    {
   arr[i]=count;
   count++;
    printf("%d ", arr[i]);
    }

 printf("\n");
  for (int i = 0; i<N/2; i++ )
    {
    temp = arr[i];
    arr[i] = arr[N-i-1];
    arr[N-i-1] = temp;
    }

  for (int i = 0; i < N; i++) 
    {
        printf("%d ", arr[i]);
    }

    printf("\n");
}