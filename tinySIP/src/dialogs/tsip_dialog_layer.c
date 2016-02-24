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

/**@file tsip_dialog_layer.c
 * @brief SIP dialog layer.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/dialogs/tsip_dialog_layer.h"

#include "tinysip/dialogs/tsip_dialog_invite.h"
#include "tinysip/dialogs/tsip_dialog_message.h"
#include "tinysip/dialogs/tsip_dialog_info.h"
#include "tinysip/dialogs/tsip_dialog_options.h"
#include "tinysip/dialogs/tsip_dialog_publish.h"
#include "tinysip/dialogs/tsip_dialog_register.h"
#include "tinysip/dialogs/tsip_dialog_subscribe.h"

#include "tinysip/transactions/tsip_transac_layer.h"
#include "tinysip/transports/tsip_transport_layer.h"

#include "tsk_debug.h"

extern tsip_ssession_handle_t *tsip_ssession_create_2(const tsip_stack_t* stack, const struct tsip_message_s* message);

/*== Predicate function to find dialog by type */
static int pred_find_dialog_by_type(const tsk_list_item_t *item, const void *type)
{
    if(item && item->data) {
        tsip_dialog_t *dialog = item->data;
        return (dialog->type - *((tsip_dialog_type_t*)type));
    }
    return -1;
}

/*== Predicate function to find dialog by not type */
static int pred_find_dialog_by_not_type(const tsk_list_item_t *item, const void *type)
{
    if(item && item->data) {
        tsip_dialog_t *dialog = item->data;
        if(dialog->type != *((tsip_dialog_type_t*)type)) {
            return 0;
        }
    }
    return -1;
}

/*== Predicate function to find dialog by callid */
static int pred_find_dialog_by_callid(const tsk_list_item_t *item, const void *callid)
{
    if(item && item->data && callid) {
        return tsk_strcmp(((tsip_dialog_t*)item->data)->callid, ((const char*)callid));
    }
    return -1;
}

tsip_dialog_layer_t* tsip_dialog_layer_create(tsip_stack_t* stack)
{
    return tsk_object_new(tsip_dialog_layer_def_t, stack);
}

// it's up to the caller to release the returned object
tsip_dialog_t* tsip_dialog_layer_find_by_ss(tsip_dialog_layer_t *self, const tsip_ssession_handle_t *ss)
{
    return tsip_dialog_layer_find_by_ssid(self, tsip_ssession_get_id(ss));
}

// it's up to the caller to release the returned object
tsip_dialog_t* tsip_dialog_layer_find_by_ssid(tsip_dialog_layer_t *self, tsip_ssession_id_t ssid)
{
    tsip_dialog_t *ret = 0;
    tsip_dialog_t *dialog;
    tsk_list_item_t *item;

    tsk_safeobj_lock(self);

    tsk_list_foreach(item, self->dialogs) {
        dialog = item->data;
        if(tsip_ssession_get_id(dialog->ss) == ssid) {
            ret = dialog;
            break;
        }
    }

    tsk_safeobj_unlock(self);

    return tsk_object_ref(ret);
}

// it's up to the caller to release the returned object
tsip_dialog_t* tsip_dialog_layer_find_by_callid(tsip_dialog_layer_t *self, const char* callid)
{
    if(!self || !callid) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }
    else {
        tsip_dialog_t *dialog = tsk_null;
        tsk_list_item_t *item;
        //--tsk_safeobj_lock(self);
        tsk_list_foreach(item, self->dialogs) {
            if(tsk_striequals(TSIP_DIALOG(item->data)->callid, callid)) {
                dialog = tsk_object_ref(item->data);
                break;
            }
        }
        //--tsk_safeobj_unlock(self);
        return dialog;
    }
}

tsk_bool_t tsip_dialog_layer_have_dialog_with_callid(const tsip_dialog_layer_t *self, const char* callid)
{
    tsk_bool_t found = tsk_false;
    if(self) {
        tsk_safeobj_lock(self);
        if(tsk_list_find_item_by_pred(self->dialogs, pred_find_dialog_by_callid, callid) != tsk_null) {
            found = tsk_true;
        }
        tsk_safeobj_unlock(self);
    }
    return found;
}

