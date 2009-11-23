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

/**@file tsk_mutex.c
 * @brief Pthread Mutex.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tsk_mutex.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <pthread.h>

// FIXME: Momory leaks in mutex

/**@defgroup tsk_mutex_group Pthread Mutex
*/

/**@ingroup tsk_mutex_group
* Creates new mutex handle. You MUST use @ref tsk_mutex_destroy to free the mutex handle.
* @retval New mutex handle.
* @sa @ref tsk_mutex_destroy
*/
tsk_mutex_handle_t* tsk_mutex_create()
{
	tsk_mutex_handle_t *handle = tsk_calloc(1, sizeof(pthread_mutex_t));
	if(handle)
	{
		if(pthread_mutex_init((pthread_mutex_t*)handle, 0))
		{
			TSK_DEBUG_ERROR("Failed to initialize the new mutex.");
		}
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new mutex.");
	}
	return handle;
}

/**@ingroup tsk_mutex_group
* Lock a mutex. You must use @ref tsk_mutex_unlock to unlock the mutex.
* @param handle The handle of the mutex to lock.
* @retval Zero if succeed and non-zero otherwise.
* @sa @ref tsk_mutex_unlock.
*/
int tsk_mutex_lock(tsk_mutex_handle_t* handle)
{
	int ret = EINVAL;
	if(handle)
	{
		if(ret = pthread_mutex_lock((pthread_mutex_t*)handle))
		{
			TSK_DEBUG_ERROR("Failed to lock the mutex: %d", ret);
		}
	}
	return ret;
}

/**@ingroup tsk_mutex_group
* Unlock a mutex previously locked using @ref tsk_mutex_lock.
* @param handle The handle of the mutex to unlock.
* @retval Zero if succeed and non-zero otherwise.
* @sa @ref tsk_mutex_lock.
*/
int tsk_mutex_unlock(tsk_mutex_handle_t* handle)
{
	int ret = EINVAL;
	if(handle)
	{
		if(ret = pthread_mutex_unlock((pthread_mutex_t*)handle))
		{
			if(ret == EPERM)
			{
				TSK_DEBUG_INFO("The calling thread does not own the mutex: %d", ret);
			}
			else
			{
				TSK_DEBUG_ERROR("Failed to unlock the mutex: %d", ret);
			}
		}
	}
	return ret;
}

/**@ingroup tsk_mutex_group
* Internal function to free a mutex previously created using @ref TSK_MUTEX_CREATE. You MUST use @ref TSK_MUTEX_SAFE_FREE to safely free a mutex.
* @param mutex The mutex to free.
* @sa @ref TSK_MUTEX_SAFE_FREE
*/
void tsk_mutex_destroy(tsk_mutex_handle_t** handle)
{
	if(handle && *handle)
	{
		pthread_mutex_destroy((pthread_mutex_t*)*handle);
		tsk_free(handle);
	}
	else
	{
		TSK_DEBUG_WARN("Cannot free an uninitialized mutex");
	}
}
