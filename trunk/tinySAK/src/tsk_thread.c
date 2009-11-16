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

/**@file tsk_thread.c
 * @brief Useful functions for threading.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tsk_thread.h"
#include "tsk_memory.h"

#include <pthread.h>

/**@defgroup tsk_thread_group Useful functions for threading.
*/

/**@ingroup tsk_thread_group
* Creates a new thread.
* @param tid Pthread handle id to the created thread.
* @param start The function to be run as the new threads start routine 
* @param arg An address for the argument for the threads start routine 
* @retval If successful, returns zero. Otherwise, an error number is returned to indicate the error. 
*/
int tsk_thread_create(void** tid, void *(*start) (void *), void *arg)
{
	*tid = tsk_calloc2(1, sizeof(pthread_t));
	return pthread_create((pthread_t*)*tid, 0, start, arg);
}

/**@ingroup tsk_thread_group
* Join a thread.
* @param tid Pthread handle to the target thread.
* @retval If successful, returns zero. Otherwise, an error number is returned to indicate the error. 
*/
int tsk_thread_join(void** tid)
{
	int ret = pthread_join(*((pthread_t*)*tid), 0);
	tsk_free2(tid);
	return ret;
}