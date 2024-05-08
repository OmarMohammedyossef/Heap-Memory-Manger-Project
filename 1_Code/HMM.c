/*******************************************************************************/
/******* Author    : Omar Elshamy               ********************************/
/******* Date      : 15 /4/ 2023                ********************************/
/******* Version   : 0.1                        ********************************/
/******* File Name : Heap memeory manager.c     ********************************/
/*******************************************************************************/
/***************************** Includes section ********************************/
/* System includes */
#include <assert.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <pthread.h>
#include <unistd.h>
/*****************************  Project includes  *************************************************/
#include "HMM.h"
static pthread_mutex_t alloc_mutex = PTHREAD_MUTEX_INITIALIZER;
/************************************** Enable if you want to replace libc malloc/free *****************************************/
#if 1
void * malloc(size_t size)
{
	if (pthread_mutex_lock(&alloc_mutex)!=0)
	{
        return NULL;
    }
    void *ptr = My_malloc(size);
    pthread_mutex_unlock(&alloc_mutex);
    return ptr;  
}

void free(void *ptr)
{
if (pthread_mutex_lock(&alloc_mutex)!=0)
{
    return;
}
My_free(ptr);
pthread_mutex_unlock(&alloc_mutex);
}

void *calloc(size_t nmemb, size_t size)
{
	if (pthread_mutex_lock(&alloc_mutex)!=0)
	{
        return NULL;
    }
    void *ptr = My_calloc(nmemb, size);
    pthread_mutex_unlock(&alloc_mutex);
    return ptr; 
	
}
void *realloc(void *ptr, size_t size)
{
	if (pthread_mutex_lock(&alloc_mutex)!=0)
	{
        return NULL;
    }
    void *ret_ptr = My_realloc(ptr, size);
    pthread_mutex_unlock(&alloc_mutex);
    return ret_ptr; 
}
#endif

/****************************** Macros section ****************************************************/
#define     _BSD_SOURCE
#define     Sbrk_up    (0x1000 * 33)
#define     Sbrk_down  (0x1000 * 32)

/****************************** Global variables section *******************************************/
meta_data_t *firet_p = NULL;
meta_data_t *current_program_break = NULL;


/***********************************  Functions Implementation  ************************************/

/*********************************** My_malloc Implementation **************************************/
 /**
 * @brief Allocates memory and returns a pointer to the allocated memory block.
 *
 * This function allocates memory of the specified size and returns a pointer to
 * the allocated memory block. It manages memory allocation by maintaining a
 * doubly linked list of memory blocks.
 *
 * @param size Size of the memory block to allocate.
 * @return void* Pointer to the allocated memory block, or NULL if allocation fails.
 */
