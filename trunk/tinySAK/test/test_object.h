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
#ifndef _TEST_OBJECT_H_
#define _TEST_OBJECT_H_

struct String {
	TSK_DECLARE_OBJECT;
	char * text;
};

static void * String_ctor (void * _self, va_list * app)
{ 
	struct String * self = _self;
	const char * text = va_arg(* app, const char *);
	self->text = malloc(strlen(text) + 1);
	assert(self->text);
	strcpy(self->text, text);
	return self;
}

static void String_dtor (void * _self)
{ 
	struct String * self = _self;
	free(self->text), self->text = 0;
}

/*static void * String_clone (const void * _self)
{ 
	const struct String * self = _self;
	return tsk_object_new(String, self->text);
}*/

static const tsk_object_def_t _String = 
{
	sizeof(struct String),
	String_ctor, String_dtor,
	0, 0
};
const void *String = & _String;

//const void *tsk_string_def_t = &tsk_string_def_s;


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

	tsk_object_delete(&a);
	tsk_object_delete(&b);
}

#endif /* _TEST_OBJECT_H_ */