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

/**@file tsdp_parser_message.h
 * @brief SDP message parser.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSDP_PARSER_MESSAGE_H
#define TINYSDP_PARSER_MESSAGE_H

#include "tinysdp_config.h"
#include "tinysdp/tsdp_message.h"
#include "tsk_ragel_state.h"

TSDP_BEGIN_DECLS

TINYSDP_API tsdp_message_t* tsdp_message_parse(const void *input, tsk_size_t size);

TSDP_END_DECLS

#endif /* TINYSDP_PARSER_MESSAGE_H */

