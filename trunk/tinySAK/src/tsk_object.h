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
	const void* base; \
	size_t	refCount

#define TSK_DECLARE_DEF(prefix, name) \
	static const tsk_object_def_t ##prefix##_##name##_def_s = \
	{\
		sizeof(##prefix##_##name##_t),	\
		##prefix##_##name##_create,		\
		##prefix##_##name##_destroy,	\
		##prefix##_##name##_clone,		\
		##prefix##_##name##_cmp,		\
		##prefix##_##name##_icmp,		\
	};\
	const void *##prefix##_##name##_def_t = &##prefix##_##name##_def_s;

#define TSK_OBJECT_DEF(self)			((const tsk_object_def_t*)self)

/**
* Base object.
*/
typedef struct tsk_object_def_s
{
	size_t size;													/**< The size of an tsk object. */
	void*	(* constructor) (void *self, va_list *app);				/**< The constructor. */
	void*	(* destructor) (void *);								/**< The destructor. */
	int		(* objcmp) (const void *object1, const void *object2);	/**< Compare two object */
}
tsk_object_def_t;

TINYSAK_API void* tsk_object_new(const tsk_object_def_t *objdef, ...);
TINYSAK_API void* tsk_object_new2(const tsk_object_def_t *objdef, va_list* ap);
TINYSAK_API size_t tsk_object_sizeof(const void *self);
TINYSAK_API int tsk_object_cmp(const void *self, const void *object);
//TINYSAK_API int tsk_object_icmp(const void *self, const void *object);
TINYSAK_API void* tsk_object_ref(void *self);
TINYSAK_API void* tsk_object_unref(void *self);
TINYSAK_API void tsk_object_delete(void *self);

#endif /* TSK_OBJECT_H */
