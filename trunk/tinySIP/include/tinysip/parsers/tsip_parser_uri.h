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

/**@file tsip_parser_uri.h
 * @brief SIP/SIPS/TEL URI parser.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_PARSER_URI_H
#define TINYSIP_PARSER_URI_H

#include "tinysip_config.h"
#include "tinysip/tsip_uri.h"

#include "tinysip/parsers/tsip_ragel_state.h"

#include <stdint.h>

TINYSIP_API tsip_uri_t *tsip_uri_parse(const char *data, size_t size);

#endif /* TINYSIP_PARSER_URI_H */