// it's up to the caller to release the returned object
tsip_dialog_t* tsip_dialog_layer_find(const tsip_dialog_layer_t *self, const char* callid, const char* to_tag, const char* from_tag, tsip_request_type_t type, tsk_bool_t *cid_matched)
{
    tsip_dialog_t *ret = tsk_null;
    tsip_dialog_t *dialog;
    tsk_list_item_t *item;

    *cid_matched = tsk_false;

    tsk_safeobj_lock(self);

    tsk_list_foreach(item, self->dialogs) {
        dialog = item->data;
        if(tsk_strequals(dialog->callid, callid)) {
            tsk_bool_t is_cancel = (type == tsip_CANCEL); // Incoming CANCEL
            tsk_bool_t is_register = (type == tsip_REGISTER); // Incoming REGISTER
            tsk_bool_t is_notify = (type == tsip_NOTIFY); // Incoming NOTIFY
            *cid_matched = tsk_true;
            /* CANCEL Request will have the same local tag than the INVITE request
            	the remote tag could be null if the CANCEL request is received immediately after a 100 Trying
            */
            if((is_cancel || tsk_strequals(dialog->tag_local, from_tag)) && (!dialog->tag_remote || tsk_strequals(dialog->tag_remote, to_tag))) {
                ret = tsk_object_ref(dialog);
                break;
            }
            /* REGISTER is dialogless which means that each reREGISTER or unREGISTER will have empty to tag  */
            if(is_register /* Do not check tags */) {
                ret = tsk_object_ref(dialog);
                break;
            }
            /*	NOTIFY could arrive before the 200 SUBSCRIBE => This is why we don't try to match both tags

            	RFC 3265 - 3.1.4.4. Confirmation of Subscription Creation
            	Due to the potential for both out-of-order messages and forking, the
            	subscriber MUST be prepared to receive NOTIFY messages before the
            	SUBSCRIBE transaction has completed.
             */
            if(is_notify /* Do not check tags */) {
                ret = tsk_object_ref(dialog);
                break;
            }
        }
    }

    tsk_safeobj_unlock(self);

    return ret;
}

tsk_size_t tsip_dialog_layer_count_active_calls(tsip_dialog_layer_t *self)
{
    tsk_size_t count = 0;

    tsip_dialog_t *dialog;
    tsk_list_item_t *item;

    tsk_safeobj_lock(self);

    tsk_list_foreach(item, self->dialogs) {
        if ((dialog = item->data) && dialog->type == tsip_dialog_INVITE && dialog->state != tsip_initial && dialog->state != tsip_terminated) {
            ++count;
        }
    }

    tsk_safeobj_unlock(self);

    return count;
}

