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

/**@file tsip_dialog.h
 * @brief SIP dialogtion base class as per RFC 3261 subclause 17.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_DIALOG_H
#define TINYSIP_DIALOG_H

#include "tinysip_config.h"

#include "tinysip/tsip_timers.h"

#include "tsk_object.h"
#include "tsk_list.h"

#define TSIP_DIALOG(self)				((tsip_dialog_t*)(self))
#define TSIP_DIALOG_GET_STATE(self)		TSIP_DIALOG(self)->state

typedef enum tsip_dialog_state_e
{
	tsip_early,
	tsip_established,
	tsip_terminated
}
tsip_dialog_state_t;

#define TSIP_DECLARE_DIALOG struct { \
	TSK_DECLARE_OBJECT; \
	\
	tsip_dialog_state_t state; \
	\
	char* tag_local; \
	char* tag_remote; \
	\
	int32_t cseq_value; \
	char* cseq_method; \
	\
	char* callid; \
}

typedef TSIP_DECLARE_DIALOG tsip_dialog_t;

typedef tsk_list_t tsip_dialogs_L_t;

#endif /* TINYSIP_DIALOG_H */