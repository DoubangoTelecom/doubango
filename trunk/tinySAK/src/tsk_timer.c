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
#include "tsk_semaphore.h"
#include "tsk_time.h"


/**@defgroup tsk_timer_group Timers Management
*/

#define TSK_TIMER_CREATE(timeout, callback, arg)	tsk_object_new(tsk_timer_def_t, timeout, callback, arg)
#define TSK_TIMER_SAFE_FREE(self)					tsk_object_unref(self), self = 0
#define TSK_TIMER_TIMEOUT(self)						((tsk_timer_t*)self)->timeout
#define TSK_TIMER_GET_FIRST()						(tsk_manager_timers && tsk_manager_timers->head) ? (tsk_timer_t*)(((tsk_list_item_t*)(tsk_manager_timers->head))->data) : 0

typedef struct tsk_timer_s
{
	TSK_DECLARE_OBJECT;

	tsk_timer_id_t id;	/**< Unique timer identifier. */
	const void *arg; /**< Opaque data to return with the callback function. */
	uint64_t timeout; /**< When the timer will timeout(as EPOCH time). */
	tsk_timer_callback callback; /**< The callback function to call after @ref timeout milliseconds. */
}
tsk_timer_t;

typedef tsk_list_t tsk_timers_L_t;


/*== Global variables. */
static tsk_condwait_handle_t *tsk_manager_condwait = 0;
static tsk_mutex_handle_t *tsk_manager_timers_mutex = 0;
static tsk_semaphore_handle_t *tsk_manager_semaphore = 0;
static int tsk_manager_running = 0;
static int tsk_manager_break = 0;
static tsk_timers_L_t *tsk_manager_timers = 0;
static void *tsk_manager_thread_id = 0;

/*== Definitions */
static void *__tsk_timer_manager_loop(void *param); 
static int __tsk_pred_find_timer_by_id(const tsk_list_item_t *item, const void *id);
static void __tsk_timer_manager_raise(tsk_timer_t *timer);

/**@ingroup tsk_timer_group
* Start the timer manager.
*/
int tsk_timer_manager_start()
{
	int err = -1;
	if(!tsk_manager_running && !tsk_manager_condwait)
	{
		tsk_manager_timers = TSK_LIST_CREATE();
		tsk_manager_semaphore = tsk_semaphore_create();
		tsk_manager_condwait = tsk_condwait_create();
		tsk_manager_timers_mutex = tsk_mutex_create();

		tsk_manager_break = 0;
		if(err = tsk_thread_create(&tsk_manager_thread_id, __tsk_timer_manager_loop, 0))
		{
			tsk_semaphore_destroy(&tsk_manager_semaphore);
			tsk_condwait_destroy(&tsk_manager_condwait);
			tsk_mutex_destroy(&tsk_manager_timers_mutex);
			tsk_object_unref(tsk_manager_timers);
			TSK_DEBUG_FATAL("Failed to start timer manager: %d\n", err);
			return err;
		}
		
		while(!tsk_manager_running) tsk_thread_sleep(500);
	}

	return err;
}

#if defined(DEBUG) || defined(_DEBUG)
void tsk_timer_manager_debug()
{
	if(tsk_manager_timers && tsk_manager_running)
	{
		int index = 0;
		tsk_list_item_t *item = 0;

		tsk_mutex_lock(tsk_manager_timers_mutex);

		tsk_list_foreach(item, tsk_manager_timers)
		{
			tsk_timer_t* timer = item->data;
			TSK_DEBUG_INFO("timer [%d]- %llu, %llu", timer->id, timer->timeout, tsk_time_epoch());
		}

		tsk_mutex_unlock(tsk_manager_timers_mutex);
	}
}
#endif

int tsk_timer_manager_stop()
{
	if(tsk_manager_running && tsk_manager_condwait)
	{
		tsk_manager_break = 1;
		tsk_semaphore_increment(tsk_manager_semaphore);
		tsk_condwait_broadcast(tsk_manager_condwait);
		tsk_thread_join(&tsk_manager_thread_id);

		tsk_semaphore_destroy(&tsk_manager_semaphore);
		tsk_condwait_destroy(&tsk_manager_condwait);
		tsk_mutex_destroy(&tsk_manager_timers_mutex);
		tsk_object_unref(tsk_manager_timers);
		tsk_manager_running = 0;
	}
	return 0;
}

