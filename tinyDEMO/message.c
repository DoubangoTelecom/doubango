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
#include "message.h"

#include "tinysms.h" /* Binary SMS API*/

#include <ctype.h>

extern ctx_t* ctx;
extern const session_t* session_handle_cmd(cmd_type_t , const opts_L_t*);

tsk_bool_t is_valid_telnum(const tsip_uri_t* uri);
tsk_buffer_t* sms_submit(const tsip_uri_t* smsc, const tsip_uri_t* dest, const char* ascii_pay);

int message_handle_event(const tsip_event_t *_event)
{
    const tsip_message_event_t* msg_event = TSIP_MESSAGE_EVENT(_event);
    const session_t* session = tsk_null;
    tsip_ssession_id_t sid;
    int ret = 0;

    /* Find associated session */
    sid = tsip_ssession_get_id(_event->ss);
    if(!(session = session_get_by_sid(ctx->sessions, sid))) {
        if(tsip_ssession_have_ownership(_event->ss)) {
            /* it's or own session and we fail to match it ==> should never happen */
            TSK_DEBUG_ERROR("Failed to match session event.");
            ret = -2;
            goto bail;
        }
        else {
            /* it's a "server-side-session" (incoming MESSAGE) */
            session_t* _session;
            if((_session = session_server_create(st_message, _event->ss)) && (session = _session)) {
                tsk_list_push_back_data(ctx->sessions, (void**)&_session);
            }
            else {
                TSK_DEBUG_ERROR("Failed to create \"sever-side-session\".");
                ret = -3;
                goto bail;
            }
        }
    }


    switch(msg_event->type) {
    case tsip_ao_message: { /* Answer to outgoing MESSAGE */
        TSK_DEBUG_INFO("Event: Answer to outgoing MESSAGE. Code=%d", TSIP_RESPONSE_CODE(_event->sipmessage));
        if(TSIP_RESPONSE_IS_2XX(_event->sipmessage)) {
            TSK_DEBUG_INFO("Message successfully sent.");
        }
        break;
    }

    case tsip_i_message: { /* Incoming MESSAGE */
        const char* content_type = TSIP_MESSAGE_CONTENT_TYPE(_event->sipmessage);

        TSK_DEBUG_INFO("Event: Incoming MESSAGE.");
        if(TSIP_MESSAGE_HAS_CONTENT(_event->sipmessage)) {
            const tsk_buffer_t* content = TSIP_MESSAGE_CONTENT(_event->sipmessage);
            TSK_DEBUG_INFO("MESSAGE Content-Type: %s", content_type);
            TSK_DEBUG_INFO("MESSAGE Content: %s", content->data);
        }
        /* accept() the MESSAGE to terminate the dialog */
        if(tsk_striequals("plain/text", content_type) || tsk_striequals("text/html", content_type)) {
            tsip_api_common_accept(session->handle,
                                   TSIP_ACTION_SET_HEADER("Info", "I've accept()ed your message"),// just for test
                                   TSIP_ACTION_SET_NULL());
        }
        /* reject() the MESSAGE to terminate the dialog */
        else {
            tsip_api_common_reject(session->handle,
                                   TSIP_ACTION_SET_HEADER("Info", "I've reject()ed your message"),// just for test
                                   TSIP_ACTION_SET_HEADER("In-Reply-To", "apb03a0s09dkjdfglkj49112"),// just for test
                                   TSIP_ACTION_SET_NULL());
        }
        break;
    }

    default: {
        /* Any other event */
        TSK_DEBUG_WARN("%d not a valid SIP Messaging event.", msg_event->type);
        break;
    }
    }

bail:
    return ret;
}

