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

/**@file tsip.h
 * @brief SIP (RFC 3261) and 3GPP IMS/LTE (TS 24.229) implementation.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_TSIP_H
#define TINYSIP_TSIP_H

#include "tinySIP_config.h"

#include "tinySIP/tsip_ssession.h"
#include "tinySIP/tsip_timers.h"
#include "tinySIP/tsip_event.h"

#include "tinySIP/tsip_uri.h"

#include "tnet_socket.h"
#include "dns/tnet_dns.h"

#include "tsk_runnable.h"

TSIP_BEGIN_DECLS

#define TSIP_IARI_GSMAIS				"urn%3Aurn-7%3A3gpp-application.ims.iari.gsma-is"
#define TSIP_IARI_QUOTED_GSMAIS			"\""TSIP_IARI_GSMAIS"\""
#define TSIP_ICSI_MMTEL_PSVOICE			"urn%3Aurn-7%3A3gpp-service.ims.icsi.mmtel"
#define TSIP_ICSI_QUOTED_MMTEL_PSVOICE	"\""TSIP_ICSI_MMTEL_PSVOICE"\""

#define TSIP_STACK(self)		((tsip_stack_t*)(self))

typedef uint8_t amf_t[2];
typedef uint8_t operator_id_t[16];


typedef enum tsip_stack_param_type_e
{
	pname_null = tsk_null,
#define TSIP_STACK_SET_NULL()							pname_null

	pname_option,

	/* Identity */

	/* Network */
	pname_proxy_cscf,
#define TSIP_STACK_SET_PRIVACY(STR)													pname_privacy, (const char*)STR
#define TSIP_STACK_SET_POPERATOR_ID(OPERATOR_ID)									pname_privacy, (operator_id_t)OPERATOR_ID

	
	/* Security */
	pname_amf,
	pname_operator_id,
	pname_secagree_ipsec,
	pname_secagree_tls,
	pname_tls_certs,
#define TSIP_STACK_SET_SECAGREE_IPSEC(ALG_STR, EALG_STR, MODE_STR, PROTOCOL_STR)	pname_secagree_ipsec, (const char*)ALG_STR, (const char*)EALG_STR, (const char*)MODE_STR, (const char*)PROTOCOL_STR
#define TSIP_STACK_SET_SECAGREE_TLS(USE_TLS_SECAGREE_INT)							pname_secagree_tls, (int)USE_TLS_SECAGREE_INT
#define TSIP_STACK_SET_TLS_CERTS(CA_FILE_STR, PUB_FILE_STR, PRIV_FILE_STR)			pname_tls_certs, (const char*)CA_FILE_STR, (const char*)PUB_FILE_STR, (const char*)PRIV_FILE_STR

	///* Features */
	//pname_enable_100rel,
	//pname_enable_gsmais,
	//pname_enable_precond,
	//pname_enable_3gppsms,
	//pname_enable_gsmarcs,
	//pname_enable_earlyIMS,
	//pname_enable_ofdr,
	//pname_enable_aa,
	//pname_enable_dnd,
	//pname_enable_option,

	/* QoS */

	pname_header,

}
tsip_stack_param_type_t;

typedef enum tsip_stack_option_e
{
	/* Identity */
	TSIP_STACK_OPTION_DISPLAY_NAME,
	TSIP_STACK_OPTION_IMPU,
	TSIP_STACK_OPTION_PREFERRED_IDENTITY,
	TSIP_STACK_OPTION_IMPI,
	TSIP_STACK_OPTION_PASSWORD,

	/* Network */
	TSIP_STACK_OPTION_REALM,
	TSIP_STACK_OPTION_LOCAL_IP,
	TSIP_STACK_OPTION_LOCAL_PORT,
	TSIP_STACK_OPTION_DISCOVERY_NAPTR,
	TSIP_STACK_OPTION_DISCOVERY_DHCP,

	/* Security */
	TSIP_STACK_OPTION_EARLY_IMS,
	//TSIP_STACK_OPTION_IMS_AKA_AMF,
	//TSIP_STACK_OPTION_IMS_AKA_OPERATOR_ID,
	TSIP_STACK_OPTION_SECAGREE_IPSEC,
	TSIP_STACK_OPTION_SECAGREE_TLS,
	

	/* Features */


}
tsip_stack_option_t;

/* === Options === */
#define TSIP_STACK_SET_OPTION(ID_ENUM, VALUE_STR)		pname_option, (tsip_stack_option_t)ID_ENUM, (const char*)VALUE_STR

/* === Identity === */
#define TSIP_STACK_SET_DISPLAY_NAME(NAME_STR)			TSIP_STACK_SET_OPTION(TSIP_STACK_OPTION_DISPLAY_NAME, NAME_STR)
#define TSIP_STACK_SET_IMPU(URI_STR)					TSIP_STACK_SET_OPTION(TSIP_STACK_OPTION_IMPU, URI_STR)
#define TSIP_STACK_SET_PREFERRED_IDENTITY(URI_STR)		TSIP_STACK_SET_OPTION(TSIP_STACK_OPTION_PREFERRED_IDENTITY, URI_STR)
#define TSIP_STACK_SET_IMPI(IMPI_STR)					TSIP_STACK_SET_OPTION(TSIP_STACK_OPTION_IMPI, IMPI_STR)
#define TSIP_STACK_SET_PASSWORD(PASSORD_STR)			TSIP_STACK_SET_OPTION(TSIP_STACK_OPTION_PASSWORD, PASSORD_STR)

