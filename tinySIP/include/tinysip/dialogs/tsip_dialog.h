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
 * @brief SIP dialog base class as per RFC 3261 subclause 17.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSIP_DIALOG_H
#define TINYSIP_DIALOG_H

#include "tinysip_config.h"

#include "tsip.h"
#include "tinysip/tsip_uri.h"
#include "tinysip/tsip_timers.h"
#include "tinysip/tsip_message.h"
#include "tinysip/tsip_ssession.h"

#include "tinysip/authentication/tsip_challenge.h"
#include "tinysip/tsip_action.h"

#include "tinysip/headers/tsip_header_Record_Route.h"

#include "tsk_safeobj.h"
#include "tsk_list.h"
#include "tsk_string.h"
#include "tsk_fsm.h"

TSIP_BEGIN_DECLS

#define TSIP_DIALOG(self)													((tsip_dialog_t*)(self))
#define TSIP_DIALOG_GET_STATE(self)											TSIP_DIALOG((self))->state
#define TSIP_DIALOG_GET_FSM(self)											TSIP_DIALOG((self))->fsm
#define TSIP_DIALOG_GET_SS(self)											TSIP_DIALOG((self))->ss
#define TSIP_DIALOG_GET_STACK(self)											TSIP_STACK(TSIP_DIALOG_GET_SS((self))->stack)

