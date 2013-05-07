/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSIP_DIALOG_LAYER_H
#define TINYSIP_DIALOG_LAYER_H

#include "tinysip_config.h"

#include "tsip.h"
#include "tinysip/dialogs/tsip_dialog.h"

#include "tsk_condwait.h"
#include "tsk_safeobj.h"
#include "tsk_list.h"

TSIP_BEGIN_DECLS

typedef struct tsip_dialog_layer_s
{
	TSK_DECLARE_OBJECT;

	const tsip_stack_t *stack;

	tsip_dialogs_L_t *dialogs;

	struct{
		tsk_bool_t inprogress;
		tsk_bool_t phase2; /* whether unregistering? */
		tsk_condwait_handle_t* condwait;
	} shutdown;

	TSK_DECLARE_SAFEOBJ;
}
tsip_dialog_layer_t;

typedef tsk_list_t tsip_dialog_layers_L_t;

tsip_dialog_layer_t* tsip_dialog_layer_create(tsip_stack_t* stack);

TINYSIP_API tsip_dialog_t* tsip_dialog_layer_find_by_ss(tsip_dialog_layer_t *self, const tsip_ssession_handle_t *ss);
tsip_dialog_t* tsip_dialog_layer_find_by_ssid(tsip_dialog_layer_t *self, tsip_ssession_id_t ssid);
tsip_dialog_t* tsip_dialog_layer_find_by_callid(tsip_dialog_layer_t *self, const char* callid);
tsk_bool_t tsip_dialog_layer_have_dialog_with_callid(const tsip_dialog_layer_t *self, const char* callid);


TINYSIP_API int tsip_dialog_layer_shutdownAll(tsip_dialog_layer_t *self);
int tsip_dialog_layer_signal_stack_disconnected(tsip_dialog_layer_t *self);
int tsip_dialog_layer_signal_peer_disconnected(tsip_dialog_layer_t *self, const struct tsip_transport_stream_peer_s* peer);
int tsip_dialog_layer_remove_callid_from_stream_peers(tsip_dialog_layer_t *self, const char* callid);
tsip_dialog_t* tsip_dialog_layer_new(tsip_dialog_layer_t *self, tsip_dialog_type_t type, const tsip_ssession_t *ss);
int tsip_dialog_layer_remove(tsip_dialog_layer_t *self, const tsip_dialog_t *dialog);

int tsip_dialog_layer_handle_incoming_msg(const tsip_dialog_layer_t *self, tsip_message_t* message);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_dialog_layer_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_DIALOG_LAYER_H */

