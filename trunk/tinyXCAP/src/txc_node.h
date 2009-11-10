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

#ifndef _TINYXCAP_TXC_ELEMENT_H_
#define _TINYXCAP_TXC_ELEMENT_H_

#include "tinyxcap_config.h"
#include "txc.h"

TINYXCAP_API txc_request_t* txc_node_get_sel(const txc_context_t* context, const xcap_auid_type_t auid_type, ...);

#endif /* _TINYXCAP_TXC_ELEMENT_H_ */