/** Hangup all dialogs starting by non-REGISTER  */
int tsip_dialog_layer_shutdownAll(tsip_dialog_layer_t *self)
{
    if(self) {
        tsk_bool_t wait = tsk_false;
        tsk_list_item_t *item;
        tsip_dialog_t *dialog;
        tsip_dialog_type_t regtype = tsip_dialog_REGISTER;

        if(!self->shutdown.inprogress) {
            self->shutdown.inprogress = tsk_true;
            if (!self->shutdown.condwait) {
                self->shutdown.condwait = tsk_condwait_create();
            }
        }

        tsk_safeobj_lock(self);
        if(tsk_list_count(self->dialogs, pred_find_dialog_by_not_type, &regtype)) {
            /* There is no register dialogs ==> phase-1 */
            goto phase1;
        }
        else if(tsk_list_count(self->dialogs, pred_find_dialog_by_type, &regtype)) {
            /* There is at least one or more register dialogs ==> phase-2 */
            goto phase2;
        }
        else {
            tsk_safeobj_unlock(self);
            goto done;
        }

phase1:
        /* Phase 1 - shutdown all except register and silent_hangup */
        TSK_DEBUG_INFO("== Shutting down - Phase-1 started ==");
phase1_loop:
        tsk_list_foreach(item, self->dialogs) {
            dialog = item->data;
            if(dialog->type != tsip_dialog_REGISTER && !dialog->ss->silent_hangup) {
                item = tsk_object_ref(item);
                if(!tsip_dialog_shutdown(dialog, tsk_null)) {
                    wait = tsk_true;
                }

                // if "tsip_dialog_shutdown()" remove the dialog, then
                // "self->dialogs" will be unsafe
                if(!(item = tsk_object_unref(item))) {
                    goto phase1_loop;
                }
            }
        }
        tsk_safeobj_unlock(self);

        /* wait until phase-1 is completed */
        if(wait) {
            tsk_condwait_timedwait(self->shutdown.condwait, TSIP_DIALOG_SHUTDOWN_TIMEOUT);
        }

        /* lock and goto phase2 */
        tsk_safeobj_lock(self);
        wait = tsk_false;
        goto phase2;

phase2:
        /* Phase 2 - unregister */
        TSK_DEBUG_INFO("== Shutting down - Phase-2 started ==");
        self->shutdown.phase2 = tsk_true;
phase2_loop:
        tsk_list_foreach(item, self->dialogs) {
            dialog = item->data;
            if(dialog->type == tsip_dialog_REGISTER) {
                item = tsk_object_ref(item);
                if(!tsip_dialog_shutdown(dialog, tsk_null)) {
                    wait = tsk_true;
                }
                // if "tsip_dialog_shutdown()" remove the dialog, then
                // "self->dialogs" will be unsafe
                if(!(item = tsk_object_unref(item))) {
                    goto phase2_loop;
                }
            }
        }
        tsk_safeobj_unlock(self);

        /* wait until phase-2 is completed */
        if(wait) {
            tsk_condwait_timedwait(self->shutdown.condwait, TSIP_DIALOG_SHUTDOWN_TIMEOUT);
        }


        /* Phase 3 - silenthangup (dialogs will be terminated immediately) */
        TSK_DEBUG_INFO("== Shutting down - Phase-3 ==");
phase3_loop:
        tsk_list_foreach(item, self->dialogs) {
            dialog = item->data;
            if(dialog->ss->silent_hangup) {
                item = tsk_object_ref(item);
                tsip_dialog_shutdown(dialog, tsk_null);

                // if "tsip_dialog_shutdown()" remove the dialog, then
                // "self->dialogs" will became unsafe while looping
                if(!(item = tsk_object_unref(item))) {
                    goto phase3_loop;
                }
            }
        }

done:
        TSK_DEBUG_INFO("== Shutting down - Terminated ==");
        return 0;
    }
    return -1;
}

static void* TSK_STDCALL _tsip_dialog_signal_transport_error_async(void* dialog)
{
    tsip_dialog_signal_transport_error(TSIP_DIALOG(dialog));
    return tsk_null;
}

int tsip_dialog_layer_signal_stack_disconnected(tsip_dialog_layer_t *self)
{
    tsk_list_item_t *item;
    // use copy  for lock-free code and faster code. also fix issue 172 (https://code.google.com/p/idoubs/issues/detail?id=172)
    tsip_dialogs_L_t *dialogs_copy;
    tsip_dialog_t *dialog;
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (!(dialogs_copy = tsk_list_create())) {
        TSK_DEBUG_ERROR("Failed to create list");
        return -1;
    }

    tsk_safeobj_lock(self);
    tsk_list_pushback_list(dialogs_copy, self->dialogs);
    tsk_safeobj_unlock(self);

    tsk_list_foreach(item, dialogs_copy) {
        if((dialog = TSIP_DIALOG(item->data))) {
            tsip_dialog_signal_transport_error(dialog);
        }
    }
    TSK_OBJECT_SAFE_FREE(dialogs_copy);
    return 0;
}

