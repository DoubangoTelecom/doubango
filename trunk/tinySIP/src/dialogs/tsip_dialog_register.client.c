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

/**@file tsip_dialog_register.client.c
 * @brief SIP dialog register (Client side).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/dialogs/tsip_dialog_register.h"
#include "tinySIP/parsers/tsip_parser_uri.h"

#include "tinySIP/transports/tsip_transport_layer.h"

#include "tinySIP/headers/tsip_header_Path.h"
#include "tinySIP/headers/tsip_header_P_Associated_URI.h"
#include "tinySIP/headers/tsip_header_Min_Expires.h"
#include "tinySIP/headers/tsip_header_Service_Route.h"
#include "tinySIP/headers/tsip_header_Supported.h"

#include "tinySIP/api/tsip_api_register.h"

#include "tsk_memory.h"
#include "tsk_debug.h"
#include "tsk_time.h"

#define DEBUG_STATE_MACHINE											1
#define TSIP_DIALOG_REGISTER_TIMER_SCHEDULE(TX)						TSIP_DIALOG_TIMER_SCHEDULE(register, TX)
#define TSIP_DIALOG_REGISTER_SIGNAL(self, type, code, phrase, message)	\
	tsip_register_event_signal(type, TSIP_DIALOG_GET_STACK(self), TSIP_DIALOG(self)->operation, code, phrase, message)


/* ======================== internal functions ======================== */
int send_register(tsip_dialog_register_t *self, tsk_bool_t initial);
int tsip_dialog_register_OnTerminated(tsip_dialog_register_t *self);

/* ======================== transitions ======================== */
int tsip_dialog_register_Started_2_Trying_X_send(va_list *app);
int tsip_dialog_register_Trying_2_Trying_X_1xx(va_list *app);
int tsip_dialog_register_Trying_2_Terminated_X_2xx(va_list *app);
int tsip_dialog_register_Trying_2_Connected_X_2xx(va_list *app);
int tsip_dialog_register_Trying_2_Trying_X_401_407_421_494(va_list *app);
int tsip_dialog_register_Trying_2_Trying_X_423(va_list *app);
int tsip_dialog_register_Trying_2_Terminated_X_300_to_699(va_list *app);
int tsip_dialog_register_Trying_2_Terminated_X_cancel(va_list *app);
int tsip_dialog_register_Trying_2_Trying_X_NOTIFY(va_list *app);
int tsip_dialog_register_Connected_2_Trying_X_unregister(va_list *app);
int tsip_dialog_register_Connected_2_Trying_X_refresh(va_list *app);
int tsip_dialog_register_Connected_2_Connected_X_NOTIFY(va_list *app);
int tsip_dialog_register_Connected_2_Terminated_X_NOTIFY(va_list *app);
int tsip_dialog_register_Any_2_Trying_X_hangup(va_list *app);
int tsip_dialog_register_Any_2_Terminated_X_transportError(va_list *app);
int tsip_dialog_register_Any_2_Terminated_X_Error(va_list *app);


/* ======================== conds ======================== */
int _fsm_cond_unregistering(tsip_dialog_register_t* dialog, tsip_message_t* message)
{
	return dialog->unregistering ? 1 : 0;
}
int _fsm_cond_registering(tsip_dialog_register_t* dialog, tsip_message_t* message)
{
	return !_fsm_cond_unregistering(dialog, message);
}

/* ======================== actions ======================== */
typedef enum _fsm_action_e
{
	_fsm_action_send,
	_fsm_action_1xx,
	_fsm_action_2xx,
	_fsm_action_401_407_421_494,
	_fsm_action_423,
	_fsm_action_300_to_699,
	_fsm_action_cancel,
	_fsm_action_unregister,
	_fsm_action_refresh,
	_fsm_action_hangup,
	_fsm_action_transporterror,
	_fsm_action_error,
}
_fsm_action_t;

/* ======================== states ======================== */
typedef enum _fsm_state_e
{
	_fsm_state_Started,
	_fsm_state_Trying,
	_fsm_state_Connected,
	_fsm_state_Terminated
}
_fsm_state_t;

