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

/**@file tsip_api_invite.h
 * @brief Public messaging (INVITE) functions.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSIP_TSIP_INVITE_H
#define TINYSIP_TSIP_INVITE_H

#include "tinysip_config.h"

#include "tinysip/tsip_event.h"

#include "tinymedia/tmedia_common.h"

TSIP_BEGIN_DECLS

#define TSIP_INVITE_EVENT(self)		((tsip_invite_event_t*)(self))

//@tinyWRAP
typedef enum tsip_invite_event_type_e {
    // ============================
    //	Sip Events
    //
    tsip_i_newcall,
    tsip_i_prechecking,

    //! in-dialog requests/reponses
    tsip_i_request,
    tsip_ao_request,

    /* 3GPP TS 24.629: Explicit Call Transfer (ECT) */
    tsip_o_ect_trying,
    tsip_o_ect_accepted,
    tsip_o_ect_completed,
    tsip_o_ect_failed,
    tsip_o_ect_notify,
    tsip_i_ect_requested,
    tsip_i_ect_newcall,
    tsip_i_ect_completed,
    tsip_i_ect_failed,
    tsip_i_ect_notify,

    // ============================
    //	Media Events
    //

    tsip_m_early_media,
    tsip_m_updating, // Trying to update from Audio -> Video for example
    tsip_m_updated, // succeed to update

    /* 3GPP TS 24.610: Communication Hold */
    tsip_m_local_hold_ok,
    tsip_m_local_hold_nok,
    tsip_m_local_resume_ok,
    tsip_m_local_resume_nok,
    tsip_m_remote_hold,
    tsip_m_remote_resume,
}
tsip_invite_event_type_t;

typedef struct tsip_invite_event_e {
    TSIP_DECLARE_EVENT;

    tsip_invite_event_type_t type;

    struct {
        unsigned toto:1;
    } av;

    /*struct{
    	char* dir;
    } msrp;*/
}
tsip_invite_event_t;

int tsip_invite_event_signal(tsip_invite_event_type_t type, tsip_ssession_handle_t* ss, short status_code, const char *phrase, const struct tsip_message_s* sipmessage);

TINYSIP_API int tsip_api_invite_send_invite(const tsip_ssession_handle_t *ss, tmedia_type_t type, ...);
TINYSIP_API int tsip_api_invite_send_info(const tsip_ssession_handle_t *ss, ...);
TINYSIP_API int tsip_api_invite_send_hold(const tsip_ssession_handle_t *ss, tmedia_type_t type, ...);
TINYSIP_API int tsip_api_invite_send_resume(const tsip_ssession_handle_t *ss, tmedia_type_t type, ...);
TINYSIP_API int tsip_api_invite_send_large_message(const tsip_ssession_handle_t *ss, ...);
TINYSIP_API int tsip_api_invite_send_ect(const tsip_ssession_handle_t *ss, const char* toUri, ...);
TINYSIP_API int tsip_api_invite_send_ect_accept(const tsip_ssession_handle_t *ss, ...);
TINYSIP_API int tsip_api_invite_send_ect_reject(const tsip_ssession_handle_t *ss, ...);
TINYSIP_API int tsip_api_invite_send_sos(const tsip_ssession_handle_t *ss, ...);
TINYSIP_API int tsip_api_invite_send_dtmf(const tsip_ssession_handle_t *ss, int event, ...);
TINYSIP_API int tsip_api_invite_send_bye(const tsip_ssession_handle_t *ss, ...);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_invite_event_def_t;

#if 1 // Backward Compatibility
#	define tsip_action_INVITE	tsip_api_invite_send_invite
#	define tsip_action_HOLD	tsip_api_invite_send_hold
#	define tsip_action_RESUME	tsip_api_invite_send_resume
#	define tsip_action_LARGE_MESSAGE	tsip_api_invite_send_large_message
#	define tsip_action_ECT	tsip_api_invite_send_ect
#	define tsip_action_SOS	tsip_api_invite_send_sos
#	define tsip_action_DTMF	tsip_api_invite_send_dtmf
#	define tsip_action_BYE	tsip_api_invite_send_bye
#endif


TSIP_END_DECLS

#endif /* TINYSIP_TSIP_INVITE_H */
