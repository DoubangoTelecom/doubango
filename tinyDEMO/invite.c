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
#include "invite.h"

#include "tinymsrp.h"


extern ctx_t* ctx;
extern const session_t* session_handle_cmd(cmd_type_t , const opts_L_t*);

static int invite_msrp_cb(const tmsrp_event_t* _event);

int invite_handle_event(const tsip_event_t *_event)
{
	const tsip_invite_event_t* inv_event = TSIP_INVITE_EVENT(_event);
	const session_t* session;
	tsip_ssession_id_t sid;
	int ret = 0;

	/* Find associated session */
	sid = tsip_ssession_get_id(_event->ss);
	if(!(session = session_get_by_sid(ctx->sessions, sid))){
		if(inv_event->type == tsip_i_newcall){
			/* It's a new incoming call */
			if(!tsip_ssession_have_ownership(_event->ss)){
				session_t* _session;
				if((_session = session_server_create(st_invite, _event->ss)) && (session = _session)){
					tsk_list_push_back_data(ctx->sessions, (void**)&_session);
				}
				else{
					TSK_DEBUG_ERROR("Failed to create \"sever-side-session\".");
					ret = -3;
					goto bail;
				}
			}
		}
		else{
			/* it's or own session and we fail to match it ==> should never happen */
			TSK_DEBUG_ERROR("Failed to match session event.");
			ret = -2;
			goto bail;
		}
	}

	if(!session || !session->handle){
		/* guard ==> should never happen */
		TSK_DEBUG_ERROR("Null session.");
		goto bail;
	}

	switch(inv_event->type){
		// ============================
		//	Sip Events
		//
		case tsip_i_newcall:
			{	/* New call */
				tmedia_type_t media_type = tsip_ssession_get_mediatype(session);
				tsip_api_common_accept(session->handle,
					TSIP_ACTION_SET_NULL());
				/*tsip_api_common_reject(session->handle,
					TSIP_ACTION_SET_NULL());*/
				break;
			}
		case tsip_i_request:
			TSK_DEBUG_INFO("invite_handle_event(tsip_i_request)");
			break;
		case tsip_ao_request:
			TSK_DEBUG_INFO("invite_handle_event(tsip_ao_request)");
			break;
		
		/* Explicit Call Transfer (ECT) */
		case tsip_o_ect_trying:
		case tsip_o_ect_accepted:
		case tsip_o_ect_completed:
		case tsip_o_ect_failed:
		case tsip_o_ect_notify:
		case tsip_i_ect_requested:
		case tsip_i_ect_newcall:
		case tsip_i_ect_completed:
		case tsip_i_ect_failed:
		case tsip_i_ect_notify:
			TSK_DEBUG_INFO("ECT event");
			break;
		
		// ============================
		//	Media Events
		//
		
		/* Early Media started */
		case tsip_m_early_media:
			TSK_DEBUG_INFO("invite_handle_event(tsip_m_early_media)");
			break;

		/* 3GPP TS 24.610: Communication Hold  */
		case tsip_m_local_hold_ok:
			TSK_DEBUG_INFO("invite_handle_event(tsip_m_local_hold_ok)");
			break;
		case tsip_m_local_hold_nok:
			TSK_DEBUG_INFO("invite_handle_event(tsip_m_local_hold_nok)");
			break;
		case tsip_m_local_resume_ok:
			TSK_DEBUG_INFO("invite_handle_event(tsip_m_local_resume_ok)");
			break;
		case tsip_m_local_resume_nok:
			TSK_DEBUG_INFO("invite_handle_event(tsip_m_local_resume_nok)");
			break;
		case tsip_m_remote_hold:
			TSK_DEBUG_INFO("invite_handle_event(tsip_m_remote_hold)");
			break;
		case tsip_m_remote_resume:
			TSK_DEBUG_INFO("invite_handle_event(tsip_m_remote_resume)");
			break;

		default:
			break;
	}

bail:
	return ret;
}

