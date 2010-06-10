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

#include "tinysip/tsip_event.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tinysip/transactions/tsip_transac_layer.h"
#include "tinysip/dialogs/tsip_dialog_layer.h"
#include "tinysip/transports/tsip_transport_layer.h"

#include "tinysip/api/tsip_api_register.h"
#include "tinysip/api/tsip_api_subscribe.h"
#include "tinysip/api/tsip_api_message.h"

#include "tnet.h"

#include "tsk_memory.h"
#include "tsk_debug.h"
#include "tsk_time.h"

#include <stdarg.h>
#include <string.h>

static void *run(void* self);

/* For tests:
* http://www.voip-info.org/wiki/view/PROTOS+Test-Suite
* http://tools.ietf.org/html/rfc4475
* http://portal.etsi.org/docbox/EC_Files/EC_Files/ts_10202702v030101p.pdf
*/


/**@defgroup tsip_stack_group 3GPP IMS/LTE Stack
*/

extern tsip_event_t* tsip_event_create(tsip_ssession_t* ss, short code, const char* phrase, const tsip_message_t* sipmessage, tsip_event_type_t type);
#define TSIP_STACK_SIGNAL(self, code, phrase) \
	{ \
		tsip_event_t* e; \
		if((e = tsip_event_create(tsk_null, code, phrase, tsk_null, tsip_event_stack))){ \
			TSK_RUNNABLE_ENQUEUE_OBJECT(TSK_RUNNABLE(self), e); \
		} \
	}

