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