int tsip_dialog_layer_signal_peer_disconnected(tsip_dialog_layer_t *self, const struct tsip_transport_stream_peer_s* peer)
{
    tsip_dialog_t *dialog;
    const tsk_list_item_t *item;

    if(!self || !peer) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    //!\ must not lock the entire layer

    // tsk_safeobj_lock(self);

    tsk_list_lock(peer->dialogs_cids);
    tsk_list_foreach(item, peer->dialogs_cids) {
        if((dialog = tsip_dialog_layer_find_by_callid(self, TSK_STRING_STR(item->data)))) {
            tsip_dialog_signal_transport_error(dialog);
            TSK_OBJECT_SAFE_FREE(dialog);
        }
        else {
            // To avoid this WARN, you should call tsip_dialog_layer_have_dialog_with_callid() before adding a callid to a peer
            TSK_DEBUG_WARN("Stream peer holds call-id='%s' but the dialog layer doesn't know it", TSK_STRING_STR(item->data));
        }
    }
    tsk_list_unlock(peer->dialogs_cids);

    // tsk_safeobj_unlock(self);

    return 0;
}

int tsip_dialog_layer_remove_callid_from_stream_peers(tsip_dialog_layer_t *self, const char* callid)
{
    if(self) {
        return tsip_transport_layer_remove_callid_from_stream_peers(self->stack->layer_transport, callid);
    }
    TSK_DEBUG_ERROR("Invalid parameter");
    return -1;
}

/* the caller of this function must unref() the returned object */
tsip_dialog_t* tsip_dialog_layer_new(tsip_dialog_layer_t *self, tsip_dialog_type_t type, const tsip_ssession_t *ss)
{
    tsip_dialog_t* ret = tsk_null;
    tsip_dialog_t* dialog;
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        goto bail;
    }

    switch(type) {
    case tsip_dialog_INVITE: {
        if((dialog = (tsip_dialog_t*)tsip_dialog_invite_create(ss, tsk_null))) {
            ret = tsk_object_ref(dialog);
            tsk_list_push_back_data(self->dialogs, (void**)&dialog);
        }
        break;
    }
    case tsip_dialog_MESSAGE: {
        if((dialog = (tsip_dialog_t*)tsip_dialog_message_create(ss))) {
            ret = tsk_object_ref(dialog);
            tsk_list_push_back_data(self->dialogs, (void**)&dialog);
        }
        break;
    }
    case tsip_dialog_INFO: {
        if((dialog = (tsip_dialog_t*)tsip_dialog_info_create(ss))) {
            ret = tsk_object_ref(dialog);
            tsk_list_push_back_data(self->dialogs, (void**)&dialog);
        }
        break;
    }
    case tsip_dialog_OPTIONS: {
        if((dialog = (tsip_dialog_t*)tsip_dialog_options_create(ss))) {
            ret = tsk_object_ref(dialog);
            tsk_list_push_back_data(self->dialogs, (void**)&dialog);
        }
        break;
    }
    case tsip_dialog_PUBLISH: {
        if((dialog = (tsip_dialog_t*)tsip_dialog_publish_create(ss))) {
            ret = tsk_object_ref(dialog);
            tsk_list_push_back_data(self->dialogs, (void**)&dialog);
        }
        break;
    }
    case tsip_dialog_REGISTER: {
        if((dialog = (tsip_dialog_t*)tsip_dialog_register_create(ss, tsk_null))) {
            ret = tsk_object_ref(dialog);
            tsk_list_push_back_data(self->dialogs, (void**)&dialog);
        }
        break;
    }
    case tsip_dialog_SUBSCRIBE: {
        if((dialog = (tsip_dialog_t*)tsip_dialog_subscribe_create(ss))) {
            ret = tsk_object_ref(dialog);
            tsk_list_push_back_data(self->dialogs, (void**)&dialog);
        }
        break;
    }

    default: {
        TSK_DEBUG_ERROR("Dialog type not supported.");
        break;
    }
    }

