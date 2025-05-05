#include "task3.h"

void *loader(void *arg)
{
  while (1)
  {
    pthread_mutex_lock(&m);

    if (program_finished)
    {
      pthread_mutex_unlock(&m);
      break;
    }

    int random_shop = rand() % 5;
    shop[random_shop] += LOADER_SUPPLY;
    printf("Loader added 5000 to shop %d. New stock: %d\n", random_shop, shop[random_shop]);
    pthread_mutex_unlock(&m);
    sleep(1);
  }

  return NULL;
}