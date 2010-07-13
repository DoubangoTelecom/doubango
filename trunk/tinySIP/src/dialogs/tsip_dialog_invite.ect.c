/*
* Copyright (C) 2009-2010 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
*	
* This file is part of Open Source Doubango Framework.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as publishd by
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

/**@file tsip_dialog_invite.hold.c
 * @brief Explicit Communication Transfer (ECT) using IP Multimedia (IM) Core Network (CN) subsystem (3GPP TS 24.629)
 * The Explicit Communication transfer (ECT) service provides a party involved in a communication to transfer that
 * communication to a third party.
 * Only Consultative transfer is supportted (A2). Blind transfer (A1) will be added later.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/dialogs/tsip_dialog_invite.h"

#include "tinysip/dialogs/tsip_dialog_invite.common.h"

#include "tinysip/headers/tsip_header_Refer_To.h"
#include "tinysip/headers/tsip_header_Supported.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"

/* ======================== transitions ======================== */
static int x0400_Connected_2_oECTing_X_oREFER(va_list *app);
static int x0401_oECTing_2_oECTed_X_i2xx(va_list *app); // should be 202?
static int x0402_oECTing_2_Connected_X_i300_to_699(va_list *app);
static int x0401_oECTed_2_oECTed_X_iNOTIFY(va_list *app); // Notify 1xx
static int x0401_oECTed_2_Connected_X_iNOTIFY(va_list *app); // Notify 200_to_699

/* ======================== conds ======================== */
static tsk_bool_t _fsm_cond_is_resp2REFER(tsip_dialog_invite_t* self, tsip_message_t* message)
{
	return TSIP_RESPONSE_IS_TO_REFER(message);
}


int tsip_dialog_invite_ect_init(tsip_dialog_invite_t *self)
{
	tsk_fsm_set(TSIP_DIALOG_GET_FSM(self),

		/*=======================
		* === Outgoing Transfer === 
		*/
		// Connected -> (send REFER) -> ECTing
		// ECTing -> (i2xx) -> ECTed
		// ECTing -> (i300-699) -> Connected
		// ECTed -> (iNotify 1xx) -> ECTed
		// ECTed -> (iNotify non-1xx) -> Connected
		

		/*=======================
		* === Incoming Transfer === 
		*/
		
		TSK_FSM_ADD_NULL());

	return 0;
}













int send_REFER(tsip_dialog_invite_t *self, const char* to)
{
	int ret = 0;
	tsip_request_t *refer = tsk_null;
	tsip_uri_t* toUri = tsk_null;

	if(!self || !to){
		TSK_DEBUG_ERROR("Invalid parameter");
		goto bail;
	}

	if(!(toUri = tsip_uri_parse(to, tsk_strlen(to)))){
		TSK_DEBUG_ERROR("Failed to parse %s", to);
		goto bail;
	}
	else{
		tsk_params_add_param(&toUri->params, "method", "INVITE");
	}

	if((refer = tsip_dialog_request_new(TSIP_DIALOG(self), "REFER"))){
		/* Add headers */
		tsip_message_add_headers(refer,
			TSIP_HEADER_REFER_TO_VA_ARGS(toUri),
			TSIP_HEADER_SUPPORTED_VA_ARGS("norefersub"),
			tsk_null);

		ret = tsip_dialog_request_send(TSIP_DIALOG(self), refer);
		TSK_OBJECT_SAFE_FREE(refer);
	}

bail:
	TSK_OBJECT_SAFE_FREE(toUri);
	return ret;
}
