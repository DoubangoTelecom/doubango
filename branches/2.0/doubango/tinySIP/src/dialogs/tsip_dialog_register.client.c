/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/dialogs/tsip_dialog_register.h"
#include "tinysip/dialogs/tsip_dialog_register.common.h"

#include "tinysip/transports/tsip_transport_layer.h"
#include "tinysip/transactions/tsip_transac_layer.h"

#include "tinysip/headers/tsip_header_Path.h"
#include "tinysip/headers/tsip_header_Service_Route.h"
#include "tinysip/headers/tsip_header_P_Associated_URI.h"
#include "tinysip/headers/tsip_header_Min_Expires.h"

#include "tinysip/parsers/tsip_parser_uri.h"

/* ======================== external functions ======================== */
extern int tsip_dialog_register_timer_callback(const tsip_dialog_register_t* self, tsk_timer_id_t timer_id);
extern int tsip_dialog_register_send_REGISTER(tsip_dialog_register_t *self, tsk_bool_t initial);

/* ======================== transitions ======================== */
static int tsip_dialog_register_Started_2_InProgress_X_oRegister(va_list *app);
static int tsip_dialog_register_InProgress_2_InProgress_X_1xx(va_list *app);
static int tsip_dialog_register_InProgress_2_Terminated_X_2xx(va_list *app);
static int tsip_dialog_register_InProgress_2_Connected_X_2xx(va_list *app);
static int tsip_dialog_register_InProgress_2_InProgress_X_401_407_421_494(va_list *app);
static int tsip_dialog_register_InProgress_2_InProgress_X_423(va_list *app);
static int tsip_dialog_register_InProgress_2_Terminated_X_300_to_699(va_list *app);
static int tsip_dialog_register_InProgress_2_Terminated_X_cancel(va_list *app);
static int tsip_dialog_register_Connected_2_InProgress_X_oRegister(va_list *app);


/* ======================== conds ======================== */
static tsk_bool_t _fsm_cond_client_unregistering(tsip_dialog_register_t* dialog, tsip_message_t* message)
{
	return !dialog->is_server && dialog->unregistering;
}
static tsk_bool_t _fsm_cond_client_registering(tsip_dialog_register_t* dialog, tsip_message_t* message)
{
	return !_fsm_cond_client_unregistering(dialog, message);
}


