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
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsip_transac_ist.h
 * @brief SIP INVITE Server Transaction as per RFC 3261 subclause 17.2.1.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_TRANSAC_IST_H
#define TINYSIP_TRANSAC_IST_H

#include "tinySIP_config.h"
#include "tinySIP/transactions/tsip_transac.h"

#include "tsk_fsm.h"

TSIP_BEGIN_DECLS

#define TSIP_TRANSAC_IST_CREATE(reliable, cseq_value, callid, dialog)		tsk_object_new(tsip_transac_ist_def_t, (tsk_bool_t)reliable, (int32_t)cseq_value, (const char*)callid, (tsip_dialog_t*)dialog)

#define TSIP_TRANSAC_IST(self)												((tsip_transac_ist_t*)(self))


typedef struct tsip_transac_ist
{
	TSIP_DECLARE_TRANSAC;

	tsk_fsm_t *fsm;

	tsip_response_t* lastResponse;

	tsip_timer_t timerH;
	tsip_timer_t timerI;
	tsip_timer_t timerG;
	tsip_timer_t timerL;
}
tsip_transac_ist_t;

int tsip_transac_ist_start(tsip_transac_ist_t *self, const tsip_request_t* request);

TINYSIP_GEXTERN const void *tsip_transac_ist_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_TRANSAC_IST_H */

