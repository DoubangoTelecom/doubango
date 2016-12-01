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

/**@file tmsrp.h
 * @brief MSRP API.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#ifndef TINYMSRP_TMSRP_H
#define TINYMSRP_TMSRP_H

#include "tinymsrp_config.h"

#include "tinymsrp/tmsrp_message.h"

TMSRP_BEGIN_DECLS

TINYMSRP_API tmsrp_request_t* tmsrp_create_bodiless(const tmsrp_uri_t* To, const tmsrp_uri_t* From);
TINYMSRP_API tmsrp_response_t* tmsrp_create_response(const tmsrp_request_t* request, short status, const char* comment);
TINYMSRP_API tmsrp_request_t* tmsrp_create_report(const tmsrp_request_t* SEND, short status, const char* reason);
TINYMSRP_API tsk_bool_t tmsrp_isReportRequired(const tmsrp_request_t* request, tsk_bool_t failed);

TMSRP_END_DECLS

#endif /* TINYMSRP_TMSRP_H */
