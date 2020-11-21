CC=clang
CFLAGS=-pthread -Wall -I.
DEPS=scheduler.h ptcontainer.h tcb.h myheader.h scheduler_types.h
OBJ = scheduler.o ptcontainer.o main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

a.out: $(OBJ)
    $(CC) $@ $^ $(CFLAGS)
