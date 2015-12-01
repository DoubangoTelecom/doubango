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

/**@file tsip_transac_nist.h
 * @brief SIP Non-INVITE Server Transaction as per RFC 3261 subclause 17.2.2.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSIP_TRANSAC_NIST_H
#define TINYSIP_TRANSAC_NIST_H

#include "tinysip_config.h"

#include "tinysip/transactions/tsip_transac.h"
#include "tinysip/tsip_message.h"

TSIP_BEGIN_DECLS

#define TSIP_TRANSAC_NIST(self)															((tsip_transac_nist_t*)(self))

typedef struct tsip_transac_nist
{
	TSIP_DECLARE_TRANSAC;

	tsip_response_t* lastResponse;
	tsip_timer_t timerJ;
}
tsip_transac_nist_t;

tsip_transac_nist_t* tsip_transac_nist_create(int32_t cseq_value, const char* cseq_method, const char* callid, tsip_transac_dst_t* dst);
int tsip_transac_nist_start(tsip_transac_nist_t *self, const tsip_request_t* request);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_transac_nist_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_TRANSAC_NIST_H */

