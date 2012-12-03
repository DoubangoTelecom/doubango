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

/**@file thttp_session.h
 * @brief HTTP session.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef THTTP_SESSION_H
#define THTTP_SESSION_H

#include "tinyhttp_config.h"

#include "tinyhttp/auth/thttp_challenge.h"
#include "tinyhttp/thttp_message.h"

#include "tinyhttp/thttp_dialog.h"

#include "tnet_types.h"

#include "tsk_object.h"
#include "tsk_list.h"
#include "tsk_params.h"
#include "tsk_options.h"

THTTP_BEGIN_DECLS

//FD
struct thttp_message_s;

typedef uint64_t thttp_session_id_t;			
#define THTTP_SESSION_INVALID_ID				0
#define THTTP_SESSION_INVALID_HANDLE			tsk_null

/** List of all supported options.
* To pass an option to the sesion, use @ref THTTP_SESSION_SET_OPTION() macro.
*/
typedef enum thttp_session_option_e
{
	THTTP_SESSION_OPTION_TIMEOUT,
	THTTP_SESSION_OPTION_TTL,
	// To be continued...
}
thttp_session_option_t;

typedef enum thttp_session_param_type_e
{
	httpp_null = 0,

	httpp_option,
	httpp_cred,
	httpp_header,
	httpp_userdata,
}
thttp_session_param_type_t;

/**@ingroup thttp_session_group
* @def THTTP_SESSION_SET_OPTION
* Adds or updates an option. 
* This is a helper macro for @ref thttp_session_create and @ref thttp_session_set.
* @param ID_ENUM The id of the option to add/update (@ref thttp_session_option_t).
* @param VALUE_STR The new value of the option (<i>const char*</i>).
*
* @code
// session = thttp_session_create(stack,
thttp_session_set(session,
	THTTP_SESSION_SET_PARAM(THTTP_SESSION_OPTION_TIMEOUT, "6000"),
	THTTP_SESSION_SET_NULL());
* @endcode
*/
/**@ingroup thttp_session_group
* @def THTTP_SESSION_SET_CRED
* Sets the user's creadentials.
* This is a helper macro for @ref thttp_session_create and @ref thttp_session_set.
* @param USERNAME_STR The username (const char*).
* @param PASSWORD_STR The password(const char*).
*
* @code
// session = thttp_session_create(stack,
thttp_session_set(session,
	THTTP_SESSION_SET_CRED("ali baba", "open sesame"),
	THTTP_SESSION_SET_NULL());
* @endcode
*/
/**@ingroup thttp_session_group
* @def THTTP_SESSION_SET_HEADER
* Adds new HTTP headers to the session. The value of the header will be updated if it already exist.
* This is a helper macro for @ref thttp_session_create and @ref thttp_session_set.
* @param NAME_STR The name of the header (<i>const char*</i>) to add or update.
* @param VALUE_STR The value of the header (<i>const char*</i>). Should not contains the trailing CRLF.
* @sa @ref THTTP_SESSION_UNSET_HEADER
*
* @code
// session = thttp_session_create(stack,
thttp_session_set(session,
	THTTP_SESSION_SET_HEADER("Pragma", "No-Cache"),
	THTTP_SESSION_SET_HEADER("Connection", "Keep-Alive"),
	THTTP_SESSION_SET_NULL());
* @endcode
*/
/**@ingroup thttp_session_group
* @def THTTP_SESSION_UNSET_HEADER
* Removes a header. This header should be previously added by using @ref THTTP_SESSION_SET_HEADER().
* This is a helper macro for @ref thttp_session_create and @ref thttp_session_set.
* @param NAME_STR The name of the header (<i>const char*</i>) to remove.
* @sa @ref THTTP_SESSION_SET_HEADER
*
* @code
// session = thttp_session_create(stack,
thttp_session_set(session,
	THTTP_SESSION_UNSET_HEADER("Pragma"),
	THTTP_SESSION_UNSET_HEADER("Connection"),
	THTTP_SESSION_SET_NULL());
* @endcode
*/
/**@ingroup thttp_session_group
* @def THTTP_SESSION_SET_USERDATA
* Sets user data (context). Will be return to the application layer each time the callback function is called.
* This is a helper macro for @ref thttp_session_create and @ref thttp_session_set.
* @param USERDATA_PTR A pointer to the data(const void*).
*
* @code
// session = thttp_session_create(stack,
thttp_session_set(session,
	THTTP_SESSION_SET_USERDATA(ctx),
	THTTP_SESSION_SET_NULL());
* @endcode
*/
/**@ingroup thttp_session_group
* @def THTTP_SESSION_SET_NULL
* Ends session parameters. Must always be the last one.
*/
#define THTTP_SESSION_SET_OPTION(ID_ENUM, VALUE_STR)			httpp_option, (thttp_session_option_t)ID_ENUM, (const char*)VALUE_STR
#define THTTP_SESSION_SET_CRED(USERNAME_STR, PASSWORD_STR)		httpp_cred, (const char*)USERNAME_STR, (const char*)PASSWORD_STR
#define THTTP_SESSION_SET_HEADER(NAME_STR, VALUE_STR)			httpp_header, (const char*)NAME_STR, (const char*)VALUE_STR
#define THTTP_SESSION_UNSET_HEADER(NAME_STR)					THTTP_SESSION_SET_HEADER(NAME_STR, (const char*)-1)
#define THTTP_SESSION_SET_USERDATA(USERDATA_PTR)				httpp_userdata, (const void*)USERDATA_PTR
#define THTTP_SESSION_SET_NULL()								httpp_null

typedef struct thttp_session_s
{
	TSK_DECLARE_OBJECT;

	thttp_session_id_t id;
	const struct thttp_stack_s* stack;
	const void* userdata; // user's context
	tsk_options_L_t *options;
	tsk_params_L_t *headers;

	tnet_fd_t fd;

	thttp_challenges_L_t *challenges;
	thttp_dialogs_L_t* dialogs;

	struct{
		char* usename;
		char* password;
	}cred;

	TSK_DECLARE_SAFEOBJ;
}
thttp_session_t;

typedef tsk_list_t thttp_sessions_L_t; /**< List of @ref thttp_session_handle_t elements. */

/** Pointer to a HTTP/HTTPS session. */
typedef void thttp_session_handle_t;
/** Pointer to a HTTP/HTTPS stack object. */
typedef void thttp_stack_handle_t;

TINYHTTP_API thttp_session_handle_t* thttp_session_create(const thttp_stack_handle_t* stack, ...);
TINYHTTP_API int thttp_session_set(thttp_session_handle_t *self, ...);
TINYHTTP_API thttp_session_id_t thttp_session_get_id(const thttp_session_handle_t *self);
TINYHTTP_API const void* thttp_session_get_userdata(const thttp_session_handle_t *self);
TINYHTTP_API int thttp_session_closefd(thttp_session_handle_t *self);

int thttp_session_update_challenges(thttp_session_t *self, const thttp_response_t* response, tsk_bool_t answered);

int thttp_session_signal_closed(thttp_session_t *self);
int thttp_session_signal_error(thttp_session_t *self);


thttp_session_t* thttp_session_get_by_fd(thttp_sessions_L_t* sessions, tnet_fd_t fd);


TINYHTTP_GEXTERN const tsk_object_def_t *thttp_session_def_t;

THTTP_END_DECLS

#endif /* THTTP_SESSION_H */

