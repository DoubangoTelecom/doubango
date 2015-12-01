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

/**@file tsip_dialog_invite.ect.c
 * @brief Explicit Communication Transfer (ECT) using IP Multimedia (IM) Core Network (CN) subsystem (3GPP TS 24.629)
 * The Explicit Communication transfer (ECT) service provides a party involved in a communication to transfer that
 * communication to a third party.
 * This code inplement Consultative transfer mode (A.2).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/dialogs/tsip_dialog_invite.h"

#include "tinysip/dialogs/tsip_dialog_invite.common.h"

#include "tinysip/parsers/tsip_parser_message.h"

#include "tinysip/headers/tsip_header_Refer_To.h"
#include "tinysip/headers/tsip_header_Referred_By.h"
#include "tinysip/headers/tsip_header_Refer_Sub.h"
#include "tinysip/headers/tsip_header_Supported.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tinymedia/tmedia_defaults.h"

#include "tsk_debug.h"

static int send_NOTIFY(tsip_dialog_invite_t *self, short code, const char* phrase);
static int send_REFER(tsip_dialog_invite_t *self, const char* to);
static short get_SipFragResponseCode(const tsip_request_t* notify);
static tsip_response_t * get_SipFragMessage(const tsip_request_t* notify);

extern int send_RESPONSE(tsip_dialog_invite_t *self, const tsip_request_t* request, short code, const char* phrase, tsk_bool_t force_sdp);
extern int send_ERROR(tsip_dialog_invite_t* self, const tsip_request_t* request, short code, const char* phrase, const char* reason);
extern int send_BYE(tsip_dialog_invite_t *self);

/* ======================== transitions ======================== */
static int x0400_Connected_2_oECTing_X_oECT(va_list *app);
static int x0400_oECTing_2_oECTing_X_i2xx(va_list *app); // should be 202?
static int x0400_oECTing_2_Connected_X_i3456(va_list *app);
static int x0400_oECTing_2_oECTing_X_iNOTIFY(va_list *app); // Notify 1xx
static int x0400_oECTing_2_Connected_X_iNOTIFY(va_list *app); // Notify 200_to_699

static int x0400_Connected_2_iECTreq_X_iREFER(va_list *app);
static int x0400_iECTreq_2_Connected_X_reject(va_list *app);
static int x0400_iECTreq_2_iECTing_X_accept(va_list *app);
static int x0400_iECTing_2_iECTing_X_1xxfNOTIFY(va_list *app);
static int x0400_iECTing_2_Connected_X_23456fNOTIFY(va_list *app);
#define x0400_Connected_2_Connected_X_fREFER tsk_null

/* ======================== conds ======================== */
static tsk_bool_t _fsm_cond_is_resp2REFER(tsip_dialog_invite_t* self, tsip_message_t* message)
{
	return TSIP_RESPONSE_IS_TO_REFER(message);
}
static tsk_bool_t _fsm_cond_is_1xxNOTIFY(tsip_dialog_invite_t* self, tsip_request_t* notify)
{
	short code = get_SipFragResponseCode(notify);
	return (code >= 100 && code <= 199);
}
static tsk_bool_t _fsm_cond_is_23456NOTIFY(tsip_dialog_invite_t* self, tsip_request_t* notify)
{
	short code = get_SipFragResponseCode(notify);
	return (code >= 200 && code <= 699);
}
static tsk_bool_t _fsm_cond_is_fREFER(tsip_dialog_invite_t* self, tsip_request_t* refer)
{
	const tsip_header_Refer_To_t* Refer_To = (const tsip_header_Refer_To_t*)tsip_message_get_header(refer, tsip_htype_Refer_To);
	return (!Refer_To || !Refer_To->uri);
}
static tsk_bool_t _fsm_cond_is_1xxfNOTIFY(tsip_dialog_invite_t* self, tsip_response_t* response)
{
	short code = TSIP_RESPONSE_CODE(response);
	return (code >= 100 && code <= 199);
}
static tsk_bool_t _fsm_cond_is_23456fNOTIFY(tsip_dialog_invite_t* self, tsip_response_t* response)
{
	short code = TSIP_RESPONSE_CODE(response);
	return (code >= 200 && code <= 699);
}

