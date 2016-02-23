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
#include "register.h"

extern ctx_t* ctx;
extern const session_t* session_handle_cmd(cmd_type_t , const opts_L_t*);


int register_handle_event(const tsip_event_t *_event)
{
    const tsip_register_event_t* reg_event = TSIP_REGISTER_EVENT(_event);
    const session_t* session;
    tsip_ssession_id_t sid;

    /* Find associated session */
    sid = tsip_ssession_get_id(_event->ss);
    if(!(session = session_get_by_sid(ctx->sessions, sid))) {
        TSK_DEBUG_WARN("Failed to match session event.");
        return -1;
    }

    switch(reg_event->type) {
    case tsip_ao_register: { /* Answer to outgoing REGISTER */
        if(_event->sipmessage) {
            if(TSIP_MESSAGE_IS_RESPONSE(_event->sipmessage)) {
                TSK_DEBUG_INFO("Event: Answer to outgoing REGISTER. Code=%d and phrase=%s",
                               _event->sipmessage->line.response.status_code, _event->sipmessage->line.response.reason_phrase);
            }
            else {
                // request
            }
        }
        break;
    }
    case tsip_ao_unregister: { /* Answer to outgoing unREGISTER */
        if(_event->sipmessage) {
            if(TSIP_MESSAGE_IS_RESPONSE(_event->sipmessage)) {
                TSK_DEBUG_INFO("Event: Answer to outgoing unREGISTER. Code=%d and phrase=%s",
                               _event->sipmessage->line.response.status_code, _event->sipmessage->line.response.reason_phrase);
            }
            else {
                // request
            }
        }
        break;
    }

    /* Server events (For whose dev. Server Side IMS Services) */
    case tsip_i_register: /* Incoming REGISTER */
    case tsip_i_unregister: { /* Incoming unREGISTER */
        TSK_DEBUG_WARN("Event not support by Client Framework.");
        break;
    }

    default: {
        /* Any other event */
        TSK_DEBUG_WARN("%d not a valid SIP Registration event.", reg_event->type);
        break;
    }
    }

    return 0;
}


tsip_ssession_id_t register_handle_cmd(cmd_type_t cmd, const opts_L_t* opts)
{
    const session_t* session = tsk_null;
    tsip_ssession_id_t id = TSIP_SSESSION_INVALID_ID;

    if(!(session = session_handle_cmd(cmd, opts))) {
        goto bail;
    }
    else {
        id = tsip_ssession_get_id(session->handle);
    }

    /* action config */

    /* Execute command */
    switch(cmd) {
    case cmd_register: {
        /* Send SIP REGISTER */
        tsip_action_handle_t* action_config = action_get_config(opts);
        tsip_api_register_send_register(session->handle,
                                        TSIP_ACTION_SET_CONFIG(action_config),
                                        /* Any other TSIP_ACTION_SET_*() macros */
                                        TSIP_ACTION_SET_NULL());
        TSK_OBJECT_SAFE_FREE(action_config);
        break;
    }
    default:
        /* already handled by session_handle_cmd() */
        break;
    }

bail:
    return id;
}
