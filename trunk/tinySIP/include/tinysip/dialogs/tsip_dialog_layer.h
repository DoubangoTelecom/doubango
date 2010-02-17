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
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsip_dialog.h
 * @brief SIP dialog layer.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_DIALOG_LAYER_H
#define TINYSIP_DIALOG_LAYER_H

#include "tinysip_config.h"

#include "tsip.h"
#include "tinysip/dialogs/tsip_dialog.h"

#include "tsk_safeobj.h"
#include "tsk_list.h"

TSIP_BEGIN_DECLS

#define TSIP_DIALOG_LAYER_CREATE(stack)				tsk_object_new(tsip_dialog_layer_def_t, stack)

typedef struct tsip_dialog_layer_s
{
	TSK_DECLARE_OBJECT;

	const tsip_stack_handle_t *stack;

	tsip_dialogs_L_t *dialogs;

	TSK_DECLARE_SAFEOBJ;
}
tsip_dialog_layer_t;

typedef tsk_list_t tsip_dialog_layers_L_t;

//int tsip_dialog_layer_register(tsip_dialog_layer_t *self, const tsip_operation_handle_t *operation);

tsip_dialog_t* tsip_dialog_layer_find_by_op(tsip_dialog_layer_t *self, tsip_dialog_type_t type, const tsip_operation_handle_t *operation);

int tsip_dialog_layer_hangupAll(tsip_dialog_layer_t *self);
int tsip_dialog_layer_remove(tsip_dialog_layer_t *self, const tsip_dialog_t *dialog);

int tsip_dialog_layer_handle_incoming_msg(const tsip_dialog_layer_t *self, const tsip_message_t* message);

TINYSIP_GEXTERN const void *tsip_dialog_layer_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_DIALOG_LAYER_H */

