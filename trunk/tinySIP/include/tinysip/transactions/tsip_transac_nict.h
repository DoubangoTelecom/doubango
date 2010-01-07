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

/**@file tsip_transac_nict.h
 * @brief SIP Non-INVITE Client Transaction as per RFC 3261 subcaluse 17.1.2.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_TRANSAC_NICT_H
#define TINYSIP_TRANSAC_NICT_H

#include "tinysip_config.h"

#include "tinysip/transactions/tsip_transac.h"
#include "tinysip/tsip_message.h"

#include "tinysip/smc/tsip_transac_nict_sm.h"

TSIP_BEGIN_DECLS

#define TSIP_TRANSAC_NICT_CREATE(stack, reliable, cseq_value, cseq_method, callid)		tsk_object_new(tsip_transac_nict_def_t, (const tsip_stack_handle_t *)stack, (unsigned)reliable, (int32_t)cseq_value, (const char*)cseq_method, (const char*)callid)
#define TSIP_TRANSAC_NICT_SAFE_FREE(self)												tsk_object_unref(self), self = 0

#define TSIP_TRANSAC_NICT(self)															((tsip_transac_nict_t*)(self))

typedef struct tsip_transac_nict
{
	TSIP_DECLARE_TRANSAC;

	struct tsip_transac_nictContext _fsm; /**< The state machine context. */

	tsip_request_t* request;
	tsip_timer_t timerE;
	tsip_timer_t timerF;
	tsip_timer_t timerK;
}
tsip_transac_nict_t;

void tsip_transac_nict_init(tsip_transac_nict_t *self);
int tsip_transac_nict_start(tsip_transac_nict_t *self, tsip_request_t* request);
void tsip_transac_nict_OnTerminated(tsip_transac_nict_t *self);

void tsip_transac_nict_Started_2_Trying_X_send(tsip_transac_nict_t *self);
void tsip_transac_nict_Trying_2_Trying_X_timerE(tsip_transac_nict_t *self);
void tsip_transac_nict_Trying_2_Terminated_X_timerF(tsip_transac_nict_t *self);
void tsip_transac_nict_Trying_2_Terminated_X_transportError(tsip_transac_nict_t *self);
void tsip_transac_nict_Trying_2_Proceedding_X_1xx(tsip_transac_nict_t *self, const tsip_message_t* msg);
void tsip_transac_nict_Trying_2_Completed_X_200_to_699(tsip_transac_nict_t *self, const tsip_message_t* msg);
void tsip_transac_nict_Trying_2_Trying_X_unknown(tsip_transac_nict_t *self);
void tsip_transac_nict_Proceeding_2_Proceeding_X_timerE(tsip_transac_nict_t *self);
void tsip_transac_nict_Proceeding_2_Terminated_X_timerF(tsip_transac_nict_t *self);
void tsip_transac_nict_Proceeding_2_Terminated_X_transportError(tsip_transac_nict_t *self);
void tsip_transac_nict_Proceeding_2_Proceeding_X_1xx(tsip_transac_nict_t *self, const tsip_message_t* msg);
void tsip_transac_nict_Proceeding_2_Completed_X_200_to_699(tsip_transac_nict_t *self, const tsip_message_t* msg);
void tsip_transac_nict_Completed_2_Terminated_X_timerK(tsip_transac_nict_t *self);
void tsip_transac_nict_Any_2_Terminated_X_transportError(tsip_transac_nict_t *self);


TINYSIP_GEXTERN const void *tsip_transac_nict_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_TRANSAC_NICT_H */

