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
#include "publish.h"

extern ctx_t* ctx;

int publish_handle_event(const tsip_event_t *sipevent)
{
	const tsip_publish_event_t* pub_event = TSIP_PUBLISH_EVENT(sipevent);
	const session_t* session;
	tsip_ssession_id_t sid;

	/* Find associated session */
	sid = tsip_ssession_get_id(sipevent->ss);
	if(!(session = session_get_by_sid(ctx->sessions, sid))){
		TSK_DEBUG_WARN("Failed to match session event.");
		return -1;
	}

	switch(pub_event->type){
		
		/* Informational */
		case tsip_o_publish: /* Outgoing PUBLISH */
		case tsip_o_unpublish: /* Outgoing PUBLISH */
			{	/* Request successfully sent (you cannot suppose that the remote peer has received the request) ==> Informational */
				TSK_DEBUG_INFO("Transport layer successfully sent (un)PUBLISH request");
				break;
			}

		case tsip_ao_publish: /* Answer to outgoing PUBLISH */
			{
				TSK_DEBUG_INFO("Event: Answer to outgoing PUBLISH. Code=%d", TSIP_RESPONSE_CODE(sipevent->sipmessage));
				if(TSIP_RESPONSE_IS_2XX(sipevent->sipmessage)){
					SESSION(session)->connected = tsk_true;
				}
				break;
			}
		
		case tsip_ao_unpublish: /* Answer to outgoing unPUBLISH */
			{
				TSK_DEBUG_INFO("Event: Answer to outgoing unPUBLISH. Code=%d", TSIP_RESPONSE_CODE(sipevent->sipmessage));
				if(TSIP_RESPONSE_IS_2XX(sipevent->sipmessage)){
					SESSION(session)->connected = tsk_false;
				}
				break;
			}

		/* Server events (For whose dev. Server Side IMS Services) */
		case tsip_i_publish: /* Incoming PUBLISH */
		case tsip_i_unpublish: /* Incoming unPUBLISH */
		case tsip_ai_publish: /* Answer to Incoming PUBLISH */
		case tsip_ai_unpublish: /* Answer to Incoming unPUBLISH */
			{	
				TSK_DEBUG_WARN("Event not support by Client Framework.");
				break;
			}

		default:
			{	/* Any other event */
				TSK_DEBUG_WARN("%d not a valid SIP Subscription event.", pub_event->type);
				break;
			}
	}

	return 0;
}

tsip_ssession_id_t publish_handle_cmd(cmd_type_t cmd, const opts_L_t* opts)
{
	const session_t* session = tsk_null;
	tsip_ssession_id_t id = TSIP_SSESSION_INVALID_ID;
	const opt_t* opt;

	if(!(session = session_handle_cmd(cmd, opts))){
		return -1;
	}
	else{
		id = tsip_ssession_get_id(session->handle);
	}

	switch(cmd){
		case cmd_publish:
			{	/* Send SIP PUBLISH */
				if((opt = opt_get_by_type(opts, opt_payload))){
					tsip_action_PUBLISH(session->handle,
						TSIP_ACTION_SET_PAYLOAD(opt->value, tsk_strlen(opt->value)),
						TSIP_ACTION_SET_NULL());
				}
				else{
					tsip_action_PUBLISH(session->handle,
						TSIP_ACTION_SET_NULL());
				}
				break;
			}
		default:
			/* already handled by session_handle_cmd() */
			break;
	}

	return id;
}
