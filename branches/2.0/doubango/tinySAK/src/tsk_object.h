/* Copyright (C) 2010-2013 Mamadou Diop.
* Copyright (C) 2013 Doubango Telecom <http://doubango.org>
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

/**@file tsk_object.h
 * @brief Base object implementation.
 */
#ifndef TSK_OBJECT_H
#define TSK_OBJECT_H

#include "tinysak_config.h"

#include <stdarg.h>
#include <stdio.h>

TSK_BEGIN_DECLS

#define TSK_OBJECT(self) ((tsk_object_t*)(self))

/**@ingroup tsk_object_group
* Plain object.
*/
typedef void tsk_object_t;

/**@ingroup tsk_object_group
* @def TSK_OBJECT_SAFE_FREE
* Safely free any well-defined object. If the reference count of the object was equal to 1 then this
 * 	object will be freed otherwise the refrence counter will be decremented. 
 *	In all case this operation will set the pointer (the object itself) to NULL.<br>
 * <b>Very Important</b>: Mutexes, Semaphores and CondVars are not well-defined objects. You should never use this macro to destroy them.
 * @param	self	The object to free or unref. 
**/
#define TSK_OBJECT_SAFE_FREE(self)		if((self)) tsk_object_unref((self)), (self) = tsk_null

#define TSK_OBJECT_SAFE_FREE_ARRAY(self, count) { \
	int __i; \
	for(__i = 0; __i < (count); ++__i) \
		TSK_OBJECT_SAFE_FREE((self)[__i]); \
}
#define TSK_OBJECT_SAFE_FREE_TABLE(self) TSK_OBJECT_SAFE_FREE_ARRAY((self), (sizeof((self))/sizeof((self)[0])))

/**@ingroup tsk_object_group
* tag a structure as an object. If this macro is used then you MUST
* provide a constructor and a destructor functions into an object definition (or meta-data).
* @ref tsk_object_new or @ref tsk_object_new_2 are used to create the object and  @ref tsk_object_unref or @ref tsk_object_delete to destroy it.
* @code
* typedef struct person_s{
*	TSK_DECLARE_OBJECT;
*	int id;
*	char* firstName;
*	char* lastName;
* } person_t;
* @endcode
* To create the object:
* @code
* // person_def_t: See bellow to understand how to create an object definition.
* person_t* person = tsk_object_new(person_def_t, "My First Name", "My last Name");
* @endcode
* To safely free the object:
* @code
* TSK_OBJECT_SAFE_FREE(person);
* @endcode
*/
#define TSK_DECLARE_OBJECT \
	const void* __def__;  /**< Opaque data holding a pointer to the actual meta-data(size, constructor, destructor and comparator) */ \
	volatile long	refCount /**< Reference counter. */

/**@ingroup tsk_object_group
* Internal macro to get the definition of the object.
*/
#define TSK_OBJECT_DEF(self)			((const tsk_object_def_t*)self)

/** Object meta-data (definition) */
typedef struct tsk_object_header_s{
	TSK_DECLARE_OBJECT;
}
tsk_object_header_t;
#define TSK_OBJECT_HEADER(object)	((tsk_object_header_t*)object)

/**@ingroup tsk_object_group
* Meta-data used of define an object.
* You MUST provide at least a constructor and a destructor. The comparator should
* be provided if you would like to compare opaque object or sort linked lists.
* @code
*
* // constructor
* static void* person_create(tsk_object_t * self, va_list * app)
* {
* 	static int unique_id = 0;
* 	person_t *person = self;
* 	if(person){
* 		person->id = ++unique_id;
* 		person->firstName = tsk_strdup(va_arg(*app, const char *));
* 		person->lastName = tsk_strdup(va_arg(*app, const char *));
* 	}
* 	return self;
* }
* 
* // destructor
* static void* person_destroy(tsk_object_t * self)
* { 
* 	person_t *person = self;
* 	if(person){
* 		TSK_FREE(person->firstName);
* 		TSK_FREE(person->lastName);
* 	}
* 	return self;
* }
* 
* // comparator
* static int person_cmp(const tsk_object_t *object1, const tsk_object_t *object1)
* {
* 	const person_t *person1 = object1;
* 	const person_t *person2 = object2;
* 
* 	return (person1 && person2) ? (person1->id - person2->id) : -1;
* }
*
* // Meta-data (Object defnition)
* static const tsk_object_def_t person_def_s = 
* {
* 	sizeof(person_t),
* 	person_create,
* 	person_destroy,
* 	person_cmp, 
* }person_def_t;
* 
* @endcode
* Now, to create your object:
* @code
* person_t* person = tsk_object_new(person_def_t, "My First Name", "My last Name"); // Will call "person_create" function.
* @endcode
* Or
* @code
* #define PERSON_CREATE(firstName, lastName) tsk_object_new(person_def_t, firstName, lastName)
* person_t* person = PERSON_CREATE("My First Name", "My last Name") // For clarity, this form will be used in all projects declared using @ref TSK_DECLARE_OBJECT.
* @endcode
* To safely free your object:
* @code
* TSK_OBJECT_SAFE_FREE(person); // Will call "person_destroy" function.
* @endcode
*/
typedef struct tsk_object_def_s
{
	//! The size of the object.
	tsk_size_t size;
	//! Pointer to the constructor.
	tsk_object_t*	(* constructor) (tsk_object_t *, va_list *);
	//! Pointer to the destructor.
	tsk_object_t*	(* destructor) (tsk_object_t *);
	//! Pointer to the comparator.
	int		(* comparator) (const tsk_object_t *, const tsk_object_t *);
}
tsk_object_def_t;

TINYSAK_API tsk_object_t* tsk_object_new(const tsk_object_def_t *objdef, ...);
TINYSAK_API tsk_object_t* tsk_object_new_2(const tsk_object_def_t *objdef, va_list* ap);
TINYSAK_API tsk_size_t tsk_object_sizeof(const tsk_object_t *);
TINYSAK_API int tsk_object_cmp(const void *self, const tsk_object_t *object);
TINYSAK_API tsk_object_t* tsk_object_ref(tsk_object_t *self);
TINYSAK_API tsk_object_t* tsk_object_unref(tsk_object_t *self);
TINYSAK_API tsk_size_t tsk_object_get_refcount(tsk_object_t *self);
TINYSAK_API void tsk_object_delete(tsk_object_t *self);

TSK_END_DECLS

#endif /* TSK_OBJECT_H */

