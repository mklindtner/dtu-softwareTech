#ifndef myheader_h
#define myheader_h


void *prod_say_hello(void *number);
void *cons_say_hello(void *number);
int prod_say_hello_stop(thread_state stop_condition);
int cons_say_hello_stop(thread_state stop_condition);
int say_hello_times(int times);
void counter(void *items);
void reader(void *items);
#endif
