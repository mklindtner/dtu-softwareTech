#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#define THREADS 3

void *sayHi(void *);
void *fifth(void *);
void *tenth(void *);
void *third(void *);

typedef struct Attr
{
    int val;
    int tid;
} attr;

int main()
{
    pthread_t id[THREADS];
    // attr a;
    // a.val = 1;
    int a = 1;
    while(1)
    {
        pthread_create(&id[0],NULL, fifth, &a);
        pthread_create(&id[1],NULL, tenth, &a);
        pthread_create(&id[2],NULL, third, &a);        
    }
    // pthread_join(id[0], NULL);
    // printf("a val:%d\ta:tid:%d", a.val, a.tid);
    // pthread_join(id[0], &x);
    // printf("stuff is:%d", x);

    return 0;
}

void *sayHi(void *stuff)
{
    printf("I said:%d!\n", *(int *)stuff);
    *(int *)stuff += 1;
    pthread_exit(&stuff);
}


void *fifth(void *stuff)
{
    int second = 5 / 60 ;
    sleep(second);
    printf("|");
    pthread_exit(NULL);
}

void *tenth(void *stuff)
{
    int tenth = 10 / 60 ;
    sleep(tenth);
    printf("<");
    pthread_exit(NULL);
    
}

void *third(void *stuff)
{
    int third = 3 / 60 ; 
    sleep(third);
    printf(">");
    pthread_exit(NULL);
}