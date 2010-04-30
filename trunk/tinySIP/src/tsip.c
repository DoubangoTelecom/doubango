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

/**@file tsip.c
 * @brief SIP (RFC 3261) and 3GPP IMS/LTE (TS 24.229) implementation.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tsip.h"

#include "tinySIP/parsers/tsip_parser_uri.h"

#include "tinySIP/transactions/tsip_transac_layer.h"
#include "tinySIP/dialogs/tsip_dialog_layer.h"
#include "tinySIP/transports/tsip_transport_layer.h"

#include "tinySIP/api/tsip_api_register.h"
#include "tinySIP/api/tsip_api_subscribe.h"
#include "tinySIP/api/tsip_api_message.h"

#include "tnet.h"

#include "tsk_memory.h"
#include "tsk_debug.h"
#include "tsk_time.h"

#include <stdarg.h>
#include <string.h>

void *run(void* self);

/* For tests:
* http://www.voip-info.org/wiki/view/PROTOS+Test-Suite
* http://tools.ietf.org/html/rfc4475
* http://portal.etsi.org/docbox/EC_Files/EC_Files/ts_10202702v030101p.pdf
*/

int __tsip_stack_set_option(tsip_stack_t *self, tsip_stack_option_t option_id, const char* option_value)
{
	switch(option_id){

		/* === Identity === */
		case TSIP_STACK_OPTION_DISPLAY_NAME:
			{	/* Display name */
				tsk_strupdate(&self->identity.display_name, option_value);
				break;
			}
		case TSIP_STACK_OPTION_IMPU:
		case TSIP_STACK_OPTION_PREFERRED_IDENTITY:
			{	/* IMS Public User Identity or P-Preferred-Identity - valid SIP/TEL URI*/
				if(option_value){
					tsip_uri_t *uri = tsip_uri_parse(option_value, tsk_strlen(option_value));
					if(uri){
						if(option_id == TSIP_STACK_OPTION_IMPU){
							TSK_OBJECT_SAFE_FREE(self->identity.impu);
							self->identity.impu = uri;
						}
						else{
							TSK_OBJECT_SAFE_FREE(self->identity.preferred);
							self->identity.preferred = uri;
						}
					}
					else{
						TSK_DEBUG_ERROR("'%s' is an invalid SIP/TEL URI", option_value);
						if(option_id == TSIP_STACK_OPTION_IMPU){
							return -1; /* IMPU is mandatory but P-Preferred-Identity isn't. */
						}
					}
				}
				break;
			}
		case TSIP_STACK_OPTION_IMPI:
			{	/* IMS Private User Identity */
				if(!option_value){
					TSK_DEBUG_ERROR("IMPI is mandatory.");
					return -1; /* mandatory */
				}
				tsk_strupdate(&self->identity.impi, option_value);
				break;
			}
		case TSIP_STACK_OPTION_PASSWORD:
			{	/* User's password */
				tsk_strupdate(&self->identity.password, option_value);
				break;
			}

	/* ===  Network ===  */
		case TSIP_STACK_OPTION_REALM:
			{	/* realm (a.k.a domain name) - valid SIP/TEL URI */
				if(option_value){
					tsip_uri_t *uri = tsip_uri_parse(option_value, tsk_strlen(option_value));
					if(uri){
						if(uri->type == uri_unknown){ /* scheme is missing or unsupported? */
							tsk_strupdate(&uri->scheme, "sip");
							uri->type = uri_sip;
						}
						TSK_OBJECT_SAFE_FREE(self->network.realm); /* delete old */
						self->network.realm = uri;
					}
					else{
						TSK_DEBUG_ERROR("'%s' is an invalid SIP/TEL URI", option_value);
						return -1; /* mandatory */
					}
				}
				break;
			}
		case TSIP_STACK_OPTION_LOCAL_IP:
			{	/* IP address to bind to */
				tsk_strupdate(&self->network.local_ip, option_value);
				break;
			}
		case TSIP_STACK_OPTION_LOCAL_PORT:
			{	/* IP port to bind to */
				tnet_port_t port = atoi(option_value);
				if(port){
					self->network.local_port = port;
				}
				else{
					TSK_DEBUG_ERROR("%s is invalid as port number.", option_value);
				}
				break;
			}
		case TSIP_STACK_OPTION_DISCOVERY_NAPTR:
			{	/* Whether to use DNS NAPTR for Proxy-CSCF discovery */
				self->network.discovery_naptr = tsk_strcontains(option_value, tsk_strlen(option_value), "true");
				break;
			}
		case TSIP_STACK_OPTION_DISCOVERY_DHCP:
			{	/* Whether to use DHCPv4/v6 for Proxy-CSCF discovery */
				self->network.discovery_dhcp = tsk_strcontains(option_value, tsk_strlen(option_value), "true");
				break;
			}

	/* ===  Security ===  */
		case TSIP_STACK_OPTION_EARLY_IMS:
			{	/* Whether to enable Early IMS Security as per 3GPP TS 33.978 */
				self->security.earlyIMS = tsk_strcontains(option_value, tsk_strlen(option_value), "true");
				break;
			}
		//case TSIP_STACK_OPTION_IMS_AKA_AMF:
		//	{	/* Authentication management field (AMF) as 16-bit field */
		//		/* think about uint16_t */
		//		if(option_value){
		//			uint16_t amf = atoi(option_value);
		//			self->security.amf[0] = (amf >> 8);
		//			self->security.amf[1] = (amf & 0xFF);
		//		}
		//		break;
		//	}
		//case TSIP_STACK_OPTION_IMS_AKA_OPID:
		//	{	/* IMS-AKA, Operator Id - 16-bytes */
		//		break;
		//	}
		case TSIP_STACK_OPTION_SECAGREE_IPSEC:
			{	/* Whether to enable IPSec security agreement as per IETF RFC 3329 */
				break;
			}
		case TSIP_STACK_OPTION_SECAGREE_TLS:
			{	/* Whether to enable IPSec security agreement as per IETF RFC 3329 */
				break;
			}

	/* === Features === */
		

		default:
			{
				TSK_DEBUG_WARN("Unknown Option id %d", option_id);
				break;
			}
	}
	return 0;
}

