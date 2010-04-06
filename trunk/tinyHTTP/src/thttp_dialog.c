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

/**@file thttp_dialog.c
 * @brief HTTP Dialog.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyHTTP/thttp_dialog.h"

#include "thttp.h"

#include "tinyHTTP/thttp_action.h"
#include "tinyHTTP/thttp_session.h"
#include "tinyHTTP/thttp_url.h"
#include "tinyHTTP/parsers/thttp_parser_url.h"

#include "tinyHTTP/headers/thttp_header_Dummy.h"

#include "tnet_utils.h"

#include "tsk_debug.h"

#define DEBUG_STATE_MACHINE 1
#define THTTP_MESSAGE_DESCRIPTION(message) \
		THTTP_MESSAGE_IS_RESPONSE(message) ? THTTP_RESPONSE_PHRASE(message) : THTTP_REQUEST_METHOD(message)

/* ======================== internal functions ======================== */
int thttp_dialog_send_request(thttp_dialog_t *self);
int thttp_dialog_update_timestamp(thttp_dialog_t *self);
int thttp_dialog_OnTerminated(thttp_dialog_t *self);

///* ======================== external functions ======================== */
extern int thttp_stack_alert(const thttp_stack_t *self, const thttp_event_t* e);

/* ======================== transitions ======================== */
int thttp_dialog_Started_2_Transfering_X_request(va_list *app);
int thttp_dialog_Transfering_2_Transfering_X_401_407(va_list *app);
int thttp_dialog_Transfering_2_Terminated_X_message(va_list *app); /* Any other HTTP message except 401/407 */
int thttp_dialog_Any_2_Terminated_X_closed(va_list *app);
int thttp_dialog_Any_2_Terminated_X_Error(va_list *app);

/* ======================== conds ======================== */
tsk_bool_t _fsm_cond_i_401_407(thttp_dialog_t* self, thttp_message_t* message)
{
	return THTTP_MESSAGE_IS_RESPONSE(message) 
		&& (THTTP_RESPONSE_CODE(message) == 401 || THTTP_RESPONSE_CODE(message) == 407);
}
tsk_bool_t _fsm_cond_i_n401_n407(thttp_dialog_t* self, thttp_message_t* message)
{
	return !_fsm_cond_i_401_407(self, message);
}

