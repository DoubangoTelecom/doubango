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

/**@file thttp_action.h
 * @brief HTTP action.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef THTTP_ACTION_H
#define THTTP_ACTION_H

#include "tinyhttp_config.h"

#include "tinyhttp/thttp_session.h"

#include "tsk_buffer.h"
#include "tsk_list.h"
#include "tsk_params.h"
#include "tsk_options.h"

THTTP_BEGIN_DECLS

typedef uint64_t thttp_action_id_t;
#define THTTP_ACTION_INVALID_ID				0
#define THTTP_ACTION_INVALID_HANDLE			tsk_null

/** List of all supported options.
* To pass an option to the sesion, use @ref THTTP_ACTION_SET_OPTION() macro.
*/
typedef enum thttp_action_option_e {
    THTTP_ACTION_OPTION_TIMEOUT,

}
thttp_action_option_t;

/** List of actions.
*/
typedef enum thttp_action_type_e {
    /* Outgoing GET, PUT, HEAD, DELETE, .... */
    thttp_atype_o_request,
    /* Incoming message */
    thttp_atype_i_message,

    /* common */
    thttp_thttp_atype_closed,
    thttp_atype_error,
    thttp_atype_close,
    thttp_atype_cancel,
    thttp_atype_timedout,

}
thttp_action_type_t;

typedef enum thttp_action_param_type_e {
    thttp_aptype_null = 0,

    thttp_aptype_option,
    thttp_aptype_header,
    thttp_aptype_payload,
}
thttp_action_param_type_t;

/**@ingroup thttp_action_group
* @def THTTP_ACTION_SET_OPTION
* Adds or updates an option.
* This is a helper macro for @a thttp_action_*() functions.
* @param ID_ENUM The id of the option to add/update (@ref thttp_action_option_t).
* @param VALUE_STR The new value of the parameter (<i>const char*</i>).
*
* @code
thttp_action_GET(session, "http://www.google.com",
	THTTP_ACTION_SET_PARAM("timeout", "6000"),
	THTTP_ACTION_SET_NULL());
* @endcode
*/
/**@ingroup thttp_action_group
* @def THTTP_ACTION_SET_HEADER
* Adds new HTTP headers to the request.
* This is a helper macro for @a thttp_action_*() functions.
* @param NAME_STR The name of the header (<i>const char*</i>).
* @param VALUE_STR The value of the header (<i>const char*</i>). Should not contains the trailing CRLF.
*
* @code
thttp_action_GET(session, "http://www.doubango.org"
	THTTP_ACTION_SET_HEADER("Pragma", "No-Cache"),
	THTTP_ACTION_SET_HEADER("Connection", "Keep-Alive"),
	THTTP_ACTION_SET_NULL());
* @endcode
*/
/**@ingroup thttp_action_group
* @def THTTP_ACTION_SET_PAYLOAD
* Adds a content (or payload) to the request. You should also add a content-type header by using
* @ref THTTP_ACTION_SET_HEADER() macro. You should not add the content-length header.
* This is a helper macro for @a thttp_action_*() functions.
* @param PAY_PTR A pointer to the payload (<i>const void*</i>).
* @param PAY_SIZE The size of the payload (<i>tsk_size_t</i>).
*
* @code
thttp_action_PUT(session, "http://www.doubango.org"
	THTTP_ACTION_SET_HEADER("Pragma", "No-Cache"),
	THTTP_ACTION_SET_HEADER("Connection", "Keep-Alive"),
	THTTP_ACTION_SET_HEADER("Content-length", "application/mytype"),

	THTTP_ACTION_SET_PAYLOAD("Salut", 5),

	THTTP_ACTION_SET_NULL());
* @endcode
*/
/**@ingroup thttp_action_group
* @def THTTP_ACTION_SET_NULL
* Ends action parameters. Must always be the last one.
*/
#define THTTP_ACTION_SET_OPTION(ID_ENUM, VALUE_STR)			thttp_aptype_option, (thttp_action_option_t)ID_ENUM, (const char*)VALUE_STR
#define THTTP_ACTION_SET_HEADER(NAME_STR, VALUE_STR)		thttp_aptype_header, (const char*)NAME_STR, (const char*)VALUE_STR
#define THTTP_ACTION_SET_PAYLOAD(PAY_PTR, PAY_SIZE)			thttp_aptype_payload, (const void*)PAY_PTR, (tsk_size_t)PAY_SIZE
#define THTTP_ACTION_SET_NULL()								thttp_aptype_null

typedef struct thttp_action_s {
    TSK_DECLARE_OBJECT;

    thttp_action_type_t type;
    const char* url;
    const char* method;

    tsk_options_L_t *options;
    tsk_params_L_t *headers;
    tsk_buffer_t* payload;
}
thttp_action_t;

typedef void thttp_action_handle_t;