int __tsip_stack_set(tsip_stack_t *self, va_list* app)
{
	tsip_stack_param_type_t curr;

	while((curr = va_arg(*app, tsip_stack_param_type_t)) != pname_null){
		switch(curr){
			/* === Option === */
			case pname_option:
			{	/* (tsip_stack_option_t)ID_ENUM, (const char*)VALUE_STR */
				tsip_stack_option_t ID_ENUM = va_arg(*app, tsip_stack_option_t);
				const char* VALUE_STR = va_arg(*app, const char*);
				if(__tsip_stack_set_option(self, ID_ENUM, VALUE_STR)){
					/* Nothing to do --> va_list still safe */
				}				
				break;
			}

			/* 
			* Network 
			*/
			
			
			
			
			
			
			/*case pname_discovery_naptr:
			{
				self->network.discovery_naptr = va_arg(*app, tsk_bool_t);
				break;
			}
			case pname_discovery_dhcp:
			{
				self->network.discovery_dhcp = va_arg(*app, unsigned);
				break;
			}*/
			case pname_proxy_cscf:
			{	/* (const char*)FQDN_STR, (unsigned)PORT_UINT, (const char*)TRANSPORT_STR, (const char*)IP_VERSION_STR */
				const char* FQDN_STR = va_arg(*app, const char*);
				tnet_port_t PORT_UINT = va_arg(*app, unsigned);
				const char* TRANSPORT_STR = va_arg(*app, const char*);
				const char* IP_VERSION_STR = va_arg(*app, const char*);

				/* IP Address */
				tsk_strupdate(&self->network.proxy_cscf, FQDN_STR);
				
				/* Port */
				if(PORT_UINT){
					self->network.proxy_cscf_port = PORT_UINT;
				}

				/* Transport */
				if(!TRANSPORT_STR || tsk_striequals(TRANSPORT_STR, "UDP")){
					TNET_SOCKET_TYPE_SET_UDP(self->network.proxy_cscf_type);
				}
				else if(tsk_striequals(TRANSPORT_STR, "TCP")){
					TNET_SOCKET_TYPE_SET_TCP(self->network.proxy_cscf_type);
				}
				else if(tsk_striequals(TRANSPORT_STR, "TLS")){
					TNET_SOCKET_TYPE_SET_TLS(self->network.proxy_cscf_type);
				}
				else if(tsk_striequals(TRANSPORT_STR, "SCTP")){
					TNET_SOCKET_TYPE_SET_SCTP(self->network.proxy_cscf_type);
				}
				else{
					TSK_DEBUG_ERROR("%s not a valid transport", TRANSPORT_STR);
					/* not mandatoy */
				}
				/* whether to use ipv6 or not */
				if(IP_VERSION_STR){
					if(tsk_strcontains(IP_VERSION_STR, tsk_strlen(IP_VERSION_STR), "6")){
						TNET_SOCKET_TYPE_SET_IPV6Only(self->network.proxy_cscf_type);
					}
					if(tsk_strcontains(IP_VERSION_STR, tsk_strlen(IP_VERSION_STR), "4")){
						TNET_SOCKET_TYPE_SET_IPV4(self->network.proxy_cscf_type); /* Not IPV4only ==> '46'/'64' */
					}
				}
				break;
			}

			

			

			

			/* === Security === */
			case pname_amf:
			{ /* (uint16_t)AMF_UINT16 */
				unsigned amf = va_arg(*app, unsigned);
				self->security.amf[0] = (amf >> 8);
				self->security.amf[1] = (amf & 0xFF);
				break;
			}
			case pname_operator_id:
				{ /* (const char*)OPID_HEX_STR */
					const char* hexstr = va_arg(*app, const char*);
					size_t len = tsk_strlen(hexstr);
					if(len && !(len & 0x01)){
						size_t i, j;
						if(tsk_strindexOf(hexstr, tsk_strlen(hexstr), "0x") == 0){
							hexstr += 2;
							len -= 2;
						}
						/* reset old value */
						for(i=0; i<sizeof(operator_id_t); i++){
							self->security.operator_id[i] = 0x00;
						}
						/* set new value */
						if(len){ /* perhaps there were only 2 chars*/
							for(i = 0, j = 0; (i<sizeof(operator_id_t) && i<len); i+=2, j++){
								sscanf(&hexstr[i], "%2x", &self->security.operator_id[j]); /*do not use tsk_atox(str), because str should end with '\0'.*/
							}
						}
					}
					else{
						TSK_DEBUG_ERROR("%s is invlaid for an Operator Id value.", hexstr);
					}
					break;
				}
			case pname_secagree_ipsec:
				{	/* ALG_STR, EALG_STR, MODE_STR, PROTOCOL_STR */
					tsk_strupdate(&self->secagree_mech, "ipsec-3gpp");
					tsk_strupdate(&self->secagree_ipsec.alg, va_arg(*app, const char*));
					tsk_strupdate(&self->secagree_ipsec.ealg, va_arg(*app, const char*));
					tsk_strupdate(&self->secagree_ipsec.mode, va_arg(*app, const char*));
					tsk_strupdate(&self->secagree_ipsec.protocol, va_arg(*app, const char*));
					self->enable_secagree_ipsec = 1;
					break;
				}
			case pname_secagree_tls:
				{	/* USE_TLS_SECAGREE_INT */
					self->enable_secagree_tls = va_arg(*app, int) ? 1 : 0;
					break;
				}
			case pname_tls_certs:
				{	/*CA_FILE_STR, PUB_FILE_STR, PRIV_FILE_STR*/
					tsk_strupdate(&self->tls.ca, va_arg(*app, const char*));
					tsk_strupdate(&self->tls.pbk, va_arg(*app, const char*));
					tsk_strupdate(&self->tls.pvk, va_arg(*app, const char*));
					break;
				}
			

			/* === Headers === */
			case pname_header:
				{ /* (const char*)NAME_STR, (const char*)VALUE_STR */
					const char* NAME_STR = va_arg(*app, const char*);
					const char* VALUE_STR = va_arg(*app, const char*);
					if(VALUE_STR == ((const char*)-1)){ /* UNSET */
						tsk_params_remove_param(self->headers, NAME_STR);
					}
					else{ /* SET */
						tsk_params_add_param(&self->headers, NAME_STR, VALUE_STR);
					}
					break;
				}
			

			

			default:
			{	/* va_list will be unsafe ==> must exit */
				TSK_DEBUG_WARN("Found unknown pname.");
				goto bail;
			}
		}/* switch */
	}/* while */

bail:
	return 0;
}

