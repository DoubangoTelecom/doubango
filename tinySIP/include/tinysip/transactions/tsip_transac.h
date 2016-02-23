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
 * @brief SIP transaction base class as per RFC 3261 subclause 17.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSIP_TRANSAC_H
#define TINYSIP_TRANSAC_H

#include "tinysip_config.h"

#include "tsip.h"
#include "tinysip/tsip_timers.h"
#include "tinysip/tsip_message.h"

#include "tinysip/dialogs/tsip_dialog.h"

#include "tsk_safeobj.h"
#include "tsk_list.h"
#include "tsk_fsm.h"

TSIP_BEGIN_DECLS

#define TSIP_TRANSAC(self)						((tsip_transac_t*)(self))
#define TSIP_TRANSAC_GET_TYPE(self)				TSIP_TRANSAC((self))->type
#define TSIP_TRANSAC_GET_FSM(self)				TSIP_TRANSAC((self))->fsm
#define TSIP_TRANSAC_GET_SESSION(self)			((struct tsip_ssession_s*)(TSIP_TRANSAC_GET_DIALOG((self)) ? TSIP_TRANSAC_GET_DIALOG((self))->ss : tsk_null))
#define TSIP_TRANSAC_GET_DIALOG(self)			((struct tsip_dialog_s*)(TSIP_TRANSAC_GET_DST((self))->type == tsip_transac_dst_type_dialog ? TSIP_TRANSAC_GET_DST((self))->dialog.dlg : tsk_null))
#define TSIP_TRANSAC_GET_DST(self)				TSIP_TRANSAC((self))->dst
#define TSIP_TRANSAC_GET_STACK(self)			TSIP_TRANSAC_GET_DST((self))->stack
#define TSIP_TRANSAC_GET_TIMER_MGR(self)		TSIP_TRANSAC_GET_STACK((self))->timer_mgr

#define TSIP_TRANSAC_IS_CLIENT(self)			((self) && ((self)->type == tsip_transac_type_ict || (self)->type == tsip_transac_type_nict))
#define TSIP_TRANSAC_IS_SERVER(self)			!TSIP_TRANSAC_IS_CLIENT((self))

#define TSIP_TRANSAC_MAGIC_COOKIE				"z9hG4bK"

#define TSIP_TRANSAC_SYNC_BEGIN(self)			tsk_safeobj_lock(TSIP_TRANSAC(self))
#define TSIP_TRANSAC_SYNC_END(self)				tsk_safeobj_unlock(TSIP_TRANSAC(self))

#define TRANSAC_TIMER_SCHEDULE(name, TX) \
	self->timer##TX.id = tsk_timer_mgr_global_schedule(self->timer##TX.timeout, TSK_TIMER_CALLBACK_F(tsip_transac_##name##_timer_callback), self)

#define TRANSAC_TIMER_CANCEL(TX) \
	tsk_timer_mgr_global_cancel(self->timer##TX.id)

typedef enum tsip_transac_event_type_e {
    tsip_transac_incoming_msg,
    tsip_transac_outgoing_msg,
    tsip_transac_canceled,
    tsip_transac_terminated,
    tsip_transac_timedout,
    tsip_transac_error,
    tsip_transac_transport_error
}
tsip_transac_event_type_t;

/*typedef struct tsip_transac_event_s
{
	tsip_transac_event_type_t type;
	const tsip_message_t *msg;
}
tsip_transac_event_t;

#define TSIP_TRANSAC_EVENT_INIT(transac_event, type, msg)	\
	transac_event.type = type;								\
	transac_event.msg = msg;*/

typedef int (*tsip_transac_event_callback_f)(const void *arg, tsip_transac_event_type_t type, const tsip_message_t *msg);
#define TSIP_TRANSAC_EVENT_CALLBACK_F(callback)	 ((tsip_transac_event_callback_f)(callback))

typedef enum tsip_transac_type_e {
    tsip_transac_type_ict, /**< Invite Client Transaction. */
    tsip_transac_type_ist, /**< Invite Server Transaction. */
    tsip_transac_type_nict, /**< Non-Invite Client Transaction. */
    tsip_transac_type_nist, /**< Non-Invite Server Transaction. */
}
tsip_transac_type_t;



typedef enum tsip_transac_dst_type_e {
    tsip_transac_dst_type_dialog,
    tsip_transac_dst_type_net
}
tsip_transac_dst_type_t;

typedef struct tsip_transac_dst_s {
    TSK_DECLARE_OBJECT;

    tsip_transac_dst_type_t type;
    struct tsip_stack_s* stack;

    union {
        struct {
            tsip_dialog_t *dlg;
        } dialog;

        //struct{
        //}net;
    };
}
tsip_transac_dst_t;
#define TSIP_TRANSAC_DST(self)	((tsip_transac_dst_t*)(self))
#define TSIP_DECLARE_TRANSAC_DST tsip_transac_dst_t __transac__


typedef struct tsip_transac_s {
    TSK_DECLARE_OBJECT;

    tsip_transac_type_t type;

    struct tsip_transac_dst_s* dst;
    tsk_fsm_t *fsm;

    tsk_bool_t reliable;
    tsk_bool_t running;
    tsk_bool_t initialized;

    char *branch;

    int32_t cseq_value;
    char* cseq_method;

    char* callid;

    tsip_transac_event_callback_f callback;
}
tsip_transac_t;

#define TSIP_DECLARE_TRANSAC tsip_transac_t __transac__

typedef tsk_list_t tsip_transacs_L_t; /**< List of @ref tsip_transac_t elements. */
/*
================================*/

int tsip_transac_init(tsip_transac_t *self, tsip_transac_type_t type, int32_t cseq_value, const char* cseq_method, const char* callid, struct tsip_transac_dst_s* dst, tsk_fsm_state_id curr, tsk_fsm_state_id term);
int tsip_transac_deinit(tsip_transac_t *self);
int tsip_transac_start(tsip_transac_t *self, const tsip_request_t* request);
int tsip_transac_deliver(tsip_transac_t* self, tsip_dialog_event_type_t event_type, const tsip_message_t *msg);
int tsip_transac_send(tsip_transac_t *self, const char *branch, tsip_message_t *msg);
int tsip_transac_cmp(const tsip_transac_t *t1, const tsip_transac_t *t2);
int tsip_transac_remove(const tsip_transac_t* self);
int tsip_transac_fsm_act(tsip_transac_t* self, tsk_fsm_action_id , const tsip_message_t*);


struct tsip_transac_dst_s* tsip_transac_dst_dialog_create(tsip_dialog_t *dlg);
struct tsip_transac_dst_s* tsip_transac_dst_net_create(struct tsip_stack_s* stack);

TSIP_END_DECLS

#endif /* TINYSIP_TRANSAC_H */

