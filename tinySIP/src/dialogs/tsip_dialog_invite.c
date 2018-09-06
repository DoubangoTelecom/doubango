/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
*
* This file is part of Open Source Doubango Framework.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as publishd by
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

/**@file tsip_dialog_invite.c
 * @brief SIP dialog INVITE as per RFC 3261.
 * The SOA machine is designed as per RFC 3264 and draft-ietf-sipping-sip-offeranswer-12.
 * MMTel services implementation follow 3GPP TS 24.173.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/dialogs/tsip_dialog_invite.h"

#include "tinysip/dialogs/tsip_dialog_invite.common.h"

#include "tinysip/transactions/tsip_transac_layer.h"
#include "tinysip/transports/tsip_transport_layer.h"
#include "tinysip/dialogs/tsip_dialog_layer.h"

#include "tinysip/headers/tsip_header_Allow.h"
#include "tinysip/headers/tsip_header_Dummy.h"
#include "tinysip/headers/tsip_header_Max_Forwards.h"
#include "tinysip/headers/tsip_header_Min_SE.h"
#include "tinysip/headers/tsip_header_RAck.h"
#include "tinysip/headers/tsip_header_Require.h"
#include "tinysip/headers/tsip_header_RSeq.h"
#include "tinysip/headers/tsip_header_Session_Expires.h"
#include "tinysip/headers/tsip_header_Supported.h"

#include "tinysdp/parsers/tsdp_parser_message.h"

#include "tinymedia/tmedia_defaults.h"

#include "tsk_debug.h"

#if METROPOLIS
#	define TSIP_INFO_FASTUPDATE_OUT_INTERVAL_MIN		0 // millis
#else
#	define TSIP_INFO_FASTUPDATE_OUT_INTERVAL_MIN		1500 // millis
#endif

#if HAVE_LIBXML2
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#endif

// http://cdnet.stpi.org.tw/techroom/market/_pdf/2009/eetelecomm_09_009_OneVoiceProfile.pdf
// 3GPP TS 26.114 (MMTel): Media handling and interaction
// 3GPP TS 24.173 (MMTel): Supplementary Services
//
/* ======================== MMTel Supplementary Services ========================
3GPP TS 24.607 : Originating Identification Presentation
3GPP TS 24.608 : Terminating Identification Presentation
3GPP TS 24.607 : Originating Identification Restriction
3GPP TS 24.608 : Terminating Identification Restriction

3GPP TS 24.604 : Communication Diversion Unconditional
3GPP TS 24.604 : Communication Diversion on not Logged
3GPP TS 24.604 : Communication Diversion on Busy
3GPP TS 24.604 : Communication Diversion on not Reachable
3GPP TS 24.604 : Communication Diversion on No Reply
3GPP TS 24.611 : Barring of All Incoming Calls
3GPP TS 24.611 : Barring of All Outgoing Calls
3GPP TS 24.611 : Barring of Outgoing International Calls
3GPP TS 24.611 : Barring of Incoming Calls - When Roaming
3GPP TS 24.610 : Communication Hold
3GPP TS 24.606 : Message Waiting Indication
3GPP TS 24.615 : Communication Waiting
3GPP TS 24.605 : Ad-Hoc Multi Party Conference
*/

extern int tsip_dialog_add_session_headers(const tsip_dialog_t *self, tsip_request_t* request);

/* ======================== internal functions ======================== */
/*static*/ int tsip_dialog_invite_msession_start(tsip_dialog_invite_t *self);
/*static*/ int tsip_dialog_invite_msession_configure(tsip_dialog_invite_t *self);
/*static*/ int send_INVITEorUPDATE(tsip_dialog_invite_t *self, tsk_bool_t is_INVITE, tsk_bool_t force_sdp);
/*static*/ int send_PRACK(tsip_dialog_invite_t *self, const tsip_response_t* r1xx);
/*static*/ int send_ACK(tsip_dialog_invite_t *self, const tsip_response_t* r2xxINVITE);
/*static*/ int send_RESPONSE(tsip_dialog_invite_t *self, const tsip_request_t* request, short code, const char* phrase, tsk_bool_t force_sdp);
/*static*/ int send_ERROR(tsip_dialog_invite_t* self, const tsip_request_t* request, short code, const char* phrase, const char* reason);
/*static*/ int send_BYE(tsip_dialog_invite_t *self);
/*static*/ int send_CANCEL(tsip_dialog_invite_t *self);
/*static*/ int tsip_dialog_invite_notify_parent(tsip_dialog_invite_t *self, const tsip_response_t* response);
/*static*/ int send_INFO(tsip_dialog_invite_t *self, const char* content_type, const void* content_ptr, tsk_size_t content_size);
static int tsip_dialog_invite_OnTerminated(tsip_dialog_invite_t *self);
static int tsip_dialog_invite_msession_onerror_cb(const void* usrdata, const struct tmedia_session_s* session, const char* reason, tsk_bool_t is_fatal);
static int tsip_dialog_invite_msession_rfc5168_cb(const void* usrdata, const struct tmedia_session_s* session, const char* reason, enum tmedia_session_rfc5168_cmd_e command);

/* ======================== external functions ======================== */
extern int tsip_dialog_invite_ice_process_ro(tsip_dialog_invite_t * self, const tsdp_message_t* sdp_ro, tsk_bool_t is_remote_offer);
extern int tsip_dialog_invite_ice_set_media_type(tsip_dialog_invite_t * self, tmedia_type_t media_type);
extern int tsip_dialog_invite_stimers_cancel(tsip_dialog_invite_t* self);
extern int tsip_dialog_invite_qos_timer_cancel(tsip_dialog_invite_t* self);
extern int tsip_dialog_invite_qos_timer_schedule(tsip_dialog_invite_t* self);
extern int tsip_dialog_invite_stimers_schedule(tsip_dialog_invite_t* self, uint64_t timeout);
extern int tsip_dialog_invite_stimers_handle(tsip_dialog_invite_t* self, const tsip_message_t* message);
extern int tsip_dialog_invite_hold_handle(tsip_dialog_invite_t* self, const tsip_request_t* rINVITEorUPDATE);

extern int tsip_dialog_invite_ice_timers_set(tsip_dialog_invite_t *self, int64_t timeout);
extern tsk_bool_t tsip_dialog_invite_ice_is_enabled(const tsip_dialog_invite_t * self);
extern tsk_bool_t tsip_dialog_invite_ice_is_connected(const tsip_dialog_invite_t * self);
extern int tsip_dialog_invite_ice_process_lo(tsip_dialog_invite_t * self, const tsdp_message_t* sdp_lo);

/* ======================== transitions ======================== */
static int x0000_Connected_2_Connected_X_oDTMF(va_list *app);
static int x0000_Connected_2_Connected_X_oLMessage(va_list *app);
static int x0000_Connected_2_Connected_X_iACK(va_list *app);
static int x0000_Connected_2_Connected_X_iINVITEorUPDATE(va_list *app);
static int x0000_Connected_2_Connected_X_oINVITE(va_list *app);


static int x0000_Any_2_Any_X_i1xx(va_list *app);
static int x0000_Any_2_Any_X_oINFO(va_list *app);
static int x0000_Any_2_Any_X_iINFO(va_list *app);
static int x0000_Any_2_Any_X_i401_407_Challenge(va_list *app);
static int x0000_Any_2_Any_X_i2xxINVITEorUPDATE(va_list *app);

static int x0000_Any_2_Any_X_iPRACK(va_list *app);
static int x0000_Any_2_Any_X_iOPTIONS(va_list *app);
static int x0000_Any_2_Trying_X_oBYE(va_list *app); /* If not Connected => Cancel will be called instead. See tsip_dialog_hangup() */
static int x0000_Any_2_Terminated_X_iBYE(va_list *app);
static int x0000_Any_2_Trying_X_shutdown(va_list *app);

static int x9998_Any_2_Terminated_X_transportError(va_list *app);
static int x9999_Any_2_Any_X_Error(va_list *app);

/* ======================== conds ======================== */
static tsk_bool_t _fsm_cond_is_resp2INVITE(tsip_dialog_invite_t* self, tsip_message_t* message)
{
    return TSIP_RESPONSE_IS_TO_INVITE(message);
}
static tsk_bool_t _fsm_cond_is_resp2UPDATE(tsip_dialog_invite_t* self, tsip_message_t* message)
{
    return TSIP_RESPONSE_IS_TO_UPDATE(message);
}
static tsk_bool_t _fsm_cond_is_resp2BYE(tsip_dialog_invite_t* self, tsip_message_t* message)
{
    return TSIP_RESPONSE_IS_TO_BYE(message);
}
static tsk_bool_t _fsm_cond_is_resp2PRACK(tsip_dialog_invite_t* self, tsip_message_t* message)
{
    return TSIP_RESPONSE_IS_TO_PRACK(message);
}
static tsk_bool_t _fsm_cond_is_resp2INFO(tsip_dialog_invite_t* self, tsip_message_t* message)
{
    return TSIP_RESPONSE_IS_TO_INFO(message);
}

/* ======================== actions ======================== */
/* #include "tinysip/dialogs/tsip_dialog_invite.common.h" */

/* ======================== states ======================== */
/* #include "tinysip/dialogs/tsip_dialog_invite.common.h" */

/* ICE handler */
extern int tsip_dialog_invite_ice_init(tsip_dialog_invite_t *self);
/* Client-Side dialog */
extern int tsip_dialog_invite_client_init(tsip_dialog_invite_t *self);
/* Server-Side dialog */
extern int tsip_dialog_invite_server_init(tsip_dialog_invite_t *self);
/* 3GPP TS 24.610: Communication Hold  */
extern int tsip_dialog_invite_hold_init(tsip_dialog_invite_t *self);
/* 3GPP TS 24.629: Explicit Communication Transfer (ECT) using IP Multimedia (IM) Core Network (CN) subsystem */
extern int tsip_dialog_invite_ect_init(tsip_dialog_invite_t *self);
/* RFC 4028: Session Timers */
extern int tsip_dialog_invite_stimers_init(tsip_dialog_invite_t *self);
/* RFC 3312: Integration of Resource Management and Session Initiation Protocol (SIP) */
extern int tsip_dialog_invite_qos_init(tsip_dialog_invite_t *self);