/**
 * @fn	int tsip_dialog_register_event_callback(const tsip_dialog_register_t *self, tsip_dialog_event_type_t type,
 * 		const tsip_message_t *msg)
 *
 * @brief	Callback function called to alert the dialog for new events from the transaction/transport layers.
 *
 * @author	Mamadou
 * @date	1/4/2010
 *
 * @param [in,out]	self	A reference to the dialog.
 * @param	type		The event type. 
 * @param [in,out]	msg	The incoming SIP/IMS message. 
 *
 * @return	Zero if succeed and non-zero error code otherwise. 
**/
int tsip_dialog_register_event_callback(const tsip_dialog_register_t *self, tsip_dialog_event_type_t type, const tsip_message_t *msg)
{
	int ret = -1;

	switch(type)
	{
	case tsip_dialog_i_msg:
		{
			if(msg && TSIP_MESSAGE_IS_RESPONSE(msg))
			{
				//
				//	RESPONSE
				//
				if(TSIP_RESPONSE_IS_1XX(msg)){
					ret = tsk_fsm_act(self->fsm, _fsm_action_1xx, self, msg, self, msg);
				}
				else if(TSIP_RESPONSE_IS_2XX(msg)){
					ret = tsk_fsm_act(self->fsm, _fsm_action_2xx, self, msg, self, msg);
				}
				else if(TSIP_RESPONSE_IS(msg,401) || TSIP_RESPONSE_IS(msg,407) || TSIP_RESPONSE_IS(msg,421) || TSIP_RESPONSE_IS(msg,494)){
					ret = tsk_fsm_act(self->fsm, _fsm_action_401_407_421_494, self, msg, self, msg);
				}
				else if(TSIP_RESPONSE_IS(msg,423)){
					ret = tsk_fsm_act(self->fsm, _fsm_action_423, self, msg, self, msg);
				}
				else{
					// Alert User
					ret = tsk_fsm_act((self)->fsm, _fsm_action_error, self, msg, self, msg);
					TSK_DEBUG_WARN("Not supported status code: %d", TSIP_RESPONSE_CODE(msg));
				}
			}
			else
			{
				//
				//	REQUEST
				//
			}
			break;
		}

	case tsip_dialog_hang_up:
	case tsip_dialog_shuttingdown:
		{
			ret = tsk_fsm_act(self->fsm, _fsm_action_hangup, self, msg, self, msg, (tsk_bool_t)(type == tsip_dialog_shuttingdown));
			break;
		}

	case tsip_dialog_canceled:
		{
			ret = tsk_fsm_act((self)->fsm, _fsm_action_cancel, self, msg, self, msg);
			break;
		}

	case tsip_dialog_terminated:
	case tsip_dialog_timedout:
	case tsip_dialog_error:
	case tsip_dialog_transport_error:
		{
			ret = tsk_fsm_act((self)->fsm, _fsm_action_transporterror, self, msg, self, msg);
			break;
		}
	}

	return ret;
}

/**
 * @fn	int tsip_dialog_register_timer_callback(const tsip_dialog_register_t* self,
 * 		tsk_timer_id_t timer_id)
 *
 * @brief	Timer manager callback.
 *
 * @author	Mamadou
 * @date	1/5/2010
 *
 * @param [in,out]	self	The owner of the signaled timer. 
 * @param	timer_id		The identifier of the signaled timer.
 *
 * @return	Zero if succeed and non-zero error code otherwise.  
**/
int tsip_dialog_register_timer_callback(const tsip_dialog_register_t* self, tsk_timer_id_t timer_id)
{
	int ret = -1;

	if(self)
	{
		if(timer_id == self->timerrefresh.id){
			ret = tsk_fsm_act((self)->fsm, _fsm_action_refresh, self, tsk_null, self, tsk_null);
		}
		else if(timer_id == self->timershutdown.id){
			ret = tsk_fsm_act((self)->fsm, _fsm_action_error, self, tsk_null, self, tsk_null);
		}
	}
	return ret;
}

