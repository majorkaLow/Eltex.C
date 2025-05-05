#include "task3.h"

int random_offset(int N)
{
  return (rand() % (2 * N + 1)) - N;
}

int main(void)
{
  srand(time(NULL));

  const int NUM_SHOPS = 5;
  const int NUM_CUSTOMERS = 3;

  shop = malloc(NUM_SHOPS * sizeof(int));
  customers = malloc(NUM_CUSTOMERS * sizeof(int));
  finish_customers = malloc(NUM_CUSTOMERS * sizeof(int));
  customer_threads = malloc(NUM_CUSTOMERS * sizeof(pthread_t));

  for (int i = 0; i < NUM_SHOPS; i++)
  {
    shop[i] = INITIAL_STOCK + random_offset(1000);
  }

  for (int i = 0; i < NUM_CUSTOMERS; i++)
  {
    customers[i] = 0;
    finish_customers[i] = 0;
    int *index = malloc(sizeof(int));
    *index = i;
    pthread_create(&customer_threads[i], NULL, customer, index);
  }

  pthread_create(&loader_thread, NULL, loader, NULL);

  for (int i = 0; i < NUM_CUSTOMERS; i++)
  {
    pthread_join(customer_threads[i], NULL);
  }

  pthread_join(loader_thread, NULL);

  free(shop);
  free(customers);
  free(finish_customers);
  free(customer_threads);

  return 0;
}