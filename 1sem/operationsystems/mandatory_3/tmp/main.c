#include <stdio.h>
#include <stdlib.h>

int main()
{
    int a = 2;
    int b = 3;
    void **ptr = malloc(sizeof(void *) * 3);
    for(int i = 0; i < 3; i++)
    {
        ptr[i] = malloc(sizeof(void *));
    }

    ptr[0] = &a;
    a = 7;
    ptr[1] = &b;
    printf("original address: %p\tindex-addr:%p\n", ptr, ptr[0]);
    printf("p[0]:%d\tp[1]:%d",*(int*)ptr[0],*(int *)ptr[1]);
    return 0;
}