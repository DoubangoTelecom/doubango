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

/**@file tsip_ssession.h
 * @brief SIP ssession.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TSIP_SSESSION_H
#define TSIP_SSESSION_H

#include "tinySIP_config.h"

#include "tsk_object.h"
#include "tsk_list.h"
#include "tsk_params.h"
#include "tsk_options.h"

TSIP_BEGIN_DECLS

// Forward declarations
struct tsip_message_s;
struct tsip_action_s;

typedef uint64_t tsip_ssession_id_t;			
#define TSIP_SSESSION_INVALID_ID				0
#define TSIP_SSESSION_INVALID_HANDLE			tsk_null

#if defined(DEBUG) || defined(_DEBUG)
#	define TSIP_SSESSION_EXPIRES_DEFAULT		3600000 /* miliseconds. */
#else
#	define TSIP_SSESSION_EXPIRES_DEFAULT		600000000
#endif

typedef enum tsip_ssession_option_e
{
	TSIP_SSESSION_OPTION_EXPIRES,
	TSIP_SSESSION_OPTION_NO_CONTACT,

	//! Destination URI. Will be used to set "To" header.
	TSIP_SSESSION_OPTION_TO,
	//! Source URI. Will be used to set "From" header.
	TSIP_SSESSION_OPTION_FROM,
}
tsip_ssession_option_t;

typedef enum tsip_ssession_param_type_e
{
	sstype_null = tsk_null,

	sstype_option,
	sstype_header,
	sstype_caps,
	sstype_context,
}
tsip_ssession_param_type_t;

#define TSIP_SSESSION_SET_OPTION(ID_ENUM, VALUE_STR)			sstype_option, (tsip_ssession_option_t)ID_ENUM, (const char*)VALUE_STR
// #define TSIP_SSESSION_SET_EXPIRES(EXPIRES_UINT)					TSIP_SSESSION_SET_OPTION(TSIP_SSESSION_OPTION_EXPIRES, #EXPIRES_UINT)

#define TSIP_SSESSION_SET_HEADER(NAME_STR, VALUE_STR)			sstype_header, (const char*)NAME_STR, (const char*)VALUE_STR
#define TSIP_SSESSION_UNSET_HEADER(NAME_STR)					TSIP_SSESSION_SET_HEADER(NAME_STR, (const char*)-1)
#define TSIP_SSESSION_SET_CAPS(NAME_STR, VALUE_STR)				sstype_caps, (const char*)NAME_STR, (const char*)VALUE_STR /* RFC 3840 */
#define TSIP_SSESSION_UNSET_CAPS(NAME_STR)						TSIP_SSESSION_SET_CAPS(NAME_STR, (const char*)-1)
#define TSIP_SSESSION_SET_CONTEXT(CTX_PTR)						sstype_context, (const void*)CTX_PTR
#define TSIP_SSESSION_SET_NULL()								sstype_null

typedef struct tsip_ssession_s
{
	TSK_DECLARE_OBJECT;

	tsip_ssession_id_t id;
	unsigned owner:1;

	const struct tsip_stack_s* stack;
	const void* context; // usr context
	tsk_options_L_t *options;
	tsk_params_L_t *caps;
	tsk_params_L_t *headers;

	unsigned no_contact:1;
	struct tsip_uri_s* from;
	struct tsip_uri_s* to;
	int64_t expires;
}
tsip_ssession_t;

/** A pointer to a SIP Session */
typedef void tsip_ssession_handle_t;
/** A pointer to a LTE/IMS stack */
typedef void tsip_stack_handle_t;

TINYSIP_API tsip_ssession_handle_t* tsip_ssession_create(tsip_stack_handle_t *stack, ...);
TINYSIP_API int tsip_ssession_set(tsip_ssession_handle_t *self, ...);
TINYSIP_API tsip_ssession_id_t tsip_ssession_get_id(const tsip_ssession_handle_t *self);
TINYSIP_API int tsip_ssession_take_ownership(tsip_ssession_handle_t *self);
TINYSIP_API tsk_bool_t tsip_ssession_have_ownership(const tsip_ssession_handle_t *self);
TINYSIP_API int tsip_ssession_respond(const tsip_ssession_handle_t *self, short status, const char* phrase, const void* payload, size_t size, const struct tsip_message_s* request, ...);
TINYSIP_API const void* tsip_ssession_get_context(const tsip_ssession_handle_t *self);

//tsip_ssession_handle_t *tsip_ssession_create_2(const struct tsip_stack_s* stack, const struct tsip_message_s* message);

int tsip_ssession_hangup(const tsip_ssession_t *self, const struct tsip_action_s* action);

typedef tsk_list_t tsip_ssessions_L_t; /**< List of @ref tsip_ssession_handle_t elements. */
TINYSIP_GEXTERN const tsk_object_def_t *tsip_ssession_def_t;

TSIP_END_DECLS

#endif /* TSIP_SSESSION_H */

