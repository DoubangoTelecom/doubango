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
#include "common.h"

#include "invite.h"
#include "message.h"
#include "options.h"
#include "publish.h"
#include "register.h"
#include "subscribe.h"

#include <stdio.h>
#include <string.h>

/* === default values === */
#define DEFAULT_REALM	"open-ims.test"
#define DEFAULT_IMPI "bob@"DEFAULT_REALM
#define DEFAULT_IMPU "sip:bob@"DEFAULT_REALM

#ifndef DEFAULT_LOCAL_IP
//#	ifdef ANDROID /* On the emulator */
//#		define DEFAULT_LOCAL_IP	"10.0.2.15"
//#	else
#		define DEFAULT_LOCAL_IP	TNET_SOCKET_HOST_ANY
//#	endif
#endif

extern ctx_t* ctx;

int stack_callback(const tsip_event_t *sipevent);
int session_handle_event(const tsip_event_t *sipevent);
int session_tostring(const session_t* session);

/* our SIP callback function */
int stack_callback(const tsip_event_t *sipevent)
{
	int ret;

	if(!sipevent){ /* should never happen ...but who know? */
		TSK_DEBUG_WARN("Null SIP event.");
		return -1;
	}

	/* check if it's for our stack */
	if(sipevent->stack != ctx->stack){
		TSK_DEBUG_ERROR("We have received an event for another stack");
		return  -2;
	}

	tsk_safeobj_lock(ctx);

	switch(sipevent->type){
		case tsip_event_register:
			{	/* REGISTER */
				ret = register_handle_event(sipevent);
				break;
			}
		case tsip_event_invite:
			{	/* INVITE */
				ret = invite_handle_event(sipevent);
				break;
			}
		case tsip_event_message:
			{	/* MESSAGE */
				ret = message_handle_event(sipevent);
				break;
			}
		case tsip_event_options:
			{ /* OPTIONS */
				ret = options_handle_event(sipevent);
				break;
			}
		case tsip_event_publish:
			{ /* PUBLISH */
				ret = publish_handle_event(sipevent);
				break;
			}
		case tsip_event_subscribe:
			{	/* SUBSCRIBE */
				ret = subscribe_handle_event(sipevent);
				break;
			}

		case tsip_event_dialog:
			{	/* Common to all dialogs */
				ret = session_handle_event(sipevent);
				break;
			}

		default:
			{	/* Unsupported */
				TSK_DEBUG_WARN("%d not supported as SIP event.", sipevent->type);
				ret = -3;
				break;
			}
	}

	tsk_safeobj_unlock(ctx);

	return ret;
}

/*	==================================================================
	========================== Context ================================= 
*/

ctx_t* ctx_create()
{
	return tsk_object_new(ctx_def_t);
}

static tsk_object_t* ctx_ctor(tsk_object_t * self, va_list * app)
{
	ctx_t *ctx = self;
	if(ctx){
		/* stack */
		ctx->stack = tsip_stack_create(stack_callback, DEFAULT_REALM, DEFAULT_IMPI, DEFAULT_IMPU, /* Mandatory parameters */
			
			TSIP_STACK_SET_LOCAL_IP(DEFAULT_LOCAL_IP), /* local IP */
			TSIP_STACK_SET_NULL() /* Mandatory */);

		/* SIP Sessions */
		ctx->sessions = tsk_list_create();

		/* user's parameters */
		ctx->params = tsk_list_create();

		/* init internal mutex */
		tsk_safeobj_init(ctx);
	}
	return self;
}

