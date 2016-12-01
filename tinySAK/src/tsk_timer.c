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

/**@file tsk_timer.c
* @brief Timer Manager.
*
* @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

#if TSK_UNDER_WINDOWS
#	include <windows.h>
#endif /* TSK_UNDER_WINDOWS */


/**@defgroup tsk_timer_group Timers Management
*/

#define TSK_TIMER_CREATE(timeout, callback, arg)	tsk_object_new(tsk_timer_def_t, timeout, callback, arg)
#define TSK_TIMER_TIMEOUT(self)						((tsk_timer_t*)self)->timeout
#define TSK_TIMER_GET_FIRST()						(manager->timers && manager->timers->head) ? (tsk_timer_t*)(((tsk_list_item_t*)(manager->timers->head))->data) : 0

/**
 * @struct	tsk_timer_s
 * @brief	Timer.
**/
typedef struct tsk_timer_s {
    TSK_DECLARE_OBJECT;

    tsk_timer_id_t id;	/**< Unique timer identifier. */
    const void *arg; /**< Opaque data to return with the callback function. */
    uint64_t timeout; /**< When the timer will timeout(as EPOCH time). */
    tsk_timer_callback_f callback; /**< The callback function to call after @ref timeout milliseconds. */

    unsigned canceled:1;
}
tsk_timer_t;
typedef tsk_list_t tsk_timers_L_t; /**< List of @ref tsk_timer_t elements. */

/**
 * @struct	tsk_timer_manager_s
 *
 * @brief	Timer manager.
**/
typedef struct tsk_timer_manager_s {
    TSK_DECLARE_RUNNABLE;

    void* mainThreadId[1];
    tsk_condwait_handle_t *condwait;
    tsk_mutex_handle_t *mutex;
    tsk_semaphore_handle_t *sem;

    tsk_timers_L_t *timers;
}
tsk_timer_manager_t;
typedef tsk_list_t tsk_timer_manager_L_t; /**< List of @ref tsk_timer_manager_t elements. */

/*== Definitions */
static void* TSK_STDCALL __tsk_timer_manager_mainthread(void *param);
static int __tsk_pred_find_timer_by_id(const tsk_list_item_t *item, const void *id);
static void* TSK_STDCALL run(void* self);

/**@ingroup tsk_timer_group
*/
tsk_timer_manager_handle_t* tsk_timer_manager_create()
{
    return tsk_object_new(tsk_timer_manager_def_t);
}

/**@ingroup tsk_timer_group
* Starts the timer manager.
*/
int tsk_timer_manager_start(tsk_timer_manager_handle_t *self)
{
    int err = -1;
    tsk_timer_manager_t *manager = (tsk_timer_manager_t*)self;

    TSK_DEBUG_INFO("tsk_timer_manager_start");

    if(!manager) {
        return -1;
    }

    tsk_mutex_lock(manager->mutex);

    if(!TSK_RUNNABLE(manager)->running && !TSK_RUNNABLE(manager)->started) {
        TSK_RUNNABLE(manager)->run = run;
        if((err = tsk_runnable_start(TSK_RUNNABLE(manager), tsk_timer_def_t))) {
            //TSK_OBJECT_SAFE_FREE(manager);
            goto bail;
        }
    }
    else {
        TSK_DEBUG_INFO("Timer manager already running");
        err = 0;
    }

bail:
    tsk_mutex_unlock(manager->mutex);

    return err;
}

#if defined(DEBUG) || defined(_DEBUG) || !defined(NDEBUG)
/**@ingroup tsk_timer_group
*/
void tsk_timer_manager_debug(tsk_timer_manager_handle_t *self)
{
    tsk_timer_manager_t *manager = (tsk_timer_manager_t*)self;
    if(manager) {
        //int index = 0;
        tsk_list_item_t *item = tsk_null;

        tsk_mutex_lock(manager->mutex);

        tsk_list_foreach(item, manager->timers) {
            tsk_timer_t* timer = (tsk_timer_t*)item->data;
            TSK_DEBUG_INFO("timer [%ld]- %llu, %llu", timer->id, timer->timeout, tsk_time_now());
        }

        tsk_mutex_unlock(manager->mutex);
    }
}
#endif