bail:
    return ret;
}

int tsip_dialog_layer_remove(tsip_dialog_layer_t *self, const tsip_dialog_t *dialog)
{
    if(dialog && self) {
        tsip_dialog_type_t regtype = tsip_dialog_REGISTER;
        tsk_safeobj_lock(self);

        /* remove the dialog */
        tsk_list_remove_item_by_data(self->dialogs, dialog);

        /* whether shutting down? */
        if(self->shutdown.inprogress) {
            if(self->shutdown.phase2) { /* Phase 2 (all non-REGISTER and silent dialogs have been removed) */
                if(tsk_list_count(self->dialogs, pred_find_dialog_by_type, &regtype) == 0) {
                    /* alert only if there is not REGISTER dialog (ignore silents) */
                    TSK_DEBUG_INFO("== Shutting down - Phase-2 completed ==");
                    tsk_condwait_broadcast(self->shutdown.condwait);
                }
            }
            else { /* Phase 1 */
                if(tsk_list_count(self->dialogs, pred_find_dialog_by_not_type, &regtype) == 0) {
                    /* alert only if all dialogs except REGISTER have been removed */
                    TSK_DEBUG_INFO("== Shutting down - Phase-1 completed ==");
                    tsk_condwait_broadcast(self->shutdown.condwait);
                }
            }
        }

        tsk_safeobj_unlock(self);

        return 0;
    }

    return -1;
}

