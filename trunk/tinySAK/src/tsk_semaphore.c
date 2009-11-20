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
* Internal function to initialize a semaphore. You MUST use @ref TSK_SEMAPHORE_CREATE to create and initialize a semaphore.
* @param semaphore The semaphore to initialize.
* @sa @ref TSK_SEMAPHORE_CREATE
*/
void tsk_semaphore_init(tsk_semaphore_t* semaphore)
{
	semaphore->handle = tsk_calloc2(1, sizeof(sem_t));
	sem_init((sem_t*)semaphore->handle, PTHREAD_PROCESS_PRIVATE, 0);
}

/**@ingroup tsk_semaphore_group
* Increment a semaphore. You should use @ref tsk_semaphore_decrement to decrement the semaphore.
* @param semaphore The semaphore to increment.
* @retval Zero if succeed and otherwise the function returns -1 and sets errno to indicate the error.
* @sa @ref tsk_semaphore_decrement.
*/
int tsk_semaphore_increment(tsk_semaphore_t* semaphore)
{
	int ret = EINVAL;
	if(semaphore)
	{
		if(ret = sem_post((sem_t*)semaphore->handle))
		{
			TSK_DEBUG_ERROR("sem_post function failed: %d", ret);
		}
	}
	return ret;
}

/**@ingroup tsk_semaphore_group
* Decrement a semaphore. You should use @ref tsk_semaphore_increment to increment a semaphore.
* @param semaphore The semaphore to decrement.
* @retval Zero if succeed and otherwise the function returns -1 and sets errno to indicate the error.
* @sa @ref tsk_semaphore_increment.
*/
int tsk_semaphore_decrement(tsk_semaphore_t* semaphore)
{
	int ret = EINVAL;
	if(semaphore)
	{
		do 
		{ 
			ret = sem_wait((sem_t*)semaphore->handle); 
		} 
		while ( errno == EINTR );
	}

	if(ret)	TSK_DEBUG_ERROR("sem_wait function failed: %d", errno);

	return ret;
}

/**@ingroup tsk_semaphore_group
* Internal function to free a semaphore previously created using @ref TSK_SEMAPHORE_CREATE. You MUST use @ref TSK_SEMAPHORE_SAFE_FREE to safely free a semaphore.
* @param semaphore The semaphore to free.
* @sa @ref TSK_SEMAPHORE_SAFE_FREE
*/
void tsk_semaphore_free(tsk_semaphore_t** semaphore)
{
	if(semaphore && *semaphore)
	{
		sem_destroy((sem_t*)(*semaphore)->handle);
		TSK_FREE((*semaphore)->handle);
		tsk_free2((void**)semaphore);
	}
	else
	{
		TSK_DEBUG_WARN("Cannot free an uninitialized semaphore object");
	}
}
