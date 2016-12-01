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

/**@file tsk_runnable.h
 * @brief Base class for runnable object.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TINYSAK_RUNNABLE_H_
#define _TINYSAK_RUNNABLE_H_

#include "tinysak_config.h"

#include "tsk_object.h"
#include "tsk_semaphore.h"
#include "tsk_thread.h"
#include "tsk_list.h"

TSK_BEGIN_DECLS

/**@ingroup tsk_runnable_group
*/
typedef void* (TSK_STDCALL *tsk_runnable_func_run)(void* self);

/**@ingroup tsk_runnable_group
*/
#define TSK_RUNNABLE(self)	((tsk_runnable_t*)(self))

/**@ingroup tsk_runnable_group
* Runnable.
*/
typedef struct tsk_runnable_s {
    TSK_DECLARE_OBJECT;

    const tsk_object_def_t *objdef;

    tsk_thread_handle_t* h_thread[2/*0=default,1=delayed*/];
    tsk_runnable_func_run run;
    tsk_thread_id_t id_thread; // no way to get this value from "h_thread" on WINXP
    tsk_semaphore_handle_t *semaphore;

    tsk_bool_t running;
    tsk_bool_t started;
    tsk_bool_t initialized;
    /** whether the enqueued data are important or not.
    * if yes, the thread will not be joined until all data in the queue have been consumed.
    * default value: tsk_false
    */
    tsk_bool_t important;

    int32_t priority;

    tsk_list_t *objects;
}
tsk_runnable_t;

/**@ingroup tsk_runnable_group
*/
#define TSK_DECLARE_RUNNABLE tsk_runnable_t __runnable__

TINYSAK_API tsk_runnable_t* tsk_runnable_create();
TINYSAK_API tsk_runnable_t* tsk_runnable_create_2(int32_t priority);

TINYSAK_API int tsk_runnable_start(tsk_runnable_t *self, const tsk_object_def_t *objdef);
TINYSAK_API int tsk_runnable_set_important(tsk_runnable_t *self, tsk_bool_t important);
TINYSAK_API int tsk_runnable_set_priority(tsk_runnable_t *self, int32_t priority);
TINYSAK_API int tsk_runnable_enqueue(tsk_runnable_t *self, ...);
TINYSAK_API int tsk_runnable_stop(tsk_runnable_t *self);

TINYSAK_GEXTERN const tsk_object_def_t *tsk_runnable_def_t;


/**@ingroup tsk_runnable_group
* @def TSK_RUNNABLE_RUN_BEGIN
*/
/**@ingroup tsk_runnable_group
* @def TSK_RUNNABLE_RUN_END
*/
#define TSK_RUNNABLE_RUN_BEGIN(self) \
	TSK_RUNNABLE(self)->running = tsk_true;	\
	TSK_RUNNABLE(self)->id_thread = tsk_thread_get_id(); \
	for(;;) { \
		tsk_semaphore_decrement(TSK_RUNNABLE(self)->semaphore); \
		if(!TSK_RUNNABLE(self)->running &&  \
			(!TSK_RUNNABLE(self)->important || (TSK_RUNNABLE(self)->important && TSK_LIST_IS_EMPTY(TSK_RUNNABLE(self)->objects)))) \
			break;


#define TSK_RUNNABLE_RUN_END(self) \
	} \
	TSK_RUNNABLE(self)->running = tsk_false;

/**@ingroup tsk_runnable_group
* @def TSK_RUNNABLE_ENQUEUE
*/
/**@ingroup tsk_runnable_group
* @def TSK_RUNNABLE_ENQUEUE_OBJECT
*/
#define TSK_RUNNABLE_ENQUEUE(self, ...)												\
{																					\
	if((self) && TSK_RUNNABLE(self)->initialized){												\
		tsk_object_t *object = tsk_object_new(TSK_RUNNABLE(self)->objdef, ##__VA_ARGS__);		\
		tsk_list_push_back_data(TSK_RUNNABLE(self)->objects, (void**)&object);							\
		tsk_semaphore_increment(TSK_RUNNABLE(self)->semaphore);									\
	}																				\
	else{																			\
		TSK_DEBUG_WARN("Invalid/uninitialized runnable object.");					\
	}																				\
}

#define TSK_RUNNABLE_ENQUEUE_OBJECT(self, object)									\
{																					\
	if((self) && TSK_RUNNABLE(self)->initialized){									\
		tsk_list_push_back_data(TSK_RUNNABLE(self)->objects, (void**)&object);		\
		tsk_semaphore_increment(TSK_RUNNABLE(self)->semaphore);						\
	}																				\
	else{																			\
		TSK_DEBUG_WARN("Invalid/uninitialized runnable object.");					\
		TSK_OBJECT_SAFE_FREE(object);												\
	}																				\
}

#define TSK_RUNNABLE_ENQUEUE_OBJECT_SAFE(self, object)								\
{																					\
	if((self) && TSK_RUNNABLE(self)->initialized){									\
		tsk_list_lock(TSK_RUNNABLE(self)->objects);									\
		tsk_list_push_back_data(TSK_RUNNABLE(self)->objects, (void**)&object);		\
		tsk_list_unlock(TSK_RUNNABLE(self)->objects);								\
		tsk_semaphore_increment(TSK_RUNNABLE(self)->semaphore);						\
	}																				\
	else{																			\
		TSK_DEBUG_WARN("Invalid/uninitialized runnable object.");					\
		TSK_OBJECT_SAFE_FREE(object);												\
	}																				\
}

/**@ingroup tsk_runnable_group
*/
#define TSK_RUNNABLE_POP_FIRST(self) \
	tsk_list_pop_first_item(TSK_RUNNABLE(self)->objects)

TSK_GCC_DISABLE_WARNINGS_BEGIN("-Wunused-function")
static tsk_list_item_t* TSK_RUNNABLE_POP_FIRST_SAFE(tsk_runnable_t* self)
{
    tsk_list_item_t* item;
    tsk_list_lock(self->objects);
    item= tsk_list_pop_first_item(self->objects);
    tsk_list_unlock(self->objects);
    return item;
}
TSK_GCC_DISABLE_WARNINGS_END()

TSK_END_DECLS

#endif /* _TINYSAK_RUNNABLE_H_ */

