/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
*	
* This file is part of Open Source Doubango Framework.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*	
* DOUBANGO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsk_memory.c
 * @brief Useful memory management functions to handle memory.
 * As I'm a lazy man, some definition come from this <ahref="http://www.cplusplus.com">this website</a>
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tsk_memory.h"

#include <stdarg.h>
#include <ctype.h>
#include <stdio.h>

/**@defgroup tsk_memory_group Memory management utils
*/


/**@page tsk_memory_page Memory management Tutorial
* <H2>== Memory allocation using Heap mechanism ==</H2>
* Allocating memory using heap mechanism is not mandatory but is useful. This mechanism is useful because
* all newly allocated memories are holded by the heap and could be freed when for example your progam exit.
* The main goal is to avoid memory leaks. Here is an example of how to use this feature:
*
* @code
char* mystr = 0;
tsk_heap_t heap;

// initialize our memory heap => MANDATORY before using the heap
tsk_heap_init(&heap);
for(i=0; i<10;i++)
{
	mystr = tsk_strdup(&heap, "testing the heap (1)");
	... use the string ...

	mystr = (char*)tsk_malloc(&heap, 10);
	mystr = tsk_realloc(&heap, mystr, 100);
	... use the string ...
}
// This call will free all allocated strings (20 in our case)
tsk_heap_cleanup(&heap);

// This call will free one memory block
tsk_free(&heap, &test);
*@endcode
*
* <H2>== Memory allocation without Heap mechanism ==</H2>
* All tsk_[name] functions could be replaced by [name]. For example:
* replace tsk_[strdup] by [strdup].
*
* * @code
char* mystr = 0;

// initialize our memory heap => MANDATORY before using the heap
for(i=0; i<10;i++)
{
	mystr = tsk_strdup(0, "testing the heap (1)");
	... use the string ...
	tsk_free(0, mystr);

	mystr = (char*)tsk_malloc(0, 10);
	mystr = tsk_realloc(0, mystr, 100);
	... use the string ...
	tsk_free(0, mystr);
}
*@endcode
*/

/**@ingroup tsk_memory_group
* Predicate function used to retrive an address by ref
*/
static int tsk_memory_find_by_address(const tsk_heap_address_t* item, const void* address)
{
	return (item->data == address) ? 1 : 0;
}

/**@ingroup tsk_memory_group
* Allocates a block of size bytes of memory, returning a pointer to the beginning of the block.
* The content of the newly allocated block of memory is not initialized, remaining with indeterminate values.
* @param heap The memory heap from which to free @a ptr. Set to NULL if you don't want to use heap allocation mechanism.
* @param size Size of the memory block, in bytes.
* @retval On success, a pointer to the memory block allocated by the function.
* You MUST call @a tsk_free to free the newly allocated memory.
*/
void* tsk_malloc(tsk_heap_t *heap, size_t size)
{
	void *ret = malloc(size);
	HEAP_PUSH(heap, ret);

	return ret;
}

/**@ingroup tsk_memory_group
* Reallocate memory block
* In case that ptr is NULL, the function behaves exactly as @a tsk_malloc, assigning a new block of size bytes and returning a pointer to the beginning of it.
* The function may move the memory block to a new location, in which case the new location is returned. The content of the memory block is preserved up to the lesser of the 
* new and old sizes, even if the block is moved. If the new size is larger, the value of the newly allocated portion is indeterminate.
* In case that the size is 0, the memory previously allocated in ptr is deallocated as if a call to free was made, and a NULL pointer is returned.
* @param heap The memory heap from which to free @a ptr. Set to NULL if you don't want to use heap allocation mechanism.
* @param ptr Pointer to a memory block previously allocated with malloc, calloc or realloc to be reallocated.
* If this is NULL, a new block is allocated and a pointer to it is returned by the function.
* @param size New size for the memory block, in bytes.
* If it is 0 and ptr points to an existing block of memory, the memory block pointed by ptr is deallocated and a NULL pointer is returned. 
* @retval A pointer to the reallocated memory block, which may be either the same as the ptr argument or a new location.
* The type of this pointer is void*, which can be cast to the desired type of data pointer in order to be dereferenceable.
* If the function failed to allocate the requested block of memory, a NULL pointer is returned.
* You MUST call @a tsk_free to free the newly reallocated memory.
*/
void* tsk_realloc (tsk_heap_t *heap,  void* ptr, size_t size)
{
	void *ret = 0;
	void *old = ptr;
	tsk_heap_address_t* address = 0;
	
	/* Get old pointer from the heap */
	if(heap)
	{
		address = (tsk_heap_address_t*)tsk_list_find_item(&heap->pool, tsk_memory_find_by_address, old);
	}

	ret = realloc(ptr, size);
	if(ret && (old != ret) && address) 
	{ /* update pointer value */
		address->data = ret;
	}

	return ret;
}

/**@ingroup tsk_memory_group
* Deallocate space in memory
* @param heap The memory heap from which to free @a ptr. Set to NULL if @a ptr had not been allocated using heap allocation mechanism.
* @param ptr Pointer to a memory block previously allocated with @a tsk_malloc, @a tsk_calloc or @a tsk_realloc to be deallocated.
* If a null pointer is passed as argument, no action occurs. 
*/
void tsk_free(tsk_heap_t *heap, void** ptr)
{
	if(ptr)
	{
		HEAP_POP(heap, *ptr);
		*ptr = 0;
	}
}

/**@ingroup tsk_memory_group
* Allocates a block of memory for an array of num elements, each of them size bytes long, and initializes all its bits to zero.
* The effective result is the allocation of an zero-initialized memory block of (num * size) bytes.
* @param heap The memory heap on which to allocate the newly allocated memory. Set to NULL if
* you don't want to use heap allocation mechanism.
* @param num Number of elements to be allocated
* @param size Size of elements
* @retval A pointer to the memory block allocated by the function. The type of this pointer is always void*, which can be cast to the desired type of data pointer in order to be dereferenceable.
* If the function failed to allocate the requested block of memory, a NULL pointer is returned.
* The returned pointer can be passed to free() if you are not using memory heap mechanism.
* You MUST not directly free the returned pointer if you are using heap mechanism.
*/
void* tsk_calloc(tsk_heap_t *heap, size_t num, size_t size)
{
	void* ret = calloc(num, size);
	HEAP_PUSH(heap, ret);

	return ret;
}