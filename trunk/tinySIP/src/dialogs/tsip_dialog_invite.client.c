/*
* Copyright (C) 2009 Mamadou Diop.
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

/**@file tsip_dialog_invite.client.c
 * @brief SIP dialog INVITE as per RFC 3261.
 * The SOA machine is designed as per RFC 3264 and draft-ietf-sipping-sip-offeranswer-12.
 * MMTel services implementation follow 3GPP TS 24.173.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/dialogs/tsip_dialog_invite.h"

#include "tinysip/dialogs/tsip_dialog_invite.common.h"

#include "tsk_debug.h"

extern int send_INVITE(tsip_dialog_invite_t *self);
extern int send_ACK(tsip_dialog_invite_t *self, const tsip_response_t* r2xxINVITE);

/* Started -> (oINVITE) -> Outgoing
*/
int c0000_Started_2_Outgoing_X_oINVITE(va_list *app)
{
	int ret;
	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
	//const tsip_response_t *response = va_arg(*app, const tsip_response_t *);
	
	/* This is the first transaction when you try to make an audio/video/msrp call */
	if(self->msession_mgr == tsk_null){
		self->msession_mgr = tmedia_session_mgr_create((tmedia_audio | tmedia_video | tmedia_msrp | tmedia_t38),
		"192.168.0.12", tsk_false);
	}
	
	/* send the request */
	ret = send_INVITE(self);

	/* alert the user */
	TSIP_DIALOG_SIGNAL(self, tsip_event_code_dialog_connecting, "Dialog connecting");


	return ret;
}

/* Outgoing -> (i2xx INVITE) -> Connected
*/
int c0001_Outgoing_2_Connected_X_i2xxINVITE(va_list *app)
{
	int ret;
	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
	const tsip_response_t *r2xxINVITE = va_arg(*app, const tsip_response_t *);

	/* Update the dialog state */
	if((ret = tsip_dialog_update(TSIP_DIALOG(self), r2xxINVITE))){
		return ret;
	}

	/* alert the user */
	TSIP_DIALOG_INVITE_SIGNAL(self, tsip_ao_invite, 
		TSIP_RESPONSE_CODE(r2xxINVITE), TSIP_RESPONSE_PHRASE(r2xxINVITE), r2xxINVITE);

	/* send ACK */
	return send_ACK(self, r2xxINVITE);
}

/* Outgoing -> (i300-i699 INVITE) -> Terminated
*/
int c0002_Outgoing_2_Terminated_X_i300_to_i699INVITE(va_list *app)
{
	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	/* alert the user */
	TSIP_DIALOG_INVITE_SIGNAL(self, tsip_ao_invite, 
		TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), response);

	return 0;
}

/* Outgoing -> (oCANCEL ) -> Terminated
*/
int c0003_Outgoing_2_Terminated_X_oCANCEL(va_list *app)
{
	/* alert the user */
	return 0;
}
