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

/**@file tsip_dialog_messager.h
 * @brief SIP dialog MESSAGE.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_DIALOG_MESSAGE_H
#define TINYSIP_DIALOG_MESSAGE_H

#include "tinysip_config.h"
#include "tinysip/dialogs/tsip_dialog.h"

#include "tsk_fsm.h"

TSIP_BEGIN_DECLS

#define TSIP_DIALOG_MESSAGE_CREATE(stack, operation)		tsk_object_new(tsip_dialog_message_def_t, (const tsip_stack_handle_t *)stack, (const tsip_operation_handle_t*) operation)

#define TSIP_DIALOG_MESSAGE(self)							((tsip_dialog_message_t*)(self))

typedef struct tsip_dialog_message
{
	TSIP_DECLARE_DIALOG;

	tsk_fsm_t *fsm;
	
	tsip_message_t *msg;

	unsigned sender:1;
}
tsip_dialog_message_t;

int tsip_dialog_message_start(tsip_dialog_message_t *self);

TINYSIP_GEXTERN const void *tsip_dialog_message_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_DIALOG_MESSAGE_H */
