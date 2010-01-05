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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsip_transac_nist.c
 * @brief SIP Non-INVITE Server Transaction as per RFC 3261 subclause 17.2.2.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/transactions/tsip_transac_nist.h"


void tsip_transac_nist_init(tsip_transac_nist_t *self)
{
	tsip_transac_nistContext_Init(&self->_fsm, self);
}

int tsip_transac_nist_start(tsip_transac_nist_t *self)
{
	return -1;
}

void tsip_transac_nist_Trying_2_Proceeding_X_send_1xx(tsip_transac_nist_t *self, const tsip_message_t* msg)
{
}
void tsip_transac_nist_Trying_2_Completed_X_send_200_to_699(tsip_transac_nist_t *self, const tsip_message_t* msg)
{
}
void tsip_transac_nist_Proceeding_2_Proceeding_X_send_1xx(tsip_transac_nist_t *self, const tsip_message_t* msg)
{
}
void tsip_transac_nist_Proceeding_2_Completed_X_send_200_to_699(tsip_transac_nist_t *self, const tsip_message_t* msg)
{
}
void tsip_transac_nist_Proceeding_2_Proceeding_X_send_response(tsip_transac_nist_t *self, const tsip_message_t* msg)
{
}
void tsip_transac_nist_Completed_2_Completed_X_send_response(tsip_transac_nist_t *self, const tsip_message_t* msg)
{
}
void tsip_transac_nist_Completed_2_Terminated_X_tirmerJ(tsip_transac_nist_t *self)
{
}
void tsip_transac_nist_Any_2_Terminated_X_transportError(tsip_transac_nist_t *self)
{
}


void tsip_transac_nist_OnTerminated(tsip_transac_nist_t *self)
{
}