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
/**@file txcap_action.h
 * @brief XCAP actions.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYXCAP_TXCAP_ACTION_H
#define TINYXCAP_TXCAP_ACTION_H

#include "tinyXCAP_config.h"

#include "txcap.h"

#include "tinyHTTP/thttp_action.h"

TXCAP_BEGIN_DECLS

/** List of all supported types.
*/
typedef enum txcap_action_type_e
{
	txcap_atp_create,
	txcap_atp_replace,
	txcap_atp_fetch,
	txcap_atp_delete
}
txcap_action_type_t;

/** List of all supported targets.
*/
typedef enum txcap_action_target_e
{
	txcap_atg_element,
	txcap_atg_document,
	txcap_atg_attribute
}
txcap_action_target_t;

/** List of all supported options.
* To pass an option to the sesion, use @ref TXCAP_ACTION_SET_OPTION() macro.
*/
typedef enum txcap_action_option_e
{
	TXCAP_ACTION_OPTION_TIMEOUT = THTTP_ACTION_OPTION_TIMEOUT,

	//TXCAP_ACTION_OPTION_*** = 0xFF,
	//TXCAP_ACTION_OPTION_****,
}
txcap_action_option_t;

typedef enum txcap_action_param_type_e
{
	txcap_apt_null = tsk_null,

	txcap_apt_option,
	txcap_apt_header,
	txcap_apt_payload,
	txcap_apt_selector,
	txcap_apt_urlstring
}
txcap_action_param_type_t;

/**@ingroup txcap_action_group
* @def TXCAP_ACTION_SET_OPTION
* Adds or updates an option. 
* This is a helper macro for @a txcap_action_*() functions.
* @param ID_INT The id of the option to add/update (@ref thhtp_action_option_t).
* @param VALUE_STR The new value of the parameter (<i>const char*</i>).
*
* @code
txcap_action_GET(session, "xcap://www.google.com",
	TXCAP_ACTION_SET_PARAM("timeout", "6000"),
	TXCAP_ACTION_SET_NULL());
* @endcode
*/
/**@ingroup txcap_action_group
* @def TXCAP_ACTION_SET_HEADER
* Adds new XCAP headers to the request.
* This is a helper macro for @a txcap_action_*() functions.
* @param NAME_STR The name of the header (<i>const char*</i>).
* @param VALUE_STR The value of the header (<i>const char*</i>). Should not contains the trailing CRLF.
*
* @code
txcap_action_GET(session, "xcap://www.doubango.org"
	TXCAP_ACTION_SET_HEADER("Pragma", "No-Cache"),
	TXCAP_ACTION_SET_HEADER("Connection", "Keep-Alive"),
	TXCAP_ACTION_SET_NULL());
* @endcode
*/
/**@ingroup txcap_action_group
* @def TXCAP_ACTION_SET_PAYLOAD
* Adds a content (or payload) to the request. You should also add a content-type header by using 
* @ref TXCAP_ACTION_SET_HEADER() macro. You should not add the content-length header.
* This is a helper macro for @a txcap_action_*() functions.
* @param PAY_PTR A pointer to the payload (<i>const void*</i>).
* @param PAY_SIZE The size of the payload (<i>size_t</i>).
*
* @code
txcap_action_PUT(session, "xcap://www.doubango.org"
	TXCAP_ACTION_SET_HEADER("Pragma", "No-Cache"),
	TXCAP_ACTION_SET_HEADER("Connection", "Keep-Alive"),
	TXCAP_ACTION_SET_HEADER("Content-length", "application/mytype"),
	
	TXCAP_ACTION_SET_PAYLOAD("Salut", 5),

	TXCAP_ACTION_SET_NULL());
* @endcode
*/
/**@ingroup txcap_action_group
* @def TXCAP_ACTION_SET_SELECTOR
*/
/**@ingroup txcap_action_group
* @def TXCAP_ACTION_SET_NULL
* Ends action parameters. Must always be the last one.
*/
#define TXCAP_ACTION_SET_OPTION(ID_INT, VALUE_STR)			txcap_apt_option, (thhtp_action_option_t)ID_INT, (const char*)VALUE_STR
#define TXCAP_ACTION_SET_HEADER(NAME_STR, VALUE_STR)		txcap_apt_header, (const char*)NAME_STR, (const char*)VALUE_STR
#define TXCAP_ACTION_SET_PAYLOAD(PAY_PTR, PAY_SIZE)			txcap_apt_payload, (const void*)PAY_PTR, (size_t)PAY_SIZE
#define TXCAP_ACTION_SET_SELECTOR(AUID_STR, ...)			txcap_apt_selector, (const char*)AUID_STR, __VA_ARGS__
#define TXCAP_ACTION_SET_REQUEST_URI(URI_STR)				txcap_apt_urlstring, (const char*)URI_STR
#define TXCAP_ACTION_SET_NULL()								txcap_apt_null

TINYXCAP_API int txcap_action_perform(txcap_stack_handle_t* stack, txcap_action_type_t type, txcap_action_target_t target, ...);
#define txcap_action_create_element(stack, ...)	txcap_action_perform(stack, txcap_atp_create, txcap_atg_element, __VA_ARGS__)
#define txcap_action_create_document(stack, ...) txcap_action_perform(stack, txcap_atp_create, txcap_atg_document, __VA_ARGS__)
#define txcap_action_create_attribute(stack, ...) txcap_action_perform(stack, txcap_atp_create, txcap_atg_attribute, __VA_ARGS__)
#define txcap_action_replace_element(stack, ...) txcap_action_perform(stack, txcap_atp_replace, txcap_atg_element, __VA_ARGS__)
#define txcap_action_replace_document(stack, ...) txcap_action_perform(stack, txcap_atp_replace, txcap_atg_document, __VA_ARGS__)
#define txcap_action_replace_attribute(stack, ...) txcap_action_perform(stack, txcap_atp_replace, txcap_atg_attribute, __VA_ARGS__)
#define txcap_action_fetch_element(stack, ...) txcap_action_perform(stack, txcap_atp_fetch, txcap_atg_element, __VA_ARGS__)
#define txcap_action_fetch_document(stack, ...) txcap_action_perform(stack, txcap_atp_fetch, txcap_atg_document, __VA_ARGS__)
#define txcap_action_fetch_attribute(stack, ...) txcap_action_perform(stack, txcap_atp_fetch, txcap_atg_attribute, __VA_ARGS__)
#define txcap_action_delete_element(stack, ...) txcap_action_perform(stack, txcap_atp_delete, txcap_atg_element, __VA_ARGS__)
#define txcap_action_delete_document(stack, ...) txcap_action_perform(stack, txcap_atp_delete, txcap_atg_document, __VA_ARGS__)
#define txcap_action_delete_attribute(stack, ...) txcap_action_perform(stack, txcap_atp_delete, txcap_atg_attribute, __VA_ARGS__)

TXCAP_END_DECLS

#endif /* TINYXCAP_TXCAP_ACTION_H */
