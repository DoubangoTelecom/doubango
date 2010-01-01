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

/**@file tsip_header_Call_ID.h
 * @brief SIP header 'Call-ID'.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSIP_HEADER_CALL_ID_H_
#define _TSIP_HEADER_CALL_ID_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

/**@def TSIP_HEADER_CALL_ID_CREATE
* Creates new sip 'Call-ID' header.  You must call @ref TSIP_HEADER_CALL_ID_SAFE_FREE to free the header.
* @sa TSIP_HEADER_CALL_ID_SAFE_FREE.
*/
/**@def TSIP_HEADER_CALL_ID_SAFE_FREE
* Safely free a sip 'Call-ID' header previously created using TSIP_HEADER_CALL_ID_CREATE.
* @sa TSIP_HEADER_CALL_ID_CREATE.
*/
#define TSIP_HEADER_CALL_ID_VA_ARGS(call_id)	tsip_header_Call_ID_def_t, (const char*)call_id
#define TSIP_HEADER_CALL_ID_CREATE(call_id)		tsk_object_new(TSIP_HEADER_CALL_ID_VA_ARGS(call_id))
#define TSIP_HEADER_CALL_ID_SAFE_FREE(self)		tsk_object_unref(self), self = 0

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SIP header 'Call-ID'.
/// @author	Mamadou
/// @date	12/3/2009
///
/// @par ABNF: Call-ID / i
/// callid	=  	word  [ "@" word ] 
/// Call-ID	= 	( "Call-ID" / "i" ) HCOLON callid
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Call_ID_s
{	
	TSIP_DECLARE_HEADER;
	char *value;
}
tsip_header_Call_ID_t;

void tsip_header_Call_ID_random(tsk_istr_t *result);
tsip_header_Call_ID_t *tsip_header_Call_ID_parse(const char *data, size_t size);

TINYSIP_GEXTERN const void *tsip_header_Call_ID_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_CALL_ID_H_ */

