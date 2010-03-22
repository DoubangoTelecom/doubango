/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/dialogs/tsip_dialog_invite.h"

#include "tsk_debug.h"


/* Started -> (iINVITE) -> Incoming
*/
int s0000_Satrted_2_Incoming_X_iINVITE(va_list *app)
{
	return 0;
}
