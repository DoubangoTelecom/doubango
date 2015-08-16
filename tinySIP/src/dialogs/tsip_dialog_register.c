/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
*	
* This file is part of Open Source Doubango Framework.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as publishd by
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
#include "tinysip/dialogs/tsip_dialog_register.h"
#include "tinysip/dialogs/tsip_dialog_register.common.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tinysip/transports/tsip_transport_layer.h"

#include "tinysip/headers/tsip_header_Dummy.h"
#include "tinysip/headers/tsip_header_Supported.h"

#include "tinysip/transactions/tsip_transac_layer.h"

#include "tipsec.h"

#include "tsk_memory.h"
#include "tsk_debug.h"
#include "tsk_time.h"


/* ======================== internal functions ======================== */
/*static*/ int tsip_dialog_register_send_REGISTER(tsip_dialog_register_t *self, tsk_bool_t initial);
/*static*/ int tsip_dialog_register_send_RESPONSE(tsip_dialog_register_t *self, const tsip_request_t* request, short code, const char* phrase);
/*static*/ int tsip_dialog_register_OnTerminated(tsip_dialog_register_t *self);

/* ======================== transitions ======================== */
static int tsip_dialog_register_Any_2_InProgress_X_hangup(va_list *app);
static int tsip_dialog_register_Any_2_InProgress_X_shutdown(va_list *app);
static int tsip_dialog_register_Any_2_Terminated_X_transportError(va_list *app);
static int tsip_dialog_register_Any_2_Terminated_X_Error(va_list *app);

/* ======================== conds ======================== */
static tsk_bool_t _fsm_cond_silent_hangup(tsip_dialog_register_t* dialog, tsip_message_t* message)
{
	return TSIP_DIALOG(dialog)->ss->silent_hangup;
}
static tsk_bool_t _fsm_cond_not_silent_hangup(tsip_dialog_register_t* dialog, tsip_message_t* message)
{
	return !TSIP_DIALOG(dialog)->ss->silent_hangup;
}


/* Client-Side dialog */
extern int tsip_dialog_register_client_init(tsip_dialog_register_t *self);
/* Server-Side dialog */
extern int tsip_dialog_register_server_init(tsip_dialog_register_t *self);

/**
 * @fn	int tsip_dialog_register_event_callback(const tsip_dialog_register_t *self, tsip_dialog_event_type_t type,
 * 		const tsip_message_t *msg)
 *
 * @brief	Callback function called to alert the dialog for new events from the transaction/transport layers.
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

	switch(type){
		case tsip_dialog_i_msg:
			{
				if(msg){
					if(TSIP_MESSAGE_IS_RESPONSE(msg)){
						//
						//	RESPONSE
						//
						const tsip_action_t* action = tsip_dialog_keep_action(TSIP_DIALOG(self), msg) ? TSIP_DIALOG(self)->curr_action : tsk_null;
						if(TSIP_RESPONSE_IS_1XX(msg)){
							ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_1xx, msg, action);
						}
						else if(TSIP_RESPONSE_IS_2XX(msg)){
							ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_2xx, msg, action);
						}
						else if(TSIP_RESPONSE_IS(msg,401) || TSIP_RESPONSE_IS(msg,407) || TSIP_RESPONSE_IS(msg,421) || TSIP_RESPONSE_IS(msg,494)){
							ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_401_407_421_494, msg, action);
						}
						else if(TSIP_RESPONSE_IS(msg,423)){
							ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_423, msg, action);
						}
						else{
							// Alert User
							ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_error, msg, action);
							/* TSK_DEBUG_WARN("Not supported status code: %d", TSIP_RESPONSE_CODE(msg)); */
						}
					}
					else{
						//
						//	REQUEST
						//
						if(TSIP_REQUEST_IS_REGISTER(msg)){
							ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_iREGISTER, msg, tsk_null);
						}
					}
				}
				break;
			}

		case tsip_dialog_canceled:
			{
				ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_cancel, msg, tsk_null);
				break;
			}

		case tsip_dialog_terminated:
		case tsip_dialog_timedout:
		case tsip_dialog_error:
		case tsip_dialog_transport_error:
			{
				ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_transporterror, msg, tsk_null);
				break;
			}
            
    default: break;
	}

	return ret;
}

/**Timer manager callback.
 *
 * @param self	The owner of the signaled timer. 
 * @param	timer_id		The identifier of the signaled timer.
 *
 * @return	Zero if succeed and non-zero error code otherwise.  
**/
int tsip_dialog_register_timer_callback(const tsip_dialog_register_t* self, tsk_timer_id_t timer_id)
{
	int ret = -1;

	if(self){
		if(timer_id == self->timerrefresh.id){
			ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_oREGISTER, tsk_null, tsk_null);
		}
		else if(timer_id == self->timershutdown.id){
			ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_shutdown_timedout, tsk_null, tsk_null);
		}
	}
	return ret;
}

