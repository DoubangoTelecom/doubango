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
#ifndef _TEST_URL_H_
#define _TEST_URL_H_

/* url encoding/decoding */
void test_url()
{
	const char *url = "http://xcap.example.org/resource-lists/users/sip:RCSUser@example.org/index/~~/resource-lists/list%5B@name=%22rcs%22%5D";
	char *str1, *str2;

	/*decode url*/
	str1 = tsk_url_decode(url);
	printf("test_url/// decoded url:%s\n", str1);

	/*encode url*/
	str2 = tsk_url_encode(str1);
	printf("test_url/// encoded url:%s\n", str2);

	tsk_free((void**)&str1);
	tsk_free((void**)&str2);
}

#endif /* _TEST_URL_H_ */
