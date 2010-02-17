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
 * @brief SIP dialog SUBSCRIBE.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_DIALOG_SUBSCRIBE_H
#define TINYSIP_DIALOG_SUBSCRIBE_H

#include "tinysip_config.h"
#include "tinysip/dialogs/tsip_dialog.h"
#include "tinysip/smc/tsip_dialog_subscribe_sm.h"

TSIP_BEGIN_DECLS

#define TSIP_DIALOG_SUBSCRIBE_CREATE(stack, operation)		tsk_object_new(tsip_dialog_subscribe_def_t, (tsip_stack_handle_t *)stack, (tsip_operation_handle_t*) operation)

#define TSIP_DIALOG_SUBSCRIBE(self)							((tsip_dialog_subscribe_t*)(self))

typedef struct tsip_dialog_subscribe
{
	TSIP_DECLARE_DIALOG;

	struct tsip_dialog_subscribeContext _fsm;
	
	tsip_timer_t timerrefresh;

	unsigned unsubscribing:1;

	char* package;
}
tsip_dialog_subscribe_t;

int tsip_dialog_subscribe_start(tsip_dialog_subscribe_t *self);

void tsip_dialog_subscribe_Started_2_Trying_X_send(tsip_dialog_subscribe_t *self);
void tsip_dialog_subscribe_Trying_2_Trying_X_1xx(tsip_dialog_subscribe_t *self, const tsip_response_t* response);
void tsip_dialog_subscribe_Trying_2_Terminated_X_2xx(tsip_dialog_subscribe_t *self, const tsip_response_t* response);
void tsip_dialog_subscribe_Trying_2_Connected_X_2xx(tsip_dialog_subscribe_t *self, const tsip_response_t* response);
void tsip_dialog_subscribe_Trying_2_Trying_X_401_407_421_494(tsip_dialog_subscribe_t *self, const tsip_response_t* response);
void tsip_dialog_subscribe_Trying_2_Trying_X_423(tsip_dialog_subscribe_t *self, const tsip_response_t* response);
void tsip_dialog_subscribe_Trying_2_Terminated_X_300_to_699(tsip_dialog_subscribe_t *self, const tsip_response_t* response);
void tsip_dialog_subscribe_Trying_2_Terminated_X_cancel(tsip_dialog_subscribe_t *self);
void tsip_dialog_subscribe_Trying_2_Trying_X_NOTIFY(tsip_dialog_subscribe_t *self, const tsip_request_t* request);
void tsip_dialog_subscribe_Connected_2_Trying_X_unsubscribe(tsip_dialog_subscribe_t *self);
void tsip_dialog_subscribe_Connected_2_Trying_X_refresh(tsip_dialog_subscribe_t *self);
void tsip_dialog_subscribe_Connected_2_Connected_X_NOTIFY(tsip_dialog_subscribe_t *self, const tsip_request_t* request);
void tsip_dialog_subscribe_Connected_2_Terminated_X_NOTIFY(tsip_dialog_subscribe_t *self, const tsip_request_t* request);
void tsip_dialog_subscribe_Any_2_Trying_X_hangup(tsip_dialog_subscribe_t *self);
void tsip_dialog_subscribe_Any_2_Terminated_X_transportError(tsip_dialog_subscribe_t *self);
void tsip_dialog_subscribe_OnTerminated(tsip_dialog_subscribe_t *self);
void tsip_dialog_subscribe_OnTerminated(tsip_dialog_subscribe_t *self);

TINYSIP_GEXTERN const void *tsip_dialog_subscribe_def_t;

TSIP_END_DECLS

#endif /* TINYSIP_DIALOG_SUBSCRIBE_H */