int tsip_dialog_invite_event_callback(const tsip_dialog_invite_t *self, tsip_dialog_event_type_t type, const tsip_message_t *msg)
{
    int ret = -1;

    switch(type) {
    case tsip_dialog_i_msg: {
        if(msg) {
            if(TSIP_MESSAGE_IS_RESPONSE(msg)) { /* Response */
                const tsip_action_t* action = tsip_dialog_keep_action(TSIP_DIALOG(self), msg) ? TSIP_DIALOG(self)->curr_action : tsk_null;
                if(TSIP_RESPONSE_IS_1XX(msg)) { // 100-199
                    ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_i1xx, msg, action);
                }
                else if(TSIP_RESPONSE_IS_2XX(msg)) { // 200-299
                    ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_i2xx, msg, action);
                }
                else if(TSIP_RESPONSE_CODE(msg) == 401 || TSIP_RESPONSE_CODE(msg) == 407) { // 401,407
                    ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_i401_i407, msg, action);
                }
                else if(TSIP_RESPONSE_CODE(msg) == 422) { // 422
                    ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_i422, msg, action);
                }
                else if(TSIP_RESPONSE_IS_3456(msg)) { // 300-699
                    ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_i300_to_i699, msg, action);
                }
                else; // Ignore
            }
            else { /* Request */
                if(TSIP_REQUEST_IS_INVITE(msg)) { // INVITE
                    ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_iINVITE, msg, tsk_null);
                }
                else if(TSIP_REQUEST_IS_UPDATE(msg)) { // UPDATE
                    ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_iUPDATE, msg, tsk_null);
                }
                else if(TSIP_REQUEST_IS_PRACK(msg)) { // PRACK
                    ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_iPRACK, msg, tsk_null);
                }
                else if(TSIP_REQUEST_IS_ACK(msg)) { // ACK
                    ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_iACK, msg, tsk_null);
                }
                else if(TSIP_REQUEST_IS_OPTIONS(msg)) { // OPTIONS
                    ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_iOPTIONS, msg, tsk_null);
                }
                else if(TSIP_REQUEST_IS_BYE(msg)) { // BYE
                    ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_iBYE, msg, tsk_null);
                }
                else if(TSIP_REQUEST_IS_CANCEL(msg)) { // CANCEL
                    ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_iCANCEL, msg, tsk_null);
                }
                else if(TSIP_REQUEST_IS_INFO(msg)) { // INFO
                    ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_iINFO, msg, tsk_null);
                }
                else if(TSIP_REQUEST_IS_NOTIFY(msg)) { // NOTIFY
                    ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_iNOTIFY, msg, tsk_null);
                }
                else if(TSIP_REQUEST_IS_REFER(msg)) { // REFER
                    ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_iREFER, msg, tsk_null);
                }
            }
        }
        break;
    }

    case tsip_dialog_canceled: {
        ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_oCANCEL, msg, tsk_null);
        break;
    }

    case tsip_dialog_timedout: {
        // Do nothing if request type is "INFO"
        if(!TSIP_MESSAGE_IS_REQUEST(msg) || !TSIP_REQUEST_IS_INFO(msg)) {
            ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_transporterror, msg, tsk_null);
        }
        break;
    }
    case tsip_dialog_terminated:
    case tsip_dialog_error:
    case tsip_dialog_transport_error: {
        ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_transporterror, msg, tsk_null);
        break;
    }

    default:
        break;
    }

    return ret;
}

/**Timer manager callback.
 *
 * @param self	The owner of the signaled timer.
 * @param	timer_id		The identifier of the signaled timer.
 *
 * @return	Zero if succeed and non-zero error code otherwise.
**/
int tsip_dialog_invite_timer_callback(const tsip_dialog_invite_t* self, tsk_timer_id_t timer_id)
{
    int ret = -1;

    if(self) {
        if(timer_id == self->stimers.timer.id) {
            ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_timerRefresh, tsk_null, tsk_null);
        }
        else if(timer_id == self->timer100rel.id) {
            ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_timer100rel, tsk_null, tsk_null);
        }
        else if(timer_id == self->qos.timer.id) {
            ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_timerRSVP, tsk_null, tsk_null);
        }
        else if(timer_id == self->timershutdown.id) {
            ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_shutdown_timedout, tsk_null, tsk_null);
        }
    }
    return ret;
}

tsip_dialog_invite_t* tsip_dialog_invite_create(const tsip_ssession_handle_t* ss, const char* call_id)
{
    return tsk_object_new(tsip_dialog_invite_def_t,  ss, call_id);
}

int tsip_dialog_invite_init(tsip_dialog_invite_t *self)
{
    /* special cases (fsm) should be tried first */

    /* ICE */
    tsip_dialog_invite_ice_init(self);
    /* Client-Side dialog */
    tsip_dialog_invite_client_init(self);
    /* Server-Side dialog */
    tsip_dialog_invite_server_init(self);
    /* 3GPP TS 24.610: Communication Hold  */
    tsip_dialog_invite_hold_init(self);
    /* 3GPP TS 24.629: Explicit Communication Transfer (ECT) using IP Multimedia (IM) Core Network (CN) subsystem */
    tsip_dialog_invite_ect_init(self);
    /* RFC 4028: Session Timers */
    tsip_dialog_invite_stimers_init(self);
    /* RFC 3312: Integration of Resource Management and Session Initiation Protocol (SIP) */
    tsip_dialog_invite_qos_init(self);

    /* Initialize the state machine (all other cases) */
    tsk_fsm_set(TSIP_DIALOG_GET_FSM(self),

                /*=======================
                * === Started ===
                */
                // Started -> (Any) -> Started
                TSK_FSM_ADD_ALWAYS_NOTHING(_fsm_state_Started, "tsip_dialog_invite_Started_2_Started_X_any"),

                /*=======================
                * === Connected ===
                */
                // Connected -> (Send DTMF) -> Connected
                TSK_FSM_ADD_ALWAYS(_fsm_state_Connected, _fsm_action_dtmf_send, _fsm_state_Connected, x0000_Connected_2_Connected_X_oDTMF, "x0000_Connected_2_Connected_X_oDTMF"),
                // Connected -> (Send MSRP message) -> Connected
                TSK_FSM_ADD_ALWAYS(_fsm_state_Connected, _fsm_action_msrp_send_msg, _fsm_state_Connected, x0000_Connected_2_Connected_X_oLMessage, "x0000_Connected_2_Connected_X_oLMessage"),
                // Connected -> (iACK) -> Connected
                TSK_FSM_ADD_ALWAYS(_fsm_state_Connected, _fsm_action_iACK, _fsm_state_Connected, x0000_Connected_2_Connected_X_iACK, "x0000_Connected_2_Connected_X_iACK"),
                // Connected -> (iINVITE) -> Connected
                TSK_FSM_ADD_ALWAYS(_fsm_state_Connected, _fsm_action_iINVITE, _fsm_state_Connected, x0000_Connected_2_Connected_X_iINVITEorUPDATE, "x0000_Connected_2_Connected_X_iINVITE"),
                // Connected -> (iUPDATE) -> Connected
                TSK_FSM_ADD_ALWAYS(_fsm_state_Connected, _fsm_action_iUPDATE, _fsm_state_Connected, x0000_Connected_2_Connected_X_iINVITEorUPDATE, "x0000_Connected_2_Connected_X_iUPDATE"),
                // Connected -> (send reINVITE) -> Connected
                TSK_FSM_ADD_ALWAYS(_fsm_state_Connected, _fsm_action_oINVITE, _fsm_state_Connected, x0000_Connected_2_Connected_X_oINVITE, "x0000_Connected_2_Connected_X_oINVITE"),

                /*=======================
                * === BYE/SHUTDOWN ===
                */
                // Any -> (oBYE) -> Trying
                TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_oBYE, _fsm_state_Trying, x0000_Any_2_Trying_X_oBYE, "x0000_Any_2_Trying_X_oBYE"),
                // Any -> (iBYE) -> Terminated
                TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_iBYE, _fsm_state_Terminated, x0000_Any_2_Terminated_X_iBYE, "x0000_Any_2_Terminated_X_iBYE"),
                // Any -> (i401/407 BYE) -> Any
                TSK_FSM_ADD(tsk_fsm_state_any, _fsm_action_i401_i407, _fsm_cond_is_resp2BYE, tsk_fsm_state_any, x0000_Any_2_Any_X_i401_407_Challenge, "x0000_Any_2_Any_X_i401_407_Challenge"),
                // Any -> (i3xx-i6xx BYE) -> Terminated
                TSK_FSM_ADD(tsk_fsm_state_any, _fsm_action_i300_to_i699, _fsm_cond_is_resp2BYE, _fsm_state_Terminated, tsk_null, "x0000_Any_2_Terminated_X_i3xxTOi6xxBYE"),
                // Any -> (i2xxx BYE) -> Terminated
                TSK_FSM_ADD(tsk_fsm_state_any, _fsm_action_i2xx, _fsm_cond_is_resp2BYE, _fsm_state_Terminated, tsk_null, "x0000_Any_2_Terminated_X_i2xxBYE"),
                // Any -> (Shutdown) -> Trying
                TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_oShutdown, _fsm_state_Trying, x0000_Any_2_Trying_X_shutdown, "x0000_Any_2_Trying_X_shutdown"),
                // Any -> (shutdown timedout) -> Terminated
                TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_shutdown_timedout, _fsm_state_Terminated, tsk_null, "tsip_dialog_invite_shutdown_timedout"),


                /*=======================
                * === Any ===
                */
                // Any -> (i1xx) -> Any
                TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_i1xx, tsk_fsm_state_any, x0000_Any_2_Any_X_i1xx, "x0000_Any_2_Any_X_i1xx"),
                // Any -> (oINFO) -> Any
                TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_oINFO, tsk_fsm_state_any, x0000_Any_2_Any_X_oINFO, "x0000_Any_2_Any_X_oINFO"),
                // Any -> (iINFO) -> Any
                TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_iINFO, tsk_fsm_state_any, x0000_Any_2_Any_X_iINFO, "x0000_Any_2_Any_X_iINFO"),
                // Any -> (i401/407)
                //
                // Any -> (iPRACK) -> Any
                TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_iPRACK, tsk_fsm_state_any, x0000_Any_2_Any_X_iPRACK, "x0000_Any_2_Any_X_iPRACK"),
                // Any -> (iOPTIONS) -> Any
                TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_iOPTIONS, tsk_fsm_state_any, x0000_Any_2_Any_X_iOPTIONS, "x0000_Any_2_Any_X_iOPTIONS"),
                // Any -> (i2xx INVITE) -> Any
                TSK_FSM_ADD(tsk_fsm_state_any, _fsm_action_i2xx, _fsm_cond_is_resp2INVITE, tsk_fsm_state_any, x0000_Any_2_Any_X_i2xxINVITEorUPDATE, "x0000_Any_2_Any_X_i2xxINVITE"),
                // Any -> (i2xx UPDATE) -> Any
                TSK_FSM_ADD(tsk_fsm_state_any, _fsm_action_i2xx, _fsm_cond_is_resp2UPDATE, tsk_fsm_state_any, x0000_Any_2_Any_X_i2xxINVITEorUPDATE, "x0000_Any_2_Any_X_i2xxUPDATE"),
                // Any -> (i401/407 INVITE) -> Any
                TSK_FSM_ADD(tsk_fsm_state_any, _fsm_action_i401_i407, _fsm_cond_is_resp2INVITE, tsk_fsm_state_any, x0000_Any_2_Any_X_i401_407_Challenge, "x0000_Any_2_Any_X_i401_407_Challenge"),
                // Any -> (i401/407  UPDATE) -> Any
                TSK_FSM_ADD(tsk_fsm_state_any, _fsm_action_i401_i407, _fsm_cond_is_resp2UPDATE, tsk_fsm_state_any, x0000_Any_2_Any_X_i401_407_Challenge, "x0000_Any_2_Any_X_i401_407_Challenge"),
                // Any -> (i2xx PRACK) -> Any
                TSK_FSM_ADD(tsk_fsm_state_any, _fsm_action_i2xx, _fsm_cond_is_resp2PRACK, tsk_fsm_state_any, tsk_null, "x0000_Any_2_Any_X_i2xxPRACK"),
                // Any -> (i2xx INFO) -> Any
                TSK_FSM_ADD(tsk_fsm_state_any, _fsm_action_i2xx, _fsm_cond_is_resp2INFO, tsk_fsm_state_any, tsk_null, "x0000_Any_2_Any_X_i2xxINFO"),
                // Any -> (transport error) -> Terminated
                TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_transporterror, _fsm_state_Terminated, x9998_Any_2_Terminated_X_transportError, "x9998_Any_2_Terminated_X_transportError"),
                // Any -> (transport error) -> Terminated
                TSK_FSM_ADD_ALWAYS(tsk_fsm_state_any, _fsm_action_error, _fsm_state_Terminated, x9999_Any_2_Any_X_Error, "x9999_Any_2_Any_X_Error"),

                TSK_FSM_ADD_NULL());

    /* Sets callback function */
    TSIP_DIALOG(self)->callback = TSIP_DIALOG_EVENT_CALLBACK_F(tsip_dialog_invite_event_callback);

    /* Timers */
    self->timer100rel.id = TSK_INVALID_TIMER_ID;
    self->stimers.timer.id = TSK_INVALID_TIMER_ID;
    self->timershutdown.id = TSK_INVALID_TIMER_ID;
    self->timershutdown.timeout = TSIP_DIALOG_SHUTDOWN_TIMEOUT;

    return 0;
}

