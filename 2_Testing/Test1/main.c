/*******************************************************************************/
/******* Author    : Omar Elshamy               ********************************/
/******* Date      : 15 /4/ 2023                ********************************/
/******* Version   : 0.1                        ********************************/
/******* File Name : main.c                     ********************************/
/*******************************************************************************/
/***************************** Includes section ********************************/
/* System includes */
/***************************** Includes section ********************************/
/* System includes */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* Project includes */
#include "HMM.h"
#define NUM_OPERATIONS 100000

#define MAX_SIZE (10000)

enum Operation
{
    MALLOC,
    CALLOC,
    REALLOC,
    FREE
};

void *allocated_blocks[NUM_OPERATIONS];

void perform_random_operations()
{

    srand(time(NULL));

    for (int i = 0; i < NUM_OPERATIONS; i++)
    {
        enum Operation op = rand() % 4;
        switch (op)
        {
        case MALLOC:
        {
            size_t size = rand() % MAX_SIZE + 1;
            allocated_blocks[i] = malloc(size);
            printf("Allocated block %p with size %zu\n", allocated_blocks[i], size);
            break;
        }
        case CALLOC:
        {
            size_t num = rand() % MAX_SIZE + 1;
            size_t size = rand() % MAX_SIZE + 1;
            allocated_blocks[i] = calloc(num, MAX_SIZE/num);
            printf("Allocated block %p with %zu elements of size %zu\n", allocated_blocks[i], num, size);
            break;
        }
        case REALLOC: {
            if (i > 0) {
                int idx = rand() % i;
                size_t size = rand() % MAX_SIZE + 1;
                allocated_blocks[idx] = realloc(allocated_blocks[idx], size);
                printf("Reallocated block %p with size %zu\n", allocated_blocks[idx], size);
            }
            break;
        }
        case FREE:
        {
            if (i > 0)
            {
                int idx = rand() % i;
                free(allocated_blocks[idx]);
                allocated_blocks[idx] = NULL;
                printf("Freed block %p\n", allocated_blocks[idx]);
            }
            break;
        }
        }
    }
}


int main()
{    printf("before PK = %p\n",sbrk(0));;
    perform_random_operations();
    printf( " after PK = %p\n",sbrk(0));
    printf("DONE\n");
    return 0;
}
