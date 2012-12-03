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

/**@file tsip_header_User_Agent.h
 * @brief SIP header 'User-Agent'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_USER_AGENT_H_
#define _TSIP_HEADER_USER_AGENT_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

#define TSIP_HEADER_USER_AGENT_VA_ARGS(ua)		tsip_header_User_Agent_def_t, (const char*)ua

#define TSIP_HEADER_USER_AGENT_DEFAULT			"IM-client/OMA1.0 doubango/v1.0.0"
////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	SIP header 'User-Agent'.
///
/// @par ABNF : User-Agent	= 	"User-Agent" HCOLON server-val *(LWS server-val)
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_User_Agent_s
{	
	TSIP_DECLARE_HEADER;
	char *value;
}
tsip_header_User_Agent_t;

TINYSIP_API tsip_header_User_Agent_t* tsip_header_User_Agent_create(const char* ua);
TINYSIP_API tsip_header_User_Agent_t* tsip_header_User_Agent_create_null();

TINYSIP_API tsip_header_User_Agent_t *tsip_header_User_Agent_parse(const char *data, tsk_size_t size);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_header_User_Agent_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_USER_AGENT_H_ */

