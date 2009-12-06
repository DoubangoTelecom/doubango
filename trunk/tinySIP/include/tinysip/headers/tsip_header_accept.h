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

/**@file tsip_header_accept.h
 * @brief SIP header 'Accept-Contact'.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSIP_HEADER_ACCEPT_H_
#define _TSIP_HEADER_ACCEPT_H_
#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	tsip_header_accept_s
///
/// @brief	SIP header 'Accept' as per RFC 3261 subclause 20.1.
/// @author	Mamadou
/// @date	12/3/2009
///
/// @par ABNF
/// 	Accept = "Accept" HCOLON [ accept-range *(COMMA accept-range) ] ; example: ;
/// 	Accept: application/dialog-info+xml 
///		accept-range = media-range *(SEMI accept-param)
/// 	media-range = ( "*/*" / ( m-type SLASH "*" ) / ( m-type SLASH m-subtype ) )  *( SEMI m-parameter ) 
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Accept_s
{	
	TSIP_DECLARE_HEADER;
}
tsip_header_Accept_t;

#endif /* _TSIP_HEADER_ACCEPT_H_ */