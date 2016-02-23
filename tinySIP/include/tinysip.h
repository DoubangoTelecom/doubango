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

/**@file tinysip.h
 * @brief SIP (RFC 3261) and 3GPP IMS/LTE (TS 24.229) API.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef TINYSIP_TINYSIP_H
#define TINYSIP_TINYSIP_H

/* === tinySAK === */
#include "tsk.h"

/* === tinyNET === */
#include "tinynet.h"

/* === tinyMEDIA === */
#include "tinymedia.h"

/* === tinySDP === */
#include "tinysdp.h"

/* === tinySIP === */
#include "tsip.h"

#include "tinysip/parsers/tsip_parser_uri.h"
#include "tinysip/parsers/tsip_parser_header.h"
#include "tinysip/parsers/tsip_parser_message.h"

#include "tinysip/tsip_ssession.h"
#include "tinysip/tsip_action.h"

#include "tinysip/tsip_message.h"

#include "tinysip/headers/tsip_headers.h"

#include "tinysip/api/tsip_api_common.h"
#include "tinysip/api/tsip_api_invite.h"
#include "tinysip/api/tsip_api_message.h"
#include "tinysip/api/tsip_api_info.h"
#include "tinysip/api/tsip_api_options.h"
#include "tinysip/api/tsip_api_publish.h"
#include "tinysip/api/tsip_api_register.h"
#include "tinysip/api/tsip_api_subscribe.h"

#endif /* TINYSIP_TINYSIP_H */
