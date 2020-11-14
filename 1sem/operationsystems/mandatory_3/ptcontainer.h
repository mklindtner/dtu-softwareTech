#ifndef ptcontainer_h
#define ptcontainer_h
#include "semaphore.h"
#include "tcb.h"


void produce();
void consume();
void *runbuffer1(void *p);
void *consumer(void *tcb_state);
void *producer(void *tcb_state);
void produce(void *result, tcb_state *tcb_state);
#endif