/**
 * @fn	int tsip_dialog_register_init(tsip_dialog_register_t *self)
 *
 * @brief	Initializes the dialog.
 *
 * @author	Mamadou
 * @date	1/4/2010
 *
 * @param [in,out]	self	The dialog to initialize. 
**/
int tsip_dialog_register_init(tsip_dialog_register_t *self)
{
	/* Initialize the state machine. */
	tsk_fsm_set(self->fsm,
			
			/*=======================
			* === Started === 
			*/
			// Started -> (Send) -> Trying
			TSK_FSM_ADD_ALWAYS(_fsm_state_Started, _fsm_action_send, _fsm_state_Trying, tsip_dialog_register_Started_2_Trying_X_send, "tsip_dialog_register_Started_2_Trying_X_send"),
			// Started -> (Any) -> Started
			TSK_FSM_ADD_ALWAYS_NOTHING(_fsm_state_Started, "tsip_dialog_register_Started_2_Started_X_any"),
			

			/*=======================
			* === Trying === 
			*/
			// Trying -> (1xx) -> Trying
			TSK_FSM_ADD_ALWAYS(_fsm_state_Trying, _fsm_action_1xx, _fsm_state_Trying, tsip_dialog_register_Trying_2_Trying_X_1xx, "tsip_dialog_register_Trying_2_Trying_X_1xx"),
			// Trying -> (2xx) -> Terminated
			TSK_FSM_ADD(_fsm_state_Trying, _fsm_action_2xx, _fsm_cond_unregistering, _fsm_state_Terminated, tsip_dialog_register_Trying_2_Terminated_X_2xx, "tsip_dialog_register_Trying_2_Terminated_X_2xx"),
			// Trying -> (2xx) -> Connected
			TSK_FSM_ADD(_fsm_state_Trying, _fsm_action_2xx, _fsm_cond_registering, _fsm_state_Connected, tsip_dialog_register_Trying_2_Connected_X_2xx, "tsip_dialog_register_Trying_2_Connected_X_2xx"),
			// Trying -> (401/407/421/494) -> Trying
			TSK_FSM_ADD_ALWAYS(_fsm_state_Trying, _fsm_action_401_407_421_494, _fsm_state_Trying, tsip_dialog_register_Trying_2_Trying_X_401_407_421_494, "tsip_dialog_register_Trying_2_Trying_X_401_407_421_494"),
			// Trying -> (423) -> Trying
			TSK_FSM_ADD_ALWAYS(_fsm_state_Trying, _fsm_action_423, _fsm_state_Trying, tsip_dialog_register_Trying_2_Trying_X_423, "tsip_dialog_register_Trying_2_Trying_X_423"),
			// Trying -> (300_to_699) -> Terminated
			TSK_FSM_ADD_ALWAYS(_fsm_state_Trying, _fsm_action_300_to_699, _fsm_state_Terminated, tsip_dialog_register_Trying_2_Terminated_X_300_to_699, "tsip_dialog_register_Trying_2_Terminated_X_300_to_699"),
			// Trying -> (cancel) -> Terminated
			TSK_FSM_ADD_ALWAYS(_fsm_state_Trying, _fsm_action_cancel, _fsm_state_Terminated, tsip_dialog_register_Trying_2_Terminated_X_cancel, "tsip_dialog_register_Trying_2_Terminated_X_cancel"),
			// Trying -> (Any) -> Trying
			TSK_FSM_ADD_ALWAYS_NOTHING(_fsm_state_Trying, "tsip_dialog_register_Trying_2_Trying_X_any"),


			/*=======================
			* === Connected === 
			*/
			// Connected -> (unregister) -> Trying
			TSK_FSM_ADD_ALWAYS(_fsm_state_Connected, _fsm_action_unregister, _fsm_state_Trying, tsip_dialog_register_Connected_2_Trying_X_unregister, "tsip_dialog_register_Connected_2_Trying_X_unregister"),
			// Connected -> (refresh) -> Trying
			TSK_FSM_ADD_ALWAYS(_fsm_state_Connected, _fsm_action_refresh, _fsm_state_Trying, tsip_dialog_register_Connected_2_Trying_X_refresh, "tsip_dialog_register_Connected_2_Trying_X_refresh"),

			/*=======================
			* === Any === 
			*/
			// Any -> (transport error) -> Terminated
			TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_transporterror, _fsm_state_Terminated, tsip_dialog_register_Any_2_Terminated_X_transportError, "tsip_dialog_register_Any_2_Terminated_X_transportError"),
			// Any -> (error) -> Terminated
			TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_error, _fsm_state_Terminated, tsip_dialog_register_Any_2_Terminated_X_Error, "tsip_dialog_register_Any_2_Terminated_X_Error"),
			// Any -> (hangup) -> Trying
			TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_hangup, _fsm_state_Trying, tsip_dialog_register_Any_2_Trying_X_hangup, "tsip_dialog_register_Any_2_Trying_X_hangup"),

			TSK_FSM_ADD_NULL());

	/* Sets callback function */
	TSIP_DIALOG(self)->callback = TSIP_DIALOG_EVENT_CALLBACK(tsip_dialog_register_event_callback);
	
	/* Timers */
	self->timerrefresh.id = TSK_INVALID_TIMER_ID;
	self->timerrefresh.timeout = TSIP_DIALOG(self)->expires;
	self->timershutdown.id = TSK_INVALID_TIMER_ID;
	self->timershutdown.timeout = TSIP_DIALOG_SHUTDOWN_TIMEOUT;

	return 0;
}

