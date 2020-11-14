// #include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "scheduler.h"
#include "myheader.h"
#include "ptcontainer.h"

#define THREADS 4
#define TCB_BLOCK_SIZE 3
#define PRODUCE_THREADS 3
#define CONSUME_THREADS 1


int main()
{
    printf("==========IN MAIN========\n");   
    
    void *args;
    int baz = 3;
    args = &baz;
    int id = 0;
    calls pcalls, ccalls;
    pcalls.call = prod_say_hello;
    pcalls.args = args;
    pcalls.call_stop = prod_say_hello_stop;
    pcalls.increase_measure = say_hello_times;

    ccalls.call = cons_say_hello;
    // ccalls.args = args;
    ccalls.call_stop = cons_say_hello_stop;
    ccalls.increase_measure = say_hello_times;

    tcb *tcb_1 = generate_tcb(medium, THREADS, pcalls, ccalls, id);  
    tcb *tcb_2 = generate_tcb(high, THREADS, pcalls, ccalls, id);
    tcb *tcb_3 = generate_tcb(low, THREADS, pcalls, ccalls, id);    
    tcb *tcbs[] = {tcb_1, tcb_2, tcb_3}; //must be same as block_size, consider: how to take size?    
    printf("===SCHEDULER START===\n");
    scheduler* scheduler = instantiate_scheduler();
    scheduler_start(scheduler, tcbs, TCB_BLOCK_SIZE, priority);
    // scheduler_cleanup(scheduler);
    return 0;
}

//receiver is resposinble for cleaning it up
// tcb *generate_tcb(priorities priority, int threads, void *(*c)(void *), void *call_args, int (*p_stop)(thread_state prod_state), int id)
tcb *generate_tcb(priorities priority, int threads, calls pcall, calls ccall, int id)
{
    // printf("f addr: %p\n", f);
    tcb *tcb_inst = malloc(sizeof(tcb));
    tcb_inst->id = id++;
    tcb_inst->produce_threads = PRODUCE_THREADS;
    tcb_inst->consume_threads = CONSUME_THREADS;
    tcb_inst->priority = priority;
    tcb_inst->location = created;
    tcb_inst->tcb_state = malloc(sizeof(tcb_state));

    tcb_inst->tcb_state->producer = malloc(sizeof(tcb_calls));
    tcb_inst->tcb_state->producer->call = pcall.call;
    tcb_inst->tcb_state->producer->arg = pcall.args;
    tcb_inst->tcb_state->producer->call_stop = pcall.call_stop;
    tcb_inst->tcb_state->producer->state.increase_measure = pcall.increase_measure;

    tcb_inst->tcb_state->consumer = malloc(sizeof(tcb_calls));
    tcb_inst->tcb_state->consumer->call = ccall.call;
    // tcb_inst->tcb_state->consumer->arg = ccall.args;
    tcb_inst->tcb_state->consumer->call_stop = ccall.call_stop;
    tcb_inst->tcb_state->consumer->state.increase_measure = ccall.increase_measure;
    return tcb_inst;
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
    return stop_condition.measure >= 2 ? 1 : 0;
}   

//i did not think about this1
int cons_say_hello_stop(thread_state stop_condition)
{
    return stop_condition.measure >= 3 ? 1 : 0;
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
*/