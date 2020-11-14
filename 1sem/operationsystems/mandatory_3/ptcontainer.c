#include <stdio.h>
#include "ptcontainer.h"
// #include <pthread.h>
// #include <semaphore.h>
// #define buffersize 10
// #define sharelock 1
// #define locksize 1
// #define has_element_size 0
// #define item_size 8
//make producer/consumer here
//look at p.255


// int counter = 0; 
void *runbuffer(void *p)
{
    tcb_state *actionable = (tcb_state *)p;
    // const int sizeofbuffer = buffersize;
    locks locks;    
    sem_t locklist, has_elements, isfull;
    locks.locklist = &locklist;
    locks.has_elements = &has_elements;
    locks.isfull = &isfull;

    // printf("starting locklist\n");
    // sem_init(&locklist, sharelock, locksize);
    // sem_init(locks.locklist, sharelock, locksize);
    // // printf("starting has_elements\n");
    // sem_init(&has_elements, sharelock, has_element_size);
    // sem_init(locks.has_elements, sharelock, has_element_size);
    // // printf("starting isfull\n");
    // sem_init(&isfull, sharelock, actionable->items_size);
    // int val;
    // sem_getvalue(&isfull, &val);
    // printf("val: %d\n", val);
    // sem_init(locks.isfull, sharelock, actionable->items_size);
    // producer(actionable, &locklist, &has_elements, &isfull);
    // producer(actionable, locks);
    producer(actionable);
    // consumer(&locklist, &has_elements, &isfull);
    return NULL;
}

void *runbuffer1(void *p) {return NULL;}

// void producer(tcb_state *tcb_state, locks ll)
// void producer(tcb_state *tcb_state, sem_t *locklist,sem_t *has_elements, sem_t *isfull)
void producer(tcb_state *tcb_state)
{
    while (1)
    {
        void *result = tcb_state->call(tcb_state->call_arg);
        // printf("res: %d\n", *(int *)result);
        //produce here
        // printf("produce here elements\n");
        sem_wait(tcb_state->isfull);
        // sem_wait(ll.isfull);
        sem_wait(tcb_state->locklist);
        // sem_wait(ll.locklist);
        // printf("counter is:%d\n", counter++);
        // printf("value of ll.isfull:%s", ll.isfull);
        // int s;
        // sem_getvalue(locklist, &s);
        // printf("s: %d\n", s);
        append(result, tcb_state);
        // printf("appending\n");
        sem_post(tcb_state->locklist);
        // sem_post(ll.locklist);
        sem_post(tcb_state->has_elements);
        // sem_post(ll.has_elements);
        return;
    }
}


void append(void *result, tcb_state *tcb_state)
{
    printf("tcb_state counter: %d\n", *(tcb_state->queue_counter));
    tcb_state->items[(*tcb_state->queue_counter)++] = result;
    // items[counter++] = result;
}  

void consumer(sem_t *locklist, sem_t *has_elements, sem_t *isfull)
{
    while (1)
    {
        // printf("inside consumer");
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

// void consume(void *result)
// {
//     --counter; //#flex
// }
