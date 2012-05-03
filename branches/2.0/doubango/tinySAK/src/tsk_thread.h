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

/**@file tsk_thread.h
 * @brief Utility functions for threading.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TINYSAK_THREAD_H_
#define _TINYSAK_THREAD_H_

#include "tinysak_config.h"

typedef void tsk_thread_handle_t;
#if TSK_UNDER_WINDOWS
	typedef unsigned long tsk_thread_id_t;
#else
#	include <pthread.h>
typedef pthread_t tsk_thread_id_t;
#endif

TSK_BEGIN_DECLS

TINYSAK_API void tsk_thread_sleep(uint64_t ms);
TINYSAK_API int tsk_thread_create(tsk_thread_handle_t** handle, void *(*start) (void *), void *arg);
TINYSAK_API int tsk_thread_set_priority(tsk_thread_handle_t* handle, int32_t priority);
TINYSAK_API int tsk_thread_set_priority_2(int32_t priority);
TINYSAK_API tsk_thread_id_t tsk_thread_get_id();
TINYSAK_API tsk_bool_t tsk_thread_id_equals(tsk_thread_id_t* id_1, tsk_thread_id_t *id_2);
TINYSAK_API int tsk_thread_destroy(tsk_thread_handle_t** handle);
TINYSAK_API int tsk_thread_join(tsk_thread_handle_t** handle);

TSK_END_DECLS

#endif


