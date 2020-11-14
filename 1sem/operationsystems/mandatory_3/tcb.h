#ifndef tcb_h
#define tcb_h
#include <semaphore.h>
typedef enum {created, suspend, ready, running}states;
typedef enum {high, medium, low}priorities;

typedef struct 
{
    int measure; //time or amount of times a thread should run
    int (*increase_measure)(int measure);
}thread_state;

typedef struct
{
    void *(*call)(void *call_arg); 
    void *arg;
    int (*call_stop)(thread_state prod_state);
    thread_state state;
}tcb_calls;

typedef struct 
{
    //all threads state
    void **items;
    int *queue_counter;
    int items_size;
    sem_t *isfull;
    sem_t *locklist;
    sem_t *has_elements;
    tcb_calls *producer;
    tcb_calls *consumer;
}tcb_state; 

typedef struct ThreadControlBlock
{
    int id;
    pthread_t pid;
    int runtime;
    int priority; 
    int produce_threads;
    int consume_threads;
    states location;
    tcb_state *tcb_state;
} tcb;


//consumer putting everything the user needs to fill out into a seperate
// typedef struct 
// {
//     int id;
//     int produce_threads;
//     int consume_thread;
//     priorities priority;
//     states state;
//     tcb_state *tcb_state;
// }ui;




#endif