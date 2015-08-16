/*
* Copyright (C) 2012 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango(dot)org>
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

/**@file thttp_header_Sec_WebSocket_Protocol.h
 * @brief WebSocket 'Sec-WebSocket-Protocol.h' header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango(dot)org>
 *
 */
#ifndef _THTTP_HEADER_SEC_WEBSOCKET_PROTOCOL_H_
#define _THTTP_HEADER_SEC_WEBSOCKET_PROTOCOL_H_

#include "tinyhttp_config.h"
#include "tinyhttp/headers/thttp_header.h"

#include "tsk_string.h"

THTTP_BEGIN_DECLS

#define THTTP_HEADER_SEC_WEBSOCKET_PROTOCOL_VA_ARGS(protocol)		thttp_header_Sec_WebSocket_Protocol_def_t, (const char*)protocol

typedef struct thttp_header_Sec_WebSocket_Protocol_s
{	
	THTTP_DECLARE_HEADER;
	
	tsk_strings_L_t *values;
}
thttp_header_Sec_WebSocket_Protocol_t;

thttp_header_Sec_WebSocket_Protocol_t *thttp_header_Sec_WebSocket_Protocol_parse(const char *data, tsk_size_t size);

thttp_header_Sec_WebSocket_Protocol_t* thttp_header_Sec_WebSocket_Protocol_create(const char* protocol);
thttp_header_Sec_WebSocket_Protocol_t* thttp_header_Sec_WebSocket_Protocol_create_null();

TINYHTTP_GEXTERN const tsk_object_def_t *thttp_header_Sec_WebSocket_Protocol_def_t;

THTTP_END_DECLS

#endif /* _THTTP_HEADER_SEC_WEBSOCKET_PROTOCOL_H_ */
