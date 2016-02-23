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

/**@file tsip_transac.c
 * @brief SIP transaction base class as per RFC 3261 subclause 17.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/transactions/tsip_transac.h"

#include "tinysip/transports/tsip_transport_layer.h"
#include "tinysip/transactions/tsip_transac_layer.h"

#include "tinysip/transactions/tsip_transac_ist.h"
#include "tinysip/transactions/tsip_transac_nist.h"
#include "tinysip/transactions/tsip_transac_nict.h"
#include "tinysip/transactions/tsip_transac_ict.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

static tsk_object_t* tsip_transac_dst_ctor(tsk_object_t * _self, va_list * app)
{
    tsip_transac_dst_t *dst = _self;
    if(dst) {

    }
    return _self;
}
static tsk_object_t* tsip_transac_dst_dtor(tsk_object_t * _self)
{
    tsip_transac_dst_t *dst = _self;
    if(dst) {
        TSK_OBJECT_SAFE_FREE(dst->stack);
        switch(dst->type) {
        case tsip_transac_dst_type_dialog: {
            TSK_OBJECT_SAFE_FREE(dst->dialog.dlg);
            break;
        }
        case tsip_transac_dst_type_net: {
            break;
        }
        }
    }
    return _self;
}
static const tsk_object_def_t tsip_transac_dst_def_s = {
    sizeof(tsip_transac_dst_t),
    tsip_transac_dst_ctor,
    tsip_transac_dst_dtor,
    tsk_null,
};
const tsk_object_def_t *tsip_transac_dst_def_t = &tsip_transac_dst_def_s;

static struct tsip_transac_dst_s* tsip_transac_dst_create(tsip_transac_dst_type_t type, struct tsip_stack_s* stack)
{
    struct tsip_transac_dst_s* dst = tsk_object_new(tsip_transac_dst_def_t);
    if(dst) {
        dst->type = type;
        dst->stack = tsk_object_ref(stack);
    }
    return dst;
}

struct tsip_transac_dst_s* tsip_transac_dst_dialog_create(tsip_dialog_t *dlg)
{
    struct tsip_transac_dst_s* dst;
    if((dst =  tsip_transac_dst_create(tsip_transac_dst_type_dialog, TSIP_DIALOG_GET_STACK(dlg)))) {
        dst->dialog.dlg = tsk_object_ref(dlg);
    }
    return dst;
}

struct tsip_transac_dst_s* tsip_transac_dst_net_create(struct tsip_stack_s* stack)
{
    struct tsip_transac_dst_s* dst;
    if((dst =  tsip_transac_dst_create(tsip_transac_dst_type_net, stack))) {
    }
    return dst;
}

static int tsip_transac_dst_deliver(struct tsip_transac_dst_s* self, tsip_dialog_event_type_t event_type, const tsip_message_t *msg)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    switch(self->type) {
    case tsip_transac_dst_type_dialog: {
        return self->dialog.dlg->callback(
                   self->dialog.dlg,
                   event_type,
                   msg
               );
    }
    case tsip_transac_dst_type_net: {
        if(!msg) {
            TSK_DEBUG_ERROR("Message is null");
            return -1;
        }

        // all messages coming from WebSocket transport have to be updated (AoR, Via...) before network delivering
        // all other messages MUST not unless specified from the dialog layer
        TSIP_MESSAGE(msg)->update |= (TNET_SOCKET_TYPE_IS_WS(msg->src_net_type) || TNET_SOCKET_TYPE_IS_WSS(msg->src_net_type));

        return tsip_transport_layer_send(
                   self->stack->layer_transport,
                   msg->firstVia ? msg->firstVia->branch : tsk_null,
                   TSIP_MESSAGE(msg)
               );
    }
    default: {
        TSK_DEBUG_ERROR("Unexpected code called");
        return -2;
    }
    }
}


int tsip_transac_init(tsip_transac_t *self, tsip_transac_type_t type, int32_t cseq_value, const char* cseq_method, const char* callid, struct tsip_transac_dst_s* dst, tsk_fsm_state_id curr, tsk_fsm_state_id term)
{
    if(self && !self->initialized) {
        self->type = type;
        self->cseq_value = cseq_value;
        tsk_strupdate(&self->cseq_method, cseq_method);
        tsk_strupdate(&self->callid, callid);
        self->dst = tsk_object_ref(dst);

        /* FSM */
        self->fsm = tsk_fsm_create(curr, term);

        self->initialized = tsk_true;

        return 0;
    }
    return -1;
}

