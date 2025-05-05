#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define INITIAL_STOCK 10000
#define CUSTOMER_NEEDS 100000
#define LOADER_SUPPLY 5000

extern int *finish_customers;
extern int *shop;
extern int *customers;
extern int num_of_finished_customers;
extern int program_finished;

extern pthread_t *customer_threads;
extern pthread_t loader_thread;

extern pthread_mutex_t m;
extern pthread_cond_t cond1;

void *loader(void *arg);
void trading_watcher(int index, int flag);
void *customer(void *arg);