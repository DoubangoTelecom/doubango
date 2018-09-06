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

/**@file tsip_dialog_invite.common.h
 * @brief SIP dialog INVITE (common variables).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSIP_DIALOG_INVITE_COMMON_H
#define TINYSIP_DIALOG_INVITE_COMMON_H

#include "tinysip/api/tsip_api_invite.h"

#define DEBUG_STATE_MACHINE										1
#define TSIP_DIALOG_INVITE_SIGNAL(self, type, code, phrase, message)	\
	tsip_invite_event_signal(type, TSIP_DIALOG(self)->ss, code, phrase, message)
#define TSIP_DIALOG_INVITE_TIMER_SCHEDULE(TX)						TSIP_DIALOG_TIMER_SCHEDULE(invite, TX)

#define TSIP_DIALOG_INVITE_ICE_CONNCHECK_TIMEOUT	16000

/* ======================== actions ======================== */
typedef enum _fsm_action_e {
    _fsm_action_accept = tsip_atype_accept,
    _fsm_action_reject = tsip_atype_hangup,
    _fsm_action_dtmf_send = tsip_atype_dtmf_send,
    _fsm_action_msrp_send_msg = tsip_atype_lmessage,
    _fsm_action_oINVITE = tsip_atype_invite,
    _fsm_action_oCANCEL = tsip_atype_cancel,
    _fsm_action_oHold = tsip_atype_hold,
    _fsm_action_oResume = tsip_atype_resume,
    _fsm_action_oECT = tsip_atype_ect,
    _fsm_action_iECT_ACCEPT = tsip_atype_ect_accept,
    _fsm_action_iECT_REJECT = tsip_atype_ect_reject,
    _fsm_action_iECT_lNOTIFY = tsip_atype_ect_lnotify,
    _fsm_action_oINFO = tsip_atype_info_send,
    _fsm_action_oBYE = tsip_atype_hangup,
    _fsm_action_oShutdown = tsip_atype_shutdown,
    _fsm_action_transporterror = tsip_atype_transport_error,

    _fsm_action_iINVITE = 0xFF,
    _fsm_action_oUPDATE,
    _fsm_action_iUPDATE,
    _fsm_action_iCANCEL,
    _fsm_action_iPRACK,
    _fsm_action_oPRACK,
    _fsm_action_iACK,
    _fsm_action_oACK,
    _fsm_action_iOPTIONS,
    _fsm_action_oOPTIONS,
    _fsm_action_iBYE,
    _fsm_action_iREFER,
    _fsm_action_iINFO,
    _fsm_action_iNOTIFY,

    _fsm_action_timer100rel,
    _fsm_action_timerRefresh,
    _fsm_action_timerRSVP,

    _fsm_action_i1xx,
    _fsm_action_i2xx,
    _fsm_action_i300_to_i699,
    _fsm_action_i401_i407,
    _fsm_action_i422,

    _fsm_action_shutdown_timedout, /* Any -> Terminated */
    _fsm_action_error,
}
_fsm_action_t;

/* ======================== states ======================== */
typedef enum _fsm_state_e {
    _fsm_state_Started,
    _fsm_state_Outgoing,
    _fsm_state_Incoming,
    _fsm_state_PreChecking,
    _fsm_state_Trying,
    _fsm_state_Ringing,
    _fsm_state_Cancelling,
    _fsm_state_InProgress,

    _fsm_state_Holding,
    _fsm_state_Resuming,

    _fsm_state_oECTing,
    _fsm_state_iECTing,
    _fsm_state_iECTreq,

    _fsm_state_Connected,
    _fsm_state_Terminated
}
_fsm_state_t;


#define send_INVITE(self, force_sdp) send_INVITEorUPDATE(self, tsk_true, force_sdp)
#define send_UPDATE(self, force_sdp) send_INVITEorUPDATE(self, tsk_false, force_sdp)

#else
#error "This file must only be included in a source(.c or .cxx)"

#endif /* TINYSIP_DIALOG_INVITE_COMMON_H */
