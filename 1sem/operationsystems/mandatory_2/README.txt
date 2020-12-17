 Memory Management
===================

The problem will focus on memory.  You will implement your own 
version of malloc() and free(), using a variety of allocation strategies.

You will be implementing a memory manager for a block of memory.  You will
implement routines for allocating and deallocating memory, and keeping track of
what memory is in use.  You will implement four strategies for selecting in
which block to place a new requested memory block:

  1) First-fit: select the first suitable block with smallest address.
  2) Best-fit: select the smallest suitable block.
  3) Worst-fit: select the largest suitable block.
  4) Next-fit: select the first suitable block after
     the last block allocated (with wraparound
     from end to beginning).


Here, "suitable" means "free, and large enough to fit the new data".

Here are the functions you will need to implement:

initmem():
  Initialize memory structures.

mymalloc():
  Like malloc(), this allocates a new block of memory.

myfree():
  Like free(), this deallocates a block of memory.

mem_holes():
  How many free blocks are in memory?

mem_allocated():
  How much memory is currently allocated?

mem_free():
  How much memory is NOT allocated?

mem_largest_free():
  How large is the largest free block?

mem_small_free():
  How many small unallocated blocks are currently in memory?

mem_is_alloc():
  Is a particular byte allocated or not?

A structure has been provided for use to implement these functions.  It is a
doubly-linked list of blocks in memory (both allocated and free blocks).  Every
malloc and free can create new blocks, or combine existing blocks.  You may
modify this structure, or even use a different one entirely.  However, do not
change function prototypes or files other than mymem.c.

IMPORTANT NOTE: Regardless of how you implement memory management, make sure
that there are no adjacent free blocks.  Any such blocks should be merged into
one large block.

A few functions are given to help you monitor what happens when you
call your functions.  Most important is the try_mymem() function.  If you run
your code with "mem -try <args>", it will call this function, which you can use
to demonstrate the effects of your memory operations.  These functions have no
effect on test code, so use them to your advantage.

Running your code:

After running "make", run

1) "mem" to see the available tests and strategies.
2) "mem -test <test> <strategy>" to test your code with provided tests.
3) "mem -try <args>" to run your code with your own tests
   (the try_mymem function).

You can also use "make test" and "make stage1-test" for testing.  "make
stage1-test" only runs the tests relevant to stage 1.

Running "mem -test -f0 ..." will allow tests to run even
after previous tests have failed.  Similarly, using "all" for a test or strategy
name runs all of the tests or strategies.  Note that if "all" is selected as the
strategy, the 4 tests are shown as one.

One of the tests, "stress", runs an assortment of randomized tests on each
strategy.  The results of the tests are placed in "tests.out" .  You may want to
view this file to see the relative performance of each strategy.


Stage 1
-------

Implement all the above functions, for the first-fit strategy. Use "mem -test all first"
 to test your implementation


Stage 2
-------

Implement  all the other three strategies out of: 
worst-fit
best-fit
next-fit.  
You can work in a group and discuss the assignment but you all have to implement individually.
Answer the 10 questions asked below.
The strategy is passed to initmem(), and stored in the global variable "myStrategy".
Some of your functions will need to check this variable to implement the
correct strategy.

You can test your code with "mem -test all worst", etc., or test all 4 together
with "mem -test all all".  The latter command does not test the strategies
separately; your code passes the test only if all four strategies pass.


Answer the following questions as part of your report
=====================================================

1) Why is it so important that adjacent free blocks not be left as such?  What
would happen if they were permitted? 

A: 
  Fragmentation can occur, if small blocks are freed which are then not used again.
  Over time the system will be split into smaller and smaller blocks, taking up larger and larger space.

2) Which function(s) need to be concerned about adjacent free blocks?

A: 
  myfree is responsible for cleaning up and combining adjacent free blocks (know as the buddy system).


3) Name one advantage of each strategy.

A: 
  first-fit: on average this allocates the fastest. 
  best-fit: has the most accuracte memory allocation
  next-fit: Has a better runtime than best or first for memory allocation, since it starts from a roving pointer.
  worst-fit: best runtime over long periods, it subdues some of the problems with fragmentation for best/first/next


4) Run the stress test on all strategies, and look at the results (tests.out).
What is the significance of "Average largest free block"?  Which strategy
generally has the best performance in this metric?  Why do you think this is?
  A:
    If we're presuming to use the same allocations for each algorithm, bestfit has the smallest "Average largest free block".
    For the largest "Average largest free block" we can assume it to be worst-case though the test did not display this convincingly.
 
    By having the smallest "largest free block", we may expect the most splits for each block to occur.

    An argument for this occurence, is that a larger block means more smaller blocks have been allocated and that
    these blocks (when freed) will devolve into further smaller blocks, making the least use of the largest memory block.

    Thus a higher number of memory blocks will exist in the system, as the largest memory block remain untouched.
    This increases the chance of internal memory fragmentation.    

    If our assumptions hold so far, we can also argue that locality (hit rate) is much faster for the algorithm with the largest "free memory block".
    This is due to the fact that less occupied blocks will exist in the system, making the likelyhood of hitting an available block, before the end, faster.

5) In the stress test results (see Question 4), what is the significance of
"Average number of small blocks"?  Which strategy generally has the best
performance in this metric?  Why do you think this is?
  A: The higher the number of "Average number of small blocks" the more fragmentation occurs within the system, bestfit leads in this strategy.
  Worstfit performs best in this metric, with the least amount of small blocks. This is because it allocates higher memory blocks thus increasing the chance of the memoryblock being resused after release.


6) Eventually, the many mallocs and frees produces many small blocks scattered
across the memory pool.  There may be enough space to allocate a new block, but
not in one place.  It is possible to compact the memory, so all the free blocks
are moved to one large free block.  How would you implement this in the system
you have built? 

A: 
  //allocates the request at the end of any mymalloc function, 

  
  if we found no space for a request and there's, more or equal, space in free memory than requested
  we combine all memory blocks into one large block add it to the tail of the system.
  On each block we combine the previous block w. the following block. And add the size to a total.
  Once each block has been removed from the doubly link list, we create a struct and append it to the tail of the list.
  Lastly we call mymalloc again to assure the proper algorithm is called.

  //end of each algorithm
  if (mem_free() >= alloc(request) && next == NULL) { combine_alloc(); mymalloc(requested);}; 

7) If you did implement memory compaction, what changes would you need to make
in how such a system is invoked (i.e. from a user's perspective)?
  A:
    nothing changes for those using mymalloc(), it remains the same and more consistent (we can now free and call mymalloc as 
    many times as wanted)

8) How would you use the system you have built to implement realloc?  (Brief
explanation; no code)
  A:
    traverse the doubly linked list between w.limits of: from, from+size;
    reset each value in said address i.e. *(next->ptr) = 0; 

9) Which function(s) need to know which strategy is being used?  Briefly explain
why this/these and not others.
  A:
    mymalloc
      -calls appropriate algorithm.
      -Uses an enum, which is created from StrategyFromString

    strategy_name
      -returns name of strategy

    StrategyFromString
      - returns enum strategy, takes a char*      

10) Give one advantage of implementing memory management using a linked list
over a bit array, where every bit tells whether its corresponding byte is
allocated.
  -linked list is not contigious in memory
  -we can keep memory in different locations on the actual hardware (because it's not contigious)
  -virtual memory becomes easier to implement due to the flexibility of the linked list compared to a bit array
  -you cannot increase or decreate the size of each memoryblock for a bit array
