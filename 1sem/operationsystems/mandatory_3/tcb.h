#ifndef tcb_h
#define tcb_h
#include <semaphore.h>
typedef enum {created, suspend, ready, running}states;
typedef enum {high, medium, low}priorities;

typedef struct 
{
    void *(*call)(void *call_args); 
    void *call_arg;
    void **items;
    int *queue_counter;
    int items_size;
    sem_t *isfull;
    sem_t *locklist;
    sem_t *has_elements;
}tcb_state; 

typedef struct ThreadControlBlock
{
    int id;
    pthread_t pid;
    int runtime;
    int priority; 
    int produce_threads;
    int consume_threads;
    states state;
    tcb_state *tcb_state;
} tcb;


// typedef struct 
// {
//     int id;
//     int produce_threads;
//     int consume_thread;
//     priorities priority;
//     states state;
//     tcb_state *tcb_state;
// }ui;

//consumer putting everything the user needs to fill out into a seperate



#endif