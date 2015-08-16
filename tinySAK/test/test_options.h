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
#ifndef _TEST_OPTIONS_H_
#define _TEST_OPTIONS_H_

typedef enum options_e
{
	o_project,
	o_license,
	o_contact,
	o_version,
	o_date
}
options_t;

void test_options()
{
	tsk_options_L_t *options = tsk_list_create();
	tsk_bool_t b;
	int i;
	const char* string;
	const tsk_option_t* o;
	
	tsk_options_add_option(&options, o_project, "doubango");
	tsk_options_add_option(&options, o_project, "doubangooo");
	tsk_options_add_option(&options, o_license, "GPLv3");
	tsk_options_add_option(&options, o_contact, "Mamadou Diop <diopmamadou(at)doubango.org>");
	tsk_options_add_option(&options, o_version, "10");

	b = tsk_options_have_option(options, o_project);
	b = tsk_options_have_option(options, o_date);
	tsk_options_remove_option(options, o_project);
	b = tsk_options_have_option(options, o_project);
	i = tsk_options_get_option_value_as_int(options, o_version);
	string = tsk_options_get_option_value(options, o_version);
	o = tsk_options_get_option_by_id(options, o_contact);
	
	TSK_OBJECT_SAFE_FREE(options);
}


#endif /*_TEST_OPTIONS_H_ */
