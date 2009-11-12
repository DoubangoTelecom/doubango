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

/**@file txc_ietf-pres-rules.h
 * @brief <a href="http://tools.ietf.org/html/rfc5025">RFC 5025 - IETF Presence Authorization Rules</a>
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#include "tinyxcap_config.h"
#include "txc.h"

#ifndef _TINYXCAP_IETF_PRES_RULES_H_
#define _TINYXCAP_IETF_PRES_RULES_H_

TINYXCAP_API char* txc_ietf_presrules_serialize(const txc_context_t *context);

#endif /* _TINYXCAP_IETF_PRES_RULES_H_ */