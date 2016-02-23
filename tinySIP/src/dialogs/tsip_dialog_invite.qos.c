/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

/**@file tsip_dialog_invite.qos.c
 * @brief Integration of Resource Management and Session Initiation Protocol (SIP) (RFC 3312)
 * QoS Reservation.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/dialogs/tsip_dialog_invite.h"

#include "tinysip/dialogs/tsip_dialog_invite.common.h"

#include "tsk_debug.h"

#define TSIP_DIALOG_INVITE_QOS_RES_TIMEOUT 20

/* ======================== external functions ======================== */
extern int tsip_dialog_invite_timer_callback(const tsip_dialog_invite_t* self, tsk_timer_id_t timer_id);
extern int send_INVITEorUPDATE(tsip_dialog_invite_t *self, tsk_bool_t is_INVITE, tsk_bool_t force_sdp);

/* ======================== transitions ======================== */
static int x0300_Any_2_Any_X_timerRSVP(va_list *app);

/* Init FSM */
int tsip_dialog_invite_qos_init(tsip_dialog_invite_t *self)
{
    tsk_fsm_set(TSIP_DIALOG_GET_FSM(self),

                // Any -> (timerRSVP) -> Any
                TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_timerRSVP, tsk_fsm_state_any, x0300_Any_2_Any_X_timerRSVP, "x0300_Any_2_Any_X_timerRSVP"),


                TSK_FSM_ADD_NULL());

    return 0;
}

//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------

// Any -> (tiner RSVP) -> Any
int x0300_Any_2_Any_X_timerRSVP(va_list *app)
{
    tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
    return send_UPDATE(self, tsk_true);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++




/* cancel the timer */
int tsip_dialog_invite_qos_timer_cancel(tsip_dialog_invite_t* self)
{
    return tsk_timer_mgr_global_cancel(self->qos.timer.id);
}

/* schedule the timer */
int tsip_dialog_invite_qos_timer_schedule(tsip_dialog_invite_t* self)
{
    /*	To emulate bandwidth reservation (Because RSVP protocol is not supported) */
    self->qos.timer.id = tsk_timer_mgr_global_schedule(TSIP_DIALOG_INVITE_QOS_RES_TIMEOUT, TSK_TIMER_CALLBACK_F(tsip_dialog_invite_timer_callback), self);

    return 0;
}