static tsk_object_t* ctx_dtor(tsk_object_t * self)
{ 
	ctx_t *ctx = self;
	if(ctx){
		/* Stop the stack (as sessions are alive, you will continue to receive callbacks)*/
		tsip_stack_stop(ctx->stack);

		/* sessions : should be freed before the stack as explained on the Programmer's Guide 
		* As all dialogs have been hanged up, the list should be empty ...but who know?*/
		TSK_OBJECT_SAFE_FREE(ctx->sessions);

		/* Destroy the stack */
		TSK_OBJECT_SAFE_FREE(ctx->stack);

		/* Identity */
		TSK_FREE(ctx->identity.display_name);
		TSK_FREE(ctx->identity.impu);
		TSK_FREE(ctx->identity.preferred);
		TSK_FREE(ctx->identity.impi);
		TSK_FREE(ctx->identity.password);

		/* Network */
		TSK_FREE(ctx->network.local_ip);
		TSK_FREE(ctx->network.proxy_cscf);
		TSK_FREE(ctx->network.proxy_cscf_trans);
		TSK_FREE(ctx->network.realm);

		/* Security */
		TSK_FREE(ctx->security.operator_id);

		/* Params */
		TSK_OBJECT_SAFE_FREE(ctx->params);

		/* deinit internal mutex */
		tsk_safeobj_deinit(ctx);
	}

	return self;
}

static const tsk_object_def_t ctx_def_s = 
{
	sizeof(ctx_t),
	ctx_ctor, 
	ctx_dtor,
	tsk_null, 
};
const tsk_object_def_t *ctx_def_t = &ctx_def_s;


/*	==================================================================
	========================== Stack ================================= 
*/

int stack_dump()
{
	const tsk_list_item_t* item;

	tsk_list_foreach(item, ctx->sessions){
		session_tostring(item->data);
	}
	return 0;
}

int stack_config(const opts_L_t* opts)
{
	const tsk_list_item_t* item;
	const opt_t* opt;
	int ret = 0;
	tsk_param_t* param;
	tsk_bool_t pcscf_changed = tsk_false;

	if(!opts){
		return -1;
	}

	tsk_list_foreach(item, opts){
		opt = item->data;
		
		/* Stack-level option */
		if(opt->lv != lv_none && opt->lv != lv_stack){
			continue;
		}

		switch(opt->type){
			case opt_amf:
				{
					break;
				}
			case opt_dhcpv4:
			case opt_dhcpv6:
				{
					tsip_stack_set(ctx->stack,
						TSIP_STACK_SET_DISCOVERY_DHCP(tsk_true),
						TSIP_STACK_SET_NULL());
					break;
				}
			case opt_dname:
				{
					break;
				}
			case opt_dns_naptr:
				{
					tsip_stack_set(ctx->stack,
						TSIP_STACK_SET_DISCOVERY_NAPTR(tsk_true),
						TSIP_STACK_SET_NULL());
					break;
				}
			case opt_header:
				{
					if((param = tsk_params_parse_param(opt->value, tsk_strlen(opt->value)))){
						ret = tsip_stack_set(ctx->stack, 
							TSIP_STACK_SET_HEADER(param->name, param->value),
							TSIP_STACK_SET_NULL());
						TSK_OBJECT_SAFE_FREE(param);
					}
					break;
				}
			case opt_impi:
				{
					tsk_strupdate(&ctx->identity.impi, opt->value);
					ret = tsip_stack_set(ctx->stack, 
							TSIP_STACK_SET_IMPI(ctx->identity.impi),
							TSIP_STACK_SET_NULL());
					break;
				}
			case opt_impu:
				{
					tsk_strupdate(&ctx->identity.impu, opt->value);
					ret = tsip_stack_set(ctx->stack, 
							TSIP_STACK_SET_IMPU(ctx->identity.impu),
							TSIP_STACK_SET_NULL());
					break;
				}
			case opt_ipv6:
				{
					pcscf_changed = tsk_true;
					ctx->network.ipv6 = tsk_true;
					break;
				}
			case opt_local_ip:
				{
					ret = tsip_stack_set(ctx->stack, 
							TSIP_STACK_SET_LOCAL_IP(opt->value),
							TSIP_STACK_SET_NULL());
					break;
				}
			case opt_local_port:
				{
					unsigned port = (unsigned)atoi(opt->value);
					ret = tsip_stack_set(ctx->stack, 
							TSIP_STACK_SET_LOCAL_PORT(port),
							TSIP_STACK_SET_NULL());
					break;
				}
			case opt_opid:
				{
					break;
				}
			case opt_password:
				{
					ret = tsip_stack_set(ctx->stack, 
							TSIP_STACK_SET_PASSWORD(opt->value),
							TSIP_STACK_SET_NULL());
					break;
				}
			case opt_pcscf_ip:
				{
					pcscf_changed = tsk_true;
					tsk_strupdate(&ctx->network.proxy_cscf, opt->value);
					break;
				}
			case opt_pcscf_port:
				{
					pcscf_changed = tsk_true;
					ctx->network.proxy_cscf_port = atoi(opt->value);
					break;
				}
			case opt_pcscf_trans:
				{
					pcscf_changed = tsk_true;
					tsk_strupdate(&ctx->network.proxy_cscf_trans, opt->value);
					break;
				}
			case opt_realm:
				{
					ret = tsip_stack_set(ctx->stack,
						TSIP_STACK_SET_REALM(opt->value),
						TSIP_STACK_SET_NULL());
					break;
				}

		}/* switch */

	} /* foreach */

	/* whether Proxy-CSCF config has changed */
	if(pcscf_changed){
		ret = tsip_stack_set(ctx->stack,
			TSIP_STACK_SET_PROXY_CSCF(ctx->network.proxy_cscf, ctx->network.proxy_cscf_port, ctx->network.proxy_cscf_trans, ctx->network.ipv6 ? "ipv6" : "ipv4"),
			TSIP_STACK_SET_NULL());
	}
	
	return ret;
}

