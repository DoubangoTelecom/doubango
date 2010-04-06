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
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file thttp_action.h
 * @brief HTTP action.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef THTTP_ACTION_H
#define THTTP_ACTION_H

#include "tinyHTTP_config.h"

#include "tinyHTTP/thttp_session.h"

#include "tsk_buffer.h"
#include "tsk_list.h"
#include "tsk_params.h"

THTTP_BEGIN_DECLS

#define THTTP_ACTION_CREATE(type, url, method, app)		tsk_object_new(thttp_action_def_t, (thttp_action_type_t)type, (const char*)url, (const char*)method, (va_list*)app)

typedef uint64_t thttp_action_id_t;			
#define THTTP_ACTION_INVALID_ID				0
#define THTTP_ACTION_INVALID_HANDLE			tsk_null

typedef enum thttp_action_type_e
{
	/* Outgoing GET, PUT, HEAD, DELETE, .... */
	atype_o_request,
	/* Incoming message */
	atype_i_message,

	/* common */
	atype_closed,
	atype_close,
	atype_cancel,

}
thttp_action_type_t;

typedef enum thttp_action_param_type_e
{
	aptype_null = tsk_null,

	aptype_param,
	aptype_header,
	aptype_payload,
}
thttp_action_param_type_t;

#define THTTP_ACTION_SET_PARAM(NAME_STR, VALUE_STR)			aptype_param, (const char*)NAME_STR, (const char*)VALUE_STR
#define THTTP_ACTION_SET_HEADER(NAME_STR, VALUE_STR)		aptype_header, (const char*)NAME_STR, (const char*)VALUE_STR
#define THTTP_ACTION_SET_PAYLOAD(PAY_PTR, PAY_SIZE)			aptype_payload, (const void*)PAY_PTR, (size_t)PAY_SIZE
#define THTTP_ACTION_SET_NULL()								aptype_null

typedef struct thttp_action_s
{
	TSK_DECLARE_OBJECT;
	
	thttp_action_type_t type;
	const char* url;
	const char* method;

	tsk_params_L_t *params;
	tsk_params_L_t *headers;
	tsk_buffer_t* payload;
}
thttp_action_t;

typedef void thttp_action_handle_t;

TINYHTTP_API int thttp_action_perform(thttp_session_handle_t *session, const char* url, const char* method, ...);
#define thttp_action_CONNECT(session, url, ...) thttp_action_perform((thttp_session_handle_t *)session, url, "CONNECT", __VA_ARGS__)
#define thttp_action_DELETE(session, url, ...) thttp_action_perform((thttp_session_handle_t *)session, url, "DELETE", __VA_ARGS__)
#define thttp_action_GET(session, url, ...) thttp_action_perform((thttp_session_handle_t *)session, url, "GET", __VA_ARGS__)
#define thttp_action_HEAD(session, url, ...) thttp_action_perform((thttp_session_handle_t *)session, url, "HEAD", __VA_ARGS__)
#define thttp_action_OPTIONS(session, url, ...) thttp_action_perform((thttp_session_handle_t *)session, url, "OPTIONS", __VA_ARGS__)
#define thttp_action_PATCH(session, url, ...) thttp_action_perform((thttp_session_handle_t *)session, url, "PATCH", __VA_ARGS__)
#define thttp_action_POST(session, url, ...) thttp_action_perform((thttp_session_handle_t *)session, url, "POST", __VA_ARGS__)
#define thttp_action_PUT(session, url, ...) thttp_action_perform((thttp_session_handle_t *)session, url, "PUT", __VA_ARGS__)
#define thttp_action_TRACE(session, url, ...) thttp_action_perform((thttp_session_handle_t *)session, url, "TRACE", __VA_ARGS__)

typedef tsk_list_t thttp_actions_L_t; /**< List of @ref thttp_action_handle_t elements. */
TINYHTTP_GEXTERN const tsk_object_def_t *thttp_action_def_t;

THTTP_END_DECLS

#endif /* THTTP_ACTION_H */

