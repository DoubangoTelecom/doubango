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

/**@file tsk_buffer.h
 * @brief Buffer manager.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TINYSAK_BUFFER_H_
#define _TINYSAK_BUFFER_H_

#include "tinysak_config.h"
#include "tsk_list.h"

/**@ingroup tsk_buffer_group
* @def TSK_BUFFER
* Converts to @ref tsk_buffer_t object.
* @param self @ref tsk_buffer_t object.
*/
/**@ingroup tsk_buffer_group
* @def TSK_BUFFER_DATA
* Gets the internal buffer.
* @param self @ref tsk_buffer_t object.
*/
/**@ingroup tsk_buffer_group
* @def TSK_BUFFER_SIZE
* Gets the size of the internal buffer.
* @param self @ref tsk_buffer_t object.
*/

TSK_BEGIN_DECLS


#define TSK_BUFFER(self)					((tsk_buffer_t*)self)
#define TSK_BUFFER_DATA(self)				(self ? TSK_BUFFER(self)->data : tsk_null)
#define TSK_BUFFER_SIZE(self)				(self ? TSK_BUFFER(self)->size : 0)

/**@ingroup tsk_buffer_group
* @def TSK_BUFFER_TO_STRING
* Gets a the internal buffer as a pointer to a string (const char*).
* @param self @ref tsk_buffer_t object.
*/
/**@ingroup tsk_buffer_group
* @def TSK_BUFFER_TO_U8
* Gets a the internal buffer as a pointer to an unsigned string (uint8_t*).
* @param self @ref tsk_buffer_t object.
*/
#define TSK_BUFFER_TO_STRING(self)			(self ? (const char*)TSK_BUFFER_DATA(self) : tsk_null)
#define TSK_BUFFER_TO_U8(self)				(self ? (uint8_t*)TSK_BUFFER_DATA(self) : tsk_null)

/**@ingroup tsk_buffer_group
* Buffer object.
*/
typedef struct tsk_buffer_s
{
	TSK_DECLARE_OBJECT;

	void *data; /**< Interanl data. */
	tsk_size_t size; /**< The size of the internal data. */
}
tsk_buffer_t;

typedef tsk_list_t tsk_buffers_L_t; /**<@ingroup tsk_buffer_group List of @ref tsk_buffer_t elements. */

TINYSAK_API tsk_buffer_t* tsk_buffer_create(const void* data, tsk_size_t size);
TINYSAK_API tsk_buffer_t* tsk_buffer_create_null();

TINYSAK_API int tsk_buffer_append_2(tsk_buffer_t* self, const char* format, ...);
TINYSAK_API int tsk_buffer_append(tsk_buffer_t* self, const void* data, tsk_size_t size);
TINYSAK_API int tsk_buffer_realloc(tsk_buffer_t* self, tsk_size_t size);
TINYSAK_API int tsk_buffer_remove(tsk_buffer_t* self, tsk_size_t position, tsk_size_t size);
TINYSAK_API int tsk_buffer_insert(tsk_buffer_t* self, tsk_size_t position, const void*data, tsk_size_t size);
TINYSAK_API int tsk_buffer_copy(tsk_buffer_t* self, tsk_size_t start, const void* data, tsk_size_t size);
TINYSAK_API int tsk_buffer_cleanup(tsk_buffer_t* self);
TINYSAK_API int tsk_buffer_takeownership(tsk_buffer_t* self, void** data, tsk_size_t size);

TINYSAK_GEXTERN const tsk_object_def_t *tsk_buffer_def_t;

TSK_END_DECLS

#endif /* _TINYSAK_BUFFER_H_ */

