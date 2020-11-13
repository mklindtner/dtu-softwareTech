// #include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "scheduler.h"
#include "myheader.h"
#include "ptcontainer.h"

#define cutoff 10
#define THREADS 2
#define BLOCK_SIZE 3
#define ITEM_SIZE 5
#define PRODUCE_THREADS 2
#define CONSUME_THREADS 1

int id;
int main()
{
    printf("==========IN MAIN========");   
    scheduler* scheduler = instantiate_scheduler();
    void *args = malloc(sizeof(void *) * 2);
    args = "hi";
    tcb *tcb_1 = generate_tcb(medium, THREADS, say_hello, args);  
    tcb *tcb_2 = generate_tcb(high, THREADS, say_hello, NULL);
    tcb *tcb_3 = generate_tcb(low, THREADS, say_hello, NULL);
    tcb *tcbs[] = {tcb_1, tcb_2, tcb_3}; //must be same as block_size, consider: how to take size?    
    printf("===SCHEDULER START===");
    scheduler_start(scheduler, tcbs, BLOCK_SIZE, priority);
    // scheduler_cleanup(scheduler);
    return 0;
}

//receiver is resposinble for cleaning it up
tcb *generate_tcb(priorities priority, int threads, void *(*c)(void *), void *call_args)
{
    // printf("f addr: %p\n", f);
    tcb *tcb_inst = malloc(sizeof(tcb));
    tcb_inst->id = id++;
    tcb_inst->produce_threads = PRODUCE_THREADS;
    tcb_inst->consume_threads = CONSUME_THREADS;
    tcb_inst->priority = priority;
    tcb_inst->state = created;
    tcb_inst->callable = malloc(sizeof(callable));
    tcb_inst->callable->call = c;
    tcb_inst->callable->call_arg = call_args;
    return tcb_inst;
}

void *say_hello(void *nothing)
{    
    printf("hello\n");
    void *res = malloc(sizeof(int) * 3);    
    return res;
}

// int items_size
void counter(void *items)
{
    int k = 0;
    // for(int i=0; i < items_size; i++)
    // {
    //     items[i] = malloc(sizeof(int));
    //     *((int *)(items[i])) = k++;
    // }
    // return NULL;
}

void reader(void *items)
{
    // for(int i=0; i < items_size; i++)
    // {
    //     printf("number: %dLALALALALALLAAL\n", *(int *)items[i]);
    // }
    // return NULL;
}