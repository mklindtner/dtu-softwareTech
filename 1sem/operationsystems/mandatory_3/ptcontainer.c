#include <stdio.h>
#include "ptcontainer.h"
// #include <pthread.h>
// #include <semaphore.h>
#define buffersize 10
#define sharelock 1
#define locksize 1
#define has_element_size 0
// #define item_size 8
//make producer/consumer here
//look at p.255

/*
    1) function
    2) function arguments
    3) return type of function
    do some file shit
    do some make shit

    scheduler
        make while loop for prempting ..
        cleanup shit (threads etc.)
*/

/*
    // void *items[];
    // void (*f)(void *);
    // item_type type;

*/

// void *items[item_size];
int counter; //give as arg mby
void *runbuffer(void *p)
{
    callable *func = (callable *)p;
    // struct tcb *tcb = (struct tcb *)p;
    const int sizeofbuffer = buffersize;
    sem_t locklist, has_elements, isfull;
    // printf("starting locklist\n");
    sem_init(&locklist, sharelock, locksize);
    // printf("starting has_elements\n");
    sem_init(&has_elements, sharelock, has_element_size);
    // printf("starting isfull\n");
    sem_init(&isfull, 1, buffersize);
    producer(func->call, func->call_arg, func->items, &locklist, &has_elements, &isfull);
    consumer(&locklist, &has_elements, &isfull);
    return NULL;
}

void *runbuffer1(void *p) {return NULL;}

void producer(void *(*produce)(void *), void *produce_args, void **items, sem_t *locklist,sem_t *has_elements, sem_t *isfull)
{
    while (1)
    {
        void *result = produce(produce_args);
        //produce here
        // printf("produce here elements\n");
        sem_wait(isfull);
        sem_wait(locklist);
        append(result,items);
        // printf("appending\n");
        sem_post(locklist);
        sem_post(has_elements);
        return;
    }
}


void append(void *result, void **items)
{
    items[counter++] = result;
}  

void consumer(sem_t *locklist, sem_t *has_elements, sem_t *isfull)
{
    while (1)
    {
        sem_wait(has_elements);
        sem_wait(locklist);
        // printf("take here elements\n");
        sem_post(locklist);
        sem_post(isfull);
        // printf("consume it here\n");
        // printf("=======FINISHED=========\n");
        return;
    }
}

void consume(void *result)
{
    --counter; //#flex
}
