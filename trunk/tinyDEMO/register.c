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
#include "register.h"

#include "common.h"

extern context_t* context;

int register_handle_event(const struct tsip_event_s *sipevent)
{
	const tsip_register_event_t* reg_event = TSIP_REGISTER_EVENT(sipevent);
	session_t* session;
	tsip_ssession_id_t id;

	/* Find associated session */
	id = tsip_ssession_get_id(sipevent->ss);
	if(!(session = (session_t*)tsk_list_find_item_by_pred(context->sessions, pred_find_session_by_id, &id))){
		TSK_DEBUG_WARN("Failed to match session event.");
		return -1;
	}
	

	switch(reg_event->type){
		
		/* Informational */
		case tsip_o_register: /* Outgoing REGISTER */
		case tsip_o_unregister: /* Outgoing REGISTER */
			{	/* Request successfully sent (you cannot suppose that the remote peer has received the request) ==> Informational */
				TSK_DEBUG_INFO("Transport layer successfully sent (un)REGISTER request");
				break;
			}

		case tsip_ao_register: /* Answer to outgoing REGISTER */
			{
				TSK_DEBUG_INFO("Event: Answer to outgoing REGISTER. Code=%d", TSIP_RESPONSE_CODE(sipevent->sipmessage));
				if(TSIP_RESPONSE_IS_2XX(sipevent->sipmessage)){
					session->connected = tsk_true;
				}
				break;
			}
		
		case tsip_ao_unregister: /* Answer to outgoing unREGISTER */
			{
				TSK_DEBUG_INFO("Event: Answer to outgoing unREGISTER. Code=%d", TSIP_RESPONSE_CODE(sipevent->sipmessage));
				if(TSIP_RESPONSE_IS_2XX(sipevent->sipmessage)){
					session->connected = tsk_false;
				}
				break;
			}

		/* Server events (For whose dev. Server Side IMS Services) */
		case tsip_i_register: /* Incoming REGISTER */
		case tsip_i_unregister: /* Incoming unREGISTER */
		case tsip_ai_register: /* Answer to Incoming REGISTER */
		case tsip_ai_unregister: /* Answer to Incoming unREGISTER */
			{	
				TSK_DEBUG_WARN("Event not support by Client Framework.");
				break;
			}

		default:
			{	/* Any other event */
				TSK_DEBUG_WARN("%d not a valid SIP Registration event.", reg_event->type);
				break;
			}
	}

	return 0;
}


int register_handle_cmd(cmd_type_t cmd, const tsk_options_L_t* options)
{
	session_t* session = tsk_null;
	if(!(session = session_handle_cmd(cmd, options))){
		goto bail;
	}

	switch(cmd){
		case cmd_register:
			{	/* Send SIP REGISTER */
				tsip_action_REGISTER(session->handle,
					TSIP_ACTION_SET_NULL());
				break;
			}
		default:
			/* already handled by session_handle_cmd() */
			break;
	}

bail:
	TSK_OBJECT_SAFE_FREE(session);

	return 0;
}