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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsip_transac_ist.h
 * @brief SIP INVITE Server Transaction as per RFC 3261 subclause 17.2.1.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_TRANSAC_IST_H
#define TINYSIP_TRANSAC_IST_H

#include "tinysip_config.h"
#include "tinysip/smc/tsip_transac_ist_sm.h"
#include "tinysip/transactions/tsip_transac.h"

TSIP_BEGIN_DECLS

typedef struct tsip_transac_ist
{
	TSIP_DECLARE_TRANSAC;

	struct tsip_transac_istContext _fsm; /**< The state machine context. */
}
tsip_transac_ist_t;

void tsip_transac_ist_init(tsip_transac_ist_t *self);

TSIP_END_DECLS

#endif /* TINYSIP_TRANSAC_IST_H */

