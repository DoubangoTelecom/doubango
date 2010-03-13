/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSIP_HEADER_SUBSCRIPTION_STATE_H_
#define _TSIP_HEADER_SUBSCRIPTION_STATE_H_

#include "tinySIP_config.h"
#include "tinySIP/headers/tsip_header.h"

TSIP_BEGIN_DECLS

/**@def TSIP_HEADER_SUBSCRIPTION_STATE_CREATE
* Creates new sip 'Subscription_State' header.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/
#define TSIP_HEADER_SUBSCRIPTION_STATE_CREATE()		tsk_object_new(tsip_header_Subscription_State_def_t)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SIP header 'Subscription-State'.
/// @author	Mamadou
/// @date	12/3/2009
///
/// @par ABNF: Subscription-State	= 	( "Subscription-State" / "o" ) HCOLON substate-value *( SEMI subexp-params )
/// substate-value	= 	"active" / "pending" / "terminated" / extension-substate
/// extension-substate	= 	token
/// subexp-params	= 	("reason" EQUAL event-reason-value) / ("expires" EQUAL delta-seconds) / ("retry-after" EQUAL delta-seconds) / generic-param
/// event-reason-value	= 	"deactivated" / "probation" / "rejected" / "timeout" / "giveup" / "noresource" / event-reason-extension
/// event-reason-extension 	= 	token
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Subscription_State_s
{	
	TSIP_DECLARE_HEADER;

	char* state;
	char* reason;
	int32_t expires;
	int32_t retry_after;
}
tsip_header_Subscription_State_t;

tsip_header_Subscription_State_t *tsip_header_Subscription_State_parse(const char *data, size_t size);

TINYSIP_GEXTERN const void *tsip_header_Subscription_State_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_SUBSCRIPTION_STATE_H_ */