int stack_run(const opts_L_t* opts)
{
	if(!ctx->stack){
		TSK_DEBUG_ERROR("Stack is Null.");
		return -1;
	}
	else{
		return tsip_stack_start(ctx->stack);
	}
}

/*	==================================================================
	========================== Session ================================= 
*/

/* Find SIP session by id */
int pred_find_session_by_id(const tsk_list_item_t *item, const void* id)
{
	const session_t* session;
	if(item && item->data){
		session = item->data;
		return (int)(tsip_ssession_get_id(session->handle) 
			- *((tsip_ssession_id_t*)id));
	}
	return -1;
}

session_t* session_create(session_type_t type)
{
	return tsk_object_new(session_def_t, type);
}

const session_t* session_get_by_sid(const sessions_L_t* sessions, tsip_ssession_id_t sid)
{
	const tsk_list_item_t* item;
	if((item = tsk_list_find_item_by_pred(sessions, pred_find_session_by_id, &sid))){
		return item->data;
	}
	else{
		return tsk_null;
	}
}


int session_tostring(const session_t* session)
{
	//char* temp = tsk_null;
	printf("== Session: ");
	if(session){
		/* Session Id */
		printf("sid=%llu", tsip_ssession_get_id(session->handle));
		/* Type */
		printf(" type=");
		switch(session->type){
			case st_invite:
				printf("INVITE");
				break;
			case st_message:
				printf("MESSAGE");
				break;
			case st_publish:
				printf("PUBLISH");
				break;
			case st_register:
				printf("REGISTER");
				break;
			case st_subscribe:
				printf("SUBSCRIBE");
				break;
			default:
				printf("(null)");
				break;
		}
		/* From */
		printf(" from=%s", session->from ? session->from : ctx->identity.impu);
		/* From */
		printf(" to=%s", session->to ? session->to : ctx->identity.impu);
	}
	else{
		printf("(invalid)");
	}
	printf("\n");
	return -1;
}


/* handle events -common to all sessions */
int session_handle_event(const tsip_event_t *sipevent)
{
	const session_t* session;

	/* Find associated session */
	if(!(session = session_get_by_sid(ctx->sessions, tsip_ssession_get_id(sipevent->ss)))){
		TSK_DEBUG_WARN("Failed to match session event.");
		return -1;
	}
	switch(sipevent->code)
	{
		/* === 7xx ==> errors ===  */
	case tsip_event_code_transport_error:
	case tsip_event_code_global_error:
	case tsip_event_code_message_error:
		break;

		/* === 8xx ==> success ===  */
	case tsip_event_code_request_incoming:
	case tsip_event_code_request_cancelled:
	case tsip_event_code_request_sent:
		break;

		/* === 9xx ==> Informational ===  */
	case tsip_event_code_dialog_terminated:
		{	/* we no longer need the session 
			* -> remove and destroy the session */
			tsk_list_remove_item_by_data(ctx->sessions, session);
			break;
		}
	}
	return 0;
}