int tsip_dialog_invite_ect_init(tsip_dialog_invite_t *self)
{
	tsk_fsm_set(TSIP_DIALOG_GET_FSM(self),

		/*=======================
		* === Outgoing Transfer === 
		*/
		// Connected -> (oREFER) -> oECTing
		TSK_FSM_ADD_ALWAYS(_fsm_state_Connected, _fsm_action_oECT, _fsm_state_oECTing, x0400_Connected_2_oECTing_X_oECT, "x0400_Connected_2_oECTing_X_oECT"),
		// oECTing -> (i2xx REFER) -> oECTing
		TSK_FSM_ADD(_fsm_state_oECTing, _fsm_action_i2xx, _fsm_cond_is_resp2REFER, _fsm_state_oECTing, x0400_oECTing_2_oECTing_X_i2xx, "x0400_oECTing_2_oECTing_X_i2xx"),
		// oECTing -> (i300-699 REFER) -> Connected
		TSK_FSM_ADD(_fsm_state_oECTing, _fsm_action_i300_to_i699, _fsm_cond_is_resp2REFER, _fsm_state_Connected, x0400_oECTing_2_Connected_X_i3456, "x0400_ECTing_2_Connected_X_i36"),
		// oECTing -> (iNotify 1xx sipfrag) -> oECTing
		TSK_FSM_ADD(_fsm_state_oECTing, _fsm_action_iNOTIFY, _fsm_cond_is_1xxNOTIFY, _fsm_state_oECTing, x0400_oECTing_2_oECTing_X_iNOTIFY, "x0400_oECTing_2_oECTing_X_iNOTIFY"),
		// oECTing -> (iNotify 23456 sipfrag) -> Connected
		TSK_FSM_ADD(_fsm_state_oECTing, _fsm_action_iNOTIFY, _fsm_cond_is_23456NOTIFY, _fsm_state_Connected, x0400_oECTing_2_Connected_X_iNOTIFY, "x0400_oECTing_2_Connected_X_iNOTIFY"),

		/*=======================
		* === Incoming Transfer === 
		*/
		// Connected -> (iREFER invalid) -> Connected
		TSK_FSM_ADD(_fsm_state_Connected, _fsm_action_iREFER, _fsm_cond_is_fREFER, _fsm_state_Connected, x0400_Connected_2_Connected_X_fREFER, "x0400_Connected_2_Connected_X_fREFER"),
		// Connected -> (iREFER) -> iECTreq
		TSK_FSM_ADD_ALWAYS(_fsm_state_Connected, _fsm_action_iREFER, _fsm_state_iECTreq, x0400_Connected_2_iECTreq_X_iREFER, "x0400_Connected_2_iECTreq_X_iREFER"),
		// iECTreq -> (reject) -> Connected
		TSK_FSM_ADD_ALWAYS(_fsm_state_iECTreq, _fsm_action_iECT_REJECT, _fsm_state_Connected, x0400_iECTreq_2_Connected_X_reject, "x0400_iECTreq_2_Connected_X_reject"),
		// iECTreq -> (accept) -> iECTing
		TSK_FSM_ADD_ALWAYS(_fsm_state_iECTreq, _fsm_action_iECT_ACCEPT, _fsm_state_iECTing, x0400_iECTreq_2_iECTing_X_accept, "x0400_iECTreq_2_iECTing_X_accept"),
		// iECTing -> (1xx lnotify) -> iECTing
		TSK_FSM_ADD(_fsm_state_iECTing, _fsm_action_iECT_lNOTIFY, _fsm_cond_is_1xxfNOTIFY, _fsm_state_iECTing, x0400_iECTing_2_iECTing_X_1xxfNOTIFY, "x0400_iECTing_2_iECTing_X_1xxfNOTIFY"),
		// iECTing -> (23456 lnotify) -> Connected
		TSK_FSM_ADD(_fsm_state_iECTing, _fsm_action_iECT_lNOTIFY, _fsm_cond_is_23456fNOTIFY, _fsm_state_Connected, x0400_iECTing_2_Connected_X_23456fNOTIFY, "x0400_iECTing_2_Connected_X_23456fNOTIFY"),
		
		TSK_FSM_ADD_NULL());

	return 0;
}


//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------

