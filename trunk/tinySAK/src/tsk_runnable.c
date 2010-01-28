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

/**@file tsk_runnable.c
 * @brief Base class for runnable object.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tsk_runnable.h"
#include "tsk_thread.h"
#include "tsk_debug.h"


/**
* Initializes a runnable object and allocate it's internal fields.
* @param self The runnable object to initialize.
* @param objdef Internal objects definition class.
* @retval Zero if succeed and nonzero error code otherwise.
* @sa tsk_runnable_deinit.
*/
static int tsk_runnable_init(tsk_runnable_t *self, const tsk_object_def_t *objdef)
{
	if(self)
	{
		if(self->initialized) return -2;
		if(!objdef) return -3;
		
		self->semaphore = tsk_semaphore_create();
		self->objdef = objdef;
		self->objects = TSK_LIST_CREATE();

		self->initialized = 1;
		return 0;
	}
	return -1;
}

/**
* DeInitializes a runnable object.
* @param self The runnable object to deinitialize.
* @retval Zero if succeed and nonzero error code otherwise.
* @sa tsk_runnable_init.
*/
static int tsk_runnable_deinit(tsk_runnable_t *self)
{
	if(self)
	{
		if(!self->initialized) return -2;
		else if(self->running) return -3;

		tsk_semaphore_destroy(&self->semaphore);
		TSK_OBJECT_SAFE_FREE(self->objects);

		self->initialized = 0;

		return 0;
	}
	return -1;
}

/**
* Starts a runnable object.
* @param self The runnable object to start.
* @retval Zero if succeed and nonzero error code otherwise.
* @sa tsk_runnable_stop.
*/
int tsk_runnable_start(tsk_runnable_t *self, const tsk_object_def_t *objdef)
{
	int ret = -1;
	if(self)
	{
		if(self->running) return -2;
		else if(!self->run) return -3;
		else if(tsk_runnable_init(self, objdef)) return -4;

		/* call run() function in new thread. */
		if(ret = tsk_thread_create(&(self->tid[0]), self->run, self))
		{
			TSK_DEBUG_ERROR("Failed to start new thread.");
			return ret;
		}
		self->running = 1;
	}
	return ret;
}

/**
* Stops a runnable object.
* @param self The runnable object to stop.
* @retval Zero if succeed and nonzero error code otherwise.
* @sa tsk_runnable_start.
*/
int tsk_runnable_stop(tsk_runnable_t *self)
{
	int ret = -1;
	if(self)
	{
		if(!self->initialized) 
		{
			return -2;
		}
		else if(!self->running) 
		{
			return -3;
		}

		self->running = 0;
		tsk_semaphore_increment(self->semaphore);

		if(ret = tsk_thread_join(&(self->tid[0])))
		{
			self->running = 1;
			TSK_DEBUG_ERROR("Failed to join a thread.");
			return ret;
		}
		tsk_runnable_deinit(self);
	}
	return ret;
}