void *My_malloc (size_t size)
{ 
	if (size < Minmum_size)
	{
		size= Minmum_size;
	}
	if (size >= Maxmum_size)
	{
		return NULL;
	}
   /* Align size to nearest multiple of 8 */
   size = (((size + 7) / 8) * 8);
   /* Handle the case of the first node in the list */
   if (firet_p == NULL)
      {
         /* Perform initial setup and allocation */
         state_t free_flag = False;
         firet_p = (meta_data_t *) sbrk (0);	/*  get the address of start of the heap  */
         while (((uint8_t *) firet_p + size_meta_data + size) > (uint8_t *) (current_program_break))	/*  extend the program preak to be above the size of data  */
	         {
	            current_program_break = (meta_data_t *) sbrk ( Sbrk_up);
	            current_program_break = (meta_data_t *) sbrk (0);
	            if (current_program_break == (void *) -1)
	               {
	                  return (void *)-1;
	               }
	         }
         if ((insert_beginning (&firet_p, size, False)) == False)	/*  creat first node in my list  */
	         {
	            return NULL;
	         }
         
         return ((uint8_t *) firet_p + size_meta_data);	/*  return to user the address of data to start allocating  */
      }
  else
      {
        /* Handle subsequent allocations */
        /**SEARCH ON FIRST FREE BLOCK*/
        meta_data_t *lastNode = NULL;
        meta_data_t *TempNode = NULL;
		meta_data_t *bastNode = NULL;
	    lastNode = firet_p;
	    while (NULL != lastNode)
	        {
	   	        if (lastNode->free_flag == True)
		               {
		                	if ((lastNode->size) == (size))
		            	   		{
									lastNode->free_flag = False;
		  	                		/*replace data only (return data adress to overwrite of previous data) */
		  	                 		return ((uint8_t *) lastNode + size_meta_data);
			            		}
		                 	else if ((lastNode->size) > (size))
			            	    {
                        		     /**best fit algroithm*/
      			        		    size_t temp_size=0;
                        		    size_t min_size=0;
                        		    temp_size= lastNode->size-size; 
                        		    if (min_size!=0)
                        		    {
                        		       if (min_size >temp_size)
                        		          {
                        		             min_size=temp_size;
                        		             bastNode  =lastNode;
                        		          }
                        		    }
                        		    else
                        		    {/**firest test*/
                        		       min_size=temp_size;
                        		       bastNode  =lastNode;
                        		    }
		  	            		    
		        	            }
		        	        else if ((lastNode->size) < (size))
		        	         {		/*do nothing and search on another node* */}
		        	        else
		        	           {  /**nothing**/}
		        	   }
		           else
		        	   {		/*node allocated => do Nothing and search on another node */}
		           /*save address of the last node */
		           if (lastNode->next_block == NULL)
		        	   	{
		        	     TempNode = lastNode;
		           		}
		           lastNode = lastNode->next_block;	/* traverse nodes */
		    }
		if (bastNode != NULL)	/*case of all nodes alocatted */
		    {
					/*  size of free node is above size neadded  */
		            /*split and allocate */
		        	split_node (&bastNode, size);
		        	return ((uint8_t *) bastNode + size_meta_data);	/*  return to user the address of data to start allocating  */
			}	
		if (lastNode == NULL)	/*case of all nodes alocatted */
		    {
		        /*insert new node in tail */
		        state_t free_flag = False;
	            while (((uint8_t *) TempNode + size_meta_data + TempNode->size +  size_meta_data + size) >(uint8_t *) (current_program_break))
		           {
		               current_program_break = (meta_data_t *) sbrk ( Sbrk_up);	/*  extend the program preak to be above the size of data  */
		               current_program_break = (meta_data_t *) sbrk (0);
		               if (current_program_break == (void *) -1)
		                   {
		                      return (void *)-1;
		                   }
		           }
		        TempNode =(meta_data_t *) ((uint8_t *) TempNode + TempNode->size +size_meta_data);
		        if (append_node ( size, free_flag) == False)	/*  creat new node in the tail in my list  */
		           {
		              return NULL;
		           }
		        return ((uint8_t *) TempNode + size_meta_data);
		    }
		     else {/* Nothing -> error */ }
      }
  return NULL;			/* Unable to allocate memory */
}

/************************************* My_free Implementation **********************************/
/**
 * @brief Memory deallocation function.
 * 
 * This function deallocates memory previously allocated by My_malloc function.
 * It traverses the list of memory blocks to find the block corresponding to the given pointer,
 * marks it as free, and potentially merges adjacent free blocks to optimize memory usage.
 * 
 * @param free_ptr Pointer to the memory block to deallocate.
 */
void My_free (void *free_ptr)
{
   meta_data_t *lastNode = NULL;
	            
   if (NULL != firet_p)
      {
	      lastNode = firet_p;
	      while (NULL != lastNode)
	         {
	            if (((uint8_t *)free_ptr - size_meta_data )== (uint8_t *)lastNode)
	            	/**cheack if pointer = start of any node*/
	            	{
	            	   if (lastNode->free_flag == False)
	            			/**free node not the first node*/
	            		   {
	            		      lastNode->free_flag = True;	/*  make the flag of node is free  */
	            		      merge_nodes ((meta_data_t *)(free_ptr - size_meta_data ));	/*cheack of makeing merge becouse the list have more than one node */
	            		      break;
	            		      /**free and merge is done insha'allah*/
	            		   }
	            	   else
	            		   {
							    break;
	            	         /**the node is already free*/
	            		   }
	            	}
	            else
	            	{/**not this node so=> search on next node*/}

	            lastNode = lastNode->next_block;	/* traverse nodes */
	         }
	   }
	else
	   {/**all nodes is free*/}

	uint32_t N_down = sdbrk_down ();
	if (N_down > 0)		/*  chaeck on the last node is a big size (above 32 page) to sbrk  down */
	   {
	      for (int i = 0; i < N_down; i++)
	         {
	            /*  sdbr down  by 32 page (as glibc) */
	            current_program_break = (meta_data_t *) sbrk (-Sbrk_down);
		        current_program_break = (meta_data_t *) sbrk (0);
				 
	      	}
      }
   else{ /**do nothing*/}
}

