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

/**@file tsip_header_P_Access_Network_Info.h
 * @brief SIP header 'P-Access-Network-Info'.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_P_ACCESS_NETWORK_INFO_H_
#define _TSIP_HEADER_P_ACCESS_NETWORK_INFO_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

#define TSIP_HEADER_P_ACCESS_NETWORK_INFO_VA_ARGS(value)	tsip_header_P_Access_Network_Info_def_t, (const char*)value

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	SIP header 'P-Access-Network-Info' as per RFC 3455.
///
/// @par ABNF: P-Access-Network-Info  	= 	"P-Access-Network-Info" HCOLON access-net-spec
/// access-net-spec	= 	access-type *( SEMI access-info )
/// access-type	= 	"IEEE-802.11a" / "IEEE-802.11b" / "3GPP-GERAN" / "3GPP-UTRAN-FDD" / "3GPP-UTRAN-TDD" / "3GPP-CDMA2000" / token
/// access-info	= 	cgi-3gpp / utran-cell-id-3gpp / extension-access-info
/// extension-access-info	= 	gen-value
/// cgi-3gpp	= 	"cgi-3gpp" EQUAL (token / quoted-string)
/// utran-cell-id-3gpp	= 	"utran-cell-id-3gpp" EQUAL (token / quoted-string)
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_P_Access_Network_Info_s
{	
	TSIP_DECLARE_HEADER;

	char *value;
}
tsip_header_P_Access_Network_Info_t;

TINYSIP_API tsip_header_P_Access_Network_Info_t* tsip_header_P_Access_Network_Info_create(const char* value);
TINYSIP_API tsip_header_P_Access_Network_Info_t* tsip_header_P_Access_Network_Info_create_null();

TINYSIP_API tsip_header_P_Access_Network_Info_t *tsip_header_P_Access_Network_Info_parse(const char *data, tsk_size_t size);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_header_P_Access_Network_Info_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_P_ACCESS_NETWORK_INFO_H_ */