int tsip_transac_deinit(tsip_transac_t *self)
{
    if(self && self->initialized) {
        /* FSM */
        TSK_OBJECT_SAFE_FREE(self->fsm);

        TSK_FREE(self->branch);
        TSK_FREE(self->cseq_method);
        TSK_FREE(self->callid);
        TSK_OBJECT_SAFE_FREE(self->dst);

        self->initialized = tsk_false;

        return 0;
    }
    return -1;
}

int tsip_transac_start(tsip_transac_t *self, const tsip_request_t* request)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    switch(self->type) {
    case tsip_transac_type_nist: {
        return tsip_transac_nist_start(TSIP_TRANSAC_NIST(self), request);
    }
    case tsip_transac_type_ist: {
        return tsip_transac_ist_start(TSIP_TRANSAC_IST(self), request);
    }
    case tsip_transac_type_nict: {
        return tsip_transac_nict_start(TSIP_TRANSAC_NICT(self), request);
    }
    case tsip_transac_type_ict: {
        return tsip_transac_ict_start(TSIP_TRANSAC_ICT(self), request);
    }
    }

    TSK_DEBUG_ERROR("Unexpected code called");
    return -2;
}

// deliver the message to the destination (e.g. local dialog)
int tsip_transac_deliver(tsip_transac_t* self, tsip_dialog_event_type_t event_type, const tsip_message_t *msg)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    return tsip_transac_dst_deliver(self->dst, event_type, msg);
}

// send the message over the network
int tsip_transac_send(tsip_transac_t *self, const char *branch, tsip_message_t *msg)
{
    if(self && TSIP_TRANSAC_GET_STACK(self)->layer_transport && msg) {
        const struct tsip_ssession_s* ss = TSIP_TRANSAC_GET_SESSION(self);
        if(ss) {
            // set SigComp identifier as the message is directly sent to the transport layer
            tsk_strupdate(&msg->sigcomp_id, ss->sigcomp_id);
        }
        return tsip_transport_layer_send(TSIP_TRANSAC_GET_STACK(self)->layer_transport, branch, TSIP_MESSAGE(msg));
    }
    TSK_DEBUG_ERROR("Invalid parameter");
    return -1;
}

int tsip_transac_cmp(const tsip_transac_t *t1, const tsip_transac_t *t2)
{
    if(t1 && t2) {
        if(tsk_strequals(t1->branch, t2->branch) && tsk_strequals(t1->cseq_method, t2->cseq_method)) {
            return 0;
        }
    }
    return -1;
}

int tsip_transac_remove(const tsip_transac_t* self)
{
    int ret;
    tsip_transac_t* safe_copy;

    safe_copy = (tsip_transac_t*)tsk_object_ref(TSK_OBJECT(self));
    ret = tsip_transac_layer_remove(TSIP_TRANSAC_GET_STACK(self)->layer_transac, safe_copy);
    tsk_object_unref(safe_copy);

    return ret;
}

int tsip_transac_fsm_act(tsip_transac_t* self, tsk_fsm_action_id action_id, const tsip_message_t* message)
{
    int ret;
    tsip_transac_t* safe_copy;

    if(!self || !self->fsm) {
        TSK_DEBUG_WARN("Invalid parameter.");
        return -1;
    }

    safe_copy = tsk_object_ref(TSK_OBJECT(self));
    ret = tsk_fsm_act(self->fsm, action_id, safe_copy, message, self, message);
    tsk_object_unref(safe_copy);

    return ret;
}
