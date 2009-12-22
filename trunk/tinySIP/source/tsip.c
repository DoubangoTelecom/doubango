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

/**@file tsip.c
 * @brief SIP (RFC 3261) and 3GPP IMS (TS 24.229) implementation.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tsip.h"

#include "tinysip/tsip_uri.h"
#include "tinysip/tsip_timers.h"

#include "tinysip/transactions/tsip_transac_layer.h"
#include "tinysip/dialogs/tsip_dialog_layer.h"
#include "tinysip/transports/tsip_transport_layer.h"

#include "tsk_timer.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <stdarg.h>

void *run(void* self);

#define TSIP_EVENT_CREATE(str)				tsk_object_new(tsip_event_def_t, str)
#define TSIP_EVENT_SAFE_FREE(self)			tsk_object_unref(self), self = 0


typedef struct tsip_stack_s
{
	TSK_DECLARE_RUNNABLE;

	tsip_stack_callback callback;

	/* Identity */
	tsip_uri_t *uri;
	tsip_uri_t *preferred_uri;
	char *user_name;
	char *password;

	/* Network */
	char *local_ip;
	uint16_t local_port;
	unsigned enable_ipv6:1;
	char *privacy;
	operator_id_t operator_id;
	amf_t amf;
	char *netinfo;
	char *realm;
	tsip_uri_t *proxy;

	/* Services */
	unsigned enable_100rel:1;
	unsigned enable_gsmais:1;
	unsigned enable_precond:1;
	unsigned enable_3gppsms:1;
	unsigned enable_gsmarcs:1;
	unsigned enable_earlyIMS:1;
	unsigned enable_ofdr:1;
	unsigned enable_aa:1;
	unsigned enable_dnd:1;
	unsigned enable_option:1;

	/* QoS */



	/* Internals. */
	tsk_timer_manager_handle_t* timer_mgr;
	tsip_timers_t timers;
	tsip_operations_L_t *operations;

	/* Layers */
	tsip_dialog_layer_t *layer_dialog;
	tsip_transac_layer_t *layer_transac;
	tsip_transport_layer_t *layer_transport;
}
tsip_stack_t;


int __tsip_stack_set(tsip_stack_t *self, va_list values)
{
	tsip_stack_param_type_t curr;

	while((curr=va_arg(values, tsip_stack_param_type_t)) != pname_null)
	{
		switch(curr)
		{
			/* 
			* Identity 
			*/
			case pname_uri:
			{
				
			}
			case pname_preferred_uri:
			{
				
			}
			case pname_user_name:
			{
				
			}
			case pname_password:
			{
				
			}

			/* 
			* Network 
			*/
			case pname_local_ip:
			{
				
			}
			case pname_local_port:
			{
				
			}
			case pname_enable_ipv6:
			{
				
			}
			case pname_privacy:
			{
				
			}
			case pname_operator_id:
			{

			}
			case pname_amf:
			{

			}
			case pname_netinfo:
			{

			}
			case pname_realm:
			{

			}
			case pname_proxy:
			{

			}

			/* 
			* Services 
			*/
			case pname_enable_100rel:
			{

			}
			case pname_enable_gsmais:
			{

			}
			case pname_enable_precond:
			{

			}
			case pname_enable_3gppsms:
			{

			}
			case pname_enable_gsmarcs:
			{

			}
			case pname_enable_earlyIMS:
			{

			}
			case pname_enable_ofdr:
			{

			}
			case pname_enable_aa:
			{

			}
			case pname_enable_dnd:
			{

			}
			case pname_enable_option:
			{

			}

			/* 
			* QoS 
			*/

			default:
			{
				return -1;
			}
		}
	}

	return 0;
}


tsip_stack_handle_t* tsip_stack_create(tsip_stack_callback callback, ...)
{
	tsip_stack_t *stack = tsk_calloc(1, sizeof(tsip_stack_t));
	va_list params;

	if(!stack) return 0;

	va_start(params, callback);
	if(__tsip_stack_set(stack, params))
	{
		// Delete the stack?
	}
	va_end(params);

	/* Internals */
	stack->callback = callback;
	stack->timer_mgr = TSK_TIMER_MANAGER_CREATE();
	stack->operations = TSK_LIST_CREATE();

	/* Layers */
	stack->layer_dialog = TSIP_DIALOG_LAYER_CREATE(stack);
	stack->layer_transac = TSIP_TRANSAC_LAYER_CREATE(stack);
	stack->layer_transport = TSIP_TRANSPORT_LAYER_CREATE(stack);

	return stack;
}

