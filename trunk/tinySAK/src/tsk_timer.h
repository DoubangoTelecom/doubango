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

/**@file tsk_timers.h
 * @brief Timer Management.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TINYSAK_TIMER_H_
#define _TINYSAK_TIMER_H_

#include "tinySAK_config.h"

TSK_BEGIN_DECLS

#define TSK_TIMER_MANAGER_CREATE()					tsk_object_new(tsk_timer_manager_def_t)
#define TSK_TIMER_MANAGER_SAFE_FREE(self)			tsk_object_unref(self), self = 0
#define TSK_TIMER_CALLBACK(callback)				((tsk_timer_callback)callback)

#define TSK_INVALID_TIMER_ID						0
#define TSK_TIMER_ID_IS_VALID(id)					(id != TSK_INVALID_TIMER_ID)


typedef void tsk_timer_manager_handle_t;
typedef uint64_t tsk_timer_id_t;
typedef int (*tsk_timer_callback)(const void* arg, tsk_timer_id_t timer_id); /**< Callback function called when scheduled timer timeout. */

TINYSAK_API int tsk_timer_manager_start(tsk_timer_manager_handle_t *self);
TINYSAK_API int tsk_timer_manager_isready(tsk_timer_manager_handle_t *self);
TINYSAK_API int tsk_timer_manager_stop(tsk_timer_manager_handle_t *self);
#if defined(DEBUG) || defined(_DEBUG)
TINYSAK_API void tsk_timer_manager_debug(tsk_timer_manager_handle_t *self);
#endif

TINYSAK_API tsk_timer_id_t tsk_timer_manager_schedule(tsk_timer_manager_handle_t *self, uint64_t timeout, tsk_timer_callback callback, const void *arg);
TINYSAK_API int tsk_timer_manager_cancel(tsk_timer_manager_handle_t *self, tsk_timer_id_t id);

TINYSAK_GEXTERN const void *tsk_timer_def_t;
TINYSAK_GEXTERN const void *tsk_timer_manager_def_t;

TSK_END_DECLS

#endif /* _TINYSAK_TIMER_H_ */
