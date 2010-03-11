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

/**@file thttp_operation.c
 * @brief HTTP/HTTPS operation.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyHTTP/thttp_operation.h"
#include "thttp.h"

#include "tinyHTTP/thttp_message.h"
#include "tinyHTTP/parsers/thttp_parser_url.h"

#include "tinyHTTP/headers/thttp_header_Dummy.h"
#include "tinyHTTP/headers/thttp_header_WWW_Authenticate.h"

#include "tinyHTTP/auth/thttp_challenge.h"

#include "tnet_utils.h"

#include "tsk_debug.h"

#define DEBUG_STATE_MACHINE 1
#define THTTP_MESSAGE_DESCRIPTION(message) \
		THTTP_MESSAGE_IS_RESPONSE(message) ? THTTP_RESPONSE_PHRASE(message) : THTTP_REQUEST_METHOD(message)

typedef struct thttp_operation_s
{
	TSK_DECLARE_OBJECT;

	thttp_operation_id_t id;

	tsk_fsm_t *fsm;

	const thttp_stack_handle_t* stack;
	tsk_params_L_t *params;
	tsk_params_L_t *headers;

	tnet_fd_t fd;
	tsk_buffer_t* buf;

	struct{
		char* username;
		char* password;

		thttp_challenges_L_t *challenges;
	}cred;
}
thttp_operation_t;

/* ======================== internal functions ======================== */
int thttp_operation_OnTerminated(thttp_operation_t *self);
int thttp_operation_SignalMessage(const thttp_operation_handle_t *self, const thttp_message_t* message);
int thttp_operation_update_challenges(thttp_operation_t *self, const thttp_response_t* response);
int thttp_operation_message_new(thttp_operation_handle_t* self, thttp_message_t** message);

/* ======================== external functions ======================== */
extern int thttp_stack_alert(const thttp_stack_handle_t *self, const thttp_event_t* e);

/* ======================== transitions ======================== */
int thttp_operation_Started_2_Transfering_X_perform(va_list *app);
int thttp_operation_Transfering_2_Transfering_X_401_407(va_list *app);
int thttp_operation_Transfering_2_Transfering_X_message(va_list *app); /* Any other HTTP message except 401/407 */
int thttp_operation_Transfering_2_Transfering_X_perform(va_list *app);
int thttp_operation_Any_2_Terminated_X_closed(va_list *app);
int thttp_operation_Any_2_Terminated_X_Error(va_list *app);

/* ======================== conds ======================== */


/* ======================== actions ======================== */
typedef enum _fsm_action_e
{
	_fsm_action_perform,
	_fsm_action_401_407,
	_fsm_action_message,
	_fsm_action_closed,
	_fsm_action_transporterror,
	_fsm_action_error,
}
_fsm_action_t;

/* ======================== states ======================== */
typedef enum _fsm_state_e
{
	_fsm_state_Started,
	_fsm_state_Transfering,
	_fsm_state_Terminated
}
_fsm_state_t;


int __thttp_operation_set(thttp_operation_t *self, va_list values)
{
	thttp_operation_param_type_t curr;

	if(!self){
		return -1;
	}

	while((curr=va_arg(values, thttp_operation_param_type_t)) != optype_null)
	{
		switch(curr)
		{
			case optype_param:
			case optype_header:
				{
					const char* name = va_arg(values, const char *);
					const char* value = va_arg(values, const char *);
					
					if(curr == optype_param){
						tsk_params_add_param(&self->params, name, value);
					} else if(curr == optype_header){
						tsk_params_add_param(&self->headers, name, value);
					}
					break;
				}

			default:
				{
					TSK_DEBUG_ERROR("NOT SUPPORTED.");
					goto bail;
				}
		}
	}

bail:
	return 0;
}

int thttp_operation_set(thttp_operation_handle_t *self, ...)
{
	if(self){
		int ret;
		va_list params;

		thttp_operation_t *operation = self;

		if(operation->id == THTTP_OPERATION_INVALID_ID){
			return -2;
		}
		
		va_start(params, self);
		ret = __thttp_operation_set(operation, params);
		va_end(params);
		return ret;
	}

	return -1;
}

thttp_operation_id_t thttp_operation_get_id(const thttp_operation_handle_t *self)
{
	if(self){
		const thttp_operation_t *operation = self;
		return operation->id;
	}
	return THTTP_OPERATION_INVALID_ID;
}

