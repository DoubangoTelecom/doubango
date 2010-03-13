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
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsip_header_Authentication_Info.h
 * @brief SIP header 'Authentication-Info'.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSIP_HEADER_AUTHENTICATION_INFO_H_
#define _TSIP_HEADER_AUTHENTICATION_INFO_H_

#include "tinySIP_config.h"
#include "tinySIP/headers/tsip_header.h"


TSIP_BEGIN_DECLS

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SIP header 'Authentication-Info'.
/// @author	Mamadou
/// @date	12/3/2009
///
/// @par ABNF
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Authentication_Info_s
{	
	TSIP_DECLARE_HEADER;
}
tsip_header_Authentication_Info_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_AUTHENTICATION_INFO_H_ */


