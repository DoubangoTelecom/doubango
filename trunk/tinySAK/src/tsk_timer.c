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
#include "tsk_runnable.h"
#include "tsk_condwait.h"
#include "tsk_semaphore.h"
#include "tsk_time.h"


/**@defgroup tsk_timer_group Timers Management
*/

#define TSK_TIMER_CREATE(timeout, callback, arg)	tsk_object_new(tsk_timer_def_t, timeout, callback, arg)
#define TSK_TIMER_TIMEOUT(self)						((tsk_timer_t*)self)->timeout
#define TSK_TIMER_GET_FIRST()						(manager->timers && manager->timers->head) ? (tsk_timer_t*)(((tsk_list_item_t*)(manager->timers->head))->data) : 0

/**
 * @struct	tsk_timer_s
 *
 * @brief	Timer.
 *
 * @author	Mamadou
 * @date	12/20/2009
**/
typedef struct tsk_timer_s
{
	TSK_DECLARE_OBJECT;

	tsk_timer_id_t id;	/**< Unique timer identifier. */
	const void *arg; /**< Opaque data to return with the callback function. */
	uint64_t timeout; /**< When the timer will timeout(as EPOCH time). */
	tsk_timer_callback callback; /**< The callback function to call after @ref timeout milliseconds. */

	unsigned canceled:1;
}
tsk_timer_t;
typedef tsk_list_t tsk_timers_L_t; /**< List of @ref tsk_timer_t elements. */

/**
 * @struct	tsk_timer_manager_s
 *
 * @brief	Timer manager.
 *
 * @author	Mamadou
 * @date	12/20/2009
**/
typedef struct tsk_timer_manager_s
{
	TSK_DECLARE_RUNNABLE;

	unsigned active:1;
	void* mainThreadId[1];
	tsk_condwait_handle_t *condwait;
	tsk_mutex_handle_t *mutex;
	tsk_semaphore_handle_t *sem;

	tsk_timers_L_t *timers;
}
tsk_timer_manager_t;
typedef tsk_list_t tsk_timer_manager_L_t; /**< List of @ref tsk_timer_manager_t elements. */

/*== Definitions */
static void *__tsk_timer_manager_mainthread(void *param); 
static int __tsk_pred_find_timer_by_id(const tsk_list_item_t *item, const void *id);
static void __tsk_timer_manager_raise(tsk_timer_t *timer);
static void *run(void* self);

/**@ingroup tsk_timer_group
* Start the timer manager.
*/
int tsk_timer_manager_start(tsk_timer_manager_handle_t *self)
{
	int err = -1;
	tsk_timer_manager_t *manager = self;
	if(manager && !TSK_RUNNABLE(manager)->running)
	{				
		TSK_RUNNABLE(manager)->run = run;
		if(err = tsk_runnable_start(TSK_RUNNABLE(manager), tsk_timer_def_t))
		{
			TSK_OBJECT_SAFE_FREE(manager);
			return err;
		}

		if(err = tsk_thread_create(&(manager->mainThreadId[0]), __tsk_timer_manager_mainthread, manager))
		{
			TSK_OBJECT_SAFE_FREE(manager);
			
			TSK_DEBUG_FATAL("Failed to start timer manager: %d\n", err);
			return err;
		}
	}
	
	return err;
}

int tsk_timer_manager_isready(tsk_timer_manager_handle_t *self)
{
	tsk_timer_manager_t *manager = self;
	if(manager)
	{
		return (TSK_RUNNABLE(manager)->running && manager->active);
	}
	return 0;
}

#if defined(DEBUG) || defined(_DEBUG) || !defined(NDEBUG)
void tsk_timer_manager_debug(tsk_timer_manager_handle_t *self)
{
	tsk_timer_manager_t *manager = self;
	if(manager)
	{
		int index = 0;
		tsk_list_item_t *item = 0;

		tsk_mutex_lock(manager->mutex);
		
		tsk_list_foreach(item, manager->timers)
		{
			tsk_timer_t* timer = item->data;
			TSK_DEBUG_INFO("timer [%d]- %llu, %llu", timer->id, timer->timeout, tsk_time_epoch());
		}

		tsk_mutex_unlock(manager->mutex);
	}
}
#endif

int tsk_timer_manager_stop(tsk_timer_manager_handle_t *self)
{
	int ret = -1;
	tsk_timer_manager_t *manager = self;
	if(manager && TSK_RUNNABLE(manager)->running)
	{
		if(ret = tsk_runnable_stop(TSK_RUNNABLE(manager)))
		{
			return ret;
		}
		
		tsk_semaphore_increment(manager->sem);
		tsk_condwait_signal(manager->condwait);
		
		return tsk_thread_join(manager->mainThreadId);
	}
	return ret;
}

