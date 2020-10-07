#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "mymem.h"
#include <time.h>

/* The main structure for implementing memory allocation.
 * You may change this to fit your implementation.
 */

struct memoryList
{
	// doubly-linked list
	struct memoryList *last;
	struct memoryList *next;

	int size;	// How many bytes in this block?
	char alloc; // 1 if this block is allocated,
				// 0 if this block is free.
	void *ptr;	// location of block in memory pool.
};

strategies myStrategy = NotSet; // Current strategy

size_t mySize;
void *myMemory = NULL;

static struct memoryList *head;
static struct memoryList *next;
static struct memoryLIst *tail;

/* initmem must be called prior to mymalloc and myfree.

   initmem may be called more than once in a given exeuction;
   when this occurs, all memory you previously malloc'ed  *must* be freed,
   including any existing bookkeeping data.

   strategy must be one of the following:
		- "best" (best-fit)
		- "worst" (worst-fit)
		- "first" (first-fit)
		- "next" (next-fit)
   sz specifies the number of bytes that will be available, in total, for all mymalloc requests.
*/

void *first_fit(size_t requested)
{
	for(next = head; next != NULL; next=next->next)
	{
		if (next->size >= requested && next->alloc == 0)
		{
			int mem_diff = next->size - requested;

			// printf("next w. memory address: %p\n", next->ptr);
			struct memoryList *newMemBlock = (struct memoryList *)malloc(sizeof(struct memoryList));

			if (head == next)
			{
				head = newMemBlock;
			}

			//case: mem is exactly 0 on free block
			if (mem_diff == 0)
			{
				next->alloc = 1;
				return next->ptr;
			}
			//[head(free),left,next(free,8),right,free(next)] -> [head,left,newMem(1),next(free,7),right,free(next)]
			struct memoryList *left = next->last;

			//memBlock
			newMemBlock->next = next;
			newMemBlock->last = left;
			newMemBlock->alloc = 1;
			newMemBlock->size = requested;
			newMemBlock->ptr = next->ptr;

			//next
			next->last = newMemBlock;
			next->size -= requested;
			next->ptr += requested;

			//not in head
			if (left)
			{
				left->next = newMemBlock;
			}

			return newMemBlock->ptr;
		}
	}
	return NULL;
}

void initmem(strategies strategy, size_t sz)
{
	myStrategy = strategy;

	/* all implementations will need an actual block of memory to use */
	mySize = sz;

	if (myMemory != NULL)
		free(myMemory); /* in case this is not the first time initmem2 is called */

	/* TODO: release any other memory you were using for bookkeeping when doing a re-initialization! */
	/* TODO: Initialize memory management structure. */
	myMemory = malloc(sz);

	if (head == NULL)
	{
		head = malloc(sizeof(struct memoryList));
		tail = head;
	}
	else
	{
		next = head;
		while(next)
		{			
			struct memoryList* tmp = next->next;
			free(next);
			next = tmp;			
		}
		free(head);
		head = malloc(sizeof(struct memoryList)); //reset head
	}
	head->ptr = myMemory;
	head->size = sz;
	head->last = NULL;
	head->next = NULL;
	head->alloc = 0;
}

/* Allocate a block of memory with the requested size.
 *  If the requested block is not available, mymalloc returns NULL.
 *  Otherwise, it returns a pointer to the newly allocated block.
 *  Restriction: requested >= 1 
 */
void *mymalloc(size_t requested)
{
	assert((int)myStrategy > 0);

	switch (myStrategy)
	{
	case NotSet:
		return NULL;
	case First:
		return first_fit(requested);
	case Best:
		return NULL;
	case Worst:
		return NULL;
	case Next:
		return NULL;
	}
	return NULL;
}

/* Frees a block of memory previously allocated by mymalloc. */
//mergesort
void myfree(void *block)
{
	next = head;	
	while (1)
	{
		if (!next)
		{
			return;
		}
		if (next->ptr == block)
		{
			break;
		}
		next = next->next;
	}

	next->alloc = 0;

	if (next->next && next->next->alloc == 0)
	{
		struct memoryList *right = next->next;
		struct memoryList *left = next->last;
		right->last = left;
		right->size += next->size;
		right->ptr = next->ptr;
		if (left)
		{
			left->next = right;
		}

		//case: [newMem(free)(head), right(free)] -> [right(head)]
		if (head == next)
		{
			head = right;
		}
		free(next);
	}

	if (next->last && next->last->alloc == 0)
	{
		struct memoryList *right = next->next;
		struct memoryList *left = next->last;
		left->next = right;
		if (right)
		{
			right->last = left;
		}
		left->size += next->size;

		free(next);
	}

	return;
}

/****** Memory status/property functions ******
 * Implement these functions.
 * Note that when refered to "memory" here, it is meant that the 
 * memory pool this module manages via initmem/mymalloc/myfree. 
 */

/* Get the number of contiguous areas of free space in memory. */
int mem_holes()
{
	char contigiousBlock = 0;
	int totalBlocks = 0;
	for(next=head; next!=NULL; next=next->next)
	{
		if (!next->alloc)
		{
			if (!contigiousBlock)
			{
				totalBlocks += 1;
				contigiousBlock = 1;
			}
		}
		if (next->alloc && contigiousBlock)
		{
			contigiousBlock = 0;
		}
	}
	return totalBlocks;
}