// Connected -> (oREFER) -> oECTing
static int x0400_Connected_2_oECTing_X_oECT(va_list *app)
{
	int ret;
	tsip_dialog_invite_t *self;
	const tsip_action_t* action;

	self = va_arg(*app, tsip_dialog_invite_t *);
	va_arg(*app, const tsip_message_t *);
	action = va_arg(*app, const tsip_action_t *);

	ret = send_REFER(self, action->ect.to);

	if(ret == 0){
		TSIP_DIALOG_INVITE_SIGNAL(self, tsip_o_ect_trying, 
			tsip_event_code_dialog_request_sent, "Call Transfer Initiated", tsk_null);
	}
	else; //Must never happen

	return ret;
}

// ECTing -> (i2xx REFER) -> oECTing
static int x0400_oECTing_2_oECTing_X_i2xx(va_list *app)
{
	tsip_dialog_invite_t *self;
	const tsip_response_t* response;
	const tsip_header_Refer_Sub_t* Refer_Sub;

	self = va_arg(*app, tsip_dialog_invite_t *);
	response = va_arg(*app, const tsip_message_t *);
	Refer_Sub = (const tsip_header_Refer_Sub_t*)tsip_message_get_header(response, tsip_htype_Refer_Sub);
	if(Refer_Sub){
		self->refersub = Refer_Sub->sub;
	}
	if(tsip_message_required(response, "norefersub")){
		self->required.norefersub = tsk_true;
	}

	return TSIP_DIALOG_INVITE_SIGNAL(self, tsip_o_ect_accepted, 
			TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), response);
}

// oECTing -> (i300-699 REFER) -> Connected
static int x0400_oECTing_2_Connected_X_i3456(va_list *app)
{
	tsip_dialog_invite_t *self;
	const tsip_response_t* response;

	self = va_arg(*app, tsip_dialog_invite_t *);
	response = va_arg(*app, const tsip_message_t *);

	return TSIP_DIALOG_INVITE_SIGNAL(self, tsip_o_ect_failed, 
			TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), response);
}

// oECTing -> (iNotify 1xx sipfrag) -> oECTing
static int x0400_oECTing_2_oECTing_X_iNOTIFY(va_list *app)
{
	int ret = 0;
	tsip_dialog_invite_t *self;
	const tsip_request_t* notify;
	tsip_response_t *sipfrag = tsk_null;

	self = va_arg(*app, tsip_dialog_invite_t *);
	notify = va_arg(*app, const tsip_message_t *);

	sipfrag = get_SipFragMessage(notify);
	if(sipfrag){
		send_RESPONSE(self, notify, 200, "Ok", tsk_false);
		TSIP_DIALOG_INVITE_SIGNAL(self, tsip_o_ect_notify, 
			TSIP_RESPONSE_CODE(sipfrag), TSIP_RESPONSE_PHRASE(sipfrag), notify);
	}

	TSK_OBJECT_SAFE_FREE(sipfrag);

	return ret;
}

// oECTing -> (iNotify 23456 sipfrag) -> Connected
static int x0400_oECTing_2_Connected_X_iNOTIFY(va_list *app)
{
	int ret = 0;
	tsip_dialog_invite_t *self;
	const tsip_request_t* notify;
	tsip_response_t *sipfrag = tsk_null;

	self = va_arg(*app, tsip_dialog_invite_t *);
	notify = va_arg(*app, const tsip_message_t *);

	sipfrag = get_SipFragMessage(notify);
	if(sipfrag){
		send_RESPONSE(self, notify, 200, "Ok", tsk_false);
		TSIP_DIALOG_INVITE_SIGNAL(self, tsip_o_ect_notify, 
			TSIP_RESPONSE_CODE(sipfrag), TSIP_RESPONSE_PHRASE(sipfrag), notify);
	}

	TSK_OBJECT_SAFE_FREE(sipfrag);

	return ret;
}


// Connected -> (iREFER) -> iECTreq
static int x0400_Connected_2_iECTreq_X_iREFER(va_list *app)
{
	int ret;
	tsip_dialog_invite_t *self;
	const tsip_request_t* refer;
	
	self = va_arg(*app, tsip_dialog_invite_t *);
	refer = va_arg(*app, const tsip_message_t *);

	TSK_OBJECT_SAFE_FREE(self->last_iRefer);
	self->last_iRefer = tsk_object_ref((tsk_object_t*)refer);
	
	ret = send_RESPONSE(self, self->last_iRefer, 100, "Asking for Transfer", tsk_false);
	TSIP_DIALOG_INVITE_SIGNAL(self, tsip_i_ect_requested, 
			tsip_event_code_dialog_request_incoming, "Incoming Request", self->last_iRefer);
	
	return ret;
}

