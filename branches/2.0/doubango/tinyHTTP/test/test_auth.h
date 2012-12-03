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
	{ "Aladdin"/*Ali baba*/, "open sesame", "QWxhZGRpbjpvcGVuIHNlc2FtZQ==" },
};

void test_basic_auth()
{
	char *response = tsk_null;
	size_t i, size;

	for(i=0; i<sizeof(auth_basic_msgs)/sizeof(struct auth_basic_msg); i++)
	{
		size = thttp_auth_basic_response(auth_basic_msgs[i].userid, auth_basic_msgs[i].password, &response);
		if(tsk_striequals(auth_basic_msgs[i].xres, response)){
			TSK_DEBUG_INFO("[HTTP_BASIC-%d] ==> OK", i);
		}
		else{
			TSK_DEBUG_INFO("[HTTP_BASIC-%d] ==> NOK", i);
		}
	
		TSK_FREE(response);
	}
}


//========================================================================

struct auth_ws
{
	const char* key;
	const char* xres;
};

struct auth_ws auth_ws_msgs[] = 
{
	{ "dGhlIHNhbXBsZSBub25jZQ==", "s3pPLMBiTxaQ9kYGzzhZRbK+xOo=" },
	{ "x3JJHMbDL1EzLkh9GBhXDw==", "HSmrc0sMlYUkAGmm5OPpG2HaGWk=" },
};

void test_ws_auth()
{
	thttp_auth_ws_keystring_t response = {0};
	size_t i, size;

	for(i=0; i<sizeof(auth_ws_msgs)/sizeof(struct auth_ws); i++)
	{
		size = thttp_auth_ws_response(auth_ws_msgs[i].key, &response);
		if(tsk_striequals(auth_ws_msgs[i].xres, response)){
			TSK_DEBUG_INFO("[WS_AUTH-%d] ==> OK", i);
		}
		else{
			TSK_DEBUG_INFO("[WS_AUTH-%d] ==> NOK", i);
		}
	}
}



//========================================================================


struct auth_digest_msg
{
	const char* username;
	const char* password;
	const char* realm;
	const char* nonce;
	const char* method;
	const char* uri;
	const char* qop;
	const char* nc;
	const char* cnonce;
	const char* opaque;
	const char* entitybody;

	const char* response;
};

struct auth_digest_msg auth_digest_msgs[] = 
{
	{
		"Mufasa",
		"Circle Of Life",
		"testrealm@host.com",
		"dcd98b7102dd2f0e8b11d0f600bfb0c093",
		"GET",
		"/dir/index.html",
		"auth",
		"00000001",
		"0a4f113b",
		"5ccc069c403ebaf9f0171e9517f40e41",
		0,

		"6629fae49393a05397450978507c4ef1"
	}
	,


};

void test_digest_auth()
{
	tsk_md5string_t response, ha1, ha2;
	size_t i;

	for(i=0; i<sizeof(auth_digest_msgs)/sizeof(struct auth_digest_msg); i++)
	{
		tsk_buffer_t* entitybody;
		/* HA1 */
		thttp_auth_digest_HA1(auth_digest_msgs[i].username, 
			auth_digest_msgs[i].realm, 
			auth_digest_msgs[i].password, 
			&ha1);

		/* HA2 */
		entitybody = tsk_buffer_create(auth_digest_msgs[i].entitybody, strlen(auth_digest_msgs[i].entitybody));
		thttp_auth_digest_HA2(auth_digest_msgs[i].method,
			auth_digest_msgs[i].uri,
			entitybody,
			auth_digest_msgs[i].qop,
			&ha2);
		TSK_OBJECT_SAFE_FREE(entitybody);

		/* RESPONSE */
		thttp_auth_digest_response(&ha1, 
			auth_digest_msgs[i].nonce,
			auth_digest_msgs[i].nc,
			auth_digest_msgs[i].cnonce,
			auth_digest_msgs[i].qop,
			&ha2,
			&response);

		if(tsk_striequals(auth_digest_msgs[i].response, response)){
			TSK_DEBUG_INFO("[HTTP_DIGEST-%d] ==> OK", i);
		}
		else{
			TSK_DEBUG_INFO("[HTTP_DIGEST-%d] ==> NOK", i);
		}
	}
}




#endif /* _TEST_AUTH_H_ */
