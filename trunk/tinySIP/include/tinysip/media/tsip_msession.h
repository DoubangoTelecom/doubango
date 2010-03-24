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

/**@file tsip_msession.h
 * @brief SIP media session.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_MEDIA_SESSION_H
#define TINYSIP_MEDIA_SESSION_H

#include "tinySIP_config.h"

#include "tsk_object.h"

TSIP_BEGIN_DECLS

#define TSIP_MSESSION_CREATE()		tsk_object_new(tsip_msession_def_t)

typedef enum tsip_msession_state_e
{
	ms_none,
	ms_running,
	ms_paused,
	ms_stopped
}
tsip_msession_state_t;

typedef struct tsip_msession_s
{
	TSK_DECLARE_OBJECT;

	tsip_msession_state_t state;
}
tsip_msession_t;

int tsip_msession_start(tsip_msession_t* self);
int tsip_msession_pause(tsip_msession_t* self);
int tsip_msession_stop(tsip_msession_t* self);

TINYSIP_GEXTERN const void *tsip_msession_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_MEDIA_SESSION_H */