/**
 * @fn	int tsip_dialog_register_start(tsip_dialog_register_t *self)
 *
 * @brief	Starts the dialog state machine.
 *
 * @author	Mamadou
 * @date	1/4/2010
 *
 * @param [in,out]	self	The dialog to start. 
 *
 * @return	Zero if succeed and non-zero error code otherwise. 
**/
int tsip_dialog_register_start(tsip_dialog_register_t *self)
{
	int ret = -1;
	if(self && !TSIP_DIALOG(self)->running)
	{
		/* Send request */
		ret = tsk_fsm_act(self->fsm, _fsm_action_send, self, tsk_null, self, tsk_null);
	}
	return ret;
}


//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------

/* Started -> (send) -> Trying
*/
int tsip_dialog_register_Started_2_Trying_X_send(va_list *app)
{
	tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
	const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

	TSIP_DIALOG(self)->running = tsk_true;

	return send_register(self, tsk_true);
}

/* Trying -> (1xx) -> Trying
*/
int tsip_dialog_register_Trying_2_Trying_X_1xx(va_list *app)
{
	tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
	const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

	return tsip_dialog_update(TSIP_DIALOG(self), message);
}

/* Trying -> (2xx) -> Connected
*/
//#include "tsk_thread.h"
int tsip_dialog_register_Trying_2_Connected_X_2xx(va_list *app)
{
	tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
	const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

	/* Alert the user. */
	TSIP_DIALOG_REGISTER_SIGNAL(self, tsip_ao_register, 
		TSIP_RESPONSE_CODE(message), TSIP_RESPONSE_PHRASE(message), message);

	/*	- Set P-associated-uriS
	*	- Update service-routes
	*	- Update Pats
	*/
	{
		size_t index;
		const tsip_header_Path_t *hdr_Path;
		const tsip_header_Service_Route_t *hdr_Service_Route;
		const tsip_header_P_Associated_URI_t *hdr_P_Associated_URI_t;
		tsip_uri_t *uri;

		/* To avoid memory leaks ==> delete all concerned objects (it worth nothing) */
		TSK_OBJECT_SAFE_FREE(TSIP_DIALOG_GET_STACK(self)->associated_uris);
		TSK_OBJECT_SAFE_FREE(TSIP_DIALOG_GET_STACK(self)->service_routes);
		TSK_OBJECT_SAFE_FREE(TSIP_DIALOG_GET_STACK(self)->paths);

		/* Associated URIs */
		for(index = 0; (hdr_P_Associated_URI_t = (const tsip_header_P_Associated_URI_t*)tsip_message_get_headerAt(message, tsip_htype_P_Associated_URI, index)); index++){
			if(!TSIP_DIALOG_GET_STACK(self)->associated_uris){
				TSIP_DIALOG_GET_STACK(self)->associated_uris = TSK_LIST_CREATE();
			}
			uri = tsk_object_ref(hdr_P_Associated_URI_t->uri);
			tsk_list_push_back_data(TSIP_DIALOG_GET_STACK(self)->associated_uris, (void**)&uri);
		}

		/*	Service-Route (3GPP TS 24.229)
			store the list of service route values contained in the Service-Route header field and bind the list to the contact
			address used in registration, in order to build a proper preloaded Route header field value for new dialogs and
			standalone transactions when using the respective contact address.
		*/
		for(index = 0; (hdr_Service_Route = (const tsip_header_Service_Route_t*)tsip_message_get_headerAt(message, tsip_htype_Service_Route, index)); index++){
			if(!TSIP_DIALOG_GET_STACK(self)->service_routes){
				TSIP_DIALOG_GET_STACK(self)->service_routes = TSK_LIST_CREATE();
			}
			uri = tsk_object_ref(hdr_Service_Route->uri);
			tsk_list_push_back_data(TSIP_DIALOG_GET_STACK(self)->service_routes, (void**)&uri);
		}

		/* Paths */
		for(index = 0; (hdr_Path = (const tsip_header_Path_t*)tsip_message_get_headerAt(message, tsip_htype_Path, index)); index++){
			if(TSIP_DIALOG_GET_STACK(self)->paths == 0){
				TSIP_DIALOG_GET_STACK(self)->paths = TSK_LIST_CREATE();
			}
			uri = tsk_object_ref(hdr_Path->uri);
			tsk_list_push_back_data(TSIP_DIALOG_GET_STACK(self)->paths, (void**)&uri);
		}
	}

	/* 3GPP TS 24.229 - 5.1.1.2 Initial registration */
	if((TSIP_DIALOG(self)->state ==tsip_initial))
	{
		tsk_bool_t barred = tsk_true;
		const tsk_list_item_t *item;
		const tsip_uri_t *uri;
		const tsip_uri_t *uri_first = 0;

	/*	
		b) store as the default public user identity the first URI on the list of URIs present in the P-Associated-URI header
		field and bind it to the respective contact address of the UE and the associated set of security associations or TLS
		session;
		NOTE 4: When using the respective contact address and associated set of security associations or TLS session, the
		UE can utilize additional URIs contained in the P-Associated-URI header field and bound it to the
		respective contact address of the UE and the associated set of security associations or TLS session, e.g. for
		application purposes.
		c) treat the identity under registration as a barred public user identity, if it is not included in the P-Associated-URI
		header field;
	*/
		tsk_list_foreach(item, TSIP_DIALOG_GET_STACK(self)->associated_uris)
		{
			uri = item->data;
			if(item == TSIP_DIALOG_GET_STACK(self)->associated_uris->head){
				uri_first = item->data;
			}
			if(!tsk_object_cmp(TSIP_DIALOG_GET_STACK(self)->preferred_identity, uri)){
				barred = 0;
				break;
			}
		}

		if(barred && uri_first){
			TSK_OBJECT_SAFE_FREE(TSIP_DIALOG_GET_STACK(self)->preferred_identity);
			TSIP_DIALOG_GET_STACK(self)->preferred_identity = tsk_object_ref((void*)uri_first);
		}
	}

	/* Update the dialog state. */
	tsip_dialog_update(TSIP_DIALOG(self), message);

	/* Request timeout for dialog refresh (re-registration). */
	self->timerrefresh.timeout = tsip_dialog_get_newdelay(TSIP_DIALOG(self), message);
	TSIP_DIALOG_REGISTER_TIMER_SCHEDULE(refresh);

	return 0;
}

