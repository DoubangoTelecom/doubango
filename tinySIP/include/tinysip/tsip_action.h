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

/**@file tsip_action.h
 * @brief SIP action.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TSIP_ACTION_H
#define TSIP_ACTION_H

#include "tinysip_config.h"

#include "tsk_buffer.h"
#include "tsk_options.h"
#include "tsk_params.h"

#include "tinymedia/tmedia_common.h"
#include "tinymedia/tmedia_params.h"

TSIP_BEGIN_DECLS

typedef uint64_t tsip_action_id_t;
#define TSIP_ACTION_INVALID_ID				0
#define TSIP_ACTION_INVALID_HANDLE			tsk_null

/** List of all supported actions */
typedef enum tsip_action_type_e {
    //! Used as configuration action
    tsip_atype_config,
    tsip_atype_dtmf_send,

    /* === REGISTER == */
    tsip_atype_register, /**< Sends SIP REGISTER request */
    //! Unregister by sending SIP REGISTER request with expires value equals to zero
#define tsip_atype_unregister tsip_atype_hangup

    /* === SUBSCRIBE === */
    tsip_atype_subscribe, /**< Sends SIP SUBSCRIBE request */
    //! Unsubsribe by sending SIP SUBSCRIBE request with expires value equals to zero
#define tsip_atype_unsubscribe tsip_atype_hangup

    /* === MESSAGE === */
    tsip_atype_message_send, /**< Sends SIP MESSAGE request */

    /* === INFO === */
    tsip_atype_info_send, /**< Sends SIP INFO request */

    /* === PUBLISH === */
    tsip_atype_publish, /**< Sends SIP PUBLISH request */
    //! Unpublish by sending SIP PUBLISH request with expires value equals to zero
#define tsip_atype_unpublish tsip_atype_hangup

    /* === OPTIONS === */
    tsip_atype_options_send, /**< Sends SIP OPTIONS request */

    /* === INVITE === */
    tsip_atype_invite, /**< Sends SIP INVITE/reINVITE request */
    tsip_atype_hold, /**< Puts the session on hold state */
    tsip_atype_resume, /**< Resumes a previously held session */
    tsip_atype_ect, /**< Transfer the call */
    tsip_atype_ect_accept, /**< Accept call transfer request */
    tsip_atype_ect_reject, /**< Reject call transfer request */
    tsip_atype_ect_lnotify, /**< Intra-Dialog notify. Never called by the end-user */
    tsip_atype_lmessage, /**< Large message (MSRP). The session must be connected */
#define tsip_atype_bye tsip_atype_hangup


    /* === common === */
    //! Accept incoming call (INVITE) or message (SIP MESSAGE)
    tsip_atype_accept,
    //! Reject incoming call (INVITE) or message (SIP MESSAGE)
#define tsip_atype_reject tsip_atype_hangup
    //! Cancel an outgoing request
    tsip_atype_cancel,
    //! Hangup any SIP dialog (BYE, unREGISTER, unSUBSCRIBE ...). If the dialog is in early state, then it will be canceled.
    tsip_atype_hangup,
    //! Shutdown a SIP dialog. Should only be called by the stack.
    tsip_atype_shutdown,
    //! Signal transport error. Should only be called by the stack.
    tsip_atype_transport_error,
}
tsip_action_type_t;

/* internal enum used to pass parameters from the application layer to the stack */
typedef enum tsip_action_param_type_e {
    aptype_null = 0,

    aptype_header,
    aptype_config,
    aptype_payload,
    aptype_resp_line,
    aptype_media_type,
    aptype_media,
}
tsip_action_param_type_t;

#define TSIP_ACTION_SET_HEADER(NAME_STR, VALUE_STR)			aptype_header, (const char*)NAME_STR, (const char*)VALUE_STR
#define TSIP_ACTION_SET_PAYLOAD(PAY_PTR, PAY_SIZE)			aptype_payload, (const void*)PAY_PTR, (tsk_size_t)PAY_SIZE
#define TSIP_ACTION_SET_RESP_LINE(CODE_INT, PHRASE_STR)		aptype_resp_line, (int32_t)CODE_INT, (const char*)PHRASE_STR
#define TSIP_ACTION_SET_CONFIG(ACTION_CONFIG_HANDLE)		aptype_config, (const tsip_action_handle_t*)ACTION_CONFIG_HANDLE
#define TSIP_ACTION_SET_MEDIA_TYPE(TYPE_ENUM)				aptype_media_type, (enum tmedia_type_e)TYPE_ENUM
#define TSIP_ACTION_SET_MEDIA(...)							aptype_media, ##__VA_ARGS__
#define TSIP_ACTION_SET_NULL()								aptype_null

/* private action object. public api should use tsip_action_handle_t. */
typedef struct tsip_action_s {
    TSK_DECLARE_OBJECT;

    tsip_action_type_t type;
    tsk_params_L_t *headers;
    tsk_buffer_t* payload;

    struct {
        short code;
        char* phrase;
    } line_resp;

    struct {
        tmedia_type_t type;
        tmedia_params_L_t *params;
    } media;

    struct {
        int volume; // useless (manager will always use "10")
        int event;
    } dtmf;

    struct {
        char* to;
    } ect;
}
tsip_action_t;

/**< Handle to SIP action */
typedef void tsip_action_handle_t;

TINYSIP_API tsip_action_handle_t* tsip_action_create(tsip_action_type_t type, ...);
TINYSIP_API int tsip_action_set(tsip_action_handle_t* self, ...);

typedef tsk_list_t tsip_actions_L_t; /**< List of @ref tsip_action_handle_t elements. */
TINYSIP_GEXTERN const tsk_object_def_t *tsip_action_def_t;

TSIP_END_DECLS

#endif /* TSIP_ACTION_H */

