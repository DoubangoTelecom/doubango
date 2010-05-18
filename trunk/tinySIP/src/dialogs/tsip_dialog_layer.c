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
#include "tinysip/dialogs/tsip_dialog_layer.h"

#include "tinysip/dialogs/tsip_dialog_invite.h"
#include "tinysip/dialogs/tsip_dialog_message.h"
#include "tinysip/dialogs/tsip_dialog_options.h"
#include "tinysip/dialogs/tsip_dialog_publish.h"
#include "tinysip/dialogs/tsip_dialog_register.h"
#include "tinysip/dialogs/tsip_dialog_subscribe.h"

#include "tinysip/transactions/tsip_transac_layer.h"
#include "tinysip/transports/tsip_transport_layer.h"

#include "tsk_debug.h"

extern tsip_ssession_handle_t *tsip_ssession_create_2(const tsip_stack_t* stack, const struct tsip_message_s* message);

/*== Predicate function to find dialog by type */
static int pred_find_dialog_by_type(const tsk_list_item_t *item, const void *type)
{
	if(item && item->data){
		tsip_dialog_t *dialog = item->data;
		return (dialog->type - *((tsip_dialog_type_t*)type));
	}
	return -1;
}

/*== Predicate function to find dialog by not type */
static int pred_find_dialog_by_not_type(const tsk_list_item_t *item, const void *type)
{
	if(item && item->data){
		tsip_dialog_t *dialog = item->data;
		if(dialog->type != *((tsip_dialog_type_t*)type)){
			return 0;
		}
	}
	return -1;
}

tsip_dialog_layer_t* tsip_dialog_layer_create(tsip_stack_t* stack)
{
	return tsk_object_new(tsip_dialog_layer_def_t, stack);
}

// MUST tsk_object_unref(ret)
tsip_dialog_t* tsip_dialog_layer_find_by_ss(tsip_dialog_layer_t *self, const tsip_ssession_handle_t *ss)
{
	tsip_dialog_t *ret = 0;
	tsip_dialog_t *dialog;
	tsk_list_item_t *item;

	tsk_safeobj_lock(self);

	tsk_list_foreach(item, self->dialogs){
		dialog = item->data;
		if( tsip_ssession_get_id(dialog->ss) == tsip_ssession_get_id(ss) ){
			ret = dialog;
			break;
		}
	}

	tsk_safeobj_unlock(self);

	return tsk_object_ref(ret);
}

tsip_dialog_t* tsip_dialog_layer_find(const tsip_dialog_layer_t *self, const char* callid, const char* to_tag, const char* from_tag, tsk_bool_t *cid_matched)
{
	tsip_dialog_t *ret = tsk_null;
	tsip_dialog_t *dialog;
	tsk_list_item_t *item;

	*cid_matched = tsk_false;

	tsk_safeobj_lock(self);

	tsk_list_foreach(item, self->dialogs){
		dialog = item->data;
		if(tsk_strequals(dialog->callid, callid)){
			*cid_matched = tsk_true;
			if(tsk_strequals(dialog->tag_local, from_tag) && tsk_strequals(dialog->tag_remote, to_tag)){
				ret = tsk_object_ref(dialog);
				break;
			}
		}
	}

	tsk_safeobj_unlock(self);

	return ret;
}

