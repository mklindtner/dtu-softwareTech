#ifndef myheader_h
#define myheader_h

typedef struct 
{
    void *(*call)(void *);
    void *args;
    int (*call_stop)(thread_state state);
    int (*increase_measure)(int measure);
} calls;

void *prod_say_hello(void *number);
void *cons_say_hello(void *number);
int prod_say_hello_stop(thread_state stop_condition);
int cons_say_hello_stop(thread_state stop_condition);
int say_hello_times(int times);
tcb *generate_tcb(priorities priority, int threads, calls pcall, calls ccall, int id);
void counter(void *items);
void reader(void *items);


#endif
