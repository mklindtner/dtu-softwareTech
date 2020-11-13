#ifndef ptcontainer_h
#define ptcontainer_h
#include "semaphore.h"
#include "tcb.h"
// #include "tcb.h"
#define ITEM_SIZE 5

// typedef struct {
//     int size;
//     void *items[ITEM_SIZE];
//     // producer p;
//     // consumer c;
// } items;

// typedef struct {
//     void (*work)(void *, int);    
// } producer;


// typedef struct {
//     void (*work)(void *, int);
// } consumer;

// void producer(void *(*produce)(void *), void *produce_args, sem_t *locklist, sem_t *has_elements, sem_t *isfull);
void producer(void *(*produce)(void *), void *produce_args, void **items, sem_t *locklist,sem_t *has_elements, sem_t *isfull);
void consumer(sem_t *locklist, sem_t *has_elements, sem_t *isfull);
void *runbuffer(void *);
void produce();
void consume();
void *runbuffer1(void *p);
void append(void *result, void **items);

// void producer(void *items[], int items_size, void (*prod_item)(void *it[], int item_size));
// void consumer(void *items[], void (*con_item)(void *it[], int item_size), int items_size);

#endif