const tsk_param_t* thttp_operation_get_param(const thttp_operation_handle_t *self, const char* pname)
{
	if(self){
		const thttp_operation_t *operation = self;
		return tsk_params_get_param_by_name(operation->params, pname);
	}
	return THTTP_NULL;
}

const tsk_param_t* thttp_operation_get_header(const thttp_operation_handle_t *self, const char* hname)
{
	if(self){
		const thttp_operation_t *operation = self;
		return tsk_params_get_param_by_name(operation->headers, hname);
	}
	return THTTP_NULL;
}

const tsk_params_L_t* thttp_operation_get_headers(const thttp_operation_handle_t *self)
{
	if(self){
		return ((const thttp_operation_t *)self)->headers;
	}
	return THTTP_NULL;
}

const tsk_params_L_t* thttp_operation_get_params(const thttp_operation_handle_t *self)
{
	if(self){
		return ((const thttp_operation_t *)self)->params;
	}
	return THTTP_NULL;
}

tnet_fd_t thttp_operation_get_fd(const thttp_operation_handle_t *self)
{
	if(self){
		return ((const thttp_operation_t *)self)->fd;
	}
	return TNET_INVALID_FD;
}

tsk_buffer_t* thttp_operation_get_buf(const thttp_operation_handle_t *self)
{
	if(self){
		return ((const thttp_operation_t *)self)->buf;
	}
	return 0;
}

int thttp_operation_set_fd(thttp_operation_handle_t *self, tnet_fd_t fd)
{
	thttp_operation_t* op;

	if(self){
		op = self;
		if(op->fd != TNET_INVALID_FD){
			tnet_sockfd_close(&op->fd);
		}
		op->fd = fd;
	}
	return -1;
}

int thttp_operation_perform(thttp_operation_handle_t* self)
{
	int ret = -1;
	thttp_message_t *message = 0;
	thttp_operation_t* op = self;

	if(!op){
		goto bail;
	}

	if(!(ret = thttp_operation_message_new(self, &message))){
		/* Sends the message. */
		ret = tsk_fsm_act(op->fsm, _fsm_action_perform, op, message, op, message);
	}
	
bail:
	TSK_OBJECT_SAFE_FREE(message);
	return ret;
}







//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------



int thttp_operation_Started_2_Transfering_X_perform(va_list *app)
{
	thttp_operation_t *self = va_arg(*app, thttp_operation_t*);
	const thttp_message_t *message = va_arg(*app, const thttp_message_t *);

	return thttp_stack_send((thttp_stack_handle_t*)self->stack, self, message);
}

int thttp_operation_Transfering_2_Transfering_X_401_407(va_list *app)
{
	int ret;
	thttp_operation_t *self = va_arg(*app, thttp_operation_t*);
	const thttp_response_t *response = va_arg(*app, const thttp_response_t *);
	thttp_message_t *message = 0;

	if((ret = thttp_operation_update_challenges(self, response))){
		// Alert the user.
		TSK_DEBUG_ERROR("HTTP authentication failed.");
		return ret;
	}
	
	/* Retry with creadentials. */
	if(!(ret = thttp_operation_message_new(self, &message))){
		/* Sends the message. */
		ret = thttp_stack_send((thttp_stack_handle_t*)self->stack, self, message);
	}

	TSK_OBJECT_SAFE_FREE(message);

	return ret;
}

int thttp_operation_Transfering_2_Transfering_X_message(va_list *app)
{
	thttp_operation_t *self = va_arg(*app, thttp_operation_t*);
	const thttp_message_t *message = va_arg(*app, const thttp_message_t *);
	thttp_event_t* e = 0;	
	
	/* Alert the user. */
	e = THTTP_EVENT_CREATE(thttp_event_message, self->id, THTTP_MESSAGE_DESCRIPTION(message), message);
	thttp_stack_alert(self->stack, e);
	TSK_OBJECT_SAFE_FREE(e);
	
	return 0;
}

int thttp_operation_Transfering_2_Transfering_X_perform(va_list *app)
{
	thttp_operation_t *self = va_arg(*app, thttp_operation_t*);
	const thttp_message_t *message = va_arg(*app, const thttp_message_t *);

	return thttp_stack_send((thttp_stack_handle_t*)self->stack, self, message);
}

