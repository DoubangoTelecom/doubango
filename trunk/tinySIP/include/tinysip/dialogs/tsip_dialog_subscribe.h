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

/**@file tsip_dialog_subscribe.h
 * @brief SIP dialog SUBSCRIBE as per RFC 3265.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_DIALOG_SUBSCRIBE_H
#define TINYSIP_DIALOG_SUBSCRIBE_H

#include "tinysip_config.h"
#include "tinysip/dialogs/tsip_dialog.h"

#include "tsk_fsm.h"

TSIP_BEGIN_DECLS

#define TSIP_DIALOG_SUBSCRIBE_CREATE(stack, operation)		tsk_object_new(tsip_dialog_subscribe_def_t, (tsip_stack_handle_t *)stack, (tsip_operation_handle_t*) operation)

#define TSIP_DIALOG_SUBSCRIBE(self)							((tsip_dialog_subscribe_t*)(self))

typedef struct tsip_dialog_subscribe
{
	TSIP_DECLARE_DIALOG;

	tsk_fsm_t *fsm;
		
	tsip_timer_t timerrefresh;

	unsigned unsubscribing:1;
}
tsip_dialog_subscribe_t;

int tsip_dialog_subscribe_start(tsip_dialog_subscribe_t *self);

TINYSIP_GEXTERN const void *tsip_dialog_subscribe_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_DIALOG_SUBSCRIBE_H */