// iECTreq -> (reject) -> Connected
static int x0400_iECTreq_2_Connected_X_reject(va_list *app)
{
	int ret;
	tsip_dialog_invite_t *self;
	const tsip_action_t* action;
	short code;
	const char* phrase;
	char* reason = tsk_null;

	self = va_arg(*app, tsip_dialog_invite_t *);
	va_arg(*app, const tsip_message_t *);
	action = va_arg(*app, const tsip_action_t *);

	// Send Reject
	code = action->line_resp.code>=300 ? action->line_resp.code : 603;
	phrase = action->line_resp.phrase ? action->line_resp.phrase : "Decline Transfer";
	tsk_sprintf(&reason, "SIP; cause=%hi; text=\"%s\"", code, phrase);
	ret = send_ERROR(self, self->last_iRefer, code, phrase, reason);
	TSK_FREE(reason);

	return ret;
}

// iECTreq -> (accept) -> iECTing
static int x0400_iECTreq_2_iECTing_X_accept(va_list *app)
{
	int ret;
	tsip_dialog_invite_t *self;
	const tsip_header_Refer_To_t* Refer_To;
	const tsip_action_t* action;

	self = va_arg(*app, tsip_dialog_invite_t *);
	va_arg(*app, const tsip_message_t *);
	action = va_arg(*app, const tsip_action_t *);

	// Send 200 OK
	ret = send_RESPONSE(self, self->last_iRefer, 200, "Transfering", tsk_false);
	Refer_To = (const tsip_header_Refer_To_t*)tsip_message_get_header(self->last_iRefer, tsip_htype_Refer_To); // Not null: already checked
	// Make call to the referToUri
	TSK_OBJECT_SAFE_FREE(self->ss_transf);
	self->ss_transf = tsip_ssession_create(TSIP_DIALOG_GET_STACK(self),
		TSIP_SSESSION_SET_PARENT_ID(TSIP_DIALOG_GET_SS(self)->id),
		TSIP_SSESSION_SET_NULL());
#if TSIP_UNDER_WINDOWS // because of DirectShow Attach()
	self->ss_transf->media.type = tmedia_audio;
#else
	self->ss_transf->media.type = self->msession_mgr ? self->msession_mgr->type : tmedia_defaults_get_media_type();
#endif
	self->ss_transf->owner = tsk_false;// not owned by the end-user -> will be destroyed as soon as the dialog dtor is called
	
	if(ret == 0){
		ret = tsip_invite_event_signal(tsip_i_ect_newcall, self->ss_transf, 
						tsip_event_code_dialog_request_outgoing, "ECTing", self->last_iRefer);
	}
	
	ret = tsip_ssession_set(self->ss_transf,
		TSIP_SSESSION_SET_TO_OBJ(Refer_To->uri),
		TSIP_SSESSION_SET_NULL());
	ret = tsip_api_invite_send_invite(self->ss_transf, self->ss_transf->media.type,
		TSIP_ACTION_SET_NULL());	

	return ret;
}


// iECTing -> (1xx lnotify) -> iECTing
static int x0400_iECTing_2_iECTing_X_1xxfNOTIFY(va_list *app)
{
	int ret;
	tsip_dialog_invite_t *self;
	const tsip_response_t* response;

	self = va_arg(*app, tsip_dialog_invite_t *);
	response = va_arg(*app, const tsip_message_t *);
	
	// send NOTIFY (event if norefersub enabled) and alert user
	ret = send_NOTIFY(self, TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response));

	return ret;
}

// iECTing -> (23456 lnotify) -> Connected
static int x0400_iECTing_2_Connected_X_23456fNOTIFY(va_list *app)
{
	int ret;
	tsip_dialog_invite_t *self;
	const tsip_response_t* response;
	short code;
	
	self = va_arg(*app, tsip_dialog_invite_t *);
	response = va_arg(*app, const tsip_message_t *);
	code = TSIP_RESPONSE_CODE(response);
	
	// send NOTIFY (event if norefersub enabled) and alert user
	ret = send_NOTIFY(self, code, TSIP_RESPONSE_PHRASE(response));
	
	if(code >= 200 && code <= 299){
		TSIP_DIALOG_INVITE_SIGNAL(self, tsip_i_ect_completed, 
			TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), self->last_iRefer);
		// hang up the call
		ret = send_BYE(self);
	}
	else{
		TSIP_DIALOG_INVITE_SIGNAL(self, tsip_i_ect_failed, 
			TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), self->last_iRefer);
	}
	
	return ret;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++


