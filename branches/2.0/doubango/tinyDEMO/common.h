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
#if !defined(TINYDEMO_COMMON_H)
#define TINYDEMO_COMMON_H

#include "demo_config.h"

#include "cmd.h"

#include "tinysip.h" /* 3GPP IMS/LTE API */

_BEGIN_DECLS

typedef enum session_type_e
{
	st_none,

	st_invite,
	st_message,
	st_options,
	st_publish,
	st_register,
	st_subscribe,
}
session_type_t;

typedef struct session_s
{
	TSK_DECLARE_OBJECT;
	
	tsip_ssession_handle_t* handle;
	
	session_type_t type;
	char* from;
	char* to;
	tsk_bool_t connected;
}
session_t;
typedef tsk_list_t sessions_L_t;
#define SESSION(self) ((session_t*)(self))

typedef struct ctx_s
{
	TSK_DECLARE_OBJECT;

	tsip_stack_handle_t *stack;

	/* Identity */
	struct{
		char* display_name;
		char *impu;
		char *preferred;
		char *impi;
		char *password;
	} identity;

	/* Network */
	struct{
		char *local_ip;
		tnet_port_t local_port;

		char *proxy_cscf;
		tnet_port_t proxy_cscf_port;
		char* proxy_cscf_trans;
		
		char *realm;

		tsk_bool_t ipv6;
		tsk_bool_t naptr;
		tsk_bool_t dhcp;
	} network;

	/* Security */
	struct{
		tsk_bool_t earlyIMS;
		char* operator_id;
		uint16_t amf;
	} security;
	
	sessions_L_t* sessions;
	tsk_params_L_t* params;
	
	TSK_DECLARE_SAFEOBJ; /* For thread-safeness */
}
ctx_t;

ctx_t* ctx_create();

int stack_dump();
int stack_config(const opts_L_t* opts);
int stack_run(const opts_L_t* opts);

session_t* session_create(session_type_t, tsip_ssession_handle_t* );
#define session_client_create(type) session_create(type, tsk_null)
#define session_server_create(type, handle) session_create(type, handle)

const session_t* session_get_by_sid(const sessions_L_t* , tsip_ssession_id_t );
int session_tostring(const session_t* );
int session_hangup(tsip_ssession_id_t sid);

tsip_action_handle_t* action_get_config(const opts_L_t* opts);

const tsk_object_def_t *session_def_t;
const tsk_object_def_t *ctx_def_t;

_END_DECLS

#endif /* TINYDEMO_COMMON_H */
