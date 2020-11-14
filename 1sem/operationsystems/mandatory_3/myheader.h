#ifndef myheader_h
#define myheader_h

void *say_hello(void *);
int say_hello_stop(thread_state stop_condition);
tcb *generate_tcb(priorities priority, int threads, void *(*call)(void *), void *call_args, int (*p_stop)(thread_state state), int id);
void counter(void *items);
void reader(void *items);


#endif