tsk_timer_id_t tsk_timer_manager_schedule(uint64_t timeout, tsk_timer_callback callback, const void *arg)
{
	tsk_timer_id_t timer_id = INVALID_TIMER_ID;

	if(tsk_manager_timers && tsk_manager_running)
	{
		tsk_timer_t *timer;

		timer = TSK_TIMER_CREATE(timeout, callback, arg);
		timer_id = timer->id;
		tsk_mutex_lock(tsk_manager_timers_mutex);
		tsk_list_push_ascending_data(tsk_manager_timers, ((void**) &timer));
		tsk_mutex_unlock(tsk_manager_timers_mutex);

		tsk_semaphore_increment(tsk_manager_semaphore);

		tsk_condwait_broadcast(tsk_manager_condwait);
	}

	return timer_id;
}

int tsk_timer_manager_cancel(tsk_timer_id_t id)
{
	int ret = -1;
	if(tsk_manager_timers && tsk_manager_running)
	{
		const tsk_list_item_t *item;
		tsk_mutex_lock(tsk_manager_timers_mutex);
		item = tsk_list_find_item_by_pred(tsk_manager_timers, __tsk_pred_find_timer_by_id, &id);
		if(item && item->data)
		{
			tsk_timer_t *timer = item->data;
			timer->callback(timer->arg, tsk_canceled);
			tsk_list_remove_item(tsk_manager_timers, (tsk_list_item_t*)item);
			ret = 0;
		}
		tsk_mutex_unlock(tsk_manager_timers_mutex);
	}
	return ret;
}

static void __tsk_timer_manager_raise(tsk_timer_t *timer)
{
	if(tsk_manager_timers && tsk_manager_running)
	{
		if(timer)
		{
			if(timer->callback)
			{
				timer->callback(timer->arg, tsk_timedout);
			}
			tsk_mutex_lock(tsk_manager_timers_mutex);
			tsk_list_remove_item_by_pred(tsk_manager_timers, __tsk_pred_find_timer_by_id, &(timer->id));
			tsk_mutex_unlock(tsk_manager_timers_mutex);
		}
	}
}

static int __tsk_pred_find_timer_by_id(const tsk_list_item_t *item, const void *id)
{
	if(item && item->data)
	{
		tsk_timer_t *timer = item->data;
		return (int)(timer->id - *((tsk_timer_id_t*)id));
	}
	return -1;
}

static void *__tsk_timer_manager_loop(void *param)
{
	static tsk_timer_t *curr;
	static uint64_t epoch;

	TSK_DEBUG_INFO("TIMER MANAGER -- START");
	tsk_manager_running = 1;
	
	for(;;)
	{
		tsk_semaphore_decrement(tsk_manager_semaphore);

peek_first:
		if(tsk_manager_break)
		{
			break;
		}

		tsk_mutex_lock(tsk_manager_timers_mutex);
		curr = TSK_TIMER_GET_FIRST();
		tsk_mutex_unlock(tsk_manager_timers_mutex);

		if(curr) 
		{
			epoch = tsk_time_epoch();
			if(epoch >= curr->timeout)
			{
				__tsk_timer_manager_raise(curr);
			}
			else
			{
				if(tsk_condwait_timedwait(tsk_manager_condwait, (curr->timeout - epoch)))
				{
					break;
				}
				else
				{
					goto peek_first;
				}
			}
		}
	}
	
	TSK_DEBUG_INFO("TIMER MANAGER -- STOP");

	tsk_manager_running = 0;
	return 0;
}





















//========================================================
//	Timer object definition
//
static void* tsk_timer_create(void * self, va_list * app)
{
	static tsk_timer_id_t tsk_unique_timer_id = 1;
	tsk_timer_t *timer = self;
	if(timer)
	{
		timer->id = tsk_unique_timer_id++;
		timer->timeout = va_arg(*app, uint64_t);
		timer->callback = va_arg(*app, tsk_timer_callback);
		timer->arg = va_arg(*app, const void *);

		timer->timeout += tsk_time_epoch();
	}
	return self;
}

static void* tsk_timer_destroy(void * self)
{ 
	tsk_timer_t *timer = self;
	
	if(timer)
	{

	}

	return self;
}

static int tsk_timer_cmp(const void *obj1, const void *obj2)
{
	const tsk_timer_t *t1 = obj1;
	const tsk_timer_t *t2 = obj2;

	if(t1 && t2)
	{
		return (int)(t1->timeout - t2->timeout);
	}
	else if(!t1 && !t2) return 0;
	else return -1;
}

static const tsk_object_def_t tsk_timer_def_s = 
{
	sizeof(tsk_timer_t),
	tsk_timer_create, 
	tsk_timer_destroy,
	tsk_timer_cmp, 
};
const void * tsk_timer_def_t = &tsk_timer_def_s;