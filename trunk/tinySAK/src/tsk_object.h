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

/**@file tsk_object.h
 * @brief Base object implementation.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TSK_OBJECT_H
#define TSK_OBJECT_H

#include "tinySAK_config.h"

#include <stdarg.h>
#include <stdio.h>

#define TSK_DECLARE_OBJECT \
	const void* base


#define TSK_OBJECT_DEF(self)			((tsk_object_def_t*)self)
#define TSK_OBJECT_DEF_CONST(self)		((const tsk_object_def_t*)self)

/**
* TSK OBJECT.
*/
typedef struct tsk_object_def_s
{
	size_t size;													/**< The size of an tsk object. */
	void*	(* constructor) (void * self, va_list * app);			/**< The constructor. */
	void	(* destructor) (void *);								/**< The destructor. */
	void*	(* cloner) (const void * self);							/**< The object cloner. */
	int		(* equals) (const void * self, const void * object);	/**< Compare two object. */

	size_t	refCount;
}
tsk_object_def_t;

TINYSAK_API void* tsk_object_new(const tsk_object_def_t *objdef, ...);
TINYSAK_API size_t tsk_object_sizeof(const void *self);
TINYSAK_API int tsk_object_equals(const void *self, const void *object);
TINYSAK_API void* tsk_object_ref(void *self);
TINYSAK_API void* tsk_object_unref(void *self);
TINYSAK_API void tsk_object_delete(void **self);

#endif /* TSK_OBJECT_H */