tsip_ssession_id_t message_handle_cmd(cmd_type_t cmd, const opts_L_t* opts)
{
    const session_t* session = tsk_null;
    const opt_t* opt;
    tsip_ssession_id_t id = TSIP_SSESSION_INVALID_ID;

    if(!(session = session_handle_cmd(cmd, opts))) {
        goto bail;
    }
    else {
        id = tsip_ssession_get_id(session->handle);
    }

    switch(cmd) {
    case cmd_message: {
        /* Send SIP MESSAGE */
        tsip_action_handle_t* action_config = action_get_config(opts);
        tsip_api_message_send_message(session->handle,
                                      TSIP_ACTION_SET_CONFIG(action_config),
                                      /* Any other TSIP_ACTION_SET_*() macros */
                                      TSIP_ACTION_SET_NULL());
        TSK_OBJECT_SAFE_FREE(action_config);
        break;
    }
    case cmd_sms: {
        /* Binary SMS (RP-DATA) */
        const char* ascii_pay = tsk_null;
        tsk_buffer_t* binary_pay = tsk_null;
        if((opt = opt_get_by_type(opts, opt_smsc)) && !tsk_strnullORempty(opt->value)) {
            tsip_uri_t* smsc = tsip_uri_parse(opt->value, tsk_strlen(opt->value));
            tsip_uri_t* dest = tsk_null;
            if(smsc) {
                /* Valid phone number for the SMSC address? */
                if(!is_valid_telnum(smsc)) {
                    TSK_DEBUG_ERROR("[%s] contains invalid telephone number", opt->value);
                    goto done;
                }
                tsip_ssession_set(session->handle,
                                  TSIP_SSESSION_SET_TO(opt->value),
                                  TSIP_SSESSION_SET_NULL());

                /* Destination URI */
                if((opt = opt_get_by_type(opts, opt_to)) && !tsk_strnullORempty(opt->value)) {
                    if((dest = tsip_uri_parse(opt->value, tsk_strlen(opt->value)))) {
                        if(!is_valid_telnum(dest)) {
                            TSK_DEBUG_ERROR("[%s] contains invalid telephone number", opt->value);
                            goto done;
                        }
                    }
                    else {
                        TSK_DEBUG_ERROR("[%s] is an invalid SIP/tel uri", opt->value);
                        goto done;
                    }
                }
                else {
                    TSK_DEBUG_ERROR("++sms command need --to");
                    goto done;
                }

                /* Payload? */
                if((opt = opt_get_by_type(opts, opt_payload)) && !tsk_strnullORempty(opt->value)) {
                    ascii_pay = opt->value;
                }
                else {
                    TSK_DEBUG_ERROR("++sms command need --to");
                    goto done;
                }

                /* Create the binary content */
                if(!(binary_pay = sms_submit(smsc, dest, ascii_pay))) {
                    TSK_DEBUG_ERROR("Failed to encode RP-DATA(SMS-SUBMIT) message.");
                    goto done;
                }

                /* Send the message */
                tsip_api_message_send_message(session->handle,
                                              /* TSIP_ACTION_SET_HEADER("Content-Type", "application/vnd.3gpp.sms"), */
                                              /* TSIP_ACTION_SET_HEADER("Transfer-Encoding", "binary"),*/
                                              TSIP_ACTION_SET_PAYLOAD(binary_pay->data, binary_pay->size),
                                              TSIP_ACTION_SET_NULL());

done:
                TSK_OBJECT_SAFE_FREE(binary_pay);
                TSK_OBJECT_SAFE_FREE(dest);
                TSK_OBJECT_SAFE_FREE(smsc);
            }
            else {
                TSK_DEBUG_ERROR("[%s] is an invalid SIP/tel uri", opt->value);
                break;
            }
        }
        else {
            TSK_DEBUG_ERROR("++sms command need --smsc");
            break;
        }
        break;
    }
    default:
        /* already handled by session_handle_cmd() */
        break;
    }

bail:
    return id;
}


tsk_bool_t is_valid_telnum(const tsip_uri_t* uri)
{
    tsk_size_t i;
    tsk_size_t len;

    if(!uri || tsk_strnullORempty(uri->user_name)) {
        return tsk_false;
    }

    for(i = 0, len = tsk_strlen(uri->user_name); i<len; i++) {
        if(uri->user_name[i] != '+' && !isdigit(uri->user_name[i])) {
            return tsk_false;
        }
    }
    return tsk_true;
}

tsk_buffer_t* sms_submit(const tsip_uri_t* smsc, const tsip_uri_t* dest, const char* ascii_pay)
{
    static uint8_t mr = 0x00;

    int ret;
    tsk_buffer_t* buffer = tsk_null;
    tsms_tpdu_submit_t* sms_submit = tsk_null;
    tsms_rpdu_data_t* rp_data = tsk_null;

    // create SMS-SUBMIT message
    sms_submit = tsms_tpdu_submit_create(++mr, (const uint8_t*)smsc->user_name, (const uint8_t*)dest->user_name);
    // Set content for SMS-SUBMIT
    if((buffer = tsms_pack_to_7bit(ascii_pay))) {
        if((ret = tsms_tpdu_submit_set_userdata(sms_submit, buffer, tsms_alpha_7bit))) {
            goto bail;
        }
        TSK_OBJECT_SAFE_FREE(buffer);
    }
    // create Mobile Originated (MO) RP-DATA message
    if((rp_data = tsms_rpdu_data_create_mo(mr, (const uint8_t*)smsc->user_name, TSMS_TPDU_MESSAGE(sms_submit)))) {
        // serialize into a buffer
        if((buffer = tsk_buffer_create_null())) {
            ret = tsms_rpdu_data_serialize(rp_data, buffer);
        }
    }

bail:
    if(ret) { /* Failed? */
        TSK_OBJECT_SAFE_FREE(buffer);
    }
    TSK_OBJECT_SAFE_FREE(sms_submit);
    TSK_OBJECT_SAFE_FREE(rp_data);

    return buffer;
}
