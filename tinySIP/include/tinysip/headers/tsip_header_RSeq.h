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

/**@file tsip_header_RSeq.h
 * @brief SIP header 'RSeq'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_RSEQ_H_
#define _TSIP_HEADER_RSEQ_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

#define TSIP_HEADER_RSEQ_VA_ARGS(seq)		tsip_header_RSeq_def_t, (int32_t) seq

#define TSIP_HEADER_RSEQ_NONE						0
#define TSIP_HEADER_RSEQ_DEFAULT					1

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	SIP header 'RSeq' as per RFC 3262.
///
/// @par ABNF: "RSeq" HCOLON response-num
///
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_RSeq_s {
    TSIP_DECLARE_HEADER;
    uint32_t seq;
}
tsip_header_RSeq_t;


TINYSIP_API tsip_header_RSeq_t* tsip_header_RSeq_create(uint32_t seq);
TINYSIP_API tsip_header_RSeq_t* tsip_header_RSeq_create_null();

TINYSIP_API tsip_header_RSeq_t *tsip_header_RSeq_parse(const char *data, tsk_size_t size);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_header_RSeq_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_RSEQ_H_ */

