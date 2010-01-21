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

/**@file tsip_header_CSeq.h
 * @brief SIP header 'CSeq'.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSIP_HEADER_CSEQ_H_
#define _TSIP_HEADER_CSEQ_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

/**@def TSIP_HEADER_CSEQ_CREATE
* Creates new sip 'CSeq' header.  You must call @ref TSK_OBJECT_SAFE_FREE to free the header.
* @sa TSK_OBJECT_SAFE_FREE.
*/

#define TSIP_HEADER_CSEQ_VA_ARGS(seq, method)		tsip_header_CSeq_def_t, (int32_t) seq, (const char*)method
#define TSIP_HEADER_CSEQ_CREATE(seq, method)		tsk_object_new(TSIP_HEADER_CSEQ_VA_ARGS(seq, method))

#define TSIP_HEADER_CSEQ_NONE						-1
#define TSIP_HEADER_CSEQ_DEFAULT					1


////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SIP header 'CSeq'.
/// @author	Mamadou
/// @date	12/3/2009
///
/// @par ABNF: CSeq	= 	"CSeq" HCOLON 1*DIGIT LWS Method
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_CSeq_s
{	
	TSIP_DECLARE_HEADER;

	char *method;
	int32_t seq;
}
tsip_header_CSeq_t;

tsip_header_CSeq_t *tsip_header_CSeq_parse(const char *data, size_t size);

TINYSIP_GEXTERN const void *tsip_header_CSeq_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_CSEQ_H_ */

