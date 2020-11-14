#include "scheduler.h"
#include <pthread.h>
#include <stdlib.h>

#define COUNT_OF(x) ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x])))))

#define ITEMS_SIZE 2


void printElements(scheduler *scheduler, int, queues queue);

scheduler *instantiate_scheduler()
{
    container *rdy_queue = malloc(sizeof(container));
    rdy_queue->name = "ready";
    rdy_queue->size = CONTAINER_SIZE;
    rdy_queue->free_pos = START_POS;

    container *blocked_container = malloc(sizeof(container));
    blocked_container->name = "blocked";
    blocked_container->size = CONTAINER_SIZE;
    blocked_container->free_pos = START_POS;

    scheduler *scheduler = malloc(sizeof(scheduler));
    scheduler->containers[ready_queue] = rdy_queue;
    scheduler->containers[blocked_queue] = blocked_container;
    // scheduler->running = NULL;
    scheduler->sp = priority;
    return scheduler;
}

void scheduler_cleanup(scheduler *scheduler)
{
    //free everything from scheduler
    free(scheduler->containers[ready_queue]);
    free(scheduler->containers[blocked_queue]);
    free(scheduler);
}

void scheduler_start(scheduler *scheduler, tcb *blocks[], int block_size, scheduling_policy sc)
{
    for (int i = 0; i < block_size; i++)
    {
        if (append_element(scheduler, ready_queue, blocks[i]) == 0)
        {
            printf("unable to append element");
            return;
        }
    }    
    //while here 
    swp_preempt(scheduler, block_size, sc);
    block_size -= 1;
}


void **initialize_items()
{
    void **items = malloc(sizeof(void *));
    for(int i = 0; i < ITEMS_SIZE; i++)
    {
        items[i] = malloc(sizeof(void *));
    }
    return items;
}

void runner(tcb *tcb)
{
    printf("prod amount: %d\n", tcb->produce_threads);
    for(int i = 0; i < tcb->produce_threads; i++)
    {        
        printf("========CREATING PTHREAD FOR PRODUCER: tcb_ID: %d=======\n", tcb->id);
        // printf("i:%d\tpthreads:%d\n", i, tcb->produce_threads);
        pthread_create(&tcb->pid, NULL, runbuffer, tcb->callable); 
    }

    // for(int i = 0; i < tcb->consume_threads; i++)
    // {
    //     printf("========CREATING PTHREAD FOR CONSUMER=======\n");
    //     pthread_create(&tcb->pid, NULL, runbuffer, NULL);
    // }
    pthread_join(tcb->pid, NULL); //this should be where the end result is
}

//queue must not be empty
//preempt can only be called w. ready queue
void swp_preempt(scheduler *scheduler, int block_size, scheduling_policy sc)
{
    tcb *next;
    void **items = initialize_items();
    int queue_counter = 0;
    if (sc == priority)
    {
        next = find_highest_prio(scheduler, block_size, ready_queue);
        if (!remove_element(scheduler, block_size, ready_queue, next->id))
        {
            printf("unable to delete element");
        }
        if (!append_element(scheduler, ready_queue, scheduler->running))
        {
            printf("unable to append element");
        }
        // printf("running should be:%d\n", next->id);
        scheduler->running = next;        
        scheduler->running->callable->items = items;
        scheduler->running->callable->queue_counter = &queue_counter;
        scheduler->running->callable->items_size = ITEMS_SIZE;
    }
    // printf("prod_threads:%d\ttcb_id:%d\tblock_size:%d", next->produce_threads, next->id, );
    // if (sc == randnum) { /*change running at random intervals */};    
    runner(scheduler->running);
}

tcb *find_highest_prio(scheduler *scheduler, int block_size, queues queue)
{
    tcb *highest = scheduler->containers[queue]->queue[0];
    for (int i = 0; i < block_size; i++)
    {
        tcb *cur = scheduler->containers[queue]->queue[i];
        if (cur->priority < highest->priority)
        {
            highest = cur;
        }
    }
    return highest;
}

