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