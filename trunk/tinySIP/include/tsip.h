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

/**@file tsip.h
 * @brief SIP (RFC 3261) and 3GPP IMS (TS 24.229) implementation.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_TSIP_H
#define TINYSIP_TSIP_H

#include "tinysip_config.h"

#include "tsk_runnable.h"

#include <stdint.h>

typedef uint8_t amf_t[2];
typedef uint8_t operator_id_t[16];

typedef int (*tsip_stack_callback)(struct tsip_event_s *sipevent);

typedef enum tsip_stack_param_type_e
{
	/* Identity */
	pname_uri,
	pname_preferred_uri,
	pname_user_name,
	pname_password,
#define TSIP_STACK_SET_URI(STR)							pname_uri, (const char*)STR
#define TSIP_STACK_SET_PREFERRED_URI(STR)				pname_preferred_uri, (const char*)STR
#define TSIP_STACK_SET_USER_NAME(STR)					pname_user_name, (const char*)STR
#define TSIP_STACK_SET_PASSWORD(STR)					pname_password, (const char*)STR

	/* Network */
	pname_local_ip,
	pname_local_port,
	pname_enable_ipv6,
	pname_privacy,
	pname_operator_id,
	pname_amf,
	pname_netinfo,
	pname_realm,
	pname_proxy,
#define TSIP_STACK_SET_LOCAL_IP(STR)					pname_local_ip, (const char*)STR
#define TSIP_STACK_SET_LOCAL_PORT(UINT16)				pname_local_port, (uint16_t)UINT16
#define TSIP_STACK_SET_ENABLE_IPV6(INT)					pname_enable_ipv6, (int)INT
#define TSIP_STACK_SET_PRIVACY(STR)						pname_privacy, (const char*)STR
#define TSIP_STACK_SET_POPERATOR_ID(OPERATOR_ID)		pname_privacy, (operator_id_t)OPERATOR_ID
#define TSIP_STACK_SET_AMF(AMF)							pname_amf, (amf_t)AMF
#define TSIP_STACK_SET_NETINFO(STR)						pname_netinfo, (const char*)STR
#define TSIP_STACK_SET_REALM(STR)						pname_realm, (const char*)STR
#define TSIP_STACK_SET_PROXY(STR)						pname_proxy, (const char*)STR

	/* Services */
	pname_enable_100rel,
	pname_enable_gsmais,
	pname_enable_precond,
	pname_enable_3gppsms,
	pname_enable_gsmarcs,
	pname_enable_earlyIMS,
	pname_enable_ofdr,
	pname_enable_aa,
	pname_enable_dnd,
	pname_enable_option,

	/* QoS */

	pname_null,
#define TSIP_STACK_SET_NULL()							pname_null
}
tsip_stack_param_type_t;

typedef void tsip_stack_handle_t;

typedef struct tsip_event_s
{
	TSK_DECLARE_OBJECT;

	const tsip_stack_handle_t * stack;
	short status_code;
	char *reason_phrase;
}
tsip_event_t;
TINYSIP_API const void *tsip_event_def_t;

TINYSIP_API tsip_stack_handle_t *tsip_stack_create(tsip_stack_callback callback, ...);
TINYSIP_API int tsip_stack_start(tsip_stack_handle_t *self);
TINYSIP_API int tsip_stack_set(tsip_stack_handle_t *self, ...);
TINYSIP_API int tsip_stack_stop(tsip_stack_handle_t *self);
TINYSIP_API int tsip_stack_destroy(tsip_stack_handle_t *self);


#define TSIP_STACK_EVENT_RAISE(stack, status_code, reason_phrase) \
	TSK_RUNNABLE_ENQUEUE(TSK_RUNNABLE(stack), (const tsip_stack_handle_t*)stack, (short)status_code, (const char*)reason_phrase);

#endif /* TINYSIP_TSIP_H */