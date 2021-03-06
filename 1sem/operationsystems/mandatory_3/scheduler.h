#ifndef scheduler_h
#define scheduler_h

#include <stdio.h>
#include "tcb.h" //includes pthread and semaphore
#include "ptcontainer.h"
#define timepolicy 1
#define QUEUES_SIZE 2
#define CONTAINER_SIZE 10 
#define START_POS 0

#define ITEMS_SIZE 9
#define SHARELOCK 1
#define LISTLOCK 1
#define ELEMENT_EXIST 0

#define COUNT_OF(x) ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x])))))

typedef enum {randnum, priority}scheduling_policy;

typedef enum {ready_queue, blocked_queue}queues;

typedef struct 
{
    int size;
    tcb *queue[CONTAINER_SIZE]; 
    char *name;
    int free_pos;
} container;

//new is moved directy to ready queue
typedef struct Scheduler
{
    container *containers[QUEUES_SIZE];
    scheduling_policy sp;    
    tcb *running;
} scheduler;

void scheduler_start(scheduler *scheduler, tcb *blocks[], int block_size, scheduling_policy sc);
scheduler* instantiate_scheduler();
void scheduler_cleanup(scheduler *scheduler);
tcb *find_highest_prio(scheduler *scheduler, int block_size, queues queue);
tcb *find_element(scheduler *scheduler, queues queue, int id);
void context_switch_prio(scheduler *scheduler, tcb *competitor);
int append_element(scheduler *scheduler, queues queue, tcb *tcb);
int remove_element(scheduler *scheduler, int *block_size, queues queue, int id);
void swp_preempt(scheduler *scheduler, int *block_size, scheduling_policy sc);
void runner(tcb *tcb);
void **initialize_items();
container *make_container(container *old_container, int new_size);
void print_elements(scheduler *scheduler, int *, queues queue);


/*
    when queue_size < THREADS + consumer = producer resets?

*/



/*
    * ===scheduler===
    * test context switch
    * make consumer/producer for each queue

*/


/*
    * ===scheduler===
    * change jobs at random
    * have a list of jobs (list of threads) - done
    * have the current job (threads)
    * do work on a single job (use threads)
    * ===producer/consumer pattern===
    * have each thread produce a result & store the result
    * take out each value stored result
*/

/*
    * ===use scheduler===
    * instantiate a scheduler
    * run_processes(schedule)
    * 
*/
#endif