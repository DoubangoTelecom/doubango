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

/**@file tsip_api_common.h
 * @brief Public common functions.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSIP_TSIP_COMMON_H
#define TINYSIP_TSIP_COMMON_H

#include "tinysip_config.h"

#include "tinysip/tsip_ssession.h"

TSIP_BEGIN_DECLS

TINYSIP_API int tsip_api_common_reject(const tsip_ssession_handle_t *ss, ...);
TINYSIP_API int tsip_api_common_hangup(const tsip_ssession_handle_t *ss, ...);
TINYSIP_API int tsip_api_common_accept(const tsip_ssession_handle_t *ss, ...);


#if 1 // Backward Compatibility
#	define tsip_action_REJECT	tsip_api_common_reject
#	define tsip_action_HANGUP	tsip_api_common_hangup
#	define tsip_action_ACCEPT	tsip_api_common_accept
#endif

TSIP_END_DECLS

#endif /* TINYSIP_TSIP_COMMON_H */
