/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
#if !defined(_TEST_OBJECT_H_) && 0
#define _TEST_OBJECT_H_

typedef struct person_s
{
	TSK_DECLARE_OBJECT; // Mandatory

	char* name;
	struct person_s* girlfriend;
}
person_t;

// (a student is a person)
typedef struct student_s
{
	struct person_s* person; // Must be the first element
	char* school;
}
student_t;

//// (as a student is a person you can do)
//student_t* s;
//((person_t*)s)->name = tsk_strdup("bob");


// constructor
static tsk_object_t* person_create(tsk_object_t * self, va_list * app)
{
 	person_t *person = self;
 	if(person){
 		person->name = tsk_strdup(va_arg(*app, const char *));
 	}
 	return self;
 }

 // destructor
 static tsk_object_t* person_destroy(tsk_object_t * self)
 { 
 	person_t *person = self;
 	if(person){
 		TSK_FREE(person->name);
		tsk_object_unref(person->girlfriend);
 	}
 	return self; // return
 }
 
 // comparator
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

 //(Object defnition)
 static const tsk_object_def_t person_def_t = 
 {
 	sizeof(person_t),
 	person_create,
 	person_destroy,
 	person_cmp
 };

 // create a person
#define PERSON_CREATE(name)				tsk_object_new(&person_def_t, (const char*)name)

/* test object */
void test_object()
{
	// creates a person: will call the constructor
	person_t* bob = PERSON_CREATE("bob");
	// creates bob's girlfriend
	bob->girlfriend = PERSON_CREATE("alice");
	// deletes bob: will delete both bob and bob's girlfriend field by calling their destructors
	TSK_OBJECT_SAFE_FREE(bob);
}

#endif /* _TEST_OBJECT_H_ */
