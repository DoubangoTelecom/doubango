/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou [at) doubango (DOT) org>
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

/**@file tmsrp_parser_message.h
 * @brief MSRP message parser.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#ifndef TINYMSRP_PARSER_MESSAGE_H
#define TINYMSRP_PARSER_MESSAGE_H

#include "tinymsrp_config.h"
#include "tinymsrp/tmsrp_message.h"
#include "tsk_ragel_state.h"

TMSRP_BEGIN_DECLS

TINYMSRP_API tmsrp_message_t* tmsrp_message_parse_2(const void *input, tsk_size_t size, tsk_size_t* msg_size);
TINYMSRP_API tmsrp_message_t* tmsrp_message_parse(const void *input, tsk_size_t size);

TMSRP_END_DECLS

#endif /* TINYMSRP_PARSER_MESSAGE_H */

