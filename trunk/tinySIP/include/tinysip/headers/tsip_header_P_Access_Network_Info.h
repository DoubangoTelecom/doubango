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

/**@file tsip_header_P_Access_Network_Info.h
 * @brief SIP header 'P-Access-Network-Info'.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSIP_HEADER_P_ACCESS_NETWORK_INFO_H_
#define _TSIP_HEADER_P_ACCESS_NETWORK_INFO_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

/**@def TSIP_HEADER_P_ACCESS_NETWORK_INFO_CREATE
* Creates new sip 'P-Access-Network-Info' header.  You must call @ref TSIP_HEADER_P_ACCESS_NETWORK_INFO_SAFE_FREE to free the header.
* @sa TSIP_HEADER_P-ACCESS_NETWORK_INFO_SAFE_FREE.
*/
/**@def TSIP_HEADER_P_ACCESS_NETWORK_INFO_SAFE_FREE
* Safely free a sip 'P-Access-Network-Info' header previously created using TSIP_HEADER_P_ACCESS_NETWORK_INFO_CREATE.
* @sa TSIP_HEADER_P_ACCESS_NETWORK_INFO_CREATE.
*/
#define TSIP_HEADER_P_ACCESS_NETWORK_INFO_VA_ARGS(value)	tsip_header_P_Access_Network_Info_def_t, (const char*)value
#define TSIP_HEADER_P_ACCESS_NETWORK_INFO_CREATE(value)		tsk_object_new(TSIP_HEADER_P_ACCESS_NETWORK_INFO_VA_ARGS(value))
#define TSIP_HEADER_P_ACCESS_NETWORK_INFO_SAFE_FREE(self)	tsk_object_unref(self), self = 0

#define TSIP_HEADER_P_ACCESS_NETWORK_INFO_CREATE_NULL()		TSIP_HEADER_P_ACCESS_NETWORK_INFO_CREATE(0)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SIP header 'P-Access-Network-Info' as per RFC 3455.
/// @author	Mamadou
/// @date	12/3/2009
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

tsip_header_P_Access_Network_Info_t *tsip_header_P_Access_Network_Info_parse(const char *data, size_t size);

TINYSIP_GEXTERN const void *tsip_header_P_Access_Network_Info_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_P_ACCESS_NETWORK_INFO_H_ */

