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

/**@file tmsrp_parser_uri.h
 * @brief MSRP/MSRPS URI parser.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#ifndef TINYMSRP_PARSER_URI_H
#define TINYMSRP_PARSER_URI_H

#include "tinymsrp_config.h"
#include "tinymsrp/tmsrp_uri.h"

#include "tsk_ragel_state.h"

TMSRP_BEGIN_DECLS

TINYMSRP_API tmsrp_uri_t *tmsrp_uri_parse(const char *data, tsk_size_t size);

TMSRP_END_DECLS

#endif /* TINYMSRP_PARSER_URI_H */