/**@ingroup tsk_timer_group
*/
int tsk_timer_manager_stop(tsk_timer_manager_handle_t *self)
{
    int ret = -1;
    tsk_timer_manager_t *manager = (tsk_timer_manager_t*)self;

    if(!manager) {
        TSK_DEBUG_ERROR("Invalid paramater");
        return -1;
    }

    // all functions called below are thread-safe ==> do not lock
    // "mainthread" uses manager->mutex and runs in a separate thread ==> deadlock

    if(TSK_RUNNABLE(manager)->running) {
        if((ret = tsk_runnable_stop(TSK_RUNNABLE(manager)))) {
            goto bail;
        }

        tsk_semaphore_increment(manager->sem);
        tsk_condwait_signal(manager->condwait);

        ret = tsk_thread_join(manager->mainThreadId);
        goto bail;
    }
    else {
        ret = 0; /* already running. */
        goto bail;
    }

bail:
    tsk_list_clear_items(manager->timers);
    return ret;
}

/**@ingroup tsk_timer_group
*/
tsk_timer_id_t tsk_timer_manager_schedule(tsk_timer_manager_handle_t *self, uint64_t timeout, tsk_timer_callback_f callback, const void *arg)
{
    tsk_timer_id_t timer_id = TSK_INVALID_TIMER_ID;
    tsk_timer_manager_t *manager = (tsk_timer_manager_t*)self;

    if(manager && (TSK_RUNNABLE(manager)->running || TSK_RUNNABLE(manager)->started)) {
        tsk_timer_t *timer;

        timer = (tsk_timer_t*)TSK_TIMER_CREATE(timeout, callback, arg);
        timer_id = timer->id;
        tsk_mutex_lock(manager->mutex);
        tsk_list_push_ascending_data(manager->timers, ((void**) &timer));
        tsk_mutex_unlock(manager->mutex);

        // tsk_timer_manager_debug(self);

        tsk_condwait_signal(manager->condwait);
        tsk_semaphore_increment(manager->sem);
    }

    return timer_id;
}

/**@ingroup tsk_timer_group
*/
int tsk_timer_manager_cancel(tsk_timer_manager_handle_t *self, tsk_timer_id_t id)
{
    int ret = -1;
    tsk_timer_manager_t *manager = (tsk_timer_manager_t*)self;

    /* Check validity. */
    if(!TSK_TIMER_ID_IS_VALID(id)) { /* Very common. */
        return 0;
    }

    if(!TSK_LIST_IS_EMPTY(manager->timers) && TSK_RUNNABLE(manager)->running) {
        const tsk_list_item_t *item;
        tsk_mutex_lock(manager->mutex);
        item = tsk_list_find_item_by_pred(manager->timers, __tsk_pred_find_timer_by_id, &id);
        if(item && item->data) {
            tsk_timer_t *timer = (tsk_timer_t*)item->data;
            timer->canceled = 1;
            timer->callback = tsk_null;

            if(item == manager->timers->head) {
                /* The timer we are waiting on ? ==> remove it now. */
                tsk_condwait_signal(manager->condwait);
            }

            ret = 0;
        }
        tsk_mutex_unlock(manager->mutex);
    }
    return ret;
}

