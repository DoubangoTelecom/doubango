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

/**@file tsip_transac_ict.h
 * @brief SIP INVITE Client Transaction as per RFC 3261 subclause 17.1.1.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSIP_TRANSAC_ICT_H
#define TINYSIP_TRANSAC_ICT_H

#include "tinysip_config.h"

#include "tinysip/transactions/tsip_transac.h"
#include "tinysip/tsip_message.h"

TSIP_BEGIN_DECLS

#define TSIP_TRANSAC_ICT(self)															((tsip_transac_ict_t*)(self))

typedef struct tsip_transac_ict
{
	TSIP_DECLARE_TRANSAC;
	
	tsip_request_t* request;

	tsip_timer_t timerA;
	tsip_timer_t timerB;
	tsip_timer_t timerD;
	tsip_timer_t timerM;
}
tsip_transac_ict_t;

tsip_transac_ict_t* tsip_transac_ict_create(int32_t cseq_value, const char* callid, tsip_transac_dst_t* dst);
int tsip_transac_ict_start(tsip_transac_ict_t *self, const tsip_request_t* request);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_transac_ict_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_TRANSAC_ICT_H */