/*********************************** calloc Implementation *********************************/
/**
 * @brief Memory allocation function with zero initialization.
 * 
 * This function allocates memory for an array of elements, each of a specified size,
 * and initializes the memory to zero. It is similar to the standard library's calloc function.
 * 
 * @param element_number Number of elements to allocate.
 * @param element_size Size of each element in bytes.
 * @return void* Pointer to the allocated memory block if successful, NULL otherwise.
 */
void *My_calloc (size_t element_number, size_t element_size)
{
 	/* Check for special cases where number of elements or element size is zero */
 	if (element_number == 0 || element_size == 0)
 	  {
 	      /**allocate with minimum size*/
		  return My_malloc (Minmum_size);	/* Return pointer to the allocated memory */
 	  }
   else
 	  {
 	      /* Calculate total size needed for allocation */
 	      size_t total_size = element_size * element_number;
             /* Align size to nearest multiple of 8 */
		  total_size = (((total_size+7)/8)*8) ;
		 /* Check for overflow */
 	      if (total_size < 0)
	         {
	           return NULL;		/* Overflow detected */
	         }
 	       /* Allocate memory using My_malloc function */
 	      void *new_ptr = My_malloc (total_size);
         if (new_ptr != NULL)
	         {
	            /* Initialize allocated memory with zeros */
	            memset (new_ptr, 0, total_size);
	            return new_ptr;	/* Return pointer to the allocated memory */
	         }
 	      else
	         {			/*  if malloc can not  allocate -> return null */
	            return NULL;		/* Unable to allocate */
	         }
 	   }
   return NULL;			/* Unable to allocate */
}

/*********************************** realloc Implementation *********************************/
/**
 * @brief Reallocate memory for a previously allocated block or allocate a new block.
 * 
 * This function adjusts the size of the memory block pointed to by allocated_ptr.
 * If allocated_ptr is NULL, it behaves like My_malloc and allocates a new memory block.
 * If new_size is 0 and allocated_ptr is not NULL, it behaves like My_free and deallocates the memory block.
 * If allocated_ptr is not NULL and new_size is greater than zero, it adjusts the size of the memory block.
 * 
 * @param allocated_ptr Pointer to the previously allocated memory block, or NULL.
 * @param new_size New size for the memory block, in bytes.
 * @return void* Pointer to the reallocated or newly allocated memory block if successful, NULL otherwise.
 */
