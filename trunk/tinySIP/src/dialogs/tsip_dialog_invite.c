/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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

/**@file tsip_dialog_invite.c
 * @brief SIP dialog INVITE as per RFC 3261.
 * The SOA machine is designed as per RFC 3264 and draft-ietf-sipping-sip-offeranswer-12.
 * MMTel services implementation follow 3GPP TS 24.173.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/dialogs/tsip_dialog_invite.h"

#include "tinySIP/headers/tsip_header_RAck.h"
#include "tinySIP/headers/tsip_header_RSeq.h"

#include "tsk_debug.h"

// http://cdnet.stpi.org.tw/techroom/market/_pdf/2009/eetelecomm_09_009_OneVoiceProfile.pdf
// 3GPP TS 26.114 (MMTel): Media handling and interaction
// 3GPP TS 24.173 (MMTel): Supplementary Services
//
/* ======================== MMTel Supplementary Services ======================== 
3GPP TS 24.607 : Originating Identification Presentation
3GPP TS 24.608 : Terminating Identification Presentation
3GPP TS 24.607 : Originating Identification Restriction
3GPP TS 24.608 : Terminating Identification Restriction 

3GPP TS 24.604 : Communication Diversion Unconditional
3GPP TS 24.604 : Communication Diversion on not Logged
3GPP TS 24.604 : Communication Diversion on Busy 
3GPP TS 24.604 : Communication Diversion on not Reachable
3GPP TS 24.604 : Communication Diversion on No Reply
3GPP TS 24.611 : Barring of All Incoming Calls
3GPP TS 24.611 : Barring of All Outgoing Calls
3GPP TS 24.611 : Barring of Outgoing International Calls
3GPP TS 24.611 : Barring of Incoming Calls - When Roaming
3GPP TS 24.610 : Communication Hold 
3GPP TS 24.606 : Message Waiting Indication
3GPP TS 24.615 : Communication Waiting
3GPP TS 24.605 : Ad-Hoc Multi Party Conference
*/

#define DEBUG_STATE_MACHINE											1
#define TSIP_DIALOG_INVITE_SIGNAL(self, type, code, phrase, message)	\
	tsip_invite_event_signal(type, TSIP_DIALOG_GET_STACK(self), TSIP_DIALOG(self)->operation, code, phrase, message)

/* ======================== internal functions ======================== */
int send_INVITE(tsip_dialog_invite_t *self);
int send_PRACK(tsip_dialog_invite_t *self, const tsip_response_t* r1xx);
int tsip_dialog_invite_OnTerminated(tsip_dialog_invite_t *self);

/* ======================== transitions ======================== */
extern int c0000_Started_2_Outgoing_X_oINVITE(va_list *app);
extern int c0001_Outgoing_2_Connected_X_i2xx(va_list *app); // 2xx INVITE
extern int c0002_Outgoing_2_Terminated_X_i300_to_i699(va_list *app); // 300-699 INVITE
extern int c0003_Outgoing_2_Terminated_X_oCANCEL(va_list *app);

extern int s0000_Satrted_2_Incoming_X_iINVITE(va_list *app);
extern int s0000_Incoming_2_Connected_X_o2xx(va_list *app); // 2xx INVITE
extern int s0000_Incoming_2_Terminated_X_oCANCEL(va_list *app);

int x0000_Any_2_Trying_X_oINVITE(va_list *app);
int x0000_Any_2_Trying_X_oUPDATE(va_list *app);
int x0000_Any_2_Any_X_iINVITE(va_list *app);
int x0000_Any_2_Any_X_iUPDATE(va_list *app);
int x0000_Any_2_Any_X_i1xx(va_list *app);
int x0000_Any_2_Any_X_i2xx(va_list *app);
int x0000_Any_2_Any_X_i401_i407_i421_i494(va_list *app);
int x0000_Any_2_Any_X_iPRACK(va_list *app);
int x0000_Any_2_Any_X_iACK(va_list *app);
int x9998_Any_2_Any_X_transportError(va_list *app);
int x9999_Any_2_Any_X_Error(va_list *app);


/* ======================== conds ======================== */
tsk_bool_t _fsm_cond_is_resp2INVITE(tsip_dialog_invite_t* self, tsip_message_t* message)
{
	if(message->CSeq){
		return tsk_striequals(message->CSeq->method, "INVITE");
	}
	return tsk_false;
}

