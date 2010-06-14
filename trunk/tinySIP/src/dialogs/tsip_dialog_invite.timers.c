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

/**@file tsip_dialog_invite.timers.c
 * @brief SIP dialog INVITE as per RFC 3261.
 * The SOA machine is designed as per RFC 3264 and draft-ietf-sipping-sip-offeranswer-12.
 * Session Timers as per RFC 4028.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/dialogs/tsip_dialog_invite.h"

#include "tinysip/dialogs/tsip_dialog_invite.common.h"

extern int tsip_dialog_invite_timer_callback(const tsip_dialog_invite_t* self, tsk_timer_id_t timer_id);

int tsip_dialog_invite_stimers_cancel(tsip_dialog_invite_t* self)
{
	return tsk_timer_manager_cancel(TSIP_DIALOG_GET_STACK(self)->timer_mgr, self->stimers.timer.id);
}

int tsip_dialog_invite_stimers_schedule(tsip_dialog_invite_t* self, uint64_t timeout)
{
	self->stimers.timer.timeout = timeout;
	self->stimers.timer.id = tsk_timer_manager_schedule(TSIP_DIALOG_GET_STACK(self)->timer_mgr, timeout, TSK_TIMER_CALLBACK_F(tsip_dialog_invite_timer_callback), self);

	return 0;
}
