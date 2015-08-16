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

/**@file tsip_challenge.h
 * @brief SIP authentication challenge.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSIP_AUTHENTICATION_CHALLENGE_H
#define TINYSIP_AUTHENTICATION_CHALLENGE_H

#include "tinysip_config.h"

#include "tsip.h"

#include "tinysip/tsip_message.h"
#include "tinysip/headers/tsip_header.h"

#include "tinysip/authentication/tsip_milenage.h"

#include "tinyhttp/auth/thttp_auth.h"

#include "tsk_object.h"
#include "tsk_list.h"
#include "tsk_md5.h"

TSIP_BEGIN_DECLS


typedef struct tsip_challenge_s
{
	TSK_DECLARE_OBJECT;

	const tsip_stack_handle_t *stack;

	tsk_bool_t isproxy;

	char* username;
	char* scheme;
	char* realm;
	char* nonce;
	char* opaque;
	char* algorithm;
	const char* qop;

	char* ha1_hexstr;

	AKA_CK_T ck;
	AKA_IK_T ik;

	tsk_md5string_t cnonce;
	unsigned nc;
}
tsip_challenge_t;

typedef tsk_list_t tsip_challenges_L_t;

TINYSIP_API tsip_challenge_t* tsip_challenge_create(tsip_stack_t* stack, tsk_bool_t isproxy, const char* scheme, const char* realm, const char* nonce, const char* opaque, const char* algorithm, const char* qop);
tsip_challenge_t* tsip_challenge_create_null(tsip_stack_t* stack);

int tsip_challenge_set_cred(tsip_challenge_t *self, const char* username, const char* ha1_hexstr);
int tsip_challenge_update(tsip_challenge_t *self, const char* scheme, const char* realm, const char* nonce, const char* opaque, const char* algorithm, const char* qop);
TINYSIP_API tsip_header_t *tsip_challenge_create_header_authorization(tsip_challenge_t *self, const tsip_request_t *request);
tsip_header_t *tsip_challenge_create_empty_header_authorization(const char* username, const char* realm, const char* uristring);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_challenge_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_AUTHENTICATION_CHALLENGE_H */

