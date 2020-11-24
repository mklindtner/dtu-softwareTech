#include "scheduler.h"
#include <pthread.h>
#include <stdlib.h>
#define PRODUCE_THREADS 2
#define CONSUME_THREADS 2


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
    scheduler->sp = priority; //add as parameter
    return scheduler;
}

void scheduler_cleanup(scheduler *scheduler)
{
    //free everything from scheduler not implemented yet
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
    int *tcb_blocks_size = &block_size;
    while (block_size > 0)
    {
        // printf("block size:%d\n", *tcb_blocks_size);
        swp_preempt(scheduler, tcb_blocks_size, sc);
    }
}

void **initialize_items()
{
    void **items = malloc(sizeof(void *) * ITEMS_SIZE);
    for (int i = 0; i < ITEMS_SIZE; i++)
    {
        items[i] = malloc(sizeof(void *));
    }
    return items;
}

void runner(tcb *tcb)
{
    // printf("prod amount: %d\n", tcb->produce_threads);
    for (int i = 0; i < tcb->produce_threads; i++)
    {
        printf("========CREATING PTHREAD FOR PRODUCER: tcb_ID: %d=======\n", tcb->id);
        // printf("i:%d\tpthreads:%d\n", i, tcb->produce_threads);
        pthread_create(&tcb->pid, NULL, producer, tcb->tcb_state);
    }

    for (int i = 0; i < tcb->consume_threads; i++)
    {
        printf("========CREATING PTHREAD FOR CONSUMER=======\n");
        pthread_create(&tcb->pid, NULL, consumer, tcb->tcb_state);
    }
    pthread_join(tcb->pid, NULL); //this should be where the end result is
}

//queue must not be empty
//preempt can only be called w. ready queue
void swp_preempt(scheduler *scheduler, int *block_size, scheduling_policy sc)
{
    tcb *next;
    void **items = initialize_items();
    int queue_counter = 0;
    if (sc == priority)
    {
        next = find_highest_prio(scheduler, *block_size, ready_queue);
        // printf("===next id:%d===\n", next->id);
        if (!remove_element(scheduler, block_size, ready_queue, next->id))
        {
            printf("unable to delete element");
        }
        // if (!append_element(scheduler, ready_queue, scheduler->running))
        // {
        //     printf("unable to append element");
        // }
        // printf("running should be:%d\n", next->id);

        //make as function
        next->tcb_state->items = items;
        next->tcb_state->queue_counter = &queue_counter;
        next->tcb_state->items_size = ITEMS_SIZE;

        sem_t isfull, locklist, has_elements;
        next->tcb_state->isfull = &isfull;
        next->tcb_state->locklist = &locklist;
        next->tcb_state->has_elements = &has_elements;
        sem_init(next->tcb_state->isfull, SHARELOCK, next->tcb_state->items_size);
        sem_init(next->tcb_state->locklist, SHARELOCK, LISTLOCK);
        sem_init(next->tcb_state->has_elements, SHARELOCK, ELEMENT_EXIST);
        scheduler->running = next;
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
int remove_element(scheduler *scheduler, int *block_size, queues queue, int id)
{
    int pos = -1;
    for (int i = 0; i < *block_size; i++)
    {
        tcb *cur = scheduler->containers[queue]->queue[i];
        if (cur->id == id)
        {
            pos = id;
            break;
        }
    }

    if (pos == -1)
        return 0;

    //here
    // printf("===========\n");
    // print_elements(scheduler, block_size, queue);

    //overwrite the element to be deleted

    container *new_container = make_container(scheduler->containers[queue], (*block_size)-1);
    int new_queue_idx = 0;
    for (int i = 0; i < *block_size; i++)
    {
        if (scheduler->containers[queue]->queue[i]->id == id)
        {
            continue;
        }
        new_container->queue[new_queue_idx++] = scheduler->containers[queue]->queue[i];
    }
    container *tmp = scheduler->containers[queue];

    free(tmp); //needs to free every pointer under tmp
    scheduler->containers[queue] = new_container;

    // printf("-----------\n");
    *block_size -= 1;
    // print_elements(scheduler, block_size, queue);
    // printf("===========\n");
    return 1;
}

container * make_container(container *old_container, int new_size)
{
    container *new_block = malloc(sizeof(container));
    new_block->free_pos = old_container->free_pos;
    new_block->name = old_container->name;
    tcb *new_queue = malloc(sizeof(tcb));
    new_block->queue[new_size] = new_queue;
    new_block->size = old_container->size;
    return new_block;
}

//debug function
void print_elements(scheduler *scheduler, int *block_size, queues queue)
{
    // printf("====elements====\n");
    for (int i = 0; i < *block_size; i++)
    {
        printf("id:%d\telem:%d\tpriority:%d\n", i, scheduler->containers[queue]->queue[i]->id, scheduler->containers[queue]->queue[i]->priority);
    }
    // printf("\n=====end=====\n");
}

void activate_work_priority() {}
void activate_work_time() {}

void context_switch_timer(tcb running)
{
    if (running.runtime > timepolicy)
    {
        //send to blocked queue
    }
}

/*
        ===problems===
    *  instante_scheduler: better way to instantiate   
    *  find_element: has to make entire *queue[] instead of queue[] because NULL cannot be assigned
*/


tcb *generate_tcb(priorities priority, calls *pcall, calls *ccall, int *id)
{
    // printf("f addr: %p\n", f);
    tcb *tcb_inst = malloc(sizeof(tcb));
    tcb_inst->id = (*id)++;
    tcb_inst->produce_threads = PRODUCE_THREADS;
    tcb_inst->consume_threads = CONSUME_THREADS;
    tcb_inst->priority = priority;
    tcb_inst->location = created;
    tcb_inst->tcb_state = malloc(sizeof(tcb_state));

    tcb_inst->tcb_state->producer = malloc(sizeof(tcb_calls));
    tcb_inst->tcb_state->producer->call = pcall->call;
    tcb_inst->tcb_state->producer->arg = pcall->args;
    tcb_inst->tcb_state->producer->call_stop = pcall->call_stop;
    tcb_inst->tcb_state->producer->state.increase_measure = pcall->increase_measure;
    tcb_inst->tcb_state->decrement_lock = malloc(sizeof(pthread_mutex_lock));
    tcb_inst->tcb_state->producers_left = PRODUCE_THREADS;

    tcb_inst->tcb_state->consumer = malloc(sizeof(tcb_calls));
    tcb_inst->tcb_state->consumer->call = ccall->call;
    // tcb_inst->tcb_state->consumer->arg = ccall.args;
    tcb_inst->tcb_state->consumer->call_stop = ccall->call_stop;
    tcb_inst->tcb_state->consumer->state.increase_measure = ccall->increase_measure;
    
    return tcb_inst;
}