/**@ingroup thttp_action_group
* @def thttp_action_CONNECT
* Sends @a CONNECT method request. This function is non-blocking and the result will be posted to the callback function.
* @param session The @a session (or connection) to use.
* @param urlstring The Request-URI of the request.
* @param ... Any @b THTTP_ACTION_SET_*() macros. MUST ends with @ref THTTP_ACTION_SET_NULL().
* @retval Zero if succeed and non-zero error code otherwise.
*/
/**@ingroup thttp_action_group
* @def thttp_action_DELETE
* Sends @a DELETE method request. This function is non-blocking and the result will be posted to the callback function.
* @param session The @a session (or connection) to use.
* @param urlstring The Request-URI of the request.
* @param ... Any @b THTTP_ACTION_SET_*() macros. MUST ends with @ref THTTP_ACTION_SET_NULL().
* @retval Zero if succeed and non-zero error code otherwise.
*/
/**@ingroup thttp_action_group
* @def thttp_action_GET
* Sends @a GET method request. This function is non-blocking and the result will be posted to the callback function.
* @param session The @a session (or connection) to use.
* @param urlstring The Request-URI of the request.
* @param ... Any @b THTTP_ACTION_SET_*() macros. MUST ends with @ref THTTP_ACTION_SET_NULL().
* @retval Zero if succeed and non-zero error code otherwise.
*/
/**@ingroup thttp_action_group
* @def thttp_action_HEAD
* Sends @a HEAD method request. This function is non-blocking and the result will be posted to the callback function.

* @param session The @a session (or connection) to use.
* @param urlstring The Request-URI of the request.
* @param ... Any @b THTTP_ACTION_SET_*() macros. MUST ends with @ref THTTP_ACTION_SET_NULL().
* @retval Zero if succeed and non-zero error code otherwise.
*/
/**@ingroup thttp_action_group
* @def thttp_action_OPTIONS
* Sends @a OPTIONS method request. This function is non-blocking and the result will be posted to the callback function.
* @param session The @a session (or connection) to use.
* @param urlstring The Request-URI of the request.
* @param ... Any @b THTTP_ACTION_SET_*() macros. MUST ends with @ref THTTP_ACTION_SET_NULL().
* @retval Zero if succeed and non-zero error code otherwise.
*/
/**@ingroup thttp_action_group
* @def thttp_action_PATCH
* Sends @a PATCH method request. This function is non-blocking and the result will be posted to the callback function.

* @param session The @a session (or connection) to use.
* @param urlstring The Request-URI of the request.
* @param ... Any @b THTTP_ACTION_SET_*() macros. MUST ends with @ref THTTP_ACTION_SET_NULL().
* @retval Zero if succeed and non-zero error code otherwise.
*/
/**@ingroup thttp_action_group
* @def thttp_action_POST
* Sends @a POST method request. This function is non-blocking and the result will be posted to the callback function.
* @param session The @a session (or connection) to use.
* @param urlstring The Request-URI of the request.
* @param ... Any @b THTTP_ACTION_SET_*() macros. MUST ends with @ref THTTP_ACTION_SET_NULL().
* @retval Zero if succeed and non-zero error code otherwise.
*/
/**@ingroup thttp_action_group
* @def thttp_action_PUT
* Sends @a PUT method request. This function is non-blocking and the result will be posted to the callback function.
* @param session The @a session (or connection) to use.
* @param urlstring The Request-URI of the request.
* @param ... Any @b THTTP_ACTION_SET_*() macros. MUST ends with @ref THTTP_ACTION_SET_NULL().
* @retval Zero if succeed and non-zero error code otherwise.
*/
/**@ingroup thttp_action_group
* @def thttp_action_TRACE
* Sends @a TRACE method request. This function is non-blocking and the result will be posted to the callback function.
* @param session The @a session (or connection) to use.
* @param urlstring The Request-URI of the request.
* @param ... Any @b THTTP_ACTION_SET_*() macros. MUST ends with @ref THTTP_ACTION_SET_NULL().
* @retval Zero if succeed and non-zero error code otherwise.
*/
TINYHTTP_API int thttp_action_perform(thttp_session_handle_t *session, const char* urlstring, const char* method, ...);
#define thttp_action_CONNECT(session, urlstring, ...) thttp_action_perform(session, urlstring, "CONNECT", __VA_ARGS__)
#define thttp_action_DELETE(session, urlstring, ...) thttp_action_perform(session, urlstring, "DELETE", __VA_ARGS__)
#define thttp_action_GET(session, urlstring, ...) thttp_action_perform(session, urlstring, "GET", __VA_ARGS__)
#define thttp_action_HEAD(session, urlstring, ...) thttp_action_perform(session, urlstring, "HEAD", __VA_ARGS__)
#define thttp_action_OPTIONS(session, urlstring, ...) thttp_action_perform(session, urlstring, "OPTIONS", __VA_ARGS__)
#define thttp_action_PATCH(session, urlstring, ...) thttp_action_perform(session, urlstring, "PATCH", __VA_ARGS__)
#define thttp_action_POST(session, urlstring, ...) thttp_action_perform(session, urlstring, "POST", __VA_ARGS__)
#define thttp_action_PUT(session, urlstring, ...) thttp_action_perform(session, urlstring, "PUT", __VA_ARGS__)
#define thttp_action_TRACE(session, urlstring, ...) thttp_action_perform(session, urlstring, "TRACE", __VA_ARGS__)

TINYHTTP_API thttp_action_t* thttp_action_create(thttp_action_type_t type, const char* urlstring, const char* method, va_list* app);

typedef tsk_list_t thttp_actions_L_t; /**< List of @ref thttp_action_handle_t elements. */
TINYHTTP_GEXTERN const tsk_object_def_t *thttp_action_def_t;

THTTP_END_DECLS

#endif /* THTTP_ACTION_H */