tsip_stack_handle_t* tsip_stack_create(tsip_stack_callback_f callback, const char* realm_uri, const char* impi_uri, const char* impu_uri, ...)
{
	tsip_stack_t* stack = tsk_null;
	va_list ap;	

	/* === check values === */
	if(!realm_uri || !impi_uri || !impu_uri){
		TSK_DEBUG_ERROR("Invalid parameter.");
		goto bail;
	}
	
	/* === create the stack === */
	if(!(stack = tsk_object_new(tsip_stack_def_t))){ /* should never happen */
		TSK_DEBUG_ERROR("Failed to create the stack.");
		goto bail;
	}
	
	/* === Set mandatory values (realm, IMPI and IMPU) === */
	if(tsip_stack_set(stack,
			TSIP_STACK_SET_OPTION(TSIP_STACK_OPTION_REALM, realm_uri),
			TSIP_STACK_SET_OPTION(TSIP_STACK_OPTION_IMPI, impi_uri),
			TSIP_STACK_SET_OPTION(TSIP_STACK_OPTION_IMPU, impu_uri),

			TSIP_STACK_SET_NULL())){
		TSK_DEBUG_ERROR("Invalid parameter.");
		TSK_OBJECT_SAFE_FREE(stack);
		goto bail;
	}
	
	/* === Default values === */
	stack->network.local_ip = TNET_SOCKET_HOST_ANY;
	stack->network.local_port = TNET_SOCKET_PORT_ANY;
	
	stack->network.proxy_cscf_port = 5060;
	stack->network.proxy_cscf_type = tnet_socket_type_udp_ipv4;
	
	/* === Set user supplied parameters === */
	va_start(ap, impu_uri);
	if(__tsip_stack_set(stack, &ap)){
		TSK_DEBUG_ERROR("Invalid parameter.");
		TSK_OBJECT_SAFE_FREE(stack);
		va_end(ap);
		goto bail;
	}
	va_end(ap);

	/* === Internals === */
	stack->callback = callback;
	stack->timer_mgr = tsk_timer_manager_create();
	stack->ssessions = tsk_list_create();
	if(!stack->headers){ /* could be created by tsk_params_add_param() */
		stack->headers = tsk_list_create();
	}

	/* ===	Layers === */
	stack->layer_dialog = tsip_dialog_layer_create(stack);
	stack->layer_transac = tsip_transac_layer_create(stack);
	stack->layer_transport = tsip_transport_layer_create(stack);

	/* === DNS context === */
	stack->dns_ctx = tnet_dns_ctx_create();

	/* === DHCP context === */

bail:
	return stack;
}

