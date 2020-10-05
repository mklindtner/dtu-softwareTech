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
	next = head;
	while (1)
	{
		if (!next)
		{
			// printf("next is null, we done boiz\n");
			return NULL;
		}
		//next is always free
		if (next->size >= requested && next->alloc == 0)
		{
			// printf("\nallocating dat sweet memory ASS\n");
			int mem_diff = next->size - requested;
			// printf("next w. memory address: %p\n", next->ptr);
			struct memoryList *newMemBlock = (struct memoryList *)malloc(sizeof(struct memoryList));

			if (mem_diff == 0)
			{
				next->alloc = 1;
				return next->ptr;
			}
			//behavior: [left, next, right] -> [left, newMem, next, right]
			//behavior: [leftleft, left, next, right] -> [leftleft, left, newMem, next, right]

			struct memoryList *left = next->last;

			newMemBlock->last = left;
			newMemBlock->next = next;

			newMemBlock->ptr = next->ptr;
			newMemBlock->size = requested;

			newMemBlock->alloc = 1;

			next->last = newMemBlock;
			next->size -= requested;
			next->ptr = next->ptr + requested;
	
			//case: [NULL, next]
			if(left)
			{
				left->next = newMemBlock;
			}
			// printf("\naddr of next->ptr:%p\n", next->ptr);
			// printf("addr of newMemblokc->ptr:%p\n", newMemBlock->ptr);
			// printf("---\n");
			// print_memory();
			// printf("<=====>\n");
			return newMemBlock->ptr;
		}
		// next = next->next;
		next = next->last;
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
	}
	else
	{
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
//0x55ef27ad66e1
//0x55ef27ad6710

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

// static struct memoryList* releasememory(struct memoryList *next)
// {

// }

/* Frees a block of memory previously allocated by mymalloc. */
//mergesort
void myfree(void *block)
{
	// printf("addr to free: %p\n", block);
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
		next = next->last;
	}
	// printf("next:%p,setting next alloc to 0\n", next);
	next->alloc = 0;

	if (next->next && next->next->alloc == 0)
	{
		printf("next->next->alloc:%d\tptr-addr:%p\n", next->next->alloc, next->next->ptr);
		//print_memory();

		// struct memoryList *right = next->next;
		// struct memoryList *left = next->last;
		// right->last = left;
		// right->size += next->size;
		// right->ptr = next->ptr;
		// if (left)
		// {
		// 	left->next = right;
		// }
		//free(next);
	}

	if (next->last && next->last->alloc == 0)
	{
		printf("left is free");
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
	next = head;
	char contigiousBlock = 0;
	// char contigiousBlockAlloc = 0;
	int totalBlocks = 0;
	while (1)
	{
		if (next == NULL)
		{
			next = head;
			// printf("\n---total areas:%d--\n", totalBlocks);
			return totalBlocks;
		}
		// printf("next alloc:%d\n", next->alloc);
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
		next = next->last;
	}
	return 0;
}

/* Get the number of bytes allocated */
int mem_allocated()
{
	next = head;
	int size_allocated = 0;
	while (1)
	{
		if (next == NULL)
		{
			next = head;
			return size_allocated;
		}
		if (next->alloc == 1)
		{
			size_allocated += next->size;
		}
		next = next->last;
	}
	return -1;
}

/* Number of non-allocated bytes */
int mem_free()
{
	next = head;
	int size_unavailable = 0;
	while (1)
	{
		if (next == NULL)
		{
			next = head;
			return size_unavailable;
		}
		if (next->alloc == 0)
		{
			size_unavailable += next->size;
		}
		next = next->last;
	}
	return -1;
}

/* Number of bytes in the largest contiguous area of unallocated memory */
int mem_largest_free()
{
	int largestArea = 0;
	int tmp = 0;
	while (1)
	{
		if (next == NULL)
		{
			next = head;
			return largestArea;
		}

		if (next->alloc == 0)
		{
			if (++tmp > largestArea)
			{
				largestArea = tmp;
			}
		}
		else
		{
			tmp = 0;
		}
		next = next->last;
	}
	return -1;
}

/* Number of free blocks smaller than "size" bytes. */
int mem_small_free(int size)
{
	int smallest = size;
	int tmp = 0;
	while (1)
	{
		if (next == NULL)
		{
			next = head;
			return smallest;
		}

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
		next = next->last;
	}
	return -1;
}

char mem_is_alloc(void *ptr)
{
	next = head;
	while (1)
	{
		if (next == NULL)
		{
			return 0;
		}
		if (next->alloc == 1 && next->ptr == ptr)
		{
			return 1;
		}
		next = next->last;
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
			printf("alloc: %d\n", next->alloc);
			printf("size of current: %d\n", next->size);
			printf("ptr addr:%p\n", next->ptr);
			if (next->next)
			{
				printf("next->next->ptr:%p\n", next->next->ptr);
			}
			printf("-----\n");
		}
		else
		{

			break;
		}
		// printf("next ptr adr:%p\n", next->ptr);
		next = next->last;
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

	/* A simple example.  
	   Each algorithm should produce a different layout. */

	initmem(strat, 500);
	a = mymalloc(10);
	b = mymalloc(1);
	myfree(a);
	c = mymalloc(1);
	// a = mymalloc(100);
	// b = mymalloc(200);
	// c = mymalloc(100);
	// d = mymalloc(100);
	// myfree(a);
	// myfree(c);
	// printf("a mem-addr:%p\n", a);
	// printf("b mem-addr: %p\n", b);
	// printf("c addr:%p\n", c);
	// e = mymalloc(50);
	// myfree(e);

	print_memory();
	// print_memory_status();
}