int thttp_operation_Any_2_Terminated_X_closed(va_list *app)
{
	return 0;
}

int thttp_operation_Any_2_Terminated_X_Error(va_list *app)
{
	return 0;
}



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++




int thttp_operation_OnTerminated(thttp_operation_t *self)
{
	TSK_DEBUG_INFO("=== OPERATION terminated ===");
	
	return 0;	
}

int thttp_operation_SignalMessage(const thttp_operation_handle_t *self, const thttp_message_t* message)
{
	const thttp_operation_t* operation = self;
	int ret = -1;

	if(!operation || !message){
		goto bail;
	}

	if(THTTP_RESPONSE_IS(message, 401) || THTTP_RESPONSE_IS(message, 407)){
		ret = tsk_fsm_act(operation->fsm, _fsm_action_401_407, operation, message, operation, message);
	}
	else{
		ret = tsk_fsm_act(operation->fsm, _fsm_action_message, operation, message, operation, message);
	}

bail:
	return ret;
}

int thttp_operation_update_challenges(thttp_operation_t *self, const thttp_response_t* response)
{
	int ret = -1;
	size_t i;

	tsk_list_item_t *item;

	thttp_challenge_t *challenge;
	
	const thttp_header_WWW_Authenticate_t *WWW_Authenticate;
	const thttp_header_Proxy_Authenticate_t *Proxy_Authenticate;

	/* RFC 2617 - Digest Operation

	*	(A) The client response to a WWW-Authenticate challenge for a protection
		space starts an authentication session with that protection space.
		The authentication session lasts until the client receives another
		WWW-Authenticate challenge from any server in the protection space.

		(B) The server may return a 401 response with a new nonce value, causing the client
		to retry the request; by specifying stale=TRUE with this response,
		the server tells the client to retry with the new nonce, but without
		prompting for a new username and password.
	*/

	/* FIXME: As we perform the same task ==> Use only one loop.
	*/

	for(i =0; (WWW_Authenticate = (const thttp_header_WWW_Authenticate_t*)thttp_message_get_headerAt(response, thttp_htype_WWW_Authenticate, i)); i++)
	{
		int isnew = 1;

		tsk_list_foreach(item, self->cred.challenges)
		{
			challenge = item->data;
			if(challenge->isproxy) continue;
			
			if(tsk_strequals(challenge->realm, WWW_Authenticate->realm) && (WWW_Authenticate->stale /*|| acceptNewVector*/))
			{
				/*== (B) ==*/
				if((ret = thttp_challenge_update(challenge, 
					WWW_Authenticate->scheme, 
					WWW_Authenticate->realm, 
					WWW_Authenticate->nonce, 
					WWW_Authenticate->opaque, 
					WWW_Authenticate->algorithm, 
					WWW_Authenticate->qop)))
				{
					return ret;
				}
				else
				{
					isnew = 0;
					continue;
				}
			}
			else return -1;
		}

		if(isnew)
		{
			if((challenge = THTTP_CHALLENGE_CREATE(0, /* Not proxy */
					WWW_Authenticate->scheme, 
					WWW_Authenticate->realm, 
					WWW_Authenticate->nonce, 
					WWW_Authenticate->opaque, 
					WWW_Authenticate->algorithm, 
					WWW_Authenticate->qop)))
			{
				tsk_list_push_back_data(self->cred.challenges, (void**)&challenge);
			}
			else return -1;
		}
	}
	
	for(i=0; (Proxy_Authenticate = (const thttp_header_Proxy_Authenticate_t*)thttp_message_get_headerAt(response, thttp_htype_Proxy_Authenticate, i)); i++)
	{
		int isnew = 1;

		tsk_list_foreach(item, self->cred.challenges)
		{
			challenge = item->data;
			if(!challenge->isproxy) continue;
			
			if(tsk_strequals(challenge->realm, Proxy_Authenticate->realm) && (Proxy_Authenticate->stale /*|| acceptNewVector*/))
			{
				/*== (B) ==*/
				if((ret = thttp_challenge_update(challenge, 
					Proxy_Authenticate->scheme, 
					Proxy_Authenticate->realm, 
					Proxy_Authenticate->nonce, 
					Proxy_Authenticate->opaque, 
					Proxy_Authenticate->algorithm, 
					Proxy_Authenticate->qop)))
				{
					return ret;
				}
				else
				{
					isnew = 0;
					continue;
				}
			}
			else return -1;
		}

		if(isnew)
		{
			if((challenge = THTTP_CHALLENGE_CREATE(1, /* Proxy */
					Proxy_Authenticate->scheme, 
					Proxy_Authenticate->realm, 
					Proxy_Authenticate->nonce,
					Proxy_Authenticate->opaque,
					Proxy_Authenticate->algorithm, 
					Proxy_Authenticate->qop)))
			{
				tsk_list_push_back_data(self->cred.challenges, (void**)&challenge);
			}
			else return -1;
		}
	}	
	return 0;

}

