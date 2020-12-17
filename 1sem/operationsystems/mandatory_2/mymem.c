#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "mymem.h"
#include <time.h>

strategies myStrategy = NotSet; // Current strategy

size_t mySize;
void *myMemory = NULL;

static struct memoryList *head;
static struct memoryList *next;

//ptr for nextfit
static struct memoryList *roving;


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
	//cleanup any memoryList structs that are unused
	if (head != NULL)
	{
		next = head;
		while (next)
		{
			memoryList *tmp = next->next;
			free(next);
			next = NULL;
			next = tmp;
		}
		head = NULL;
	}
	//reset head and roving
	head = (memoryList *)malloc(sizeof(memoryList)); 
	roving = head;
	head->ptr = myMemory;
	head->size = sz;
	head->last = NULL;
	head->next = NULL;
	head->alloc = 0;
}

//find the memblock furthest from the correct size
void *worst_fit(size_t requested)
{
	int mem_size = -1;
	memoryList *biggest = NULL;
	for (next = head; next != NULL; next = next->next)
	{
		//if ll is taken, move on
		if (next->size < requested || next->alloc == 1)
			continue;
		//first time assignment
		if (mem_size == -1)
		{
			biggest = next;
			mem_size = next->size;
		}
		else if (mem_size < next->size)
		{
			biggest = next;
			mem_size = next->size;
		}
	}

	//if no blocks were available, the function cannot allocate the requested memory
	if (!biggest)
	{
		return NULL;
	}

	return allocate_memblock_left(biggest, requested);
}

//find the memoryblock closest to the correct size
void *best_fit(size_t requested)
{
	int best_fit_mem = -1;
	memoryList *best_fit = NULL;

	for (next = head; next != NULL; next = next->next)
	{

		int mem_diff = next->size - requested; 
		//we reject to allocate a block if we don't have enough spaec or it's already allocated
		if (mem_diff < 0 || next->alloc == 1 || next->size < requested)
		{
			continue;
		}

		//perfect fit
		if (mem_diff == 0)
		{
			next->alloc = 1;
			return next->ptr;
		}

		//first available block big enough
		if (best_fit_mem == -1)
		{
			best_fit = next;
			best_fit_mem = mem_diff;
		}
		else if (best_fit_mem > mem_diff) //mem_diff is less but not 0
		{
			best_fit = next;
			best_fit_mem = mem_diff;
		}
	}

	if (!best_fit)
	{
		return NULL;
	}
	return allocate_memblock_left(best_fit, requested);
}

void *next_fit(size_t requested)
{
	memoryList *startPoint = roving;
	//go from rover ptr to end	
	for (next = roving; next != NULL; next = next->next)
	{
		//reverse from earlier, now we only consider if it's correct
		if (next->size >= requested && next->alloc == 0)
		{
			void *memptr = allocate_memblock_left(next, requested);
			
			//go back to head, last slot fit perfectly
			if (next->ptr == memptr && next->next == NULL) 
			{
				roving = head;
			}
			else if (next->ptr == memptr) //memblock fit perfecty
			{
				roving = next;
			}
			else //default
			{
				roving = next->last;
			}
			return memptr;
		}
	}
	//go from start to roving ptr
	for (next = head; next != startPoint; next = next->next)
	{
		if (next->size >= requested && next->alloc == 0)
		{
			void *memptr = allocate_memblock_left(next, requested);
			//if block fits exactly
			if (next->ptr == memptr)
				roving = next;
			else
				roving = next->last;
			return memptr;
		}
	}
	return NULL;
}

//find the first available block
void *first_fit(size_t requested)
{
	for (next = head; next != NULL; next = next->next)
	{
		if (next->size >= requested && next->alloc == 0)
		{
			return allocate_memblock_left(next, requested);
		}
	}
	return NULL;
}

//allocates a block to the left of next, reassigns head, if head is next.
void *allocate_memblock_left(struct memoryList *next, size_t requested)
{
	int mem_diff = next->size - requested;

	if (mem_diff == 0)
	{
		next->alloc = 1;
		return next->ptr;
	}

	memoryList *newMemBlock = (memoryList *)malloc(sizeof(memoryList));

	//case: alloc(5): [0(head,free,6),1(tail,free)] -> [1(head,5),0(free,1),1(tail,free)]
	//case:first time allocation, newMemBlock becomes head
	if (head == next)
	{
		head = newMemBlock;
	}

	memoryList *left = next->last;

	//newMemblock is moved to the left of next, attributes is set accordingly
	newMemBlock->next = next;
	newMemBlock->last = left;
	newMemBlock->alloc = 1;
	newMemBlock->size = requested;
	newMemBlock->ptr = next->ptr;

	next->last = newMemBlock;
	next->size -= requested;
	next->ptr += requested;

	//case: not in head, left should point to the newMemblock
	if (left)
	{
		left->next = newMemBlock;
	}

	return newMemBlock->ptr;
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
		return best_fit(requested);
	case Worst:
		return worst_fit(requested);
	case Next:
		return next_fit(requested);
	}
	return NULL;
}

