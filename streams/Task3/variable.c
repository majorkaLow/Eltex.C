#include "task3.h"

int *finish_customers;
int *shop;
int *customers;
int num_of_finished_customers = 0;
int program_finished = 0;

pthread_t *customer_threads;
pthread_t loader_thread;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;