/* ======================== actions ======================== */
typedef enum _fsm_action_e
{
	_fsm_action_request = atype_o_request,
	_fsm_action_close = atype_close,
	_fsm_action_message = atype_i_message,
	_fsm_action_closed = atype_closed,
	
	_fsm_action_transporterror = 0xFF,
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








//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------

/* Started -> (request) -> Transfering */
int thttp_dialog_Started_2_Transfering_X_request(va_list *app)
{
	thttp_dialog_t *self;
	const thttp_action_t* action;

	self = va_arg(*app, thttp_dialog_t *);
	va_arg(*app, const thttp_message_t *);
	action = va_arg(*app, const thttp_action_t *);

	if(!self->action){
		self->action = tsk_object_ref((void*)action);
	}
	
	return thttp_dialog_send_request(self);
}

/* Transfering -> (401/407) -> Transfering */
int thttp_dialog_Transfering_2_Transfering_X_401_407(va_list *app)
{
	int ret;
	thttp_dialog_t *self;
	const thttp_response_t* response;

	self = va_arg(*app, thttp_dialog_t*);
	response = va_arg(*app, const thttp_response_t *);
	// will use the current action parameters

	if((ret = thttp_session_update_challenges(self->session, response, !self->challenged))){
		thttp_event_t* e = tsk_null;
		TSK_DEBUG_ERROR("HTTP authentication failed.");
		
		if((e = THTTP_EVENT_CREATE(thttp_event_auth_failed, self->session, "Authentication Failed.", tsk_null))){
			thttp_stack_alert(self->session->stack, e);
			TSK_OBJECT_SAFE_FREE(e);
		}

		return ret;
	}
	
	self->challenged = tsk_true;

	/* Retry with creadentials. */
	return thttp_dialog_send_request(self);
}

/* Transfering -> (message) -> Terminated */
int thttp_dialog_Transfering_2_Terminated_X_message(va_list *app)
{
	thttp_dialog_t *self = va_arg(*app, thttp_dialog_t*);
	const thttp_message_t *message = va_arg(*app, const thttp_message_t *);
	thttp_event_t* e = tsk_null;
	int ret = -2;
	
	/* Alert the user. */
	if((e = THTTP_EVENT_CREATE(thttp_event_message, self->session, THTTP_MESSAGE_DESCRIPTION(message), message))){
		ret = thttp_stack_alert(self->session->stack, e);
		TSK_OBJECT_SAFE_FREE(e);
	}
	
	return ret;
}

/* Any -> (closed) -> Terminated */
int thttp_dialog_Any_2_Terminated_X_closed(va_list *app)
{
	int ret = -2;
	thttp_dialog_t *self = va_arg(*app, thttp_dialog_t *);
	thttp_event_t* e = tsk_null;
	//self->fd = TNET_INVALID_FD; // to avoid close(fd) in the destructor

	// alert the user
	/* Alert the user. */
	if((e = THTTP_EVENT_CREATE(thttp_event_closed, self->session, "Connection closed", tsk_null))){
		ret = thttp_stack_alert(self->session->stack, e);
		TSK_OBJECT_SAFE_FREE(e);
	}

	return 0;
}

/* Any -> (error) -> Terminated */
int thttp_dialog_Any_2_Terminated_X_Error(va_list *app)
{
	return 0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int thttp_dialog_fsm_act(thttp_dialog_t* self, tsk_fsm_action_id action_id, const thttp_message_t* message, const thttp_action_t* action)
{
	if(!self || !self->fsm){
		return -1;
	}
	return tsk_fsm_act(self->fsm, action_id, self, message, self, message, action);
}

thttp_dialog_t* thttp_dialog_get_by_session(thttp_dialogs_L_t* dialogs, const struct thttp_session_s* session)
{
	thttp_dialog_t* ret = tsk_null;
	const tsk_list_item_t *item;
	
	if(!dialogs){
		goto bail;
	}
	
	tsk_list_foreach(item, dialogs)
	{
		if(!tsk_object_cmp(((thttp_dialog_t*)item->data)->session, session)){
			ret = tsk_object_ref(item->data);
			goto bail;
		}
	}

bail:
	return ret;
}

// create new dialog and add it to the stack's list of dialogs
// you must free the returned object
thttp_dialog_t* thttp_dialog_new(thttp_session_t* session)
{
	thttp_dialog_t* ret = tsk_null;
	thttp_dialog_t* dialog;
	if(session && session->stack){
		if((dialog = THTTP_DIALOG_CREATE(session))){
			ret = tsk_object_ref(dialog);
			tsk_list_push_back_data(session->dialogs, (void**)&dialog);
		}
	}
	return ret;
}

thttp_dialog_t* thttp_dialog_get_by_age(thttp_dialogs_L_t* dialogs)
{
	thttp_dialog_t* ret = tsk_null;
	thttp_dialog_t* dialog = tsk_null;
	const tsk_list_item_t *item;
	if(dialogs){
		tsk_list_foreach(item, dialogs){
			if(!dialog || (dialog->timestamp >=((thttp_dialog_t*)item->data)->timestamp)){
				dialog = (thttp_dialog_t*)item->data;
			}
		}
		ret = tsk_object_ref(dialog);
	}
	return ret;
}

int thttp_dialog_send_request(thttp_dialog_t *self)
{
	int ret = -1;
	const tsk_list_item_t* item;
	thttp_request_t* request = tsk_null;
	tsk_buffer_t* output = tsk_null;
	thttp_url_t* url;
	tnet_socket_type_t type;

	if(!self || !self->session || !self->action){
		return -1;
	}

	if(!self->action->method || !self->action->url){
		return -2;
	}

	if((url = thttp_url_parse(self->action->url, strlen(self->action->url)))){
		request = THTTP_REQUEST_CREATE(self->action->method, url);
		TSK_OBJECT_SAFE_FREE(url);
	}
	else{
		TSK_DEBUG_ERROR("%s is an invalid HTTP/HTTPS URL.", self->action->url);
		ret = -3;
		goto bail;
	}		

	/* ==Add headers associated to the session== */
	tsk_list_foreach(item, self->session->headers){
		THTTP_MESSAGE_ADD_HEADER(request, THTTP_HEADER_DUMMY_VA_ARGS(TSK_PARAM(item->data)->name, TSK_PARAM(item->data)->value));
	}

	/* ==Add headers associated to the action== */
	if(self->action){
		tsk_list_foreach(item, self->action->headers){
			THTTP_MESSAGE_ADD_HEADER(request, THTTP_HEADER_DUMMY_VA_ARGS(TSK_PARAM(item->data)->name, TSK_PARAM(item->data)->value));
		}
	}

	/* ==Add creadentials== */
	if(!TSK_LIST_IS_EMPTY(self->session->challenges))
	{
		thttp_challenge_t *challenge;
		thttp_header_t* auth_hdr;
		tsk_list_foreach(item, self->session->challenges)
		{
			challenge = item->data;
			if((auth_hdr = thttp_challenge_create_header_authorization(challenge, self->session->cred.usename, self->session->cred.password, request))){
				thttp_message_add_header(request, auth_hdr);
				tsk_object_unref(auth_hdr), auth_hdr = tsk_null;
			}
		}
	}

	/* ==Sends the request== */
	output = TSK_BUFFER_CREATE_NULL();
	type = tnet_transport_get_type(self->session->stack->transport);

	/* Serialize the message and send it */
	if((ret = thttp_message_serialize(request, output))){
		TSK_DEBUG_ERROR("Failed to serialize the HTTP request.");
		goto bail;
	}
	else{
		if(request->url->type == url_https){
			TNET_SOCKET_TYPE_SET_TLS(type);
		}
		else{
			TNET_SOCKET_TYPE_SET_TCP(type);
		}
	}
	
	/* connect to the server not already done */
	if(self->session->fd == TNET_INVALID_FD){
		if((self->session->fd = tnet_transport_connectto(self->session->stack->transport, request->url->host, request->url->port, type)) == TNET_INVALID_FD){
			TSK_DEBUG_ERROR("Failed to connect to %:%d.", request->url->host, request->url->port);
			goto bail;
		}
		/* Wait for the socket for writability */
		if((ret = tnet_sockfd_waitUntilWritable(self->session->fd, TNET_CONNECT_TIMEOUT))){
			TSK_DEBUG_ERROR("%d milliseconds elapsed and the socket is still not connected.", TNET_CONNECT_TIMEOUT);
			if(tnet_transport_remove_socket(self->session->stack->transport, &self->session->fd)){
				tnet_sockfd_close(&self->session->fd);
			}
			goto bail;
		}
	}
	
	if(tnet_transport_send(self->session->stack->transport, self->session->fd, output->data, output->size)){
		TSK_DEBUG_INFO("HTTP/HTTPS message successfully sent.");
		thttp_dialog_update_timestamp(self);
		ret = 0;
	}
	else{
		TSK_DEBUG_INFO("Failed to sent HTTP/HTTPS message.");
		ret = -15;
	}

bail:
	TSK_OBJECT_SAFE_FREE(request);
	TSK_OBJECT_SAFE_FREE(output);

	return ret;
}

int thttp_dialog_update_timestamp(thttp_dialog_t *self)
{
	static uint64_t timestamp = 0;
	if(self){
		self->timestamp = timestamp++;
		return 0;
	}
	return -1;
}

int thttp_dialog_OnTerminated(thttp_dialog_t *self)
{
//	thttp_stack_t* stack;
	int ret = -1;
	TSK_DEBUG_INFO("=== HTTP/HTTPS Dialog terminated ===");

	if(self->session){
		tsk_list_remove_item_by_data(self->session->dialogs, self);
		return 0;
	}

	//if(self && self->session && self->session->stack){
	//	/* Remove from the dialog layer. */
	//	stack = (thttp_stack_t*)self->session->stack;
	//	tsk_safeobj_lock(stack);
	//	tsk_list_remove_item_by_data(stack->dialogs, self);
	//	tsk_safeobj_unlock(stack);
	//	return 0;
	//}

	return -1;
}










//=================================================================================================
//	HTTP Dialog object definition
//
static tsk_object_t* thttp_dialog_create(tsk_object_t * self, va_list * app)
{
	thttp_dialog_t *dialog = self;
	static thttp_dialog_id_t unique_id = 0;
	if(dialog){
		dialog->id = ++unique_id;
		dialog->session = tsk_object_ref(va_arg(*app, thttp_session_t*));
		
		dialog->buf = TSK_BUFFER_CREATE_NULL();

		/* create and init FSM */
		dialog->fsm = TSK_FSM_CREATE(_fsm_state_Started, _fsm_state_Terminated);
		dialog->fsm->debug = DEBUG_STATE_MACHINE;
		tsk_fsm_set_callback_terminated(dialog->fsm, TSK_FSM_ONTERMINATED_F(thttp_dialog_OnTerminated), dialog);
		tsk_fsm_set(dialog->fsm,
			
			/*=======================
			* === Started === 
			*/
			// Started -> (request) -> Transfering
			TSK_FSM_ADD_ALWAYS(_fsm_state_Started, _fsm_action_request, _fsm_state_Transfering, thttp_dialog_Started_2_Transfering_X_request, "thttp_dialog_Started_2_Transfering_X_request"),
			// Started -> (Any) -> Started
			TSK_FSM_ADD_ALWAYS_NOTHING(_fsm_state_Started, "thttp_dialog_Started_2_Started_X_any"),
			

			/*=======================
			* === Transfering === 
			*/
			// Transfering -> (401/407) -> Transfering
			TSK_FSM_ADD(_fsm_state_Transfering, _fsm_action_message, _fsm_cond_i_401_407, _fsm_state_Transfering, thttp_dialog_Transfering_2_Transfering_X_401_407, "thttp_dialog_Transfering_2_Transfering_X_401_407"),
			// Transfering -> (message-non 401/407-) -> Terminated
			TSK_FSM_ADD(_fsm_state_Transfering, _fsm_action_message, _fsm_cond_i_n401_n407, _fsm_state_Terminated, thttp_dialog_Transfering_2_Terminated_X_message, "thttp_dialog_Transfering_2_Terminated_X_message"),
			/*=======================
			* === Any === 
			*/
			
			// Any -> (closed) -> Terminated
			TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_closed, _fsm_state_Terminated, thttp_dialog_Any_2_Terminated_X_closed, "thttp_dialog_Any_2_Terminated_X_closed"),
			// Any -> (error) -> Terminated
			TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_error, _fsm_state_Terminated, thttp_dialog_Any_2_Terminated_X_Error, "thttp_dialog_Any_2_Terminated_X_Error"),

			TSK_FSM_ADD_NULL());

		thttp_dialog_update_timestamp(self);
	}
	return self;
}

static tsk_object_t* thttp_dialog_destroy(tsk_object_t * self)
{ 
	thttp_dialog_t *dialog = self;
	if(dialog){
		TSK_DEBUG_INFO("*** HTTP/HTTPS Dialog destroyed ***");

		TSK_OBJECT_SAFE_FREE(dialog->fsm);
	
		TSK_OBJECT_SAFE_FREE(dialog->session);
		TSK_OBJECT_SAFE_FREE(dialog->action);

		TSK_OBJECT_SAFE_FREE(dialog->buf);
	}

	return self;
}

static int thttp_dialog_cmp(const tsk_object_t *_d1, const tsk_object_t *_d2)
{
	const thttp_dialog_t *d1 = _d1;
	const thttp_dialog_t *d2 = _d2;

	if(d1 && d2){
		return (int)(d1->id-d2->id);
	}
	return -1;
}

static const tsk_object_def_t thttp_dialog_def_s = 
{
	sizeof(thttp_dialog_t),
	thttp_dialog_create, 
	thttp_dialog_destroy,
	thttp_dialog_cmp, 
};
const tsk_object_def_t *thttp_dialog_def_t = &thttp_dialog_def_s;