// start sending
int tsip_dialog_invite_start(tsip_dialog_invite_t *self)
{
    int ret = -1;
    if(self && !TSIP_DIALOG(self)->running) {
        if(!(ret = tsip_dialog_fsm_act(TSIP_DIALOG(self), _fsm_action_oINVITE, tsk_null, tsk_null))) {
            TSIP_DIALOG(self)->running = tsk_true;
        }
    }
    return ret;
}

int tsip_dialog_invite_process_ro(tsip_dialog_invite_t *self, const tsip_message_t* message)
{
    tsdp_message_t* sdp_ro = tsk_null;
    tmedia_type_t old_media_type;
    tmedia_type_t new_media_type;
    tsk_bool_t media_session_was_null;
    int ret = 0;
    tmedia_ro_type_t ro_type = tmedia_ro_type_none;

    if(!self || !message) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (self->is_cancelling) {
        TSK_DEBUG_INFO("Cancelling the INVITE...ignore the incoming SDP");
        return 0;
    }

    /* Parse SDP content */
    if(TSIP_MESSAGE_HAS_CONTENT(message)) {
        if(tsk_striequals("application/sdp", TSIP_MESSAGE_CONTENT_TYPE(message))) {
            if(!(sdp_ro = tsdp_message_parse(TSIP_MESSAGE_CONTENT_DATA(message), TSIP_MESSAGE_CONTENT_DATA_LENGTH(message)))) {
                TSK_DEBUG_ERROR("Failed to parse remote sdp message:\n [%s]", (const char*)TSIP_MESSAGE_CONTENT_DATA(message));
                return -2;
            }
            // ICE processing
            if(self->supported.ice) {
                tsip_dialog_invite_ice_process_ro(self, sdp_ro, TSIP_MESSAGE_IS_REQUEST(message));
            }
        }
        else {
            TSK_DEBUG_ERROR("[%s] content-type is not supportted", TSIP_MESSAGE_CONTENT_TYPE(message));
            return -3;
        }
    }
    else {
        if(TSIP_DIALOG(self)->state == tsip_initial && TSIP_REQUEST_IS_INVITE(message)) { /* Bodiless initial INVITE */
            TSIP_DIALOG_GET_SS(self)->media.type = tmedia_defaults_get_media_type(); // Default media for initial INVITE to send with the first reliable answer
        }
        else {
            return 0;
        }
    }

    ro_type = (TSIP_REQUEST_IS_INVITE(message) || TSIP_REQUEST_IS_UPDATE(message)) // ACK/PRACK can only contain a response if the initial INVITE was bodiless
              ? tmedia_ro_type_offer
              :(TSIP_RESPONSE_IS_1XX(message) ? tmedia_ro_type_provisional : tmedia_ro_type_answer);
    media_session_was_null = (self->msession_mgr == tsk_null);
    old_media_type = TSIP_DIALOG_GET_SS(self)->media.type;
    new_media_type = sdp_ro ? tmedia_type_from_sdp(sdp_ro) : old_media_type;

    /* Create session Manager if not already done */
    if(!self->msession_mgr) {
        int32_t transport_idx = TSIP_DIALOG_GET_STACK(self)->network.transport_idx_default;
        TSIP_DIALOG_GET_SS(self)->media.type = new_media_type;
        self->msession_mgr = tmedia_session_mgr_create(TSIP_DIALOG_GET_SS(self)->media.type, TSIP_DIALOG_GET_STACK(self)->network.local_ip[transport_idx],
                             TNET_SOCKET_TYPE_IS_IPV6(TSIP_DIALOG_GET_STACK(self)->network.proxy_cscf_type[transport_idx]), (sdp_ro == tsk_null));
        if(TSIP_DIALOG_GET_STACK(self)->natt.ctx) {
            tmedia_session_mgr_set_natt_ctx(self->msession_mgr, TSIP_DIALOG_GET_STACK(self)->natt.ctx, TSIP_DIALOG_GET_STACK(self)->network.aor.ip[transport_idx]);
        }
        ret = tmedia_session_mgr_set_ice_ctx(self->msession_mgr, self->ice.ctx_audio, self->ice.ctx_video);
    }

    if(sdp_ro) {
        if (tmedia_session_mgr_is_new_ro(self->msession_mgr, sdp_ro)) {
            ret = tsip_dialog_invite_msession_configure(self);
        }
        if((ret = tmedia_session_mgr_set_ro(self->msession_mgr, sdp_ro, ro_type))) {
            TSK_DEBUG_ERROR("Failed to set remote offer");
            goto bail;
        }
    }

    // is media update?
    // (old_media_type == new_media_type) means the new session are rejected. This is way we match the CSeq
    if(!media_session_was_null && (old_media_type != new_media_type || (TSIP_MESSAGE_IS_RESPONSE(message) && self->cseq_out_media_update == message->CSeq->seq)) && (self->msession_mgr->sdp.lo && self->msession_mgr->sdp.ro)) {
        // at this point the media session manager has been succeffuly started and all is ok
        TSIP_DIALOG_GET_SS(self)->media.type = new_media_type;
        TSIP_DIALOG_INVITE_SIGNAL(self, tsip_m_updated,
                                  TSIP_RESPONSE_CODE(message), TSIP_RESPONSE_PHRASE(message), message);
    }

    /* start session manager */
    if(!self->msession_mgr->started && (self->msession_mgr->sdp.lo && self->msession_mgr->sdp.ro)) {
        /* Set MSRP Callback */
        if((self->msession_mgr->type & tmedia_msrp) == tmedia_msrp) {
            tmedia_session_mgr_set_msrp_cb(self->msession_mgr, TSIP_DIALOG_GET_SS(self)->userdata, TSIP_DIALOG_GET_SS(self)->media.msrp.callback);
        }
        /* starts session manager*/
        ret = tsip_dialog_invite_msession_start(self);

        if(ret == 0 && TSIP_DIALOG(self)->state == tsip_early) {
            TSIP_DIALOG_INVITE_SIGNAL(self, tsip_m_early_media,
                                      TSIP_RESPONSE_CODE(message), TSIP_RESPONSE_PHRASE(message), message);
        }
    }

bail:
    TSK_OBJECT_SAFE_FREE(sdp_ro);

    return ret;
}


//--------------------------------------------------------
//				== STATE MACHINE BEGIN ==
//--------------------------------------------------------

int x0000_Connected_2_Connected_X_oDTMF(va_list *app)
{
    int ret;
    tsip_dialog_invite_t *self;
    const tsip_action_t* action;

    self = va_arg(*app, tsip_dialog_invite_t *);
    va_arg(*app, const tsip_message_t *);
    action = va_arg(*app, const tsip_action_t *);

    if(action) {
        ret = tmedia_session_mgr_send_dtmf(self->msession_mgr, action->dtmf.event);
    }
    else {
        TSK_DEBUG_ERROR("Invalid action");
    }

    return 0; /* always */
}

int x0000_Connected_2_Connected_X_oLMessage(va_list *app)
{
    int ret;
    tsip_dialog_invite_t *self;
    const tsip_action_t* action;

    self = va_arg(*app, tsip_dialog_invite_t *);
    va_arg(*app, const tsip_message_t *);
    action = va_arg(*app, const tsip_action_t *);

    if(action && action->payload) {
        ret = tmedia_session_mgr_send_message(self->msession_mgr, action->payload->data, action->payload->size,
                                              action->media.params);
    }
    else {
        TSK_DEBUG_ERROR("Invalid action");
    }

    return 0;
}

/* Connected -> (iACK) -> Connected */
int x0000_Connected_2_Connected_X_iACK(va_list *app)
{
    tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
    const tsip_request_t *rACK = va_arg(*app, const tsip_request_t *);
    int ret;

    // Nothing to do (in future will be used to ensure the session)

    /* No longer waiting for the initial ACK */
    self->is_initial_iack_pending = tsk_false;

    /* Process remote offer */
    if((ret = tsip_dialog_invite_process_ro(self, rACK))) {
        /* Send error */
        return ret;
    }

    /* Starts media session if not already done */
    if(!self->msession_mgr->started && (self->msession_mgr->sdp.lo && self->msession_mgr->sdp.ro)) {
        ret = tsip_dialog_invite_msession_start(self);
    }

    // starts ICE timers now that both parties receive the "candidates"
    if(tsip_dialog_invite_ice_is_enabled(self)) {
        tsip_dialog_invite_ice_timers_set(self, (self->required.ice ? -1 : TSIP_DIALOG_INVITE_ICE_CONNCHECK_TIMEOUT));
    }

    /* alert the user */
    TSIP_DIALOG_INVITE_SIGNAL(self, tsip_i_request,
                              tsip_event_code_dialog_request_incoming, "Incoming Request", rACK);

    return 0;
}

