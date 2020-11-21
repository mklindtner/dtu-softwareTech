What is the Idea of the application:
    An imitation of a Long term scheduler for threads. It takes a number of ThreadControlBlocks(tcb) and executes each one at a time. The scheduler has a ready queue and a blocked queue, it uses to different modes. One is priority (cooperation), the other mode is a time limit.
    Preemption occurs if a tcb has finished executing and a new tcb is required. If the time limit has a occured the tcb is not finished it will shut down the threads, move the tcb to the blocked queue and allow for the context switch to store the information for the tcb. The block is then moved to the ready block once said block has enough space. When loaded back into the running mechanism the context switch loads this specific tcb state in and spin up the threads again.
    
    A tcb is reminicent of a Process Control Block, though instead of representing a process, a tcb represents a job which needs to run concurrently under a single process. 
    The user is expected to implement an array of threadcontrolblocks and deliver them to the scheduler. The tcb is defined below and the program in main provides an example of an implementation.

Why is the concurrency needed:
    By looking at the way a long term scheduler uses concurrency to handle processes this scheduler can make use of the same places. In this project a generic producer/consumer pattern is provided (see scheduleroverview.PNG) the tcb provide the number of producers/consumer. Each thread is appropriate spun up providing/taking their results into a shared queue, this queue is made thread-safe. 

    If the project where to move forward concurrency could also be implemented for choosing the correct tcb through the context switch 
    

What could be the potential issues specifically
    For the consumer/producer the exact size for the array must be provided 


Address race condition
    When to threads a writing to a critical section at the same time, the output becomes undefined.

Solution for race condition
    Mutual exclusion/binary semaphore
        simply lock the critical section each time a thread writes to this section
    Sempahores 
        if multiple threads should write to a critical section but they should still be limited (ex. queue in consumer/producer that was implemented in this project)
    Monitors
       by using condition variables and a mutex, we can ensure that multiple critical sections a only written to by one thread at a time.

Address deadlock and starvation
    deadlock
        thread A, holds Resource B and needs Resource A. Thread B holds resource A and needs Resource B. Both have no timelimit.

    starvation
        when a thread cannot access a resource because other threads a being to greedy (taking to long term)

Solution for deadlock and starvation
    solutions for deadlocks
        a timer which forces a thread to release a they resource for a given time
        an outside party forces the thread to release the resource is hold if a deadlock is detected between multiple threads
    
    solutions for starvation
        a queue using FIFO principles when threads a waiting for a resource
        a simple priority scheme which increase the priority of a thread as time rises 

how to compile:
    make -pthread ptcontainer.c scheduler.c main.c
        

ThreadControlBlock:
    An object which describes the amount of consumer and producer threads that should be generated,
    together w. various necessary states for these.
    this includes:
        A function for how the producer handles each input.
        A function defining when a producer is finished.
        Amount of producer-threads
        A function for how each consumer handles each output.
        A function defining when a consumer is finished. 
        Amount of consumer-threads
        a tcb id



Need to do:
    create a makefile that works
    fix deleteelem
    add concurrency to choosing tcb's?
    Setup new file for tcb
    implement the context switch as a monitor