/** Hangup all dialogs staring by REGISTER  */
int tsip_dialog_layer_shutdownAll(tsip_dialog_layer_t *self)
{
	if(self){
		tsk_list_item_t *item;
		tsip_dialog_t *dialog;
		tsip_dialog_type_t regtype = tsip_dialog_REGISTER;

		if(!self->shutdown.inprogress){
			self->shutdown.inprogress = tsk_true;
			self->shutdown.condwait = tsk_condwait_create();
		}
		
		tsk_safeobj_lock(self);
		if(tsk_list_count(self->dialogs, pred_find_dialog_by_not_type, &regtype)){
			/* There are non-register dialogs ==> phase-1 */
			goto phase1;
		}
		else if(tsk_list_count(self->dialogs, pred_find_dialog_by_type, &regtype)){
			/* There are one or more register dialogs ==> phase-2 */
			goto phase2;
		}
		else{
			tsk_safeobj_unlock(self);
			goto done;
		}

phase1:
		/* Phase 1 - shutdown all except register */
		TSK_DEBUG_INFO("== Shutting down - Phase-1 started ==");
		tsk_list_foreach(item, self->dialogs){
			dialog = item->data;
			if(dialog->type != tsip_dialog_REGISTER){
				tsip_dialog_shutdown(dialog, tsk_null);
			}
		}
		tsk_safeobj_unlock(self);
		
		/* wait until phase-1 is completed */
		tsk_condwait_timedwait(self->shutdown.condwait, TSIP_DIALOG_SHUTDOWN_TIMEOUT);
		
		/* lock and goto phase2 */
		tsk_safeobj_lock(self);
		goto phase2;

phase2:
		/* Phase 2 - unregister */
		TSK_DEBUG_INFO("== Shutting down - Phase-2 started ==");
		self->shutdown.phase2 = tsk_true;
		tsk_list_foreach(item, self->dialogs){
			dialog = item->data;
			if(dialog->type == tsip_dialog_REGISTER){
				tsip_dialog_shutdown(dialog, tsk_null);
			}
		}
		tsk_safeobj_unlock(self);

		/* wait until phase-2 is completed */
		tsk_condwait_timedwait(self->shutdown.condwait, TSIP_DIALOG_SHUTDOWN_TIMEOUT);

done:
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
				if((dialog = (tsip_dialog_t*)tsip_dialog_options_create(ss))){
					ret = tsk_object_ref(dialog);
					tsk_list_push_back_data(self->dialogs, (void**)&dialog);
				}
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
	if(dialog && self){
		tsip_dialog_type_t regtype = tsip_dialog_REGISTER;
		tsk_safeobj_lock(self);
		
		/* remove the dialog */
		tsk_list_remove_item_by_data(self->dialogs, dialog);
		
		/* whether shutting down? */
		if(self->shutdown.inprogress){
			if(self->shutdown.phase2){ /* Phase 2 */
				if(TSK_LIST_IS_EMPTY(self->dialogs)){
					/* Alert only if all dialogs have been removed. */
					TSK_DEBUG_INFO("== Shutting down - Phase-2 completed ==");
					tsk_condwait_broadcast(self->shutdown.condwait);
				}
			}
			else{ /* Phase 1 */
				if(tsk_list_count(self->dialogs, pred_find_dialog_by_not_type, &regtype) == 0){
					/* Alert only if all dialogs except REGISTER have been removed. */
					TSK_DEBUG_INFO("== Shutting down - Phase-1 completed ==");
					tsk_condwait_broadcast(self->shutdown.condwait);
				}
			}
		}

		tsk_safeobj_unlock(self);

		return 0;
	}

	return -1;
}

int tsip_dialog_layer_handle_incoming_msg(const tsip_dialog_layer_t *self, const tsip_message_t* message)
{
	int ret = -1;
	tsk_bool_t cid_matched;
	tsip_dialog_t* dialog;
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
		transac = tsip_transac_layer_new(layer_transac, tsk_false, message, TSIP_DIALOG(dialog));
		tsk_object_unref(dialog);
	}
	else{		
		if(TSIP_MESSAGE_IS_REQUEST(message)){
			tsip_ssession_t* ss = tsk_null;
			tsip_dialog_t* newdialog = tsk_null;
			
			switch(message->request_type){
				case tsip_MESSAGE:
					{	/* Server incoming MESSAGE */
						if((ss = tsip_ssession_create_2(self->stack, message))){
							newdialog = (tsip_dialog_t*)tsip_dialog_message_create(ss);
						}
						break;
					}

				case tsip_INVITE:
					{	/* incoming INVITE */
						if((ss = tsip_ssession_create_2(self->stack, message))){
							newdialog = (tsip_dialog_t*)tsip_dialog_invite_create(ss);
						}
						break;
					}

				default:
					{
						break;
					}
			}//switch

			if(newdialog){
				transac = tsip_transac_layer_new(layer_transac, tsk_false, message, newdialog);
				tsk_list_push_back_data(self->dialogs, (void**)&newdialog); /* add new dialog to the layer */
			}

			/* The dialog will become the owner of the SIP session
			* => when destoyed => SIP session will be destroyed, unless the user-end takes ownership() */
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

		/* condwait */
		if(layer->shutdown.condwait){
			tsk_condwait_destroy(&layer->shutdown.condwait);
		}

		tsk_safeobj_deinit(layer);

		TSK_DEBUG_INFO("*** Dialog Layer destroyed ***");
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
