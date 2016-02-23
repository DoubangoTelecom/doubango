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

/**@file tsip_parser_message.h
 * @brief SIP message parser.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSIP_PARSER_MESSAGE_H
#define TINYSIP_PARSER_MESSAGE_H

#include "tinysip_config.h"
#include "tinysip/tsip_message.h"
#include "tsk_ragel_state.h"

TSIP_BEGIN_DECLS

TINYSIP_API tsk_bool_t tsip_message_parse(tsk_ragel_state_t *state, tsip_message_t **result, tsk_bool_t extract_content);

TSIP_END_DECLS

#endif /* TINYSIP_PARSER_MESSAGE_H */

