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

/**@file tsip_dialog_invite.hold.c
 * @brief Communication Hold (3GPP TS 24.610)
 * The Communication Hold supplementary service enables a user to suspend the reception of media stream(s) of an established IP multimedia session, 
 * and resume the media stream(s) at a later time.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/dialogs/tsip_dialog_invite.h"

#include "tinysip/dialogs/tsip_dialog_invite.common.h"

#include "tsk_debug.h"

/* ======================== transitions ======================== */
static int x0100_Connected_2_Connected_X_oHold(va_list *app);
static int x0101_Connected_2_Connected_X_oResume(va_list *app);

/* ======================== external functions ======================== */
extern int send_INVITEorUPDATE(tsip_dialog_invite_t *self, tsk_bool_t is_INVITE);

int tsip_dialog_invite_hold_init(tsip_dialog_invite_t *self)
{
	tsk_fsm_set(TSIP_DIALOG_GET_FSM(self),

		// Connected -> (send HOLD) -> Connected
		TSK_FSM_ADD_ALWAYS(_fsm_state_Connected, _fsm_action_oHold, _fsm_state_Connected, x0100_Connected_2_Connected_X_oHold, "x0100_Connected_2_Connected_X_oHold"),
		// Connected -> (send RESUME) -> Connected
		TSK_FSM_ADD_ALWAYS(_fsm_state_Connected, _fsm_action_oResume, _fsm_state_Connected, x0101_Connected_2_Connected_X_oResume, "x0101_Connected_2_Connected_X_oResume"),
		
		TSK_FSM_ADD_NULL());

	return 0;
}

// Connected -> (send HOLD) -> Connected
int x0100_Connected_2_Connected_X_oHold(va_list *app)
{
	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
	int ret = 0;

	if(!self->msession_mgr){
		TSK_DEBUG_WARN("Media Session manager is Null");
		return 0;
	}

	/* put on hold */
	/* FIXME: only put on hold the specified media */
	ret = tmedia_session_mgr_hold(self->msession_mgr, (tmedia_audio | tmedia_video | tmedia_msrp | tmedia_t38));
	/* send the request */
	ret = send_INVITE(self);

	return ret;
}

int x0101_Connected_2_Connected_X_oResume(va_list *app)
{
	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);

	int ret = 0;

	if(!self->msession_mgr){
		TSK_DEBUG_WARN("Media Session manager is Null");
		return 0;
	}

	/* Resume */
	/* FIXME: only put on hold the specified media */
	ret = tmedia_session_mgr_resume(self->msession_mgr, (tmedia_audio | tmedia_video | tmedia_msrp | tmedia_t38));
	/* send the request */
	ret = send_INVITE(self);

	return ret;
}