/* Get the number of bytes allocated */
int mem_allocated()
{
	int size_allocated = 0;
	for(next=head; next!=NULL; next=next->next)
	{	
		if (next->alloc == 1)
		{
			size_allocated += next->size;
		}
	}
	return size_allocated;
}

/* Number of non-allocated bytes */
int mem_free()
{
	int size_unavailable = 0;
	for (next=head; next!=NULL; next=next->next)
	{
		if (next->alloc == 0)
		{
			size_unavailable += next->size;
		}
	}
	return size_unavailable;
}

/* Number of bytes in the largest contiguous area of unallocated memory */
int mem_largest_free()
{
	int largestArea = 0;
	for (next=head; next!=NULL; next=next->next)
	{
		if (next->alloc == 0)
		{
			if (next->size > largestArea)
			{
				largestArea = next->size;
			}
		}
	}
	return largestArea;
}

/* Number of free blocks smaller than "size" bytes. */
int mem_small_free(int size)
{
	int smallest = size;
	int tmp = 0;
	for (next=head; next!=NULL; next=next->next)
	{
		if (next->alloc == 0)
		{
			if (++tmp < smallest)
			{
				smallest = tmp;
			}
		}
		else
		{
			tmp = 0;
		}
	}
	return smallest;
}

char mem_is_alloc(void *ptr)
{
	for (next=head; next!=NULL; next=next->next)
	{
		if (next->alloc == 1 && next->ptr == ptr)
		{
			return 1;
		}
	}
	return 0;
}

/* 
 * Feel free to use these functions, but do not modify them.  
 * The test code uses them, but you may find them useful.
 */

//Returns a pointer to the memory pool.
void *mem_pool()
{
	return myMemory;
}

// Returns the total number of bytes in the memory pool. */
int mem_total()
{
	return mySize;
}

// Get string name for a strategy.
char *strategy_name(strategies strategy)
{
	switch (strategy)
	{
	case Best:
		return "best";
	case Worst:
		return "worst";
	case First:
		return "first";
	case Next:
		return "next";
	default:
		return "unknown";
	}
}

// Get strategy from name.
strategies strategyFromString(char *strategy)
{
	if (!strcmp(strategy, "best"))
	{
		return Best;
	}
	else if (!strcmp(strategy, "worst"))
	{
		return Worst;
	}
	else if (!strcmp(strategy, "first"))
	{
		return First;
	}
	else if (!strcmp(strategy, "next"))
	{
		return Next;
	}
	else
	{
		return 0;
	}
}

/* 
 * These functions are for you to modify however you see fit.  These will not
 * be used in tests, but you may find them useful for debugging.
 */

/* Use this function to print out the current contents of memory. */
void print_memory()
{
	printf("mem_holes:%d\n---\n", mem_holes());
	next = head;
	printf("\nhead adr:%p\n", next->ptr);
	while (1)
	{
		if (next)
		{
			struct memoryList *tmp = next;
			printf("alloc: %d\n", next->alloc);
			printf("size of current: %d\n", next->size);
			printf("ptr addr:%p\n", next->ptr);
			printf("correct_largest(89):%d\n", mem_largest_free());
			// printf("mem_free:%d", mem_free());
			// printf("mem_allocated:%d", mem_allocated());
			next = tmp;
			if (next->next)
			{
				printf("next->next->ptr:%p\n", next->next->ptr);
			}
			else
			{
				printf("no next->next\n");
			}
			printf("-----\n");
		}
		else
		{
			break;
		}
		// printf("next ptr adr:%p\n", next->ptr);
		// next = next->last;
		next = next->next;
	}
	return;
}

/* Use this function to track memory allocation performance.  
 * This function does not depend on your implementation, 
 * but on the functions you wrote above.
 */
void print_memory_status()
{
	printf("%d out of %d bytes allocated.\n", mem_allocated(), mem_total());
	printf("%d bytes are free in %d holes; maximum allocatable block is %d bytes.\n", mem_free(), mem_holes(), mem_largest_free());
	printf("Average hole size is %f.\n\n", ((float)mem_free()) / mem_holes());
}

/* Use this function to see what happens when your malloc and free
 * implementations are called.  Run "mem -try <args>" to call this function.
 * We have given you a simple example to start.
 */
void try_mymem(int argc, char **argv)
{
	strategies strat;
	void *a, *b, *c, *d, *e;
	if (argc > 1)
		strat = strategyFromString(argv[1]);
	else
		strat = First;

	int inst_mem = 10000;
	
	initmem(strat, inst_mem);
	print_memory();
	// for(int i = 0; i < inst_mem;i++)
	// {
	// 	myalloc(i);
	// }
	// for(int i = 0; i < inst_mem;i++)
	// {
	// 	myfree();
	// }
	// a = mymalloc(10);
	// // b = mymalloc(1);
	// myfree(a);
	// c = mymalloc(1);
	initmem(strat, 8000);
	print_memory();

	initmem(strat, 7000);
	print_memory();
	
	initmem(strat, 6000);
	print_memory();
}
