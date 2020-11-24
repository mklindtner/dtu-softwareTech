#ifndef tcb_h
#define tcb_h
#include <semaphore.h>
#include <pthread.h>
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
    void **items;
    int *queue_counter;
    int items_size;
    sem_t *isfull;
    sem_t *locklist;
    sem_t *has_elements;
    tcb_calls *producer;
    tcb_calls *consumer;
    int producers_left;
    pthread_mutex_t *decrement_lock;
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

typedef struct 
{
    void *(*call)(void *);
    void *args;
    int (*call_stop)(thread_state state);
    int (*increase_measure)(int measure);
} calls;

tcb *generate_tcb(priorities priority, calls *pcall, calls *ccall, int *id);
calls *instantiate_call(void *(*work)(void *arg), void *work_args, int (*stop)(thread_state state), int (*stop_incr)(int addition));
#endif