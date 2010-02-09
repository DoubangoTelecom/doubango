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

/**@file tsk_buffer.h
 * @brief Data buffer.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TINYSAK_BUFFER_H_
#define _TINYSAK_BUFFER_H_

#include "tinySAK_config.h"
#include "tsk_list.h"

TSK_BEGIN_DECLS

#define TSK_BUFFER_CREATE(data, size)		tsk_object_new(tsk_buffer_def_t, (const void*)data, (size_t)size)
#define TSK_BUFFER_CREATE_NULL()			TSK_BUFFER_CREATE(0,0)

#define TSK_BUFFER(self)					((tsk_buffer_t*)self)
#define TSK_BUFFER_DATA(self)				(self ? TSK_BUFFER(self)->data : 0)
#define TSK_BUFFER_SIZE(self)				(self ? TSK_BUFFER(self)->size : 0)

#define TSK_BUFFER_TO_STRING(self)			(self ? (const char*)TSK_BUFFER_DATA(self) : 0)
#define TSK_BUFFER_TO_U8(self)				(self ? (uint8_t*)TSK_BUFFER_DATA(self) : 0)

typedef struct tsk_buffer_s
{
	TSK_DECLARE_OBJECT;

	void *data;
	size_t size;
}
tsk_buffer_t;

typedef tsk_list_t tsk_buffers_L_t; /**< List of @ref tsk_buffer_t elements. */

TINYSAK_API int tsk_buffer_appendEx(tsk_buffer_t* self, const char* format, ...);
TINYSAK_API int tsk_buffer_append(tsk_buffer_t* self, const void* data, size_t size);
TINYSAK_API int tsk_buffer_realloc(tsk_buffer_t* self, size_t size);
TINYSAK_API int tsk_buffer_remove(tsk_buffer_t* self, size_t position, size_t size);
TINYSAK_API int tsk_buffer_insert(tsk_buffer_t* self, size_t position, const void*data, size_t size);
TINYSAK_API int tsk_buffer_cleanup(tsk_buffer_t* self);

TINYSAK_GEXTERN const void *tsk_buffer_def_t;

TSK_END_DECLS

#endif

