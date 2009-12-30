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
#ifndef _TEST_AUTH_H_
#define _TEST_AUTH_H_

struct auth_basic_msg
{
	const char* userid;
	const char* password;

	const char* xres;
};

struct auth_basic_msg auth_basic_msgs[] = 
{
	{ "Aladdin", "open sesame", "QWxhZGRpbjpvcGVuIHNlc2FtZQ==" },
};

void test_basic_auth()
{
	char *response = 0;
	size_t i, size;

	for(i=0; i<sizeof(auth_basic_msgs)/sizeof(struct auth_basic_msg); i++)
	{
		/*===========
		*	Basic 
		*/
		size = thttp_auth_basic_response(auth_basic_msgs[i].userid, auth_basic_msgs[i].password, &response);
		if(tsk_striequals(auth_basic_msgs[i].xres, response))
		{
			TSK_DEBUG_INFO("[HTTP_BASIC-%d] ==> OK", i);
		}
		else
		{
			TSK_DEBUG_INFO("[HTTP_BASIC-%d] ==> NOK", i);
		}
	
		TSK_FREE(response);
	}
}

#endif /* _TEST_AUTH_H_ */