/* Connected -> (iINVITE or iUPDATE) -> Connected */
int x0000_Connected_2_Connected_X_iINVITEorUPDATE(va_list *app)
{
    tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
    const tsip_request_t *rINVITEorUPDATE = va_arg(*app, const tsip_request_t *);

    int ret = 0;
    tsk_bool_t bodiless_invite;
    tmedia_type_t old_media_type = self->msession_mgr ? self->msession_mgr->type : tmedia_none;
    tmedia_type_t new_media_type;
    tsk_bool_t force_sdp;

    /* process remote offer */
    if((ret = tsip_dialog_invite_process_ro(self, rINVITEorUPDATE))) {
        /* Send error */
        return ret;
    }

    // force SDP in 200 OK even if the request has the same SDP version
    force_sdp = TSIP_MESSAGE_HAS_CONTENT(rINVITEorUPDATE);

    // get new media_type after processing the remote offer
    new_media_type = self->msession_mgr ? self->msession_mgr->type : tmedia_none;

    /** response to bodiless iINVITE always contains SDP as explained below
    	RFC3261 - 14.1 UAC Behavior
    	   The same offer-answer model that applies to session descriptions in
    	   INVITEs (Section 13.2.1) applies to re-INVITEs.  As a result, a UAC
    	   that wants to add a media stream, for example, will create a new
    	   offer that contains this media stream, and send that in an INVITE
    	   request to its peer.  It is important to note that the full
    	   description of the session, not just the change, is sent.  This
    	   supports stateless session processing in various elements, and
    	   supports failover and recovery capabilities.  Of course, a UAC MAY
    	   send a re-INVITE with no session description, in which case the first
    	   reliable non-failure response to the re-INVITE will contain the offer
    	   (in this specification, that is a 2xx response).
    */
    bodiless_invite = !TSIP_MESSAGE_HAS_CONTENT(rINVITEorUPDATE) && TSIP_REQUEST_IS_INVITE(rINVITEorUPDATE);

    /* session timers (must be before sending response) */
    if(self->stimers.timer.timeout) {
        tsip_dialog_invite_stimers_handle(self, rINVITEorUPDATE);
    }

    /* hold/resume */
    tsip_dialog_invite_hold_handle(self, rINVITEorUPDATE);

    // send the response
    ret = send_RESPONSE(self, rINVITEorUPDATE, 200, "OK",
                        (self->msession_mgr && (force_sdp || bodiless_invite || self->msession_mgr->ro_changed || self->msession_mgr->state_changed || (old_media_type != new_media_type))));

    /* alert the user */
    TSIP_DIALOG_INVITE_SIGNAL(self, tsip_i_request,
                              tsip_event_code_dialog_request_incoming, "Incoming Request.", rINVITEorUPDATE);

    return ret;
}

/* Connected -> (send reINVITE) -> Connected */
static int x0000_Connected_2_Connected_X_oINVITE(va_list *app)
{
    int ret;
    tsk_bool_t mediaType_changed;
    tsip_dialog_invite_t *self;
    const tsip_action_t* action;

    self = va_arg(*app, tsip_dialog_invite_t *);
    va_arg(*app, const tsip_message_t *);
    action = va_arg(*app, const tsip_action_t *);

    /* Get Media type from the action */
    mediaType_changed = (TSIP_DIALOG_GET_SS(self)->media.type != action->media.type && action->media.type != tmedia_none);
    if (mediaType_changed) {
        if (self->msession_mgr) {
            ret = tmedia_session_mgr_set_media_type(self->msession_mgr, action->media.type);
        }
        self->cseq_out_media_update = TSIP_DIALOG(self)->cseq_value + 1;
    }

    /* Appy media params received from the user */
    if(!TSK_LIST_IS_EMPTY(action->media.params)) {
        ret = tmedia_session_mgr_set_3(self->msession_mgr, action->media.params);
    }

    /* send the request */
    ret = send_INVITE(self, mediaType_changed);

    /* alert the user */
    if(mediaType_changed) {
        TSIP_DIALOG_INVITE_SIGNAL(self, tsip_m_updating,
                                  tsip_event_code_dialog_request_outgoing, "Updating media type", self->last_oInvite);
    }

    return ret;
}

/* Any -> (iPRACK) -> Any */
int x0000_Any_2_Any_X_iPRACK(va_list *app)
{
    tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
    const tsip_request_t *rPRACK = va_arg(*app, const tsip_request_t *);

    const tsip_header_RAck_t* RAck;

    if((RAck = (const tsip_header_RAck_t*)tsip_message_get_header(rPRACK, tsip_htype_RAck))) {
        if((RAck->seq == self->rseq) &&
                (tsk_striequals(RAck->method, self->last_o1xxrel->CSeq->method)) &&
                (RAck->cseq == self->last_o1xxrel->CSeq->seq)) {

            ++self->rseq;
            return send_RESPONSE(self, rPRACK, 200, "OK", tsk_false);
        }
    }

    /* Send 488 */
    return send_ERROR(self, rPRACK, 488, "Failed to match PRACK request", "SIP; cause=488; text=\"Failed to match PRACK request\"");
}

/* Any -> (iOPTIONS) -> Any */
int x0000_Any_2_Any_X_iOPTIONS(va_list *app)
{
    tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
    const tsip_request_t *rOPTIONS = va_arg(*app, const tsip_request_t *);

    /* Alert user */

    /* Send 2xx */
    send_RESPONSE(self, rOPTIONS, 200, "OK", tsk_false);

    return 0;
}


/*	Any --> (i401/407 INVITE or UPDATE) --> Any */
int x0000_Any_2_Any_X_i401_407_Challenge(va_list *app)
{
    tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
    const tsip_response_t *response = va_arg(*app, const tsip_response_t *);
    int ret;

    if((ret = tsip_dialog_update(TSIP_DIALOG(self), response))) {
        /* Alert the user. */
        TSIP_DIALOG_INVITE_SIGNAL(self, tsip_ao_request,
                                  TSIP_RESPONSE_CODE(response), TSIP_RESPONSE_PHRASE(response), response);

        return ret;
    }

    if(TSIP_RESPONSE_IS_TO_INVITE(response) || TSIP_RESPONSE_IS_TO_UPDATE(response)) {
        return send_INVITEorUPDATE(self, TSIP_RESPONSE_IS_TO_INVITE(response), tsk_false);
    }
    else if(TSIP_RESPONSE_IS_TO_BYE(response)) {
        return send_BYE(self);
    }
    else {
        TSK_DEBUG_ERROR("Unexpected code called");
        return 0;
    }
}

/*	Any --> (i2xx INVITE or i2xx UPDATE) --> Any */
int x0000_Any_2_Any_X_i2xxINVITEorUPDATE(va_list *app)
{
    tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
    const tsip_response_t *r2xx = va_arg(*app, const tsip_response_t *);
    int ret = 0;

    /* Update the dialog state */
    if((ret = tsip_dialog_update(TSIP_DIALOG(self), r2xx))) {
        return ret;
    }

    /* session timers */
    if(self->stimers.timer.timeout) {
        tsip_dialog_invite_stimers_handle(self, r2xx);
    }

    /* Process remote offer */
    if((ret = tsip_dialog_invite_process_ro(self, r2xx))) {
        send_BYE(self);
        return ret;
    }

    /* send ACK */
    if(TSIP_RESPONSE_IS_TO_INVITE(r2xx)) {
        ret = send_ACK(self, r2xx);
    }

    // starts ICE timers now that both parties received the "candidates"
    if(tsip_dialog_invite_ice_is_enabled(self)) {
        tsip_dialog_invite_ice_timers_set(self, (self->required.ice ? -1 : TSIP_DIALOG_INVITE_ICE_CONNCHECK_TIMEOUT));
    }

    return ret;
}


/* Any -> (oBYE) -> Trying */
int x0000_Any_2_Trying_X_oBYE(va_list *app)
{
    tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
    int ret;

    /* Alert the user */
    TSIP_DIALOG_SIGNAL(self, tsip_event_code_dialog_terminating, "Terminating dialog");

    /* send BYE */
    if((ret = send_BYE(self)) == 0) {
#if !TSIP_UNDER_APPLE // FIXME: hangs up on iOS (RTP transport runnable join never exits)
        // stop session manager
        if(self->msession_mgr && self->msession_mgr->started) {
            tmedia_session_mgr_stop(self->msession_mgr);
        }
#endif
    }
    return ret;
}

/* Any -> (iBYE) -> Terminated */
int x0000_Any_2_Terminated_X_iBYE(va_list *app)
{
    tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
    const tsip_request_t *rBYE = va_arg(*app, const tsip_request_t *);

    /* set last error (or info) */
    tsip_dialog_set_lasterror(TSIP_DIALOG(self), "Call Terminated", tsip_event_code_dialog_terminated);

    /* send 200 OK */
    return send_RESPONSE(self, rBYE, 200, "OK", tsk_false);
}

/* Any -> Shutdown -> Terminated */
int x0000_Any_2_Trying_X_shutdown(va_list *app)
{
    tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);

    /* schedule shutdow timeout */
    TSIP_DIALOG_INVITE_TIMER_SCHEDULE(shutdown);

    /* alert user */
    TSIP_DIALOG_SIGNAL(self, tsip_event_code_dialog_terminating, "Terminating dialog");

    if(TSIP_DIALOG(self)->state == tsip_established) {
        return send_BYE(self);
    }
    else if(TSIP_DIALOG(self)->state == tsip_early) {
        return send_CANCEL(self);
    }

    return 0;
}


/* Any -> (i1xx) -> Any */
int x0000_Any_2_Any_X_i1xx(va_list *app)
{
    tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);
    const tsip_response_t *r1xx = va_arg(*app, const tsip_response_t *);
    int ret = 0;

    /* Update the dialog state */
    if((ret = tsip_dialog_update(TSIP_DIALOG(self), r1xx))) {
        return ret;
    }

    /* RFC 3262 - 4 UAC Behavior
    	If a provisional response is received for an initial request, and
    	that response contains a Require header field containing the option
    	tag 100rel, the response is to be sent reliably.  If the response is
    	a 100 (Trying) (as opposed to 101 to 199), this option tag MUST be
    	ignored, and the procedures below MUST NOT be used.

    	Assuming the response is to be transmitted reliably, the UAC MUST
    	create a new request with method PRACK.  This request is sent within
    	the dialog associated with the provisional response (indeed, the
    	provisional response may have created the dialog).  PRACK requests
    	MAY contain bodies, which are interpreted according to their type and
    	disposition.

    	Note that the PRACK is like any other non-INVITE request within a
    	dialog.  In particular, a UAC SHOULD NOT retransmit the PRACK request
    	when it receives a retransmission of the provisional response being
    	acknowledged, although doing so does not create a protocol error.

    	Additional information: We should only process the SDP from reliable responses (require:100rel)
    	but there was many problem with some clients sending SDP with this tag: tiscali, DTAG, samsung, ...
    */
    if((TSIP_RESPONSE_CODE(r1xx) >= 101 && TSIP_RESPONSE_CODE(r1xx) <=199)) {
        /* Process Remote offer */
        if(TSIP_MESSAGE_HAS_CONTENT(r1xx) && (ret = tsip_dialog_invite_process_ro(self, r1xx))) {
            /* Send Error */
            return ret;
        }
        // don't send PRACK if 100rel is only inside "supported" header
        if(tsip_message_required(r1xx, "100rel") && (ret = send_PRACK(self, r1xx))) {
            return ret;
        }
    }

    /* QoS Reservation */
    if((self->qos.timer.id == TSK_INVALID_TIMER_ID) && tsip_message_required(r1xx, "precondition") && !tmedia_session_mgr_canresume(self->msession_mgr)) {
        tsip_dialog_invite_qos_timer_schedule(self);
    }

    /* alert the user */
    ret = TSIP_DIALOG_INVITE_SIGNAL(self, tsip_ao_request,
                                    TSIP_RESPONSE_CODE(r1xx), TSIP_RESPONSE_PHRASE(r1xx), r1xx);
    if(self->is_transf) {
        ret = tsip_dialog_invite_notify_parent(self, r1xx);
    }

    return ret;
}

