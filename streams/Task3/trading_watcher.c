#include "task3.h"

void trading_watcher(int index, int flag)
{
  pthread_mutex_lock(&m);
  if (flag == 1)
  {
    pthread_cancel(customer_threads[index]);
  }
  else
  {
    pthread_cancel(loader_thread);
  }
  pthread_mutex_unlock(&m);
}