int tsip_stack_start(tsip_stack_handle_t *self)
{
	int ret = -1;
	tsip_stack_t *stack = self;

	if(stack){

		if(stack->started){
			TSK_DEBUG_WARN("Stack Already started");
			return 0;
		}
		
		/* === Timer manager === */
		if((ret = tsk_timer_manager_start(stack->timer_mgr))){
			goto bail;
		}

		/* === Set P-Preferred-Identity === */
		if(!stack->identity.preferred && stack->identity.impu){
			stack->identity.preferred = tsk_object_ref((void*)stack->identity.impu);
		}
		/* === Transport type === */
		if(stack->secagree_mech){
			if(tsk_striequals(stack->secagree_mech, "ipsec-3gpp")){
				TNET_SOCKET_TYPE_SET_IPSEC(stack->network.proxy_cscf_type);
			}
			//else if if(tsk_striquals(stack->secagree_mech, "ipsec-ike"))
		}

		/* === Use DNS NAPTR+SRV for the P-CSCF discovery? === */
		if(!stack->network.proxy_cscf || (stack->network.discovery_dhcp || stack->network.discovery_naptr)){
			if(stack->network.discovery_dhcp){ /* DHCP v4/v6 */
				/* FIXME: */
				ret = -2;
			} /* DHCP */
			else{ /* DNS NAPTR + SRV*/
				char* hostname = tsk_null;
				tnet_port_t port = 0;

				if(!(ret = tnet_dns_query_naptr_srv(stack->dns_ctx, stack->network.realm->host, 
					TNET_SOCKET_TYPE_IS_DGRAM(stack->network.proxy_cscf_type) ? "SIP+D2U" :
					(TNET_SOCKET_TYPE_IS_TLS(stack->network.proxy_cscf_type) ? "SIPS+D2T" : "SIP+D2T"),
					&hostname, &port))){
					tsk_strupdate(&stack->network.proxy_cscf, hostname);
					if(!stack->network.proxy_cscf_port || stack->network.proxy_cscf_port==5060){ /* Only if the Proxy-CSCF port is missing or default */
						stack->network.proxy_cscf_port = port;
					}
				}
				else{
					TSK_DEBUG_ERROR("P-CSCF discovery using DNS NAPTR failed. The stack will use the user supplied address and port.");
				}
				
				TSK_FREE(hostname);
			} /* NAPTR */
		}

		/* Check Proxy-CSCF IP address */
		if(stack->network.proxy_cscf){
			TSK_DEBUG_INFO("Proxy-CSCF=[%s]:%d", stack->network.proxy_cscf, stack->network.proxy_cscf_port);
		}
		else{
			TSK_DEBUG_ERROR("Proxy-CSCF IP address is Null.");
			goto bail;
		}
		
		/* === Get Best source address ===  */
		if(!stack->network.local_ip){ /* loacal-ip is missing? */
			tnet_ip_t bestsource;
			if((ret = tnet_getbestsource(stack->network.proxy_cscf, stack->network.proxy_cscf_port, stack->network.proxy_cscf_type, &bestsource))){ /* FIXME: what about linux version? */
				TSK_DEBUG_ERROR("Failed to get best source [%d].", ret);
				/* do not exit ==> will use default IP address */
			}
			else{
				tsk_strupdate(&stack->network.local_ip, bestsource);
			}
		}

		/* === Runnable === */
		TSK_RUNNABLE(stack)->run = run;
		if((ret = tsk_runnable_start(TSK_RUNNABLE(stack), tsip_event_def_t))){
			goto bail;
		}
		
		/* === Transport Layer === */
		/* Adds the default transport to the transport Layer */
		if((ret = tsip_transport_layer_add(stack->layer_transport, stack->network.local_ip, stack->network.local_port, stack->network.proxy_cscf_type, "SIP transport"))){
			goto bail;
		}
		/* Starts the transport Layer */
		if((ret = tsip_transport_layer_start(stack->layer_transport))){
			goto bail;
		}
		
		/* ===	ALL IS OK === */
		if(stack->layer_transac){ /* For transaction layer */
			stack->layer_transac->reliable = TNET_SOCKET_TYPE_IS_STREAM(stack->network.proxy_cscf_type);
		}
		
		stack->started = tsk_true;

		TSK_DEBUG_INFO("SIP STACK -- START");

		return ret;
	}

bail:
	return ret;
}

