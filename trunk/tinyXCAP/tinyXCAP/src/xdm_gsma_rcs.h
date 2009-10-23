/****************************************************************************
			 _             _                             
			| |           | |                            
		  _ | | ___  _   _| | _   ____ ____   ____  ___  
		 / || |/ _ \| | | | || \ / _  |  _ \ / _  |/ _ \ 
		( (_| | |_| | |_| | |_) | ( | | | | ( ( | | |_| |
		 \____|\___/ \____|____/ \_||_|_| |_|\_|| |\___/ 
											(_____|   
	
	Copyright (C) 2009 xxxyyyzzz <imsframework(at)gmail.com>

	This file is part of Open Source Doubango IMS Client Framework project.

    DOUBANGO is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
	
    DOUBANGO is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
	
    You should have received a copy of the GNU General Public License
    along with DOUBANGO.
****************************************************************************/
#ifndef _TINYXCAP_XDM_GSMA_RCS_H_
#define _TINYXCAP_XDM_GSMA_RCS_H_

#include "tinyxcap_config.h"
#include "xdm.h"

#include "xdm_rlist.h"
#include "xdm_rls.h"

/* GSMA RCS2: http://www.gsmworld.com/documents/rcs/rcs2_june09/R2_090831_RCS_Release_2_Technical_Realisation_v1_0.pdf */
/* GSMA RCS1: http://www.gsmworld.com/documents/rcs/rcs1_updates/R1_090831_RCS_Release_1_Technical_Realisation_v1_1.pdf */

/* GSMA RCS1 subclause 4.4.2 */
#define XDM_RNAME_GSMA_RCS_RCS						"rcs"
#define XDM_RNAME_GSMA_RCS_BLOCKEDCONTACTS			"rcs_blockedcontacts"
#define XDM_RNAME_GSMA_RCS_REVOKEDCONTACTS			"rcs_revokedcontacts"

TINYXCAP_API xdm_rlist_list2_L_t* xdm_gsmarcs_create_rlist2(const xdm_context_t* context);
TINYXCAP_API xdm_rls_service_L_t* xdm_gsmarcs_create_rls(const xdm_context_t* context);
TINYXCAP_API char* xdm_gsmarcs_create_oma_presrules(const xdm_context_t* context);

#endif /* _TINYXCAP_XDM_GSMA_RCS_H_ */