/** Initializes the dialog.
 *
 * @param [in,out]	self	The dialog to initialize. 
**/
int tsip_dialog_register_client_init(tsip_dialog_register_t *self)
{
	/* Initialize the state machine. */
	tsk_fsm_set(TSIP_DIALOG_GET_FSM(self),
			
			/*=======================
			* === Started === 
			*/
			// Started -> (REGISTER) -> InProgress
			TSK_FSM_ADD_ALWAYS(_fsm_state_Started, _fsm_action_oREGISTER, _fsm_state_InProgress, tsip_dialog_register_Started_2_InProgress_X_oRegister, "tsip_dialog_register_Started_2_InProgress_X_oRegister"),
			// Started -> (Any) -> Started
			//TSK_FSM_ADD_ALWAYS_NOTHING(_fsm_state_Started, "tsip_dialog_register_Started_2_Started_X_any"),
			

			/*=======================
			* === InProgress === 
			*/
			// InProgress -> (1xx) -> InProgress
			TSK_FSM_ADD_ALWAYS(_fsm_state_InProgress, _fsm_action_1xx, _fsm_state_InProgress, tsip_dialog_register_InProgress_2_InProgress_X_1xx, "tsip_dialog_register_InProgress_2_InProgress_X_1xx"),
			// InProgress -> (2xx) -> Terminated
			TSK_FSM_ADD(_fsm_state_InProgress, _fsm_action_2xx, _fsm_cond_client_unregistering, _fsm_state_Terminated, tsip_dialog_register_InProgress_2_Terminated_X_2xx, "tsip_dialog_register_InProgress_2_Terminated_X_2xx"),
			// InProgress -> (2xx) -> Connected
			TSK_FSM_ADD(_fsm_state_InProgress, _fsm_action_2xx, _fsm_cond_client_registering, _fsm_state_Connected, tsip_dialog_register_InProgress_2_Connected_X_2xx, "tsip_dialog_register_InProgress_2_Connected_X_2xx"),
			// InProgress -> (401/407/421/494) -> InProgress
			TSK_FSM_ADD_ALWAYS(_fsm_state_InProgress, _fsm_action_401_407_421_494, _fsm_state_InProgress, tsip_dialog_register_InProgress_2_InProgress_X_401_407_421_494, "tsip_dialog_register_InProgress_2_InProgress_X_401_407_421_494"),
			// InProgress -> (423) -> InProgress
			TSK_FSM_ADD_ALWAYS(_fsm_state_InProgress, _fsm_action_423, _fsm_state_InProgress, tsip_dialog_register_InProgress_2_InProgress_X_423, "tsip_dialog_register_InProgress_2_InProgress_X_423"),
			// InProgress -> (300_to_699) -> Terminated
			TSK_FSM_ADD_ALWAYS(_fsm_state_InProgress, _fsm_action_300_to_699, _fsm_state_Terminated, tsip_dialog_register_InProgress_2_Terminated_X_300_to_699, "tsip_dialog_register_InProgress_2_Terminated_X_300_to_699"),
			// InProgress -> (cancel) -> Terminated
			TSK_FSM_ADD_ALWAYS(_fsm_state_InProgress, _fsm_action_cancel, _fsm_state_Terminated, tsip_dialog_register_InProgress_2_Terminated_X_cancel, "tsip_dialog_register_InProgress_2_Terminated_X_cancel"),
			// InProgress -> (hangup) -> Terminated
			TSK_FSM_ADD_ALWAYS(_fsm_state_InProgress, _fsm_action_hangup, _fsm_state_Terminated, tsk_null, "tsip_dialog_register_InProgress_2_Terminated_X_hangup"),
			// InProgress -> (shutdown) -> Terminated
			TSK_FSM_ADD_ALWAYS(_fsm_state_InProgress, _fsm_action_shutdown, _fsm_state_Terminated, tsk_null, "tsip_dialog_register_InProgress_2_Terminated_X_shutdown"),
			// InProgress -> (Any) -> InProgress
			//TSK_FSM_ADD_ALWAYS_NOTHING(_fsm_state_InProgress, "tsip_dialog_register_InProgress_2_InProgress_X_any"),


			/*=======================
			* === Connected === 
			*/
			// Connected -> (register) -> InProgress [refresh case]
			TSK_FSM_ADD_ALWAYS(_fsm_state_Connected, _fsm_action_oREGISTER, _fsm_state_InProgress, tsip_dialog_register_Connected_2_InProgress_X_oRegister, "tsip_dialog_register_Connected_2_InProgress_X_oRegister"),


			TSK_FSM_ADD_NULL());

	return 0;
}

//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------

/* Started -> (REGISTER) -> InProgress
*/
int tsip_dialog_register_Started_2_InProgress_X_oRegister(va_list *app)
{
	tsip_dialog_register_t *self;

	self = va_arg(*app, tsip_dialog_register_t *);

	TSIP_DIALOG(self)->running = tsk_true;

	/* alert the user */
	TSIP_DIALOG_SIGNAL(self, tsip_event_code_dialog_connecting, "Dialog connecting");

	return tsip_dialog_register_send_REGISTER(self, tsk_true);
}

/* InProgress -> (1xx) -> InProgress
*/
int tsip_dialog_register_InProgress_2_InProgress_X_1xx(va_list *app)
{
	tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	/* Alert the user (session) */
	TSIP_DIALOG_REGISTER_SIGNAL(self, tsip_ao_register, 
		TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), response);

	return tsip_dialog_update(TSIP_DIALOG(self), response);
}

