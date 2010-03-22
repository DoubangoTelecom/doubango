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

/**@file tsip_header_SIP_ETag.h
 * @brief SIP header 'SIP-ETag'.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSIP_HEADER_SIP_ETAG_H_
#define _TSIP_HEADER_SIP_ETAG_H_

#include "tinySIP_config.h"
#include "tinySIP/headers/tsip_header.h"

TSIP_BEGIN_DECLS

/**@def TSIP_HEADER_SIP_ETAG_CREATE
* Creates new sip 'SIP-ETag' header as per RFC 3903.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/
#define TSIP_HEADER_SIP_ETAG_VA_ARGS(etag)		tsip_header_SIP_ETag_def_t, (const char*)etag
#define TSIP_HEADER_SIP_ETAG_CREATE(etag)		tsk_object_new(TSIP_HEADER_SIP_ETAG_VA_ARGS(etag))
#define TSIP_HEADER_SIP_ETAG_CREATE_NULL()		TSIP_HEADER_SIP_ETAG_CREATE(tsk_null)


/**
 * @struct	tsip_header_SIP_ETag_s
 *
 * 	SIP header 'SIP-ETag' as per RFC 3903.
 * 	@par ABNF 
 *	SIP-ETag	= 	"SIP-ETag" HCOLON entity-tag
 *	entity-tag = token 
**/
typedef struct tsip_header_SIP_ETag_s
{	
	TSIP_DECLARE_HEADER;
	char *value;
}
tsip_header_SIP_ETag_t;

tsip_header_SIP_ETag_t *tsip_header_SIP_ETag_parse(const char *data, size_t size);

TINYSIP_GEXTERN const void *tsip_header_SIP_ETag_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_SIP_ETAG_H_ */

