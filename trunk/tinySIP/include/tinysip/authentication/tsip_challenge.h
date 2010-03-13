/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_AUTHENTICATION_CHALLENGE_H
#define TINYSIP_AUTHENTICATION_CHALLENGE_H

#include "tinySIP_config.h"

#include "tsip.h"

#include "tinySIP/tsip_message.h"
#include "tinySIP/headers/tsip_header.h"

#include "tinySIP/authentication/tsip_milenage.h"

#include "tinyhttp/auth/thttp_auth.h"

#include "tsk_object.h"
#include "tsk_list.h"
#include "tsk_md5.h"

TSIP_BEGIN_DECLS

#define TSIP_CHALLENGE_CREATE(stack, isproxy, scheme, realm, nonce, opaque, algorithm, qop)		tsk_object_new(tsip_challenge_def_t, stack, (unsigned)isproxy,(const char*)scheme, (const char*)realm, (const char*)nonce, (const char*)opaque, (const char*)algorithm, (const char*)qop)

#define TSIP_CHALLENGE_CREATE_NULL(stack)														TSIP_CHALLENGE_CREATE(stack, 0,0,0,0,0,0,0)

typedef struct tsip_challenge_s
{
	TSK_DECLARE_OBJECT;

	const tsip_stack_handle_t *stack;

	unsigned isproxy:1;

	char* scheme;
	char* realm;
	char* nonce;
	char* opaque;
	char* algorithm;
	const char* qop;

	AKA_CK_T ck;
	AKA_IK_T ik;

	tsk_md5string_t cnonce;
	unsigned nc;
}
tsip_challenge_t;

typedef tsk_list_t tsip_challenges_L_t;

int tsip_challenge_update(tsip_challenge_t *self, const char* scheme, const char* realm, const char* nonce, const char* opaque, const char* algorithm, const char* qop);
tsip_header_t *tsip_challenge_create_header_authorization(tsip_challenge_t *self, const tsip_request_t *request);
tsip_header_t *tsip_challenge_create_empty_header_authorization(const char* username, const char* realm, const char* uristring);

TINYSIP_GEXTERN const void *tsip_challenge_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_AUTHENTICATION_CHALLENGE_H */

