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

/**@file tsip_api_subscribe.h
 * @brief Public subscription (SUBSCRIBE) functions.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSIP_TSIP_SUBSCRIBE_H
#define TINYSIP_TSIP_SUBSCRIBE_H

#include "tinysip_config.h"

#include "tinysip/tsip_event.h"

TSIP_BEGIN_DECLS

#define TSIP_SUBSCRIBE_EVENT(self)		((tsip_subscribe_event_t*)(self))

//@tinyWRAP
typedef enum tsip_subscribe_event_type_e {
    tsip_i_subscribe,
    tsip_ao_subscribe,

    tsip_i_unsubscribe,
    tsip_ao_unsubscribe,

    tsip_i_notify,
    tsip_ao_notify
}
tsip_subscribe_event_type_t;

typedef struct tsip_subscribe_event_e {
    TSIP_DECLARE_EVENT;

    tsip_subscribe_event_type_t type;
}
tsip_subscribe_event_t;

int tsip_subscribe_event_signal(tsip_subscribe_event_type_t type, tsip_ssession_t* ss, short status_code, const char *phrase, const struct tsip_message_s* sipmessage);

TINYSIP_API int tsip_api_subscribe_send_subscribe(const tsip_ssession_handle_t *ss, ...);
TINYSIP_API int tsip_api_subscribe_send_unsubscribe(const tsip_ssession_handle_t *ss, ...);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_subscribe_event_def_t;

#if 1 // Backward Compatibility
#	define tsip_action_SUBSCRIBE	tsip_api_subscribe_send_subscribe
#	define tsip_action_UNSUBSCRIBE	tsip_api_subscribe_send_unsubscribe
#endif

TSIP_END_DECLS

#endif /* TINYSIP_TSIP_SUBSCRIBE_H */
