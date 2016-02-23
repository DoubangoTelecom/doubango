/*
 * Copyright (C) 2010-2015 Mamadou DIOP.
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

/**@file tsk_semaphore.c
 * @brief Pthread/Windows Semaphore utility functions.
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tsk_semaphore.h"
#include "tsk_memory.h"
#include "tsk_debug.h"
#include "tsk_string.h"
#include "tsk_time.h"

/* Apple claims that they fully support POSIX semaphore but ...
 */
#if defined(__APPLE__) /* Mac OSX/Darwin/Iphone/Ipod Touch */
#	define TSK_USE_NAMED_SEM	1
#else
#	define TSK_USE_NAMED_SEM	0
#endif

#if !defined(NAME_MAX)
#   define	NAME_MAX		  255
#endif

#if TSK_UNDER_WINDOWS /* Windows XP/Vista/7/CE */

#	include <windows.h>
#	include "tsk_errno.h"
#	define SEMAPHORE_S void
typedef HANDLE	SEMAPHORE_T;
#	if TSK_UNDER_WINDOWS_RT
#		if !defined(CreateSemaphoreEx)
#			define CreateSemaphoreEx CreateSemaphoreExW
#		endif
#	endif
//#else if define(__APPLE__) /* Mac OSX/Darwin/Iphone/Ipod Touch */
//#	include <march/semaphore.h>
//#	include <march/task.h>
#else /* All *nix */

#	include <pthread.h>
#	include <semaphore.h>
#	if TSK_USE_NAMED_SEM
#	include <fcntl.h> /* O_CREAT */
#	include <sys/stat.h> /* S_IRUSR, S_IWUSR*/

typedef struct named_sem_s {
    sem_t* sem;
    char name [NAME_MAX + 1];
} named_sem_t;
#		define SEMAPHORE_S named_sem_t
#		define GET_SEM(PSEM) (((named_sem_t*)(PSEM))->sem)
#	else
#		define SEMAPHORE_S sem_t
#		define GET_SEM(PSEM) ((PSEM))
#	endif /* TSK_USE_NAMED_SEM */
typedef sem_t* SEMAPHORE_T;

#endif

#if defined(__GNUC__) || defined(__SYMBIAN32__)
#	include <errno.h>
#endif



/**@defgroup tsk_semaphore_group Pthread/Windows Semaphore functions.
 */

/**@ingroup tsk_semaphore_group
 * Creates new semaphore handle.
 * @retval A New semaphore handle.
 * You MUST call @ref tsk_semaphore_destroy to free the semaphore.
 * @sa @ref tsk_semaphore_destroy
 */
tsk_semaphore_handle_t* tsk_semaphore_create()
{
    return tsk_semaphore_create_2(0);
}

tsk_semaphore_handle_t* tsk_semaphore_create_2(int initial_val)
{
    SEMAPHORE_T handle = tsk_null;

#if TSK_UNDER_WINDOWS
#	if TSK_UNDER_WINDOWS_RT
    handle = CreateSemaphoreEx(NULL, initial_val, 0x7FFFFFFF, NULL, 0x00000000, SEMAPHORE_ALL_ACCESS);
#	else
    handle = CreateSemaphore(NULL, initial_val, 0x7FFFFFFF, NULL);
#	endif
#else
    handle = tsk_calloc(1, sizeof(SEMAPHORE_S));

#if TSK_USE_NAMED_SEM
    named_sem_t * nsem = (named_sem_t*)handle;
    snprintf(nsem->name, (sizeof(nsem->name)/sizeof(nsem->name[0])) - 1, "/sem/%llu/%d.", tsk_time_epoch(), rand() ^ rand());
    if ((nsem->sem = sem_open(nsem->name, O_CREAT /*| O_EXCL*/, S_IRUSR | S_IWUSR, initial_val)) == SEM_FAILED) {
#else
    if (sem_init((SEMAPHORE_T)handle, 0, initial_val)) {
#endif
        TSK_FREE(handle);
        TSK_DEBUG_ERROR("Failed to initialize the new semaphore (errno=%d).", errno);
    }
#endif
    if (!handle) {
        TSK_DEBUG_ERROR("Failed to create new semaphore");
    }
    return handle;
}

/**@ingroup tsk_semaphore_group
 * Increments a semaphore.
 * @param handle The semaphore to increment.
 * @retval Zero if succeed and otherwise the function returns -1 and sets errno to indicate the error.
 * @sa @ref tsk_semaphore_decrement.
 */
int tsk_semaphore_increment(tsk_semaphore_handle_t* handle)
{
    int ret = EINVAL;
    if (handle) {
#if TSK_UNDER_WINDOWS
        if((ret = ReleaseSemaphore((SEMAPHORE_T)handle, 1L, NULL) ? 0 : -1))
#else
        if((ret = sem_post((SEMAPHORE_T)GET_SEM(handle))))
#endif
        {
            TSK_DEBUG_ERROR("sem_post function failed: %d", ret);
        }
    }
    return ret;
}

/**@ingroup tsk_semaphore_group
 * Decrements a semaphore.
 * @param handle The semaphore to decrement.
 * @retval Zero if succeed and otherwise the function returns -1 and sets errno to indicate the error.
 * @sa @ref tsk_semaphore_increment.
 */
int tsk_semaphore_decrement(tsk_semaphore_handle_t* handle)
{
    int ret = EINVAL;
    if (handle) {
#if TSK_UNDER_WINDOWS
#	   if TSK_UNDER_WINDOWS_RT
        ret = (WaitForSingleObjectEx((SEMAPHORE_T)handle, INFINITE, TRUE) == WAIT_OBJECT_0) ? 0 : -1;
#	   else
        ret = (WaitForSingleObject((SEMAPHORE_T)handle, INFINITE) == WAIT_OBJECT_0) ? 0 : -1;
#endif
        if (ret)	{
            TSK_DEBUG_ERROR("sem_wait function failed: %d", ret);
        }
#else
        do {
            ret = sem_wait((SEMAPHORE_T)GET_SEM(handle));
        }
        while ( errno == EINTR );
        if(ret)	{
            TSK_DEBUG_ERROR("sem_wait function failed: %d", errno);
        }
#endif
    }

    return ret;
}

/**@ingroup tsk_semaphore_group
 * Destroy a semaphore previously created using @ref tsk_semaphore_create.
 * @param handle The semaphore to free.
 * @sa @ref tsk_semaphore_create
 */
void tsk_semaphore_destroy(tsk_semaphore_handle_t** handle)
{
    if(handle && *handle) {
#if TSK_UNDER_WINDOWS
        CloseHandle((SEMAPHORE_T)*handle);
        *handle = tsk_null;
#else
#	if TSK_USE_NAMED_SEM
        named_sem_t * nsem = ((named_sem_t*)*handle);
        sem_close(nsem->sem);
#else
        sem_destroy((SEMAPHORE_T)GET_SEM(*handle));
#endif /* TSK_USE_NAMED_SEM */
        tsk_free(handle);
#endif
    }
    else {
        TSK_DEBUG_WARN("Cannot free an uninitialized semaphore object");
    }
}