int thttp_operation_message_new(thttp_operation_handle_t* self, thttp_message_t** message)
{
	int ret = -1;
	thttp_operation_t* op;
	const tsk_param_t* param;
	const tsk_list_item_t* item;
	const char* method;

	if(!self){
		goto bail;
	}

	op = self;
	if((method = tsk_params_get_param_value(op->params, "Method"))){ /* REQUEST */
		thttp_url_t* url = 0;
		const char* urlstring;
		if((urlstring = tsk_params_get_param_value(op->params, "Url")) && (url = thttp_url_parse(urlstring, strlen(urlstring)))){
			*message = THTTP_REQUEST_CREATE(method, url);
			TSK_OBJECT_SAFE_FREE(url);
		}
		else{
			TSK_DEBUG_ERROR("MUST supply a valid URI.");
			ret = -2;
			goto bail;
		}
	}
	else{ /* RESPONSE */
	}

	if(!message || !*message || !(*message)->url){ /* Only requests are supported in this version. */
		goto bail;
	}

	/* Add headers associated to the operation. */
	tsk_list_foreach(item, op->headers)
	{
		param = (const tsk_param_t*)item->data;
		if(!param->tag){
			THTTP_MESSAGE_ADD_HEADER(*message, THTTP_HEADER_DUMMY_VA_ARGS(param->name, param->value));
		}
	}

	/* Add creadentials */
	if(THTTP_MESSAGE_IS_REQUEST(*message) && !TSK_LIST_IS_EMPTY(op->cred.challenges))
	{
		thttp_challenge_t *challenge;
		thttp_header_t* auth_hdr;
		tsk_list_foreach(item, op->cred.challenges)
		{
			challenge = item->data;
			auth_hdr = thttp_challenge_create_header_authorization(challenge, "sip:mercuro1@colibria.com", "mercuro1", *message);
			if(auth_hdr){
				thttp_message_add_header(*message, auth_hdr);
				tsk_object_unref(auth_hdr), auth_hdr = 0;
			}
		}
	}

	// all is ok
	ret = 0;

bail:
	return ret;
}








