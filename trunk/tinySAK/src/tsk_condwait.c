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

/**@file tsk_condwait.c
 * @brief Pthread CondWait
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#include "tsk_condwait.h"
#include "tsk_memory.h"
#include "tsk_debug.h"
#include "tsk_macros.h"
#include "tsk_time.h"
#include "tsk_mutex.h"
#include <pthread.h>

#include <time.h>
#if defined(WIN32) || defined(_WIN32) || defined(_WIN32_WCE)
#include <windows.h>
#else 
#include <sys/time.h>
#endif


/**@defgroup tsk_condwait_group Pthread CondWait
*/

/** Pthread CondWait.
*/
typedef struct tsk_condwait_s
{
	pthread_cond_t* pcond; /**< Handle pointing to the condwait */
	tsk_mutex_handle_t* mutex;  /**< Locker*/
}
tsk_condwait_t;

/**@ingroup tsk_condwait_group
* Creates new Pthread conwait handle. You MUST call @ref tsk_condwait_destroy to free the handle.
* @retval New condwait handle.
* @sa @ref tsk_condwait_destroy.
*/
tsk_condwait_handle_t* tsk_condwait_create()
{
	tsk_condwait_t *condwait = tsk_calloc(1, sizeof(tsk_condwait_t));
	if(condwait)
	{
		condwait->pcond = (pthread_cond_t*)tsk_calloc(1, sizeof(pthread_cond_t));
		if(pthread_cond_init(condwait->pcond, 0))
		{
			TSK_DEBUG_ERROR("Failed to initialize the new conwait.");
		}
		if(!(condwait->mutex = tsk_mutex_create()))
		{
			if(!pthread_mutex_init((pthread_mutex_t*)condwait->mutex, 0))
			{
				TSK_DEBUG_ERROR("Failed to initialize the new mutex.");
			}
		}
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new conwait.");
	}
	return condwait;
}

/**@ingroup tsk_condwait_group
* Wait for a condition indefinitely.
* @param handle CondWait handle created using @ref tsk_condwait_create.
* @retval Zero if succeed and non-zero otherwise.
* @sa @ref tsk_condwait_timedwait.
*/
int tsk_condwait_wait(tsk_condwait_handle_t* handle)
{
	int ret = EINVAL;
	tsk_condwait_t *condwait = (tsk_condwait_t*)handle;

	if(condwait && condwait->mutex)
	{
		tsk_mutex_lock(condwait->mutex);
		if(ret = pthread_cond_wait(condwait->pcond, (pthread_mutex_t*)condwait->mutex))
		{
			TSK_DEBUG_ERROR("pthread_cond_wait function failed: %d", ret);
		}
		tsk_mutex_unlock(condwait->mutex);
	}
	return ret;
}

/**@ingroup tsk_condwait_group
* Wait for a condition @ref ms milliseconds.
* @param handle CondWait context created using @ref tsk_condwait_create.
* @param ms The number of milliseconds to wait for a given condition.
* @retval Zero if succeed or @ref ETIMEDOUT if timedout and non-zero otherwise.
* @sa @ref tsk_condwait_wait.
*/
int tsk_condwait_timedwait(tsk_condwait_handle_t* handle, unsigned int ms)
{
	int ret = EINVAL;
	tsk_condwait_t *condwait = (tsk_condwait_t*)handle;

	if(condwait && condwait->mutex)
	{
		struct timespec   ts = {0, 0};
		struct timeval    tv = {0, 0};
		/*int rc =*/  tsk_gettimeofday(&tv, 0);

		ts.tv_sec  = ( tv.tv_sec + (ms/1000) );
		ts.tv_nsec += ( (tv.tv_usec * 1000) + (ms % 1000 * 1000000) );
		if(ts.tv_nsec > 999999999) ts.tv_sec+=1, ts.tv_nsec = ts.tv_nsec % 1000000000;
		
		tsk_mutex_lock(condwait->mutex);
		if(ret = pthread_cond_timedwait(condwait->pcond, (pthread_mutex_t*)condwait->mutex, &ts))
		{
			if(ret == ETIMEDOUT)
			{
				TSK_DEBUG_INFO("pthread_cond_timedwait function timedout: %d", ret);
			}
			else
			{
				TSK_DEBUG_ERROR("pthread_cond_timedwait function failed: %d", ret);
			}
		}
		tsk_mutex_unlock(condwait->mutex);

		return ret;
	}
	return ret;
}

/**@ingroup tsk_condwait_group
* Wakes up at least one thread that is currently waiting on @ref condwait variable.
* @param handle CondWait handle created using @ref tsk_condwait_create.
* @retval Zero if succeed and non-zero otherwise.
* @sa @ref tsk_condwait_broadcast.
*/
int tsk_condwait_signal(tsk_condwait_handle_t* handle)
{
	int ret = EINVAL;
	tsk_condwait_t *condwait = (tsk_condwait_t*)handle;

	if(condwait && condwait->mutex)
	{
		tsk_mutex_lock(condwait->mutex);
		if(ret = pthread_cond_signal(condwait->pcond))
		{
			TSK_DEBUG_ERROR("pthread_cond_signal function failed: %d", ret);
		}
		tsk_mutex_unlock(condwait->mutex);
	}
	return ret;
}

/**@ingroup tsk_condwait_group
* Wakes up all threads that are currently waiting on @ref condwait variable.
* @param handle CondWait handle created using @ref tsk_condwait_create.
* @retval Zero if succeed and non-zero otherwise.
* @sa @ref tsk_condwait_signal.
*/
int tsk_condwait_broadcast(tsk_condwait_handle_t* handle)
{
	int ret = EINVAL;
	tsk_condwait_t *condwait = (tsk_condwait_t*)handle;

	if(condwait && condwait->mutex)
	{
		tsk_mutex_lock(condwait->mutex);
		if(ret = pthread_cond_broadcast(condwait->pcond))
		{
			TSK_DEBUG_ERROR("pthread_cond_broadcast function failed: %d", ret);
		}
		tsk_mutex_unlock(condwait->mutex);
	}
	return ret;
}

/**@ingroup tsk_condwait_group
* Destroy/Free a condwait variable previously created using @ref tsk_condwait_create.
* @param handle The condwait handle to free.
* @sa @ref tsk_condwait_create
*/
void tsk_condwait_destroy(tsk_condwait_handle_t** handle)
{
	tsk_condwait_t **condwait = (tsk_condwait_t**)handle;
	
	if(condwait && *condwait)
	{
		tsk_mutex_destroy(&((*condwait)->mutex));
		pthread_cond_destroy((*condwait)->pcond);
		TSK_FREE((*condwait)->pcond);
		tsk_free(condwait);
	}
	else
	{
		TSK_DEBUG_WARN("Cannot free an uninitialized condwait object");
	}
}