/* Any -> (oINFO) -> Any */
int x0000_Any_2_Any_X_oINFO(va_list *app)
{
    tsip_dialog_invite_t *self;
    const tsip_action_t* action;
    tsip_request_t* rINFO;

    self = va_arg(*app, tsip_dialog_invite_t *);
    va_arg(*app, const tsip_message_t *);
    action = va_arg(*app, const tsip_action_t *);

    if((rINFO = tsip_dialog_request_new(TSIP_DIALOG(self), "INFO"))) {
        int ret;
        if(action) {
            ret = tsip_dialog_apply_action(TSIP_MESSAGE(rINFO), action);
        }
        ret = tsip_dialog_request_send(TSIP_DIALOG(self), rINFO);
        TSK_OBJECT_SAFE_FREE(rINFO);
        return ret;
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new INFO request");
        return -1;
    }
}

int x0000_Any_2_Any_X_iINFO(va_list *app)
{
    tsip_dialog_invite_t * self = va_arg(*app, tsip_dialog_invite_t *);
    tsip_request_t* rINFO = (tsip_request_t*)va_arg(*app, const tsip_message_t *);
    int ret = -1;

    if (rINFO) {
        ret = send_RESPONSE(self, rINFO, 200, "Ok", tsk_false);
        {
            // int tmedia_session_mgr_recv_rtcp_event(tmedia_session_mgr_t* self, tmedia_type_t media_type, tmedia_rtcp_event_type_t event_type, uint32_t ssrc_media);
            if (self->msession_mgr && TSIP_MESSAGE_HAS_CONTENT(rINFO)) {
                if (tsk_striequals("application/media_control+xml", TSIP_MESSAGE_CONTENT_TYPE(rINFO))) { /* rfc5168: XML Schema for Media Control */
                    static uint32_t __ssrc_media_fake = 0;
                    static tmedia_type_t __tmedia_type_video = tmedia_video; // TODO: add bfcpvideo?
                    const char* content_ptr = (const char*)TSIP_MESSAGE_CONTENT_DATA(rINFO);
                    tsk_size_t content_size = (tsk_size_t)TSIP_MESSAGE_CONTENT_DATA_LENGTH(rINFO);
                    tsk_bool_t is_fir = tsk_false;
                    uint64_t sessionId = 0;
#if HAVE_LIBXML2
                    {
                        xmlDoc *pDoc;
                        xmlNode *pRootElement;
                        xmlXPathContext *pPathCtx;
                        xmlXPathObject *pPathObj;
                        static const xmlChar* __xpath_expr_picture_fast_update = (const xmlChar*)"/media_control/vc_primitive/to_encoder/picture_fast_update";
                        static const xmlChar* __xpath_expr_stream_id = (const xmlChar*)"/media_control/vc_primitive/stream_id";

                        if (!(pDoc = xmlParseDoc(content_ptr))) {
                            TSK_DEBUG_ERROR("Failed to parse XML content [%s]", content_ptr);
                            return 0;
                        }
                        if (!(pRootElement = xmlDocGetRootElement(pDoc))) {
                            TSK_DEBUG_ERROR("Failed to get root element from XML content [%s]", content_ptr);
                            xmlFreeDoc(pDoc);
                            return 0;
                        }
                        if (!(pPathCtx = xmlXPathNewContext(pDoc))) {
                            TSK_DEBUG_ERROR("Failed to create path context from XML content [%s]", content_ptr);
                            xmlFreeDoc(pDoc);
                            return 0;
                        }
                        // picture_fast_update
                        if (!(pPathObj = xmlXPathEvalExpression(__xpath_expr_picture_fast_update, pPathCtx))) {
                            TSK_DEBUG_ERROR("Error: unable to evaluate xpath expression: %s", __xpath_expr_picture_fast_update);
                            xmlXPathFreeContext(pPathCtx);
                            xmlFreeDoc(pDoc);
                            return 0;
                        }
                        is_fir = (pPathObj->type == XPATH_NODESET && pPathObj->nodesetval->nodeNr > 0);
                        xmlXPathFreeObject(pPathObj);
                        // stream_id
                        if (!(pPathObj = xmlXPathEvalExpression(__xpath_expr_stream_id, pPathCtx))) {
                            TSK_DEBUG_ERROR("Error: unable to evaluate xpath expression: %s", __xpath_expr_stream_id);
                            xmlXPathFreeContext(pPathCtx);
                            xmlFreeDoc(pDoc);
                        }
                        else if (pPathObj->type == XPATH_NODESET && pPathObj->nodesetval->nodeNr > 0 && pPathObj->nodesetval->nodeTab[0]->children && pPathObj->nodesetval->nodeTab[0]->children->content) {
                            sessionId = tsk_atoi64((const char*)pPathObj->nodesetval->nodeTab[0]->children->content);
                        }
                        xmlXPathFreeObject(pPathObj);

                        xmlXPathFreeContext(pPathCtx);
                        xmlFreeDoc(pDoc);
                    }
#else
                    is_fir = (tsk_strcontains(content_ptr, content_size, "to_encoder") && tsk_strcontains(content_ptr, content_size, "picture_fast_update"));
#endif
                    if (is_fir) {
                        TSK_DEBUG_INFO("Incoming SIP INFO(picture_fast_update)");
                        ret = sessionId
                              ? tmedia_session_mgr_recv_rtcp_event_2(self->msession_mgr, tmedia_rtcp_event_type_fir, sessionId)
                              : tmedia_session_mgr_recv_rtcp_event(self->msession_mgr, __tmedia_type_video, tmedia_rtcp_event_type_fir, __ssrc_media_fake);
                    }
                    else {
                        TSK_DEBUG_INFO("Incoming SIP INFO(unknown)");
                    }
                }
            }
        }
        TSIP_DIALOG_INVITE_SIGNAL(self, tsip_i_request,
                                  tsip_event_code_dialog_request_incoming, "Incoming Request", rINFO);
    }

    return ret;
}

int x9998_Any_2_Terminated_X_transportError(va_list *app)
{
    tsip_dialog_invite_t *self = va_arg(*app, tsip_dialog_invite_t *);

    /* set last error (or info) */
    tsip_dialog_set_lasterror(TSIP_DIALOG(self), "Transport error", tsip_event_code_dialog_terminated);

    return 0;
}

