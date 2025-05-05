#include "task3.h"

void *customer(void *arg)
{
  int index = *(int *)arg;
  free(arg);

  while (1)
  {
    pthread_mutex_lock(&m);

    if (program_finished)
    {
      pthread_mutex_unlock(&m);
      break;
    }

    if (customers[index] >= CUSTOMER_NEEDS)
    {
      if (finish_customers[index] != -1)
      {
        finish_customers[index] = -1;
        num_of_finished_customers++;
        printf("Customer %d has fulfilled their needs.\n", index);
      }

      if (num_of_finished_customers == 3)
      {
        program_finished = 1;
        printf("All customers have finished.\n");
      }
    }

    if (finish_customers[index] != -1)
    {
      int random_shop = rand() % 5;
      if (shop[random_shop] > 10000)
      {
        customers[index] += 10000;
        shop[random_shop] -= 10000;
        printf("Customer %d entered shop %d. Customer's goods: %d. Remaining stock in shop: %d\n", index, random_shop, customers[index], shop[random_shop]);
        sleep(2);
      }
    }

    pthread_mutex_unlock(&m);
    sleep(1);
  }

  return NULL;
}