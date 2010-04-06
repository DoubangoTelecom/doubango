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

/**@file txc_oma-prescont.h
 * @brief <a href="http://www.openmobilealliance.org/technical/release_program/docs/PresenceSIMPLE/V2_0-20081223-C/OMA-TS-Presence_SIMPLE_Content_XDM-V1_0-20081223-C.pdf">[OMA-TS-Presence-SIMPLE_Content_XDM-V1_0-20081223-C] - OMA Presence Content (Avatar)</a>
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TINYXCAP_TXC_OMA_PRESCONT_H_
#define _TINYXCAP_TXC_OMA_PRESCONT_H_

#include "tinyxcap_config.h"
#include "txc.h"
#include "tsk_xml.h"

/** OMA Presence-Content structure */
typedef struct txc_oma_prescont_s
{
	char* mime_type; /**< The Mime-Type*/
	char* encoding; /**< The type of encoding (e.g. base64)*/
	char* description; /**< The description*/
	char* data; /**< The content */
}
txc_oma_prescont_t;

TINYXCAP_API txc_oma_prescont_t* txc_oma_prescont_create(const char* buffer, size_t size);
TINYXCAP_API void txc_oma_prescont_free(txc_oma_prescont_t **pres_cont);

#endif /* _TINYXCAP_TXC_OMA_PRESCONT_H_ */