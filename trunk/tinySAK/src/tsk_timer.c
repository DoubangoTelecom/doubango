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

/**@file tsk_timer.c
* @brief Timers Mangement.
*
* @author Mamadou Diop <diopmamadou(at)yahoo.fr>
*
* @date Created: Sat Nov 8 16:54:58 2009 mdiop
*/
#include "tsk_timer.h"
#include "tsk_debug.h"
#include "tsk_list.h"
#include "tsk_thread.h"
#include "tsk_condwait.h"


/**@defgroup tsk_timer_group Timers Management
*/

typedef struct tsk_timer_s
{
	TSK_DECLARE_OBJECT;

	const void *arg; /**< Opaque data to return to the callback function. */
	uint64_t timeout; /**< Number of milliseconds after which the callback function is called. */
	tsk_timer_callback callback; /**< The callback function to after @ref timeout milliseconds. */
}
tsk_timer_t;

typedef tsk_list_t tsk_timers_L_t;

/*== Global variables. */
static tsk_condwait_handle_t *tsk_manager_condwait = 0;
static tsk_mutex_handle_t *tsk_manager_mutex = 0;
static int tsk_manager_running = 0;
static int tsk_manager_break = 0;
static tsk_timers_L_t *tsk_manager_timers = 0;
static void *tsk_manager_thread_id = 0;

/*== Definitions */
static void *tsk_timer_manager_loop(void *param); 

/**@ingroup tsk_timer_group
* Start the timer manager.
*/
int tsk_timer_manager_start()
{
	if(!tsk_manager_running && !tsk_manager_condwait)
	{
		tsk_manager_condwait = tsk_condwait_create();
		tsk_manager_mutex = tsk_condwait_get_mutex(tsk_manager_condwait);

		//tsk_mutex_lock(tsk_manager_mutex); /* First lock. */
		tsk_manager_break = 0;
		tsk_thread_create(&tsk_manager_thread_id, tsk_timer_manager_loop, 0);
	}
	return 0;
}

int tsk_timer_manager_stop()
{
	if(tsk_manager_running && tsk_manager_condwait)
	{
		tsk_manager_break = 1;
		tsk_condwait_broadcast(tsk_manager_condwait);
		//tsk_mutex_unlock(tsk_manager_mutex);
		tsk_thread_join(&tsk_manager_thread_id);
		tsk_condwait_destroy(&tsk_manager_condwait);
		tsk_manager_running = 0;
	}
	return 0;
}

tsk_timer_id_t tsk_timer_manager_schedule(uint64_t timeout, tsk_timer_callback callback, const void *arg)
{
	return 0;	
}

void tsk_timer_manager_cancel(tsk_timer_id_t id)
{
	
}

static void *tsk_timer_manager_loop(void *param)
{
	TSK_DEBUG_INFO("TIMER MANAGER -- START");
	tsk_manager_running = 1;
	
	for(;;)
	{
		//tsk_mutex_lock(tsk_manager_mutex);
		//tsk_condwait_wait(tsk_manager_condwait);

		if(tsk_manager_break)
		{
			break;
		}
	}
	
	TSK_DEBUG_INFO("TIMER MANAGER -- STOP");

	return 0;
}