//should take a producer instead
int append_element(scheduler *scheduler, queues queue, tcb *tcb)
{

    int pos = scheduler->containers[queue]->free_pos;
    if (pos < scheduler->containers[queue]->size)
    {
        int *i = &scheduler->containers[queue]->free_pos;
        scheduler->containers[queue]->queue[(*i)++] = tcb;
        return 1;
    }
    return 0;
}

tcb *find_element(scheduler *scheduler, queues queue, int id)
{
    for (int i = 0; i < COUNT_OF(scheduler->containers[queue]->queue); i++)
    {
        tcb *cur = scheduler->containers[queue]->queue[i];
        if (cur->id == id)
            return cur;
    }
    return NULL;
}

//does not free the tcb to be removed
int remove_element(scheduler *scheduler, int block_size, queues queue, int id)
{
    int pos = -1;
    // int size = sizeof(scheduler->containers[queue]->queue);
    for (int i = 0; i < block_size; i++)
    {
        tcb *cur = scheduler->containers[queue]->queue[i];
        if (cur->id == id)
            pos = id;
    }
    if (pos == -1)
        return 0;

    //overwrite the element to be deleted
    for (int i = pos - 1; i < block_size; i++)
    {
        scheduler->containers[queue]->queue[i] = scheduler->containers[queue]->queue[i + 1];
    }
    return 1;
}

//debug function
void printElements(scheduler *scheduler, int block_size, queues queue)
{
    printf("====elements====\n");
    for (int i = 0; i < block_size; i++)
    {
        printf("id:%d\t", scheduler->containers[queue]->queue[i]->id);
    }
    printf("\n=====end=====\n");
}


// void activate_work(scheduler *scheduler, priorities priority) 
// {
//     //make thread     
//     pthread_t p[scheduler->running->num_threads];
//     for(int i = 0; i < scheduler->running->num_threads; i++)
//     {
//         pthread_create(&p[i], NULL, scheduler->running->work, scheduler->running->work_arg);
//     }    
//     //switch case(priority)
    
//     // for(int i = 0; i < scheduler->running->num_threads; i++)
//     // {

//     //     pthread_join(p, );
//     // }
//     //do work for each thread    
//     //thread.join()/cleanup    
// }

/*
    make function that will store values in producer/consumer pattern
    work( void(*prod)(void *))??
*/

void activate_work_priority() {}
void activate_work_time() {}


// void context_switch_prio(scheduler *scheduler, tcb *competitor)
// {
//     if(scheduler->running->priority > competitor->priority)
//     {
//         // int elems = (sizeof(scheduler->ready_queue) /  sizeof(scheduler->ready_queue[0]));
//         int total_queue = sizeof(scheduler->containers->ready_queue);
//         // if(scheduler->containers->free_pos_ready < total_queue)
//         // {
//         //     // scheduler->ready_queue[elems+1] = scheduler->running;
//         //     scheduler->containers->ready_queue[scheduler->containers->free_pos_ready++] = scheduler->running;
//         //     scheduler->running->state = ready;
//         //     competitor->state = running;
//         //     scheduler->running = competitor;
//         // }
//     }
// }

void context_switch_timer(tcb running)
{
    if (running.runtime > timepolicy)
    {
        //send to blocked queue
    }
}

// void scheduler_prempt(scheduler *scheduler, tcb *src)
// {
//     tcb *tmp = scheduler->running;
//     scheduler->containers->ready_queue[src->id];
// }

// int scheduler_append_tcb(scheduler *scheduler, tcb *tcb)
// {
//     if(scheduler->containers->free_pos_ready < scheduler->containers->blocked_size)
//     {
//         scheduler->containers->ready_queue[scheduler->containers->free_pos_ready++] = tcb;
//         return 1;
//     }
//     return 0;
// }

/*
        ===problems===
    *  instante_scheduler: better way to instantiate   
    *  find_element: has to make entire *queue[] instead of queue[] because NULL cannot be assigned
*/