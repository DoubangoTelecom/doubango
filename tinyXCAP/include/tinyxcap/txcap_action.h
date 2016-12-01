/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou [at) doubango (DOT) org>
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
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#ifndef TINYXCAP_TXCAP_ACTION_H
#define TINYXCAP_TXCAP_ACTION_H

#include "tinyxcap_config.h"

#include "txcap.h"

#include "tinyhttp/thttp_action.h"

TXCAP_BEGIN_DECLS

#define	TXCAP_MIME_TYPE_ELEMENT			"application/xcap-el+xml" /**< as per rfc 4825 subclause 15.2.1 */
#define	TXCAP_MIME_TYPE_ATTRIBUTE		"application/xcap-att+xml" /**< as per rfc 4825 subclause 15.2.2 */
#define	TXCAP_MIME_TYPE_NS				"application/xcap-ns+xml" /**< as per rfc 4825 subclause 15.2.3 */
#define	TXCAP_MIME_TYPE_ERROR			"application/xcap-error+xml" /**< as per rfc 4825 subclause 15.2.4 */

/** List of all supported types.
*/
typedef enum txcap_action_type_e {
    txcap_atp_create,
    txcap_atp_replace,
    txcap_atp_fetch,
    txcap_atp_delete
}
txcap_action_type_t;

/** List of all supported targets.
*/
typedef enum txcap_action_target_e {
    txcap_atg_element,
    txcap_atg_document,
    txcap_atg_attribute
}
txcap_action_target_t;

/** List of all supported options.
* To pass an option to the sesion, use @ref TXCAP_ACTION_SET_OPTION() macro.
*/
typedef enum txcap_action_option_e {
    TXCAP_ACTION_OPTION_TIMEOUT = THTTP_ACTION_OPTION_TIMEOUT,

    //TXCAP_ACTION_OPTION_*** = 0xFF,
    //TXCAP_ACTION_OPTION_****,
}
txcap_action_option_t;

typedef enum txcap_action_param_type_e {
    txcap_apt_null = 0,

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
* @param ID_INT The id of the option to add/update (@ref txcap_action_option_t).
* @param VALUE_STR The new value of the option (<i>const char*</i>).
*
* @code
int ret = txcap_action_fetch_document(stack,
	// action-level options
	TXCAP_ACTION_SET_OPTION(TXCAP_ACTION_OPTION_TIMEOUT, "6000"),
	// selector
	TXCAP_ACTION_SET_SELECTOR("resource-lists",
		TXCAP_SELECTOR_NODE_SET_NULL()),
	// ends parameters
	TXCAP_ACTION_SET_NULL()
	);
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
int ret = txcap_action_fetch_element(stack,
	// action-level headers
	TXCAP_ACTION_SET_HEADER("Pragma", "No-Cache"),
	// selector
	TXCAP_ACTION_SET_SELECTOR("resource-lists",
		TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "rcs"),
		TXCAP_SELECTOR_NODE_SET_NULL()),
	// ends parameters
	TXCAP_ACTION_SET_NULL()
	);
* @endcode
*
* @sa @ref TXCAP_STACK_SET_HEADER
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
const char* PAYLOAD = "....";
int ret = txcap_action_create_element(stack,
	// selector
	TXCAP_ACTION_SET_SELECTOR("resource-lists",
		TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "newlist"),
		TXCAP_SELECTOR_NODE_SET_NULL()),
	// payload
	TXCAP_ACTION_SET_PAYLOAD(PAYLOAD, strlen(PAYLOAD)),
	// ends parameters
	TXCAP_ACTION_SET_NULL()
	);
