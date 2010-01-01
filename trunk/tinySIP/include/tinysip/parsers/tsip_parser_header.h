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

/**@file tsip_parser_header.h
 * @brief SIP headers parser.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_PARSER_HEADERS_H
#define TINYSIP_PARSER_HEADERS_H

#include "tinysip_config.h"
#include "tinysip/tsip_message.h"
#include "tinysip/parsers/tsip_ragel_state.h"

TSIP_BEGIN_DECLS

TSIP_BOOLEAN tsip_header_parse(tsip_ragel_state_t *state, tsip_message_t *message);

TSIP_END_DECLS

#endif /* TINYSIP_PARSER_HEADERS_H */

