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

typedef enum tsk_timer_retcode_e
{
	tsk_error,
	tsk_canceled,
	tsk_timedout,
}
tsk_timer_retcode_t;

typedef uint64_t tsk_timer_id_t;
typedef void (*tsk_timer_callback)(const void* arg, tsk_timer_retcode_t code);

TINYSAK_API int tsk_timer_manager_start();
TINYSAK_API int tsk_timer_manager_stop();

TINYSAK_API tsk_timer_id_t tsk_timer_manager_schedule(uint64_t timeout, tsk_timer_callback callback, const void *arg);
TINYSAK_API void tsk_timer_manager_cancel(tsk_timer_id_t id);

TINYSAK_API const void *tsk_timer_def_t;

#endif /* _TINYSAK_TIMER_H_ */