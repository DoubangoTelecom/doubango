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
/**@file txcap.h
 * @brief RFC 4825 (XCAP) implementation.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#ifndef TINYXCAP_TXCAP_H
#define TINYXCAP_TXCAP_H

#include "tinyxcap_config.h"

#include "tinyxcap/txcap_auid.h"

#include "tsk_options.h"

#include "tinyhttp.h"

TXCAP_BEGIN_DECLS

typedef enum txcap_stack_option_e {
    //! request timeout
    TXCAP_STACK_OPTION_TIMEOUT,
    //! time to live
    TXCAP_STACK_OPTION_TTL,

    //! xcap-root
    TXCAP_STACK_OPTION_ROOT,
    //! user's password
    TXCAP_STACK_OPTION_PASSWORD,
    //! xcap user indentifier (e.g. sip:bob@example.com)
    TXCAP_STACK_OPTION_XUI,
    //! local ip address
    TXCAP_STACK_OPTION_LOCAL_IP,
    //! local port
    TXCAP_STACK_OPTION_LOCAL_PORT,
}
txcap_stack_option_t;

typedef enum txcap_stack_param_type_e {
    xcapp_null = 0,

    xcapp_option,
    xcapp_header,
    xcapp_userdata,
    xcapp_auid,
}
txcap_stack_param_type_t;

/* For systems that support SIP applications, it is
   RECOMMENDED that the XUI be equal to the Address-of-Record (AOR) for
   the user (i.e., sip:joe@example.com) */

/**@ingroup txcap_stack_group
* @def TXCAP_STACK_SET_OPTION
* Adds or updates an option.
* This is a helper macro for @ref txcap_stack_create() and @ref txcap_stack_set().
* @param ID_ENUM The id of the option to add/update (@ref txcap_stack_option_t).
* @param VALUE_STR The new value of the option (<i>const char*</i>).
*
* @code
int ret = txcap_stack_set(stack,
		// stack-level options
        TXCAP_STACK_SET_OPTION(TXCAP_STACK_OPTION_TIMEOUT, "6000"),
        TXCAP_STACK_SET_NULL());
* @endcode
*/
/**@ingroup txcap_stack_group
* @def TXCAP_STACK_SET_PASSWORD
* Sets or updates the user's password.
* @param PASSWORD_STR The new password (<i>const char*</i>).
*
* @code
int ret = txcap_stack_set(stack,
        TXCAP_STACK_SET_PASSWORD("mysecret"),
        TXCAP_STACK_SET_NULL());
* @endcode
*
* @sa @ref TXCAP_STACK_OPTION_PASSWORD
*/
/**@ingroup txcap_stack_group
* @def TXCAP_STACK_SET_ROOT
* Sets or updates the xcap-root Url.
* @param XCAP_ROOT_STR A valid Http Url(<i>const char*</i>).
*
* @code
int ret = txcap_stack_set(stack,
        TXCAP_STACK_SET_ROOT("http://192.168.0.10:8080/services"),
        TXCAP_STACK_SET_NULL());
* @endcode
*
* @sa @ref TXCAP_STACK_OPTION_ROOT
*/
/**@ingroup txcap_stack_group
* @def TXCAP_STACK_SET_XUI
* Sets or updates the user's identifier.
* @param XUI_STR The new identifier (<i>const char*</i>).For systems that support SIP applications, it is
* RECOMMENDED that the XUI be equal to the Address-of-Record (AOR) for the user (i.e., sip:bob@example.com).
*
* @code
int ret = txcap_stack_set(stack,
        TXCAP_STACK_SET_XUI("sip:bob@example.com"),
        TXCAP_STACK_SET_NULL());
* @endcode
*
* @sa @ref TXCAP_STACK_OPTION_PASSWORD
*/
/**@ingroup txcap_stack_group
* @def TXCAP_STACK_SET_LOCAL_IP
*/
/**@ingroup txcap_stack_group
* @def TXCAP_STACK_SET_LOCAL_PORT
*/
/**@ingroup txcap_stack_group
* @def TXCAP_STACK_SET_HEADER
* Adds new stack-level HTTP header. This header will be added to all outgoing requests.
* If you want that the header only appear in the current outgoing request, then you should use
* @ref TXCAP_ACTION_SET_HEADER().
* @param NAME_STR The name of the header.
* @param VALUE_STR The value of the header. Will be added "as is".
*
* @code
int ret = txcap_stack_set(stack,
        TXCAP_STACK_SET_HEADER("User-Agent", "XDM-client/OMA1.1"),
        TXCAP_STACK_SET_NULL());
* @endcode
*
* @sa @ref TXCAP_ACTION_SET_HEADER(), @ref TXCAP_STACK_UNSET_HEADER()
*/
/**@ingroup txcap_stack_group
* @def TXCAP_STACK_UNSET_HEADER
* Removes a stack-level HTTP header.
* @param NAME_STR The name of the header.
*
* @code
int ret = txcap_stack_set(stack,
        TXCAP_STACK_UNSET_HEADER("User-Agent"),
        TXCAP_STACK_SET_NULL());
* @endcode
*
* @sa @ref TXCAP_ACTION_UNSET_HEADER(), @ref TXCAP_ACTION_SET_HEADER()
*/
/**@ingroup txcap_stack_group
* @def TXCAP_STACK_SET_CONTEXT
* Sets or updates the user's context. The context will be returned to the application layer
* throught the callback function.
* @param CTX_PTR A pointer to the context (<i>const void*</i>).
*
* @code
const struct application_s context;
int ret = txcap_stack_set(stack,
        TXCAP_STACK_SET_CONTEXT(&context),
        TXCAP_STACK_SET_NULL());
* @endcode
*/
/**@ingroup txcap_stack_group
* @def TXCAP_STACK_SET_AUID
* Register or update an AUID.
* @param ID_STR The identifier associated to this aplication usage (e.g. "xcap-caps").
* @param MIME_TYPE_STR The mime-type (e.g. "application/xcap-caps+xml").
* @param NS_STR The namespace (e.g. "urn:ietf:params:xml:ns:xcap-caps").
* @param DOC_NAME_STR The document name (e.g. "index").
* @param IS_GLOBAL_BOOL The scope (@a tsk_true or @a tsk_false).
*
* @code
// the code below shows how to register two new AUIDs
int ret = txcap_stack_set(stack,
        TXCAP_STACK_SET_AUID("my-xcap-caps", "application/my-xcap-caps+xml", "urn:ietf:params:xml:ns:my-xcap-caps", "my-document", tsk_true),
        TXCAP_STACK_SET_AUID("my-resource-lists", "application/my-resource-lists+xml", "urn:ietf:params:xml:ns:my-resource-lists", "my-document", tsk_false),
        TXCAP_STACK_SET_NULL());
* @endcode
*/
/**@ingroup txcap_stack_group
* @def TXCAP_STACK_SET_NULL
* Ends the stack parameters. Mandatory and should be the last one.
*/

