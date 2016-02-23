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
/**@file tinyhttp.h
 * @brief API functions.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
/* === tinyNET (tinyNET/src) === */
#include "tnet.h"

/* === tinySAK (tinySAK/src)=== */
#include "tsk.h"

/* === tinyHTTP(tinyHTTP/include) === */
#include "thttp.h"

#include "tinyhttp/thttp_action.h"

#include "tinyhttp/parsers/thttp_parser_message.h"
#include "tinyhttp/parsers/thttp_parser_url.h"
#include "tinyhttp/parsers/thttp_parser_header.h"

#include "tinyhttp/headers/thttp_header_Authorization.h"
#include "tinyhttp/headers/thttp_header_Content_Length.h"
#include "tinyhttp/headers/thttp_header_Content_Type.h"
#include "tinyhttp/headers/thttp_header_Dummy.h"
#include "tinyhttp/headers/thttp_header_ETag.h"
#include "tinyhttp/headers/thttp_header_Sec_WebSocket_Accept.h"
#include "tinyhttp/headers/thttp_header_Sec_WebSocket_Key.h"
#include "tinyhttp/headers/thttp_header_Sec_WebSocket_Protocol.h"
#include "tinyhttp/headers/thttp_header_Sec_WebSocket_Version.h"
#include "tinyhttp/headers/thttp_header_Transfer_Encoding.h"
#include "tinyhttp/headers/thttp_header_WWW_Authenticate.h"