tsip_ssession_id_t invite_handle_cmd(cmd_type_t cmd, const opts_L_t* opts)
{
	const session_t* session = tsk_null;
	tsip_ssession_id_t id = TSIP_SSESSION_INVALID_ID;

	if(!(session = session_handle_cmd(cmd, opts))){
		goto bail;
	}
	else{
		id = tsip_ssession_get_id(session->handle);
	}

	switch(cmd){
		case cmd_audio:
		case cmd_audiovideo:
			{	/* Make Audio/Video call */
				tsip_action_handle_t* action_config = action_get_config(opts);
				tsip_api_invite_send_invite(session->handle, (cmd == cmd_audio) ? tmedia_audio : (tmedia_audio|tmedia_video),
					TSIP_ACTION_SET_CONFIG(action_config),
					/* Any other TSIP_ACTION_SET_*() macros */
					TSIP_ACTION_SET_NULL());
				TSK_OBJECT_SAFE_FREE(action_config);
				break;
			}

		case cmd_file:
			{	/* Send file using MSRP protocol */
				tsip_action_handle_t* action_config = action_get_config(opts);
				const opt_t* opt = opt_get_by_type(opts, opt_path); // existance already checked
				/* Set Callback function */
				tsip_ssession_set(session->handle,
					TSIP_SSESSION_SET_MEDIA(
						TSIP_MSESSION_SET_MSRP_CB(invite_msrp_cb),
						TSIP_MSESSION_SET_NULL()
					),
					TSIP_SSESSION_SET_NULL());

				/* Send INVITE */
				tsip_api_invite_send_invite(session->handle, tmedia_msrp,
					TSIP_ACTION_SET_CONFIG(action_config),
					
					TSIP_ACTION_SET_MEDIA(
						TMEDIA_SESSION_MSRP_SET_STR("file-path", opt->value),
						TMEDIA_SESSION_MSRP_SET_STR("accept-types", "application/octet-stream"),
						//TMEDIA_SESSION_MSRP_SET_STR("accept-wrapped-types", "application/octet-stream"),
						//TMEDIA_SESSION_MSRP_SET_STR("file-selector", "name:\"test.sn\" type:application/octet-stream size:3740 hash:sha-1:27:D0:AE:39:48:77:37:1D:FD:39:7E:2D:78:2F:BC:7B:94:48:29:81"),
						//TMEDIA_SESSION_MSRP_SET_STR("file-disposition", "attachment"),
						//TMEDIA_SESSION_MSRP_SET_STR("file-date", "creation:2010-02-13T17:50:31.763Z"),
						//TMEDIA_SESSION_MSRP_SET_STR("file-icon", "cid:test@doubango.org"),

						TMEDIA_SESSION_SET_NULL()
					),
					
					TSIP_ACTION_SET_NULL());
				TSK_OBJECT_SAFE_FREE(action_config);
				break;
			}

		case cmd_dtmf:
			{
				const opt_t* opt = opt_get_by_type(opts, opt_event); // existance already checked
				tsip_action_handle_t* action_config = action_get_config(opts);
				tsip_api_invite_send_dtmf(session->handle, atoi(opt->value), 
					TSIP_ACTION_SET_CONFIG(action_config),
					/* Any other TSIP_ACTION_SET_*() macros */
					TSIP_ACTION_SET_NULL());
				TSK_OBJECT_SAFE_FREE(action_config);
				break;
			}

		case cmd_ect:
			{	/* Explict Call Transfer */
				const opt_t* opt = opt_get_by_type(opts, opt_to); // existance already checked
				tsip_action_handle_t* action_config = action_get_config(opts);
				tsip_api_invite_send_ect(session->handle, opt? opt->value : "sip:anonymous@example.com", 
					TSIP_ACTION_SET_CONFIG(action_config),
					/* Any other TSIP_ACTION_SET_*() macros */
					TSIP_ACTION_SET_NULL());
				TSK_OBJECT_SAFE_FREE(action_config);
				break;
			}

		case cmd_hold:
			{	/* Put the session on hold state */
				tsip_action_handle_t* action_config = action_get_config(opts);
				tsip_api_invite_send_hold(session->handle, tmedia_all,
					TSIP_ACTION_SET_CONFIG(action_config),
					/* Any other TSIP_ACTION_SET_*() macros */
					TSIP_ACTION_SET_NULL());
				TSK_OBJECT_SAFE_FREE(action_config);
				break;
			}
		case cmd_resume:
			{	/* Put the session on hold state */
				tsip_action_handle_t* action_config = action_get_config(opts);
				tsip_api_invite_send_resume(session->handle, tmedia_all,
					TSIP_ACTION_SET_CONFIG(action_config),
					/* Any other TSIP_ACTION_SET_*() macros */
					TSIP_ACTION_SET_NULL());
				TSK_OBJECT_SAFE_FREE(action_config);
				break;
			}
		default:
			/* already handled by session_handle_cmd() */
			break;
	}

bail:
	return id;
}


int invite_msrp_cb(const tmsrp_event_t* _event)
{
	const session_t* session = _event->callback_data;

	if(_event->message){
		if(_event->message->end_line.cflag == '$'){
			TSK_DEBUG_INFO("Last Chunck");
		}
		switch(_event->message->type){
			case tmsrp_request:
				{	/* MSRP Request */
					
					
					break;
				}
			case tmsrp_response:
				{	/* MSRP Response */
					if(_event->message->ByteRange){
						TSK_DEBUG_INFO("MSRP Response code=%hi %lld-%lld/%lld", TMSRP_RESPONSE_CODE(_event->message),
							_event->message->ByteRange->start, _event->message->ByteRange->end, _event->message->ByteRange->total);
					}
					break;
				}
			default:
				TSK_DEBUG_ERROR("Invalid MSRP message");
				break;
		}
	}

	return 0;
}