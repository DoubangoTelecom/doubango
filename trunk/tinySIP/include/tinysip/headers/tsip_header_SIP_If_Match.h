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

/**@file tsip_header_SIP_If_Match.h
 * @brief SIP header 'SIP-If-Match'.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSIP_HEADER_SIP_IF_MATCH_H_
#define _TSIP_HEADER_SIP_IF_MATCH_H_

#include "tinySIP_config.h"
#include "tinySIP/headers/tsip_header.h"

TSIP_BEGIN_DECLS

/**@def TSIP_HEADER_SIP_IF_MATCH_CREATE
* Creates new sip 'SIP-If-Match' header as per RFC 3903.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/
#define TSIP_HEADER_SIP_IF_MATCH_VA_ARGS(etag)		tsip_header_SIP_If_Match_def_t, (const char*)etag
#define TSIP_HEADER_SIP_IF_MATCH_CREATE(etag)		tsk_object_new(TSIP_HEADER_SIP_IF_MATCH_VA_ARGS(etag))
#define TSIP_HEADER_SIP_IF_MATCH_CREATE_NULL()		TSIP_HEADER_SIP_IF_MATCH_CREATE(TSIP_NULL)


/**
 * @struct	tsip_header_SIP_If_Match_s
 *
 * 	SIP header 'SIP-If-Match' as per RFC 3903.
 * 	@par ABNF 
 *	SIP-If-Match	= 	"SIP-If-Match" HCOLON entity-tag
 *	entity-tag = token 
**/
typedef struct tsip_header_SIP_If_Match_s
{	
	TSIP_DECLARE_HEADER;
	char *value;
}
tsip_header_SIP_If_Match_t;

tsip_header_SIP_If_Match_t *tsip_header_SIP_If_Match_parse(const char *data, size_t size);

TINYSIP_GEXTERN const void *tsip_header_SIP_If_Match_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_SIP_IF_MATCH_H_ */

