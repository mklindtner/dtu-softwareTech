// #include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "scheduler.h"
#include "myheader.h"
#include "ptcontainer.h"

#define TCB_BLOCK_SIZE 3

calls *instantiate_call(void *(*work)(void *arg), void *work_args, int (*stop)(thread_state state), int (*stop_incr)(int addition));

int main()
{
    printf("==========IN MAIN========\n");       
    void *args;
    int baz = 3;
    args = &baz;
    int id = 0;
    calls *pcalls, *ccalls;
    // pcalls = instantiate_call(prod_say_hello, args);
    pcalls = instantiate_call(prod_say_hello, args, prod_say_hello_stop, say_hello_times);
    // pcalls = instantiate_call(prod_say_hello, args, prod_say_hello_stop, say_hello_times);
    // pcalls.call = prod_say_hello;
    // pcalls.args = args;
    // pcalls.call_stop = prod_say_hello_stop;
    // pcalls.increase_measure = say_hello_times;

    ccalls = instantiate_call(cons_say_hello, NULL, cons_say_hello_stop, say_hello_times);
    // ccalls = instantiate_call(cons_say_hello, NULL);
    // ccalls.call = cons_say_hello;
    // ccalls.call_stop = cons_say_hello_stop;
    // ccalls.increase_measure = say_hello_times;

    tcb *tcb_1 = generate_tcb(low, pcalls, ccalls, &id);  
    tcb *tcb_2 = generate_tcb(medium, pcalls, ccalls, &id);
    tcb *tcb_3 = generate_tcb(high, pcalls, ccalls, &id); 
    printf("id1: %d\tid2: %d\tid3: %d\n", tcb_1->id, tcb_2->id, tcb_3->id);
    tcb *tcbs[] = {tcb_1, tcb_2, tcb_3}; //must be same as block_size, consider: how to take size?    
    printf("===SCHEDULER START===\n");
    scheduler* scheduler = instantiate_scheduler();
    scheduler_start(scheduler, tcbs, TCB_BLOCK_SIZE, priority);
    scheduler_cleanup(scheduler);
    return 0;
}

calls *instantiate_call(void *(*work)(void *arg), void *work_args, int (*stop)(thread_state state), int (*stop_incr)(int addition))
{
    calls *caller = (calls *)malloc(sizeof(calls));
    caller->call = work;
    caller->args = work_args;
    caller->call_stop = stop;
    caller->increase_measure = stop_incr;
    return caller;
}



//must allocate void *
void *prod_say_hello(void *number)
{    
    printf("hello from say_hello\n");
    // printf("hello: %d\n", *(int *)nothing);    
    void *res = malloc(sizeof(void *));
    int a = 7;
    *(int *)res = a + *(int *)number;
    return res;
}

void *cons_say_hello(void *number)
{
    printf("I am consuming ,nomh, nomh, nomh!\n");
    return NULL;
}

int prod_say_hello_stop(thread_state stop_condition)
{
    return stop_condition.measure >= 1 ? 1 : 0;
}   

//i did not think about this1
int cons_say_hello_stop(thread_state stop_condition)
{
    return stop_condition.measure >= 1 ? 1 : 0;
}

int say_hello_times(int times)
{
    return (++times);
}

/*
    scheduler
        make while loop for prempting ..
        cleanup shit (threads etc.)
    tcbs
        make sure prempting works
            tcb0 prints out even numbers
            tcb1 prints out odd numbers

    problems
        consumer/producer cannot be of same number, consumer must always be less right now
        if you declare more producer/consumer than the entire array, outOfBounds will occur
*/