static int send_NOTIFY(tsip_dialog_invite_t *self, short code, const char* phrase)
{
	tsip_request_t *notify = tsk_null;
	int ret = -1;

	if((notify = tsip_dialog_request_new(TSIP_DIALOG(self), "NOTIFY"))){
		char* sipfrag = tsk_null;
		tsk_sprintf(&sipfrag, "%s %hi %s\r\n", TSIP_MESSAGE_VERSION_DEFAULT, code, phrase);
		ret = tsip_message_add_content(notify, "message/sipfrag", sipfrag, tsk_strlen(sipfrag));
		ret = tsip_dialog_request_send(TSIP_DIALOG(self), notify);
		if(ret == 0){
			TSIP_DIALOG_INVITE_SIGNAL(self, tsip_o_ect_notify, code, phrase, notify);
		}
		TSK_FREE(sipfrag);
		TSK_OBJECT_SAFE_FREE(notify);		
	}
	else{
		TSK_DEBUG_ERROR("Failed to create request");
	}
	return ret;
}

static int send_REFER(tsip_dialog_invite_t *self, const char* to)
{
	int ret = 0;
	tsip_request_t *refer = tsk_null;
	tsip_uri_t* toUri = tsk_null;

	if(!self || !to){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!(toUri = tsip_uri_parse(to, tsk_strlen(to)))){
		TSK_DEBUG_ERROR("Failed to parse %s", to);
		return -1;
	}
	else{
		// tsk_params_add_param(&toUri->params, "method", "INVITE");
	}

	if((refer = tsip_dialog_request_new(TSIP_DIALOG(self), "REFER"))){
		tsk_istr_t cid;
		tsk_strrandom(&cid);
		/* Add headers */
		tsip_message_add_headers(refer,
			TSIP_HEADER_REFER_TO_VA_ARGS(toUri),
			TSIP_HEADER_REFERRED_BY_VA_ARGS(TSIP_DIALOG_GET_STACK(self)->identity.impu, cid),
			TSIP_HEADER_REFER_SUB_VA_ARGS(self->refersub),
			tsk_null);
		if(self->supported.norefersub){
			tsip_message_add_headers(refer,
				TSIP_HEADER_SUPPORTED_VA_ARGS("norefersub"),
				tsk_null);
		}
		
		ret = tsip_dialog_request_send(TSIP_DIALOG(self), refer);
		TSK_OBJECT_SAFE_FREE(refer);
	}
	
	TSK_OBJECT_SAFE_FREE(toUri);
	return ret;
}

static tsip_response_t * get_SipFragMessage(const tsip_request_t* notify)
{
	tsip_response_t *sipfrag = tsk_null;
	if(TSIP_MESSAGE_HAS_CONTENT(notify) && tsk_striequals(notify->Content_Type->type, "message/sipfrag")){
		tsk_ragel_state_t state;
		tsk_bool_t ret;
		char* content = tsk_null;

		// sipfrag is a "tsip_message_t" with an extra \r\n
		content = tsk_strndup(notify->Content->data, notify->Content->size);
		if(tsk_strLastIndexOf(content, tsk_strlen(content), "\r\n") != tsk_strlen(content) - 2){//Hack for XXX buggy client
			tsk_strcat(&content, "\r\n");
		}
		tsk_strcat(&content, "\r\n");
		
		tsk_ragel_state_init(&state, content, tsk_strlen(content));
		ret = tsip_message_parse(&state, &sipfrag, tsk_false);
		TSK_FREE(content);
		if(ret && TSIP_MESSAGE_IS_RESPONSE(sipfrag)){
			return sipfrag;
		}
		TSK_OBJECT_SAFE_FREE(sipfrag);
	}
	return sipfrag;
}

static short get_SipFragResponseCode(const tsip_request_t* notify)
{
	tsip_response_t *sipfrag = get_SipFragMessage(notify);
	short code = 0;
	if(sipfrag){
		code = TSIP_RESPONSE_CODE(sipfrag);
		TSK_OBJECT_SAFE_FREE(sipfrag);
	}
	return code;
}