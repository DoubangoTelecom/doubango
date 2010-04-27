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
#	ifdef ANDROID /* On the emulator */
#		define DEFAULT_LOCAL_IP	"10.0.2.15"
#	else
#		define DEFAULT_LOCAL_IP	TNET_SOCKET_HOST_ANY
#	endif
#endif
#define DEFAULT_USER_AGENT	"IM-client/OMA1.0 doubango/v1.0.0"

extern context_t* context;

int stack_callback(const tsip_event_t *sipevent);
int session_tostring(const session_t* session);

/* our SIP callback function */
int stack_callback(const tsip_event_t *sipevent)
{
	if(!sipevent){ /* should never happen ...but who know? */
		TSK_DEBUG_WARN("Null SIP event.");
		return -1;
	}

	/* check if it's for our stack */
	if(sipevent->stack != context->stack){
		TSK_DEBUG_ERROR("We have received an event for another stack");
		return -2;
	}

	switch(sipevent->type){
		case tsip_event_register:
			{	/* REGISTER */
				return register_handle_event(sipevent);
			}
		case tsip_event_invite:
			{	/* INVITE */
				return invite_handle_event(sipevent);
			}
		case tsip_event_message:
			{	/* MESSAGE */
				return message_handle_event(sipevent);
			}
		case tsip_event_publish:
			{ /* PUBLISH */
				return publish_handle_event(sipevent);
			}
		case tsip_event_subscribe:
			{	/* SUBSCRIBE */
				return subscribe_handle_event(sipevent);
			}

		default:
			{	/* UNsupported */
				TSK_DEBUG_WARN("%d not supported as SIP event.", sipevent->type);
				return -3;
			}
	}
	return 0;
}

/*	==================================================================
	========================== Context ================================= 
*/

static tsk_object_t* context_ctor(tsk_object_t * self, va_list * app)
{
	context_t *context = self;
	if(context){
		/* stack */
		context->stack = tsip_stack_create(stack_callback, DEFAULT_REALM, DEFAULT_IMPI, DEFAULT_IMPU, /* Mandatory parameters */
			
			TSIP_STACK_SET_HEADER("User-Agent", DEFAULT_USER_AGENT), /* SIP 'User-Agent' stack-level header. */
			TSIP_STACK_SET_LOCAL_IP(DEFAULT_LOCAL_IP), /* local IP */
			TSIP_STACK_SET_NULL() /* Mandatory */);

		/* SIP Sessions */
		context->sessions = tsk_list_create();

		/* init internal mutex */
		tsk_safeobj_init(context);
	}
	return self;
}

static tsk_object_t* context_dtor(tsk_object_t * self)
{ 
	context_t *context = self;
	if(context){
		/* stack */
		TSK_OBJECT_SAFE_FREE(context->stack);

		/* deinit internal mutex */
		tsk_safeobj_deinit(context);
	}

	return self;
}

static const tsk_object_def_t context_def_s = 
{
	sizeof(context_t),
	context_ctor, 
	context_dtor,
	tsk_null, 
};
const tsk_object_def_t *context_def_t = &context_def_s;


/*	==================================================================
	========================== Stack ================================= 
*/

int stack_dump()
{
	const tsk_list_item_t* item;

	tsk_list_foreach(item, context->sessions){
		session_tostring(item->data);
	}
	return 0;
}

int stack_config(const tsk_options_L_t* options)
{
	const tsk_list_item_t* item;
	const tsk_option_t* option;
	int ret = 0;
	tsk_param_t* param;

	if(!options){
		return -1;
	}

	tsk_list_foreach(item, options){
		option = item->data;

		switch(option->id){
			case opt_amf:
				{
					break;
				}
			case opt_dhcpv4:
				{
					break;
				}
			case opt_dhcpv6:
				{
					break;
				}
			case opt_dname:
				{
					break;
				}
			case opt_dns_naptr:
				{
					break;
				}
			case opt_header:
				{
					if((param = tsk_params_parse_param(option->value, tsk_strlen(option->value)))){
						ret = tsip_stack_set(context->stack, TSIP_STACK_SET_HEADER(param->name, param->value),
							TSIP_STACK_SET_NULL());
						TSK_OBJECT_SAFE_FREE(param);
					}
					break;
				}
			case opt_impi:
				{
					tsk_strupdate(&context->identity.impi, option->value);
					ret = tsip_stack_set(context->stack, TSIP_STACK_SET_IMPI(context->identity.impi),
							TSIP_STACK_SET_NULL());
					break;
				}
			case opt_impu:
				{
					tsk_strupdate(&context->identity.impu, option->value);
					ret = tsip_stack_set(context->stack, TSIP_STACK_SET_IMPU(context->identity.impu),
							TSIP_STACK_SET_NULL());
					break;
				}
			case opt_ipv6:
				{
					break;
				}
			case opt_local_ip:
				{
					break;
				}
			case opt_local_port:
				{
					break;
				}
			case opt_opid:
				{
					break;
				}
			case opt_password:
				{
					ret = tsip_stack_set(context->stack, TSIP_STACK_SET_PASSWORD(option->value),
							TSIP_STACK_SET_NULL());
					break;
				}
			case opt_pcscf_ip:
				{
					break;
				}
			case opt_pcscf_port:
				{
					break;
				}
			case opt_pcscf_trans:
				{
					break;
				}
			case opt_realm:
				{
					ret = tsip_stack_set(context->stack,
						TSIP_STACK_SET_REALM(option->value),
						TSIP_STACK_SET_NULL());
					break;
				}

		}/* switch */
	} /* foreach */
	
	return ret;
}

