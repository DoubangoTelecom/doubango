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

/**@file tsip_header_Subscription_State.h
 * @brief SIP header 'Subscription-State'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_SUBSCRIPTION_STATE_H_
#define _TSIP_HEADER_SUBSCRIPTION_STATE_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS


////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	SIP header 'Subscription-State'.
///
/// @par ABNF: Subscription-State	= 	( "Subscription-State" / "o" ) HCOLON substate-value *( SEMI subexp-params )
/// substate-value	= 	"active" / "pending" / "terminated" / extension-substate
/// extension-substate	= 	token
/// subexp-params	= 	("reason" EQUAL event-reason-value) / ("expires" EQUAL delta-seconds) / ("retry-after" EQUAL delta-seconds) / generic-param
/// event-reason-value	= 	"deactivated" / "probation" / "rejected" / "timeout" / "giveup" / "noresource" / event-reason-extension
/// event-reason-extension 	= 	token
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Subscription_State_s {
    TSIP_DECLARE_HEADER;

    char* state;
    char* reason;
    int32_t expires;
    int32_t retry_after;
}
tsip_header_Subscription_State_t;

tsip_header_Subscription_State_t* tsip_header_Subscription_State_create();

tsip_header_Subscription_State_t *tsip_header_Subscription_State_parse(const char *data, tsk_size_t size);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_header_Subscription_State_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_SUBSCRIPTION_STATE_H_ */

