#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *elements = malloc(sizeof(int) * 10);
    elements[0] = 0;
    elements[1] = 1;
    elements[2] = 2;
    int del = 2, length = 3;
    for(int i = 0; i < length; i++)
    {
        printf("i:%d, el:%d\n", i, elements[i]);
    }
    for(int i = del-1; i < length; i++)
    {
        elements[i] = elements[i+1];
    }
    printf("============\n");
    for(int i = 0; i < length-1; i++)
    {
        printf("i:%d, el:%d\n", i, elements[i]);
    }
    return 0;
}