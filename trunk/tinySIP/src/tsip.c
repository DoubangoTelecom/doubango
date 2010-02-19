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

/**@file tsip.c
 * @brief SIP (RFC 3261) and 3GPP IMS (TS 24.229) implementation.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tsip.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tinysip/transactions/tsip_transac_layer.h"
#include "tinysip/dialogs/tsip_dialog_layer.h"
#include "tinysip/transports/tsip_transport_layer.h"

#include "tnet.h"

#include "tsk_memory.h"
#include "tsk_debug.h"
#include "tsk_time.h"

#include <stdarg.h>
#include <string.h>

void *run(void* self);


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
			case pname_preferred_identity:
			{
				const char *uristring = va_arg(values, const char*);
				if(uristring)
				{
					tsip_uri_t *uri = tsip_uri_parse(uristring, strlen(uristring));
					if(uri)
					{
						if(curr == pname_public_identity)
						{
							TSK_OBJECT_SAFE_FREE(self->public_identity);
							self->public_identity = uri;
						}
						else
						{
							TSK_OBJECT_SAFE_FREE(self->preferred_identity);
							self->preferred_identity = uri;
						}
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
#if defined (__GNUC__)
				self->local_port = (uint16_t)va_arg(values, unsigned);
#else
				self->local_port = va_arg(values, uint16_t);
#endif
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
						TSK_OBJECT_SAFE_FREE(self->realm);
						self->realm = uri;
					}
				}
				break;
			}
			case pname_discovery_naptr:
			{
				self->use_dns_naptr = va_arg(values, unsigned);
				break;
			}
			case pname_discovery_dhcp:
			{
				self->use_dhcp = va_arg(values, unsigned);
				break;
			}
			case pname_proxy_cscf:
			{
				const char* pcscf = va_arg(values, const char*);
				const char* transport = va_arg(values, const char*);
				int use_ipv6 = va_arg(values, int);

				tsk_strupdate(&self->proxy_cscf, pcscf);

				TSK_DEBUG_INFO("P-CSCF ==>%s", self->proxy_cscf);

				if(tsk_striequals(transport, "UDP")) TNET_SOCKET_TYPE_AS_UDP(self->proxy_cscf_type);
				else if(tsk_striequals(transport, "TCP")) TNET_SOCKET_TYPE_AS_TCP(self->proxy_cscf_type);
				else if(tsk_striequals(transport, "TLS")) TNET_SOCKET_TYPE_AS_TLS(self->proxy_cscf_type);
				else if(tsk_striequals(transport, "SCTP")) TNET_SOCKET_TYPE_AS_SCTP(self->proxy_cscf_type);

				if(use_ipv6) TNET_SOCKET_TYPE_AS_IPV6(self->proxy_cscf_type);
				else TNET_SOCKET_TYPE_AS_IPV4(self->proxy_cscf_type);
				break;
			}

			case pname_proxy_cscf_port:
			{
				self->proxy_cscf_port = va_arg(values, int);
				break;
			}

			case pname_device_id:
			{
				if(self->device_id)
					TSK_FREE(self->device_id);
				tsk_sprintf(&self->device_id, "\"%s\"", va_arg(values, const char*));
				break;
			}

			case pname_mobility:
			{
				tsk_strupdate(&self->mobility, va_arg(values, const char*));
				break;
			}

			case pname_sec_agree_mech:
			{
				tsk_strupdate(&self->sec_agree_mech, va_arg(values, const char*));
				break;
			}

			/* 
			* Features 
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

static unsigned __tsip_initialized = 0;

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

	stack->proxy_cscf_port = 5060;
	stack->proxy_cscf_type = tnet_socket_type_udp_ipv4;
	
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
	*	DNS context
	*/
	stack->dns_ctx = TNET_DNS_CTX_CREATE();

	/*
	*	DHCP context
	*/

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

		/* Use DNS NAPTR for the P-CSCF discovery? */
		if(stack->use_dns_naptr)
		{
			char* hostname = 0;
			tnet_port_t port = 0;

			if(!tnet_dns_query_naptr_srv(stack->dns_ctx, stack->realm->host, "SIP+D2U"/*FIXME*/, &hostname, &port))
			{
				tsk_strupdate(&stack->proxy_cscf, hostname);
				stack->proxy_cscf_port = port;
			}
			else
			{
				TSK_DEBUG_ERROR("P-CSCF discovery using DNS NAPTR failed. The stack will use the user supplied address and port.");
			}

			TSK_FREE(hostname);
		}

		/*
		* FIXME:
		*/
		if(tsip_transport_layer_add(stack->layer_transport, stack->local_ip, stack->local_port, stack->proxy_cscf_type, "FIXME")){
			// WHAT ???
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

		/*	ALL IS OK
		*	
		*/
		if(stack->layer_transac){
			stack->layer_transac->reliable = TNET_SOCKET_TYPE_IS_STREAM(stack->proxy_cscf_type);
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

int tsip_stack_set_callback_register(tsip_stack_handle_t *self, tsip_register_callback callback)
{
	if(self)
	{
		tsip_stack_t *stack = self;
		stack->callback_register = callback;
		
		return 0;
	}

	return -1;
}

int tsip_stack_set_callback_subscribe(tsip_stack_handle_t *self, tsip_subscribe_callback callback)
{
	if(self)
	{
		tsip_stack_t *stack = self;
		stack->callback_subscribe = callback;
		
		return 0;
	}

	return -1;
}

int tsip_stack_set_callback_message(tsip_stack_handle_t *self, tsip_message_callback callback)
{
	if(self)
	{
		tsip_stack_t *stack = self;
		stack->callback_message = callback;
		
		return 0;
	}

	return -1;
}

//int tsip_stack_alert(const tsip_stack_handle_t *self, tsip_operation_id_t opid, short status_code, char *reason_phrase, int incoming, tsip_event_type_t type)
//{
//	if(self)
//	{
//		const tsip_stack_t *stack = self;
//
//		TSK_RUNNABLE_ENQUEUE(TSK_RUNNABLE(stack), stack, opid, status_code, reason_phrase, incoming, type);
//		//tsip_event_t *event = TSIP_EVENT_CREATE(stack, opid, status_code, reason_phrase, incoming, type);
//		//TSK_RUNNABLE_ENQUEUE_OBJECT(TSK_RUNNABLE(stack), event);
//
//		return 0;
//	}
//	return -1;
//}

//#include "tsk_thread.h"
int tsip_stack_stop(tsip_stack_handle_t *self)
{
	if(self)
	{
		int ret;
		tsip_stack_t *stack = self;


		/* Hangup all dialogs */
		tsip_dialog_layer_hangupAll(stack->layer_dialog);
		//tsk_thread_sleep(50000000);

		/* Stop timer manager. */
		ret = tsk_timer_manager_stop(stack->timer_mgr);

		if(ret = tsk_runnable_stop(TSK_RUNNABLE(stack)))
		{
			//return ret;
		}

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

		/* Identity */
		TSK_FREE(stack->display_name);
		TSK_OBJECT_SAFE_FREE(stack->public_identity);
		TSK_OBJECT_SAFE_FREE(stack->preferred_identity);
		//TSK_OBJECT_SAFE_FREE(stack->associated_identity);
		TSK_FREE(stack->private_identity);
		TSK_FREE(stack->password);

		/* Network */
		TSK_FREE(stack->local_ip);
		TSK_FREE(stack->privacy);
		TSK_FREE(stack->netinfo);
		TSK_OBJECT_SAFE_FREE(stack->realm);
		TSK_FREE(stack->proxy_cscf);
		TSK_FREE(stack->device_id);
		TSK_FREE(stack->mobility);
		TSK_FREE(stack->sec_agree_mech);
		TSK_OBJECT_SAFE_FREE(stack->paths);
		TSK_OBJECT_SAFE_FREE(stack->service_routes);
		TSK_OBJECT_SAFE_FREE(stack->associated_uris);
		
		/* DNS */
		TSK_OBJECT_SAFE_FREE(stack->dns_ctx);

		/* DHCP */

		/* features */

		/* QoS */

		/* Internals. */
		TSK_OBJECT_SAFE_FREE(stack->timer_mgr);
		TSK_OBJECT_SAFE_FREE(stack->operations);

		/* Layers */
		TSK_OBJECT_SAFE_FREE(stack->layer_dialog);
		TSK_OBJECT_SAFE_FREE(stack->layer_transac);
		TSK_OBJECT_SAFE_FREE(stack->layer_transport);

		return 0;
	}

	return -1;
}

tsip_uri_t* tsip_stack_get_contacturi(const tsip_stack_handle_t *self, const char* protocol)
{
	if(self)
	{
		const tsip_stack_t *stack = self;
		tsk_list_item_t *item;
		tsk_list_foreach(item, stack->layer_transport->transports)
		{
			tsip_transport_t *transport = item->data;

			if(transport)
			{
				if(tsk_strequals(transport->protocol, protocol))
				{
					tsip_uri_t* uri = 0;
					if((uri = tsip_transport_get_uri(transport, 0)))
					{
						tsk_strupdate(&uri->user_name, stack->public_identity->user_name);
						return uri;
					}
				}
			}
		}
	}
	return 0;
}

tsip_uri_t* tsip_stack_get_pcscf_uri(const tsip_stack_handle_t *self, int lr)
{
	if(self)
	{
		const tsip_stack_t *stack = self;
		if(stack->layer_transport->transports && !TSK_LIST_IS_EMPTY(stack->layer_transport->transports))
		{
			tsip_transport_t *transport = stack->layer_transport->transports->head->data;
			if(transport)
			{
				tsip_uri_t* uri = 0;
			
				char* uristring = 0;
				tsk_sprintf(&uristring, "%s:%s:%d;%s;transport=%s",
					transport->scheme,
					stack->proxy_cscf,
					stack->proxy_cscf_port,
					lr ? "lr" : "",
					transport->protocol);
				if(uristring){
					if((uri = tsip_uri_parse(uristring, strlen(uristring)))){
						tnet_socket_type_t type = tsip_transport_get_socket_type(transport);
						uri->host_type = TNET_SOCKET_TYPE_IS_IPV6(type) ? host_ipv6 : host_ipv4;
					}
					TSK_FREE(uristring);
				}
				
				return uri;
			}
		}
	}
	return 0;
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

const struct tsip_dialog_layer_s * tsip_stack_get_dialog_layer(const tsip_stack_handle_t *self)
{
	if(self)
	{
		const tsip_stack_t *stack = self;
		return stack->layer_dialog;
	}
	return 0;
}

const struct tsip_transac_layer_s* tsip_stack_get_transac_layer(const tsip_stack_handle_t *self)
{
	if(self)
	{
		const tsip_stack_t *stack = self;
		return stack->layer_transac;
	}
	return 0;
}

const struct tsip_transport_layer_s* tsip_stack_get_transport_layer(const tsip_stack_handle_t *self)
{
	if(self)
	{
		const tsip_stack_t *stack = self;
		return stack->layer_transport;
	}
	return 0;
}










void *run(void* self)
{
	tsk_list_item_t *curr;
	tsip_stack_t *stack = self;

	TSK_RUNNABLE_RUN_BEGIN(stack);

	TSK_DEBUG_INFO("STACK::run -- START");
	
	if(curr = TSK_RUNNABLE_POP_FIRST(stack))
	{
		tsip_event_t *sipevent = (tsip_event_t*)curr->data;
		
		switch(sipevent->type)
		{
		case tsip_event_register:
			{
				if(stack->callback_register){
					stack->callback_register(TSIP_REGISTER_EVENT(sipevent));
					break;
				}
			}

		case tsip_event_subscribe:
			{
				if(stack->callback_subscribe){
					stack->callback_subscribe(TSIP_SUBSCRIBE_EVENT(sipevent));
					break;
				}
			}

		case tsip_event_message:
			{
				if(stack->callback_message){
					stack->callback_message(TSIP_MESSAGE_EVENT(sipevent));
					break;
				}
			}
		default:
			{
				if(stack->callback){
					stack->callback(sipevent);
				}
				break;
			}
		}
		
		tsk_object_unref(curr);
	}
	
	TSK_RUNNABLE_RUN_END(self);

	TSK_DEBUG_INFO("STACK::run -- STOP");
	return 0;
}
