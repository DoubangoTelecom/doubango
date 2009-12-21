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
 * @brief SIP dialogtion layer.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/dialogs/tsip_dialog_layer.h"













//========================================================
//	Dialog layer object definition
//
static void* tsip_dialog_layer_create(void * self, va_list * app)
{
	tsip_dialog_layer_t *layer = self;
	if(layer)
	{
		layer->stack = va_arg(*app, const tsip_stack_handle_t *);
	}
	return self;
}

static void* tsip_dialog_layer_destroy(void * self)
{ 
	tsip_dialog_layer_t *layer = self;
	if(layer)
	{
		TSK_LIST_SAFE_FREE(layer->dialogs);
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