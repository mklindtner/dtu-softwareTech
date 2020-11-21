#include <stdio.h>
#include "ptcontainer.h"
//look at p.255

void *runbuffer1(void *p) { return NULL; }

void *producer(void *p)
{
    tcb_state *actionable = (tcb_state *)p;
    thread_state state;
    state.measure = actionable->producer->state.measure;
    while (1)
    {
        if(actionable->producer->call_stop(state))
            return NULL;
        sem_wait(actionable->isfull);
        sem_wait(actionable->locklist);
        void *thread_result = actionable->producer->call(actionable->producer->arg);
        produce(thread_result, actionable);
        state.measure = actionable->producer->state.increase_measure(state.measure);
        // state.measure++;
        // printf("===result inside===\nresult: %d\tresult address: %p\n", *(int *)thread_result, thread_result);
        sem_post(actionable->locklist);
        sem_post(actionable->has_elements);
        // return NULL;
    }
}

void produce(void *result, tcb_state *tcb_state)
{

    // printf("result from append: %d\n", *(int *)result);
    // printf("before: tcb_state counter: %d\n", *(tcb_state->queue_counter));
    // printf("===before insertion===\nresultzssss: %d\tresult address:%p\n", *(int *)result, result);
    // printf("==========================\n");

    // printf("tcb_state_id: %p\n", tcb_state);
    int idx = *(int *)(tcb_state->queue_counter);
    tcb_state->items[idx] = result;
    // printf("===after insertion===\nresult: %d\tresult address:%p\n", *(int *)result, result);
    // printf("items[idx]: %d\n", *(int *)(tcb_state->items[idx]));
    // printf("result from tcb_state:%d\ta-addr:%p\n", *(int *)tcb_state->items[*(int *)tcb_state->queue_counter], tcb_state->items[*(int *)tcb_state->queue_counter]);
    *(tcb_state->queue_counter) += 1;
    // printf("after: tcb_state counter: %d\n", *(tcb_state->queue_counter));
}

void *consumer(void *p)
{
    tcb_state *actionable = (tcb_state *)p;
    thread_state state;
    state = actionable->consumer->state;
    while (1)
    {        
        int sem_val;
        sem_getvalue(actionable->has_elements, &sem_val);
        if(sem_val == 0 && actionable->consumer->call_stop(state))
            return NULL;
        // printf("inside consumer\n");
        sem_wait(actionable->has_elements);
        sem_wait(actionable->locklist);        
        void *consumable = consume(actionable);        
        actionable->consumer->call(consumable);
        state.measure = actionable->consumer->state.increase_measure(state.measure);
        // printf("take here elements\n");
        sem_post(actionable->locklist);
        sem_post(actionable->isfull);
        // printf("consume it here\n");
        // return NULL;
    }
    printf("=======FINISHED=========\n");
}

void *consume(tcb_state *tcb_state)
{
    *(int *)tcb_state->queue_counter -= 1;
    // printf("counter: %d\t", *(int *)tcb_state->queue_counter);
    void *res = tcb_state->items[*(int *)tcb_state->queue_counter];
    // printf("res: %d\n", *(int *)res);
    return res;
}
