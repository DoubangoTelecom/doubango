/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsk_memory.c
 * @brief Useful memory management functions to handle memory.
 * As I'm a lazy man, some definition come from <ahref="http://www.cplusplus.com">this website</a>
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <stdarg.h>
#include <ctype.h>
#include <stdio.h>

/**@defgroup tsk_memory_group Utility functions for memory management.
*/

/**@ingroup tsk_memory_group
* Allocates a block of size bytes of memory, returning a pointer to the beginning of the block.
* The content of the newly allocated block of memory is not initialized, remaining with indeterminate values.
* @param size Size of the memory block, in bytes.
* @retval On success, a pointer to the memory block allocated by the function.
* It is up to you to free the returned pointer.
*/
void* tsk_malloc(tsk_size_t size)
{
	void *ret = malloc(size);
	if(!ret){
		TSK_DEBUG_ERROR("Memory allocation failed");
	}

	return ret;
}

/**@ingroup tsk_memory_group
* Reallocate memory block.
* In case that ptr is NULL, the function behaves exactly as @a tsk_malloc, assigning a new block of size bytes and returning a pointer to the beginning of it.
* The function may move the memory block to a new location, in which case the new location is returned. The content of the memory block is preserved up to the lesser of the 
* new and old sizes, even if the block is moved. If the new size is larger, the value of the newly allocated portion is indeterminate.
* In case that the size is 0, the memory previously allocated in ptr is deallocated as if a call to free was made, and a NULL pointer is returned.
* @param ptr Pointer to a memory block previously allocated with malloc, calloc or realloc to be reallocated.
* If this is NULL, a new block is allocated and a pointer to it is returned by the function.
* @param size New size for the memory block, in bytes.
* If it is 0 and ptr points to an existing block of memory, the memory block pointed by ptr is deallocated and a NULL pointer is returned. 
* @retval A pointer to the reallocated memory block, which may be either the same as the ptr argument or a new location.
* The type of this pointer is void*, which can be cast to the desired type of data pointer in order to be dereferenceable.
* If the function failed to allocate the requested block of memory, a NULL pointer is returned.
* It is up to you to free the returned pointer.
*/
void* tsk_realloc (void* ptr, tsk_size_t size)
{
	void *ret = tsk_null;
	
	if(size) {
		if(ptr){
			if(!(ret = realloc(ptr, size))){
				TSK_DEBUG_ERROR("Memory reallocation failed");
			}
		}
		else{
			if(!(ret = calloc(size, 1))){
				TSK_DEBUG_ERROR("Memory allocation (%u) failed", size);
			}
		}
	}

	return ret;
}

/**@ingroup tsk_memory_group
* Deallocate space in memory.
* @param ptr Pointer to a memory block previously allocated with @a tsk_malloc, @a tsk_calloc or @a tsk_realloc to be deallocated.
* If a null pointer is passed as argument, no action occurs. 
*/
void tsk_free(void** ptr)
{
	if(ptr && *ptr){
		free(*ptr);
		*ptr = tsk_null;
	}
}

/**@ingroup tsk_memory_group
* Allocates a block of memory for an array of num elements, each of them size bytes long, and initializes all its bits to zero.
* The effective result is the allocation of an zero-initialized memory block of (num * size) bytes.
* @param num Number of elements to be allocated
* @param size Size of elements
* @retval A pointer to the memory block allocated by the function. The type of this pointer is always void*, which can be cast to the desired type of data pointer in order to be dereferenceable.
* If the function failed to allocate the requested block of memory, a NULL pointer is returned.
* It is up to you to free the returned pointer.
*/
void* tsk_calloc(tsk_size_t num, tsk_size_t size)
{
	void* ret = tsk_null;
	if(num && size){
		ret = calloc(num, size);
		if(!ret){
			TSK_DEBUG_ERROR("Memory allocation failed. num=%u and size=%u", num, size);
		}
	}

	return ret;
}