int x9999_Any_2_Any_X_Error(va_list *app)
{
    return 0;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//				== STATE MACHINE END ==
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int tsip_dialog_invite_msession_configure(tsip_dialog_invite_t *self)
{
    tmedia_srtp_mode_t srtp_mode;
    tmedia_mode_t avpf_mode;
    tsk_bool_t is_rtcweb_enabled;
    tsk_bool_t is_webrtc2sip_mode_enabled;

    if(!self || !self->msession_mgr) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    is_webrtc2sip_mode_enabled = (TSIP_DIALOG_GET_STACK(self)->network.mode == tsip_stack_mode_webrtc2sip);
    is_rtcweb_enabled = (((tsip_ssession_t*)TSIP_DIALOG(self)->ss)->media.profile == tmedia_profile_rtcweb);
    srtp_mode = is_rtcweb_enabled ? tmedia_srtp_mode_mandatory : ((tsip_ssession_t*)TSIP_DIALOG(self)->ss)->media.srtp_mode;
    avpf_mode = is_rtcweb_enabled ? tmedia_mode_mandatory : ((tsip_ssession_t*)TSIP_DIALOG(self)->ss)->media.avpf_mode;

    // set callback functions
    tmedia_session_mgr_set_onerror_cbfn(self->msession_mgr, self, tsip_dialog_invite_msession_onerror_cb);
    tmedia_session_mgr_set_rfc5168_cbfn(self->msession_mgr, self, tsip_dialog_invite_msession_rfc5168_cb);

    // set params
    return tmedia_session_mgr_set(self->msession_mgr,
                                  TMEDIA_SESSION_SET_INT32(self->msession_mgr->type, "srtp-mode", srtp_mode),
                                  TMEDIA_SESSION_SET_INT32(self->msession_mgr->type, "avpf-mode", avpf_mode),
                                  TMEDIA_SESSION_SET_INT32(self->msession_mgr->type, "webrtc2sip-mode-enabled", is_webrtc2sip_mode_enabled), // hack the media stack
                                  TMEDIA_SESSION_SET_INT32(self->msession_mgr->type, "rtcp-enabled", self->use_rtcp),
                                  TMEDIA_SESSION_SET_INT32(self->msession_mgr->type, "rtcpmux-enabled", self->use_rtcpmux),
                                  TMEDIA_SESSION_SET_INT32(self->msession_mgr->type, "codecs-supported", ((tsip_ssession_t*)TSIP_DIALOG(self)->ss)->media.codecs),

                                  TMEDIA_SESSION_SET_INT32(self->msession_mgr->type, "bypass-encoding", ((tsip_ssession_t*)TSIP_DIALOG(self)->ss)->media.bypass_encoding),
                                  TMEDIA_SESSION_SET_INT32(self->msession_mgr->type, "bypass-decoding", ((tsip_ssession_t*)TSIP_DIALOG(self)->ss)->media.bypass_decoding),

                                  TMEDIA_SESSION_SET_INT32(tmedia_audio, "rtp-ssrc", ((tsip_ssession_t*)TSIP_DIALOG(self)->ss)->media.rtp.ssrc.audio),
                                  TMEDIA_SESSION_SET_INT32(tmedia_video, "rtp-ssrc", ((tsip_ssession_t*)TSIP_DIALOG(self)->ss)->media.rtp.ssrc.video),

                                  TMEDIA_SESSION_SET_STR(self->msession_mgr->type, "dtls-file-ca", TSIP_DIALOG_GET_STACK(self)->security.tls.ca),
                                  TMEDIA_SESSION_SET_STR(self->msession_mgr->type, "dtls-file-pbk", TSIP_DIALOG_GET_STACK(self)->security.tls.pbk),
                                  TMEDIA_SESSION_SET_STR(self->msession_mgr->type, "dtls-file-pvk", TSIP_DIALOG_GET_STACK(self)->security.tls.pvk),
                                  TMEDIA_SESSION_SET_INT32(self->msession_mgr->type, "dtls-cert-verify", TSIP_DIALOG_GET_STACK(self)->security.tls.verify),

                                  TMEDIA_SESSION_SET_INT32(tmedia_video, "fps", ((tsip_ssession_t*)TSIP_DIALOG(self)->ss)->media.video_fps),
                                  TMEDIA_SESSION_SET_INT32(tmedia_video, "bandwidth-max-upload", ((tsip_ssession_t*)TSIP_DIALOG(self)->ss)->media.video_bw_up),
                                  TMEDIA_SESSION_SET_INT32(tmedia_video, "bandwidth-max-download", ((tsip_ssession_t*)TSIP_DIALOG(self)->ss)->media.video_bw_down),
                                  TMEDIA_SESSION_SET_INT32(tmedia_video, "pref-size", ((tsip_ssession_t*)TSIP_DIALOG(self)->ss)->media.video_pref_size),

                                  tsk_null);
}

int tsip_dialog_invite_msession_start(tsip_dialog_invite_t *self)
{
    if(!self || !self->msession_mgr) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(tsip_dialog_invite_ice_is_enabled(self) && !tsip_dialog_invite_ice_is_connected(self)) {
        if(self->msession_mgr->type != self->ice.media_type) {
            TSK_DEBUG_INFO("Media session type(%d)<>ICE media type(%d)", self->msession_mgr->type, self->ice.media_type);
            // make sure to use the right media types
            tsip_dialog_invite_ice_set_media_type(self, self->msession_mgr->type);
        }
        self->ice.start_smgr = tsk_true;
    }
    else {
        self->ice.start_smgr = tsk_false;
        return tmedia_session_mgr_start(self->msession_mgr);
    }
    return 0;
}

// send INVITE/UPDATE request
int send_INVITEorUPDATE(tsip_dialog_invite_t *self, tsk_bool_t is_INVITE, tsk_bool_t force_sdp)
{
    int ret = -1;
    tsip_request_t *request = tsk_null;
    tsk_bool_t bodiless = tsk_false;

#if BODILESS_INVITE
    if(TSIP_DIALOG(self)->state == tsip_initial) {
        bodiless = tsk_true;
    }
#endif

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        goto bail;
    }

    if((request = tsip_dialog_request_new(TSIP_DIALOG(self), is_INVITE ? "INVITE" : "UPDATE"))) {
        /* apply action params to the request (will add a content if the action contains one) */
        if(TSIP_DIALOG(self)->curr_action) {
            tsip_dialog_apply_action(request, TSIP_DIALOG(self)->curr_action);
        }

        if(!bodiless) {
            /* add our payload if current action does not have one */
            if((force_sdp || is_INVITE) || ((self->msession_mgr && self->msession_mgr->state_changed) || (TSIP_DIALOG(self)->state == tsip_initial))) {
                if(!TSIP_DIALOG(self)->curr_action || !TSIP_DIALOG(self)->curr_action->payload) {
                    const tsdp_message_t* sdp_lo;
                    char* sdp;
                    if((sdp_lo = tmedia_session_mgr_get_lo(self->msession_mgr)) && (sdp = tsdp_message_tostring(sdp_lo))) {
                        tsip_message_add_content(request, "application/sdp", sdp, tsk_strlen(sdp));
                        if(tsip_dialog_invite_ice_is_enabled(self)) {
                            ret = tsip_dialog_invite_ice_process_lo(self, sdp_lo);
                        }
                        TSK_FREE(sdp);
                    }
                }
            }
        }

        /* Session timers */
        if(self->stimers.timer.timeout) {
            if(self->required.timer) {
                tsip_message_add_headers(request,
                                         TSIP_HEADER_SESSION_EXPIRES_VA_ARGS(self->stimers.timer.timeout, !self->stimers.is_refresher),
                                         TSIP_HEADER_REQUIRE_VA_ARGS("timer"),
                                         tsk_null
                                        );
            }
            else if(self->supported.timer) {
                tsip_message_add_headers(request,
                                         TSIP_HEADER_SESSION_EXPIRES_VA_ARGS(self->stimers.timer.timeout, !self->stimers.is_refresher),
                                         TSIP_HEADER_SUPPORTED_VA_ARGS("timer"),
                                         tsk_null
                                        );
            }

        }

        if(self->stimers.minse) {
            tsip_message_add_headers(request,
                                     TSIP_HEADER_MIN_SE_VA_ARGS(self->stimers.minse),
                                     tsk_null
                                    );
        }

        /* 100rel */
        if(self->required._100rel) {
            tsip_message_add_headers(request,
                                     TSIP_HEADER_REQUIRE_VA_ARGS("100rel"),
                                     tsk_null
                                    );
        }
        else if(self->supported._100rel) {
            tsip_message_add_headers(request,
                                     TSIP_HEADER_SUPPORTED_VA_ARGS("100rel"),
                                     tsk_null
                                    );
        }

        /* QoS */
        if(self->required.precondition) {
            tsip_message_add_headers(request,
                                     TSIP_HEADER_REQUIRE_VA_ARGS("precondition"),
                                     tsk_null
                                    );
        }
        else if(self->supported.precondition) {
            tsip_message_add_headers(request,
                                     TSIP_HEADER_SUPPORTED_VA_ARGS("precondition"),
                                     tsk_null
                                    );
        }


        /* Always added headers */
        // Explicit Communication Transfer (3GPP TS 24.629)
        /*tsip_message_add_headers(request,
        			TSIP_HEADER_SUPPORTED_VA_ARGS("norefersub,replaces"),
        			tsk_null
        		);*/

        /* send the request */
        ret = tsip_dialog_request_send(TSIP_DIALOG(self), request);
        if(ret == 0) {
            /* update last INVITE */
            TSK_OBJECT_SAFE_FREE(self->last_oInvite);
            self->last_oInvite = request;
        }
        else {
            TSK_OBJECT_SAFE_FREE(request);
        }
    }

bail:
    return ret;
}

// Send PRACK
int send_PRACK(tsip_dialog_invite_t *self, const tsip_response_t* r1xx)
{
    // "Require: 100rel\r\n" should be checked by the caller of this function
    int ret = -1;
    tsip_request_t *request = tsk_null;
    const tsip_header_RSeq_t* RSeq;

    if(!self || !r1xx || !r1xx->CSeq) {
        TSK_DEBUG_ERROR("Invalid parameter");
        goto bail;
    }


    /*	RFC 3262 - 4 UAC Behavior
    The UAC MUST maintain a sequence number that indicates the most recently
    received in-order reliable provisional response for the initial request.
    */
    if((RSeq = (const tsip_header_RSeq_t*)tsip_message_get_header(r1xx, tsip_htype_RSeq))) {

        /*	RFC 3262 - 4 UAC Behavior
        If the UAC receives another reliable provisional
        response to the same request, and its RSeq value is not one higher
        than the value of the sequence number, that response MUST NOT be
        acknowledged with a PRACK, and MUST NOT be processed further by the
        UAC.  An implementation MAY discard the response, or MAY cache the
        response in the hopes of receiving the missing responses.
        */
        if(self->rseq && (RSeq->seq <= self->rseq)) {
            TSK_DEBUG_WARN("1xx.RSeq value is not one higher than lastINVITE.RSeq.");
            ret = 0; /* Not error */
            goto bail;
        }
        self->rseq = RSeq->seq;
    }

    /* RFC 3262 - 4 UAC Behavior
    Assuming the response is to be transmitted reliably, the UAC MUST
    create a new request with method PRACK.
    */
    if(!(request = tsip_dialog_request_new(TSIP_DIALOG(self), "PRACK"))) {
        goto bail;
    }

    /* RFC 3262 - 7.2 RAck
    	The first number is the value from the RSeq header in the provisional
    	response that is being acknowledged.  The next number, and the
    	method, are copied from the CSeq in the response that is being
    	acknowledged.  The method name in the RAck header is case sensitive.
    */
    TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_RACK_VA_ARGS(self->rseq, r1xx->CSeq->seq, r1xx->CSeq->method));
    //TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_DUMMY_VA_ARGS("Test", "value"));

    /*	Initial INVITE was a bodiless request and 100rel is supported (I'm Alice)
    	1. Alice sends an initial INVITE without offer
    	2. Bob's answer is sent in the first reliable provisional response, in this case it's a 1xx INVITE response
    	3. Alice's answer is sent in the PRACK response
    */
    if(self->is_client && (self->last_oInvite && !TSIP_MESSAGE_HAS_CONTENT(self->last_oInvite))) {
        const tsdp_message_t* sdp_lo;
        char* sdp;
        if((sdp_lo = tmedia_session_mgr_get_lo(self->msession_mgr)) && (sdp = tsdp_message_tostring(sdp_lo))) {
            tsip_message_add_content(request, "application/sdp", sdp, tsk_strlen(sdp));
            TSK_FREE(sdp);
        }
    }

    // Send request
    ret = tsip_dialog_request_send(TSIP_DIALOG(self), request);

bail:
    TSK_OBJECT_SAFE_FREE(request);
    return ret;
}

// Send CANCEL
int send_CANCEL(tsip_dialog_invite_t *self)
{
    int ret = -1;
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        goto bail;
    }
    /* RFC 3261 - 9 Canceling a Request
    	If the request being cancelled contains a Route header field, the
    	CANCEL request MUST include that Route header field's values.
    	==> up to tsip_dialog_request_new()
    */

    /*	RFC 3261 - 9 Canceling a Request
    	Once the CANCEL is constructed, the client SHOULD check whether it
    	has received any response (provisional or final) for the request
    	being cancelled (herein referred to as the "original request").

    	If no provisional response has been received, the CANCEL request MUST
    	NOT be sent; rather, the client MUST wait for the arrival of a
    	provisional response before sending the request.
    	==> up to the caller to check that we are not in the initial state and the FSM
    	is in Trying state.
    */

    /*	RFC 3261 - 9 Canceling a Request
    	The following procedures are used to construct a CANCEL request.  The
    	Request-URI, Call-ID, To, the numeric part of CSeq, and From header
    	fields in the CANCEL request MUST be identical to those in the
    	request being cancelled, including tags.  A CANCEL constructed by a
    	client MUST have only a single Via header field value matching the
    	top Via value in the request being cancelled.  Using the same values
    	for these header fields allows the CANCEL to be matched with the
    	request it cancels (Section 9.2 indicates how such matching occurs).
    	However, the method part of the CSeq header field MUST have a value
    	of CANCEL.  This allows it to be identified and processed as a
    	transaction in its own right (See Section 17)
    */
    if(self->last_oInvite) {
        /* to avoid concurrent access, take a reference to the request */
        tsip_request_t* last_oInvite = tsk_object_ref(self->last_oInvite);
        tsip_request_t* cancel;

        if((cancel = tsip_request_create("CANCEL", last_oInvite->line.request.uri))) {
            const tsk_list_item_t* item;
            const tsip_header_t* header;

            tsip_message_add_headers(cancel,
                                     TSIP_HEADER_CSEQ_VA_ARGS(last_oInvite->CSeq->seq, "CANCEL"),
                                     TSIP_HEADER_MAX_FORWARDS_VA_ARGS(TSIP_HEADER_MAX_FORWARDS_DEFAULT),
                                     TSIP_HEADER_CONTENT_LENGTH_VA_ARGS(0),
                                     tsk_null);

            cancel->Call_ID = tsk_object_ref(last_oInvite->Call_ID);
            cancel->To = tsk_object_ref(last_oInvite->To);
            cancel->From = tsk_object_ref(last_oInvite->From);
            cancel->firstVia = tsk_object_ref(last_oInvite->firstVia);
            cancel->sigcomp_id = tsk_strdup(TSIP_DIALOG_GET_SS(self)->sigcomp_id);

            // Copy Authorizations, Routes and Proxy-Auth
            tsk_list_foreach(item, last_oInvite->headers) {
                if(!(header = TSIP_HEADER(item->data))) {
                    continue;
                }

                switch(header->type) {
                case tsip_htype_Route:
                case tsip_htype_Proxy_Authorization:
                case tsip_htype_Authorization:
                    header = tsk_object_ref((void*)header);
                    if(!cancel->headers) {
                        cancel->headers = tsk_list_create();
                    }
                    tsk_list_push_back_data(cancel->headers, (void**)&header);
                    break;
                default:
                    break;
                }
            }

            ret = tsip_dialog_add_session_headers(TSIP_DIALOG(self), cancel);
            ret = tsip_dialog_request_send(TSIP_DIALOG(self), cancel);
            TSK_OBJECT_SAFE_FREE(cancel);
        }
        else {
            TSK_DEBUG_ERROR("Failed to create CANCEL request");
            ret = -2;
        }

        TSK_OBJECT_SAFE_FREE(last_oInvite);
        return ret;
    }
    else {
        TSK_DEBUG_WARN("There is no INVITE request to cancel");
        return 0;
    }

