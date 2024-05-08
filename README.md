## Heap Memory Management Library - README.md

This project provides a comprehensive memory management solution for Linux systems. It includes various components and utilities to efficiently manage memory allocation and deallocation, optimizing memory usage in applications.
It offers functions for common memory management operations:
    void *My_malloc (size_t size): Allocates a block of memory of the specified size on the heap.
    void My_free (void *free_ptr): Deallocates a previously allocated memory block pointed to by ptr.
    void *My_calloc (size_t element_number, size_t element_size): Allocates memory for an array of nmemb elements of size size and initializes all elements to zero.
    void * My_realloc (void *allocated_ptr, size_t new_size): Resizes a previously allocated memory block pointed to by ptr to the new size size.


# Features:
Efficient Memory Management: Utilizes a doubly linked list to track free memory blocks, enabling efficient          allocation and deallocation.
Reduced Fragmentation: Implements strategies like splitting large free blocks to minimize external fragmentation and improve memory utilization.
Automatic SBRK Calls: Expands the program break with a large block when necessary to allocate memory for requests that exceed the available free space.


### Makefile Overview

The provided Makefile defines the following targets:

* **all:** This is a convenience target that calls `clean` , `build` and `run` sequentially.
* **clean:** Removes all object files (`.o`), static library (`.a`), and shared library (`.so`) generated during compilation.
* **build:**
    * Compiles the `HMM.c`, `Doubly_Free_list.c` source file, creating object files.
    * Compiles the `HMM.c`, `Doubly_Free_list.c` source file with dynamic linking, creating the shared library `libhmm.so`.

**static:**
    * Compiles the `HMM.c`, `Doubly_Free_list.c` source file with static linking, creating object files.
    * Creates the static library `libhmm.a` from the object files.
    * Compiles the `HMM.c`, `Doubly_Free_list.c` source file with dynamic linking, creating the shared library `libhmm.so`.

 **dynamic:**
    * Sets the `LD_PRELOAD` environment variable to point to the `libhmm.so` library.
    * Provides instructions on running your programs to test the library.
    * Suggests testing various commands like  `nano`, `ls`, `cat`, `echo`, etc.


### Usage

1. **Build the library:**
<<<<<<< HEAD
   ```
=======

   ```bash
>>>>>>> 28080e009876c2044c57411cc945987dc22feefc
   make 
   ```

2. **Build static library:**
<<<<<<< HEAD
   ```
=======

  
   ```bash
>>>>>>> 28080e009876c2044c57411cc945987dc22feefc
   make static
   ```

3. **Build static library:**
<<<<<<< HEAD
   ```
   make synamic
=======

   
   ```bash
   make dynamic
>>>>>>> 28080e009876c2044c57411cc945987dc22feefc
   ```



# Testing:
Implement some test scripts and other testing strategies to ensure the correctness and robustness of my memory manager code and finally i replaced the glibc allocator with our custom solution. The results were truly impressive, ls and echo run with my heap memory manager 