/* === Network === */
#define TSIP_STACK_SET_REALM(URI_STR)					TSIP_STACK_SET_OPTION(TSIP_STACK_OPTION_REALM, URI_STR)
#define TSIP_STACK_SET_LOCAL_IP(IP_STR)					TSIP_STACK_SET_OPTION(TSIP_STACK_OPTION_LOCAL_IP, IP_STR)
#define TSIP_STACK_SET_LOCAL_PORT(PORT_UINT)			TSIP_STACK_SET_OPTION(TSIP_STACK_OPTION_LOCAL_PORT, #PORT_UINT)
#define TSIP_STACK_SET_DISCOVERY_NAPTR(ENABLED_BOOL)	TSIP_STACK_SET_OPTION(TSIP_STACK_OPTION_DISCOVERY_NAPTR, #ENABLED_BOOL)
#define TSIP_STACK_SET_DISCOVERY_DHCP(ENABLED_BOOL)		TSIP_STACK_SET_OPTION(TSIP_STACK_OPTION_DISCOVERY_DHCP, #ENABLED_BOOL)

#define TSIP_STACK_SET_PROXY_CSCF(FQDN_STR, PORT_UINT, TRANSPORT_STR, IP_VERSION_STR)			pname_proxy_cscf, (const char*)FQDN_STR, (unsigned)PORT_UINT, (const char*)TRANSPORT_STR, (const char*)IP_VERSION_STR

#define QUOTEME_(x) #x
#define QUOTEME(x) QUOTEME_(x)

/* === Security === */
#define TSIP_STACK_SET_EARLY_IMS(ENABLED_BOOL)				TSIP_STACK_SET_OPTION(TSIP_STACK_OPTION_EARLY_IMS, #ENABLED_BOOL)
#define TSIP_STACK_SET_IMS_AKA_AMF(AMF_UINT16)				pname_amf, (uint16_t)AMF_UINT16
#define TSIP_STACK_SET_IMS_AKA_OPERATOR_ID(OPID_HEX_STR)	pname_operator_id, (const char*)OPID_HEX_STR

/* === Headers === */
#define TSIP_STACK_SET_HEADER(NAME_STR, VALUE_STR)		pname_header, (const char*)NAME_STR, (const char*)VALUE_STR
#define TSIP_STACK_UNSET_HEADER(NAME_STR)				TSIP_STACK_SET_HEADER(NAME_STR, ((const char*)-1))

/* IMS/LTE stack (for internal use). only tsip_stack_handle_t should be visible. */
typedef struct tsip_stack_s
{
	TSK_DECLARE_RUNNABLE;
	
	tsip_stack_callback_f callback;

	/* Identity */
	struct{
		char* display_name;
		tsip_uri_t *impu;
		tsip_uri_t *preferred;
		char *impi;
		char *password;
	} identity;

	/* Network */
	struct{
		char *local_ip;
		tnet_port_t local_port;

		char *proxy_cscf;
		tnet_port_t proxy_cscf_port;
		tnet_socket_type_t proxy_cscf_type;
		
		tsip_uri_t *realm;

		tsk_bool_t discovery_naptr;
		tsk_bool_t discovery_dhcp;
	} network;

	/* Security */
	struct{
		tsk_bool_t earlyIMS;
		operator_id_t operator_id;
		amf_t amf;
	} security;
	
	
	
	
	
	
	
	
	tsip_uris_L_t* paths;
	tsip_uris_L_t* service_routes;
	tsip_uris_L_t* associated_uris;

	/* Security */
	char* secagree_mech;
	unsigned enable_secagree_tls:1;
	unsigned enable_secagree_ipsec:1;
	struct{
		char* alg;
		char* ealg;
		char* mode;
		char* protocol;
	} secagree_ipsec;
	struct {
		char* ca;
		char* pbk;
		char* pvk;
	}tls;

	/* DNS context */
	tnet_dns_ctx_t *dns_ctx;

	/* DHCP context */

	

	/* QoS */

	/* Internals. */
	tsk_timer_manager_handle_t* timer_mgr;
	tsip_timers_t timers;
	tsip_ssessions_L_t *ssessions;
	tsk_params_L_t *headers;

	/* Layers */
	struct tsip_dialog_layer_s *layer_dialog;
	struct tsip_transac_layer_s *layer_transac;
	struct tsip_transport_layer_s *layer_transport;
}
tsip_stack_t;




TINYSIP_API tsip_stack_handle_t *tsip_stack_create(tsip_stack_callback_f callback, const char* realm_uri, const char* impi_uri, const char* impu_uri, ...);
TINYSIP_API int tsip_stack_start(tsip_stack_handle_t *self);
TINYSIP_API int tsip_stack_set(tsip_stack_handle_t *self, ...);
TINYSIP_API int tsip_stack_stop(tsip_stack_handle_t *self);

#define TSIP_STACK_EVENT_RAISE(stack, status_code, reason_phrase, incoming, type) \
	TSK_RUNNABLE_ENQUEUE(TSK_RUNNABLE(stack), (const tsip_stack_handle_t*)stack, (short)status_code, (const char*)reason_phrase, (unsigned)incoming, (tsip_event_type_t)type);


TINYSIP_GEXTERN const tsk_object_def_t *tsip_stack_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_TSIP_H */
