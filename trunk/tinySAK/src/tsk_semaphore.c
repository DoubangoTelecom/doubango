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

/**@file tsk_semaphore.c
* @brief Pthread Semaphore.
*
* @author Mamadou Diop <diopmamadou(at)yahoo.fr>
*
* @date Created: Sat Nov 8 16:54:58 2009 mdiop
*/
#include "tsk_semaphore.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <semaphore.h>
#include <pthread.h>

/**@defgroup tsk_semaphore_group Pthread Semaphore
*/

/**@ingroup tsk_semaphore_group
* Creates new Pthread semaphore. You MUST use @ref tsk_semaphore_free to free the handle.
* @retval The New semaphore handle.
* @sa @ref tsk_semaphore_free
*/
tsk_semaphore_handle_t* tsk_semaphore_create()
{
	tsk_semaphore_handle_t *handle = tsk_calloc(1, sizeof(sem_t));
	if(handle)
	{
		if(sem_init((sem_t*)handle, PTHREAD_PROCESS_PRIVATE, 0))
		{
			TSK_DEBUG_ERROR("Failed to initialize the new semaphore.");
		}
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new mutex.");
	}
	return handle;
}

/**@ingroup tsk_semaphore_group
* Increment a semaphore. You should use @ref tsk_semaphore_decrement to decrement the semaphore.
* @param handle The semaphore to increment.
* @retval Zero if succeed and otherwise the function returns -1 and sets errno to indicate the error.
* @sa @ref tsk_semaphore_decrement.
*/
int tsk_semaphore_increment(tsk_semaphore_handle_t* handle)
{
	int ret = EINVAL;
	if(handle)
	{
		if(ret = sem_post((sem_t*)handle))
		{
			TSK_DEBUG_ERROR("sem_post function failed: %d", ret);
		}
	}
	return ret;
}

/**@ingroup tsk_semaphore_group
* Decrement a semaphore. You should use @ref tsk_semaphore_increment to increment a semaphore.
* @param handle The semaphore to decrement.
* @retval Zero if succeed and otherwise the function returns -1 and sets errno to indicate the error.
* @sa @ref tsk_semaphore_increment.
*/
int tsk_semaphore_decrement(tsk_semaphore_handle_t* handle)
{
	int ret = EINVAL;
	if(handle)
	{
		do 
		{ 
			ret = sem_wait((sem_t*)handle); 
		} 
		while ( errno == EINTR );
	}

	if(ret)	TSK_DEBUG_ERROR("sem_wait function failed: %d", errno);

	return ret;
}

/**@ingroup tsk_semaphore_group
* Destroy a semaphore previously created using @ref tsk_semaphore_create.
* @param handle The semaphore to free.
* @sa @ref tsk_semaphore_create
*/
void tsk_semaphore_destroy(tsk_semaphore_handle_t** handle)
{
	if(handle && *handle)
	{
		sem_destroy((sem_t*)*handle);
		tsk_free(handle);
	}
	else
	{
		TSK_DEBUG_WARN("Cannot free an uninitialized semaphore object");
	}
}
