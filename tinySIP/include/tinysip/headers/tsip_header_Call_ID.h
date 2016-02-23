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

/**@file tsip_header_Call_ID.h
 * @brief SIP header 'Call-ID'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_CALL_ID_H_
#define _TSIP_HEADER_CALL_ID_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

#include "tsk_uuid.h"

TSIP_BEGIN_DECLS

#define TSIP_HEADER_CALL_ID_VA_ARGS(call_id)	tsip_header_Call_ID_def_t, (const char*)call_id

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	SIP header 'Call-ID'.
///
/// @par ABNF: Call-ID / i
/// callid	=  	word  [ "@" word ]
/// Call-ID	= 	( "Call-ID" / "i" ) HCOLON callid
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Call_ID_s {
    TSIP_DECLARE_HEADER;
    char *value;
}
tsip_header_Call_ID_t;

TINYSIP_API tsip_header_Call_ID_t* tsip_header_Call_ID_create(const char* call_id);

int tsip_header_Call_ID_random(tsk_uuidstring_t *result);
TINYSIP_API tsip_header_Call_ID_t *tsip_header_Call_ID_parse(const char *data, tsk_size_t size);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_header_Call_ID_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_CALL_ID_H_ */