// this function is only called if no transaction match
// for responses, the transaction will always match
int tsip_dialog_layer_handle_incoming_msg(const tsip_dialog_layer_t *self, tsip_message_t* message)
{
    int ret = -1;
    tsk_bool_t cid_matched;
    tsip_dialog_t* dialog;
    tsip_transac_t* transac = tsk_null;
    const tsip_transac_layer_t *layer_transac = self->stack->layer_transac;

    if(!layer_transac) {
        goto bail;
    }

    //tsk_safeobj_lock(self);
    dialog = tsip_dialog_layer_find(self, message->Call_ID->value,
                                    TSIP_MESSAGE_IS_RESPONSE(message) ? message->To->tag : message->From->tag,
                                    TSIP_MESSAGE_IS_RESPONSE(message) ? message->From->tag : message->To->tag,
                                    TSIP_MESSAGE_IS_REQUEST(message) ? TSIP_MESSAGE_AS_REQUEST(message)->line.request.request_type : tsip_NONE,
                                    &cid_matched);
    //tsk_safeobj_unlock(self);

    if(dialog) {
        if(TSIP_REQUEST_IS_CANCEL(message) || TSIP_REQUEST_IS_ACK(message)) {
            ret = dialog->callback(dialog, tsip_dialog_i_msg, message);
            tsk_object_unref(dialog);
            goto bail;
        }
        else {
            static tsk_bool_t isCT = tsk_false;
            tsip_transac_dst_t* dst = tsip_transac_dst_dialog_create(dialog);
            transac = tsip_transac_layer_new(
                          layer_transac,
                          isCT,
                          message,
                          dst
                      );
            TSK_OBJECT_SAFE_FREE(dst);
            TSK_OBJECT_SAFE_FREE(dialog);
        }
    }
    else {
        /* MediaProxyMode : forward all non-INVITE messages */
        if(self->stack->network.mode == tsip_stack_mode_webrtc2sip) {
            tsk_bool_t b2bua;

            if(TSIP_MESSAGE_IS_REQUEST(message)) {
                // requests received over TCP/TLS/UDP must contain "ws-src-ip" and "ws-src-port" parameters
                if(!TNET_SOCKET_TYPE_IS_WS(message->src_net_type) && !TNET_SOCKET_TYPE_IS_WSS(message->src_net_type)) {
                    const char* ws_src_ip = tsk_params_get_param_value(message->line.request.uri->params, "ws-src-ip");
                    const tnet_port_t ws_src_port = (tnet_port_t)tsk_params_get_param_value_as_int(message->line.request.uri->params, "ws-src-port");
                    if(!tsip_transport_layer_have_stream_peer_with_remote_ip(self->stack->layer_transport, ws_src_ip, ws_src_port)) {
                        if(!TSIP_REQUEST_IS_ACK(message)) { // ACK do not expect response
#if 0 // code commented because when using mjserver, rejecting the forked INVITE terminate all dialogs: have to check if it's conform to RFC 3261 or not
                            tsip_response_t* response = tsip_response_new(488, "WebSocket Peer not connected", message);
                            ret = tsip_transport_layer_send(self->stack->layer_transport, "no-branch", response);
                            TSK_OBJECT_SAFE_FREE(response);
                            return ret;
#else
                            TSK_DEBUG_INFO("Request for peer at %s:%d cannot be delivered", ws_src_ip, ws_src_port);
#endif
                        }
                        return 0;
                    }
                }
            }

            // "rtcweb-breaker" parameter will be in the Contact header for outgoing request and in the request-uri for incoming requests
            b2bua = TSIP_REQUEST_IS_INVITE(message) && message->Contact && message->Contact->uri &&
                    (tsk_striequals(tsk_params_get_param_value(message->Contact->uri->params, "rtcweb-breaker"), "yes")
                     || tsk_striequals(tsk_params_get_param_value(message->line.request.uri->params, "rtcweb-breaker"), "yes"));

            if(!b2bua) {
                // forward the message
                static tsk_bool_t isCT = tsk_true;
                tsip_transac_dst_t* dst;
                tsip_transac_t* transac;

                TSIP_MESSAGE(message)->update = tsk_true; // update AoR and Via
                if((dst = tsip_transac_dst_net_create(TSIP_STACK(self->stack)))) {
                    if((transac = tsip_transac_layer_new(self->stack->layer_transac, isCT, message, dst))) {
                        ret = tsip_transac_start(transac, message);
                        TSK_OBJECT_SAFE_FREE(transac);
                    }
                    TSK_OBJECT_SAFE_FREE(dst);
                }
                return ret;
            }
        }

        if(TSIP_MESSAGE_IS_REQUEST(message)) {
            tsip_ssession_t* ss = tsk_null;
            tsip_dialog_t* newdialog = tsk_null;

            switch(message->line.request.request_type) {
            case tsip_MESSAGE: {
                /* Server incoming MESSAGE */
                if((ss = tsip_ssession_create_2(self->stack, message))) {
                    newdialog = (tsip_dialog_t*)tsip_dialog_message_create(ss);
                }
                break;
            }
            case tsip_INFO: {
                /* Server incoming INFO */
                if((ss = tsip_ssession_create_2(self->stack, message))) {
                    newdialog = (tsip_dialog_t*)tsip_dialog_info_create(ss);
                }
                break;
            }
            case tsip_OPTIONS: {
                /* Server incoming OPTIONS */
                if((ss = tsip_ssession_create_2(self->stack, message))) {
                    newdialog = (tsip_dialog_t*)tsip_dialog_options_create(ss);
                }
                break;
            }

            case tsip_REGISTER: {
                /* incoming REGISTER */
                if((ss = tsip_ssession_create_2(self->stack, message))) {
                    newdialog = (tsip_dialog_t*)tsip_dialog_register_create(ss, message->Call_ID ? message->Call_ID->value : tsk_null);
                }
                break;
            }

            case tsip_INVITE: {
                /* incoming INVITE */
                if((ss = tsip_ssession_create_2(self->stack, message))) {
                    newdialog = (tsip_dialog_t*)tsip_dialog_invite_create(ss, message->Call_ID ? message->Call_ID->value : tsk_null);
                }
                break;
            }

            default: {
                break;
            }
            }//switch

            // for new dialog, create a new transac and start it later
            if(newdialog) {
                static const tsk_bool_t isCT = tsk_false;
                tsip_transac_dst_t* dst = tsip_transac_dst_dialog_create(newdialog);
                transac = tsip_transac_layer_new(
                              layer_transac,
                              isCT,
                              message,
                              dst
                          );
                if(message->local_fd > 0 && TNET_SOCKET_TYPE_IS_STREAM(message->src_net_type)) {
                    tsip_dialog_set_connected_fd(newdialog, message->local_fd);
                }
                tsk_list_push_back_data(self->dialogs, (void**)&newdialog); /* add new dialog to the layer */
                TSK_OBJECT_SAFE_FREE(dst);
            }

            /* The dialog will become the owner of the SIP session
            * => when destoyed => SIP session will be destroyed, unless the user-end takes ownership() */
            TSK_OBJECT_SAFE_FREE(ss);
        }
    }

    if(transac) {
        ret = tsip_transac_start(transac, message);
        tsk_object_unref(transac);
    }
    /* - No transaction match for the SIP request
       - ACK do not expect any response (http://code.google.com/p/imsdroid/issues/detail?id=420)
    */
    else if(TSIP_MESSAGE_IS_REQUEST(message) && !TSIP_REQUEST_IS_ACK(message)) {
        const tsip_transport_layer_t *layer;
        tsip_response_t* response = tsk_null;
        if(!dialog && cid_matched) {
            dialog = tsip_dialog_layer_find_by_callid((tsip_dialog_layer_t *)self, message->Call_ID->value);
        }

        if((layer = self->stack->layer_transport)) {
            if(cid_matched) { /* We are receiving our own message. */
                response = tsip_response_new(482, "Loop Detected (Check your iFCs)", message);
                if(response && !response->To->tag) { /* Early dialog? */
                    response->To->tag = tsk_strdup("doubango");
                }
            }
            else {
                switch(message->line.request.request_type) {
                case tsip_OPTIONS: // Hacked to work on Tiscali IMS networks
                case tsip_INFO:
                    response = tsip_response_new(405, "Method Not Allowed", message);
                    break;
                default:
                    response = tsip_response_new(481, "Dialog/Transaction Does Not Exist", message);
                    break;
                }
            }
            if(response) {
                if(dialog && TSIP_DIALOG_GET_SS(dialog)) {
                    tsk_strupdate(&response->sigcomp_id, TSIP_DIALOG_GET_SS(dialog)->sigcomp_id);
                }
                ret = tsip_transport_layer_send(layer, response->firstVia ? response->firstVia->branch : "no-branch", response);
                TSK_OBJECT_SAFE_FREE(response);
            }
        }

        TSK_OBJECT_SAFE_FREE(dialog);
    }

bail:
    return ret;
}