/* Trying -> (2xx) -> Terminated
*/
int tsip_dialog_register_Trying_2_Terminated_X_2xx(va_list *app)
{
	tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
	const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

	/* Alert the user. */
	TSIP_DIALOG_REGISTER_SIGNAL(self, tsip_ao_unregister, 
		TSIP_RESPONSE_CODE(message), TSIP_RESPONSE_PHRASE(message), message);

	return 0;
}

/*	Trying --> (401/407/421/494) --> Trying
*/
int tsip_dialog_register_Trying_2_Trying_X_401_407_421_494(va_list *app)
{
	tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
	const tsip_message_t *message = va_arg(*app, const tsip_message_t *);
	int ret;

	if((ret = tsip_dialog_update(TSIP_DIALOG(self), message))){
		/* Alert the user. */
		TSIP_DIALOG_REGISTER_SIGNAL(self, self->unregistering ? tsip_ao_unregister : tsip_ao_register,
			TSIP_RESPONSE_CODE(message), TSIP_RESPONSE_PHRASE(message), message);
		
		return ret;
	}

	/* Ensure IPSec SAs */
	if(TSIP_DIALOG_GET_STACK(self)->secagree_mech && tsk_striequals(TSIP_DIALOG_GET_STACK(self)->secagree_mech, "ipsec-3gpp")){
		tsip_transport_ensureTempSAs(TSIP_DIALOG_GET_STACK(self)->layer_transport, message, TSIP_DIALOG(self)->expires);
	}
	
	return send_register(self, tsk_false);
}