/* Frees a block of memory previously allocated by mymalloc. */
void myfree(void *block)
{
	char freeleft = 0, freeright = 0;
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
	memoryList *tofree = next;

	//buddysystem: if right is also 0 they are combined
	if (next->next && next->next->alloc == 0)
	{
		memoryList *right = next->next;
		memoryList *left = next->last;
		right->last = left;
		right->size += next->size;
		right->ptr = next->ptr;

		//case: roving is standing on removed memoryBlock
		roving = right;

		//case: a memblock exists to the left of next
		if (left)
		{
			left->next = right;
		}

		//case: [newMem(free)(head), right(free)] -> [right(head)]
		if (head == next)
		{
			head = right;
		}
		freeright = 1;
		next = right;
	}

	//buddysystem: if left is also 0 they are combined
	if (next->last && next->last->alloc == 0)
	{
		memoryList *right = next->next;
		memoryList *left = next->last;
		left->next = right;

		//case: roving is standing on removed memoryBlock
		roving = left;

		//case: a memblock exists to the right of next
		if (right)
		{
			right->last = left;
		}
		left->size += next->size;
		freeleft = 1;
	}

	//nothing points to tofree, it now points nowhere and should be released 
	if (freeleft || freeright)
	{
		free(tofree);
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
	for (next = head; next != NULL; next = next->next)
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
	for (next = head; next != NULL; next = next->next)
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
	for (next = head; next != NULL; next = next->next)
	{
		if (next->alloc == 0)
		{
			size_unavailable += next->size;
		}
	}
	return size_unavailable;
}

int mem_largest_free()
{
	int largestArea = 0;
	for (next = head; next != NULL; next = next->next)
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
//To fix next: simply return 1 (or figure out what is wrong .. ???)
int mem_small_free(int size)
{
	int blocks_found = 0;
	for (next = head; next != NULL; next = next->next)
	{
		// printf("\n next-ptr:%p",next->ptr);
		if (next->alloc == 0 && next->size <= size)
		{
			++blocks_found;
		}
	}
	return blocks_found;
}

char mem_is_alloc(void *ptr)
{
	for (next = head; next != NULL; next = next->next)
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

	FILE *log;
	log = fopen("tests.log", "a");

	next = head;
	while (1)
	{
		if (next)
		{
			// fprintf(log,"\n------\n");
			memoryList *tmp = next;
			// fprintf(log,"alloc: %d\n", next->alloc);
			// fprintf(log,"size of current: %d\n", next->size);
			// fprintf(log,"ptr addr:%p\n", next->ptr);
			// fprintf(log,"mem_holes:%d\n", mem_holes());

			fprintf(log, "next->ptr:%p\n", next);
			fprintf(log, "roving->ptr:%p\n", roving->ptr);
			next = tmp;
		}
		else
		{
			fprintf(log, "============\n");
			fprintf(log, "summary\n");
			fprintf(log, "mem free:%d\n", mem_free());
			fprintf(log, "free largest:%d\n", mem_largest_free());
			fprintf(log, "head is start:%d\n", mem_pool() == head->ptr);
			fprintf(log, "head addr:%p\n", head->ptr);
			fprintf(log, "roving ptr:%p\n", roving->ptr);
			fprintf(log, "============\n");
			break;
		}
		next = next->next;
	}
	fclose(log);
	return;
}

void print_memory_terminal()
{
	next = head;
	while (1)
	{
		if (next)
		{
			printf("\n------\n");
			memoryList *tmp = next;
			printf("alloc: %d\n", next->alloc);
			printf("size of current: %d\n", next->size);
			printf("ptr addr:%p\n", next->ptr);
			printf("mem_holes:%d\n", mem_holes());
			printf("smaller than 9: %d\n", mem_small_free(9));

			printf("rowing memory block: %p\n", roving->ptr);
			next = tmp;
			if (next->next)
			{
				printf("next->next->ptr:%p\n", next->next->ptr);
			}
		}
		else
		{
			printf("============\n");
			printf("summary\n");
			printf("mem free:%d\n", mem_free());
			printf("free largest:%d\n", mem_largest_free());
			printf("head is first:%d\n", mem_pool() == head->ptr);
			printf("head addr:%p\n", head->ptr);
			printf("rowing memory block: %p\n", roving->ptr);
			printf("============\n");
			break;
		}
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
	// void *a, *b, *c, *d, *e, *f, *g;
	// void *first, *second, *third;
	if (argc > 1)
		strat = strategyFromString(argv[1]);
	else
		strat = First;

	initmem(strat, 10000);
	// a = mymalloc(100);
	// b = mymalloc(100);
	// myfree(a);
	// mymalloc(100);

	print_memory_terminal();
}
