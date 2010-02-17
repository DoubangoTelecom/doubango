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

/**@file tsip_transac.c
 * @brief SIP transaction base class as per RFC 3261 subclause 17.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/transactions/tsip_transac.h"

#include "tinysip/transports/tsip_transport_layer.h"
#include "tinysip/transactions/tsip_transac_layer.h"

#include "tinysip/transactions/tsip_transac_nist.h"
#include "tinysip/transactions/tsip_transac_nict.h"

#include "tsk_string.h"
#include "tsk_memory.h"

int tsip_transac_init(tsip_transac_t *self, const tsip_stack_handle_t * stack, tsip_transac_type_t type, unsigned reliable, int32_t cseq_value, const char* cseq_method, const char* callid)
{
	if(self && !self->initialized)
	{
		tsk_safeobj_init(self);

		self->stack = stack;
		self->timer_mgr = tsip_stack_get_timer_mgr(stack);
		self->type = type;
		self->reliable = reliable;
		self->cseq_value = cseq_value;
		self->cseq_method = tsk_strdup(cseq_method);
		self->callid = tsk_strdup(callid);
				
		return 0;
	}
	return -1;
}

int tsip_transac_deinit(tsip_transac_t *self)
{
	if(self && self->initialized)
	{
		self->stack = 0;
		
		TSK_FREE(self->branch);
		TSK_FREE(self->cseq_method);
		TSK_FREE(self->callid);

		tsk_safeobj_deinit(self);

		self->initialized = 0;

		return 0;
	}
	return -1;
}

int tsip_transac_start(tsip_transac_t *self, const tsip_request_t* request)
{
	int ret = -1;
	if(self)
	{
		if(self->type == tsip_nist)
		{
			ret = tsip_transac_nist_start(TSIP_TRANSAC_NIST(self), request);
		}
		else if(self->type == tsip_ist)
		{
		}
		else if(self->type == tsip_nict)
		{
			ret = tsip_transac_nict_start(TSIP_TRANSAC_NICT(self), request);
		}
		else if(self->type == tsip_ict)
		{
		}
	}

	return ret;
}

int tsip_transac_send(tsip_transac_t *self, const char *branch, const tsip_message_t *msg)
{
	if(self && self->stack)
	{
		const tsip_transport_layer_t *layer = tsip_stack_get_transport_layer(self->stack);
		if(layer)
		{
			return tsip_transport_layer_send(layer, branch, msg);
		}
	}
	return -1;
}

int tsip_transac_cmp(const tsip_transac_t *t1, const tsip_transac_t *t2)
{
	if(t1 && t2)
	{
		if(tsk_strequals(t1->branch, t2->branch) && tsk_strequals(t1->cseq_method, t2->cseq_method))
		{
			return 0;
		}
	}
	return -1;
}

int tsip_transac_remove_callback(const tsip_transac_t* self, tsk_timer_id_t timer_id)
{
	return tsip_transac_layer_remove(TSIP_STACK(self->stack)->layer_transac, TSIP_TRANSAC(self));
}