int tsk_timer_manager_destroy(tsk_timer_manager_handle_t **self)
{
    if(!self || !*self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    TSK_OBJECT_SAFE_FREE(*self);
    return 0;
}

static void* TSK_STDCALL run(void* self)
{
    int ret;
    tsk_list_item_t *curr;
    tsk_timer_manager_t *manager = (tsk_timer_manager_t*)self;

    TSK_RUNNABLE(manager)->running = tsk_true; // VERY IMPORTANT --> needed by the main thread

    /* create main thread */
    if((ret = tsk_thread_create(&(manager->mainThreadId[0]), __tsk_timer_manager_mainthread, manager))) {
        TSK_DEBUG_FATAL("Failed to create mainthread: %d\n", ret);
        return tsk_null;
    }

    TSK_DEBUG_INFO("Timer manager run()::enter");

    TSK_RUNNABLE_RUN_BEGIN(manager);

    if((curr = TSK_RUNNABLE_POP_FIRST_SAFE(TSK_RUNNABLE(manager)))) {
        tsk_timer_t *timer = (tsk_timer_t *)curr->data;
        if(timer->callback) {
            timer->callback(timer->arg, timer->id);
        }
        tsk_object_unref(curr);
    }

    TSK_RUNNABLE_RUN_END(manager);

    TSK_DEBUG_INFO("Timer manager run()::exit");

    return tsk_null;
}

static int __tsk_pred_find_timer_by_id(const tsk_list_item_t *item, const void *id)
{
    tsk_timer_t *timer;
    if(item && item->data) {
        timer = (tsk_timer_t*)item->data;
        return (int)(timer->id - *((tsk_timer_id_t*)id));
    }
    return -1;
}

static void* TSK_STDCALL __tsk_timer_manager_mainthread(void *param)
{
    int ret;
    tsk_timer_t *curr;
    uint64_t now;
    tsk_timer_manager_t *manager = (tsk_timer_manager_t*)param;

    TSK_DEBUG_INFO("TIMER MANAGER -- START");

    while(TSK_RUNNABLE(manager)->running) {
        tsk_semaphore_decrement(manager->sem);

peek_first:
        if(!TSK_RUNNABLE(manager)->running) {
            break;
        }

        tsk_mutex_lock(manager->mutex);
        curr = TSK_TIMER_GET_FIRST();
        tsk_mutex_unlock(manager->mutex);

        if (curr && !curr->canceled) {
            now = tsk_time_now();
            if (now >= curr->timeout) {
                tsk_timer_t *timer = (tsk_timer_t*)tsk_object_ref(curr);
                //TSK_DEBUG_INFO("Timer raise %llu", timer->id);

                tsk_mutex_lock(manager->mutex); // must lock() before enqueue()
                TSK_RUNNABLE_ENQUEUE_OBJECT_SAFE(TSK_RUNNABLE(manager), timer);
                tsk_list_remove_item_by_data(manager->timers, curr);
                tsk_mutex_unlock(manager->mutex);
                TSK_OBJECT_SAFE_FREE(timer);
            }
            else {
                if((ret = tsk_condwait_timedwait(manager->condwait, (curr->timeout - now)))) {
                    TSK_DEBUG_ERROR("CONWAIT for timer manager failed [%d]", ret);
                    break;
                }
                else {
                    goto peek_first;
                }
            }
        }
        else if(curr) {
            tsk_mutex_lock(manager->mutex);
            /* TSK_DEBUG_INFO("Timer canceled %llu", curr->id); */
            tsk_list_remove_item_by_data(manager->timers, curr);
            tsk_mutex_unlock(manager->mutex);
        }
    } /* while() */

    TSK_DEBUG_INFO("TIMER MANAGER -- STOP");

    return tsk_null;
}





/* ================= Global Timer Manager ================= */

static tsk_timer_manager_t* __timer_mgr = tsk_null;

tsk_timer_manager_handle_t* tsk_timer_mgr_global_ref()
{
    if(!__timer_mgr) {
        __timer_mgr = (tsk_timer_manager_t*)tsk_timer_manager_create();
    }
    else {
        __timer_mgr = (tsk_timer_manager_t*)tsk_object_ref(__timer_mgr);
    }
    return __timer_mgr;
}

int tsk_timer_mgr_global_start()
{
    int ret = 0;
    if(!__timer_mgr) {
        TSK_DEBUG_ERROR("No global Timer manager could be found");
        return -1;
    }
    if(!TSK_RUNNABLE(__timer_mgr)->running && !TSK_RUNNABLE(__timer_mgr)->started) {
        if((ret = tsk_timer_manager_start(__timer_mgr))) {
            return ret;
        }
    }
    tsk_mutex_lock(__timer_mgr->mutex);
    tsk_mutex_unlock(__timer_mgr->mutex);
    return ret;
}

tsk_timer_id_t tsk_timer_mgr_global_schedule(uint64_t timeout, tsk_timer_callback_f callback, const void *arg)
{
    if(!__timer_mgr) {
        TSK_DEBUG_ERROR("No global Timer manager could be found");
        return TSK_INVALID_TIMER_ID;
    }
    return tsk_timer_manager_schedule(__timer_mgr, timeout, callback, arg);
}

int tsk_timer_mgr_global_cancel(tsk_timer_id_t id)
{
    if(!__timer_mgr) {
        TSK_DEBUG_ERROR("No global Timer manager could be found");
        return -1;
    }
    return tsk_timer_manager_cancel(__timer_mgr, id);
}

int tsk_timer_mgr_global_unref(tsk_timer_manager_handle_t** mgr_global)
{
    if(!mgr_global || !*mgr_global) {
        return 0;
    }
    if((*mgr_global != __timer_mgr)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    __timer_mgr = (tsk_timer_manager_t*)tsk_object_unref(TSK_OBJECT(*mgr_global));
    *mgr_global = tsk_null;
    return 0;
}







//=================================================================================================
//	Timer manager object definition
//
static tsk_object_t* tsk_timer_manager_ctor(tsk_object_t * self, va_list * app)
{
    tsk_timer_manager_t *manager = (tsk_timer_manager_t*)self;
    if(manager) {
        manager->timers = tsk_list_create();
        manager->sem = tsk_semaphore_create();
        manager->condwait = tsk_condwait_create();
        manager->mutex = tsk_mutex_create();
    }
    return self;
}

static tsk_object_t* tsk_timer_manager_dtor(tsk_object_t * self)
{
    tsk_timer_manager_t *manager = (tsk_timer_manager_t*)self;

    if(manager) {
        tsk_timer_manager_stop(manager);

        tsk_semaphore_destroy(&manager->sem);
        tsk_condwait_destroy(&manager->condwait);
        tsk_mutex_destroy(&manager->mutex);
        TSK_OBJECT_SAFE_FREE(manager->timers);
    }

    return self;
}

static const tsk_object_def_t tsk_timer_manager_def_s = {
    sizeof(tsk_timer_manager_t),
    tsk_timer_manager_ctor,
    tsk_timer_manager_dtor,
    tsk_null,
};
const tsk_object_def_t * tsk_timer_manager_def_t = &tsk_timer_manager_def_s;






//=================================================================================================
//	Timer object definition
//
static tsk_object_t* tsk_timer_ctor(tsk_object_t * self, va_list * app)
{
    static volatile tsk_timer_id_t __tsk_unique_timer_id = 0;
    tsk_timer_t *timer = (tsk_timer_t*)self;
    if(timer) {
        tsk_atomic_inc(&__tsk_unique_timer_id);
        timer->id = __tsk_unique_timer_id;
        timer->timeout = va_arg(*app, uint64_t);
        timer->callback = va_arg(*app, tsk_timer_callback_f);
        timer->arg = va_arg(*app, const void *);

        timer->timeout += tsk_time_now();
    }
    return self;
}

static tsk_object_t* tsk_timer_dtor(tsk_object_t * self)
{
    tsk_timer_t *timer = (tsk_timer_t*)self;
    if(timer) {
    }

    return self;
}

static int tsk_timer_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
    const tsk_timer_t *t1 = (const tsk_timer_t *)obj1;
    const tsk_timer_t *t2 = (const tsk_timer_t *)obj2;

    if(t1 && t2) {
        return (int)(t1->timeout - t2->timeout);
    }
    else if(!t1 && !t2) {
        return 0;
    }
    else {
        return -1;
    }
}

static const tsk_object_def_t tsk_timer_def_s = {
    sizeof(tsk_timer_t),
    tsk_timer_ctor,
    tsk_timer_dtor,
    tsk_timer_cmp,
};
const tsk_object_def_t * tsk_timer_def_t = &tsk_timer_def_s;


