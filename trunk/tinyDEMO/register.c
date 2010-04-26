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

int register_handle_event(struct context_s* context, const struct tsip_event_s *sipevent)
{
	const tsip_register_event_t* reg_event = TSIP_REGISTER_EVENT(sipevent);

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
				TSK_DEBUG_INFO("Event: Answer to outgoing REGISTER.");
				break;
			}
		
		case tsip_ao_unregister: /* Answer to outgoing unREGISTER */
			{
				TSK_DEBUG_INFO("Event: Answer to outgoing unREGISTER.");
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


int register_handle_cmd(struct context_s* context, cmd_type_t cmd, const tsk_options_L_t* options)
{
	tsip_ssession_handle_t* ss = tsk_null;
	const tsk_option_t* option;
	const tsk_list_item_t* item;
	tsk_param_t* param;
	int ret = 0;

	/* Check if there is a Register session with is Id */
	if((option = tsk_options_get_option_by_id(options, opt_sid))){
		tsip_ssession_id_t id = atoi(option->value);
		if((item = tsk_list_find_item_by_pred(context->sessions, pred_find_session_by_id, &id))){
			ss = (tsip_ssession_handle_t*)item->data;
		}
	}

	/* === Command === */
	switch(cmd){
		case cmd_register:
			{
				if(!ss){ /* Create registeration */
					tsip_ssession_handle_t* _ss = tsip_ssession_create(context->stack->ims_stack,
						TSIP_SSESSION_SET_NULL());

					ss = _ss;
					tsk_list_push_back_data(context->sessions, (void**)&_ss);
				}
				break;
			}
		default:
			{
				TSK_DEBUG_WARN("Registration: Cannot handle this command [%d]", cmd);
				goto bail;
			}
	} /* switch */

	if(!ss){
		TSK_DEBUG_ERROR("SIP Session is Null");
		goto bail;
	}

	/* === Options === */
	tsk_list_foreach(item, options){
		option = item->data;
		
		switch(option->id){

			case opt_caps:
				{
					if((param = tsk_params_parse_param(option->value, tsk_strlen(option->value)))){
						ret = tsip_ssession_set(ss, TSIP_SSESSION_SET_CAPS(param->name, param->value),
							TSIP_SSESSION_SET_NULL());
					}
					break;
				}
			case opt_from:
				{
					break;
				}
			case opt_header:
				{
					if((param = tsk_params_parse_param(option->value, tsk_strlen(option->value)))){
						ret = tsip_ssession_set(ss, TSIP_SSESSION_SET_HEADER(param->name, param->value),
							TSIP_SSESSION_SET_NULL());
					}
					break;
				}
			default:
				{
					TSK_DEBUG_WARN("Registration: %d not valid option.");
					break;
				}
		}

	} /* foreach */

	if(cmd==cmd_register){
		ret = tsip_action_REGISTER(ss,
			TSIP_ACTION_SET_NULL());
	}

bail:
	return ret;
}