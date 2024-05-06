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
#include <assert.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
/* Project includes */
#include "HMM.h"

/************************************************  Main  ****************************************************/
/*
 * Memory allocation and deallocation test using custom memory management functions.
 * 
 * This test randomly allocates and frees memory blocks using the custom memory management functions:
 * - `My_malloc`: Allocates a block of memory.
 * - `My_free`: Frees a previously allocated memory block.
 * - `My_calloc`: Allocates and initializes a block of memory.
 * - `My_realloc`: Resizes a previously allocated memory block.
 * 
 * The test performs a specified number of iterations, randomly choosing between allocating or freeing memory.
 * It uses an array of pointers to track allocated memory blocks.
 * 
 * @note The custom memory management functions are not defined in this code snippet.
 * 
 * @note This code assumes that the custom memory management functions are implemented correctly.
 * 
 * @note The constants `NUM_ALLOCS`, `MAX_SIZE`, and `MAX_ITERATIONS` control the test parameters.
 * 
 * @note This code requires the inclusion of appropriate headers for the custom memory management functions.
 */
/*******************************************************************************************************************/
#if 0
void * malloc(size_t size)
  {
    return My_malloc(size);
  }

void free(void *ptr)
  {
    My_free(ptr);
  }

void *calloc(size_t nmemb, size_t size)
  {
    return My_calloc(nmemb , size);
  }
void *realloc(void *ptr, size_t size)
  { 
    return My_relloc(ptr , size);
  }
#endif
/****************************** Macros section *******************************************/
#define NUM_ALLOCS 10000
#define MAX_SIZE 102400
#define MAX_ITERATIONS 10000 /*milion random variables**/

/*
 * Performs a random allocation and deallocation test using the custom memory management functions.
 */
void random_alloc_free_test() 
{
    srand((unsigned int)time(NULL));
    void* pointers[NUM_ALLOCS] = {NULL};
    
    for ( int i =0; i < MAX_ITERATIONS; ++i) 
    {
        int index = rand() % NUM_ALLOCS;
        if (pointers[index] == NULL) 
        {
            // Allocate memory
            size_t size = (size_t)(rand() % MAX_SIZE) + 1;
            pointers[index] = malloc(size);
            if (pointers[index] != NULL) 
            {
                printf("Allocated memory of size %zu at address %p\n", size, pointers[index]);
            } else 
            {
                fprintf(stderr, "Allocation failed for size %zu\n", size);
            }
        } 
        else 
        {
                // Free remaining allocated memory
            printf("Freeing memory at address %p\n", pointers[index]);
            free(pointers[index]);
            pointers[index] = NULL;
        }
    }
    
    // Free remaining allocated memory
    for ( int i =0;i < NUM_ALLOCS; ++i) {
        if (pointers[i] != NULL) {
            printf("Freeing remaining memory at address %p\n", pointers[i]);
            free(pointers[i]);
            pointers[i] = NULL;
        }
    }
}

int main() 
{
    printf("Starting random allocation and deallocation test...\n");
    random_alloc_free_test();
    printf("Test complete.\n");
    return 0;
}
