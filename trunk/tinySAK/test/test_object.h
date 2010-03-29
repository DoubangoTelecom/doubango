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
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/
#ifndef _TEST_OBJECT_H_
#define _TEST_OBJECT_H_

typedef struct person_s
{
	TSK_DECLARE_OBJECT; // Mandatory

	char* name;
	person_t* mother;
}
person_t;

// constructor
static void* person_create(tsk_object_t * self, va_list * app)
{
 	person_t *person = self;
 	if(person){
 		person->name = tsk_strdup(va_arg(*app, const char *));
 	}
 	return self;
 }

 // destructor
 static void* person_destroy(tsk_object_t * self)
 { 
 	person_t *person = self;
 	if(person){
 		TSK_FREE(person->firstName);
 		TSK_FREE(person->lastName);
 	}
 	return self;
 }
 
 // comparator
 static int person_cmp(const tsk_object_t *object1, const tsk_object_t *object1)
 {
 	const person_t *person1 = object1;
 	const person_t *person2 = object2;
	int ret;
	
	// do they have the same name?
	if((ret = tsk_stricmp(person1->name, person2->name))){
		return ret;
	}
	// do they have the same mother?
	if((ret = tsk_object_cmp(person1->mother, person2->mother))){
		return ret;
	}
	
	// they are the same
	return 0;
 }

 //(Object defnition)
 static const tsk_object_def_t person_def_s = 
 {
 	sizeof(person_t),
 	person_create,
 	person_destroy,
 	person_cmp, 
 }person_def_t;

/* test object */
void test_object()
{
	tsk_string_t *a = tsk_object_new(tsk_string_def_t, "first string");
	tsk_string_t *b = tsk_object_new(tsk_string_def_t, "second string");

	a = tsk_object_ref(a);
	a = tsk_object_ref(a);

	a = tsk_object_unref(a);
	a = tsk_object_unref(a);
	a = tsk_object_unref(a);

	tsk_object_delete(a);
	tsk_object_delete(b);
}

#endif /* _TEST_OBJECT_H_ */
