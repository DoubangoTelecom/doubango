/*
* Copyright (C) 2010-2011 Mamadou Diop.
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

/**@file tinymsrp.h
 * @brief tinyMSRP API.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#ifndef TINYMEDIA_TINYMSRP_H
#define TINYMEDIA_TINYMSRP_H

/* === tinyNET (tinyNET/src) === */
#include "tnet.h"

/* === tinySAK (tinySAK/src)=== */
#include "tsk.h"

#include "tmsrp.h"

#include "tinymsrp/tmsrp_message.h"
#include "tinymsrp/tmsrp_event.h"
#include "tinymsrp/tmsrp_uri.h"

#include "tinymsrp/headers/tmsrp_header_Dummy.h"
#include "tinymsrp/headers/tmsrp_header_Authentication-Info.h"
#include "tinymsrp/headers/tmsrp_header_Authorization.h"
#include "tinymsrp/headers/tmsrp_header_Byte-Range.h"
#include "tinymsrp/headers/tmsrp_header_Expires.h"
#include "tinymsrp/headers/tmsrp_header_Failure-Report.h"
#include "tinymsrp/headers/tmsrp_header_From-Path.h"
#include "tinymsrp/headers/tmsrp_header_Max-Expires.h"
#include "tinymsrp/headers/tmsrp_header_Message-ID.h"
#include "tinymsrp/headers/tmsrp_header_Min-Expires.h"
#include "tinymsrp/headers/tmsrp_header_Status.h"
#include "tinymsrp/headers/tmsrp_header_Success-Report.h"
#include "tinymsrp/headers/tmsrp_header_To-Path.h"
#include "tinymsrp/headers/tmsrp_header_Use-Path.h"
#include "tinymsrp/headers/tmsrp_header_WWW-Authenticate.h"

#include "tinymsrp/parsers/tmsrp_parser_message.h"
#include "tinymsrp/parsers/tmsrp_parser_uri.h"

#include "tinymsrp/session/tmsrp_config.h"
#include "tinymsrp/session/tmsrp_data.h"
#include "tinymsrp/session/tmsrp_receiver.h"
#include "tinymsrp/session/tmsrp_sender.h"

#endif /* TINYMEDIA_TINYMSRP_H */
