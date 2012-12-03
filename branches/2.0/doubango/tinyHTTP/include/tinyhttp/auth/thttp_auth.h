/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
/**@file thttp_auth.h
 * @brief HTTP basic/digest authetication (RFC 2617) implementations.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYHTTP_THTTP_AUTH_H
#define TINYHTTP_THTTP_AUTH_H

#include "tinyhttp_config.h"

#include "tsk_md5.h"
#include "tsk_buffer.h"

THTTP_BEGIN_DECLS

typedef char nonce_count_t[9];
typedef char thttp_auth_ws_keystring_t[255];
#define THTTP_NCOUNT_2_STRING(nc_int32, nc_string)							\
	{																		\
		tsk_size_t i = 7;														\
		do{																	\
			nc_string[7-i]= "0123456789abcdef"[(nc_int32 >> i*4) & 0xF];	\
		}																	\
		while(i--);															\
		nc_string[8] = '\0';												\
	}

TINYHTTP_API tsk_size_t thttp_auth_basic_response(const char* userid, const char* password, char** response);

TINYHTTP_API int thttp_auth_digest_HA1(const char* username, const char* realm, const char* password, tsk_md5string_t* ha1);
TINYHTTP_API int thttp_auth_digest_HA1sess(const char* username, const char* realm, const char* password, const char* nonce, const char* cnonce, tsk_md5string_t* ha1sess);

TINYHTTP_API int thttp_auth_digest_HA2(const char* method, const char* url, const tsk_buffer_t* entity_body, const char* qop, tsk_md5string_t* ha2);

TINYHTTP_API int thttp_auth_digest_response(const tsk_md5string_t *ha1, const char* nonce, const nonce_count_t noncecount, const char* cnonce, 
											const char* qop, const tsk_md5string_t* ha2, tsk_md5string_t* response);

TINYHTTP_API tsk_size_t thttp_auth_ws_response(const char* key, thttp_auth_ws_keystring_t* response);

THTTP_END_DECLS

#endif /* TINYHTTP_THTTP_H */