int stack_run(const tsk_options_L_t* options)
{
	if(!context->stack){
		TSK_DEBUG_ERROR("Stack is Null.");
		return -1;
	}
	else{
		return tsip_stack_start(context->stack);
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

int session_tostring(const session_t* session)
{
	char* temp = tsk_null;
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
		printf(" from=%s", session->from ? session->from : context->identity.impu);
		/* From */
		printf(" to=%s", session->to ? session->to : context->identity.impu);
	}
	else{
		printf("(invalid)");
	}
	printf("\n");
	return -1;
}

session_t*  session_handle_cmd(cmd_type_t cmd, const tsk_options_L_t* options)
{
	session_t* session = tsk_null;
	const tsk_option_t* option;
	const tsk_list_item_t* item;
	tsk_param_t* param;
	int ret = 0;

	/* Check if there is a session with is Id */
	if((option = tsk_options_get_option_by_id(options, opt_sid))){
		tsip_ssession_id_t id = atoi(option->value);
		if((item = tsk_list_find_item_by_pred(context->sessions, pred_find_session_by_id, &id))){
			session = tsk_object_ref(item->data);
		}
	}

#define TYPE_FROM_CMD(_CMD) \
	/*(_CMD==cmd_invite ? st_invite :*/  \
	(_CMD==cmd_message ? st_message : \
	(_CMD==cmd_publish ? st_publish : \
	(_CMD==cmd_register ? st_register : \
	(_CMD==cmd_subscribe ? st_subscribe : st_none))))
	
	/* === Command === */
	switch(cmd){
		//case cmd_invite:
		case cmd_message:
		case cmd_publish:
		case cmd_register:
		case cmd_subscribe:
			{
				if(!session){ /* Create subscription */
					session_t* _session = session_create(TYPE_FROM_CMD(cmd));
					session = tsk_object_ref(_session);
					tsk_list_push_back_data(context->sessions, (void**)&_session);
				}
				break;
			}
		default:
			{
				TSK_DEBUG_WARN("Registration: Cannot handle this command [%d]", cmd);
				goto bail;
			}
	} /* switch */

	if(!session){
		TSK_DEBUG_ERROR("SIP Session is Null");
		goto bail;
	}

	/* === Options === */
	tsk_list_foreach(item, options){
		option = item->data;
		
		switch(option->id){
			case opt_expires:
				{	
					if(!tsk_strnullORempty(option->value)){
						ret = tsip_ssession_set(session->handle, 
							TSIP_SSESSION_SET_OPTION(TSIP_SSESSION_OPTION_EXPIRES, option->value),
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
					if((param = tsk_params_parse_param(option->value, tsk_strlen(option->value)))){
						ret = tsip_ssession_set(session->handle, 
							TSIP_SSESSION_SET_HEADER(param->name, param->value),
							TSIP_SSESSION_SET_NULL());
						TSK_OBJECT_SAFE_FREE(param);
					}
					break;
				}
			case opt_to:
				{	/* You should use  TSIP_SSESSION_SET_OPTION(TSIP_SSESSION_OPTION_TO, value)
						instead of TSIP_SSESSION_SET_HEADER() to set the destination URI. */
					if(!tsk_strnullORempty(option->value)){
						ret = tsip_ssession_set(session->handle, 
							TSIP_SSESSION_SET_OPTION(TSIP_SSESSION_OPTION_TO, option->value),
							TSIP_SSESSION_SET_NULL());
					}
					break;
				}
			default:
				{
					TSK_DEBUG_WARN("Subscription: %d not valid option.");
					break;
				}
		}

	} /* foreach */

bail:
	return session;
}

static tsk_object_t* session_ctor(tsk_object_t * self, va_list * app)
{
	session_t *session = self;
	if(session){
		session->type = va_arg(*app, session_type_t);
		session->handle = tsip_ssession_create(context->stack,
			TSIP_SSESSION_SET_NULL());
	}
	return self;
}

static tsk_object_t* session_dtor(tsk_object_t * self)
{ 
	session_t *session = self;
	if(session){
		
	}

	return self;
}

static const tsk_object_def_t session_def_s = 
{
	sizeof(session_t),
	session_ctor, 
	session_dtor,
	tsk_null, 
};
const tsk_object_def_t *session_def_t = &session_def_s;
