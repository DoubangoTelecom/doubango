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
/**@file txcap.h
 * @brief RFC 4825 (XCAP) implementation.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYXCAP_TXCAP_H
#define TINYXCAP_TXCAP_H

#include "tinyXCAP_config.h"

#include "tinyXCAP/txcap_auid.h"

#include "tsk_options.h"

#include "tinyhttp.h"

TXCAP_BEGIN_DECLS

typedef enum txcap_stack_option_e
{
	TXCAP_STACK_OPTION_TIMEOUT,
	TXCAP_STACK_OPTION_TTL,
	
	TXCAP_STACK_OPTION_PASSWORD,
	TXCAP_STACK_OPTION_XUI,
}
txcap_stack_option_t;

typedef enum txcap_stack_param_type_e
{
	xcapp_null = tsk_null,

	xcapp_option,
	xcapp_header,
	xcapp_context,
	xcapp_auid,
}
txcap_stack_param_type_t;


#define TXCAP_STACK_SET_OPTION(ID_INT, VALUE_STR)											xcapp_option,  (txcap_stack_option_t)ID_INT, (const char*)VALUE_STR
#define TXCAP_STACK_SET_PASSWORD(PASSWORD_STR)												TXCAP_STACK_SET_OPTION(TXCAP_STACK_OPTION_PASSWORD, PASSWORD_STR)
#define TXCAP_STACK_SET_XUI(XUI_STR)														TXCAP_STACK_SET_OPTION(TXCAP_STACK_OPTION_XUI, XUI_STR)
#define TXCAP_STACK_SET_HEADER(NAME_STR, VALUE_STR)											xcapp_header, (const char*)NAME_STR, (const char*)VALUE_STR
#define TXCAP_STACK_SET_CONTEXT(CTX_PTR)													xcapp_context, (const void*)CTX_PTR
#define TXCAP_STACK_SET_AUID(ID_STR, MIME_TYPE_STR, NS_STR, DOC_NAME_STR, IS_GLOBAL_BOOL)	xcapp_auid, (const char*)ID_STR, (const char*)MIME_TYPE_STR, (const char*)NS_STR, (const char*)DOC_NAME_STR, (tsk_bool_t)IS_GLOBAL_BOOL
#define TXCAP_STACK_SET_SELECTOR(...)

#define TXCAP_STACK_SET_NULL()							xcapp_null


typedef struct txcap_stack_s
{
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
typedef void txcap_stack_handle_t;

TINYXCAP_API txcap_stack_handle_t* txcap_stack_create(thttp_stack_callback callback, const char* xui, const char* password, const char* xcap_root, ...);
TINYXCAP_API int txcap_stack_start(txcap_stack_handle_t* self);
TINYXCAP_API int txcap_stack_set(txcap_stack_handle_t* self, ...);
TINYXCAP_API int txcap_stack_perform(txcap_stack_handle_t* self, const char* urlstring, ...);
TINYXCAP_API int txcap_stack_stop(txcap_stack_handle_t* self);

TINYXCAP_GEXTERN const tsk_object_def_t *txcap_stack_def_t;

TXCAP_END_DECLS

#endif // TINYXCAP_CONFIG_H
