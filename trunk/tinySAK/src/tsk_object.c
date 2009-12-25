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

/**@file tsk_object.c
 * @brief Base object implementation.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tsk_object.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

typedef struct tsk_object_header_s
{
	const void* base;
	size_t	refCount;
}
tsk_object_header_t;
#define TSK_OBJECT_HEADER_GET(object)	((tsk_object_header_t*)object)

void* tsk_object_new(const tsk_object_def_t *objdef, ...)
{
	void *newobj = tsk_calloc(1, objdef->size);
	if(newobj)
	{
		(*(const tsk_object_def_t **) newobj) = objdef;
		TSK_OBJECT_HEADER_GET(newobj)->refCount = 1;
		if(objdef->constructor)
		{ 
			va_list ap;
			va_start(ap, objdef);
			newobj = objdef->constructor(newobj, &ap);
			va_end(ap);
		}
		else
		{
			TSK_DEBUG_WARN("No constructor found.");
		}
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new tsk_object.");
	}

	return newobj;
}

void* tsk_object_new2(const tsk_object_def_t *objdef, va_list* ap)
{
	void *newobj = tsk_calloc(1, objdef->size);
	if(newobj)
	{
		(*(const tsk_object_def_t **) newobj) = objdef;
		TSK_OBJECT_HEADER_GET(newobj)->refCount = 1;
		if(objdef->constructor)
		{ 
			newobj = objdef->constructor(newobj, ap);
		}
		else
		{
			TSK_DEBUG_WARN("No constructor found.");
		}
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new tsk_object.");
	}

	return newobj;
}

size_t tsk_object_sizeof(const void *self)
{
	const tsk_object_def_t **objdef = self;
	if(objdef && *objdef)
	{
		return (*objdef)->size;
	}
	else
	{
		TSK_DEBUG_ERROR("NULL tsk obkect.");
		return 0;
	}
}

int tsk_object_cmp(const void *self, const void *object)
{
	const tsk_object_def_t **objdef = self;
	if(objdef && *objdef && (*objdef)->objcmp)
	{
		return (*objdef)->objcmp(self, object);
	}
	return 0;
}

/*int tsk_object_icmp(const void *self, const void *object)
{
	const tsk_object_def_t **objdef = self;
	if(objdef && *objdef && (*objdef)->objicmp)
	{
		return (*objdef)->objicmp(self, object);
	}
	return 0;
}*/

void* tsk_object_ref(void *self)
{
	if(self)
	{
		TSK_OBJECT_HEADER_GET(self)->refCount++;
		return self;
	}
	return 0;
}

void* tsk_object_unref(void *self)
{
	if(self)
	{
		if(!--(TSK_OBJECT_HEADER_GET(self)->refCount))
		{
			tsk_object_delete(self);
			return 0;
		}
	}
	return self;
}

void tsk_object_delete(void *self)
{
	const tsk_object_def_t ** objdef = self;
	if(self && *objdef)
	{
		if((*objdef)->destructor)
		{
			self = (*objdef)->destructor(self);
		}
		else
		{
			TSK_DEBUG_WARN("No destructor found.");
		}
		free(self);
	}
}