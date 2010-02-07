/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsip_dialog_layer.c
 * @brief SIP dialog layer.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/dialogs/tsip_dialog_layer.h"

#include "tinysip/dialogs/tsip_dialog_register.h"
#include "tinysip/dialogs/tsip_dialog_message.h"

#include "tinysip/transactions/tsip_transac_layer.h"


/**
 * @fn	static tsip_dialog_t* tsip_dialog_layer_find_dialog(tsip_dialog_layer_t *self,
 * 		tsip_dialog_type_t type, const tsip_operation_handle_t *operation)
 *
 * @brief	Internal function used to find a dialog by type and  
 *
 * @author	Mamadou
 * @date	1/3/2010
 *
 * @param [in,out]	self		If non-null, the self. 
 * @param	type				The type. 
 * @param [in,out]	operation	If non-null, the operation. 
 *
 * @return	null if it fails, else. 
**/
static tsip_dialog_t* tsip_dialog_layer_find_dialog(tsip_dialog_layer_t *self, tsip_dialog_type_t type, const tsip_operation_handle_t *operation)
{
	const tsk_list_item_t *item;
	
	tsk_safeobj_lock(self);
	item = tsk_list_find_item_by_data(self->dialogs, operation);
	tsk_safeobj_unlock(self);
	
	if(item && item->data)
	{
		tsip_dialog_t *dialog = item->data;
		if(dialog && dialog->type == type)
		{
			return dialog;
		}
	}
	return 0;
}

const tsip_dialog_t* tsip_dialog_layer_find(const tsip_dialog_layer_t *self, const char* callid, const char* to_tag, const char* from_tag)
{
	tsip_dialog_t *ret = 0;
	tsip_dialog_t *dialog;
	tsk_list_item_t *item;

	tsk_safeobj_lock(self);

	tsk_list_foreach(item, self->dialogs)
	{
		dialog = item->data;
		if( tsk_strequals(dialog->callid, callid) 
			&& tsk_strequals(dialog->tag_local, to_tag)
			&& tsk_strequals(dialog->tag_remote, from_tag)
			)
		{
			ret = dialog;
			break;
		}
	}

	tsk_safeobj_unlock(self);

	return ret;
}

/**
 * @fn	int tsip_dialog_layer_register(tsip_dialog_layer_t *self,
 * 		const tsip_operation_handle_t *operation)
 *
 * @brief	Performs SIP/IMS registration operation. Action initiated by the stack layer.
 *
 * @author	Mamadou
 * @date	1/3/2010
 *
 * @param [in,out]	self		The dialog layer used to perform the operation.
 * @param [in,out]	operation	A pointer to the operation to perform. 
 *
 * @return	Zero if succeed and non-zero error code otherwise. 
**/
int tsip_dialog_layer_register(tsip_dialog_layer_t *self, const tsip_operation_handle_t *operation)
{
	int ret = -1;
	tsip_dialog_register_t *dialog;

	if(self)
	{
		//tsk_safeobj_lock(self);
		dialog = (tsip_dialog_register_t*)tsip_dialog_layer_find_dialog(self, tsip_dialog_register, operation);
		//tsk_safeobj_unlock(self);
		
		if(dialog)
		{
			
		}
		else
		{
			dialog = TSIP_DIALOG_REGISTER_CREATE(self->stack, operation);
			ret = tsip_dialog_register_start(dialog);
			tsk_list_push_back_data(self->dialogs, (void**)&dialog);
		}
	}
	return ret;
}


int tsip_dialog_layer_hangupAll(tsip_dialog_layer_t *self)
{
	if(self)
	{
		tsk_list_item_t *item;
		tsip_dialog_t *dialog;
		tsk_list_foreach(item, self->dialogs)
		{
			dialog = item->data;
			tsip_dialog_hangup(dialog);
		}
		return 0;
	}

	return -1;
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
	const tsip_dialog_t* dialog;

	//tsk_safeobj_lock(self);
	dialog = tsip_dialog_layer_find(self, message->Call_ID->value, message->To->tag, message->From->tag);
	//tsk_safeobj_unlock(self);
	
	if(dialog)
	{
		dialog->callback(dialog, tsip_dialog_msg, message);
	}
	else
	{
		const tsip_transac_layer_t *layer_transac = tsip_stack_get_transac_layer(self->stack);
		/*const*/ tsip_transac_t* transac;

		if(TSIP_MESSAGE_IS_REQUEST(message))
		{
			if(tsk_strequals("MESSAGE", TSIP_REQUEST_METHOD(message)))
			{
				tsip_dialog_message_t *dlg_msg = TSIP_DIALOG_MESSAGE_CREATE(self->stack, 0);

				transac = tsip_transac_layer_new(layer_transac, TSIP_FALSE, message);
				if(transac)
				{
					TSIP_TRANSAC(transac)->dialog = TSIP_DIALOG(dlg_msg);
				}
				tsk_list_push_back_data(self->dialogs, (void**)&dlg_msg);
			}

			else if(tsk_strequals("INVITE", TSIP_REQUEST_METHOD(message)))
			{
			}

			// ....

			if(transac)
			{
				ret = tsip_transac_start(transac, message);
			}
				
		}
	}

	

	return ret;
}





//========================================================
//	Dialog layer object definition
//
static void* tsip_dialog_layer_create(void * self, va_list * app)
{
	tsip_dialog_layer_t *layer = self;
	if(layer)
	{
		layer->stack = va_arg(*app, const tsip_stack_handle_t *);
		layer->dialogs = TSK_LIST_CREATE();

		tsk_safeobj_init(layer);
	}
	return self;
}

static void* tsip_dialog_layer_destroy(void * self)
{ 
	tsip_dialog_layer_t *layer = self;
	if(layer)
	{
		TSK_OBJECT_SAFE_FREE(layer->dialogs);

		tsk_safeobj_deinit(layer);
	}
	return self;
}

static int tsip_dialog_layer_cmp(const void *obj1, const void *obj2)
{
	return -1;
}

static const tsk_object_def_t tsip_dialog_layer_def_s = 
{
	sizeof(tsip_dialog_layer_t),
	tsip_dialog_layer_create, 
	tsip_dialog_layer_destroy,
	tsip_dialog_layer_cmp, 
};
const void *tsip_dialog_layer_def_t = &tsip_dialog_layer_def_s;
