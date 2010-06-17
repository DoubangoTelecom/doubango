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

/**@file tsip_api_invite.h
 * @brief Public messaging (INVITE) functions.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_TSIP_INVITE_H
#define TINYSIP_TSIP_INVITE_H

#include "tinysip_config.h"

#include "tinysip/tsip_event.h"

#include "tinymedia/tmedia_common.h"

TSIP_BEGIN_DECLS

#define TSIP_INVITE_EVENT(self)		((tsip_invite_event_t*)(self))

typedef enum tsip_invite_event_type_e
{
	// ============================
	//	Sip Events
	//
	tsip_i_newcall,
	
	//! in-dialog requests/reponses
	tsip_i_request,
	tsip_ao_request,
	
	/* Explicit Call Transfer (ECT) */
	tsip_o_ect_ok,
	tsip_o_ect_nok,
	tsip_i_ect,
	
	// ============================
	//	Media Events
	//
	
	/* Media State */
	tsip_m_connected,
	tsip_m_terminated,

	/* 3GPP TS 24.610: Communication Hold */
	tsip_m_local_hold_ok,
	tsip_m_local_hold_nok,
	tsip_m_local_resume_ok,
	tsip_m_local_resume_nok,
	tsip_m_remote_hold,
	tsip_m_remote_resume,
}
tsip_invite_event_type_t;

typedef struct tsip_invite_event_e
{
	TSIP_DECLARE_EVENT;

	tsip_invite_event_type_t type;

	struct{
		unsigned toto:1;
	} av;

	struct{
		unsigned titi:1;
	} msrp;
}
tsip_invite_event_t;

int tsip_invite_event_signal(tsip_invite_event_type_t type, tsip_ssession_handle_t* ss, short status_code, const char *phrase, const struct tsip_message_s* sipmessage);

TINYSIP_API int tsip_action_INVITE(const tsip_ssession_handle_t *ss, tmedia_type_t type, ...);
TINYSIP_API int tsip_action_HOLD(const tsip_ssession_handle_t *ss, tmedia_type_t type, ...);
TINYSIP_API int tsip_action_RESUME(const tsip_ssession_handle_t *ss, tmedia_type_t type, ...);
TINYSIP_API int tsip_action_ECT(const tsip_ssession_handle_t *ss, const char* toUri, ...);
TINYSIP_API int tsip_action_SOS(const tsip_ssession_handle_t *ss, ...);
TINYSIP_API int tsip_action_BYE(const tsip_ssession_handle_t *ss, ...);


TINYSIP_GEXTERN const tsk_object_def_t *tsip_invite_event_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_TSIP_INVITE_H */