/* handle commands -common to all sessions */
const session_t*  session_handle_cmd(cmd_type_t cmd, const opts_L_t* opts)
{
	const session_t* session = tsk_null;
	const opt_t* opt;
	const tsk_list_item_t* item;
	tsk_param_t* param;
	int ret = 0;

	/* Check if there is a session with is Id */
	if((opt = opt_get_by_type(opts, opt_sid))){
		tsip_ssession_id_t sid = atoi(opt->value);
			session = session_get_by_sid(ctx->sessions, sid);
	}

#define TYPE_FROM_CMD(_CMD) \
	((_CMD==cmd_audio || _CMD==cmd_video || _CMD==cmd_audiovideo || _CMD==cmd_file || _CMD==cmd_large_message) ? st_invite :  \
	((_CMD==cmd_message || _CMD==cmd_sms) ? st_message : \
	(_CMD==cmd_options ? st_options : \
	(_CMD==cmd_publish ? st_publish : \
	(_CMD==cmd_register ? st_register : \
	(_CMD==cmd_subscribe ? st_subscribe : st_none))))))
	
	/* === Command === */
	switch(cmd){
		//case cmd_invite:
		case cmd_message:
		case cmd_sms:
		case cmd_options:
		case cmd_publish:
		case cmd_register:
		case cmd_subscribe:
			{
				if(!session){ /* Create subscription */
					session_t* _session = session_create(TYPE_FROM_CMD(cmd));
					session = _session;
					tsk_list_push_back_data(ctx->sessions, (void**)&_session);
				}
				break;
			}
		default:
			{
				TSK_DEBUG_WARN("Session handling: Cannot handle this command [%d]", cmd);
				goto bail;
			}
	} /* switch */

	if(!session){
		TSK_DEBUG_ERROR("SIP Session is Null");
		goto bail;
	}

	/* === Options === */
	tsk_list_foreach(item, opts){
		opt = item->data;

		/* Session-level option? */
		if(opt->lv != lv_none && opt->lv != lv_session){
			continue;
		}
		
		switch(opt->type){
			case opt_caps:
				{
					if(!tsk_strnullORempty(opt->value)){
						if((param = tsk_params_parse_param(opt->value, tsk_strlen(opt->value)))){
							ret = tsip_ssession_set(session->handle, 
								TSIP_SSESSION_SET_CAPS(param->name, param->value),
								TSIP_SSESSION_SET_NULL());
							TSK_OBJECT_SAFE_FREE(param);
						}
					}
					break;
				}
			case opt_expires:
				{	
					if(!tsk_strnullORempty(opt->value)){
						ret = tsip_ssession_set(session->handle, 
							TSIP_SSESSION_SET_OPTION(TSIP_SSESSION_OPTION_EXPIRES, opt->value),
							TSIP_SSESSION_SET_NULL());
					}
					break;
				}
			case opt_from:
				{	/* You should use  TSIP_SSESSION_SET_OPTION(TSIP_SSESSION_OPTION_FROM, value)
						instead of TSIP_SSESSION_SET_HEADER() to set the destination URI. */
					break;
				}
			case opt_header:
				{
					if((param = tsk_params_parse_param(opt->value, tsk_strlen(opt->value)))){
						ret = tsip_ssession_set(session->handle, 
							TSIP_SSESSION_SET_HEADER(param->name, param->value),
							TSIP_SSESSION_SET_NULL());
						TSK_OBJECT_SAFE_FREE(param);
					}
					break;
				}
			case opt_payload:
				{	/* Will be handled by the caller */
					break;
				}
			case opt_to:
				{	/* You should use  TSIP_SSESSION_SET_OPTION(TSIP_SSESSION_OPTION_TO, value)
						instead of TSIP_SSESSION_SET_HEADER() to set the destination URI. */
					if((cmd != cmd_sms) && !tsk_strnullORempty(opt->value)){ /* SMS will use SMSC Address as Request URI */
						ret = tsip_ssession_set(session->handle, 
							TSIP_SSESSION_SET_OPTION(TSIP_SSESSION_OPTION_TO, opt->value),
							TSIP_SSESSION_SET_NULL());
					}
					break;
				}
			default:
				{
					/* will be handled by the caller */
					break;
				}
		}

	} /* foreach */

bail:
	return session;
}