#define TXCAP_STACK_SET_OPTION(ID_ENUM, VALUE_STR)											xcapp_option,  (txcap_stack_option_t)ID_ENUM, (const char*)VALUE_STR
#define TXCAP_STACK_SET_TIMEOUT(TIMEOUT_STR)												TXCAP_STACK_SET_OPTION(TXCAP_STACK_OPTION_TIMEOUT, TIMEOUT_STR)
#define TXCAP_STACK_SET_ROOT(XCAP_ROOT_STR)													TXCAP_STACK_SET_OPTION(TXCAP_STACK_OPTION_ROOT, XCAP_ROOT_STR)
#define TXCAP_STACK_SET_PASSWORD(PASSWORD_STR)												TXCAP_STACK_SET_OPTION(TXCAP_STACK_OPTION_PASSWORD, PASSWORD_STR)
#define TXCAP_STACK_SET_XUI(XUI_STR)														TXCAP_STACK_SET_OPTION(TXCAP_STACK_OPTION_XUI, XUI_STR)
#define TXCAP_STACK_SET_LOCAL_IP(IP_STR)													TXCAP_STACK_SET_OPTION(TXCAP_STACK_OPTION_LOCAL_IP, IP_STR)
#define TXCAP_STACK_SET_LOCAL_PORT(PORT_STR)												TXCAP_STACK_SET_OPTION(TXCAP_STACK_OPTION_LOCAL_PORT, PORT_STR)
#define TXCAP_STACK_SET_HEADER(NAME_STR, VALUE_STR)											xcapp_header, (const char*)NAME_STR, (const char*)VALUE_STR
#define TXCAP_STACK_UNSET_HEADER(NAME_STR)													TXCAP_STACK_SET_HEADER(NAME_STR, (const char*)-1)
#define TXCAP_STACK_SET_USERDATA(CTX_PTR)													xcapp_userdata, (const void*)CTX_PTR
#define TXCAP_STACK_SET_AUID(ID_STR, MIME_TYPE_STR, NS_STR, DOC_NAME_STR, IS_GLOBAL_BOOL)	xcapp_auid, (const char*)ID_STR, (const char*)MIME_TYPE_STR, (const char*)NS_STR, (const char*)DOC_NAME_STR, (tsk_bool_t)IS_GLOBAL_BOOL

#define TXCAP_STACK_SET_NULL()																xcapp_null


typedef struct txcap_stack_s {
    TSK_DECLARE_OBJECT;

    char* xui; /**< user's name as per RFC 4825 subclause 4.Also used to fill @b "X-3GPP-Intended-Identity" header.*/
    char* password; /**< user's password used to authenticate to the XDMS . */
    char* xcap_root; /**< xcap-root URI as per RFC 4825 subclause 6.1. MUST be a valid HTPP/HTTPS URL and will be used to build all request-uris. */

    thttp_session_handle_t* http_session;
    thttp_stack_handle_t* http_stack; /**< http/https stack */

    tsk_options_L_t *options; /**< list of user options */
    const void* context; /**< user's context */
    txcap_auids_L_t* auids; /**< user's auids */

    TSK_DECLARE_SAFEOBJ;
}
txcap_stack_t;
typedef void txcap_stack_handle_t;/**< Pointer to a XCAP stack. Should be created using @ref txcap_stack_create().*/

TINYXCAP_API txcap_stack_handle_t* txcap_stack_create(thttp_stack_callback_f callback, const char* xui, const char* password, const char* xcap_root, ...);
TINYXCAP_API int txcap_stack_start(txcap_stack_handle_t* self);
TINYXCAP_API int txcap_stack_set(txcap_stack_handle_t* self, ...);
TINYXCAP_API int txcap_stack_stop(txcap_stack_handle_t* self);

TINYXCAP_GEXTERN const tsk_object_def_t *txcap_stack_def_t;

TXCAP_END_DECLS

#endif // TINYXCAP_CONFIG_H