* @endcode
*/
/**@ingroup txcap_action_group
* @def TXCAP_ACTION_SET_SELECTOR
* Helps building the XCAP Request-URI. You should use @ref TXCAP_ACTION_SET_REQUEST_URI if you want to provide
* your own URI.
* @param AUID_STR The AUID (<i>const char*</i>) to use. You should use @ref TXCAP_STACK_SET_AUID macro to register the the AUID
* if it's missing. This parameter is mandatory.
* @param ... Node selection steps. You must use @a TXCAP_SELECTOR_NODE_SET_*() macros to set these parameters.
* MUST always ends with @ref TXCAP_SELECTOR_NODE_SET_NULL.
*
* @code
int ret = txcap_action_fetch_attribute(stack,
	// action-level options
	TXCAP_ACTION_SET_OPTION(TXCAP_ACTION_OPTION_TIMEOUT, "6000"),
	// headers
	TXCAP_ACTION_SET_HEADER("Pragma", "No-Cache"),
	// action-level selector
	TXCAP_ACTION_SET_SELECTOR("resource-lists",
		TXCAP_SELECTOR_NODE_SET_ATTRIBUTE("list", "name", "rcs"),
		TXCAP_SELECTOR_NODE_SET_POS("entry", 1),
		TXCAP_SELECTOR_NODE_SET_NAME("display-name"),
		TXCAP_SELECTOR_NODE_SET_NULL()),
	// ends parameters
	TXCAP_ACTION_SET_NULL()
	);
* @endcode
*
* @sa @ref TXCAP_ACTION_SET_REQUEST_URI
*/
/**@ingroup txcap_action_group
* @def TXCAP_ACTION_SET_REQUEST_URI
* Sets the request URI. This macro is useful if you want to provide your own request URI instead of using @ref TXCAP_ACTION_SET_SELECTOR.
* @param URI_STR Fully Qualified HTTP/HTTPS URI.
*
* @code
const char* PAYLOAD = "....";
int ret = txcap_action_create_element(stack,
	// custom Request URI
	TXCAP_ACTION_SET_REQUEST_URI("http://doubango.org:8080/services/mycustom/uri"),
	// payload
	TXCAP_ACTION_SET_PAYLOAD(PAYLOAD, strlen(PAYLOAD)),
	// ends parameters
	TXCAP_ACTION_SET_NULL()
	);
	getchar();
* @endcode
*
* @sa @ref TXCAP_ACTION_SET_SELECTOR
*/
/**@ingroup txcap_action_group
* @def TXCAP_ACTION_SET_NULL
* Ends action parameters. Must always be the last one.
*/
#define TXCAP_ACTION_SET_OPTION(ID_INT, VALUE_STR)			txcap_apt_option, (thttp_action_option_t)ID_INT, (const char*)VALUE_STR
#define TXCAP_ACTION_SET_HEADER(NAME_STR, VALUE_STR)		txcap_apt_header, (const char*)NAME_STR, (const char*)VALUE_STR
#define TXCAP_ACTION_UNSET_HEADER(NAME_STR)					TXCAP_ACTION_SET_HEADER(NAME_STR, (const char*)-1)
#define TXCAP_ACTION_SET_PAYLOAD(PAY_PTR, PAY_SIZE)			txcap_apt_payload, (const void*)PAY_PTR, (size_t)PAY_SIZE
#define TXCAP_ACTION_SET_SELECTOR(AUID_STR, ...)			txcap_apt_selector, (const char*)AUID_STR, ##__VA_ARGS__
#define TXCAP_ACTION_SET_REQUEST_URI(URI_STR)				txcap_apt_urlstring, (const char*)URI_STR
#define TXCAP_ACTION_SET_NULL()								txcap_apt_null


