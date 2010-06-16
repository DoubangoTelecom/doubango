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

#include "tinysip_config.h"

#include "tinymedia/tmedia_qos.h"

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


#define TSIP_SSESSION(self) ((tsip_ssession_t*)(self))

typedef enum tsip_ssession_param_type_e
{
	sstype_null = 0,
	
	sstype_header,
	sstype_caps,
	sstype_userdata,
	sstype_to,
	sstype_from,
	sstype_nocontact,
	sstype_expires,
	sstype_silent_hangup,
	sstype_sigcomp_id,
	sstype_media
}
tsip_ssession_param_type_t;

#define TSIP_SSESSION_SET_HEADER(NAME_STR, VALUE_STR)				sstype_header, (const char*)NAME_STR, (const char*)VALUE_STR
#define TSIP_SSESSION_UNSET_HEADER(NAME_STR)						TSIP_SSESSION_SET_HEADER(NAME_STR, (const char*)-1)
#define TSIP_SSESSION_SET_CAPS(NAME_STR, VALUE_STR)					sstype_caps, (const char*)NAME_STR, (const char*)VALUE_STR /* RFC 3840 */
#define TSIP_SSESSION_UNSET_CAPS(NAME_STR)							TSIP_SSESSION_SET_CAPS(NAME_STR, (const char*)-1)
#define TSIP_SSESSION_SET_USERDATA(DATA_PTR)						sstype_userdata, (const void*)DATA_PTR
#define TSIP_SSESSION_SET_TO(TO_URI_STR)							sstype_to, (const char*)TO_URI_STR
#define TSIP_SSESSION_SET_FROM(FROM_URI_STR)						sstype_from, (const char*)FROM_URI_STR
#define TSIP_SSESSION_SET_NO_CONTACT(ENABLED_BOOL)					sstype_nocontact, (tsk_bool_t)ENABLED_BOOL
#define TSIP_SSESSION_SET_EXPIRES(VALUE_UINT)						sstype_expires, (unsigned)VALUE_UINT
#define TSIP_SSESSION_SET_SILENT_HANGUP(ENABLED_BOOL)				sstype_silent_hangup, (tsk_bool_t)ENABLED_BOOL
#define TSIP_SSESSION_SET_SIGCOMP_COMPARTMENT(COMPARTMENT_ID_STR)	sstype_sigcomp_id, (const char*)COMPARTMENT_ID_STR
#define TSIP_SSESSION_SET_MEDIA(...)								sstype_media, ##__VA_ARGS__
#define TSIP_SSESSION_SET_NULL()									sstype_null

typedef enum tsip_msession_param_type_e
{
	mstype_null = 0,
	
	mstype_set_100rel,
	mstype_unset_100rel,

	mstype_set_qos,
	mstype_unset_qos,

	mstype_set_timers,
	mstype_unset_timers,
}
tsip_msession_param_type_t;

#define TSIP_MSESSION_SET_100rel()								mstype_set_100rel
#define TSIP_MSESSION_UNSET_100rel()							mstype_unset_100rel
#define TSIP_MSESSION_SET_QOS(TYPE_ENUM, STRENGTH_ENUM)			mstype_set_qos, (tmedia_qos_stype_t)TYPE_ENUM, (tmedia_qos_strength_t)STRENGTH_ENUM
#define TSIP_MSESSION_UNSET_QOS()								mstype_unset_qos
#define TSIP_MSESSION_SET_TIMERS(TIMEOUT_UINT, REFRESHER_STR)	mstype_set_timers, (unsigned)TIMEOUT_UINT, (const char*)REFRESHER_STR
#define TSIP_MSESSION_UNSET_TIMERS()							mstype_unset_timers

#define TSIP_MSESSION_SET_NULL()			mstype_null

typedef struct tsip_ssession_s
{
	TSK_DECLARE_OBJECT;
	
	tsip_ssession_id_t id;
	unsigned owner:1;
	
	const struct tsip_stack_s* stack;
	const void* userdata;

	//=======
	// SIP
	//=======
	tsk_params_L_t *caps;
	tsk_params_L_t *headers;
	
	unsigned no_contact:1;
	struct tsip_uri_s* from;
	struct tsip_uri_s* to;
	int64_t expires;
	tsk_bool_t silent_hangup;
	char* sigcomp_id;

	//=======
	// Media
	//=======
	struct{
		/* Session timers */
		struct{
			char* refresher;
			unsigned timeout;
		} timers;
		/* QoS */
		struct{
			tmedia_qos_stype_t type;
			tmedia_qos_strength_t strength;
		} qos;

		/* Features */
		unsigned enable_100rel:1;
	} media;
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
TINYSIP_API int tsip_ssession_respond(const tsip_ssession_handle_t *self, short status, const char* phrase, const void* payload, tsk_size_t size, const struct tsip_message_s* request, ...);
TINYSIP_API const void* tsip_ssession_get_userdata(const tsip_ssession_handle_t *self);
TINYSIP_API const tsip_stack_handle_t* tsip_ssession_get_stack(const tsip_ssession_handle_t *self);

int tsip_ssession_handle(const tsip_ssession_t *self, const struct tsip_action_s* action);

typedef tsk_list_t tsip_ssessions_L_t; /**< List of @ref tsip_ssession_handle_t elements. */
TINYSIP_GEXTERN const tsk_object_def_t *tsip_ssession_def_t;

TSIP_END_DECLS

#endif /* TSIP_SSESSION_H */

