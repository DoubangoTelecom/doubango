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

/**@file tsip_dialog_invite.common.h
 * @brief SIP dialog INVITE (common variables).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_DIALOG_INVITE_COMMON_H
#define TINYSIP_DIALOG_INVITE_COMMON_H

#include "tinysip/api/tsip_api_invite.h"

#define DEBUG_STATE_MACHINE										1
#define TSIP_DIALOG_INVITE_SIGNAL(self, type, code, phrase, message)	\
	tsip_invite_event_signal(type, TSIP_DIALOG(self)->ss, code, phrase, message)
#define TSIP_DIALOG_INVITE_TIMER_SCHEDULE(TX)						TSIP_DIALOG_TIMER_SCHEDULE(invite, TX)

/* ======================== actions ======================== */
typedef enum _fsm_action_e
{
	_fsm_action_oINVITE = tsip_atype_invite,
	_fsm_action_oCANCEL = tsip_atype_cancel,
	_fsm_action_oHold = tsip_atype_hold,
	_fsm_action_oResume = tsip_atype_resume,
	_fsm_action_oBYE = tsip_atype_hangup,
	_fsm_action_oShutdown = tsip_atype_shutdown,

	_fsm_action_iINVITE = 0xFF,
	_fsm_action_oUPDATE,
	_fsm_action_iUPDATE,
	_fsm_action_iCANCEL,
	_fsm_action_iPRACK,
	_fsm_action_oPRACK,
	_fsm_action_iACK,
	_fsm_action_oACK,

	_fsm_action_i1xx,
	_fsm_action_i2xx,
	_fsm_action_i300_to_i699,
	_fsm_action_i401_i407_i421_i494,

	_fsm_action_shutdown_timedout, /* Any -> Terminated */
	_fsm_action_transporterror,
	_fsm_action_error,
}
_fsm_action_t;

/* ======================== states ======================== */
typedef enum _fsm_state_e
{
	_fsm_state_Started,
	_fsm_state_Outgoing,
	_fsm_state_Incoming,
	_fsm_state_Trying,
	_fsm_state_Connected,
	_fsm_state_Terminated
}
_fsm_state_t;

#endif /* TINYSIP_DIALOG_INVITE_COMMON_H */