/**@ingroup txcap_action_group
* @def txcap_action_create_element
* Creates new element by sending a <i>HTTP/HTTPS PUT</i> request.
* The default Content-Type will be "application/xcap-el+xml", unless you provide your own Content-Type by using @ref TXCAP_ACTION_SET_HEADER().
* @param stack The HTTP/HTTPS stack created using @ref txcap_stack_create().
* @param ... Any @a TXCAP_ACTION_SET_*() macros. MUST ends with @ref TXCAP_ACTION_SET_NULL().
*/
/**@ingroup txcap_action_group
* @def txcap_action_create_document
* Creates new document by sending a <i>HTTP/HTTPS PUT</i> request.
* The default Content-Type will be the one associated with the AUID of the document, unless you provide your own Content-Type by using @ref TXCAP_ACTION_SET_HEADER().
* @param stack The HTTP/HTTPS stack created using @ref txcap_stack_create().
* @param ... Any @a TXCAP_ACTION_SET_*() macros. MUST ends with @ref TXCAP_ACTION_SET_NULL().
*/
/**@ingroup txcap_action_group
* @def txcap_action_create_attribute
* Creates new attribute by sending a <i>HTTP/HTTPS PUT</i> request.
* The default Content-Type will be "application/xcap-att+xml", unless you provide your own Content-Type by using @ref TXCAP_ACTION_SET_HEADER().
* @param stack The HTTP/HTTPS stack created using @ref txcap_stack_create().
* @param ... Any @a TXCAP_ACTION_SET_*() macros. MUST ends with @ref TXCAP_ACTION_SET_NULL().
*/
/**@ingroup txcap_action_group
* @def txcap_action_replace_element
* Replaces an element by sending a <i>HTTP/HTTPS PUT</i> request.
* The default Content-Type will be "application/xcap-el+xml", unless you provide your own Content-Type by using @ref TXCAP_ACTION_SET_HEADER().
* @param stack The HTTP/HTTPS stack created using @ref txcap_stack_create().
* @param ... Any @a TXCAP_ACTION_SET_*() macros. MUST ends with @ref TXCAP_ACTION_SET_NULL().
*/
/**@ingroup txcap_action_group
* @def txcap_action_replace_document
* Replaces a document by sending a <i>HTTP/HTTPS PUT</i> request.
* The default Content-Type will be the one associated with the AUID of the document, unless you provide your own Content-Type by using @ref TXCAP_ACTION_SET_HEADER().
* @param stack The HTTP/HTTPS stack created using @ref txcap_stack_create().
* @param ... Any @a TXCAP_ACTION_SET_*() macros. MUST ends with @ref TXCAP_ACTION_SET_NULL().
*/
/**@ingroup txcap_action_group
* @def txcap_action_replace_attribute
* Replaces an attribute by sending a <i>HTTP/HTTPS PUT</i> request.
* The default Content-Type will be "application/xcap-att+xml", unless you provide your own Content-Type by using @ref TXCAP_ACTION_SET_HEADER().
* @param stack The HTTP/HTTPS stack created using @ref txcap_stack_create().
* @param ... Any @a TXCAP_ACTION_SET_*() macros. MUST ends with @ref TXCAP_ACTION_SET_NULL().
*/
/**@ingroup txcap_action_group
* @def txcap_action_fetch_element
* Retrieves an element from the XDMS by sending a <i>HTTP/HTTPS GET</i> request.
* The default Content-Type will be "application/xcap-el+xml", unless you provide your own Content-Type by using @ref TXCAP_ACTION_SET_HEADER().
* @param stack The HTTP/HTTPS stack created using @ref txcap_stack_create().
* @param ... Any @a TXCAP_ACTION_SET_*() macros. MUST ends with @ref TXCAP_ACTION_SET_NULL().
*/
/**@ingroup txcap_action_group
* @def txcap_action_fetch_document
* Retrieves a document from the XDMS sending a <i>HTTP/HTTPS GET</i> request.
* The default Content-Type will be the one associated with the AUID of the document, unless you provide your own Content-Type by using @ref TXCAP_ACTION_SET_HEADER().
* @param stack The HTTP/HTTPS stack created using @ref txcap_stack_create().
* @param ... Any @a TXCAP_ACTION_SET_*() macros. MUST ends with @ref TXCAP_ACTION_SET_NULL().
*/
/**@ingroup txcap_action_group
* @def txcap_action_fetch_attribute
* Retrieves an attribute from the XDMS by sending a <i>HTTP/HTTPS GET</i> request.
* The default Content-Type will be "application/xcap-att+xml", unless you provide your own Content-Type by using @ref TXCAP_ACTION_SET_HEADER().
* @param stack The HTTP/HTTPS stack created using @ref txcap_stack_create().
* @param ... Any @a TXCAP_ACTION_SET_*() macros. MUST ends with @ref TXCAP_ACTION_SET_NULL().
*/
/**@ingroup txcap_action_group
* @def txcap_action_delete_element
* Deletes an element from the XDMS by sending a <i>HTTP/HTTPS DELETE</i> request.
* @param stack The HTTP/HTTPS stack created using @ref txcap_stack_create().
* @param ... Any @a TXCAP_ACTION_SET_*() macros. MUST ends with @ref TXCAP_ACTION_SET_NULL().
*/
/**@ingroup txcap_action_group
* @def txcap_action_delete_document
* Deletes a document from the XDMS sending a <i>HTTP/HTTPS DELETE</i> request.
* @param stack The HTTP/HTTPS stack created using @ref txcap_stack_create().
* @param ... Any @a TXCAP_ACTION_SET_*() macros. MUST ends with @ref TXCAP_ACTION_SET_NULL().
*/
/**@ingroup txcap_action_group
* @def txcap_action_delete_attribute
* Deletes an attribute from the XDMS by sending a <i>HTTP/HTTPS DELETE</i> request.
* @param stack The HTTP/HTTPS stack created using @ref txcap_stack_create().
* @param ... Any @a TXCAP_ACTION_SET_*() macros. MUST ends with @ref TXCAP_ACTION_SET_NULL().
*/
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