bail:
    return ret;
}

int tsip_dialog_invite_notify_parent(tsip_dialog_invite_t *self, const tsip_response_t* response)
{
    int ret = -1;
    tsip_dialog_t* dlg_parent = tsip_dialog_layer_find_by_ssid(TSIP_DIALOG_GET_STACK(self)->layer_dialog, TSIP_DIALOG_GET_SS(self)->id_parent);
    if(dlg_parent) {
        tsip_action_t* action = tsip_action_create(tsip_atype_ect_lnotify,
                                TSIP_ACTION_SET_NULL());
        if(action) {
            ret = tsip_dialog_fsm_act(dlg_parent, action->type, response, action);
            TSK_OBJECT_SAFE_FREE(action);
        }
        else {
            TSK_DEBUG_ERROR("Failed to create action object");
        }
        TSK_OBJECT_SAFE_FREE(dlg_parent);
    }
    else {
        TSK_DEBUG_ERROR("Failed to find parent with id = %llu", TSIP_DIALOG_GET_SS(self)->id_parent);
    }
    return ret;
}

// Send BYE
int send_BYE(tsip_dialog_invite_t *self)
{
    int ret = -1;
    tsip_request_t *bye = tsk_null;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        goto bail;
    }
    /* RFC 3261 - 15.1.1 UAC Behavior
    	A BYE request is constructed as would any other request within a
    	dialog, as described in Section 12.

    	Once the BYE is constructed, the UAC core creates a new non-INVITE
    	client transaction, and passes it the BYE request.  The UAC MUST
    	consider the session terminated (and therefore stop sending or
    	listening for media) as soon as the BYE request is passed to the
    	client transaction.  If the response for the BYE is a 481
    	(Call/Transaction Does Not Exist) or a 408 (Request Timeout) or no

    	response at all is received for the BYE (that is, a timeout is
    	returned by the client transaction), the UAC MUST consider the
    	session and the dialog terminated.
    */
    if((bye = tsip_dialog_request_new(TSIP_DIALOG(self), "BYE"))) {
        if(TSIP_DIALOG(self)->curr_action) {
            tsip_dialog_apply_action(bye, TSIP_DIALOG(self)->curr_action);
        }
        ret = tsip_dialog_request_send(TSIP_DIALOG(self), bye);
        TSK_OBJECT_SAFE_FREE(bye);
    }

bail:
    return ret;
}

// Send INFO
int send_INFO(tsip_dialog_invite_t *self, const char* content_type, const void* content_ptr, tsk_size_t content_size)
{
    int ret = -1;
    tsip_request_t *info = tsk_null;

    if (!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        goto bail;
    }
    if ((info = tsip_dialog_request_new(TSIP_DIALOG(self), "INFO"))) {
        if (TSIP_DIALOG(self)->curr_action) {
            ret = tsip_dialog_apply_action(info, TSIP_DIALOG(self)->curr_action);
            if (ret) {
                goto bail;
            }
        }
        if (content_type && content_ptr && content_size) {
            ret = tsip_message_add_content(info, content_type, content_ptr, content_size);
            if (ret) {
                goto bail;
            }
        }
        ret = tsip_dialog_request_send(TSIP_DIALOG(self), info);
        if (ret) {
            goto bail;
        }
    }

bail:
    TSK_OBJECT_SAFE_FREE(info);
    return ret;
}

// Send ACK
int send_ACK(tsip_dialog_invite_t *self, const tsip_response_t* r2xxINVITE)
{
    int ret = -1;
    tsip_request_t *request = tsk_null;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        goto bail;
    }

    if((request = tsip_dialog_request_new(TSIP_DIALOG(self), "ACK"))) {

        /* The initial INVITE sent by us was a bodiless request and we don't support 100rel (We are Alice)
           1. Alice sends initial INVITE without offer
           2. Bob's offer is sent in the 2xx INVITE response
           3. Alice's answer is sent in the ACK request
        */
        if(self->is_client && (self->last_oInvite && !TSIP_MESSAGE_HAS_CONTENT(self->last_oInvite))) {
            const tsdp_message_t* sdp_lo;
            char* sdp;
            if((sdp_lo = tmedia_session_mgr_get_lo(self->msession_mgr)) && (sdp = tsdp_message_tostring(sdp_lo))) {
                tsip_message_add_content(request, "application/sdp", sdp, tsk_strlen(sdp));
                TSK_FREE(sdp);
            }

            // Start media session if not done
            if(!self->msession_mgr->started && (self->msession_mgr->sdp.lo && self->msession_mgr->sdp.ro)) {
                /* Set MSRP Callback */
                if((self->msession_mgr->type & tmedia_msrp) == tmedia_msrp) {
                    tmedia_session_mgr_set_msrp_cb(self->msession_mgr, TSIP_DIALOG_GET_SS(self)->userdata, TSIP_DIALOG_GET_SS(self)->media.msrp.callback);
                }
                // starts session manager
                ret = tsip_dialog_invite_msession_start(self);
            }
        }

        /*	RFC 3261 - 13.2.2.4 2xx Responses
           The UAC core MUST generate an ACK request for each 2xx received from
           the transaction layer.  The header fields of the ACK are constructed
           in the same way as for any request sent within a dialog (see Section
           12) with the exception of the CSeq and the header fields related to
           authentication.  The sequence number of the CSeq header field MUST be
           the same as the INVITE being acknowledged, but the CSeq method MUST
           be ACK.  The ACK MUST contain the same credentials as the INVITE.  If
           the 2xx contains an offer (based on the rules above), the ACK MUST
           carry an answer in its body.  If the offer in the 2xx response is not
           acceptable, the UAC core MUST generate a valid answer in the ACK and
           then send a BYE immediately.
           ==> Credentials will be added by tsip_dialog_request_new() because they are
           associated to the dialog itself.
           ==> It's up to us to add/update the CSeq number.
           ==> ACK requests sent here will create new client transactions, which means that
           they will have there own branches. This is not the case for ACK requests sent from
           the transaction layer.
        */
        request->CSeq->seq = r2xxINVITE->CSeq->seq; /* As the 2xx has the same CSeq than the INVITE */

        /*	RFC 3261 - 13.2.2.4 2xx Responses
           Once the ACK has been constructed, the procedures of [4] are used to
           determine the destination address, port and transport.  However, the
           request is passed to the transport layer directly for transmission,
           rather than a client transaction.  This is because the UAC core
           handles retransmissions of the ACK, not the transaction layer.  The
           ACK MUST be passed to the client transport every time a
           retransmission of the 2xx final response that triggered the ACK
           arrives.
        */
        if(TSIP_DIALOG_GET_STACK(self)->layer_transport) {
            ret = tsip_transport_layer_send(TSIP_DIALOG_GET_STACK(self)->layer_transport, tsk_null, request);
        }
        else {
            ret = -1;
            TSK_DEBUG_ERROR("Not Transport layer associated to this stack");
        }
        TSK_OBJECT_SAFE_FREE(request);
    }

bail:
    return ret;
}

// Send any response
int send_RESPONSE(tsip_dialog_invite_t *self, const tsip_request_t* request, short code, const char* phrase, tsk_bool_t force_sdp)
{
    tsip_response_t *response;
    int ret = -1;

    if((response = tsip_dialog_response_new(TSIP_DIALOG(self), code, phrase, request))) {
        if(TSIP_REQUEST_IS_INVITE(request) || TSIP_REQUEST_IS_UPDATE(request)) {
            /* Session timers (for 2xx to INVITE or UPDATE) */
            if(self->required.timer) {
                tsip_message_add_headers(response,
                                         TSIP_HEADER_REQUIRE_VA_ARGS("timer"),
                                         TSIP_HEADER_SESSION_EXPIRES_VA_ARGS(self->stimers.timer.timeout, tsk_striequals(self->stimers.refresher, "uas")),
                                         tsk_null
                                        );
            }
            else if(self->supported.timer) {
                tsip_message_add_headers(response,
                                         TSIP_HEADER_SUPPORTED_VA_ARGS("timer"),
                                         TSIP_HEADER_SESSION_EXPIRES_VA_ARGS(self->stimers.timer.timeout, tsk_striequals(self->stimers.refresher, "uas")),
                                         tsk_null
                                        );
            }
            if(self->stimers.minse) {
                tsip_message_add_headers(response,
                                         TSIP_HEADER_MIN_SE_VA_ARGS(self->stimers.minse),
                                         tsk_null
                                        );
            }
            if(code == 422) {
                tsip_message_add_headers(response,
                                         TSIP_HEADER_DUMMY_VA_ARGS("Reason", "SIP; cause=422; text=\"Session Interval Too Small\""),
                                         tsk_null
                                        );
            }

            /* 180 Ringing */
            /* 183 Session in Progress */
            if(code == 180 || code == 183) {
                if(self->required._100rel) {
                    if(self->rseq == 0) {
                        self->rseq = TSK_ABS((rand() ^ rand()) + 1);
                    }
                    tsip_message_add_headers(response,
                                             TSIP_HEADER_REQUIRE_VA_ARGS("100rel"),
                                             TSIP_HEADER_RSEQ_VA_ARGS(self->rseq),
                                             tsk_null
                                            );
                    TSK_OBJECT_SAFE_FREE(self->last_o1xxrel);
                    self->last_o1xxrel = tsk_object_ref(response);

                    /* No-Initial reliable 1xx will use tsip_dialog_response_send() instead of this function
                    * ==> can reseset timeout value and make initial schedule */
                    TSIP_DIALOG_TIMER_CANCEL(100rel);
                    self->timer100rel.timeout = tsip_timers_getA();
                    TSIP_DIALOG_INVITE_TIMER_SCHEDULE(100rel);
                }
            }

            /* Precondition */
            if(code == 180 || code == 183) {
                if(self->required.precondition) {
                    tsip_message_add_headers(response,
                                             TSIP_HEADER_REQUIRE_VA_ARGS("precondition"),
                                             tsk_null
                                            );
                }
            }


            /* SDP content */
            if(self->msession_mgr && force_sdp) {
                const tsdp_message_t* sdp_lo;
                char* sdp = tsk_null;
                if((sdp_lo = tmedia_session_mgr_get_lo(self->msession_mgr)) && (sdp = tsdp_message_tostring(sdp_lo))) {
                    ret = tsip_message_add_content(response, "application/sdp", sdp, tsk_strlen(sdp));
                    if(tsip_dialog_invite_ice_is_enabled(self)) {
                        ret = tsip_dialog_invite_ice_process_lo(self, sdp_lo);
                    }
                }
                TSK_FREE(sdp);
            }

            /* Add Allow header */
            tsip_message_add_headers(response,
                                     TSIP_HEADER_DUMMY_VA_ARGS("Allow", TSIP_HEADER_ALLOW_DEFAULT),
                                     tsk_null
                                    );
        }
        else if(TSIP_REQUEST_IS_REFER(request)) {
            if(self->required.norefersub) {
                tsip_message_add_headers(response,
                                         TSIP_HEADER_REQUIRE_VA_ARGS("norefersub"),
                                         tsk_null
                                        );
            }
            if(self->supported.norefersub) {
                tsip_message_add_headers(response,
                                         TSIP_HEADER_SUPPORTED_VA_ARGS("norefersub"),
                                         tsk_null
                                        );
            }
        }


        ret = tsip_dialog_response_send(TSIP_DIALOG(self), response);
        TSK_OBJECT_SAFE_FREE(response);
    }
    return ret;
}