tsk_timer_id_t tsk_timer_manager_schedule(tsk_timer_manager_handle_t *self, uint64_t timeout, tsk_timer_callback callback, const void *arg)
{
	tsk_timer_id_t timer_id = TSK_INVALID_TIMER_ID;
	tsk_timer_manager_t *manager = self;

	if(manager && TSK_RUNNABLE(manager)->running)
	{
		tsk_timer_t *timer;

		timer = TSK_TIMER_CREATE(timeout, callback, arg);
		timer_id = timer->id;
		tsk_mutex_lock(manager->mutex);
		tsk_list_push_ascending_data(manager->timers, ((void**) &timer));
		tsk_mutex_unlock(manager->mutex);
		
		tsk_timer_manager_debug(self);

		tsk_condwait_signal(manager->condwait);
		tsk_semaphore_increment(manager->sem);
	}

	return timer_id;
}

int tsk_timer_manager_cancel(tsk_timer_manager_handle_t *self, tsk_timer_id_t id)
{
	int ret = -1;
	tsk_timer_manager_t *manager = self;
	if(!TSK_LIST_IS_EMPTY(manager->timers) && TSK_RUNNABLE(manager)->running)
	{
		const tsk_list_item_t *item;
		tsk_mutex_lock(manager->mutex);
		item = tsk_list_find_item_by_pred(manager->timers, __tsk_pred_find_timer_by_id, &id);
		if(item && item->data)
		{
			tsk_timer_t *timer = item->data;
			timer->canceled = 1;
			
			if(item == manager->timers->head)
			{	/* The timer we are waiting on ? ==> remove it now. */
				tsk_condwait_signal(manager->condwait);
			}
			
			ret = 0;
		}
		tsk_mutex_unlock(manager->mutex);
	}
	return ret;
}

static void *run(void* self)
{
	tsk_list_item_t *curr;
	tsk_timer_manager_t *manager = self;
	
	TSK_RUNNABLE_RUN_BEGIN(manager);

	if(curr = TSK_RUNNABLE_POP_FIRST(manager))
	{
		tsk_timer_t *timer = (tsk_timer_t *)curr->data;
		if(timer->callback)
		{
			timer->callback(timer->arg, timer->id);
		}
		tsk_object_unref(curr);
	}

	TSK_RUNNABLE_RUN_END(manager);

	return 0;
}

static int __tsk_pred_find_timer_by_id(const tsk_list_item_t *item, const void *id)
{
	tsk_timer_t *timer;
	if(item && item->data)
	{
		timer = item->data;
		return (int)(timer->id - *((tsk_timer_id_t*)id));
	}
	return -1;
}

static void *__tsk_timer_manager_mainthread(void *param)
{
	tsk_timer_t *curr;
	uint64_t epoch;
	tsk_timer_manager_t *manager = param;

	TSK_DEBUG_INFO("TIMER MANAGER -- START");
	
	manager->active = 1;
	while(TSK_RUNNABLE(manager)->running)
	{
		tsk_semaphore_decrement(manager->sem);

peek_first:
		if(!TSK_RUNNABLE(manager)->running)
		{
			break;
		}

		tsk_mutex_lock(manager->mutex);
		curr = TSK_TIMER_GET_FIRST();
		tsk_mutex_unlock(manager->mutex);

		if(curr && !curr->canceled) 
		{
			epoch = tsk_time_epoch();
			if(epoch >= curr->timeout)
			{
				tsk_timer_t *timer = tsk_object_ref(curr);

				tsk_mutex_lock(manager->mutex);
				TSK_RUNNABLE_ENQUEUE_OBJECT(TSK_RUNNABLE(manager), timer);
				tsk_list_remove_item_by_data(manager->timers, curr);
				tsk_mutex_unlock(manager->mutex);
			}
			else
			{
				if(tsk_condwait_timedwait(manager->condwait, (curr->timeout - epoch)))
				{
					break;
				}
				else
				{
					goto peek_first;
				}
			}
		}
		else if(curr)
		{
			tsk_mutex_lock(manager->mutex);
			tsk_list_remove_item_by_data(manager->timers, curr);
			tsk_mutex_unlock(manager->mutex);
		}
	}
	
	manager->active = 0;
	TSK_DEBUG_INFO("TIMER MANAGER -- STOP");

	return 0;
}

















//========================================================
//	Timer object definition
//
static void* tsk_timer_manager_create(void * self, va_list * app)
{
	tsk_timer_manager_t *manager = self;
	if(manager)
	{
		manager->timers = TSK_LIST_CREATE();
		manager->sem = tsk_semaphore_create();
		manager->condwait = tsk_condwait_create();
		manager->mutex = tsk_mutex_create();
	}
	return self;
}

static void* tsk_timer_manager_destroy(void * self)
{ 
	tsk_timer_manager_t *manager = self;
	
	if(manager)
	{
		tsk_timer_manager_stop(manager);

		tsk_semaphore_destroy(&manager->sem);
		tsk_condwait_destroy(&manager->condwait);
		tsk_mutex_destroy(&manager->mutex);
		tsk_object_unref(manager->timers);
	}

	return self;
}

static const tsk_object_def_t tsk_timer_manager_def_s = 
{
	sizeof(tsk_timer_manager_t),
	tsk_timer_manager_create, 
	tsk_timer_manager_destroy,
	0, 
};
const void * tsk_timer_manager_def_t = &tsk_timer_manager_def_s;






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