/* Internal function used to set all user's parameters */
int __tsip_stack_set(tsip_stack_t *self, va_list* app)
{
	tsip_stack_param_type_t curr;

	while((curr = va_arg(*app, tsip_stack_param_type_t)) != tsip_pname_null){
		switch(curr){
			
			/* === Identity === */
			case tsip_pname_display_name:
				{	/* (const char*)NAME_STR */
					const char* NAME_STR = va_arg(*app, const char*);
					tsk_strupdate(&self->identity.display_name, NAME_STR);
					break;
				}
			case tsip_pname_impu:
			case tsip_pname_preferred_id:
				{	/* (const char*)URI_STR */
					const char* URI_STR = va_arg(*app, const char*);
					if(!tsk_strnullORempty(URI_STR)){
						tsip_uri_t *uri = tsip_uri_parse(URI_STR, tsk_strlen(URI_STR));
						if(uri){
							if(curr == tsip_pname_impu){
								TSK_OBJECT_SAFE_FREE(self->identity.impu);
								self->identity.impu = uri;
							}
							else{
								TSK_OBJECT_SAFE_FREE(self->identity.preferred);
								self->identity.preferred = uri;
							}
						}
						else{
							TSK_DEBUG_ERROR("'%s' is an invalid SIP/TEL URI", URI_STR);
							if(curr == tsip_pname_impu){
								return -1; /* IMPU is mandatory but P-Preferred-Identity isn't. */
							}
						}
					}
					else if(curr == tsip_pname_impu){
						TSK_DEBUG_ERROR("IMPU (IMS Public Identity) is mandatory.");
						return -1;
					}
					break;
				}
			case tsip_pname_impi:
				{	/* (const char*)IMPI_STR */
					const char* IMPI_STR = va_arg(*app, const char*);
					if(tsk_strnullORempty(IMPI_STR)){
						TSK_DEBUG_ERROR("IMPI (IMS Private Identity) is mandatory.");
						return -1; /* mandatory */
					}
					tsk_strupdate(&self->identity.impi, IMPI_STR);
					break;
				}
			case tsip_pname_password:
				{	/* (const char*)PASSORD_STR */
					const char* PASSORD_STR = va_arg(*app, const char*);
					tsk_strupdate(&self->identity.password, PASSORD_STR);
					break;
				}

			/* === SigComp === */
			case tsip_pname_sigcomp:
				{	/* (unsigned)DMS_UINT, (unsigned)SMS_UINT, (unsigned)CPB_UINT, (tsk_bool_t)PRES_DICT_BOOL */
					self->sigcomp.dms = va_arg(*app, unsigned);
					self->sigcomp.sms = va_arg(*app, unsigned);
					self->sigcomp.cpb = va_arg(*app, unsigned);
					self->sigcomp.pres_dict = va_arg(*app, tsk_bool_t);
					break;
				}
			case tsip_pname_sigcomp_add_compartment:
				{	/* (const char*)COMPARTMENT_ID_STR */
					if(!self->sigcomp.handle){
						self->sigcomp.handle = tsip_sigcomp_handler_create(self->sigcomp.cpb, self->sigcomp.dms, self->sigcomp.sms);
						tsip_sigcomp_handler_set_dicts(self->sigcomp.handle, self->sigcomp.sip_dict, self->sigcomp.pres_dict);
					}
					tsip_sigcomp_handler_add_compartment(self->sigcomp.handle, va_arg(*app, const char*));
					break;
				}
			case tsip_pname_sigcomp_remove_compartment:
				{	/* (const char*)COMPARTMENT_ID_STR */
					if(self->sigcomp.handle){
						tsip_sigcomp_handler_remove_compartment(self->sigcomp.handle, va_arg(*app, const char*));
					}
					break;
				}

			/* === Network === */
			case tsip_pname_realm:
				{	/* (const char*)URI_STR */
					const char* URI_STR = va_arg(*app, const char*);
					tsip_uri_t *uri;
					if(!tsk_strnullORempty(URI_STR) && (uri = tsip_uri_parse(URI_STR, tsk_strlen(URI_STR)))){
						if(uri->type == uri_unknown){ /* scheme is missing or unsupported? */
							tsk_strupdate(&uri->scheme, "sip");
							uri->type = uri_sip;
						}
						TSK_OBJECT_SAFE_FREE(self->network.realm); /* delete old */
						self->network.realm = uri;
					}
					else{
						TSK_DEBUG_ERROR("'%s' is an invalid SIP/TEL URI", URI_STR);
						return -1; /* mandatory */
					}
					break;
				}
			case tsip_pname_local_ip:
				{	/* (const char*)IP_STR */
					const char* IP_STR = va_arg(*app, const char*);
					tsk_strupdate(&self->network.local_ip, IP_STR);
					break;
				}
			case tsip_pname_local_port:
				{	/* (unsigned)PORT_UINT */
					self->network.local_port = (tnet_port_t)va_arg(*app, unsigned);
					break;
				}
			case tsip_pname_aor:
				{	/* (const char*)IP_STR, (unsigned)PORT_UINT */
					const char* IP_STR = va_arg(*app, const char*);
					tnet_port_t PORT_UINT = (tnet_port_t)va_arg(*app, unsigned);
					if(!tsk_strnullORempty(IP_STR)){
						tsk_strupdate(&self->network.aor.ip, IP_STR);
					}
					if(PORT_UINT){
						self->network.aor.port = PORT_UINT;
					}
					
					break;
				}
			case tsip_pname_discovery_naptr:
				{	/* (tsk_bool_t)ENABLED_BOOL */
					self->network.discovery_naptr = va_arg(*app, tsk_bool_t);
					break;
				}
			case tsip_pname_discovery_dhcp:
				{	/* (tsk_bool_t)ENABLED_BOOL */
					self->network.discovery_dhcp = va_arg(*app, tsk_bool_t);
					break;
				}
			case tsip_pname_proxy_cscf:
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
				if(tsk_strnullORempty(TRANSPORT_STR) || tsk_striequals(TRANSPORT_STR, "UDP")){
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
				if(!tsk_strnullORempty(IP_VERSION_STR)){
					if(tsk_strcontains(IP_VERSION_STR, tsk_strlen(IP_VERSION_STR), "6")){
						TNET_SOCKET_TYPE_SET_IPV6Only(self->network.proxy_cscf_type);
					}
					if(tsk_strcontains(IP_VERSION_STR, tsk_strlen(IP_VERSION_STR), "4")){
						TNET_SOCKET_TYPE_SET_IPV4(self->network.proxy_cscf_type); /* Not IPV4only ==> '46'/'64' */
					}
				}
				break;
			}
			case pnale_dnsserver:
				{	/* (const char*)IP_STR */
					const char* IP_STR = va_arg(*app, const char*);
					if(tnet_dns_add_server(self->dns_ctx, IP_STR)){
						TSK_DEBUG_ERROR("Failed to add [%s] as DNS server", IP_STR);
					}
					break;
				}
			


			/* === Security === */
			case tsip_pname_early_ims:
				{	/* (tsk_bool_t)ENABLED_BOOL */
					self->security.earlyIMS = va_arg(*app, tsk_bool_t);
					break;
				}
			case tsip_pname_secagree_ipsec:
				{	/* (tsk_bool_t)ENABLED_BOOL */
					if((self->security.enable_secagree_ipsec = va_arg(*app, tsk_bool_t))){
						tsk_strupdate(&self->security.secagree_mech, "ipsec-3gpp");
					}
					break;
				}
			case tsip_pname_secagree_tls:
				{	/* (tsk_bool_t)ENABLED_BOOL */
					if((self->security.enable_secagree_tls = va_arg(*app, tsk_bool_t))){
						tsk_strupdate(&self->security.secagree_mech, "tls");
					}
					break;
				}
			case tsip_pname_amf:
			{ /* (uint16_t)AMF_UINT16 */
				unsigned amf = va_arg(*app, unsigned);
				self->security.amf[0] = (amf >> 8);
				self->security.amf[1] = (amf & 0xFF);
				break;
			}
			case tsip_pname_operator_id:
				{ /* (const char*)OPID_HEX_STR */
					const char* hexstr = va_arg(*app, const char*);
					tsk_size_t len = tsk_strlen(hexstr);
					if(len && !(len & 0x01)){
						tsk_size_t i, j;
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
						TSK_DEBUG_ERROR("%s is invalid for an Operator Id value.", hexstr);
					}
					break;
				}
			case tsip_pname_ipsec_params:
				{	/* (const char*)ALG_STR, (const char*)EALG_STR, (const char*)MODE_STR, (const char*)PROTOCOL_STR*/
					tsk_strupdate(&self->security.ipsec.alg, va_arg(*app, const char*));
					tsk_strupdate(&self->security.ipsec.ealg, va_arg(*app, const char*));
					tsk_strupdate(&self->security.ipsec.mode, va_arg(*app, const char*));
					tsk_strupdate(&self->security.ipsec.protocol, va_arg(*app, const char*));
					break;
				}
			case tsip_pname_tls_certs:
				{	/* (const char*)CA_FILE_STR, (const char*)PUB_FILE_STR, (const char*)PRIV_FILE_STR */
					tsk_strupdate(&self->security.tls.ca, va_arg(*app, const char*));
					tsk_strupdate(&self->security.tls.pbk, va_arg(*app, const char*));
					tsk_strupdate(&self->security.tls.pvk, va_arg(*app, const char*));
					break;
				}
			

			/* === Dummy Headers === */
			case tsip_pname_header:
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
			
			/* === User Data === */
			case tsip_pname_userdata:
				{	/* (const void*)DATA_PTR */
					self->userdata = va_arg(*app, const void*);
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

/**@ingroup tsip_stack_group
* Creates new 3GPP IMS/LTE stack handle. 
* As the 3GPP IMS/LTE stack depends on the network library (tinyNET), you MUST call <a href="http://doubango.org/API/tinyNET/tnet_8c.html#affba6c2710347476f615b0135777c640"> tnet_startup()</a> before using any SIP function (tsip_*). 
* <a href="http://doubango.org/API/tinyNET/tnet_8c.html#ac42b22a7ac5831f04326aee9de033c84"> tnet_cleanup()</a> is used to terminate use of network functions.
* @param callback Callback function to call to alert the application for new SIP or media events.
* @param realm_uri The realm is the name of the domain name to authenticate to. It should be a valid SIP URI (e.g. sip:open-ims.test).
* @param impi_uri The IMPI is a unique identifier assigned to a user (or UE) by the home network. 
* It could be either a SIP URI (e.g. sip:bob@open-ims.test), a tel URI (e.g. tel:+33100000) or any alphanumeric string (e.g. bob@open-ims.test or bob). 
* It is used to authenticate the UE (username field in SIP Authorization/Proxy-Authorization header).
* @param impu_uri As its name says, it’s you public visible identifier where you are willing to receive calls or any demands. 
* An IMPU could be either a SIP or tel URI (e.g. tel:+33100000 or sip:bob@open-ims.test). In IMS world, a user can have multiple IMPUs associated to its unique IMPI.
* @param ... Any TSIP_STACK_SET_*() macros.
* @retval A valid handle if succeed and Null-handle otherwise. As a stack is a well-defined object, you should use @a TSK_OBJECT_SAFE_FREE() to safely destroy the handle.
* 
* @code
int app_callback(const tsip_event_t *sipevent);

const char* realm_uri = "sip:open-ims.test";
const char* impi_uri = "bob@open-ims.test";
const char* impu_uri = "sip:bob@open-ims.test";

tsip_stack_handle_t* stack = tsip_stack_create(app_callback, realm_uri, impi_uri, impu_uri,
                                 TSIP_STACK_SET_PASSWORD("mysecret"),
                                 // ...other macros...
                                 TSIP_STACK_SET_NULL());
	
	// ...whatever

	TSK_OBJECT_SAFE_FREE(stack);
* @endcode
* @sa @ref tsip_stack_set()<br>@ref tsip_stack_start()
*/
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
			TSIP_STACK_SET_REALM(realm_uri),
			TSIP_STACK_SET_IMPI(impi_uri),
			TSIP_STACK_SET_IMPU(impu_uri),

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
	// all events should be delivered to the user before the stack stop
	tsk_runnable_set_important(TSK_RUNNABLE(stack), tsk_true);

	/* === SigComp === */
	// only create the handler on-demand: when compartment is added
	stack->sigcomp.dms = TSIP_SIGCOMP_DMS;
	stack->sigcomp.sms = TSIP_SIGCOMP_SMS;
	stack->sigcomp.cpb = TSIP_SIGCOMP_CPB;
	stack->sigcomp.sip_dict = TSIP_SIGCOMP_SIP_DICT;
	stack->sigcomp.pres_dict = TSIP_SIGCOMP_PRES_DICT;

	/* === DNS context === 
	* Because of TSIP_STACK_SET_DNS_SERVER(), ctx should be created before calling __tsip_stack_set()
	*/
	stack->dns_ctx = tnet_dns_ctx_create();

	/* === DHCP context === */

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

bail:
	return stack;
}

/**@ingroup tsip_stack_group
* Starts a 3GPP IMS/LTE stack. This function MUST be called before you start calling any SIP function (@a tsip_*).
* @param self The 3GPP IMS/LTE stack to start. This handle should be created using @ref tsip_stack_create().
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tsip_stack_start(tsip_stack_handle_t *self)
{
	int ret = -1;
	tsip_stack_t *stack = self;

	if(!stack){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

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
	if(!tsk_strnullORempty(stack->security.secagree_mech)){
		if(tsk_striequals(stack->security.secagree_mech, "ipsec-3gpp")){
			TNET_SOCKET_TYPE_SET_IPSEC(stack->network.proxy_cscf_type);
		}
		//else if if(tsk_striquals(stack->security.secagree_mech, "ipsec-ike"))
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
		TSK_DEBUG_ERROR("Failed to start timer manager");
		goto bail;
	}
	
	/* === Transport Layer === */
	/* Adds the default transport to the transport Layer */
	if((ret = tsip_transport_layer_add(stack->layer_transport, stack->network.local_ip, stack->network.local_port, stack->network.proxy_cscf_type, "SIP transport"))){
		TSK_DEBUG_ERROR("Failed to add new transport");
		goto bail;
	}
	/* Starts the transport Layer */
	if((ret = tsip_transport_layer_start(stack->layer_transport))){
		TSK_DEBUG_ERROR("Failed to start sip transport");
		goto bail;
	}
	else if(!stack->network.local_ip){
		/* Update the local_ip */
		if(!TSK_LIST_IS_EMPTY(stack->layer_transport->transports)){
			tnet_ip_t ip;
			if(!tnet_transport_get_ip_n_port_2(stack->layer_transport->transports->head->data, &ip, tsk_null)){
				stack->network.local_ip = tsk_strdup(ip);
			}
			else{
				TSK_DEBUG_WARN("Failed to get local_ip");
				/* Do not exit */
			}
		}
	}
	
	/* ===	ALL IS OK === */
	if(stack->layer_transac){ /* For transaction layer */
		stack->layer_transac->reliable = TNET_SOCKET_TYPE_IS_STREAM(stack->network.proxy_cscf_type);
	}
	
	stack->started = tsk_true;

	/* Signal to the end-user that the stack has been started */
	TSIP_STACK_SIGNAL(self, tsip_event_code_stack_started, "Stack started");
	
	TSK_DEBUG_INFO("SIP STACK -- START");

	return 0;
	

bail:
	TSIP_STACK_SIGNAL(self, tsip_event_code_stack_failed_to_start, "Stack failed to start");
	/* stop all running instances */
	if(stack->timer_mgr && TSK_RUNNABLE(stack->timer_mgr)->running){
		tsk_timer_manager_stop(stack->timer_mgr);
	}
	if(stack->layer_transport){
		tsip_transport_layer_shutdown(stack->layer_transport);
	}
	if(TSK_RUNNABLE(stack)->running){
		tsk_runnable_stop(TSK_RUNNABLE(stack));
	}

	return ret;
}

/**@ingroup tsip_stack_group
* Configures the stack.
* @param self The 3GPP IMS/LTE stack to configure. This handle should be created using @ref tsip_stack_create().
* @param ... Any TSIP_STACK_SET_*() or TSIP_STACK_UNSET_*() macros.
* @retval Zero if succeed and non-zero error code otherwise.
*
* @code
int ret = tsip_stack_set(stack, 
            TSIP_STACK_SET_HEADER("User-Agent", "IM-client/OMA1.0 doubango/v1.0.0"),
            TSIP_STACK_SET_NULL());
* @endcode
*
* @sa @ref tsip_stack_create()
*/
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
	else{
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
}

/**@ingroup tsip_stack_group
* Gets user's data, previously set using @ref TSIP_STACK_SET_USERDATA() macro.
* @param self Stack from which to get the user's data.
*/
const void* tsip_stack_get_userdata(const tsip_stack_handle_t *self)
{
	if(self){
		return ((const tsip_stack_t *)self)->userdata;
	}
	else{
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}
}

/**@ingroup tsip_stack_group
* Stops the stack.
* @param self The 3GPP IMS/LTE stack to stop. This handle should be created using @ref tsip_stack_create() and started using tsip_stack_start().
* This function is also called by the garbage collector when the stack is destroyed but you should call it yourself before destroying the stack.<br>
* Before stopping, the stack will hangup all SIP dialogs (starting with non-register dialogs) and destroy all sessions. This is called shutdown phase.
* At the end of this phase, all the SIP sessions will be destroyed.
* @sa @ref tsip_stack_create()<br>@ref tsip_stack_start()
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

		/* Hangup all dialogs starting by REGISTER */	
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

		/* Signal to the end-user that the stack has been stopped 
		* should be done before tsk_runnable_stop() which will stop the thread
		* responsible for the callbacks. The enqueued data have been marked as "important".
		* As both the timer manager and the transport layer have been stoped there is no
		* chance to got additional events */
		if(one_failed){
			TSIP_STACK_SIGNAL(self, tsip_event_code_stack_failed_to_stop, "Stack failed to stop");
		}
		else{
			TSIP_STACK_SIGNAL(self, tsip_event_code_stack_stopped, "Stack stopped");
		}

		/* Stop runnable (run() thread) */
		if((ret = tsk_runnable_stop(TSK_RUNNABLE(stack)))){
			TSK_DEBUG_WARN("Failed to stop the stack");
			one_failed = tsk_true;
		}

		/* Remove all SigComp Compartments */
		TSK_OBJECT_SAFE_FREE(stack->sigcomp.handle);

		if(!one_failed){
			stack->started = tsk_false;
		}

		TSK_DEBUG_INFO("SIP STACK -- STOP");

		return ret;
	}

	return -1;
}

