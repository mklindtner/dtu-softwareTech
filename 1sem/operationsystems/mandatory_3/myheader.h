#ifndef myheader_h
#define myheader_h

void *say_hello(void *);
tcb *generate_tcb(priorities priority, int threads, void *(*call)(void *), void *call_args);
void counter(void *items);
void reader(void *items);


#endif
