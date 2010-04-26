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
#define DEFAULT_REALM	"ims.inexbee.com"
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

/* === callback function for the stack === */
int stack_callback(const tsip_event_t *sipevent);


/* Find SIP session by id */
int pred_find_session_by_id(const tsk_list_item_t *item, const void* id)
{
	if(item && item->data){
		return (int)(tsip_ssession_get_id((tsip_ssession_handle_t*)item->data) 
			- *((tsip_ssession_id_t*)id));
	}
	return -1;
}

/* our SIP callback function */
int stack_callback(const tsip_event_t *sipevent)
{
	if(!sipevent){ /* should never happen ...but who know? */
		TSK_DEBUG_WARN("Null SIP event.");
		return -1;
	}

	/* check if it's for our stack */
	if(sipevent->stack != context->stack->ims_stack){
		TSK_DEBUG_ERROR("We have received an event for another stack");
		return -2;
	}

	switch(sipevent->type){
		case tsip_event_register:
			{	/* REGISTER */
				return register_handle_event(context, sipevent);
			}
		case tsip_event_invite:
			{	/* INVITE */
				return invite_handle_event(context, sipevent);
			}
		case tsip_event_message:
			{	/* MESSAGE */
				return message_handle_event(context, sipevent);
			}
		case tsip_event_publish:
			{ /* PUBLISH */
				return publish_handle_event(context, sipevent);
			}
		case tsip_event_subscribe:
			{	/* SUBSCRIBE */
				return subscribe_handle_event(context, sipevent);
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
		context->stack = tsk_object_new(stack_def_t);
		context->stack->ims_stack = tsip_stack_create(stack_callback, DEFAULT_REALM, DEFAULT_IMPI, DEFAULT_IMPU, /* Mandatory parameters */
			
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

int stack_config(tsk_options_L_t* options)
{
	const tsk_list_item_t* item;
	const tsk_option_t* option;

	if(!options){
		return -1;
	}

	tsk_list_foreach(item, options){
		option = item->data;
		

	//,			/* --dhcpv4 */
	//,			/* --dhcpv6 */
	//,			/* --dname bob */
	//opt_dns_naptr,		/* --dns-naptr */
	//opt_from,			/* --from sip:alice@open-ims.test */
	//opt_header,			/* --header Supported: norefersub */
	//opt_impi,			/* --impi bob@open-ims.test */
	//opt_impu,			/* --impu sip:bob@open-ims.test */
	//opt_ipv6,			/* --ipv6 */
	//opt_local_ip,		/* --local-ip 192.168.0.10 */
	//opt_local_port,		/* --local-port 4000 */
	//opt_opid,			/* --opid 0xA712F5D04B */
	//opt_password,		/* --password mysecret */
	//opt_pcscf_ip,		/* --pcscf-ip 192.168.0.13 */
	//opt_pcscf_port,		/* --pcscf-port 5060 */
	//opt_pcscf_trans,	/* --pcscf-trans udp */
	//opt_quit,			/* --quit */
	//opt_realm,			/* --realm open-ims.test */
	//opt_to,				/* --to sip:alice@open-ims.test */	

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
					break;
				}
			case opt_impi:
				{
					break;
				}
			case opt_impu:
				{
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
					break;
				}

		}/* switch */
	} /* foreach */
	
	return 0;
}

int stack_run(tsk_options_L_t* options)
{
	if(!context->stack->ims_stack){
		TSK_DEBUG_ERROR("Stack is Null.");
		return -1;
	}
	else{
		return tsip_stack_start(context->stack->ims_stack);
	}
}

static tsk_object_t* stack_ctor(tsk_object_t * self, va_list * app)
{
	stack_t *stack = self;
	if(stack){
		
	}
	return self;
}

static tsk_object_t* stack_dtor(tsk_object_t * self)
{ 
	stack_t *stack = self;
	if(stack){
		
	}

	return self;
}

static const tsk_object_def_t stack_def_s = 
{
	sizeof(stack_t),
	stack_ctor, 
	stack_dtor,
	tsk_null, 
};
const tsk_object_def_t *stack_def_t = &stack_def_s;
