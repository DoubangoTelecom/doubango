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

/**@file tsip_action.h
 * @brief SIP action.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TSIP_ACTION_H
#define TSIP_ACTION_H

#include "tinySIP_config.h"

#include "tsk_buffer.h"
#include "tsk_options.h"
#include "tsk_params.h"

TSIP_BEGIN_DECLS

typedef uint64_t tsip_action_id_t;			
#define TSIP_ACTION_INVALID_ID				0
#define TSIP_ACTION_INVALID_HANDLE			tsk_null

typedef enum tsip_action_option_e
{
	TSIP_ACTION_OPTION_DUMMY, /* at least one */
}
tsip_action_option_t;

typedef enum tsip_action_type_e
{
	/* REGISTER */
	atype_register,
#define atype_unregister atype_hangup
	
	/* SUBSCRIBE */
	atype_subscribe,
#define atype_unsubscribe atype_hangup

	/* MESSAGE */
	atype_message_send,

	/* PUBLISH */
	atype_publish,
#define atype_unpublish atype_hangup

	/* common */
	atype_accept,
	atype_reject,
	atype_cancel,
	atype_hangup,

}
tsip_action_type_t;

typedef enum tsip_action_param_type_e
{
	aptype_null = tsk_null,

	aptype_option,
	aptype_header,
	aptype_payload,
}
tsip_action_param_type_t;

#define TSIP_ACTION_SET_OPTION(ID_ENUM, VALUE_STR)			aptype_option, (tsip_action_option_t)ID_ENUM, (const char*)VALUE_STR
#define TSIP_ACTION_SET_HEADER(NAME_STR, VALUE_STR)			aptype_header, (const char*)NAME_STR, (const char*)VALUE_STR
#define TSIP_ACTION_SET_PAYLOAD(PAY_PTR, PAY_SIZE)			aptype_payload, (const void*)PAY_PTR, (size_t)PAY_SIZE
#define TSIP_ACTION_SET_NULL()								aptype_null

typedef struct tsip_action_s
{
	TSK_DECLARE_OBJECT;
	
	tsip_action_type_t type;
	tsk_options_L_t *options;
	tsk_params_L_t *headers;
	tsk_buffer_t* payload;
}
tsip_action_t;

typedef void tsip_action_handle_t;

tsip_action_t* tsip_action_create(tsip_action_type_t type, va_list* app);

typedef tsk_list_t tsip_actions_L_t; /**< List of @ref tsip_action_handle_t elements. */
TINYSIP_GEXTERN const tsk_object_def_t *tsip_action_def_t;

TSIP_END_DECLS

#endif /* TSIP_ACTION_H */

