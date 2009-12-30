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
/**@file thttp_auth.h
 * @brief HTTP basic/digest authetication (RFC 2617) implementations.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYHTTP_THTTP_AUTH_H
#define TINYHTTP_THTTP_AUTH_H

#include "tinyhttp_config.h"

#include "tsk_md5.h"

TINYHTTP_API size_t thttp_auth_basic_response(const char* userid, const char* password, char** response);

TINYHTTP_API int thttp_auth_digest_HA1(const char* username, const char* realm, const char* password, tsk_md5string_t* ha1);
TINYHTTP_API int thttp_auth_digest_HA1sess(const char* username, const char* realm, const char* password, const char* nonce, const char* cnonce, tsk_md5string_t* ha1sess);

TINYHTTP_API int thttp_auth_digest_HA2(const char* method, const char* uri, const char* entity_body, const char* qop, tsk_md5string_t* ha2);

#endif /* TINYHTTP_THTTP_H */