/*	Trying -> (423) -> Trying
*/
int tsip_dialog_register_Trying_2_Trying_X_423(va_list *app)
{
	tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
	const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

	tsip_header_Min_Expires_t *hdr;
	int ret = 0;

	/*
	RFC 3261 - 10.2.8 Error Responses

	If a UA receives a 423 (Interval Too Brief) response, it MAY retry
	the registration after making the expiration interval of all contact
	addresses in the REGISTER request equal to or greater than the
	expiration interval within the Min-Expires header field of the 423
	(Interval Too Brief) response.
	*/
	hdr = (tsip_header_Min_Expires_t*)tsip_message_get_header(message, tsip_htype_Min_Expires);
	if(hdr){
		TSIP_DIALOG(self)->expires = hdr->value;

		if(tsk_striequals(TSIP_DIALOG_GET_STACK(self)->secagree_mech, "ipsec-3gpp")){
			tsip_transport_cleanupSAs(TSIP_DIALOG_GET_STACK(self)->layer_transport);
			ret = send_register(self, tsk_true);
		}
		else{
			ret = send_register(self, tsk_false);
		}
	}
	else{
		ret = -1;
	}

	return ret;
}

/* Trying -> (300-699) -> Terminated
*/
int tsip_dialog_register_Trying_2_Terminated_X_300_to_699(va_list *app)
{
	tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
	const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

	/* Alert the user. */
	TSIP_DIALOG_REGISTER_SIGNAL(self, self->unregistering ? tsip_ao_unregister : tsip_ao_register, 
		TSIP_RESPONSE_CODE(message), TSIP_RESPONSE_PHRASE(message), message);
	
	return 0;
}

/* Trying -> (cancel) -> Terminated
*/
int tsip_dialog_register_Trying_2_Terminated_X_cancel(va_list *app)
{
	tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
	const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

	/* Alert the user. */
	TSIP_DIALOG_REGISTER_SIGNAL(self, self->unregistering ? tsip_ao_unregister : tsip_ao_register, 
		701, "Registration cancelled", tsk_null);

	return 0;
}

/* Connected -> (Unregister) -> Trying
*/
int tsip_dialog_register_Connected_2_Trying_X_unregister(va_list *app)
{
	tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
	const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

	return 0;
}

/* Connected -> (refresh) -> Trying
*/
int tsip_dialog_register_Connected_2_Trying_X_refresh(va_list *app)
{
	tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
	const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

	return send_register(self, tsk_true);
}

/* Any -> (hangup) -> Trying
*/
int tsip_dialog_register_Any_2_Trying_X_hangup(va_list *app)
{
	tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
	const tsip_message_t *message = va_arg(*app, const tsip_message_t *);
	tsk_bool_t shuttingdown = va_arg(*app, tsk_bool_t);

	/* Schedule timeout (shutdown). */
	if(shuttingdown){
		TSIP_DIALOG_REGISTER_TIMER_SCHEDULE(shutdown);
	}

	self->unregistering = tsk_true;
	return send_register(self, tsk_true);
}

