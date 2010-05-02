/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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

/**@file tsk_mutex.c
 * @brief Pthread/Windows Mutex utility functions.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tsk_mutex.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#if TSK_UNDER_WINDOWS
#	include <windows.h>
#	include "tsk_errno.h"
	typedef HANDLE	MUTEX_T;
#	define MUTEX_S void
#	define TSK_ERROR_NOT_OWNER ERROR_NOT_OWNER
#else
#	include <pthread.h>
#	define MUTEX_S pthread_mutex_t
	typedef MUTEX_S* MUTEX_T;
#	define TSK_ERROR_NOT_OWNER EPERM
#endif

#if defined(__GNUC__) || defined(__SYMBIAN32__)
#	include <errno.h>
#endif

// FIXME: Momory leaks in mutex

/**@defgroup tsk_mutex_group Pthread/Windows Mutex utility functions.
*/

/**@ingroup tsk_mutex_group
* Creates new recursive mutex handle.
* @retval New mutex handle. It is up to you free the returned handle using  @ref tsk_mutex_destroy.
* @sa @ref tsk_mutex_destroy.
*/
tsk_mutex_handle_t* tsk_mutex_create()
{	
	MUTEX_T handle = tsk_null;
	
#if TSK_UNDER_WINDOWS
	handle = CreateMutex(NULL, FALSE, NULL);
#else
	int ret;
	pthread_mutexattr_t   mta;
	
	if((ret = pthread_mutexattr_init(&mta))){
		TSK_DEBUG_ERROR("pthread_mutexattr_init failed with error code %d", ret);
		return tsk_null;
	}
	if((ret = pthread_mutexattr_settype(&mta, PTHREAD_MUTEX_RECURSIVE))){
		TSK_DEBUG_ERROR("pthread_mutexattr_settype failed with error code %d", ret);
		pthread_mutexattr_destroy(&mta);
		return tsk_null;
	}
	
	/* if we are here: all is ok */
	handle = tsk_calloc(1, sizeof(MUTEX_S));
	if(pthread_mutex_init((MUTEX_T)handle, &mta)){
		TSK_FREE(handle);
	}
	pthread_mutexattr_destroy(&mta);
#endif
	
	if(!handle){
		TSK_DEBUG_ERROR("Failed to create new mutex.");
	}
	return handle;
}

/**@ingroup tsk_mutex_group
* Lock a mutex. You must use @ref tsk_mutex_unlock to unlock the mutex.
* @param handle The handle of the mutex to lock.
* @retval Zero if succeed and non-zero error code otherwise.
* @sa @ref tsk_mutex_unlock.
*/
int tsk_mutex_lock(tsk_mutex_handle_t* handle)
{
	int ret = EINVAL;
	if(handle)
	{
#if TSK_UNDER_WINDOWS
		if((ret = WaitForSingleObject((MUTEX_T)handle , INFINITE)) == WAIT_FAILED)
#else
		if(ret = pthread_mutex_lock((MUTEX_T)handle))
#endif
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
#if TSK_UNDER_WINDOWS
		if((ret = ReleaseMutex((MUTEX_T)handle) ? 0 : -1)){
			ret = GetLastError();
#else
		if(ret = pthread_mutex_unlock((MUTEX_T)handle))
		{
#endif
			if(ret == TSK_ERROR_NOT_OWNER){
				TSK_DEBUG_WARN("The calling thread does not own the mutex: %d", ret);
			}
			else{
				TSK_DEBUG_ERROR("Failed to unlock the mutex: %d", ret);
			}
		}
	}
	return ret;
}

/**@ingroup tsk_mutex_group
* Free/destroy a mutex.
* @param handle The mutex to free.
* @sa @ref tsk_mutex_create.
*/
void tsk_mutex_destroy(tsk_mutex_handle_t** handle)
{
	if(handle && *handle){
#if TSK_UNDER_WINDOWS
		CloseHandle((MUTEX_T)*handle);
		*handle = 0;
#else
		pthread_mutex_destroy((MUTEX_T)*handle);
		tsk_free(handle);
#endif
	}
	else{
		TSK_DEBUG_WARN("Cannot free an uninitialized mutex");
	}
}

