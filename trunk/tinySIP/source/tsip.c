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

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tinysip/transactions/tsip_transac_layer.h"
#include "tinysip/dialogs/tsip_dialog_layer.h"
#include "tinysip/transports/tsip_transport_layer.h"

#include "tnet.h"

#include "tsk_memory.h"
#include "tsk_debug.h"
#include "tsk_time.h"

#include <stdarg.h>

void *run(void* self);

#define TSIP_EVENT_CREATE(str)				tsk_object_new(tsip_event_def_t, str)
#define TSIP_EVENT_SAFE_FREE(self)			tsk_object_unref(self), self = 0

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
			case pname_display_name:
			{
				tsk_strupdate(&self->display_name, va_arg(values, const char*));
				break;
			}
			case pname_public_identity:
			{
				const char *uristring = va_arg(values, const char*);
				if(uristring)
				{
					tsip_uri_t *uri = tsip_uri_parse(uristring, strlen(uristring));
					if(uri)
					{
						TSIP_URI_SAFE_FREE(self->public_identity);
						self->public_identity = uri;
					}
				}
				break;
			}
			case pname_private_identity:
			{
				tsk_strupdate(&self->private_identity, va_arg(values, const char*));
				break;
			}
			case pname_password:
			{
				tsk_strupdate(&self->password, va_arg(values, const char*));
				break;
			}

			/* 
			* Network 
			*/
			case pname_local_ip:
			{
				tsk_strupdate(&self->local_ip, va_arg(values, const char*));
				break;
			}
			case pname_local_port:
			{
				self->local_port = va_arg(values, uint16_t);
				break;
			}
			case pname_enable_ipv6:
			{
				self->enable_ipv6 = va_arg(values, unsigned);
				break;
			}
			case pname_privacy:
			{
				tsk_strupdate(&self->privacy, va_arg(values, const char*));
				break;
			}
			case pname_operator_id:
			{
				break;
			}
			case pname_amf:
			{
				break;
			}
			case pname_netinfo:
			{
				tsk_strupdate(&self->netinfo, va_arg(values, const char*));
				break;
			}
			case pname_realm:
			{
				const char *uristring = va_arg(values, const char*);
				if(uristring)
				{
					tsip_uri_t *uri = tsip_uri_parse(uristring, strlen(uristring));
					if(uri)
					{
						TSIP_URI_SAFE_FREE(self->realm);
						self->realm = uri;
					}
				}
				break;
			}
			case pname_proxy_cscf:
			{
				tsk_strupdate(&self->proxy_cscf, va_arg(values, const char*));
				break;
			}

			case pname_proxy_cscf_port:
			{
				self->proxy_cscf_port = va_arg(values, uint16_t);
				break;
			}

			/* 
			* Services 
			*/
			case pname_enable_100rel:
			{
				self->enable_100rel = va_arg(values, unsigned);
				break;
			}
			case pname_enable_gsmais:
			{
				self->enable_gsmais = va_arg(values, unsigned);
				break;
			}
			case pname_enable_precond:
			{
				self->enable_precond = va_arg(values, unsigned);
				break;
			}
			case pname_enable_3gppsms:
			{
				self->enable_3gppsms = va_arg(values, unsigned);
				break;
			}
			case pname_enable_gsmarcs:
			{
				self->enable_gsmarcs = va_arg(values, unsigned);
				break;
			}
			case pname_enable_earlyIMS:
			{
				self->enable_earlyIMS = va_arg(values, unsigned);
				break;
			}
			case pname_enable_ofdr:
			{
				self->enable_ofdr = va_arg(values, unsigned);
				break;
			}
			case pname_enable_aa:
			{
				self->enable_aa = va_arg(values, unsigned);
				break;
			}
			case pname_enable_dnd:
			{
				self->enable_dnd = va_arg(values, unsigned);
				break;
			}
			case pname_enable_option:
			{
				self->enable_option = va_arg(values, unsigned);
				break;
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

static __tsip_initialized = 0;

int tsip_global_init()
{
	if(!__tsip_initialized)
	{
		srand((unsigned int) tsk_time_epoch());
		if(!tnet_startup())
		{
			__tsip_initialized = 1;
			return 0;
		}
	}
	return -1;
}

int tsip_global_deinit()
{
	if(__tsip_initialized)
	{
		if(!tnet_cleanup())
		{
			__tsip_initialized = 0;
			return 0;
		}
	}
	return -1;
}

tsip_stack_handle_t* tsip_stack_create(tsip_stack_callback callback, ...)
{
	tsip_stack_t *stack = tsk_calloc(1, sizeof(tsip_stack_t));
	va_list params;

	if(!stack) return 0;

	/*
	* Default values
	*/
	stack->local_ip = TNET_SOCKET_HOST_ANY;
	stack->local_port = TNET_SOCKET_PORT_ANY;

	va_start(params, callback);
	if(__tsip_stack_set(stack, params))
	{
		// Delete the stack?
	}
	va_end(params);

	/* 
	* Internals 
	*/
	stack->callback = callback;
	stack->timer_mgr = TSK_TIMER_MANAGER_CREATE();
	stack->operations = TSK_LIST_CREATE();

	/* 
	*	Layers 
	*/
	stack->layer_dialog = TSIP_DIALOG_LAYER_CREATE(stack);
	stack->layer_transac = TSIP_TRANSAC_LAYER_CREATE(stack);
	stack->layer_transport = TSIP_TRANSPORT_LAYER_CREATE(stack);

	/*
	* FIXME:
	*/
	if(tsip_transport_layer_add(stack->layer_transport, stack->local_ip, stack->local_port, "UDP/IPV4 TRANSPORT"))
	{
		// WHAT ???
	}

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

		/*
		* Transport Layer
		*/
		tsip_transport_layer_start(stack->layer_transport);

		/* 
		* Timer manager
		*/
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


const tsk_timer_manager_handle_t* tsip_stack_get_timer_mgr(const tsip_stack_handle_t *self)
{
	if(self)
	{
		const tsip_stack_t *stack = self;
		return stack->timer_mgr;
	}
	return 0;
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

		return tsip_dialog_layer_register(stack->layer_dialog, op);

		//tsk_list_push_back_data(stack->operations, (void**)&op);
	}
	return -1;
}

int tsip_stack_unregister(tsip_stack_handle_t *self, const tsip_operation_handle_t *operation)
{
	if(self && operation)
	{
		const tsip_stack_t *stack = self;
		tsip_operation_handle_t *op = tsip_operation_clone(operation);

		//tsk_list_push_back_data(stack->operations, (void**)&op);
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