int tsip_stack_set(tsip_stack_handle_t *self, ...)
{
	if(self){
		int ret;
		tsip_stack_t *stack = self;

		va_list ap;
		va_start(ap, self);
		ret = __tsip_stack_set(stack, &ap);
		va_end(ap);
		return ret;
	}

	return -1;
}

/**
* Stops the stack. 
* Before stopping the engine will hangup all dialogs (wtring with non-register dialogs).
* To continue to receive callbacks, you should call this function before destroying the stack.
*/
int tsip_stack_stop(tsip_stack_handle_t *self)
{
	tsip_stack_t *stack = self;

	if(stack){
		int ret;
		tsk_bool_t one_failed = tsk_false;

		if(!stack->started){
			TSK_DEBUG_WARN("Stack already stopped");
			return 0;
		}

		/* Hangup all dialogs staring by REGISTER */	
		if((ret = tsip_dialog_layer_shutdownAll(stack->layer_dialog))){
			TSK_DEBUG_WARN("Failed to hang-up all dialogs");
			one_failed = tsk_true;
		}

		/* do not try to clean up transactions ==> each dialog will cancel its transactions.
		* see tsip_dialog_deinit() which call tsip_transac_layer_cancel_by_dialog() */

		/* Stop the timer manager */
		if((ret = tsk_timer_manager_stop(stack->timer_mgr))){
			TSK_DEBUG_WARN("Failed to stop the timer manager");
			one_failed = tsk_true;
		}
		
		/* Stop the transport layer */
		if((ret = tsip_transport_layer_shutdown(stack->layer_transport))){
			TSK_DEBUG_WARN("Failed to stop the transport layer");
			one_failed = tsk_true;
		}

		if((ret = tsk_runnable_stop(TSK_RUNNABLE(stack)))){
			TSK_DEBUG_WARN("Failed to stop the stack");
			one_failed = tsk_true;
		}

		if(!one_failed){
			stack->started = tsk_false;
		}

		TSK_DEBUG_INFO("SIP STACK -- STOP");

		return ret;
	}

	return -1;
}

