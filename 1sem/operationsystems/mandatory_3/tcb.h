#ifndef tcb_h
#define tcb_h
enum states{created, suspend, ready, running};

typedef struct 
{
    void *(*call)(void *call_args); 
    void *call_arg;
    void **items;
}callable;

typedef struct ThreadControlBlock
{
    int id;
    pthread_t pid;
    int runtime;
    int priority; 
    int produce_threads;
    int consume_threads;
    // int num_threads;
    enum states state;
    callable *callable;
    // void (*producer)(void *items[], int items_size, void (*prod_item)(void *it[], int));
    // void *(*prod)(void (*prod)(void *));
} tcb;



#endif