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

/**@file tsip_header_CSeq.h
 * @brief SIP header 'CSeq'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_CSEQ_H_
#define _TSIP_HEADER_CSEQ_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

#include "tinysip/tsip_message_common.h" /* tsip_request_type_t */

TSIP_BEGIN_DECLS

#define TSIP_HEADER_CSEQ_VA_ARGS(seq, method)		tsip_header_CSeq_def_t, (int32_t) seq, (const char*)method

#define TSIP_HEADER_CSEQ_NONE						0
#define TSIP_HEADER_CSEQ_DEFAULT					1

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	SIP header 'CSeq'.
///
/// @par ABNF: CSeq	= 	"CSeq" HCOLON 1*DIGIT LWS Method
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_CSeq_s {
    TSIP_DECLARE_HEADER;

    char *method;
    uint32_t seq;
    tsip_request_type_t type;
}
tsip_header_CSeq_t;

TINYSIP_API tsip_header_CSeq_t* tsip_header_CSeq_create(int32_t seq, const char*method);

TINYSIP_API tsip_header_CSeq_t *tsip_header_CSeq_parse(const char *data, tsk_size_t size);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_header_CSeq_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_CSEQ_H_ */