/* internal function used to construct a valid contact URI */
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

/* internal function used to construct a valid Proxy-CSCF URI used as the default first route */
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
						//uri->host_type = ipv6 ? thttp_host_ipv6 : thttp_host_ipv4;
					}
					TSK_FREE(uristring);
				}
				
				return uri;
			}
		}
	}
	return tsk_null;
}










static void *run(void* self)
{
	tsk_list_item_t *curr;
	tsip_stack_t *stack = self;

	TSK_DEBUG_INFO("SIP STACK::run -- START");

	TSK_RUNNABLE_RUN_BEGIN(stack);
	
	if((curr = TSK_RUNNABLE_POP_FIRST(stack))){
		tsip_event_t *sipevent = (tsip_event_t*)curr->data;
		if(stack->callback){
			sipevent->userdata = stack->userdata; // needed by sessionless events
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
		if(stack->started){
			tsip_stack_stop(stack);
		}

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

		TSK_FREE(stack->network.aor.ip);

		TSK_OBJECT_SAFE_FREE(stack->service_routes);
		TSK_OBJECT_SAFE_FREE(stack->associated_uris);
		
		/* SigComp (MUST be done after transports) */
		TSK_OBJECT_SAFE_FREE(stack->sigcomp.handle);

		/* Security(1/1) */
		TSK_FREE(stack->security.secagree_mech);
		TSK_FREE(stack->security.ipsec.alg);
		TSK_FREE(stack->security.ipsec.ealg);
		TSK_FREE(stack->security.ipsec.mode);
		TSK_FREE(stack->security.ipsec.protocol);

		TSK_FREE(stack->security.tls.ca);
		TSK_FREE(stack->security.tls.pbk);
		TSK_FREE(stack->security.tls.pvk);


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
