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
#include "options.h"

extern ctx_t* ctx;
extern const session_t* session_handle_cmd(cmd_type_t , const opts_L_t*);

int options_hack_aor(const tsip_response_t* resp);

int options_handle_event(const tsip_event_t *_event)
{
    const tsip_options_event_t* opt_event = TSIP_OPTIONS_EVENT(_event);
    const session_t* session;
    tsip_ssession_id_t sid;

    /* Find associated session */
    sid = tsip_ssession_get_id(_event->ss);
    if(!(session = session_get_by_sid(ctx->sessions, sid))) {
        TSK_DEBUG_WARN("Failed to match session event.");
        return -1;
    }

    switch(opt_event->type) {
    case tsip_i_options: /* incoming OPTIONS */
        break;
    case tsip_ao_options: /* answer to outgoing OPTIONS */
#if HACK_AOR
        if(TSIP_MESSAGE_IS_RESPONSE(_event->sipmessage)) {
            options_hack_aor(_event->sipmessage);
        }
#endif
        break;
    }

    return 0;
}

tsip_ssession_id_t options_handle_cmd(cmd_type_t cmd, const opts_L_t* opts)
{
    const session_t* session = tsk_null;
    tsip_ssession_id_t id = TSIP_SSESSION_INVALID_ID;

    if(!(session = session_handle_cmd(cmd, opts))) {
        goto bail;
    }
    else {
        id = tsip_ssession_get_id(session->handle);
    }

    /* Execute command */
    switch(cmd) {
    case cmd_options: {
        /* Send SIP OPTIONS */
        tsip_action_handle_t* action_config = action_get_config(opts);
        tsip_api_options_send_options(session->handle,
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

/* used to hack the AOR. useful for devices behind NATs or emulators (e.g. ANDROID) */
int options_hack_aor(const tsip_response_t* resp)
{
    int32_t rport;
    if(resp && resp->firstVia) {
        if(resp->firstVia->rport <=0) {
            char* received_port_ext;
            /* Ericsson SDS
            ==> Via: SIP/2.0/TCP 192.168.0.12:49744;rport;branch=z9hG4bK1273100904048;received_port_ext=49744;received=192.168.0.12 */
            if((received_port_ext = tsip_header_get_param_value((const tsip_header_t*)resp->firstVia, "received_port_ext"))) {
                rport = (int32_t)atoi(received_port_ext);
                TSK_FREE(received_port_ext);
            }
        }
        else {
            rport = resp->firstVia->rport;
        }

        tsip_stack_set(ctx->stack,
                       TSIP_STACK_SET_AOR(resp->firstVia->received, rport),
                       TSIP_STACK_SET_NULL());
        return 0;
    }

    return -1;
}