void * My_realloc (void *allocated_ptr, size_t new_size)
{
	 new_size = (((new_size+7)/8)*8) ;  /*align it to 8 bytes*/
   /* If allocated_ptr is NULL, allocate a new block with new_size */
	if (allocated_ptr == NULL)
	   {
         return  My_malloc (new_size); 
	   }
	/* If new_size is 0 and allocated_ptr is not NULL, free the block and return NULL */
   else if (new_size == 0 )
     {
	      My_free (allocated_ptr);
	      return  My_malloc (Minmum_size); 
     }
	   /* Adjust the size of the memory block */
	else
	   {
	      meta_data_t *tempNode = NULL;
	      if (NULL != firet_p)
	         {
	            tempNode = firet_p;
	            do
	               {
	                  /* Check if allocated_ptr points to the start address of a non-free node */
	                  if ((tempNode ==((meta_data_t *) ((uint8_t *)allocated_ptr - size_meta_data))) && (False ==(((meta_data_t *) ((uint8_t *)allocated_ptr -size_meta_data))->free_flag)))
	                  	{
	                  	   /* Check if new_size is smaller */
	                  	   if (new_size <(((meta_data_t *) ((uint8_t *)allocated_ptr - size_meta_data))->size))
	                  	      {		/* if new size is smaller than -> return old address and split the free space and make it new free node  */
	                  	         /*split and allocate */
	                  	         meta_data_t *newNode =(meta_data_t *) ((uint8_t *)allocated_ptr - size_meta_data);
	                  	         split_node (&newNode, new_size);
	                  	         return allocated_ptr;
	                  	      }
	                  	   /* Check if new_size is the same */
	                  	   else if (new_size ==(((meta_data_t *) ((uint8_t *)allocated_ptr -size_meta_data))->size))
	                  	      {		/* if new size is  the same size (no change) -> return old address  */
	                  	         return allocated_ptr;
	                  	      }
	                  	   /* Check if new_size is larger */
	                  	   else if (new_size >(((meta_data_t *) ((uint8_t *)allocated_ptr - size_meta_data))->size))
	                  	      {/* Check if there are next node in list */
                                 if(NULL!=(((meta_data_t *)((uint8_t *)allocated_ptr-size_meta_data))->next_block))	
                                 {/* Check if next node in free */
                                    if(True==(((meta_data_t *)((uint8_t *)allocated_ptr-size_meta_data))->next_block->free_flag))
                                    {/* Check if size of next node added to size of this node will be bigger than new size  */
                                       if(new_size<((((meta_data_t *)((uint8_t *)allocated_ptr-size_meta_data))->next_block->size)+size_meta_data+(((meta_data_t *)(allocated_ptr-size_meta_data))->size)))
                                          {
                                             meta_data_t *MyNode = (meta_data_t *)((uint8_t *)allocated_ptr-size_meta_data);
                                             meta_data_t *nextNode = MyNode->next_block;
                                             //merge
                                             MyNode->size =MyNode->size + MyNode->next_block->size + size_meta_data;
	                                          if (MyNode->next_block->next_block != NULL)
	                                             {
													meta_data_t *tmpNode = MyNode->next_block->next_block;
	                                                tmpNode->prev_block = MyNode;
	                                             }
	                                          MyNode->next_block = MyNode->next_block->next_block; 
                                          }
                                    }
                                 }
                                 else if(NULL!=(((meta_data_t *)((uint8_t *)allocated_ptr-size_meta_data))->prev_block))	
                                 {
                                    if (True==(((meta_data_t *)((uint8_t *)allocated_ptr-size_meta_data))->prev_block->free_flag))
                                    {
                                       if(new_size<((((meta_data_t *)((uint8_t *)allocated_ptr-size_meta_data))->prev_block->size)+size_meta_data+(((meta_data_t *)(allocated_ptr-size_meta_data))->size)))
                                          {
                                             meta_data_t *MyNode = (meta_data_t *)((uint8_t *)allocated_ptr-size_meta_data);
                                             meta_data_t *prevNode = MyNode->prev_block;
                                             //merge
                                             prevNode->size =prevNode->size + prevNode->next_block->size + size_meta_data;
	                                          if (prevNode->next_block->next_block != NULL)
	                                             {
													meta_data_t *tmpNode = prevNode->next_block->next_block;
	                                                tmpNode->prev_block = prevNode;
	                                             }
	                                          prevNode->next_block = prevNode->next_block->next_block;
						  memcpy(((uint8_t *)prevNode+size_meta_data),allocated_ptr,MyNode->size);
                                          }
                                    }
                                 }
                                 else
                                    {/* if new size is bigger than -> allocate new space and copy data from first space to new space and return new address   */
                                       void * new_ptr =My_malloc( new_size);
                                       if(new_ptr!= NULL)
                                             {
                                                memcpy(new_ptr,allocated_ptr,(( meta_data_t *)((uint8_t *)allocated_ptr-size_meta_data))->size);
                                                My_free(allocated_ptr);
                                             }
                                       return new_ptr;
                                    }
	                  	      }
	                  	   else{		/*errorr */ }
	            	      }
	                  else
	            		   {		/*adress not adress of this node  or adress of free block* */
	            		     return NULL;	/* Invalid address or free block */
	            		   }
	                  tempNode = tempNode->next_block;	/* traverse nodes */
	               }while (tempNode != NULL);
	         }
	      else  
            { /**all memory is free***/
	            return NULL;		/* Unable to allocate */
	         }
      }
}

/******************************************* The end *****************************************/