int tsip_stack_start(tsip_stack_handle_t *self)
{
	if(self)
	{
		int ret;
		tsip_stack_t *stack = self;
		
		TSK_RUNNABLE(stack)->run = run;
		if(ret = tsk_runnable_start(TSK_RUNNABLE(stack), tsip_event_def_t))
		{
			return ret;
		}

		/* Start timer manager. */
		if(ret = tsk_timer_manager_start(stack->timer_mgr))
		{
			// What to do ?
		}
		
		TSK_DEBUG_INFO("SIP STACK -- START");

		return ret;
	}
	return -1;
}

int tsip_stack_set(tsip_stack_handle_t *self, ...)
{
	if(self)
	{
		int ret;
		tsip_stack_t *stack = self;

		va_list params;
		va_start(params, self);
		ret = __tsip_stack_set(stack, params);
		va_end(params);
		return ret;
	}

	return -1;
}

int tsip_stack_stop(tsip_stack_handle_t *self)
{
	if(self)
	{
		int ret;
		tsip_stack_t *stack = self;

		if(ret = tsk_runnable_stop(TSK_RUNNABLE(stack)))
		{
			//return ret;
		}

		/* Stop timer manager. */
		ret = tsk_timer_manager_stop(stack->timer_mgr);

		TSK_DEBUG_INFO("SIP STACK -- STOP");

		return ret;
	}

	return -1;
}

int tsip_stack_destroy(tsip_stack_handle_t *self)
{
	if(self)
	{
		tsip_stack_t *stack = self;

		TSK_TIMER_MANAGER_SAFE_FREE(stack->timer_mgr);
		TSK_LIST_SAFE_FREE(stack->operations);

		TSIP_DIALOG_LAYER_SAFE_FREE(stack->layer_dialog);
		TSIP_TRANSAC_LAYER_SAFE_FREE(stack->layer_transac);
		TSIP_TRANSPORT_LAYER_SAFE_FREE(stack->layer_transport);

		return 0;
	}

	return -1;
}

struct tsip_dialog_layer_s * tsip_stack_get_dialog_layer(const tsip_stack_handle_t *self)
{
	if(self)
	{
		const tsip_stack_t *stack = self;
		return stack->layer_dialog;
	}
	return 0;
}

struct tsip_transac_layer_s* tsip_stack_get_transac_layer(const tsip_stack_handle_t *self)
{
	if(self)
	{
		const tsip_stack_t *stack = self;
		return stack->layer_transac;
	}
	return 0;
}

struct tsip_transport_layer_s* tsip_stack_get_transport_layer(const tsip_stack_handle_t *self)
{
	if(self)
	{
		const tsip_stack_t *stack = self;
		return stack->layer_transport;
	}
	return 0;
}






int tsip_stack_register(tsip_stack_handle_t *self, const tsip_operation_handle_t *operation)
{
	if(self && operation)
	{
		const tsip_stack_t *stack = self;
		tsip_operation_handle_t *op = tsip_operation_clone(operation);

		tsk_list_push_back_data(stack->operations, (void**)&op);
	}
	return -1;
}

int tsip_stack_unregister(tsip_stack_handle_t *self, const tsip_operation_handle_t *operation)
{
	if(self && operation)
	{
		const tsip_stack_t *stack = self;
		tsip_operation_handle_t *op = tsip_operation_clone(operation);

		tsk_list_push_back_data(stack->operations, (void**)&op);
	}
	return -1;
}











void *run(void* self)
{
	tsk_list_item_t *curr;
	tsip_stack_t *stack = self;

	TSK_RUNNABLE_RUN_BEGIN(stack);
	
	if(curr = TSK_RUNNABLE_POP_FIRST(stack))
	{
		tsip_event_t *sipevent = (tsip_event_t*)curr->data;
		
		if(stack->callback)
		{
			stack->callback(sipevent);
		}
		tsk_object_unref(curr);
	}
	
	TSK_RUNNABLE_RUN_END(self);

	return 0;
}














//========================================================
//	SIP event object definition
//
static void* tsip_event_create(void * self, va_list * app)
{
	tsip_event_t *sipevent = self;
	if(sipevent)
	{
		sipevent->stack = va_arg(*app, const tsip_stack_handle_t *);
		sipevent->status_code = va_arg(*app, short);
		sipevent->reason_phrase = tsk_strdup(va_arg(*app, const char *));

		sipevent->incoming = va_arg(*app, unsigned);
		sipevent->type = va_arg(*app, tsip_event_type_t);
	}
	return self;
}

static void* tsip_event_destroy(void * self)
{ 
	tsip_event_t *sipevent = self;
	if(sipevent)
	{
		TSK_FREE(sipevent->reason_phrase);
	}
	return self;
}

static int tsip_event_cmp(const void *obj1, const void *obj2)
{
	return -1;
}

static const tsk_object_def_t tsip_event_def_s = 
{
	sizeof(tsip_event_t),
	tsip_event_create, 
	tsip_event_destroy,
	tsip_event_cmp, 
};
const void *tsip_event_def_t = &tsip_event_def_s;