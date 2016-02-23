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

/**@file tsip_transac.h
 * @brief SIP transaction layer.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSIP_TRANSAC_LAYER_H
#define TINYSIP_TRANSAC_LAYER_H

#include "tinysip_config.h"

#include "tsip.h"
#include "tinysip/transactions/tsip_transac.h"

#include "tsk_safeobj.h"
#include "tsk_list.h"

TSIP_BEGIN_DECLS

typedef struct tsip_transac_layer_s {
    TSK_DECLARE_OBJECT;

    const struct tsip_stack_s *stack;

    tsip_transacs_L_t *transactions;

    TSK_DECLARE_SAFEOBJ;
}
tsip_transac_layer_t;

typedef tsk_list_t tsip_transac_layers_L_t;
TINYSIP_GEXTERN const tsk_object_def_t *tsip_transac_layer_def_t;

tsip_transac_layer_t* tsip_transac_layer_create(tsip_stack_t* stack);

tsip_transac_t* tsip_transac_layer_new(const tsip_transac_layer_t *self, tsk_bool_t isCT, const tsip_message_t* msg, tsip_transac_dst_t* dst);
int tsip_transac_layer_remove(tsip_transac_layer_t *self, const tsip_transac_t *transac);
int tsip_transac_layer_cancel_by_dialog(tsip_transac_layer_t *self, const struct tsip_dialog_s* dialog);

tsip_transac_t* tsip_transac_layer_find_client(const tsip_transac_layer_t *self, const tsip_message_t* message);
tsip_transac_t* tsip_transac_layer_find_server(const tsip_transac_layer_t *self, const tsip_message_t* message);

int tsip_transac_layer_handle_incoming_msg(const tsip_transac_layer_t *self, const tsip_message_t* message);

TSIP_END_DECLS

#endif /* TINYSIP_TRANSAC_LAYER_H */