/* ======================== actions ======================== */
typedef enum _fsm_action_e
{
	_fsm_action_oINVITE,
	_fsm_action_iINVITE,
	_fsm_action_oUPDATE,
	_fsm_action_iUPDATE,
	_fsm_action_oCANCEL,
	_fsm_action_iCANCEL,
	_fsm_action_iPRACK,
	_fsm_action_oPRACK,
	_fsm_action_iACK,
	_fsm_action_oACK,

	_fsm_action_i1xx,
	_fsm_action_i2xx,
	_fsm_action_i300_to_i699,
	_fsm_action_i401_i407_i421_i494,

	_fsm_action_transporterror,
	_fsm_action_error,
}
_fsm_action_t;

/* ======================== states ======================== */
typedef enum _fsm_state_e
{
	_fsm_state_Started,
	_fsm_state_Outgoing,
	_fsm_state_Incoming,
	_fsm_state_Trying,
	_fsm_state_Connected,
	_fsm_state_Terminated
}
_fsm_state_t;

int tsip_dialog_invite_event_callback(const tsip_dialog_invite_t *self, tsip_dialog_event_type_t type, const tsip_message_t *msg)
{
	int ret = -1;

	switch(type)
	{
	case tsip_dialog_i_msg:
		{
			if(msg)
			{
				if(TSIP_MESSAGE_IS_RESPONSE(msg)) /* Response */
				{
					if(TSIP_RESPONSE_IS_1XX(msg)){ // 100-199
						ret = tsk_fsm_act(self->fsm, _fsm_action_i1xx, self, msg, self, msg);
					}
					else if(TSIP_RESPONSE_IS_2XX(msg)){ // 200-299
						ret = tsk_fsm_act(self->fsm, _fsm_action_i2xx, self, msg, self, msg);
					}
					else if(TSIP_RESPONSE_CODE(msg) == 401 || TSIP_RESPONSE_CODE(msg) == 407 || TSIP_RESPONSE_CODE(msg) == 421 || TSIP_RESPONSE_CODE(msg) == 494){ // 401,407,421,494
						ret = tsk_fsm_act(self->fsm, _fsm_action_i401_i407_i421_i494, self, msg, self, msg);
					}
					else if(TSIP_RESPONSE_IS_3456(msg)){ // 300-699
						ret = tsk_fsm_act(self->fsm, _fsm_action_i300_to_i699, self, msg, self, msg);
					}
					else; // Ignore
				}
				else /* Request */
				{
					if(TSIP_REQUEST_IS_INVITE(msg)){ // INVITE
						ret = tsk_fsm_act(self->fsm, _fsm_action_iINVITE, self, msg, self, msg);
					}
					else if(TSIP_REQUEST_IS_UPDATE(msg)){ // UPDATE
						ret = tsk_fsm_act(self->fsm, _fsm_action_iUPDATE, self, msg, self, msg);
					}
					else if(TSIP_REQUEST_IS_PRACK(msg)){ // PRACK
						ret = tsk_fsm_act(self->fsm, _fsm_action_iPRACK, self, msg, self, msg);
					}
					else if(TSIP_REQUEST_IS_ACK(msg)){ // ACK
						ret = tsk_fsm_act(self->fsm, _fsm_action_iACK, self, msg, self, msg);
					}
				}
			}
			break;
		}

	case tsip_dialog_canceled:
		{
			ret = tsk_fsm_act(self->fsm, _fsm_action_oCANCEL, self, msg, self, msg);
			break;
		}

	case tsip_dialog_terminated:
	case tsip_dialog_timedout:
	case tsip_dialog_error:
	case tsip_dialog_transport_error:
		{
			ret = tsk_fsm_act(self->fsm, _fsm_action_transporterror, self, msg, self, msg);
			break;
		}
	}

	return ret;
}


