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

/* as per RCS1: http://www.gsmworld.com/documents/rcs/rcs1_updates/R1_090831_RCS_Release_1_Technical_Realisation_v1_1.pdf subclause 4.4*/
#ifndef _TINYXCAP_TXC_OMA_PRESRULES_H_
#define _TINYXCAP_TXC_OMA_PRESRULES_H_

#include "tinyxcap_config.h"
#include "txc.h"

/* urn:ietf:params:xml:ns:pres-rules */

TINYXCAP_API char* txc_oma_presrules_serialize(const txc_context_t *context);

#endif /* _TINYXCAP_TXC_OMA_PRESRULES_H_ */