/* Any -> (transport error) -> Terminated
*/
int tsip_dialog_register_Any_2_Terminated_X_transportError(va_list *app)
{
	tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
	const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

	/* Alert the user. */
	TSIP_DIALOG_REGISTER_SIGNAL(self, self->unregistering ? tsip_ao_unregister : tsip_ao_register, 
		702, "Transport error.", tsk_null);

	return 0;
}

/* Any -> (error) -> Terminated
*/
int tsip_dialog_register_Any_2_Terminated_X_Error(va_list *app)
{
	tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
	const tsip_message_t *message = va_arg(*app, const tsip_message_t *);

	/* Alert the user. */
	TSIP_DIALOG_REGISTER_SIGNAL(self, self->unregistering ? tsip_ao_unregister : tsip_ao_register, 
		703, "Global error.", tsk_null);

	return 0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++


/**
 *
 * Sends a REGISTER request. 
 *
 * @param [in,out]	self	The caller.
 * @param [in] initial Indicates whether it's an initial (new CSeq) REGISTER or not.
 * Initial REGISTER request will creates new IPSec temporary SAs.
 *
 * @return	Zero if succeed and non-zero error code otherwise. 
**/
int send_register(tsip_dialog_register_t *self, tsk_bool_t initial)
{
	tsip_request_t *request;
	int ret = -1;
	
	if(self->unregistering){
		TSIP_DIALOG(self)->expires = 0;
	}
	
	request = tsip_dialog_request_new(TSIP_DIALOG(self), "REGISTER");

	if(request)
	{
		/* == RCS phase 2
		*/
		if(TSIP_DIALOG_GET_STACK(self)->enable_gsmarcs){
			TSIP_HEADER_ADD_PARAM(request->Contact, "+g.oma.sip-im.large-message", 0);
			TSIP_HEADER_ADD_PARAM(request->Contact, "audio", 0);
			TSIP_HEADER_ADD_PARAM(request->Contact, "video", 0);
			TSIP_HEADER_ADD_PARAM(request->Contact, "+g.3gpp.cs-voice", 0);
			TSIP_HEADER_ADD_PARAM(request->Contact, "+g.3gpp.icsi-ref", TSIP_ICSI_QUOTED_MMTEL_PSVOICE);
		}

		/* mobility */
		if(TSIP_DIALOG_GET_STACK(self)->mobility){
			TSIP_HEADER_ADD_PARAM(request->Contact, "mobility", TSIP_DIALOG_GET_STACK(self)->mobility);
		}

		/* deviceID - FIXME: find reference. */
		if(TSIP_DIALOG_GET_STACK(self)->device_id){
			TSIP_HEADER_ADD_PARAM(request->Contact, "+deviceID", TSIP_DIALOG_GET_STACK(self)->device_id);
		}

		/* GSMA Image Sharing */
		if(TSIP_DIALOG_GET_STACK(self)->enable_gsmais){
			TSIP_HEADER_ADD_PARAM(request->Contact, "+g.3gpp.app_ref", TSIP_IARI_QUOTED_GSMAIS);
		}

		/* 3GPP TS 24.341 subclause 5.3.2.2 */
		if(TSIP_DIALOG_GET_STACK(self)->enable_3gppsms){
			TSIP_HEADER_ADD_PARAM(request->Contact, "+g.3gpp.smsip", 0);
		}

		/* 3GPP TS 24.229 - 5.1.1.2 Initial registration */
		if(TSIP_DIALOG(self)->state ==tsip_initial)
		{
			/*
				g) the Supported header field containing the option-tag "path", and
				1) if GRUU is supported, the option-tag "gruu"; and
				2) if multiple registrations is supported, the option-tag "outbound".
			*/
			TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_SUPPORTED_VA_ARGS("path"));
			//if(1==2/* gruu*/){
			//}
			//else if(2 == 3 /* multiple registrations */){
			//}
		}
		
		/* Create temorary SAs if initial register. */
		if(TSIP_DIALOG_GET_STACK(self)->secagree_mech){
			if(tsk_striequals(TSIP_DIALOG_GET_STACK(self)->secagree_mech, "ipsec-3gpp")){
				if(initial){
					tsip_transport_createTempSAs(TSIP_DIALOG_GET_STACK(self)->layer_transport);
				}
				else{
					AKA_CK_T ck;
					AKA_IK_T ik;
					tsip_dialog_getCKIK(TSIP_DIALOG(self), &ck, &ik);
					tsip_transport_startSAs(TSIP_DIALOG_GET_STACK(self)->layer_transport, (const tipsec_key_t*)ik, (const tipsec_key_t*)ck);
				}
			}
		}
		
		ret = tsip_dialog_request_send(TSIP_DIALOG(self), request);
		TSK_OBJECT_SAFE_FREE(request);
	}

	return ret;
}


