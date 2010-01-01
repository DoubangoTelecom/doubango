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

/**@file tsip_header_User_Agent.h
 * @brief SIP header 'User-Agent'.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSIP_HEADER_USER_AGENT_H_
#define _TSIP_HEADER_USER_AGENT_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

/**@def TSIP_HEADER_USER_AGENT_CREATE
* Creates new sip 'User-Agent' header.  You must call @ref TSIP_HEADER_USER_AGENT_SAFE_FREE to free the header.
* @sa TSIP_HEADER_USER_AGENT_SAFE_FREE.
*/
/**@def TSIP_HEADER_USER_AGENT_SAFE_FREE
* Safely free a sip 'User-Agent' header previously created using TSIP_HEADER_USER_AGENT_CREATE.
* @sa TSIP_HEADER_USER_AGENT_CREATE.
*/
#define TSIP_HEADER_USER_AGENT_VA_ARGS(ua)		tsip_header_User_Agent_def_t, (const char*)ua
#define TSIP_HEADER_USER_AGENT_CREATE(ua)		tsk_object_new(TSIP_HEADER_USER_AGENT_VA_ARGS(ua))
#define TSIP_HEADER_USER_AGENT_SAFE_FREE(self)	tsk_object_unref(self), self = 0


#define TSIP_HEADER_USER_AGENT_DEFAULT			"IM-client/OMA1.0 doubango/v0.0.0"
////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SIP header 'User-Agent'.
/// @author	Mamadou
/// @date	12/3/2009
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

tsip_header_User_Agent_t *tsip_header_User_Agent_parse(const char *data, size_t size);

TINYSIP_GEXTERN const void *tsip_header_User_Agent_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_USER_AGENT_H_ */