//========================================================
//	HTTP Operation object definition
//
static void* thttp_operation_create(void * self, va_list * app)
{
	thttp_operation_t *operation = self;
	static thttp_operation_id_t unique_id = 0;
	if(operation)
	{
		operation->stack = va_arg(*app, const thttp_stack_handle_t*);
		operation->params = TSK_LIST_CREATE();
		operation->headers = TSK_LIST_CREATE();
		operation->fd = TNET_INVALID_FD;
		operation->buf = TSK_BUFFER_CREATE_NULL();
		operation->cred.challenges = TSK_LIST_CREATE();
		operation->fsm = TSK_FSM_CREATE(_fsm_state_Started, _fsm_state_Terminated);

		if(__thttp_operation_set(self, *app)){
			operation->id = THTTP_OPERATION_INVALID_ID;
		}
		else{
			operation->id = ++unique_id;
		}

		/* init FSM */
		operation->fsm->debug = DEBUG_STATE_MACHINE;
		tsk_fsm_set_callback_terminated(operation->fsm, TSK_FSM_ONTERMINATED(thttp_operation_OnTerminated), operation);
		tsk_fsm_set(operation->fsm,
			
			/*=======================
			* === Started === 
			*/
			// Started -> (perform) -> Trying
			TSK_FSM_ADD_ALWAYS(_fsm_state_Started, _fsm_action_perform, _fsm_state_Transfering, thttp_operation_Started_2_Transfering_X_perform, "thttp_operation_Started_2_Transfering_X_perform"),
			// Started -> (Any) -> Started
			TSK_FSM_ADD_ALWAYS_NOTHING(_fsm_state_Started, "thttp_operation_Started_2_Started_X_any"),
			

			/*=======================
			* === Transfering === 
			*/
			// Transfering -> (401/407) -> Transfering
			TSK_FSM_ADD_ALWAYS(_fsm_state_Transfering, _fsm_action_401_407, _fsm_state_Transfering, thttp_operation_Transfering_2_Transfering_X_401_407, "thttp_operation_Transfering_2_Transfering_X_401_407"),
			// Transfering -> (message) -> Transfering
			TSK_FSM_ADD_ALWAYS(_fsm_state_Transfering, _fsm_action_message, _fsm_state_Transfering, thttp_operation_Transfering_2_Transfering_X_message, "thttp_operation_Transfering_2_Transfering_X_message"),
			// Transfering -> (perform) -> Transfering
			TSK_FSM_ADD_ALWAYS(_fsm_state_Transfering, _fsm_action_perform, _fsm_state_Transfering, thttp_operation_Transfering_2_Transfering_X_perform, "thttp_operation_Transfering_2_Transfering_X_perform"),
			//// Trying -> (300_to_699) -> Terminated
			//TSK_FSM_ADD_ALWAYS(_fsm_state_Trying, _fsm_action_300_to_699, _fsm_state_Terminated, thttp_operation_Trying_2_Terminated_X_300_to_699, "thttp_operation_Trying_2_Terminated_X_300_to_699"),
			//// Trying -> (cancel) -> Terminated
			//TSK_FSM_ADD_ALWAYS(_fsm_state_Trying, _fsm_action_cancel, _fsm_state_Terminated, thttp_operation_Trying_2_Terminated_X_cancel, "thttp_operation_Trying_2_Terminated_X_cancel"),
			//// Trying -> (closed) -> Terminated
			//TSK_FSM_ADD_ALWAYS(_fsm_state_Trying, _fsm_action_closed, _fsm_state_Terminated, thttp_operation_Trying_2_Terminated_X_closed, "thttp_operation_Trying_2_Terminated_X_closed"),
			// Trying -> (Any) -> Trying
			TSK_FSM_ADD_ALWAYS_NOTHING(_fsm_state_Transfering, "thttp_operation_Transfering_2_Transfering_X_any"),


			/*=======================
			* === Any === 
			*/
			// Any -> (closed) -> Terminated
			TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_closed, _fsm_state_Terminated, thttp_operation_Any_2_Terminated_X_closed, "thttp_operation_Any_2_Terminated_X_closed"),
			// Any -> (error) -> Terminated
			TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_error, _fsm_state_Terminated, thttp_operation_Any_2_Terminated_X_Error, "thttp_operation_Any_2_Terminated_X_Error"),

			TSK_FSM_ADD_NULL());

		/* add to the stack */
		thttp_stack_push_op((thttp_stack_handle_t*)operation->stack, operation);
	}

	return self;
}

static void* thttp_operation_destroy(void * self)
{ 
	thttp_operation_t *operation = self;
	if(operation){
		
		/* Remove from the stack */
		thttp_stack_pop_op((thttp_stack_handle_t*)operation->stack, operation);
		
		TSK_OBJECT_SAFE_FREE(operation->params);
		TSK_OBJECT_SAFE_FREE(operation->headers);
		TSK_OBJECT_SAFE_FREE(operation->buf);
		
		TSK_OBJECT_SAFE_FREE(operation->fsm);

		/* cred */
		TSK_FREE(operation->cred.username);
		TSK_FREE(operation->cred.password);
		TSK_OBJECT_SAFE_FREE(operation->cred.challenges);

		tnet_sockfd_close(&operation->fd);
	}
	return self;
}

static int thttp_operation_cmp(const void *obj1, const void *obj2)
{
	const thttp_operation_t *operation1 = obj1;
	const thttp_operation_t *operation2 = obj2;

	if(operation1 && operation2){
		return (int)(operation1->id-operation2->id);
	}
	return -1;
}

static const tsk_object_def_t thttp_operation_def_s = 
{
	sizeof(thttp_operation_t),
	thttp_operation_create, 
	thttp_operation_destroy,
	thttp_operation_cmp, 
};
const void *thttp_operation_def_t = &thttp_operation_def_s;