/* InProgress -> (2xx) -> Connected
*/
//#include "tsk_thread.h"
int tsip_dialog_register_InProgress_2_Connected_X_2xx(va_list *app)
{
	int ret;
	tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	tsk_bool_t first_time_to_connect = (TSIP_DIALOG(self)->state == tsip_initial);

	/*	- Set P-associated-uriS
	*	- Update service-routes
	*	- Update Pats
	*/
	{
		tsk_size_t index;
		const tsip_header_Path_t *hdr_Path;
		const tsip_header_Service_Route_t *hdr_Service_Route;
		const tsip_header_P_Associated_URI_t *hdr_P_Associated_URI_t;
		tsip_uri_t *uri;

		/* To avoid memory leaks ==> delete all concerned objects (it worth nothing) */
		TSK_OBJECT_SAFE_FREE(TSIP_DIALOG_GET_STACK(self)->associated_uris);
		TSK_OBJECT_SAFE_FREE(TSIP_DIALOG_GET_STACK(self)->service_routes);
		TSK_OBJECT_SAFE_FREE(TSIP_DIALOG_GET_STACK(self)->paths);

		/* Associated URIs */
		for(index = 0; (hdr_P_Associated_URI_t = (const tsip_header_P_Associated_URI_t*)tsip_message_get_headerAt(response, tsip_htype_P_Associated_URI, index)); index++){
			if(!TSIP_DIALOG_GET_STACK(self)->associated_uris){
				TSIP_DIALOG_GET_STACK(self)->associated_uris = tsk_list_create();
			}
			uri = tsk_object_ref(hdr_P_Associated_URI_t->uri);
			tsk_list_push_back_data(TSIP_DIALOG_GET_STACK(self)->associated_uris, (void**)&uri);
		}

		/*	Service-Route (3GPP TS 24.229)
			store the list of service route values contained in the Service-Route header field and bind the list to the contact
			address used in registration, in order to build a proper preloaded Route header field value for new dialogs and
			standalone transactions when using the respective contact address.
		*/
		for(index = 0; (hdr_Service_Route = (const tsip_header_Service_Route_t*)tsip_message_get_headerAt(response, tsip_htype_Service_Route, index)); index++){
			if(!TSIP_DIALOG_GET_STACK(self)->service_routes){
				TSIP_DIALOG_GET_STACK(self)->service_routes = tsk_list_create();
			}
			uri = tsk_object_ref(hdr_Service_Route->uri);
			tsk_list_push_back_data(TSIP_DIALOG_GET_STACK(self)->service_routes, (void**)&uri);
		}

		/* Paths */
		for(index = 0; (hdr_Path = (const tsip_header_Path_t*)tsip_message_get_headerAt(response, tsip_htype_Path, index)); index++){
			if(TSIP_DIALOG_GET_STACK(self)->paths == 0){
				TSIP_DIALOG_GET_STACK(self)->paths = tsk_list_create();
			}
			uri = tsk_object_ref(hdr_Path->uri);
			tsk_list_push_back_data(TSIP_DIALOG_GET_STACK(self)->paths, (void**)&uri);
		}
	}

	/* 3GPP TS 24.229 - 5.1.1.2 Initial registration */
	if(first_time_to_connect){
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
		tsk_list_foreach(item, TSIP_DIALOG_GET_STACK(self)->associated_uris){
			uri = item->data;
			if(item == TSIP_DIALOG_GET_STACK(self)->associated_uris->head){
				uri_first = item->data;
			}
			if(!tsk_object_cmp(TSIP_DIALOG_GET_STACK(self)->identity.preferred, uri)){
				barred = 0;
				break;
			}
		}

		if(barred && uri_first){
			TSK_OBJECT_SAFE_FREE(TSIP_DIALOG_GET_STACK(self)->identity.preferred);
			TSIP_DIALOG_GET_STACK(self)->identity.preferred = tsk_object_ref((void*)uri_first);
		}
	}
	
	/* Update the dialog state */
	if((ret = tsip_dialog_update(TSIP_DIALOG(self), response))){
		return ret;
	}
	
	/* Reset current action */
	tsip_dialog_set_curr_action(TSIP_DIALOG(self), tsk_null);
	
	/* Request timeout for dialog refresh (re-registration). */
	self->timerrefresh.timeout = tsip_dialog_get_newdelay(TSIP_DIALOG(self), response);
	TSIP_DIALOG_REGISTER_TIMER_SCHEDULE(refresh);

	/* Alert the user (session) */
	TSIP_DIALOG_REGISTER_SIGNAL(self, tsip_ao_register, 
		TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), response);
	/* Alert the user (dialog) */
	if(first_time_to_connect){
		TSIP_DIALOG_SIGNAL(self, tsip_event_code_dialog_connected, "Dialog connected");
	}
	
	return ret;
}

