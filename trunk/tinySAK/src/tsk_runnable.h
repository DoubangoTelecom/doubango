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
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsk_runnable.h
 * @brief Base class for runnable object.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TINYSAK_RUNNABLE_H_
#define _TINYSAK_RUNNABLE_H_

#include "tinySAK_config.h"

#include "tsk_object.h"
#include "tsk_semaphore.h"
#include "tsk_list.h"

TSK_BEGIN_DECLS

/**@ingroup tsk_runnable_group
*/
typedef void * (*tsk_runnable_func_run)(void* self);

/**@ingroup tsk_runnable_group
*/
#define TSK_RUNNABLE(self)	((tsk_runnable_t*)(self))

/**@ingroup tsk_runnable_group
* @def TSK_RUNNABLE_RUN_BEGIN
*/
/**@ingroup tsk_runnable_group
* @def TSK_RUNNABLE_RUN_END
*/
#define TSK_RUNNABLE_RUN_BEGIN(self)						\
	TSK_RUNNABLE(self)->running = 1;						\
	for(;;) {												\
	tsk_semaphore_decrement(TSK_RUNNABLE(self)->semaphore); \
	if(!TSK_RUNNABLE(self)->running) break;					\

#define TSK_RUNNABLE_RUN_END(self)							\
	}														\
	TSK_RUNNABLE(self)->running = 0;

/**@ingroup tsk_runnable_group
* @def TSK_RUNNABLE_ENQUEUE
*/
/**@ingroup tsk_runnable_group
* @def TSK_RUNNABLE_ENQUEUE_OBJECT
*/
#define TSK_RUNNABLE_ENQUEUE(self, ...)												\
{																					\
	if(TSK_RUNNABLE(self) && TSK_RUNNABLE(self)->initialized)						\
	{																				\
		void *object = tsk_object_new(TSK_RUNNABLE(self)->objdef, __VA_ARGS__);		\
		tsk_list_push_back_data(TSK_RUNNABLE(self)->objects, &object);				\
		tsk_semaphore_increment(TSK_RUNNABLE(self)->semaphore);						\
	}																				\
	else																			\
		TSK_DEBUG_WARN("Invalid/uninitialized runnable object.");					\
}

#define TSK_RUNNABLE_ENQUEUE_OBJECT(self, object)									\
{																					\
	if(TSK_RUNNABLE(self) && TSK_RUNNABLE(self)->initialized)						\
	{																				\
		tsk_list_push_back_data(TSK_RUNNABLE(self)->objects, (void**)&object);		\
		tsk_semaphore_increment(TSK_RUNNABLE(self)->semaphore);						\
	}																				\
	else																			\
		TSK_DEBUG_WARN("Invalid/uninitialized runnable object.");					\
}

/**@ingroup tsk_runnable_group
*/
#define TSK_RUNNABLE_POP_FIRST(self)	tsk_list_pop_first_item(TSK_RUNNABLE(self)->objects)


/**@ingroup tsk_runnable_group
* Runnable.
*/
typedef struct tsk_runnable_s
{
	TSK_DECLARE_OBJECT;
	
	const tsk_object_def_t *objdef;
	
	void* tid[1];
	tsk_runnable_func_run run;
	tsk_semaphore_handle_t *semaphore;
	
	tsk_bool_t running;
	tsk_bool_t initialized;
	
	tsk_list_t *objects;
}
tsk_runnable_t;

/**@ingroup tsk_runnable_group
*/
#define TSK_DECLARE_RUNNABLE tsk_runnable_t runnable

TINYSAK_API int tsk_runnable_start(tsk_runnable_t *self, const tsk_object_def_t *objdef);
TINYSAK_API int tsk_runnable_enqueue(tsk_runnable_t *self, ...);
TINYSAK_API int tsk_runnable_stop(tsk_runnable_t *self);

TSK_END_DECLS

#endif /* _TINYSAK_RUNNABLE_H_ */