tsip_uri_t* tsip_stack_get_contacturi(const tsip_stack_t *stack, const char* protocol)
{
	if(stack){
		tsk_list_item_t *item;
		tsk_list_foreach(item, stack->layer_transport->transports){
			tsip_transport_t *transport = item->data;

			if(transport){
				if(tsk_strequals(transport->protocol, protocol)){
					tsip_uri_t* uri = tsk_null;
					if((uri = tsip_transport_get_uri(transport, 0))){
						tsk_strupdate(&uri->user_name, stack->identity.impu->user_name);
						return uri;
					}
				}
			}
		}
	}
	return tsk_null;
}

tsip_uri_t* tsip_stack_get_pcscf_uri(const tsip_stack_t *stack, tsk_bool_t lr)
{
	if(stack){
		if(stack->layer_transport->transports && !TSK_LIST_IS_EMPTY(stack->layer_transport->transports)){
			tsip_transport_t *transport = stack->layer_transport->transports->head->data;
			if(transport){
				tsip_uri_t* uri = tsk_null;
				int ipv6 = TNET_SOCKET_TYPE_IS_IPV6(transport->type);
				int quote_ip = (ipv6 && tsk_strcontains(stack->network.proxy_cscf, tsk_strlen(stack->network.proxy_cscf), ":")) /* IPv6 IP string?*/;
			
				char* uristring = tsk_null;
				tsk_sprintf(&uristring, "%s:%s%s%s:%d;%s;transport=%s",
					transport->scheme,
					quote_ip ? "[" : "",
					stack->network.proxy_cscf,
					quote_ip ? "]" : "",
					stack->network.proxy_cscf_port,
					lr ? "lr" : "",
					transport->protocol);
				if(uristring){
					if((uri = tsip_uri_parse(uristring, tsk_strlen(uristring)))){
						//uri->host_type = ipv6 ? host_ipv6 : host_ipv4;
					}
					TSK_FREE(uristring);
				}
				
				return uri;
			}
		}
	}
	return tsk_null;
}










