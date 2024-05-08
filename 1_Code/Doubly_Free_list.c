/*******************************************************************************/
/******* Author    : Omar Elshamy               ********************************/
/******* Date      : 15 /4/ 2023                ********************************/
/******* Version   : 0.1                        ********************************/
/******* File Name : Doubly_free_list.c         ********************************/
/*******************************************************************************/
/***************************** Includes section ***************************/
/* System includes */
#include <assert.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
/* Project includes */
#include "Doubly_free_list.h"
/***************************** Global variable section *************/
meta_data_t *listHead = NULL;	/*sheared recource of head of list* */

 /*************************************************************************/
 /**
 * @brief Inserts a memory block into the doubly linked list at a specified index.
 *
 * This function inserts a new memory block into the doubly linked list
 * at the specified index, updating the metadata accordingly.
 *
 * @param list Pointer to the pointer of the head of the doubly linked list.
 * @param block_size Size of the memory block to be inserted.
 * @param index Index where the memory block should be inserted.
 * @param free_flag Flag indicating if the block is free or not.
 * @return state_t Success or failure state.
 */
void insert_node (meta_data_t ** list, size_t block_size, meta_data_t * prev, state_t free_flag)
{
  meta_data_t *newNode = NULL;
  newNode = (meta_data_t *) * list;
  if (NULL != newNode)
    {
      if (NULL == prev)
        {
          /* Insert at beginning only when index = 0 */
          insert_beginning (list, block_size, free_flag);
        }
      else
        {
          
          if (NULL != newNode)
          {
          	newNode->size = block_size;
          	newNode->free_flag = free_flag;
          	newNode->prev_block = prev;
          	newNode->next_block = prev->next_block;
          	prev->next_block = newNode;
          }
          else
          {/**/}
        }
    }
  else
    {/**node is free**/}
}

/*************************************************************************/
/**
 * @brief Inserts a memory block at the beginning of the doubly linked list.
 *
 * This function inserts a new memory block at the beginning of the doubly
 * linked list, updating the metadata accordingly.
 *
 * @param list Pointer to the pointer of the head of the doubly linked list.
 * @param block_size Size of the memory block to be inserted.
 * @param free_flag Flag indicating if the block is free or not.
 * @return state_t Success or failure state.
 */
state_t insert_beginning (meta_data_t ** list, size_t block_size, state_t free_flag)
{
  state_t ret = True;
  meta_data_t *newNode = NULL;
  if (NULL != list)
    {
      newNode = (meta_data_t *) * list;
      listHead = newNode;
      if (NULL != newNode)
        {
          newNode->size = block_size;
          newNode->free_flag = 0;
          newNode->prev_block = NULL;
          newNode->next_block = NULL;
        }
      else
        {
          ret = False;
        }
    }
  else
    {
      ret = False;
    }
  return ret;
}

/*************************************************************************/
/**
 * @brief Appends a memory block to the end of the doubly linked list.
 *
 * This function appends a new memory block to the end of the doubly linked
 * list, updating the metadata accordingly.
 *
 * @param list Pointer to the pointer of the head of the doubly linked list.
 * @param new_list Pointer to the pointer of the new memory block to be appended.
 * @param block_size Size of the memory block to be appended.
 * @param free_flag Flag indicating if the block is free or not.
 * @return state_t Success or failure state.
 */
/**can be oprmized by delete **list and **new_list*/
state_t append_node ( size_t block_size, state_t free_flag)
{
  state_t ret = True;
  meta_data_t *tempNode = NULL;
  meta_data_t *newNode = NULL;
  if (NULL != listHead)
    {
      tempNode = listHead;
      while (tempNode->next_block != NULL)
      {
        tempNode = tempNode->next_block;
      }
      newNode = (meta_data_t *) ((uint8_t *) tempNode + tempNode->size +size_meta_data);;
      newNode->size = block_size;
      newNode->free_flag = free_flag;
      newNode->next_block = NULL;
      newNode->prev_block = tempNode;
      tempNode->next_block = newNode;
    }
  else
    {
      ret = False;
    }
  return ret;
}