int tsip_dialog_invite_init(tsip_dialog_invite_t *self)
{
	//const tsk_param_t* param;

	/* Initialize the state machine. */
	tsk_fsm_set(self->fsm,

		/*=======================
		* === Started === 
		*/
		// Started -> (send INVITE) -> Outgoing
		TSK_FSM_ADD_ALWAYS(_fsm_state_Started, _fsm_action_oINVITE, _fsm_state_Outgoing, c0000_Started_2_Outgoing_X_oINVITE, "c0000_Started_2_Outgoing_X_oINVITE"),
		// Started -> (receive INVITE) -> Incoming
		TSK_FSM_ADD_ALWAYS(_fsm_state_Started, _fsm_action_iINVITE, _fsm_state_Incoming, s0000_Satrted_2_Incoming_X_iINVITE, "s0000_Satrted_2_Incoming_X_iINVITE"),
		// Started -> (Any) -> Started
		TSK_FSM_ADD_ALWAYS_NOTHING(_fsm_state_Started, "tsip_dialog_invite_Started_2_Started_X_any"),

		/*=======================
		* === Outgoing (Client) === 
		*/
		// Outgoing -> (receive 2xx INVITE) -> Connected
		TSK_FSM_ADD(_fsm_state_Outgoing, _fsm_action_i2xx, _fsm_cond_is_resp2INVITE, _fsm_state_Connected, c0001_Outgoing_2_Connected_X_i2xx, "c0001_Outgoing_2_Connected_X_i2xx"),
		// Outgoing -> (300-699 INVITE) -> Termined
		TSK_FSM_ADD(_fsm_state_Outgoing, _fsm_action_i300_to_i699, _fsm_cond_is_resp2INVITE, _fsm_state_Terminated, c0002_Outgoing_2_Terminated_X_i300_to_i699, "c0002_Outgoing_2_Terminated_X_i300_to_i699"),
		// Outgoing -> (send CANCEL) -> Terminated
		TSK_FSM_ADD_ALWAYS(_fsm_state_Outgoing, _fsm_action_oCANCEL, _fsm_state_Terminated, c0003_Outgoing_2_Terminated_X_oCANCEL, "c0003_Outgoing_2_Terminated_X_oCANCEL"),


		/*=======================
		* === Any === 
		*/
		// Any -> (1xx) -> Any
		TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_i1xx, tsk_fsm_state_any, x0000_Any_2_Any_X_i1xx, "x0000_Any_2_Any_X_i1xx"),
		// Any -> (transport error) -> Terminated
		TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_transporterror, _fsm_state_Terminated, x9998_Any_2_Any_X_transportError, "x9998_Any_2_Any_X_transportError"),
		// Any -> (transport error) -> Terminated
		TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_error, _fsm_state_Terminated, x9999_Any_2_Any_X_Error, "x9999_Any_2_Any_X_Error"),

		TSK_FSM_ADD_NULL());

	/* Sets callback function */
	TSIP_DIALOG(self)->callback = TSIP_DIALOG_EVENT_CALLBACK(tsip_dialog_invite_event_callback);

	/* Timers */

	return 0;
}

// start sending
int tsip_dialog_invite_start(tsip_dialog_invite_t *self)
{
	int ret = -1;
	if(self && !TSIP_DIALOG(self)->running){
		if(!(ret = tsk_fsm_act(self->fsm, _fsm_action_oINVITE, self, tsk_null, self, tsk_null))){
			TSIP_DIALOG(self)->running = tsk_true;
		}
	}
	return ret;
}



//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------


int x0000_Any_2_Trying_X_oREINVITE(va_list *app)
{
	return 0;
}

int x0000_Any_2_Trying_X_oUPDATE(va_list *app)
{
	return 0;
}

int x0000_Any_2_Any_X_iREINVITE(va_list *app)
{
	return 0;
}

int x0000_Any_2_Any_X_iUPDATE(va_list *app)
{
	return 0;
}

int x0000_Any_2_Any_X_i1xx(va_list *app)
{
	tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
	const tsip_response_t *r1xx = va_arg(*app, const tsip_response_t *);
	int ret = 0;

	/* RFC 3262 - 4 UAC Behavior
		If a provisional response is received for an initial request, and
		that response contains a Require header field containing the option
		tag 100rel, the response is to be sent reliably.  If the response is
		a 100 (Trying) (as opposed to 101 to 199), this option tag MUST be
		ignored, and the procedures below MUST NOT be used.
	*/
	if((TSIP_RESPONSE_CODE(r1xx) >= 101 && TSIP_RESPONSE_CODE(r1xx) <=199) && tsip_message_required(r1xx, "100rel")){
		if((ret = send_PRACK(self, r1xx))){
			return ret;
		}
	}

	return ret;
}

int x0000_Any_2_Any_X_i2xx(va_list *app)
{
	return 0;
}

int x0000_Any_2_Any_X_i401_i407_i421_i494(va_list *app)
{
	return 0;
}

int x0000_Any_2_Any_X_iPRACK(va_list *app)
{
	return 0;
}

int x0000_Any_2_Any_X_iACK(va_list *app)
{
	return 0;
}

int x9998_Any_2_Any_X_transportError(va_list *app)
{
	return 0;
}

