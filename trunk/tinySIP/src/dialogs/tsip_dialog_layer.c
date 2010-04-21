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

/**@file tsip_dialog_layer.c
 * @brief SIP dialog layer.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySIP/dialogs/tsip_dialog_layer.h"

#include "tinySIP/dialogs/tsip_dialog_invite.h"
#include "tinySIP/dialogs/tsip_dialog_message.h"
#include "tinySIP/dialogs/tsip_dialog_publish.h"
#include "tinySIP/dialogs/tsip_dialog_register.h"
#include "tinySIP/dialogs/tsip_dialog_subscribe.h"

#include "tinySIP/transactions/tsip_transac_layer.h"
#include "tinySIP/transports/tsip_transport_layer.h"

#include "tsk_debug.h"

tsip_dialog_layer_t* tsip_dialog_layer_create(tsip_stack_t* stack)
{
	return tsk_object_new(tsip_dialog_layer_def_t, stack);
}

// MUST tsk_object_unref(ret)
tsip_dialog_t* tsip_dialog_layer_find_by_op(tsip_dialog_layer_t *self, const tsip_ssession_handle_t *ss)
{
	tsip_dialog_t *ret = 0;
	tsip_dialog_t *dialog;
	tsk_list_item_t *item;

	tsk_safeobj_lock(self);

	tsk_list_foreach(item, self->dialogs)
	{
		dialog = item->data;
		if( tsip_ssession_get_id(dialog->ss) == tsip_ssession_get_id(ss) ){
			ret = dialog;
			break;
		}
	}

	tsk_safeobj_unlock(self);

	return tsk_object_ref(ret);
}

const tsip_dialog_t* tsip_dialog_layer_find(const tsip_dialog_layer_t *self, const char* callid, const char* to_tag, const char* from_tag, tsk_bool_t *cid_matched)
{
	tsip_dialog_t *ret = 0;
	tsip_dialog_t *dialog;
	tsk_list_item_t *item;

	*cid_matched = tsk_false;

	tsk_safeobj_lock(self);

	tsk_list_foreach(item, self->dialogs)
	{
		dialog = item->data;
		if(tsk_strequals(dialog->callid, callid)){
			*cid_matched = tsk_true;
			if(tsk_strequals(dialog->tag_local, from_tag) && tsk_strequals(dialog->tag_remote, to_tag)){
				ret = dialog;
				break;
			}
		}
	}

	tsk_safeobj_unlock(self);

	return ret;
}

int tsip_dialog_layer_shutdownAllExceptRegister(tsip_dialog_layer_t *self)
{
	if(self)
	{
		tsk_list_item_t *item;
		tsip_dialog_t *dialog;
		tsk_list_foreach(item, self->dialogs)
		{
			dialog = item->data;
			if(dialog->type != tsip_dialog_REGISTER){
				//tsip_dialog_shutdown(dialog);
			}
		}
		return 0;
	}
	return -1;
}

int tsip_dialog_layer_hangupAll(tsip_dialog_layer_t *self)
{
	if(self)
	{
		tsk_list_item_t *item = 0;
		tsip_dialog_t *dialog;

		tsk_safeobj_lock(self);

		tsk_list_foreach(item, self->dialogs)
		{
			dialog = item->data;
			tsip_dialog_hangup(dialog, tsk_null);
		}

		tsk_safeobj_unlock(self);
		return 0;
	}
	return -1;
}

tsip_dialog_t* tsip_dialog_layer_new(tsip_dialog_layer_t *self, tsip_dialog_type_t type, const tsip_ssession_t *ss)
{
	tsip_dialog_t* ret = tsk_null;
	tsip_dialog_t* dialog;
	if(!self){
		goto bail;
	}

	switch(type){
		case tsip_dialog_INVITE:
			{
				break;
			}
		case tsip_dialog_MESSAGE:
			{
				if((dialog = (tsip_dialog_t*)tsip_dialog_message_create(ss))){
					ret = tsk_object_ref(dialog);
					tsk_list_push_back_data(self->dialogs, (void**)&dialog);
				}
				break;
			}
		case tsip_dialog_OPTIONS:
			{
				break;
			}
		case tsip_dialog_PUBLISH:
			{
				if((dialog = (tsip_dialog_t*)tsip_dialog_publish_create(ss))){
					ret = tsk_object_ref(dialog);
					tsk_list_push_back_data(self->dialogs, (void**)&dialog);
				}
				break;
			}
		case tsip_dialog_REGISTER:
			{
				if((dialog = (tsip_dialog_t*)tsip_dialog_register_create(ss))){
					ret = tsk_object_ref(dialog);
					tsk_list_push_back_data(self->dialogs, (void**)&dialog);
				}
				break;
			}
		case tsip_dialog_SUBSCRIBE:
			{
				if((dialog = (tsip_dialog_t*)tsip_dialog_subscribe_create(ss))){
					ret = tsk_object_ref(dialog);
					tsk_list_push_back_data(self->dialogs, (void**)&dialog);
				}
				break;
			}

		default:
			{
				TSK_DEBUG_ERROR("Dialog type not supported.");
				break;
			}
	}

bail:
	return ret;
}

int tsip_dialog_layer_remove(tsip_dialog_layer_t *self, const tsip_dialog_t *dialog)
{
	if(dialog && self)
	{
		tsk_safeobj_lock(self);
		tsk_list_remove_item_by_data(self->dialogs, dialog);
		tsk_safeobj_unlock(self);

		return 0;
	}

	return -1;
}

int tsip_dialog_layer_handle_incoming_msg(const tsip_dialog_layer_t *self, const tsip_message_t* message)
{
	int ret = -1;
	tsk_bool_t cid_matched;
	const tsip_dialog_t* dialog;
	tsip_transac_t* transac = tsk_null;
	const tsip_transac_layer_t *layer_transac = self->stack->layer_transac;

	if(!layer_transac){
		goto bail;
	}

	//tsk_safeobj_lock(self);
	dialog = tsip_dialog_layer_find(self, message->Call_ID->value, 
		TSIP_MESSAGE_IS_RESPONSE(message) ? message->To->tag : message->From->tag, 
		TSIP_MESSAGE_IS_RESPONSE(message) ? message->From->tag : message->To->tag, &cid_matched);
	//tsk_safeobj_unlock(self);
	
	if(dialog){
		dialog->callback(dialog, tsip_dialog_i_msg, message);
		transac = tsip_transac_layer_new(layer_transac, tsk_false, message, TSIP_DIALOG(dialog));
	}
	else{		
		if(TSIP_MESSAGE_IS_REQUEST(message)){
			tsip_ssession_t* ss = tsip_ssession_create_2(self->stack, message);
			tsip_dialog_t* newdialog = tsk_null;
			
			// The incoming request will be passed to the dialog by the server transaction (tsip_dialog_i_msg)
			// As the request will be recived from the callback function, the dialog MUST not be started
			
			/* user do not own the session */
			ss->owner = tsk_false;
			
			switch(message->request_type){
				case tsip_MESSAGE:
					{
						newdialog = (tsip_dialog_t*)tsip_dialog_message_create(ss);
						break;
					}

				case tsip_INVITE:
					{
						newdialog = (tsip_dialog_t*)tsip_dialog_invite_create(ss);
						break;
					}

				default:
					{
						break;
					}
			}//switch

			if(newdialog){
				transac = tsip_transac_layer_new(layer_transac, tsk_false, message, newdialog);
				tsk_list_push_back_data(self->dialogs, (void**)&newdialog);
			}

			TSK_OBJECT_SAFE_FREE(ss);
		}
	}

	if(transac){
		ret = tsip_transac_start(transac, message);
		tsk_object_unref(transac);
	}
	else if(TSIP_MESSAGE_IS_REQUEST(message)){ /* No transaction match for the SIP request */
		const tsip_transport_layer_t *layer;
		tsip_response_t* response;

		if((layer = self->stack->layer_transport))
		{	
			if(cid_matched){ /* We are receiving our own message. */
				response = tsip_response_new(482, "Loop Detected (Check your iFCs)", message);
				if(response && !response->To->tag){/* Early dialog? */
					response->To->tag = tsk_strdup("doubango");
				}
			}
			else{
				response = tsip_response_new(481, "Dialog/Transaction Does Not Exist", message);
			}
			if(response){
				ret = tsip_transport_layer_send(layer, response->firstVia ? response->firstVia->branch : "no-branch", response);
				TSK_OBJECT_SAFE_FREE(response);
			}
		}
	}
	
bail:
	return ret;
}





//========================================================
//	Dialog layer object definition
//
static tsk_object_t* tsip_dialog_layer_ctor(tsk_object_t * self, va_list * app)
{
	tsip_dialog_layer_t *layer = self;
	if(layer){
		layer->stack = va_arg(*app, const tsip_stack_t *);
		layer->dialogs = tsk_list_create();

		tsk_safeobj_init(layer);
	}
	return self;
}

static tsk_object_t* tsip_dialog_layer_dtor(tsk_object_t * self)
{ 
	tsip_dialog_layer_t *layer = self;
	if(layer){
		TSK_OBJECT_SAFE_FREE(layer->dialogs);

		tsk_safeobj_deinit(layer);
	}
	return self;
}

static int tsip_dialog_layer_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	return -1;
}

static const tsk_object_def_t tsip_dialog_layer_def_s = 
{
	sizeof(tsip_dialog_layer_t),
	tsip_dialog_layer_ctor, 
	tsip_dialog_layer_dtor,
	tsip_dialog_layer_cmp, 
};
const tsk_object_def_t *tsip_dialog_layer_def_t = &tsip_dialog_layer_def_s;