/*************************************************************************/
/**
 * @brief Splits a memory block into two at a specified index.
 *
 * This function splits the memory block at the specified index into two blocks
 * with the new size, updating the metadata accordingly. It then inserts the
 * new block into the doubly linked list.
 *
 * @param list Pointer to the head of the doubly linked list.
 * @param old_size Original size of the memory block.
 * @param new_size New size for the memory block after splitting.
 * @param index Index where the split should occur.
 * @return state_t Success or failure state.
 */
void split_node (meta_data_t ** list, size_t new_size)
{
  meta_data_t *tempNode = *list;
  meta_data_t *prevNode = NULL;
  /*edit the meta data of first block **/
  size_t old_size = tempNode->size;
  tempNode->size = new_size;
  tempNode->free_flag = False;
  prevNode = tempNode;
  tempNode =
    (meta_data_t *) ((uint8_t *) tempNode + size_meta_data + new_size);
  if ((old_size - new_size) >= size_meta_data)
    {
      /*path insart_node that this node is free* */
      insert_node (&tempNode, (old_size - new_size - size_meta_data), prevNode, True);
    }
  else
    {
  		/**free size  is less than size of meta data*/
    }
}

/*************************************************************************/
/**
 * @brief Merges adjacent free memory blocks in the doubly linked list.
 *
 * This function traverses the doubly linked list, merging adjacent free memory
 * blocks into a single block, updating metadata accordingly.
 *
 * @param list Pointer to the head of the doubly linked list.
 * @return uint32_t Total number of merged blocks.
 */
void merge_nodes (meta_data_t * free_node)
{
      if (free_node->next_block != NULL)
        {/*check on the next node*/
          if (free_node->free_flag == True && free_node->next_block->free_flag == True)
          	{			/*Merge with next */
                meta_data_t *after_Node = NULL;
            	  free_node->size = free_node->size + free_node->next_block->size + size_meta_data;
            	  if (free_node->next_block->next_block != NULL)
            	  {
                   after_Node = free_node->next_block->next_block;
            	     after_Node->prev_block = free_node;
            	  }
            	  free_node->next_block = after_Node;
          	}
          	else	{	 /**test the next node*/  }
        }
      else if (free_node->prev_block != NULL)
        {		/*check on the previous node*/	
          if (free_node->free_flag == True && free_node->prev_block->free_flag == True)
          	{			/*Merge with previous */
            	  meta_data_t *after_Node = NULL;
                meta_data_t *prev_Node = free_node->prev_block;
            	  prev_Node->size = prev_Node->size +free_node->size + size_meta_data;
            	  if (free_node->next_block != NULL)
            	  {
                   after_Node = free_node->next_block;
            	     after_Node->prev_block = prev_Node;
            	  }
            	  prev_Node->next_block = after_Node;
          	}
          	else	{	 /** the prev node is blocked*/  }
        }
      else { /*no merge*/  } 
}

/*************************************************************************/
/**
 * @brief Moves the program break downwards to release memory.
 *
 * This function moves the program break downwards to deallocate memory if the
 * last block in the doubly linked list is free and its size is greater than
 * 32 times the page size.
 *
 * @param ptr Pointer to the memory block to be deallocated.
 * @return state_t Success or failure state.
 */
uint32_t sdbrk_down ()
{
  uint32_t ret = 0;
  meta_data_t *lastNode = NULL;
  if (NULL != listHead)
    {
      lastNode = listHead;
      while (NULL != lastNode->next_block)
        {
          lastNode = lastNode->next_block;
        }
      if (lastNode->free_flag == True)
        {
          while ((lastNode->size) > (0x1000 * 32))
            {
        	    ret++;
      	      lastNode->size = lastNode->size - (0x1000 * 32);
            }
        }
      else
      {
        ret = 0;
      }
    }
  else
  {
    ret = 0;
  }
  return ret;
}

/*************************************************************************/
