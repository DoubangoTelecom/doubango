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

extern ctx_t* ctx;
extern const session_t* session_handle_cmd(cmd_type_t , const opts_L_t*);

int invite_handle_event(const tsip_event_t *_event)
{
	const tsip_invite_event_t* inv_event = TSIP_INVITE_EVENT(_event);
	const session_t* session;
	tsip_ssession_id_t sid;

	/* Find associated session */
	sid = tsip_ssession_get_id(_event->ss);
	if(!(session = session_get_by_sid(ctx->sessions, sid))){
		TSK_DEBUG_WARN("Failed to match session event.");
		return -1;
	}

	switch(inv_event->type){
		// ============================
		//	Sip Events
		//
		case tsip_i_invite:
			TSK_DEBUG_INFO("invite_handle_event(tsip_i_invite)");
			break;
		case tsip_ao_invite:
			TSK_DEBUG_INFO("invite_handle_event(tsip_ao_invite)");
			break;
		
		/* BYE */
		case tsip_i_bye:
			TSK_DEBUG_INFO("invite_handle_event(tsip_i_bye)");
			break;
		case tsip_ao_bye:
			TSK_DEBUG_INFO("invite_handle_event(tsip_ao_bye)");
			break;
		
		/* UPDATE */
		case tsip_i_update:
			TSK_DEBUG_INFO("invite_handle_event(tsip_i_update)");
			break;
		case tsip_ao_update:
			TSK_DEBUG_INFO("invite_handle_event(tsip_ao_update)");
			break;
		
		/* Explicit Call Transfer (ECT) */
		case tsip_o_ect_ok:
			TSK_DEBUG_INFO("invite_handle_event(tsip_o_ect_ok)");
			break;
		case tsip_o_ect_nok:
			TSK_DEBUG_INFO("invite_handle_event(tsip_o_ect_nok)");
			break;
		case tsip_i_ect:
			TSK_DEBUG_INFO("invite_handle_event(tsip_i_ect)");
			break;
		
		// ============================
		//	Media Events
		//
		
		/* Media State */
		case tsip_m_connected:
			TSK_DEBUG_INFO("invite_handle_event(tsip_m_connected)");
			break;
		case tsip_m_terminated:
			TSK_DEBUG_INFO("invite_handle_event(tsip_m_terminated)");
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

	return 0;
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
			{	/* Make Audio call */
				tsip_action_handle_t* action_config = action_get_config(opts);
				tsip_action_INVITE(session->handle, tmedia_audio,
					TSIP_ACTION_SET_CONFIG(action_config),
					/* Any other TSIP_ACTION_SET_*() macros */
					TSIP_ACTION_SET_NULL());
				TSK_OBJECT_SAFE_FREE(action_config);
				break;
			}


		case cmd_hold:
			{	/* Put the session on hold state */
				tsip_action_handle_t* action_config = action_get_config(opts);
				tsip_action_HOLD(session->handle, tmedia_all,
					TSIP_ACTION_SET_CONFIG(action_config),
					/* Any other TSIP_ACTION_SET_*() macros */
					TSIP_ACTION_SET_NULL());
				TSK_OBJECT_SAFE_FREE(action_config);
				break;
			}
		case cmd_resume:
			{	/* Put the session on hold state */
				tsip_action_handle_t* action_config = action_get_config(opts);
				tsip_action_RESUME(session->handle, tmedia_all,
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
