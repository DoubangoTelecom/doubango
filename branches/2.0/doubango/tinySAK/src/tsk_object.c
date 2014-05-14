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

/**@file tsk_object.c
 * @brief Base object implementation.
 *
 */
#include "tsk_object.h"
#include "tsk_memory.h"
#include "tsk_debug.h"
#include "tsk_common.h"

#if TSK_UNDER_WINDOWS
#	include <windows.h>
#endif /* TSK_UNDER_WINDOWS */

/**@defgroup tsk_object_group Base object implementation.
* @brief Provides utility functions to ease Object Oriented Programming in C.
*/

#if defined (_DEBUG) || defined (DEBUG)
#	define TSK_DEBUG_OBJECTS	0
static int tsk_objects_count = 0;
#else
#	define TSK_DEBUG_OBJECTS	0
#endif

/**@ingroup tsk_object_group
* Creates new object. The object MUST be declared using @ref TSK_DECLARE_OBJECT macro.
* @param objdef The object meta-data (definition). For more infomation see @ref tsk_object_def_t.
* @param ... List of parameters to pass to the constructor(defined in the meta-data).
* @retval @ref tsk_object_t object with a reference counter equal to 1.
* @sa @ref tsk_object_new_2.
*/
tsk_object_t* tsk_object_new(const tsk_object_def_t *objdef, ...)
{
	// Do not check "objdef", let the application die if it's null
	tsk_object_t *newobj = tsk_calloc(1, objdef->size);
	if(newobj){
		(*(const tsk_object_def_t **) newobj) = objdef;
		TSK_OBJECT_HEADER(newobj)->refCount = 1;
		if(objdef->constructor){ 
			va_list ap;
			tsk_object_t * newobj_ = newobj;// save
			va_start(ap, objdef);
			newobj = objdef->constructor(newobj, &ap); // must return new
			va_end(ap);

			if(!newobj){ // null if constructor failed to initialized the object
				if(objdef->destructor){
					objdef->destructor(newobj_);
				}
				tsk_free(&newobj_);
			}

#if TSK_DEBUG_OBJECTS
		TSK_DEBUG_INFO("N∞ objects:%d", ++tsk_objects_count);
#endif
		}
		else{
			TSK_DEBUG_WARN("No constructor found.");
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new tsk_object.");
	}

	return newobj;
}

/**@ingroup tsk_object_group
* Creates new object. The object MUST be declared using @ref TSK_DECLARE_OBJECT macro.
* @param objdef The object meta-data (definition). For more infomation see @ref tsk_object_def_t.
* @param ap Variable argument list to pass to the constructor(defined in the meta-data).
* @retval @ref tsk_object_t object with a reference counter equal to 1.
* @sa @ref tsk_object_new.
*/
tsk_object_t* tsk_object_new_2(const tsk_object_def_t *objdef, va_list* ap)
{
	tsk_object_t *newobj = tsk_calloc(1, objdef->size);
	if (newobj) {
		(*(const tsk_object_def_t **) newobj) = objdef;
		TSK_OBJECT_HEADER(newobj)->refCount = 1;
		if (objdef->constructor) { 
			newobj = objdef->constructor(newobj, ap);

#if TSK_DEBUG_OBJECTS
		TSK_DEBUG_INFO("N∞ objects:%d", ++tsk_objects_count);
#endif
		}
		else {
			TSK_DEBUG_WARN("No constructor found.");
		}
	}
	else {
		TSK_DEBUG_ERROR("Failed to create new tsk_object.");
	}

	return newobj;
}

/**@ingroup tsk_object_group
* Gets the size of an opaque object.
* @param self The object for which we want to get the size.
* The object MUST be declared using @ref TSK_DECLARE_OBJECT macro and created using @ref tsk_object_new or @ref tsk_object_new_2.
* @retval The size of the object.
*/
tsk_size_t tsk_object_sizeof(const tsk_object_t *self)
{
	const tsk_object_def_t **objdef = (const tsk_object_def_t **)self;
	if (objdef && *objdef) {
		return (*objdef)->size;
	}
	else {
		TSK_DEBUG_ERROR("NULL object definition.");
		return 0;
	}
}

/**@ingroup tsk_object_group
* Compares two well-defined objects.
* If the meta-data (definition) of the first object (@a object1) do not include a function comparator then this method will amlways return -1.
* @param object1 The first object to compare.
* @param object2 The second object to compare.
* @retval Zero if the two object are equal.
* Positive value if @a object1 is greater than @a object2 and a negative value otherwise.
*/
int tsk_object_cmp(const tsk_object_t *object1, const tsk_object_t *object2)
{
	const tsk_object_def_t **objdef = (const tsk_object_def_t **)object1;

	if (objdef && *objdef && (*objdef)->comparator) {
		return (*objdef)->comparator(object1, object2);
	}
	return ((int*)object1 - (int*)object2);
}

/**@ingroup tsk_object_group
* Increment the refrence counting of the object.<br>
* Refernce counting: http://en.wikipedia.org/wiki/Reference_counting.<br>
* The object MUST be declared using @ref TSK_DECLARE_OBJECT macro and created using @ref tsk_object_new or @ref tsk_object_new_2.
* @param self The object holding the counter to increment.
* @retval The new object (incremented).
* @sa tsk_object_unref.
*/
tsk_object_t* tsk_object_ref(tsk_object_t *self)
{
	tsk_object_header_t* objhdr = TSK_OBJECT_HEADER(self);
	if (objhdr && objhdr->refCount > 0) {
		tsk_atomic_inc(&objhdr->refCount);
		return self;
	}
	return tsk_null;
}

/**@ingroup tsk_object_group
* Decrement the refrence counting of the object.<br>
* Refernce counting: http://en.wikipedia.org/wiki/Reference_counting.<br>
* The object MUST be declared using @ref TSK_DECLARE_OBJECT macro and created using @ref tsk_object_new or @ref tsk_object_new_2.
* @param self The object holding the counter to decrement.
* @retval If the refernce counter is equal to zero then NULL is returned otherwise a new object (decremented) is returned.
* @sa ref tsk_object_ref.
* @sa ref TSK_OBJECT_SAFE_FREE.
*/
tsk_object_t* tsk_object_unref(tsk_object_t *self)
{
	if (self) {
		tsk_object_header_t* objhdr = TSK_OBJECT_HEADER(self);
		if (objhdr->refCount > 0) { // If refCount is == 0 then, nothing should happen.
			tsk_atomic_dec(&objhdr->refCount);
			if (objhdr->refCount == 0) {
				tsk_object_delete(self);
				return tsk_null;
			}
		}
		else {
			return tsk_null;
		}
	}
	return self;
}

/**@ingroup tsk_object_group
* Gets the reference counting value. See @ref _Anchor_TinySAK_Object_Reference_Counting for more information.
* @retval The reference counting value.
*/
tsk_size_t tsk_object_get_refcount(tsk_object_t *self)
{
	return self ? TSK_OBJECT_HEADER(self)->refCount : 0;
}

/**@ingroup tsk_object_group
* Delete an object. This function will delete the object even if it's reference counter is greater than 1.
* This mean that this function is not safe. You should use @ref TSK_OBJECT_SAFE_FREE to safely delete an object.
* The object MUST be declared using @ref TSK_DECLARE_OBJECT macro and created using @ref tsk_object_new or @ref tsk_object_new_2.
* @param self The object to delete.
* @sa @ref TSK_OBJECT_SAFE_FREE.
*/
void tsk_object_delete(tsk_object_t *self)
{
	const tsk_object_def_t ** objdef = (const tsk_object_def_t **)self;
	if(self && *objdef){
		if ((*objdef)->destructor) {
			self = (*objdef)->destructor(self);
#if TSK_DEBUG_OBJECTS
		TSK_DEBUG_INFO("N∞ objects:%d", --tsk_objects_count);
#endif
		}
		else {
			TSK_DEBUG_WARN("No destructor found.");
		}
		if (self) {
			free(self);
		}
	}
}


/**@page _Page_TinySAK_AnsiC_Object_Programming ANSI-C Object Programming
* 
* - @ref _Anchor_TinySAK_Object_Definition "Object Definition"
* - @ref _Anchor_TinySAK_Object_Constructor "Constructor"
* - @ref _Anchor_TinySAK_Object_Destructor "Destructor"
* - @ref _Anchor_TinySAK_Object_Comparator "Comparator"
* - @ref _Anchor_TinySAK_Object_Reference_Counting "Reference counting"
* - @ref _Anchor_TinySAK_Object_Inheritence "Inheritence"
* - @ref _Anchor_TinySAK_Object_Basic_Usage "Basic usage"
* - @ref _Anchor_TinySAK_Object_Complete_Source_Code "Complete source code"
* - @ref _Anchor_TinySAK_Object_Public_Fuctions "Public functions"
*
* <hr />
*
* As you probably know, ANSI-C is not an object oriented language.<br>
* Today, OOP (Object-Oriented Programing) is the best way to program well designed softwares.<br>
* In this chapter a <b>"well-defined object"</b> is a special C structure. All functions shown in this chapter are part of <b>tinySAK</b> project.<br>
* To explain how well-defined objects are implemented and used, We'll give an example based on <b>"Person"</b> object.<br>
* The person object is declared like this:<br>
* @code
* typedef struct person_s
{
	TSK_DECLARE_OBJECT; // Mandatory and used to tag a struct as a <b>"well-defined object"<b/>

	char* name;
	struct person_s* girlfriend;
}
person_t;
* @endcode
Notice on the above code how @ref TSK_DECLARE_OBJECT macro is used to tag the struct. This macro must be the first element to appear an adds a definition (see next section) to the struct.
*
<h2>@anchor _Anchor_TinySAK_Object_Definition Object Definition</h2>
* An object definition could be considered as a class definition. The definition holds the object’s mandatory functions, size and a reference counter.<br>
* The mandatory functions are the constructor, the destructor and the comparator.<br>
* A C structure is tagged as an object by using @ref TSK_DECLARE_OBJECT macro in its body.<br>
* A pointer to an object definition shall point to a struct @ref tsk_object_def_s.<br>
* @code
typedef struct tsk_object_def_s
{
	//! The size of the object.
	tsk_size_t size;
	//! Pointer to the constructor.
	tsk_object_t*	(* constructor) (tsk_object_t *, va_list *);
	//! Pointer to the destructor.
	tsk_object_t*	(* destructor) (tsk_object_t *);
	//! Pointer to the comparator.
	int		(*comparator) (const tsk_object_t *, const tsk_object_t *);
}
tsk_object_def_t;
* @endcode
*
* <p>
* An object is created in two phases. The first phase consists of dynamically allocating the object on the heap; this is why its size is mandatory in the object definition structure. When a new object is allocated on the heap, all its members (char*, void*, int, long …) will be zeroed. In the second phase, the newly created object will be initialized by calling the supplied constructor. To perform these two phases, you should call @ref tsk_object_new() or @ref tsk_object_new_2().
* </p>
* <p>
* An object is destroyed in two phases. The first phase consists of freeing its members (void*, char* …). It’s the destructor which is responsible of this task. In the second phase, the object itself is freed. As the object cannot free itself, you should use @ref tsk_object_unref() or @ref tsk_object_delete() to perform these two phases. The difference between these two functions is explained in the coming sections.
* </p>
* A well-defined object <b>must never be freed by yourself using free() standard C function</b>. This task is up to the memory manager.<br>
* Below, an example of how to declare an object definition:<br>
* @code
* //(Object defnition)
 static const tsk_object_def_t person_def_t = 
 {
 	sizeof(person_t),
 	person_ctor,
 	person_dtor,
 	person_cmp
 };
* @endcode 
* <h2>@anchor _Anchor_TinySAK_Object_Constructor Constructor</h2>
* The constructor is only responsible for the initialization and won’t allocate the object. When passed to the constructor, the object is already allocated.<br>
* Here is an example:<br>
* @code
// (constructor)
static tsk_object_t* person_ctor(tsk_object_t * self, va_list * app)
{
 	person_t *person = (person_t *)self;
 	if(person){
 		person->name = tsk_strdup(va_arg(*app, const char *));
 	}
 	return self;
 }
* @endcode 
* <h2>@anchor _Anchor_TinySAK_Object_Destructor Destructor</h2>
* The destructor will free the object’s members and won’t free the object itself (Phase 1). The destructor function must return a pointer to itself to allow the caller to perform the second phase (freeing the object itself).<br>
* Here is an example:<br>
* @code
// (destructor)
 static tsk_object_t * person_dtor(tsk_object_t * self)
 { 
 	person_t *person = (person_t *)self;
 	if(person){
 		TSK_FREE(person->name);
		tsk_object_unref(person->girlfriend);// do not use free() as it's a well-defined object
 	}
 	return self;
 }
* @endcode 
* <h2>@anchor _Anchor_TinySAK_Object_Comparator Comparator</h2>
* The comparator function is used to compare two <i>"well-defined"</i> objects. The objects to compare shall have the same definition (or type). <br>
* Here is an example:<br>
* @code
// (comparator)
static int person_cmp(const tsk_object_t *_p1, const tsk_object_t *_p2)
 {
 	const person_t *p1 = (const person_t *)_p1;
 	const person_t *p1 = (const person_t *)_p2;
	int ret;
	
	// do they have the same name?
	if((ret = tsk_stricmp(p1->name, p2->name))){
		return ret;
	}
	// do they have the same girlfriend?
	if((ret = tsk_object_cmp(p1->girlfriend, p2->girlfriend))){
		return ret;
	}
	
	// they are the same
	return 0;
 }
* @endcode
The comparator function is also used by the linked-list to sort <i>"well-defined"</i> objects.
*
<h2>@anchor _Anchor_TinySAK_Object_Reference_Counting Reference counting</h2>
* Reference counting is used to emulate garbage collection. Each <i>"well-defined"</i> object contains a reference counter field which indicates how many object have a reference to the actual object.<br>
* When an object is created (see below) the counter value is initialized to 1; this is automatically done and you have nothing to do. The counter is incremented by 1 when you call @ref tsk_object_ref() and decremented (by 1) when you call @ref tsk_object_unref().<br>
* When the counter value reaches zero then, the object is garbaged (freed).<br>
* Calling @ref tsk_object_delete() will destroy an object regardless the value of the reference counter. You should never call @ref tsk_object_delete() but @ref tsk_object_unref() instead.
* 
* <h2>@anchor _Anchor_TinySAK_Object_Inheritence Inheritence</h2>
* As you may expect, inheritance is not supported in ANSI-C. <br>
* As any C Structure could be casted to a pointer to its first element, inheritance could be achieved like this:<br>
* @code
#include "tsk.h"
// (a student is a person)
typedef struct student_s
{
	person_t* person; // Must be the first element and a "well-defined" object
	char* school;
}
student_t;

// (as a student is a person you can do)
student_t* s = tsk_null;
//... create and initialize the object
// then, cast a "student" as "person" to initialize additional fields
((person_t*)s)->name = tsk_strdup("bob");
* @endcode
* 
* As <b>person_t</b> is a well-defined object, then <b>student_t</b> is also well-defined.<br>
*
* <h2>@anchor _Anchor_TinySAK_Object_Basic_Usage Basic usage</h2>
* Once the object’s definition is declared and all its mandatory functions implemented, it is used like this:<br>
* @code
// creates a person: will call the constructor
person_t* bob = tsk_object_new(&person_def_t, "bob");
// creates bob's girlfriend
bob->girlfriend = tsk_object_new(&person_def_t, "alice");
// deletes bob: will delete both bob and bob's girlfriend field by calling their destructors
tsk_object_unref(bob);
* @endcode
* As it’s hard to guest which parameters the construct expects, it’s common to use macro (or function) helpers. In our example the helper function will look like this:
* @code
// create a person
static person_t* person_create(const char* name)
{
	return (person_t*)tsk_object_new(&person_def_t, (const char*)name);
}
*
* @endcode
* 
* As the destructor has fixed parameters, there is a common macro to destroy all kind of well-defined objects. <br>
* @ref TSK_OBJECT_SAFE_FREE() macro is used to destroy any well-defined object. <br>
* The object will be freed only if; when decremented by 1 the reference count of the object is equal to zero. In all case (freed or not) the pointer value will be set to NULL to avoid trying to free the object again.<br>
* The above example could be rewritten like this:<br>
* @code
#include "tsk.h"

// create a person: will call the constructor
person_t* bob = person_create("bob");
// create bob's girlfriend
bob->girlfriend = person_create("alice");
// delete bob: will delete both bob and bob's girlfriend field by calling their destructors
TSK_OBJECT_SAFE_FREE(bob);
* @endcode
*
*
* <h2>@anchor _Anchor_TinySAK_Object_Complete_Source_Code Complete source code</h2>
Complete source code with minor changes.
* @code
* typedef struct person_s
{
	TSK_DECLARE_OBJECT; // Mandatory and used to tag a struct as a "well-defined" object

	char* name;
	uint32_t age;
	struct person_s* girlfriend;
}
person_t;

// (constructor)
static tsk_object_t* person_ctor(tsk_object_t * self, va_list * app)
{
 	person_t *person = self;
 	if(person){
	// va_list ignored in this example
 	}
 	return self;
 }

 // (destructor)
 static tsk_object_t * person_dtor(tsk_object_t * self)
 { 
 	person_t *person = self;
 	if(person){
 		TSK_FREE(person->name);
		tsk_object_unref(person->girlfriend); // do not use free() as it's a well-defined object
 	}
 	return self;
 }

 // (comparator)
 static int person_cmp(const tsk_object_t *_p1, const tsk_object_t *_p2)
 {
 	const person_t *p1 = (const person_t *)_p1;
 	const person_t *p1 = (const person_t *)_p2;
	int ret;
	
	// do they have the same name?
	if((ret = tsk_stricmp(p1->name, p2->name))){
		return ret;
	}
	// do they have the same girlfriend?
	if((ret = tsk_object_cmp(p1->girlfriend, p2->girlfriend))){
		return ret;
	}
	
	// they are the same
	return 0;
 }

 // helper function to create a person
static person_t* person_create(const char* name, uint32_t age, const char* girlfriend_name, uint32_t girlfriend_age)
{
	person_t* bob;
	if((bob = (person_t*)tsk_object_new(&person_def_t))){ // no name as parameter because the constructor(person_ctor()) is modified to ignore the va_list argument
		bob->name = tsk_strdup(name);
		bob->age = age;
		if((bob->girlfriend = (person_t*)tsk_object_new(&person_def_t))){
			bob->girlfriend->name = tsk_strdup(girlfriend_name);
			bob->girlfriend->age = girlfriend_age;
		}
	}
}

person_t* bob = person_create("bob", 19, "alice", 18); // refcount = 1
tsk_object_ref(bob); // refcount = 2
bob = tsk_object_unref(bob); // refcount = 1
bob = tsk_object_unref(bob); // refcount = 0 and destroyed
*
* @endcode
* 
* <h2>@anchor _Anchor_TinySAK_Object_Public_Fuctions Public functions </h2>
* - @ref tsk_object_cmp
* - @ref tsk_object_delete
* - @ref tsk_object_get_refcount
* - @ref tsk_object_new
* - @ref tsk_object_new_2
* - @ref tsk_object_ref
* - @ref tsk_object_sizeof
* - @ref tsk_object_unref
*/