/** Create SIP REGISTER dialog. */
tsip_dialog_register_t* tsip_dialog_register_create(const tsip_ssession_handle_t* ss, const char* call_id)
{
	return tsk_object_new(tsip_dialog_register_def_t, ss, call_id);
}


/** Initializes the dialog.
 *
 * @param [in,out]	self	The dialog to initialize. 
**/
int tsip_dialog_register_init(tsip_dialog_register_t *self)
{
	// Initialize client side
	tsip_dialog_register_client_init(self);
	// initialize server side
	tsip_dialog_register_server_init(self);
	
	/* Initialize common side */
	tsk_fsm_set(TSIP_DIALOG_GET_FSM(self),
			
			/*=======================
			* === Any === 
			*/
			// Any -> (hangup) -> InProgress
			TSK_FSM_ADD(tsk_fsm_state_any, _fsm_action_hangup, _fsm_cond_not_silent_hangup, _fsm_state_InProgress, tsip_dialog_register_Any_2_InProgress_X_hangup, "tsip_dialog_register_Any_2_InProgress_X_hangup"),
			// Any -> (silenthangup) -> Terminated
			TSK_FSM_ADD(tsk_fsm_state_any, _fsm_action_hangup, _fsm_cond_silent_hangup, _fsm_state_Terminated, tsk_null, "tsip_dialog_register_Any_2_InProgress_X_silenthangup"),
			// Any -> (shutdown) -> InProgress
			TSK_FSM_ADD(tsk_fsm_state_any, _fsm_action_shutdown, _fsm_cond_not_silent_shutdown, _fsm_state_InProgress, tsip_dialog_register_Any_2_InProgress_X_shutdown, "tsip_dialog_register_Any_2_InProgress_X_shutdown"),
			// Any -> (silentshutdown) -> Terminated
			TSK_FSM_ADD(tsk_fsm_state_any, _fsm_action_shutdown, _fsm_cond_silent_shutdown, _fsm_state_Terminated, tsk_null, "tsip_dialog_register_Any_2_InProgress_X_silentshutdown"),
			// Any -> (shutdown timedout) -> Terminated
			TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_shutdown_timedout, _fsm_state_Terminated, tsk_null, "tsip_dialog_register_shutdown_timedout"),			
			// Any -> (transport error) -> Terminated
			TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_transporterror, _fsm_state_Terminated, tsip_dialog_register_Any_2_Terminated_X_transportError, "tsip_dialog_register_Any_2_Terminated_X_transportError"),
			// Any -> (error) -> Terminated
			TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_error, _fsm_state_Terminated, tsip_dialog_register_Any_2_Terminated_X_Error, "tsip_dialog_register_Any_2_Terminated_X_Error"),

			TSK_FSM_ADD_NULL());
	
	/* Sets callback function */
	TSIP_DIALOG(self)->callback = TSIP_DIALOG_EVENT_CALLBACK_F(tsip_dialog_register_event_callback);
	
	/* Timers */
	self->timerrefresh.id = TSK_INVALID_TIMER_ID;
	self->timerrefresh.timeout = TSIP_DIALOG(self)->expires;
	self->timershutdown.id = TSK_INVALID_TIMER_ID;
	self->timershutdown.timeout = TSIP_DIALOG_SHUTDOWN_TIMEOUT;
	
	return 0;
}




/* Any -> (hangup) -> InProgress
*/
int tsip_dialog_register_Any_2_InProgress_X_hangup(va_list *app)
{
	tsip_dialog_register_t *self;

	self = va_arg(*app, tsip_dialog_register_t *);

	/* Alert the user */
	TSIP_DIALOG_SIGNAL(self, tsip_event_code_dialog_terminating, "Terminating dialog");

	self->unregistering = tsk_true;
	return tsip_dialog_register_send_REGISTER(self, tsk_true);
}

/* Any -> (shutdown) -> InProgress
*/
int tsip_dialog_register_Any_2_InProgress_X_shutdown(va_list *app)
{
	tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
	
	/* schedule shutdow timeout */
	TSIP_DIALOG_REGISTER_TIMER_SCHEDULE(shutdown);

	/* alert user */
	TSIP_DIALOG_SIGNAL(self, tsip_event_code_dialog_terminating, "Terminating dialog");

	self->unregistering = tsk_true;
	return tsip_dialog_register_send_REGISTER(self, tsk_true);
}