int session_hangup(tsip_ssession_id_t sid)
{
	const session_t* session;
	if((session = session_get_by_sid(ctx->sessions, sid))){
		switch(session->type){
			case st_invite:
				break;
			case st_message:
				break;
			case st_publish:
				break;
			case st_register:
				tsip_action_UNREGISTER(session->handle,
					/* You can add your parameters */
					TSIP_ACTION_SET_NULL());
				break;
			case st_subscribe:
				tsip_action_UNSUBSCRIBE(session->handle,
					/* You can add your parameters */
					TSIP_ACTION_SET_NULL());
				break;
			default:
				TSK_DEBUG_WARN("Cannot hangup session with this type [%d]", session->type);
				return -2;
		}
		return 0;
	}
	else{
		TSK_DEBUG_WARN("Failed to find session with sid=%llu", sid);
		return -1;
	}
}

static tsk_object_t* session_ctor(tsk_object_t * self, va_list * app)
{
	session_t *session = self;
	if(session){
		session->type = va_arg(*app, session_type_t);
		session->handle = tsip_ssession_create(ctx->stack,
			TSIP_SSESSION_SET_NULL());
	}
	return self;
}

static tsk_object_t* session_dtor(tsk_object_t * self)
{ 
	session_t *session = self;
	if(session){
		TSK_OBJECT_SAFE_FREE(session->handle);

		TSK_FREE(session->to);
		TSK_FREE(session->from);
	}

	return self;
}

static int session_cmp(const tsk_object_t *_ss1, const tsk_object_t *_ss2)
{
	const session_t *ss1 = _ss1;
	const session_t *ss2 = _ss2;

	if(ss1 && ss1){
		if(ss1->handle && ss2->handle){
			return tsk_object_cmp(ss1->handle, ss2->handle);
		}
		else{
			return (ss2 - ss1);
		}
	}
	else if(!ss1 && !ss2) return 0;
	else return -1;
}

static const tsk_object_def_t session_def_s = 
{
	sizeof(session_t),
	session_ctor, 
	session_dtor,
	session_cmp, 
};
const tsk_object_def_t *session_def_t = &session_def_s;



tsip_action_handle_t* action_get_config(const opts_L_t* opts)
{
	const opt_t* opt;
	const tsk_list_item_t* item;
	tsip_action_handle_t* action_config = tsk_null;
	tsk_param_t* param;

	if(TSK_LIST_IS_EMPTY(opts)){
		return tsk_null;
	}

	tsk_list_foreach(item, opts){
		opt = item->data;
		
		/* action level? */
		if(opt->lv != lv_action){
			continue;
		}

		/* create new action */
		if(!action_config && !(action_config = tsip_action_create(atype_config,
			TSIP_ACTION_SET_NULL()))) break;

		switch(opt->type){
			case opt_header:
				{
					if((param = tsk_params_parse_param(opt->value, tsk_strlen(opt->value)))){
						tsip_action_set(action_config, 
							TSIP_ACTION_SET_HEADER(param->name, param->value),
							TSIP_ACTION_SET_NULL());
						TSK_OBJECT_SAFE_FREE(param);
					}
					break;
				}
			case opt_payload:
				{
					tsip_action_set(action_config,
						TSIP_ACTION_SET_PAYLOAD(opt->value, tsk_strlen(opt->value)),
						TSIP_ACTION_SET_NULL());
					break;
				}
			default:
				{
					break;
				}
		}
	}

	return action_config;
}
