/**
* @file tsk.c
* @author  Mamadou Diop <diopmamadou(at)doubango[dot]org>
* @version 1.0
*
* @section LICENSE
*
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
#include "tsk.h"

/** @page _Page_TinySAK_Overview Swiss Army Knife

*
* This file is an overview of <b>tinySAK</b> API.
*
* <b>tinySAK</b> (a.k.a <b>T</b>iny <b>S</b>wiss <b>A</b>rmy <b>K</b>nife) is a tiny but fully featured utility API.
* This API is designed to efficiently work on embedded systems with limited memory and low computing power.<br>
* This library provide a base object class to ease Object Oriented Programming in C. There are many other
* features like multi-threading, time management, encoding, encryption or content management.
* <h1>6	ANSI-C Object Programming</h1>
* As you probably know, C is not an object oriented language.<br>
* Today, OOP (Object-Oriented Programing) is the best way to program well designed softwares.<br>
* In this chapter a “well-defined object” is a special C structure. All functions shown in this chapter are part of tinySAK project.<br>
* To explain how well-defined objects are implemented and used, I will give an example based on “Person” object.<br>
* The person object is declared like this:<br>
* @code
* typedef struct person_s
{
	TSK_DECLARE_OBJECT; // Mandatory

	char* name;
	struct person_s* girlfriend;
}
person_t;
* @endcode
<h2>6.1	Object Definition</h2>
* An object definition could be considered as a class definition. The definition holds the object’s mandatory functions, size and a reference counter.<br>
* The mandatory functions are the constructor, the destructor and the comparator.<br>
* A C structure is defined as an object by using @ref TSK_DECLARE_OBJECT macro in its body.<br>
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
* An object is destroyed in two phases. The first phase consists of freeing its members (void*, char* …). It’s the destructor which is responsible of this task. In the second phase, the object itself is destroyed. As the object cannot destroy itself, you should use @ref tsk_object_unref() or @ref tsk_object_delete() to perform these two phases. The difference between these two functions is explained in the coming sections.
* </p>
* A well-defined object must never be freed using free() standard C function.<br>
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
* <h2>6.2	Constructor</h2>
* The constructor is only responsible for the initialization and won’t allocate the object. When passed to the constructor, the object is already allocated.<br>
* Here is an example:<br>
* @code
// (constructor)
static tsk_object_t* person_ctor(tsk_object_t * self, va_list * app)
{
 	person_t *person = self;
 	if(person){
 		person->name = tsk_strdup(va_arg(*app, const char *));
 	}
 	return self;
 }
* @endcode 
* <h2>6.3	Destructor</h2>
* The destructor will free the object’s members and won’t destroy the object itself (Phase 1). The destructor function must return a pointer to itself to allow the caller to perform the second phase.<br>
* Here is an example:<br>
* @code
// (destructor)
 static tsk_object_t * person_dtor(tsk_object_t * self)
 { 
 	person_t *person = self;
 	if(person){
 		TSK_FREE(person->name);
		tsk_object_unref(person->girlfriend);
 	}
 	return self;
 }
* @endcode 
* <h2>6.4	Comparator</h2>
* The comparator function is used to compare two well-defined objects. The objects to compare shall have the same definition (or type). <br>
* Here is an example:<br>
* @code
// (comparator)
static int person_cmp(const tsk_object_t *_p1, const tsk_object_t *_p2)
 {
 	const person_t *p1 = _p1;
 	const person_t *p1 = _p2;
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
<h2>6.5	Reference counting</h2>
* Reference counting is used to emulate garbage collection. Each well-defined object contains a reference counter field which indicates how many object have a reference to the actual object.<br>
* When an object is created (see below) the counter value is initialized to 1; this is automatically done and you have nothing to do. The counter is incremented by 1 when you call @ref tsk_object_ref() and decremented (by 1) when you call @ref tsk_object_unref().<br>
* When the counter value reaches zero, then the object is garbaged (freed).<br>
* 
* <h2>6.6	Inheritence</h2>
* As you expect, inheritance is not supported in ANSI-C. <br>
* As any C Structure could be casted to a pointer to its first element, inheritance could be achieved like this:<br>
* @code
#include "tsk.h"
// (a student is a person)
typedef struct student_s
{
	person_t* person; // Must be the first element
	char* school;
}
student_t;

// (as a student is a person you can do)
student_t* s = tsk_null;
//....
((person_t*)s)->name = tsk_strdup("bob");
* @endcode
* 
* As @code person_t is a well-defined object, then @code student_t is also well-defined.<br>
* <h2>6.7	Usage</h2>
* Once the object’s definition is declared and all its mandatory functions implemented, it is used like this:<br>
* @code
// creates a person: will call the constructor
person_t* bob = tsk_object_new(&person_def_t, "bob");
// creates bob's girlfriend
bob->girlfriend = tsk_object_new(&person_def_t, "alice");
// deletes bob: will delete both bob and bob's girlfriend field by calling their destructors
tsk_object_unref(bob);
* @endcode
* As it’s hard to guest which parameters the construct expects, it’s common to use macro (or function) helpers. In our example the macro will look like this:
* @code
// create a person
#define PERSON_CREATE(name)	tsk_object_new(&person_def_t, (const char*)name)
* @endcode
* 
* As the destructor has fixed parameters, there is a common macro to destroy all kind of well-defined objects. <br>
* TSK_OBJECT_SAFE_FREE() is used to destroy any object. <br>
* The object will be freed only if; when decremented by 1 the reference count of the object is equal to zero. In all case (freed or not) the pointer value will be set to NULL.<br>
* The above example can be rewritten like this:<br>
* @code
#include "tsk.h"

// create a person: will call the constructor
person_t* bob = PERSON_CREATE("bob");
// create bob's girlfriend
bob->girlfriend = PERSON_CREATE("alice");
// delete bob: will delete both bob and bob's girlfriend field by calling their destructors
TSK_OBJECT_SAFE_FREE(bob);
* @endcode
*
* <h2>6.8	Lists</h2>
*
* <h2>7	Threading</h2>
* The framework provides an operating system agnostic threading functions for both WIN32 and Unix-like systems.<br>
* 
* <h2>7.1	Threads</h2>
* You don’t need thousands of functions to manage threads. In the Framework we only need to create, pause and destroy threads.<br>
* Threads can be created using @ref tsk_thread_create() and joined using @ref tsk_thread_join().<br>
* You can temporary cease the executing of a thread by calling @ref tsk_thread_sleep().<br>
* @code
#include "tsk.h"

void* MyThreadFunction(void *arg)
{
	printf("arg=%d", *((int*)arg));
	return tsk_null;
}

void test_threads()
{
	void* tid[1] = {tsk_null}; // thread id
	int arg = 112; // arg to pass to the function
	
	// creates the thread
	tsk_thread_create(&tid[0], MyThreadFunction, &arg);

	// joins the thread
	tsk_thread_join(&(tid[0]));
}
* @endcode
* 
* <h2>7.2 Mutexes</h2>
* Mutexes (Mutual exclusion) are used to protect a portion of code or function against concurrent access. Concurrent access happens when two or several threads try to execute the same portion of code at nearly the same time.<br>
* @code
#include "tsk.h"

// create the mutext
tsk_mutex_handle_t *mutex = tsk_mutex_create();
	
tsk_mutex_lock(mutex);
// ...portion of code to protect
tsk_mutex_unlock(mutex);

// destroy the mutex
tsk_mutex_destroy(&mutex);
* @endcode
* Mutexes are not well-defined objects; you should use @ref tsk_mutex_destroy instead of TSK_OBJECT_SAFE_FREE() to destroy them.<br>
* 
* <h2>7.3 Thread-Safe Objects</h2>
* 
* Any C Structure could be declared as thread-safe using @ref TSK_DECLARE_SAFEOBJ macro. It’s not mandatory for the object to be well-defined.<br>
* A thread-safe object is initialized using @ref tsk_safeobj_init() and deinitilized using @ref tsk_safeobj_deinit(). To lock and unlock a portion of code which accesses the object you should use @ref tsk_safeobj_lock() and @ref tsk_safeobj_unlock() respectively.<br>
* 
* <h2>7.4 Semaphores</h2>
* Only counting semaphores are supported by the framework.
* Counting semaphores are used to control the access to a portion of code which might be executed by multiple threads. A thread will have rights to execute the portion of code only if the semaphore’s internal counter value is different than zero. Before executing the code to control, a thread should decrement the counter to check if it has permit.<br>
* @code
#include "tsk.h"

// (create the semaphore)
tsk_semaphore_handle_t *sem = tsk_semaphore_create();
// (increment the counter)
tsk_semaphore_increment(sem);
// (decrement the counter)
tsk_semaphore_decrement(sem);
// (destoy the semaphore)
tsk_semaphore_destroy(&sem);
* @endcode
* Semaphores are not well-defined objects; you should use @ref tsk_semaphore_destroy instead of TSK_OBJECT_SAFE_FREE() to destroy them.<br>
* Mutexes are binary semaphores (counter value is always equals to 1 or 0).<br>
* 
* <h2>7.5 Condition Variables</h2>
* Condition variables are used to control the access to a portion of code which might be executed by multiple threads. Each thread will block until a certain condition is signaled or ms milliseconds have passed.<br>
* @ref tsk_condwait_create is used to create a condition variable, @ref tsk_condwait_wait() to wait indefinitely until the condition is signaled and @ref tsk_condwait_timedwait() to wait until the condition is signaled or ms milliseconds have passed.<br>
* @ref tsk_condwait_signal() is used to alert the first waiting thread that the condition is now true and @ref tsk_condwait_broadcast() is used to alert all waiting threads.<br>
* Condition variables are not well-defined objects; you should use @ref tsk_condwait_destroy() instead of TSK_OBJECT_SAFE_FREE() to destroy them.<br>
* 
* <h2>7.6 Runnable</h2>
* A <i>runnable</i> object is a well-defined object and is declared using @ref TSK_DECLARE_RUNNABLE() macro.<br>
* A <i>runnable</i> object must be explicitly started using @ref tsk_runnable_start() and is implicitly stopped when destroyed. You can explicitly stop the object by calling @ref tsk_runnable_stop().<br>
* 
* <h2>8	Final Sate Machine</h2>
* 
* <h2>9	Timer Manager</h2>
*
*/


