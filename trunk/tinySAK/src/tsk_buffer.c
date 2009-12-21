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

/**@file tsk_buffer.c
 * @brief Data buffer.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tsk_buffer.h"
#include "tsk_memory.h"



















//========================================================
//	Buffer object definition
//
static void* tsk_buffer_create(void * self, va_list * app)
{
	tsk_buffer_t *buffer = self;
	const void *data = va_arg(*app, const void *);
	size_t size = va_arg(*app, size_t);
	
	if(data && size)
	{
		buffer->data = tsk_calloc((size+1), sizeof(char*));
		memcpy(buffer->data, data, size);
		buffer->size = size;
	}
	return self;
}

static void* tsk_buffer_destroy(void * self)
{ 
	tsk_buffer_t *buffer = self;
	if(buffer)
	{
		TSK_FREE(buffer->data);
		buffer->size = 0;
	}

	return self;
}

static int tsk_buffer_cmp(const void *obj1, const void *obj2)
{
	return -1;
}

static const tsk_object_def_t tsk_buffer_def_s = 
{
	sizeof(tsk_buffer_t),
	tsk_buffer_create, 
	tsk_buffer_destroy,
	tsk_buffer_cmp, 
};
const void *tsk_buffer_def_t = &tsk_buffer_def_s;