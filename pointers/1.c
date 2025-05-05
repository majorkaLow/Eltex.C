#include <stdio.h>

int entry();

int changeThirdByte(int digit) {
  int inputUserValue = entry();

   char* ptr = (char*)&digit;
  ptr += 2;
 *ptr = inputUserValue;
  printf("\n%d\n", digit);
  return digit;
}

int entry() {
int num = -1;
unsigned char check=0;

while (num < 0){
check = scanf("%d", &num);

if (check != 1 || num < 0) {
while(getchar() != '\n');
num=-1;
        }
    }
    return num;
}

int main() {
int digit = entry();
changeThirdByte(digit);
return 0;
}