/* InProgress -> (2xx) -> Terminated
*/
int tsip_dialog_register_InProgress_2_Terminated_X_2xx(va_list *app)
{
	tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	/* save last error */
	tsip_dialog_set_lasterror_2(TSIP_DIALOG(self), TSIP_RESPONSE_PHRASE(response), TSIP_RESPONSE_CODE(response), response);

	/* Alert the user */
	TSIP_DIALOG_REGISTER_SIGNAL(self, tsip_ao_unregister, 
		TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), response);

	return 0;
}

/*	InProgress --> (401/407/421/494) --> InProgress
*/
int tsip_dialog_register_InProgress_2_InProgress_X_401_407_421_494(va_list *app)
{
	tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);
	int ret;

	if((ret = tsip_dialog_update(TSIP_DIALOG(self), response))){
		/* Alert the user. */
		TSIP_DIALOG_REGISTER_SIGNAL(self, self->unregistering ? tsip_ao_unregister : tsip_ao_register,
			TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), response);
		
		/* set last error (or info) */
		tsip_dialog_set_lasterror(TSIP_DIALOG(self), "Authentication failed", TSIP_RESPONSE_CODE(response));
		
		return ret;
	}

	/* Ensure IPSec SAs */
	if(TSIP_DIALOG_GET_STACK(self)->security.secagree_mech && tsk_striequals(TSIP_DIALOG_GET_STACK(self)->security.secagree_mech, "ipsec-3gpp")){
		tsip_transport_ensureTempSAs(TSIP_DIALOG_GET_STACK(self)->layer_transport, response, TSIP_DIALOG(self)->expires);
	}
	
	return tsip_dialog_register_send_REGISTER(self, tsk_false);
}

/*	InProgress -> (423) -> InProgress
*/
int tsip_dialog_register_InProgress_2_InProgress_X_423(va_list *app)
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
		TSIP_DIALOG(self)->expires = TSK_TIME_S_2_MS(hdr->value);

		if(tsk_striequals(TSIP_DIALOG_GET_STACK(self)->security.secagree_mech, "ipsec-3gpp")){
			tsip_transport_cleanupSAs(TSIP_DIALOG_GET_STACK(self)->layer_transport);
			ret = tsip_dialog_register_send_REGISTER(self, tsk_true);
		}
		else{
			ret = tsip_dialog_register_send_REGISTER(self, tsk_false);
		}
	}
	else{
		TSK_DEBUG_ERROR("Missing header: Min_Expires");
		ret = -1;
	}

	return ret;
}

/* InProgress -> (300-699) -> Terminated
*/
int tsip_dialog_register_InProgress_2_Terminated_X_300_to_699(va_list *app)
{
	tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	/* save last error */
	tsip_dialog_set_lasterror_2(TSIP_DIALOG(self), TSIP_RESPONSE_PHRASE(response), TSIP_RESPONSE_CODE(response), response);
	
	/* Alert the user. */
	TSIP_DIALOG_REGISTER_SIGNAL(self, self->unregistering ? tsip_ao_unregister : tsip_ao_register, 
		TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), response);
	
	return 0;
}

/* InProgress -> (cancel) -> Terminated
*/
int tsip_dialog_register_InProgress_2_Terminated_X_cancel(va_list *app)
{
	int ret;
	tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
	/* const tsip_message_t *message = va_arg(*app, const tsip_message_t *); */

	/* Cancel all transactions associated to this dialog (will also be done when the dialog is destroyed (worth nothing)) */
	ret = tsip_transac_layer_cancel_by_dialog(TSIP_DIALOG_GET_STACK(self)->layer_transac, TSIP_DIALOG(self));

	/* RFC 3261 - 9.1 Client Behavior
	   A CANCEL request SHOULD NOT be sent to cancel a request other than INVITE.
	   */

	/* Alert the user */
	TSIP_DIALOG_SIGNAL(self, tsip_event_code_dialog_request_cancelled, "Registration cancelled");

	return ret;
}

/* Connected -> (REGISTER) -> InProgress
*/
int tsip_dialog_register_Connected_2_InProgress_X_oRegister(va_list *app)
{
	tsip_dialog_register_t *self;

	self = va_arg(*app, tsip_dialog_register_t *);

	return tsip_dialog_register_send_REGISTER(self, tsk_true);
}


