/**
 * @fn	int tsip_dialog_register_OnTerminated(tsip_dialog_register_t *self)
 *
 * @brief	Callback function called by the state machine manager to signal that the final state has been reached.
 *
 * @author	Mamadou
 * @date	1/5/2010
 *
 * @param [in,out]	self	The state machine owner.
**/
int tsip_dialog_register_OnTerminated(tsip_dialog_register_t *self)
{
	TSK_DEBUG_INFO("=== REGISTER Dialog terminated ===");
	
	/* Cancel all timers */
	DIALOG_TIMER_CANCEL(refresh);
	DIALOG_TIMER_CANCEL(shutdown);
	
	/* Cleanup IPSec SAs */
	if(TSIP_DIALOG_GET_STACK(self)->secagree_mech && tsk_striequals(TSIP_DIALOG_GET_STACK(self)->secagree_mech, "ipsec-3gpp")){
		tsip_transport_cleanupSAs(TSIP_DIALOG_GET_STACK(self)->layer_transport);
	}

	/* Remove from the dialog layer. */
	return tsip_dialog_remove(TSIP_DIALOG(self));
}














//========================================================
//	SIP dialog REGISTER object definition
//
static void* tsip_dialog_register_create(void * self, va_list * app)
{
	tsip_dialog_register_t *dialog = self;
	if(dialog)
	{
		tsip_stack_handle_t *stack = va_arg(*app, tsip_stack_handle_t *);
		tsip_operation_handle_t *operation = va_arg(*app, tsip_operation_handle_t *);

		/* create FSM */
		dialog->fsm = TSK_FSM_CREATE(_fsm_state_Started, _fsm_state_Terminated);
		dialog->fsm->debug = DEBUG_STATE_MACHINE;
		tsk_fsm_set_callback_terminated(dialog->fsm, TSK_FSM_ONTERMINATED(tsip_dialog_register_OnTerminated), (const void*)dialog);

		/* Initialize base class */
		tsip_dialog_init(TSIP_DIALOG(self), tsip_dialog_REGISTER, stack, tsk_null, operation);

		/* Initialize the class itself */
		tsip_dialog_register_init(self);
	}
	return self;
}

static void* tsip_dialog_register_destroy(void * self)
{ 
	tsip_dialog_register_t *dialog = self;
	if(dialog)
	{
		/* DeInitialize base class */
		tsip_dialog_deinit(TSIP_DIALOG(self));
		
		/* FSM */
		TSK_OBJECT_SAFE_FREE(dialog->fsm);
	}
	return self;
}

static int tsip_dialog_register_cmp(const void *dialog1, const void *dialog2)
{
	return tsip_dialog_cmp(dialog1, dialog2);
}

static const tsk_object_def_t tsip_dialog_register_def_s = 
{
	sizeof(tsip_dialog_register_t),
	tsip_dialog_register_create, 
	tsip_dialog_register_destroy,
	tsip_dialog_register_cmp, 
};
const void *tsip_dialog_register_def_t = &tsip_dialog_register_def_s;
