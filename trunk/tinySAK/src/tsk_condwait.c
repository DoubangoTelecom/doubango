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

#include <pthread.h>

#include <time.h>
#if defined(WIN32) || defined(_WIN32) || defined(_WIN32_WCE)
#include <windows.h>
#else 
#include <sys/time.h>
#endif


/**@defgroup tsk_condwait_group Pthread CondWait
*/

/**@ingroup tsk_condwait_group
* Internal function to initialize a condwait. You MUST use @ref TSK_CONDWAIT_CREATE to create and initialize a condwait.
* @param condwait The condwait to initialize.
* @sa @ref TSK_CONDWAIT_CREATE
*/
void tsk_condwait_init(tsk_condwait_t* condwait)
{
	condwait->handle = tsk_calloc2(1, sizeof(pthread_cond_t));
	pthread_cond_init((pthread_cond_t*)condwait->handle, 0);
	TSK_MUTEX_CREATE(condwait->mutex);
}

/**@ingroup tsk_condwait_group
* Wait for a condition indefinitely.
* @param condwait CondWait context created using @ref TSK_CONDWAIT_CREATE.
* @retval Zero if succeed and non-zero otherwise.
* @sa @ref tsk_condwait_timedwait.
*/
int tsk_condwait_wait(tsk_condwait_t* condwait)
{
	int ret = EINVAL;

	if(condwait && condwait->mutex)
	{
		tsk_mutex_lock(condwait->mutex);
		if(ret = pthread_cond_wait((pthread_cond_t*)condwait->handle, (pthread_mutex_t*)condwait->mutex->handle))
		{
			TSK_DEBUG_ERROR("pthread_cond_wait function failed: %d", ret);
		}
		tsk_mutex_unlock(condwait->mutex);
	}
	return ret;
}

/**@ingroup tsk_condwait_group
* Wait for a condition @ref ms milliseconds.
* @param condwait CondWait context created using @ref TSK_CONDWAIT_CREATE.
* @param ms The number of milliseconds to wait for a given condition.
* @retval Zero if succeed or @ref ETIMEDOUT if timedout and non-zero otherwise.
* @sa @ref tsk_condwait_wait.
*/
int tsk_condwait_timedwait(tsk_condwait_t* condwait, unsigned int ms)
{
	int ret = EINVAL;

	if(condwait && condwait->mutex)
	{
		struct timespec   ts = {0, 0};
		struct timeval    tv = {0, 0};
		/*int rc =*/  tsk_gettimeofday(&tv, 0);

		ts.tv_sec  = ( tv.tv_sec + (ms/1000) );
		ts.tv_nsec += ( (tv.tv_usec * 1000) + (ms % 1000 * 1000000) );
		if(ts.tv_nsec > 999999999) ts.tv_sec+=1, ts.tv_nsec = ts.tv_nsec % 1000000000;
		
		tsk_mutex_lock(condwait->mutex);
		if(ret = pthread_cond_timedwait((pthread_cond_t*)condwait->handle, (pthread_mutex_t*)condwait->mutex->handle, &ts))
		{
			TSK_DEBUG_ERROR("pthread_cond_timedwait function failed: %d", ret);
		}
		tsk_mutex_unlock(condwait->mutex);

		return ret;
	}
	return ret;
}

/**@ingroup tsk_condwait_group
* Wakes up at least one thread that is currently waiting on @ref condwait variable.
* @param condwait CondWait context created using @ref TSK_CONDWAIT_CREATE.
* @retval Zero if succeed and non-zero otherwise.
* @sa @ref tsk_condwait_broadcast.
*/
int tsk_condwait_signal(tsk_condwait_t* condwait)
{
	int ret = EINVAL;

	if(condwait && condwait->mutex)
	{
		tsk_mutex_lock(condwait->mutex);
		if(ret = pthread_cond_signal((pthread_cond_t*)condwait->handle))
		{
			TSK_DEBUG_ERROR("pthread_cond_signal function failed: %d", ret);
		}
		tsk_mutex_unlock(condwait->mutex);
	}
	return ret;
}

/**@ingroup tsk_condwait_group
* Wakes up all threads that are currently waiting on @ref condwait variable.
* @param condwait CondWait context created using @ref TSK_CONDWAIT_CREATE.
* @retval Zero if succeed and non-zero otherwise.
* @sa @ref tsk_condwait_signal.
*/
int tsk_condwait_broadcast(tsk_condwait_t* condwait)
{
	int ret = EINVAL;

	if(condwait && condwait->mutex)
	{
		tsk_mutex_lock(condwait->mutex);
		if(ret = pthread_cond_broadcast((pthread_cond_t*)condwait->handle))
		{
			TSK_DEBUG_ERROR("pthread_cond_broadcast function failed: %d", ret);
		}
		tsk_mutex_unlock(condwait->mutex);
	}
	return ret;
}

/**@ingroup tsk_condwait_group
* Internal function to free a condwait previously created using @ref TSK_CONDWAIT_CREATE. You MUST use @ref TSK_CONDWAIT_SAFE_FREE to safely free a condwait.
* @param condwait The condwait to free.
* @sa @ref TSK_CONDWAIT_SAFE_FREE
*/
void tsk_condwait_free(tsk_condwait_t** condwait)
{
	if(condwait && *condwait)
	{
		TSK_MUTEX_SAFE_FREE((*condwait)->mutex);
		pthread_cond_destroy((pthread_cond_t*)(*condwait)->handle);
		TSK_FREE((*condwait)->handle);
		tsk_free2((void**)condwait);
	}
	else
	{
		TSK_DEBUG_WARN("Cannot free an uninitialized condwait object");
	}
}
