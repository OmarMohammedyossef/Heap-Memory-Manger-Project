/*******************************************************************************/
/******* Author    : Omar Elshamy               ********************************/
/******* Date      : 15 /4/ 2023                ********************************/
/******* Version   : 0.1                        ********************************/
/******* File Name : Doubly_free_list.h         ********************************/
/*******************************************************************************/
#ifndef __DOUBLY_FREE_LIST_H__
#define __DOUBLY_FREE_LIST_H__
/***************************** Includes section ***************************/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


/***************************** user defined data type section *************/
/**
 * @brief Enumeration representing boolean states.
 *
 * This enumeration defines boolean states False and True, which are commonly
 * used to indicate success or failure in various functions and conditions.
 */
typedef enum
{
  False = 0,   /**Success**/
  True = 1     /**Fail   **/
} state_t;

/**
 * @brief Structure defining metadata for managing memory blocks.
 *
 * This structure contains metadata information for managing memory blocks
 * in a doubly linked list structure. It includes fields such as the free flag,
 * size of the block, and pointers to the previous and next memory blocks.
 */
typedef struct meta_data
{
  state_t free_flag;		   /**< Flag indicating if the block is free or not. */
  size_t size;		   /**< Size of the memory block. */
  struct meta_data *prev_block;	   /**< Pointer to the previous memory block. */
  struct meta_data *next_block;	   /**< Pointer to the next memory block. */
} meta_data_t;

/****************************** Macros section *******************************************/
#define MM_MAX_STRUCT_NAME 32

/****************************** Macros like function section ****************************/
/*
 * Macro to retrieve the metadata of the next block based on the current block's size.
 */
#define NEXT_META_BY_SIZE(meta_data_ptr) \
    ((meta_data_t *)((uintptr_t)(meta_data_ptr) + (meta_data_ptr)->size))

/*
 * Macro to get the pointer to the next metadata block.
 */
#define NEXT_META_BLOCK(meta_data_ptr) \
   ((meta_data_ptr)->next_block)

/*
 * Macro to get the pointer to the previous metadata block.
 */
#define PREV_META_BLOCK(meta_data_ptr) \
   ((meta_data_ptr)->prev_block)

/*
 * Macro to get the size of the metadata block.
 */
#define   size_meta_data   sizeof(meta_data_t)

/*****************************  functions interface *************/
/*
 * @brief : insert a node anywhere in the list.
 * @param : pointer to the previous node, double pointer to the new node, size of this node and free flag of it.
 * @retval: return nothing.
*/
void insert_node (meta_data_t ** list, size_t block_size, meta_data_t * prev, state_t free_flag);

/*
 * @brief : insert a node at the head of the list. 
 * @param : double pointer to the list, size of this node and free flag of it.
 * @retval: return status type.
 */
state_t insert_beginning (meta_data_t ** list, size_t block_size, state_t free_flag);

/*
 * @brief : insert a node at the end of the list. 
 * @param : double pointer to the list, size of this node and free flag of it.
 * @retval: return status type.
 */
state_t append_node (  size_t block_size, state_t free_flag);


/*
 * @brief : split the node into two nodes.
 * @param : single pointer to the node and new size of this node.
 * @retval: return nothing.
 */
void split_node (meta_data_t ** list, size_t new_size);

/*
 * @brief : merge the free nodes into one nodes.
 * @param : nothing.
 * @retval: return nothing.
 */
void merge_nodes (meta_data_t * free_node);


/*
 * @brief : check on size of last node in data block.
 * @param : single pointer to the head node.
 * @retval: return status type.
 */
uint32_t sdbrk_down ( );


/****************************** Th End ************************************/
#endif /* __DOUBLY_FREE_LIST_H__ */
