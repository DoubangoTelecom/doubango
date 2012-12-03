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

/**@file tsip_header_RAck.h
 * @brief SIP header 'RAck'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_RACK_H_
#define _TSIP_HEADER_RACK_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

#define TSIP_HEADER_RACK_VA_ARGS(seq, cseq, method)		tsip_header_RAck_def_t, (uint32_t)seq, (uint32_t)cseq, (const char*)method

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	SIP header 'RAck' as per RFC 3262.
///
/// @par ABNF : "RAck" HCOLON response-num  LWS  CSeq-num  LWS  Method
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_RAck_s
{	
	TSIP_DECLARE_HEADER;

	uint32_t seq;
	uint32_t cseq;
	char* method;
}
tsip_header_RAck_t;


TINYSIP_API tsip_header_RAck_t* tsip_header_RAck_create(uint32_t seq, uint32_t cseq, const char* method);
TINYSIP_API tsip_header_RAck_t* tsip_header_RAck_create_null();

TINYSIP_API tsip_header_RAck_t *tsip_header_RAck_parse(const char *data, tsk_size_t size);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_header_RAck_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_RACK_H_ */