//========================================================
//	Dialog layer object definition
//
static tsk_object_t* tsip_dialog_layer_ctor(tsk_object_t * self, va_list * app)
{
    tsip_dialog_layer_t *layer = self;
    if(layer) {
        layer->stack = va_arg(*app, const tsip_stack_t *);
        layer->dialogs = tsk_list_create();

        tsk_safeobj_init(layer);
    }
    return self;
}

static tsk_object_t* tsip_dialog_layer_dtor(tsk_object_t * self)
{
    tsip_dialog_layer_t *layer = self;
    if(layer) {
        TSK_OBJECT_SAFE_FREE(layer->dialogs);

        /* condwait */
        if(layer->shutdown.condwait) {
            tsk_condwait_destroy(&layer->shutdown.condwait);
        }

        tsk_safeobj_deinit(layer);

        TSK_DEBUG_INFO("*** Dialog Layer destroyed ***");
    }
    return self;
}

static int tsip_dialog_layer_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
    return -1;
}

static const tsk_object_def_t tsip_dialog_layer_def_s = {
    sizeof(tsip_dialog_layer_t),
    tsip_dialog_layer_ctor,
    tsip_dialog_layer_dtor,
    tsip_dialog_layer_cmp,
};
const tsk_object_def_t *tsip_dialog_layer_def_t = &tsip_dialog_layer_def_s;
