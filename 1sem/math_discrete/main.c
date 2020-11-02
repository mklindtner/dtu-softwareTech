#include <stdio.h>
#include <stdlib.h>
#define base 10
#define maxlength 1000
#define maxrows 100

void euclidsExt(int, int, int, int, int *, int *);
void startLines(int *, int, int, int *, int *);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("must provide exactly 2 arguments");
        return -1;
    }
    int *si = malloc(sizeof(int) * maxlength);
    int *ti = malloc(sizeof(int) * maxlength);
    long greater, lower;
    int counter;
    greater = strtol(argv[1], NULL, base);
    lower = strtol(argv[2], NULL, base);

    startLines(&counter, greater, lower, si, ti);
    euclidsExt(counter, greater, lower, 0, si, ti);
    free(si);
    free(ti);
    return 0;
}

//13, 5 -> 5, 3 -> 3, 2
void euclidsExt(int counter, int greater, int lower, int qi, int *si, int *ti)
{
    if (lower <= 1)
    {
        return;
    }
    long ri = greater % lower;
    qi = greater / lower;

    si[counter] = si[counter - 2] - (qi * si[counter - 1]);
    ti[counter] = ti[counter - 2] - (qi * ti[counter - 1]);
    printf("i: %d\tri: %ld\tqi: %d\tSi: %d\tTi: %d\t\tcounter: %d\n", counter, ri, qi, si[counter], ti[counter],counter);
    counter++;
    euclidsExt(counter, lower, ri, qi, si, ti);
}

void startLines(int *counter, int greater, int lower, int *si, int *ti)
{
    ti[*counter] = 0;
    si[(*counter)++] = 1;
    ti[*counter] = 1;
    si[(*counter)++] = 0;

    printf("i:-1\tri: %d\tqi: -\tSi: 1\tTi: 0\n", greater);
    printf("i: 0\tri: %d\tqi: -\tSi: 0\tTi: 1\n", lower);
}