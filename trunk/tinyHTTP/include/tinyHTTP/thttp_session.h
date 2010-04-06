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

/**@file thttp_session.h
 * @brief HTTP session.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef THTTP_SESSION_H
#define THTTP_SESSION_H

#include "tinyHTTP_config.h"

#include "tinyHTTP/auth/thttp_challenge.h"
#include "tinyHTTP/thttp_message.h"

#include "tinyHTTP/thttp_dialog.h"

#include "tnet_types.h"

#include "tsk_object.h"
#include "tsk_list.h"
#include "tsk_params.h"

THTTP_BEGIN_DECLS

//FD
struct thttp_message_s;

typedef uint64_t thttp_session_id_t;			
#define THTTP_SESSION_INVALID_ID				0
#define THTTP_SESSION_INVALID_HANDLE			tsk_null

typedef enum thttp_session_param_type_e
{
	sptype_null = tsk_null,

	sptype_param,
	sptype_cred,
	sptype_header,
	sptype_context,
}
thttp_session_param_type_t;

/**@ingroup thttp_session_group
* @def THTTP_SESSION_SET_PARAM
* Adds or updates a parameter. 
* This is a helper macro for @ref thttp_session_create and @ref thttp_session_set.
* @param NAME_STR The name of the parameter to add/update (const char*).
* @param VALUE_STR The new value of the parameter (const char*).
* @code
// session = thttp_session_create(stack,
thttp_session_set(session,
	THTTP_SESSION_SET_PARAM("timeout", "6000"),
	THTTP_SESSION_SET_NULL());
* @endcode
*/
/**@ingroup thttp_session_group
* @def THTTP_SESSION_SET_CRED
* Sets the user's creadentials.
* This is a helper macro for @ref thttp_session_create and @ref thttp_session_set.
* @param USERNAME_STR The username (const char*).
* @param PASSWORD_STR The password(const char*).
* @code
// session = thttp_session_create(stack,
thttp_session_set(session,
	THTTP_SESSION_SET_CRED("sip:bob@example.com", "mysecret"),
	THTTP_SESSION_SET_NULL());
* @endcode
*/
/**@ingroup thttp_session_group
* @def THTTP_SESSION_SET_HEADER
* Adds new HTTP headers to the session.
* This is a helper macro for @ref thttp_session_create and @ref thttp_session_set.
* @param NAME_STR The name of the header (const char*).
* @param VALUE_STR The value of the header (const char*).
* @code
// session = thttp_session_create(stack,
thttp_session_set(session,
	THTTP_SESSION_SET_HEADER("Pragma", "No-Cache"),
	THTTP_SESSION_SET_HEADER("Connection", "Keep-Alive"),
	THTTP_SESSION_SET_NULL());
* @endcode
*/
/**@ingroup thttp_session_group
* @def THTTP_SESSION_SET_CONTEXT
* Sets user data (context). Will be return to the application layer each time the callback function is called.
* This is a helper macro for @ref thttp_session_create and @ref thttp_session_set.
* @param CTX_PTR A pointer to the data(const void*).
* @code
// session = thttp_session_create(stack,
thttp_session_set(session,
	THTTP_SESSION_SET_CONTEXT(ctx),
	THTTP_SESSION_SET_NULL());
* @endcode
*/
/**@ingroup thttp_session_group
* @def THTTP_SESSION_SET_NULL
* Ends session parameters. Must always be the last one.
*/
#define THTTP_SESSION_SET_PARAM(NAME_STR, VALUE_STR)			sptype_param, (const char*)NAME_STR, (const char*)VALUE_STR
#define THTTP_SESSION_SET_CRED(USERNAME_STR, PASSWORD_STR)		sptype_cred, (const char*)USERNAME_STR, (const char*)PASSWORD_STR
#define THTTP_SESSION_SET_HEADER(NAME_STR, VALUE_STR)			sptype_header, (const char*)NAME_STR, (const char*)VALUE_STR
#define THTTP_SESSION_SET_CONTEXT(CTX_PTR)						sptype_context, (const void*)CTX_PTR
#define THTTP_SESSION_SET_NULL()								sptype_null

typedef struct thttp_session_s
{
	TSK_DECLARE_OBJECT;

	thttp_session_id_t id;
	const struct thttp_stack_s* stack;
	const void* context; // usr context
	tsk_params_L_t *params;
	tsk_params_L_t *headers;

	tnet_fd_t fd;

	thttp_challenges_L_t *challenges;
	thttp_dialogs_L_t* dialogs;

	struct{
		char* usename;
		char* password;
	}cred;
}
thttp_session_t;

typedef tsk_list_t thttp_sessions_L_t;

/** Pointer to a HTTP/HTTPS session.
*/
typedef void thttp_session_handle_t;

TINYHTTP_API thttp_session_handle_t* thttp_session_create(const struct thttp_stack_s* stack, ...);
TINYHTTP_API int thttp_session_set(thttp_session_handle_t *self, ...);
TINYHTTP_API thttp_session_id_t thttp_session_get_id(const thttp_session_handle_t *self);
TINYHTTP_API const void* thttp_session_get_context(const thttp_session_handle_t *self);

int thttp_session_update_challenges(thttp_session_t *self, const thttp_response_t* response, tsk_bool_t first);
int thttp_session_signal_closed(thttp_session_t *self);
thttp_session_t* thttp_session_get_by_fd(thttp_sessions_L_t* sessions, tnet_fd_t fd);

typedef tsk_list_t thttp_sessions_L_t; /**< List of @ref thttp_session_handle_t elements. */
TINYHTTP_GEXTERN const tsk_object_def_t *thttp_session_def_t;

THTTP_END_DECLS

#endif /* THTTP_SESSION_H */

