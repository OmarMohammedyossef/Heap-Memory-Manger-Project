/*******************************************************************************/
/******* Author    : Omar Elshamy               ********************************/
/******* Date      : 15 /4/ 2023                ********************************/
/******* Version   : 0.1                        ********************************/
/******* File Name : Heap memeory manager.h     ********************************/
/*******************************************************************************/
#ifndef __HMM_H__
#define __HMM_H__
/***************************** Includes section ***************************/

#include "Doubly_free_list.h"
#define  Minmum_size 8
#define Maxmum_size 18446744073709551615UL

/***************************** Function  prototype section *************/

/*
 * Function: My_free
 * -------------------
 * Deallocates the memory pointed to by free_ptr.
 *
 * free_ptr: Pointer to the memory block to be deallocated.
 *
 * Returns: None
 */
void My_free (void *free_ptr);

/*
 * Function: My_malloc
 * --------------------
 * Allocates memory block of specified size.
 *
 * size: Size of the memory block to allocate.
 *
 * Returns: Pointer to the allocated memory block.
 */
void *My_malloc (size_t size);

/*
 * Function: My_calloc
 * --------------------
 * Allocates memory for an array of elements, initialized to zero.
 *
 * element_number: Number of elements to allocate.
 * element_size: Size of each element in bytes.
 *
 * Returns: Pointer to the allocated memory block.
 */
void *My_calloc (size_t element_number, size_t element_size);


/*
 * Function: My_relloc
 * --------------------
 * Resizes the memory block pointed to by allocated_ptr.
 *
 * allocated_ptr: Pointer to the memory block to be resized.
 * new_size: New size for the memory block.
 *
 * Returns: Pointer to the resized memory block.
 */
void *My_realloc (void *allocated_ptr, size_t new_size);

#endif /*__HMM_H__*/

 