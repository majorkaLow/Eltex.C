

void Spiral_Matrix(int N)
{
  int arr[N][N];
  int count=1;
  int Start_Row = 0, End_Row = N-1;
  int Start_Column = 0, End_Column = N-1;
  int dir = 1;

  while(Start_Row <= End_Row && Start_Column <= End_Column)
  {
    if (dir == 1)
    {
      for (int j = Start_Column; j <= End_Column; j++)
      {
        arr[Start_Row][j]=count++;
      }
      Start_Row++;
    }
    else if (dir == 2)
    {
      for (int i = Start_Row; i <= End_Row; i++)
      {
        arr[i][End_Column]=count++;
      }
      End_Column--;
    } else if (dir == 3)
    {
      for(int j = End_Column; j >= Start_Column; j--)
      {
        arr[End_Row][j]=count++;
      }
      End_Row--;
    }
    else if (dir == 4)
    {
      for (int i = End_Row; i >= Start_Row; i--)
      {
        arr[i][Start_Column]=count++;
      }
      Start_Column++;
    }
    dir = ( dir + 1 ) % 5;
    
  }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}