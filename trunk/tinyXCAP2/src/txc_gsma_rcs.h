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

/**@file txc_gsma_rcs.h
 * @brief GSMA <a href="http://www.gsmworld.com/documents/rcs/rcs1_updates/R1_090831_RCS_Release_1_Technical_Realisation_v1_1.pdf">RCS 1</a> and <a href="http://www.gsmworld.com/documents/rcs/rcs2_june09/R2_090831_RCS_Release_2_Technical_Realisation_v1_0.pdf ">RCS 2</a> documents
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#ifndef _TINYXCAP_TXC_GSMA_RCS_H_
#define _TINYXCAP_TXC_GSMA_RCS_H_

#include "tinyxcap_config.h"
#include "txc.h"

#include "txc_rlist.h"
#include "txc_rls.h"

/* GSMA RCS2: http://www.gsmworld.com/documents/rcs/rcs2_june09/R2_090831_RCS_Release_2_Technical_Realisation_v1_0.pdf */
/* GSMA RCS1: http://www.gsmworld.com/documents/rcs/rcs1_updates/R1_090831_RCS_Release_1_Technical_Realisation_v1_1.pdf */

/* GSMA RCS1 subclause 4.4.2 */
#define TXC_RNAME_GSMA_RCS_RCS						"rcs"
#define TXC_RNAME_GSMA_RCS_BLOCKEDCONTACTS			"rcs_blockedcontacts"
#define TXC_RNAME_GSMA_RCS_REVOKEDCONTACTS			"rcs_revokedcontacts"

TINYXCAP_API txc_rlist_list2_L_t* txc_gsmarcs_create_rlist2(const txc_context_t* context);
TINYXCAP_API txc_rls_service_L_t* txc_gsmarcs_create_rls(const txc_context_t* context);
TINYXCAP_API char* txc_gsmarcs_create_oma_presrules(const txc_context_t* context);

#endif /* _TINYXCAP_TXC_GSMA_RCS_H_ */