void *run(void* self)
{
	tsk_list_item_t *curr;
	tsip_stack_t *stack = self;

	TSK_DEBUG_INFO("SIP STACK::run -- START");

	TSK_RUNNABLE_RUN_BEGIN(stack);
	
	if((curr = TSK_RUNNABLE_POP_FIRST(stack))){
		tsip_event_t *sipevent = (tsip_event_t*)curr->data;
		if(stack->callback){
			stack->callback(sipevent);
		}				
		tsk_object_unref(curr);
	}
	
	TSK_RUNNABLE_RUN_END(self);

	TSK_DEBUG_INFO("SIP STACK::run -- STOP");
	return 0;
}










//========================================================
//	SIP stack object definition
//
static tsk_object_t* tsip_stack_ctor(tsk_object_t * self, va_list * app)
{
	tsip_stack_t *stack = self;
	if(stack){
	}
	return self;
}

static tsk_object_t* tsip_stack_dtor(tsk_object_t * self)
{ 
	tsip_stack_t *stack = self;
	if(stack){

		/* /!\ Order in which objects are destroyed is very important */

		/* Stop 
		* Will try to hangup all dialogs */
		tsip_stack_stop(stack);

		/* Layers(1/1): Transacs and dialogs use timer_mgr when destroyed 
		* Dialogs =>(use)=> transacs =>(use)=> transport. */
		TSK_OBJECT_SAFE_FREE(stack->layer_dialog);
		TSK_OBJECT_SAFE_FREE(stack->layer_transac);
		TSK_OBJECT_SAFE_FREE(stack->layer_transport);

		/* Internals(1/2) */
		TSK_OBJECT_SAFE_FREE(stack->timer_mgr);

		/* Identity */
		TSK_FREE(stack->identity.display_name);
		TSK_OBJECT_SAFE_FREE(stack->identity.impu);
		TSK_OBJECT_SAFE_FREE(stack->identity.preferred);
		//TSK_OBJECT_SAFE_FREE(stack->associated_identity);
		TSK_FREE(stack->identity.impi);
		TSK_FREE(stack->identity.password);

		/* Network(1/1) */
		TSK_FREE(stack->network.local_ip);
		TSK_OBJECT_SAFE_FREE(stack->network.realm);
		TSK_FREE(stack->network.proxy_cscf);
		TSK_OBJECT_SAFE_FREE(stack->paths);

		TSK_OBJECT_SAFE_FREE(stack->service_routes);
		TSK_OBJECT_SAFE_FREE(stack->associated_uris);
		
		/* Security(1/1) */
		TSK_FREE(stack->secagree_mech);
		TSK_FREE(stack->secagree_ipsec.alg);
		TSK_FREE(stack->secagree_ipsec.ealg);
		TSK_FREE(stack->secagree_ipsec.mode);
		TSK_FREE(stack->secagree_ipsec.protocol);

		TSK_FREE(stack->tls.ca);
		TSK_FREE(stack->tls.pbk);
		TSK_FREE(stack->tls.pvk);


		/* DNS */
		TSK_OBJECT_SAFE_FREE(stack->dns_ctx);

		/* DHCP */

		/* features */

		/* QoS */

		/* Internals (2/2) */
		TSK_OBJECT_SAFE_FREE(stack->ssessions);
		TSK_OBJECT_SAFE_FREE(stack->headers);

		TSK_DEBUG_INFO("*** SIP Stack destroyed ***");
	}
	return self;
}

static const tsk_object_def_t tsip_stack_def_s = 
{
	sizeof(tsip_stack_t),
	tsip_stack_ctor, 
	tsip_stack_dtor,
	tsk_null, 
};
const tsk_object_def_t *tsip_stack_def_t = &tsip_stack_def_s;
