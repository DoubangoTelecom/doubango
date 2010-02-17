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

/**@file tsip_dialog_register.h
 * @brief SIP dialog REGISTER.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_DIALOG_REGISTER_H
#define TINYSIP_DIALOG_REGISTER_H

#include "tinysip_config.h"
#include "tinysip/dialogs/tsip_dialog.h"
#include "tinysip/smc/tsip_dialog_register_sm.h"

TSIP_BEGIN_DECLS

#define TSIP_DIALOG_REGISTER_CREATE(stack, operation)		tsk_object_new(tsip_dialog_register_def_t, (tsip_stack_handle_t *)stack, (tsip_operation_handle_t*) operation)

#define TSIP_DIALOG_REGISTER(self)							((tsip_dialog_register_t*)(self))

typedef struct tsip_dialog_register
{
	TSIP_DECLARE_DIALOG;

	struct tsip_dialog_registerContext _fsm;
	
	tsip_timer_t timerrefresh;

	unsigned unregistering:1;
}
tsip_dialog_register_t;

//void tsip_dialog_register_init(tsip_dialog_register_t *self);
int tsip_dialog_register_start(tsip_dialog_register_t *self);

void tsip_dialog_register_Started_2_Trying_X_send(tsip_dialog_register_t *self);
void tsip_dialog_register_Trying_2_Trying_X_1xx(tsip_dialog_register_t *self, const tsip_message_t *msg);
void tsip_dialog_register_Trying_2_Connected_X_2xx(tsip_dialog_register_t *self, const tsip_message_t *msg);
void tsip_dialog_register_Trying_2_Terminated_X_2xx(tsip_dialog_register_t *self, const tsip_message_t *msg);
void tsip_dialog_register_Trying_2_Trying_X_401_407_421_494(tsip_dialog_register_t *self, const tsip_message_t *msg);
void tsip_dialog_register_Trying_2_Trying_X_423(tsip_dialog_register_t *self, const tsip_message_t *msg);
void tsip_dialog_register_Trying_2_Terminated_X_300_to_699(tsip_dialog_register_t *self, const tsip_message_t *msg);
void tsip_dialog_register_Trying_2_Terminated_X_cancel(tsip_dialog_register_t *self);
void tsip_dialog_register_Connected_2_Trying_X_unregister(tsip_dialog_register_t *self);
void tsip_dialog_register_Connected_2_Trying_X_refresh(tsip_dialog_register_t *self);
void tsip_dialog_register_Any_2_Trying_X_hangup(tsip_dialog_register_t *self);
void tsip_dialog_register_Any_2_Terminated_X_transportError(tsip_dialog_register_t *self);
void tsip_dialog_register_OnTerminated(tsip_dialog_register_t *self);


TINYSIP_GEXTERN const void *tsip_dialog_register_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_DIALOG_REGISTER_H */