/* Any -> (transport error) -> Terminated
*/
int tsip_dialog_register_Any_2_Terminated_X_transportError(va_list *app)
{
	tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
	/*const tsip_message_t *message = va_arg(*app, const tsip_message_t *);*/

	/* Alert the user. */
	TSIP_DIALOG_SIGNAL(self, tsip_event_code_dialog_transport_error, "Transport error.");

	return 0;
}

/* Any -> (error) -> Terminated
*/
int tsip_dialog_register_Any_2_Terminated_X_Error(va_list *app)
{
	tsip_dialog_register_t *self = va_arg(*app, tsip_dialog_register_t *);
	const tsip_response_t *response = va_arg(*app, const tsip_response_t *);

	/* save last error */
	tsip_dialog_set_lasterror_2(TSIP_DIALOG(self), TSIP_RESPONSE_PHRASE(response), TSIP_RESPONSE_CODE(response), response);

	/* Alert the user. */
	if(response){
		TSIP_DIALOG_REGISTER_SIGNAL(self, self->unregistering ? tsip_ao_unregister : tsip_ao_register, 
				TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), response);
	}
	else{
		TSIP_DIALOG_SIGNAL(self, tsip_event_code_dialog_global_error, "Global error.");
	}

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
int tsip_dialog_register_send_REGISTER(tsip_dialog_register_t *self, tsk_bool_t initial)
{
	tsip_request_t *request;
	int ret = -1;
	
	/* whether we are unregistering */
	if(self->unregistering){
		TSIP_DIALOG(self)->expires = 0;
	}
	
	/* creates REGISTER request */
	if((request = tsip_dialog_request_new(TSIP_DIALOG(self), "REGISTER"))){
		/* == RCS phase 2
		*/
		/*if(TSIP_DIALOG_GET_STACK(self)->enable_gsmarcs){
			TSIP_HEADER_ADD_PARAM(request->Contact, "+g.oma.sip-im.large-message", 0);
			TSIP_HEADER_ADD_PARAM(request->Contact, "audio", 0);
			TSIP_HEADER_ADD_PARAM(request->Contact, "video", 0);
			TSIP_HEADER_ADD_PARAM(request->Contact, "+g.3gpp.cs-voice", 0);
			TSIP_HEADER_ADD_PARAM(request->Contact, "+g.3gpp.icsi-ref", TSIP_ICSI_QUOTED_MMTEL_PSVOICE);
		}*/

		///* mobility */
		//if(TSIP_DIALOG_GET_STACK(self)->mobility){
		//	TSIP_HEADER_ADD_PARAM(request->Contact, "mobility", TSIP_DIALOG_GET_STACK(self)->mobility);
		//}

		///* deviceID - FIXME: find reference. */
		//if(TSIP_DIALOG_GET_STACK(self)->device_id){
		//	TSIP_HEADER_ADD_PARAM(request->Contact, "+deviceID", TSIP_DIALOG_GET_STACK(self)->device_id);
		//}

		///* GSMA Image Sharing */
		//if(TSIP_DIALOG_GET_STACK(self)->enable_gsmais){
		//	TSIP_HEADER_ADD_PARAM(request->Contact, "+g.3gpp.app_ref", TSIP_IARI_QUOTED_GSMAIS);
		//}

		///* 3GPP TS 24.341 subclause 5.3.2.2 */
		//if(TSIP_DIALOG_GET_STACK(self)->enable_3gppsms){
		//	TSIP_HEADER_ADD_PARAM(request->Contact, "+g.3gpp.smsip", 0);
		//}

		/* 3GPP TS 24.229 - 5.1.1.2 Initial registration */
		if(TSIP_DIALOG(self)->state ==tsip_initial){
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
		
		/* action parameters and payload */
		if(TSIP_DIALOG(self)->curr_action){
			const tsk_list_item_t* item;
			tsk_list_foreach(item, TSIP_DIALOG(self)->curr_action->headers){
				TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_DUMMY_VA_ARGS(TSK_PARAM(item->data)->name, TSK_PARAM(item->data)->value));
			}
			if(TSIP_DIALOG(self)->curr_action->payload){
				tsip_message_add_content(request, tsk_null, TSK_BUFFER_DATA(TSIP_DIALOG(self)->curr_action->payload), TSK_BUFFER_SIZE(TSIP_DIALOG(self)->curr_action->payload));
			}
		}
		
		/* Create temorary SAs if initial register. */
		if(TSIP_DIALOG_GET_STACK(self)->security.secagree_mech){
			if(tsk_striequals(TSIP_DIALOG_GET_STACK(self)->security.secagree_mech, "ipsec-3gpp")){
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
		
		if(!(ret = tsip_dialog_request_send(TSIP_DIALOG(self), request))){
			TSIP_DIALOG_SIGNAL(self, tsip_event_code_dialog_request_sent, "(un)REGISTER request successfully sent.");
		}
		else{
			TSIP_DIALOG_SIGNAL(self, tsip_event_code_dialog_transport_error, "Transport error.");
		}
		
		TSK_OBJECT_SAFE_FREE(request);
	}

	return ret;
}



// Send any response
int tsip_dialog_register_send_RESPONSE(tsip_dialog_register_t *self, const tsip_request_t* request, short code, const char* phrase)
{
	tsip_response_t *response;
	int ret = -1;

	if(!self || !request){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 1;
	}

	if((response = tsip_dialog_response_new(TSIP_DIALOG(self), code, phrase, request))){
		ret = tsip_dialog_response_send(TSIP_DIALOG(self), response);
		TSK_OBJECT_SAFE_FREE(response);
	}
	return ret;
}



/**
 * @fn	int tsip_dialog_register_OnTerminated(tsip_dialog_register_t *self)
 *
 * @brief	Callback function called by the state machine manager to signal that the final state has been reached.
 *
 * @param [in,out]	self	The state machine owner.
**/
int tsip_dialog_register_OnTerminated(tsip_dialog_register_t *self)
{
	TSK_DEBUG_INFO("=== REGISTER Dialog terminated ===");
	
	/* Cleanup IPSec SAs */
	if(TSIP_DIALOG_GET_STACK(self)->security.secagree_mech && tsk_striequals(TSIP_DIALOG_GET_STACK(self)->security.secagree_mech, "ipsec-3gpp")){
		tsip_transport_cleanupSAs(TSIP_DIALOG_GET_STACK(self)->layer_transport);
	}
	/* Reset values to avoid issues when the session is reused */
	self->unregistering = tsk_false;
	TSK_OBJECT_SAFE_FREE(self->last_iRegister);

	/* Alert the user */
	TSIP_DIALOG_SIGNAL_2(self, tsip_event_code_dialog_terminated,
			TSIP_DIALOG(self)->last_error.phrase ? TSIP_DIALOG(self)->last_error.phrase : "Dialog terminated",
			TSIP_DIALOG(self)->last_error.message);
	
	/* Remove from the dialog layer. */
	return tsip_dialog_remove(TSIP_DIALOG(self));
}




//========================================================
//	SIP dialog REGISTER object definition
//
static tsk_object_t* tsip_dialog_register_ctor(tsk_object_t * self, va_list * app)
{
	tsip_dialog_register_t *dialog = self;
	if(dialog){
		tsip_ssession_t *ss = va_arg(*app, tsip_ssession_t *);
		const char* call_id = va_arg(*app, const char *);

		/* Initialize base class */
		tsip_dialog_init(TSIP_DIALOG(self), tsip_dialog_REGISTER, call_id, ss, _fsm_state_Started, _fsm_state_Terminated);

		/* create FSM */
		TSIP_DIALOG_GET_FSM(self)->debug = DEBUG_STATE_MACHINE;
		tsk_fsm_set_callback_terminated(TSIP_DIALOG_GET_FSM(self), TSK_FSM_ONTERMINATED_F(tsip_dialog_register_OnTerminated), (const void*)dialog);

		/* Initialize the class itself */
		tsip_dialog_register_init(self);
	}
	return self;
}

static tsk_object_t* tsip_dialog_register_dtor(tsk_object_t * _self)
{ 
	tsip_dialog_register_t *self = _self;
	if(self){

		/* Cancel all timers */
		TSIP_DIALOG_TIMER_CANCEL(refresh);
		TSIP_DIALOG_TIMER_CANCEL(shutdown);

		/* DeInitialize base class (will cancel all transactions) */
		tsip_dialog_deinit(TSIP_DIALOG(self));

		// Delete resources
		TSK_OBJECT_SAFE_FREE(self->last_iRegister);

		TSK_DEBUG_INFO("*** REGISTER Dialog destroyed ***");
	}
	return self;
}

static int tsip_dialog_register_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	return tsip_dialog_cmp(obj1, obj2);
}

static const tsk_object_def_t tsip_dialog_register_def_s = 
{
	sizeof(tsip_dialog_register_t),
	tsip_dialog_register_ctor, 
	tsip_dialog_register_dtor,
	tsip_dialog_register_cmp, 
};
const tsk_object_def_t *tsip_dialog_register_def_t = &tsip_dialog_register_def_s;