int x9999_Any_2_Any_X_Error(va_list *app)
{
	return 0;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// send INVITE request
int send_INVITE(tsip_dialog_invite_t *self)
{
	int ret = -1;
	tsip_request_t *request = tsk_null;

	if(!self){
		goto bail;
	}

	if((request = tsip_dialog_request_new(TSIP_DIALOG(self), "INVITE"))){


		ret = tsip_dialog_request_send(TSIP_DIALOG(self), request);
		TSK_OBJECT_SAFE_FREE(request);
	}

bail:
	return ret;
}

int send_PRACK(tsip_dialog_invite_t *self, const tsip_response_t* r1xx)
{
	// "Require: 100rel\r\n" should be checked by the caller of this function
	int ret = -1;
	tsip_request_t *request = tsk_null;
	const tsip_header_RSeq_t* RSeq;

	if(!self || !r1xx || !r1xx->CSeq){
		goto bail;
	}

	/* RFC 3262 - 4 UAC Behavior
	Assuming the response is to be transmitted reliably, the UAC MUST
	create a new request with method PRACK.
	*/
	if(!(request = tsip_dialog_request_new(TSIP_DIALOG(self), "PRACK"))){
		goto bail;
	}




	/*	RFC 3262 - 4 UAC Behavior
	The UAC MUST maintain a sequence number that indicates the most recently
	received in-order reliable provisional response for the initial request.
	*/
	if((RSeq = (const tsip_header_RSeq_t*)tsip_message_get_header(r1xx, tsip_htype_RSeq))){

		/*	RFC 3262 - 4 UAC Behavior
		If the UAC receives another reliable provisional
		response to the same request, and its RSeq value is not one higher
		than the value of the sequence number, that response MUST NOT be
		acknowledged with a PRACK, and MUST NOT be processed further by the
		UAC.  An implementation MAY discard the response, or MAY cache the
		response in the hopes of receiving the missing responses.
		*/
		if(self->rseq >= RSeq->seq){
			TSK_DEBUG_WARN("1xx.RSeq value is not one higher than lastINVITE.RSeq.");
			goto bail;
		}
		self->rseq = RSeq->seq;
	}

	// Add RAck header
	TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_RACK_VA_ARGS(self->rseq, r1xx->CSeq->seq, r1xx->CSeq->method));

	ret = tsip_dialog_request_send(TSIP_DIALOG(self), request);
	
bail:
	TSK_OBJECT_SAFE_FREE(request);
	return ret;
}

int tsip_dialog_invite_OnTerminated(tsip_dialog_invite_t *self)
{
	TSK_DEBUG_INFO("=== INVITE Dialog terminated ===");

	/* Remove from the dialog layer. */
	return tsip_dialog_remove(TSIP_DIALOG(self));
}






















//========================================================
//	SIP dialog INVITE object definition
//
static void* tsip_dialog_invite_create(void * self, va_list * app)
{
	tsip_dialog_invite_t *dialog = self;
	if(dialog)
	{
		tsip_stack_handle_t *stack = va_arg(*app, tsip_stack_handle_t *);
		tsip_operation_handle_t *operation = va_arg(*app, tsip_operation_handle_t *);

		/* create FSM */
		dialog->fsm = TSK_FSM_CREATE(_fsm_state_Started, _fsm_state_Terminated);
		dialog->fsm->debug = DEBUG_STATE_MACHINE;
		tsk_fsm_set_callback_terminated(dialog->fsm, TSK_FSM_ONTERMINATED(tsip_dialog_invite_OnTerminated), (const void*)dialog);

		/* Initialize base class */
		tsip_dialog_init(TSIP_DIALOG(self), tsip_dialog_INVITE, stack, tsk_null, operation);

		/* Initialize the class itself */
		tsip_dialog_invite_init(self);
	}
	return self;
}

static void* tsip_dialog_invite_destroy(void * self)
{ 
	tsip_dialog_invite_t *dialog = self;
	if(dialog)
	{
		/* FSM */
		TSK_OBJECT_SAFE_FREE(dialog->fsm);

		/* DeInitialize base class */
		tsip_dialog_deinit(TSIP_DIALOG(self));
	}
	return self;
}

static int tsip_dialog_invite_cmp(const void *obj1, const void *obj2)
{
	return -1;
}

static const tsk_object_def_t tsip_dialog_invite_def_s = 
{
	sizeof(tsip_dialog_invite_t),
	tsip_dialog_invite_create, 
	tsip_dialog_invite_destroy,
	tsip_dialog_invite_cmp, 
};
const void *tsip_dialog_invite_def_t = &tsip_dialog_invite_def_s;
