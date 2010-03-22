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
/**@file tsdp.c
 * @brief SDP (RFC 4566) implementations with both MMTel and PoC extensions.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tsdp.h"

#include "tinySDP/headers/tsdp_header_O.h"
#include "tinySDP/headers/tsdp_header_S.h"
#include "tinySDP/headers/tsdp_header_T.h"
#include "tinySDP/headers/tsdp_header_V.h"

tsdp_message_t* tsdp_create_empty(const char* addr, tsk_bool_t ipv6)
{
	tsdp_message_t* ret = 0;

	if(!(ret = TSDP_MESSAGE_CREATE())){
		return tsk_null;
	}

	/*	RFC 3264 - 5 Generating the Initial Offer
		The numeric value of the session id and version in the o line MUST be 
		representable with a 64 bit signed integer.  The initial value of the version MUST be less than
	   (2**62)-1, to avoid rollovers.
	*/
	TSDP_MESSAGE_ADD_HEADER(ret, TSDP_HEADER_V_VA_ARGS(0));
	TSDP_MESSAGE_ADD_HEADER(ret, TSDP_HEADER_O_VA_ARGS(
		TSDP_LINE_O_USERNAME_DEFAULT,
		TSDP_LINE_O_SESSION_ID_DEFAULT,
		TSDP_LINE_O_SESSION_VER_DEFAULT,
		"IN",
		ipv6 ? "IP6" : "IP4",
		addr));

	/*	RFC 3264 - 5 Generating the Initial Offer
		The SDP "s=" line conveys the subject of the session, which is
		reasonably defined for multicast, but ill defined for unicast.  For
		unicast sessions, it is RECOMMENDED that it consist of a single space
		character (0x20) or a dash (-).

		Unfortunately, SDP does not allow the "s=" line to be empty.
	*/
	TSDP_MESSAGE_ADD_HEADER(ret, TSDP_HEADER_S_VA_ARGS(TSDP_LINE_S_VALUE_DEFAULT));

	/*	RFC 3264 - 5 Generating the Initial Offer
		The SDP "t=" line conveys the time of the session.  Generally,
		streams for unicast sessions are created and destroyed through
		external signaling means, such as SIP.  In that case, the "t=" line
		SHOULD have a value of "0 0".
	*/
	TSDP_MESSAGE_ADD_HEADER(ret, TSDP_HEADER_T_VA_ARGS(0, 0));
	
	return ret;
}

















//=================================================================================================
//	Sdp ctx object definition
//
static void* tsdp_ctx_create(void * self, va_list * app)
{
	tsdp_ctx_t *ctx = self;
	if(ctx){
	}
	return self;
}

static void* tsdp_ctx_destroy(void * self)
{ 
	tsdp_ctx_t *ctx = self;
	if(ctx){
		TSK_OBJECT_SAFE_FREE(ctx->caps);
	}
	return self;
}

static const tsk_object_def_t tsdp_ctx_def_s = 
{
	sizeof(tsdp_ctx_t),
	tsdp_ctx_create,
	tsdp_ctx_destroy,
	tsk_null,
};
const void *tsdp_ctx_def_t = &tsdp_ctx_def_s;