// Send error response
int send_ERROR(tsip_dialog_invite_t* self, const tsip_request_t* request, short code, const char* phrase, const char* reason)
{
    tsip_response_t *response;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if((response = tsip_dialog_response_new(TSIP_DIALOG(self), code, phrase, request))) {
        tsip_message_add_headers(response,
                                 TSIP_HEADER_DUMMY_VA_ARGS("Reason", reason),
                                 tsk_null
                                );

        tsip_dialog_response_send(TSIP_DIALOG(self), response);
        TSK_OBJECT_SAFE_FREE(response);
    }
    else {
        TSK_DEBUG_ERROR("Failed to create new message");
    }
    return 0;
}

// FSM callback to signal that the dialog is in the terminated state
int tsip_dialog_invite_OnTerminated(tsip_dialog_invite_t *self)
{
    TSK_DEBUG_INFO("=== INVITE Dialog terminated ===");

    /* Cancel all transactions associated to this dialog (will also be done when the dialog is destroyed )
    	worth nothing to do it here in order to cancel in-dialog request (such as INFO, REFER...)
    */
    tsip_transac_layer_cancel_by_dialog(TSIP_DIALOG_GET_STACK(self)->layer_transac, TSIP_DIALOG(self));

    /* stop session manager */
    if(self->msession_mgr && self->msession_mgr->started) {
        tmedia_session_mgr_stop(self->msession_mgr);
    }
    // because of C# and Java garbage collectors, the ICE context could
    // be destroyed (then stoppped) very late
    if(self->ice.ctx_audio) {
        tnet_ice_ctx_stop(self->ice.ctx_audio);
    }
    if(self->ice.ctx_video) {
        tnet_ice_ctx_stop(self->ice.ctx_video);
    }

    /* alert the user */
    TSIP_DIALOG_SIGNAL_2(self, tsip_event_code_dialog_terminated,
                         TSIP_DIALOG(self)->last_error.phrase ? TSIP_DIALOG(self)->last_error.phrase : "Call Terminated",
                         TSIP_DIALOG(self)->last_error.message);

    /* Remove from the dialog layer. */
    return tsip_dialog_remove(TSIP_DIALOG(self));
}

// callback function called when media session error occures asynchronously
static int tsip_dialog_invite_msession_onerror_cb(const void* usrdata, const struct tmedia_session_s* session, const char* reason, tsk_bool_t is_fatal)
{
    tsip_dialog_t *self = (tsip_dialog_t*)usrdata;

    if(self && is_fatal) {
        char* str = tsk_null;
        tsip_action_t* action;
        tsk_sprintf(&str, "SIP; cause=488; text=\"%s\"", (reason ? reason : "Internal error"));
        action = tsip_action_create(tsip_atype_hangup,
                                    TSIP_ACTION_SET_HEADER("Reason", str),
                                    TSIP_ACTION_SET_NULL());
        TSK_FREE(str);

        tsip_dialog_hangup(self, action);
        TSK_OBJECT_SAFE_FREE(action);
    }

    return 0;
}

// callback function called when media session events (related to rfc5168) occures asynchronously
static int tsip_dialog_invite_msession_rfc5168_cb(const void* usrdata, const struct tmedia_session_s* session, const char* reason, enum tmedia_session_rfc5168_cmd_e command)
{
    tsip_dialog_invite_t *self = (tsip_dialog_invite_t*)usrdata;

    if (self) {
        if (command == tmedia_session_rfc5168_cmd_picture_fast_update) {
            uint64_t now = tsk_time_now();
            if ((now - self->last_out_fastupdate_time) > TSIP_INFO_FASTUPDATE_OUT_INTERVAL_MIN) {
                char* content_ptr = tsk_null;
                static const char* __content_type = "application/media_control+xml";
                static const void* __content_format =
                    "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
                    " <media_control>\r\n"
                    "   <vc_primitive>\r\n"
                    "     <to_encoder>\r\n"
                    "       <picture_fast_update>\r\n"
                    "       </picture_fast_update>\r\n"
                    "     </to_encoder>\r\n"
                    "     <stream_id>%llu</stream_id>\r\n"
                    "   </vc_primitive>\r\n"
                    " </media_control>\r\n";
                TSK_DEBUG_INFO("Media session is asking the sigaling layer to send SIP INFO('picture_fast_update')");
                tsk_sprintf(&content_ptr, __content_format, session->id);
                self->last_out_fastupdate_time = now;
                return send_INFO(self, __content_type, content_ptr, tsk_strlen(content_ptr));
            }
            else {
                /* if too close don't update "last_fir_time" to "now" to be sure interval will increase */
                TSK_DEBUG_INFO("Outgoing SIP INFO ('picture_fast_update') requested but delay too close");
            }
        }
    }
    return 0;
}



















//========================================================
//	SIP dialog INVITE object definition
//
static tsk_object_t* tsip_dialog_invite_ctor(tsk_object_t * self, va_list * app)
{
    tsip_dialog_invite_t *dialog = self;
    if(dialog) {
        tsip_ssession_handle_t *ss = va_arg(*app, tsip_ssession_handle_t *);
        const char* call_id = va_arg(*app, const char *);

        /* Initialize base class */
        tsip_dialog_init(TSIP_DIALOG(self), tsip_dialog_INVITE, call_id, ss, _fsm_state_Started, _fsm_state_Terminated);

        /* FSM */
        TSIP_DIALOG_GET_FSM(dialog)->debug = DEBUG_STATE_MACHINE;
        tsk_fsm_set_callback_terminated(TSIP_DIALOG_GET_FSM(dialog), TSK_FSM_ONTERMINATED_F(tsip_dialog_invite_OnTerminated), (const void*)dialog);

        /* default values */
        dialog->supported._100rel = ((tsip_ssession_t*)ss)->media.enable_100rel;
        dialog->supported.norefersub = tsk_true;
        dialog->required.ice = (((tsip_ssession_t*)ss)->media.profile == tmedia_profile_rtcweb);
        dialog->supported.ice = (dialog->required.ice || ((tsip_ssession_t*)ss)->media.enable_ice);
#if 0 /* This was a patch for chrome but no longer needed when using version 23.0.1271.64 or later */
        dialog->ice.is_jingle = (((tsip_ssession_t*)ss)->media.profile == tmedia_profile_rtcweb);
#else
        dialog->ice.is_jingle = tsk_false;
#endif
        dialog->ice.last_sdp_ro_ver = -1;
        dialog->use_rtcp = (((tsip_ssession_t*)ss)->media.profile == tmedia_profile_rtcweb) ? tsk_true : ((tsip_ssession_t*)ss)->media.enable_rtcp;
        dialog->use_rtcpmux = (((tsip_ssession_t*)ss)->media.profile == tmedia_profile_rtcweb) ? tsk_true : ((tsip_ssession_t*)ss)->media.enable_rtcpmux;

        dialog->ice.last_action_id = tsk_fsm_state_none;
        dialog->refersub = tsk_true;
        dialog->is_conditional_ringing_enabled = ((tsip_ssession_t*)ss)->media.enable_conditional_ringing;
        // ... do the same for preconditions, replaces, ....

        /* Initialize the class itself */
        tsip_dialog_invite_init(self);
    }
    return self;
}

static tsk_object_t* tsip_dialog_invite_dtor(tsk_object_t * _self)
{
    tsip_dialog_invite_t *self = _self;
    if(self) {
        // Cancel all timers
        tsip_dialog_invite_stimers_cancel(self);
        tsip_dialog_invite_qos_timer_cancel(self);
        TSIP_DIALOG_TIMER_CANCEL(shutdown);
        TSIP_DIALOG_TIMER_CANCEL(100rel);

        // DeInitialize base class
        tsip_dialog_deinit(TSIP_DIALOG(self));

        // DeInitialize self
        TSK_OBJECT_SAFE_FREE(self->ss_transf);
        TSK_OBJECT_SAFE_FREE(self->msession_mgr);

        TSK_OBJECT_SAFE_FREE(self->last_oInvite);
        TSK_OBJECT_SAFE_FREE(self->last_iInvite);
        TSK_OBJECT_SAFE_FREE(self->last_o1xxrel);
        TSK_OBJECT_SAFE_FREE(self->last_iRefer);
        TSK_FREE(self->stimers.refresher);

        TSK_OBJECT_SAFE_FREE(self->ice.ctx_audio);
        TSK_OBJECT_SAFE_FREE(self->ice.ctx_video);
        TSK_OBJECT_SAFE_FREE(self->ice.last_action);
        TSK_OBJECT_SAFE_FREE(self->ice.last_message);
        //...

        TSK_DEBUG_INFO("*** INVITE Dialog destroyed ***");
    }
    return self;
}

static int tsip_dialog_invite_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
    return tsip_dialog_cmp(obj1, obj2);
}

static const tsk_object_def_t tsip_dialog_invite_def_s = {
    sizeof(tsip_dialog_invite_t),
    tsip_dialog_invite_ctor,
    tsip_dialog_invite_dtor,
    tsip_dialog_invite_cmp,
};
const tsk_object_def_t *tsip_dialog_invite_def_t = &tsip_dialog_invite_def_s;
