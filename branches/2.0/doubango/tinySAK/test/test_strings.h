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
#ifndef _TEST_STRINGS_H_
#define _TEST_STRINGS_H_

/* test string manipulation */
void test_strings()
{
	char* str = tsk_null;
	int index;

	/* IndexOf */
	index = tsk_strindexOf("C:\\test.zip", 5, "\\");
	index = tsk_strindexOf("C:\\test.zip", tsk_strlen("C:\\test.zip"), "abc");
	index = tsk_strLastIndexOf("C:\\my dirctory\\test.zip", tsk_strlen("C:\\my dirctory\test.zip"), "\\");
	index = tsk_strLastIndexOf("C:\\my dirctory\\test.zip", tsk_strlen("C:\\my dirctory\test.zip"), ".");

	/* LastIndexOf */
	
	/* concatenation */
	tsk_strcat(&str, "   first      ");
	printf("test_strings/// strcat=%s\n", str);

	/* Trim Right */
	tsk_strtrim_right(&str);
	printf("test_strings/// trim_right=%s\n", str);

	tsk_strcat(&str, "second");
	printf("test_strings/// strcat=%s\n", str);

	/* Trim Left */
	tsk_strtrim_left(&str);
	printf("test_strings/// trim_left=%s\n", str);

	tsk_free((void**)&str);
	
	/* sprintf */
	tsk_sprintf(&str, " \n\t%s %s\r  ", "first", "second");
	printf("test_strings/// strcat=\"%s\"\n", str);

	/* Trim Both */
	tsk_strtrim_both(&str);
	printf("test_strings/// trim_both=%s\n", str);

	/* Quote */
	tsk_strquote(&str);
	printf("test_strings/// quote=%s\n", str);

	/* Unquote */
	tsk_strunquote(&str);
	printf("test_strings/// unquote=%s\n", str);

	tsk_free((void**)&str);
}

#endif /* _TEST_STRINGS_H_ */
