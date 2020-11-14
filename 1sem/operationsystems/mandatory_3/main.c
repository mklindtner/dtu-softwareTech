// #include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "scheduler.h"
#include "myheader.h"
#include "ptcontainer.h"

// #define cutoff 10
#define THREADS 4
#define BLOCK_SIZE 3
#define ITEM_SIZE 5
#define PRODUCE_THREADS 3
#define CONSUME_THREADS 1


int main()
{
    printf("==========IN MAIN========\n");   
    scheduler* scheduler = instantiate_scheduler();
    void *args;
    int baz = 3;
    args = &baz;
    int id;
    tcb *tcb_1 = generate_tcb(medium, THREADS, say_hello, args,id);  
    tcb *tcb_2 = generate_tcb(high, THREADS, say_hello, args, id);
    tcb *tcb_3 = generate_tcb(low, THREADS, say_hello, args, id);
    tcb *tcbs[] = {tcb_1, tcb_2, tcb_3}; //must be same as block_size, consider: how to take size?    
    printf("===SCHEDULER START===\n");
    scheduler_start(scheduler, tcbs, BLOCK_SIZE, priority);
    // scheduler_cleanup(scheduler);
    return 0;
}

//receiver is resposinble for cleaning it up
tcb *generate_tcb(priorities priority, int threads, void *(*c)(void *), void *call_args, int id)
{
    // printf("f addr: %p\n", f);
    tcb *tcb_inst = malloc(sizeof(tcb));
    tcb_inst->id = id++;
    tcb_inst->produce_threads = PRODUCE_THREADS;
    tcb_inst->consume_threads = CONSUME_THREADS;
    tcb_inst->priority = priority;
    tcb_inst->state = created;
    tcb_inst->tcb_state = malloc(sizeof(tcb_state));
    tcb_inst->tcb_state->call = c;
    tcb_inst->tcb_state->call_arg = call_args;
    return tcb_inst;
}

void *say_hello(void *nothing)
{    
    printf("hello from say_hello\n");
    // return NULL;
    // printf("hello: %d\n", *(int *)nothing);    
    void *res;
    int a = 2;
    res = &a;    
    return res;
}

/*
    1) function
    2) function arguments
    3) return type of function
    do some file shit
    do some make shit

    scheduler
        why segmentfault sometimes?
            faults at say_hello
        what is int values?
        use args
        make while loop for prempting ..
        cleanup shit (threads etc.)

*/



// // int items_size
// void counter(void *items)
// {
//     int k = 0;
//     // for(int i=0; i < items_size; i++)
//     // {
//     //     items[i] = malloc(sizeof(int));
//     //     *((int *)(items[i])) = k++;
//     // }
//     // return NULL;
// }

// void reader(void *items)
// {
//     // for(int i=0; i < items_size; i++)
//     // {
//     //     printf("number: %dLALALALALALLAAL\n", *(int *)items[i]);
//     // }
//     // return NULL;
// }