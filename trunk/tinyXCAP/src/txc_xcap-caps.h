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

/**@file txc_xcap-caps.h
 * @brief RFC 4825 subclause <a href="http://tools.ietf.org/html/rfc4825#section-12">12. XCAP Server Capabilities</a>
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#ifndef _TINYXCAP_TXC_XCAP_CAPS_H_
#define _TINYXCAP_TXC_XCAP_CAPS_H_

#include "tinyxcap_config.h"
#include "tsk_xml.h"

/**
* xcap-caps document structure.
*/
typedef struct xcap_caps_s
{
	tsk_list_t* auids; /**< List of supported AUIDs */
	tsk_list_t*	extensions; /**< List of supported extensions */
	tsk_list_t* namespaces; /**< List of supported namespaces */
}
xcap_caps_t;

TINYXCAP_API xcap_caps_t* txc_xcap_caps_create(const char* buffer, size_t size);
TINYXCAP_API void txc_xcap_caps_free(xcap_caps_t** caps);

#endif /* _TINYXCAP_TXC_XCAP_CAPS_H_ */