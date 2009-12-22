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

/**@file tsip_transac_nict.c
 * @brief SIP Non-INVITE Client Transaction as per RFC 3261 subcaluse 17.1.2.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/transactions/tsip_transac_nict.h"


#define DEBUG_STATE_MACHINE				1
#define TIMER_SCHEDULE(timeout, arg)	tsk_timer_manager_schedule(0, timeout, tsip_transac_nict_timer_callback, arg)

/*== Timer callback function.
*/
static int tsip_transac_nict_timer_callback(const void* arg, tsk_timer_id_t timer_id)
{
	const tsip_transac_nict_t* transac = arg;
	
	if(timer_id == transac->timerE.id)
	{
		tsip_transac_nictContext_sm_timerE(&TSIP_TRANSAC_NICT(transac)->_fsm);
	}
	else if(timer_id == transac->timerF.id)
	{
		tsip_transac_nictContext_sm_timerF(&TSIP_TRANSAC_NICT(transac)->_fsm);
	}
	else if(timer_id == transac->timerK.id)
	{
		tsip_transac_nictContext_sm_timerK(&TSIP_TRANSAC_NICT(transac)->_fsm);
	}
	
	return 0;
}

void tsip_transac_nict_init(tsip_transac_nict_t *self)
{
	/* Initialize the state machine.
	*/
	tsip_transac_nictContext_Init(&self->_fsm, self);


	/* To perform Entry state action */
	//tsip_transac_nictContext_EnterStartState(&(self->_fsm));

#if defined(_DEBUG) || defined(DEBUG)
	 setDebugFlag(&(self->_fsm), DEBUG_STATE_MACHINE);
#endif
	
	 /* set timer E */
	 self->timerE.timeout = TSIP_TIMER_GET(E);
}


int tsip_transac_nict_start(tsip_transac_nict_t *self, const tsip_request_t* request)
{
	int ret = -1;
	if(self && request)
	{
		tsip_transac_nictContext_sm_send(&self->_fsm);
		return 0;
	}
	return -1;
}

/* Started -> (send) -> Trying
*/
void tsip_transac_nict_Started_2_Trying_X_send(tsip_transac_nict_t *self)
{
	/* 
		When entering this state, the client transaction SHOULD set timer F to fire in 64*T1 seconds.  The request
		MUST be passed to the transport layer for transmission.  If an unreliable transport is in use, the client transaction MUST set timer
		E to fire in T1 seconds. 
	*/

	//if(tsip_transac_send(TSIP_TRANSAC(self), (const tsip_message_t *)request))
	{
		// what?
	}
	self->timerE.id = TIMER_SCHEDULE(self->timerE.timeout, self);
}

/* Trying -> (Timer E) -> Trying
*/
void tsip_transac_nict_Trying_2_Trying_X_timerE(tsip_transac_nict_t *self)
{

}

/* Trying -> (Timer F) -> Terminated
*/
void tsip_transac_nict_Trying_2_Terminated_X_timerF(tsip_transac_nict_t *self)
{

}

/* Trying -> (Transport Error) -> Terminated
*/
void tsip_transac_nict_Trying_2_Terminated_X_transportError(tsip_transac_nict_t *self)
{

}

/* Trying -> (1xx) -> Proceeding
*/
void tsip_transac_nict_Trying_2_Proceedding_X_1xx(tsip_transac_nict_t *self)
{

}

/* Trying -> (200-699) -> Completed
*/
void tsip_transac_nict_Trying_2_Completed_X_200_to_699(tsip_transac_nict_t *self)
{
}

/* Trying -> (unknown) -> Trying
*/
void tsip_transac_nict_Trying_2_Trying_X_unknown(tsip_transac_nict_t *self)
{
}

/* Proceeding -> (TimerE) -> Proceeding
*/
void tsip_transac_nict_Proceeding_2_Proceeding_X_timerE(tsip_transac_nict_t *self)
{
}

/* Proceeding -> (Timer F) -> Proceeding
*/
void tsip_transac_nict_Proceeding_2_Terminated_X_timerF(tsip_transac_nict_t *self)
{
}

/* Proceeding -> (Transport error) -> Terminated
*/
void tsip_transac_nict_Proceeding_2_Terminated_X_transportError(tsip_transac_nict_t *self)
{
}

/* Proceeding -> (1xx) -> Proceeding
*/
void tsip_transac_nict_Proceeding_2_Proceeding_X_1xx(tsip_transac_nict_t *self)
{
}

/* Proceeding -> (200-699) -> Completed
*/
void tsip_transac_nict_Proceeding_2_Completed_X_200_to_699(tsip_transac_nict_t *self)
{
}

/* Completed -> (Timer K) -> Terminated
*/
void tsip_transac_nict_Completed_2_Terminated_X_timerK(tsip_transac_nict_t *self)
{
}























//========================================================
//	NICT object definition
//
static void* tsip_transac_nict_create(void * self, va_list * app)
{
	tsip_transac_nict_t *transac = self;
	if(transac)
	{
		const tsip_stack_handle_t *stack = va_arg(*app, const tsip_stack_handle_t *);
		tsip_transac_type_t type = va_arg(*app, tsip_transac_type_t);
		unsigned reliable = va_arg(*app, unsigned);
		int32_t cseq_value = va_arg(*app, int32_t);
		const char *cseq_method = va_arg(*app, const char *);
		const char *callid = va_arg(*app, const char *);

		/* Initialize base class */
		tsip_transac_init(TSIP_TRANSAC(transac), stack, type, reliable, cseq_value, cseq_method, callid);

		/* Initialize NICT object */
		tsip_transac_nict_init(transac);
	}
	return self;
}

static void* tsip_transac_nict_destroy(void * self)
{ 
	tsip_transac_nict_t *transac = self;
	if(transac)
	{
		
		/* DeInitialize base class */
		tsip_transac_deinit(TSIP_TRANSAC(transac));
	}
	return self;
}

static int tsip_transac_nict_cmp(const void *obj1, const void *obj2)
{
	return -1;
}

static const tsk_object_def_t tsip_transac_nict_def_s = 
{
	sizeof(tsip_transac_nict_t),
	tsip_transac_nict_create, 
	tsip_transac_nict_destroy,
	tsip_transac_nict_cmp, 
};
const void *tsip_transac_nict_def_t = &tsip_transac_nict_def_s;