#define TSIP_DIALOG_TIMER_CANCEL(TX) \
	tsk_timer_mgr_global_cancel(self->timer##TX.id)

// TX MUST be in seconds
#define TSIP_DIALOG_TIMER_SCHEDULE(name, TX)								\
	self->timer##TX.id = tsk_timer_mgr_global_schedule(self->timer##TX.timeout, TSK_TIMER_CALLBACK_F(tsip_dialog_##name##_timer_callback), self)

#define TSIP_DIALOG_SIGNAL(self, code, phrase)	\
	tsip_event_signal(tsip_event_dialog, TSIP_DIALOG(self)->ss, code, phrase)

#define TSIP_DIALOG_SIGNAL_2(self, code, phrase, message)	\
	tsip_event_signal_2(tsip_event_dialog, TSIP_DIALOG(self)->ss, code, phrase, message)

#if !defined(TSIP_DIALOG_SHUTDOWN_TIMEOUT)
#	define TSIP_DIALOG_SHUTDOWN_TIMEOUT	2000 /* miliseconds. */
#endif

#if !defined(TSIP_DIALOG_INVALID_ID)
#	define TSIP_DIALOG_INVALID_ID 0
#endif

typedef uint64_t tsip_dialog_id_t;

typedef enum tsip_dialog_state_e {
    tsip_initial,
    tsip_early,
    tsip_established,
    tsip_terminated
}
tsip_dialog_state_t;

typedef enum tsip_dialog_type_e {
    tsip_dialog_unknown,
    tsip_dialog_INVITE,
    tsip_dialog_MESSAGE,
    tsip_dialog_INFO,
    tsip_dialog_OPTIONS,
    tsip_dialog_PUBLISH,
    tsip_dialog_REGISTER,
    tsip_dialog_SUBSCRIBE,
}
tsip_dialog_type_t;

typedef enum tsip_dialog_event_type_e {
    tsip_dialog_i_msg,
    tsip_dialog_o_msg,
    tsip_dialog_transac_ok,
    tsip_dialog_canceled,
    tsip_dialog_terminated,
    tsip_dialog_timedout,
    tsip_dialog_error,
    tsip_dialog_transport_error,
}
tsip_dialog_event_type_t;

typedef int (*tsip_dialog_event_callback_f)(const void *arg, tsip_dialog_event_type_t type, const tsip_message_t *msg);
#define TSIP_DIALOG_EVENT_CALLBACK_F(callback) ((tsip_dialog_event_callback_f)(callback))

/*================================
*/
typedef struct tsip_dialog_s {
    TSK_DECLARE_OBJECT;

    tsip_dialog_type_t type;
    tsip_dialog_id_t id;

    tsk_fsm_t* fsm;

    tsip_ssession_t* ss;
    tsip_action_t* curr_action;

    tsip_dialog_state_t state;

    tsk_bool_t initialized;
    tsk_bool_t running;

    tnet_fd_t connected_fd;

    struct {
        char* phrase;
        short code;
        tsip_message_t* message;
    } last_error;

    char* tag_local;
    tsip_uri_t* uri_local;
    char* tag_remote;
    tsip_uri_t* uri_remote;

    tsip_uri_t* uri_remote_target;
    struct sockaddr_storage remote_addr; // Only valid for Dgram

    uint32_t cseq_value;
    char* cseq_method;

    int64_t expires; /* in milliseconds */

    char* callid;

    tsip_header_Record_Routes_L_t *record_routes;

    tsip_challenges_L_t *challenges;

    tsip_dialog_event_callback_f callback;

    TSK_DECLARE_SAFEOBJ;
}
tsip_dialog_t;

#define TSIP_DECLARE_DIALOG tsip_dialog_t __dialog__

typedef tsk_list_t tsip_dialogs_L_t;
/*
================================*/

tsip_request_t *tsip_dialog_request_new(const tsip_dialog_t *self, const char* method);
int tsip_dialog_request_send(const tsip_dialog_t *self, tsip_request_t* request);
tsip_response_t *tsip_dialog_response_new(tsip_dialog_t *self, short status, const char* phrase, const tsip_request_t* request);
int tsip_dialog_response_send(const tsip_dialog_t *self, tsip_response_t* response);
int tsip_dialog_apply_action(tsip_message_t* message, const tsip_action_t* action);

int64_t tsip_dialog_get_newdelay(tsip_dialog_t *self, const tsip_message_t* message);
int tsip_dialog_update(tsip_dialog_t *self, const tsip_response_t* response);
int tsip_dialog_update_2(tsip_dialog_t *self, const tsip_request_t* invite);
int tsip_dialog_getCKIK(tsip_dialog_t *self, AKA_CK_T *ck, AKA_IK_T *ik);

int tsip_dialog_init(tsip_dialog_t *self, tsip_dialog_type_t type, const char* call_id, tsip_ssession_t* ss, tsk_fsm_state_id curr, tsk_fsm_state_id term);
int tsip_dialog_fsm_act(tsip_dialog_t* self, tsk_fsm_action_id , const tsip_message_t* , const tsip_action_handle_t*);
#define tsip_dialog_fsm_act_2(self, action_id) tsip_dialog_fsm_act((self), (action_id), tsk_null, tsk_null)
tsk_bool_t tsip_dialog_keep_action(const tsip_dialog_t* self, const tsip_response_t *response);
int tsip_dialog_set_connected_fd(tsip_dialog_t* self, tnet_fd_t fd);
int tsip_dialog_set_curr_action(tsip_dialog_t* self, const tsip_action_t* action);
int tsip_dialog_set_lasterror(tsip_dialog_t* self, const char* phrase, short code);
int tsip_dialog_set_lasterror_2(tsip_dialog_t* self, const char* phrase, short code, const tsip_message_t *message);
int tsip_dialog_get_lasterror(const tsip_dialog_t* self, short *code, const char** phrase, const tsip_message_t **message);
int tsip_dialog_hangup(tsip_dialog_t *self, const tsip_action_t* action);
int tsip_dialog_shutdown(tsip_dialog_t *self, const tsip_action_t* action);
int tsip_dialog_signal_transport_error(tsip_dialog_t *self);
int tsip_dialog_remove(const tsip_dialog_t* self);
int tsip_dialog_cmp(const tsip_dialog_t *d1, const tsip_dialog_t *d2);
int tsip_dialog_deinit(tsip_dialog_t *self);

TSIP_END_DECLS

#endif /* TINYSIP_DIALOG_H */

