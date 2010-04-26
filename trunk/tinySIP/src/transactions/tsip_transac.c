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

/**@file tsip_transac.c
 * @brief SIP transaction base class as per RFC 3261 subclause 17.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/transactions/tsip_transac.h"

#include "tinySIP/transports/tsip_transport_layer.h"
#include "tinySIP/transactions/tsip_transac_layer.h"

#include "tinySIP/transactions/tsip_transac_ist.h"
#include "tinySIP/transactions/tsip_transac_nist.h"
#include "tinySIP/transactions/tsip_transac_nict.h"
#include "tinySIP/transactions/tsip_transac_ict.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

int tsip_transac_init(tsip_transac_t *self, tsip_transac_type_t type, tsk_bool_t reliable, int32_t cseq_value, const char* cseq_method, const char* callid, tsip_dialog_t* dialog, tsk_fsm_state_id curr, tsk_fsm_state_id term)
{
	if(self && !self->initialized){
		self->type = type;
		self->reliable = reliable;
		self->cseq_value = cseq_value;
		self->cseq_method = tsk_strdup(cseq_method);
		self->callid = tsk_strdup(callid);
		self->dialog = tsk_object_ref(dialog);

		/* FSM */
		self->fsm = tsk_fsm_create(curr, term);

		self->initialized = tsk_true;
				
		return 0;
	}
	return -1;
}

int tsip_transac_deinit(tsip_transac_t *self)
{
	if(self && self->initialized){	
		TSK_FREE(self->branch);
		TSK_FREE(self->cseq_method);
		TSK_FREE(self->callid);
		TSK_OBJECT_SAFE_FREE(self->dialog);

		/* FSM */
		TSK_OBJECT_SAFE_FREE(self->fsm);

		self->initialized = tsk_false;

		return 0;
	}
	return -1;
}

int tsip_transac_start(tsip_transac_t *self, const tsip_request_t* request)
{
	int ret = -1;
	if(self){
		switch(self->type){
			case tsip_nist:{
					ret = tsip_transac_nist_start(TSIP_TRANSAC_NIST(self), request);
					break;
				}
			case tsip_ist:{
					ret = tsip_transac_ist_start(TSIP_TRANSAC_IST(self), request);
					break;
				}
			case tsip_nict:{
					ret = tsip_transac_nict_start(TSIP_TRANSAC_NICT(self), request);
					break;
				}
			case tsip_ict:{
					ret = tsip_transac_ict_start(TSIP_TRANSAC_ICT(self), request);
					break;
				}
		}
	}

	return ret;
}

int tsip_transac_send(tsip_transac_t *self, const char *branch, const tsip_message_t *msg)
{
	if(self && TSIP_TRANSAC_GET_STACK(self)){
		const tsip_transport_layer_t *layer = TSIP_TRANSAC_GET_STACK(self)->layer_transport;
		if(layer){
			return tsip_transport_layer_send(layer, branch, msg);
		}
	}
	return -1;
}

int tsip_transac_cmp(const tsip_transac_t *t1, const tsip_transac_t *t2)
{
	if(t1 && t2){
		if(tsk_strequals(t1->branch, t2->branch) && tsk_strequals(t1->cseq_method, t2->cseq_method)){
			return 0;
		}
	}
	return -1;
}

int tsip_transac_remove(const tsip_transac_t* self)
{
	return tsip_transac_layer_remove(TSIP_TRANSAC_GET_STACK(self)->layer_transac, TSIP_TRANSAC(self));
}

int tsip_transac_fsm_act(tsip_transac_t* self, tsk_fsm_action_id action_id, const tsip_message_t* message)
{
	if(!self || !self->fsm){
		TSK_DEBUG_WARN("Invalid parameter.");
		return -1;
	}
	return tsk_fsm_act(self->fsm, action_id, self, message, self, message);
}
