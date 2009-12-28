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

/*=============================================================================

                                   |Request from TU
                                   |send request
               Timer E             V
               send request  +-----------+
                   +---------|           |-------------------+
                   |         |  Trying   |  Timer F          |
                   +-------->|           |  or Transport Err.|
                             +-----------+  inform TU        |
                200-699         |  |                         |
                resp. to TU     |  |1xx                      |
                +---------------+  |resp. to TU              |
                |                  |                         |
                |   Timer E        V       Timer F           |
                |   send req +-----------+ or Transport Err. |
                |  +---------|           | inform TU         |
                |  |         |Proceeding |------------------>|
                |  +-------->|           |-----+             |
                |            +-----------+     |1xx          |
                |              |      ^        |resp to TU   |
                | 200-699      |      +--------+             |
                | resp. to TU  |                             |
                |              |                             |
                |              V                             |
                |            +-----------+                   |
                |            |           |                   |
                |            | Completed |                   |
                |            |           |                   |
                |            +-----------+                   |
                |              ^   |                         |
                |              |   | Timer K                 |
                +--------------+   | -                       |
                                   |                         |
                                   V                         |
             NOTE:           +-----------+                   |
                             |           |                   |
         transitions         | Terminated|<------------------+
         labeled with        |           |
         the event           +-----------+
         over the action

=============================================================================*/

/**@file tsip_transac_nict.c
 * @brief SIP Non-INVITE Client Transaction as per RFC 3261 subcaluse 17.1.2.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/transactions/tsip_transac_nict.h"

#include "tinysip/transactions/tsip_transac_layer.h"

#include "tsk_debug.h"

#define DEBUG_STATE_MACHINE						1

#define TRANSAC_NICT_TIMER_SCHEDULE(TX)			TRANSAC_TIMER_SCHEDULE(nict, TX)

/*== Transaction event callback function.
*/
int tsip_transac_nict_event_callback(const void *arg, tsip_transac_event_type_t type, const tsip_message_t *msg)
{
	const tsip_transac_nict_t* transac = arg;
	switch(type)
	{
	case tsip_transac_msg:
		{
			if(msg && TSIP_MESSAGE_IS_RESPONSE(msg))
			{
				short status_code = msg->line_status.status_code;
				if(status_code <=199)
				{
					tsip_transac_nictContext_sm_1xx(&TSIP_TRANSAC_NICT(transac)->_fsm, msg);
				}
				else if(status_code<=699)
				{
					tsip_transac_nictContext_sm_200_to_699(&TSIP_TRANSAC_NICT(transac)->_fsm, msg);
				}
				else
				{
					TSK_DEBUG_WARN("Not supported status code: %d", status_code);
				}
			}
			break;
		}

	case tsip_transac_canceled:
		{
			break;
		}

	case tsip_transac_terminated:
		{
			break;
		}

	case tsip_transac_timedout:
		{
			break;
		}

	case tsip_transac_error:
		{
			break;
		}

	case tsip_transac_transport_error:
		{
			break;
		}
	}

	return 0;
}

/**
 * @fn	void tsip_transac_nict_init(tsip_transac_nict_t *self)
 *
 * @brief	Initializes the transaction.
 *
 * @author	Mamadou
 * @date	12/24/2009
 *
 * @param [in,out]	self	The transaction to initialize.
**/
void tsip_transac_nict_init(tsip_transac_nict_t *self)
{
	/* Initialize the state machine.
	*/
	tsip_transac_nictContext_Init(&self->_fsm, self);


	/* To perform Entry state action */
	//tsip_transac_nictContext_EnterStartState(&(self->_fsm));

	/* Set callback function */
	self->callback = tsip_transac_nict_event_callback;

#if defined(_DEBUG) || defined(DEBUG)
	 setDebugFlag(&(self->_fsm), DEBUG_STATE_MACHINE);
#endif
	 
	 self->timerE.id = TSK_INVALID_TIMER_ID;
	 self->timerF.id = TSK_INVALID_TIMER_ID;
	 self->timerK.id = TSK_INVALID_TIMER_ID;

	 self->timerE.timeout = TSIP_TIMER_GET(E);
	 self->timerF.timeout = TSIP_TIMER_GET(F);
	 self->timerK.timeout = self->reliable ? 0 : TSIP_TIMER_GET(K); /* RFC 3261 - 17.1.2.2 [Proceeding_2_Completed_X_200_to_699]*/
}


/**
 * @fn	int tsip_transac_nict_start(tsip_transac_nict_t *self, tsip_request_t* request)
 *
 * @brief	Starts the client transaction.
 *
 * @author	Mamadou
 * @date	12/24/2009
 *
 * @param [in,out]	self	The client transaction to start. 
 * @param [in,out]	request	The request to send. 
 *
 * @return	Zero if succeed and non-zero error code otherwise. 
**/
int tsip_transac_nict_start(tsip_transac_nict_t *self, tsip_request_t* request)
{
	int ret = -1;
	if(self && request && !self->running)
	{
		/* Set state machine state to started */
		setState(&self->_fsm, &tsip_transac_nict_Started);

		/* Add branch to the new client transaction. */
		self->branch = tsk_strdup(TSIP_TRANSAC_MAGIC_COOKIE);
		{
			tsk_istr_t branch;
			tsk_strrandom(&branch);
			tsk_strcat(&(self->branch), branch);
		}

		self->running = 1;
		self->request = tsk_object_ref(request);
		tsip_transac_nictContext_sm_send(&self->_fsm);

		return 0;
	}
	return -1;
}

//#include "tsk_thread.h"
void tsip_transac_nict_OnTerminated(tsip_transac_nict_t *self)
{
	/* Cancel timers */
	if(!self->reliable)
	{
		TRANSAC_TIMER_CANCEL(E);
	}
	TRANSAC_TIMER_CANCEL(F);
	TRANSAC_TIMER_CANCEL(K);

	self->running = 0;

	// FIXME:
	
	//tsk_timer_manager_debug(TSIP_STACK(self->stack)->timer_mgr);

	// FIXME:
	//if(tsip_transac_layer_remove(TSIP_STACK(self->stack)->layer_transac, TSIP_TRANSAC(self)))
	{
		// ??? 
	}

	
}








//========================================================
//	State Machine
//

/* Started -> (send) -> Trying
*/
void tsip_transac_nict_Started_2_Trying_X_send(tsip_transac_nict_t *self)
{
	//== Send the request
	tsip_transac_send(TSIP_TRANSAC(self), self->branch, TSIP_MESSAGE(self->request));

	/*	RFC 3261 - 17.1.2.2
		The "Trying" state is entered when the TU initiates a new client
		transaction with a request.  When entering this state, the client
		transaction SHOULD set timer F to fire in 64*T1 seconds.
	*/
	TRANSAC_NICT_TIMER_SCHEDULE(F);
		
	/*	RFC 3261 - 17.1.2.2
		If an  unreliable transport is in use, the client transaction MUST set timer
		E to fire in T1 seconds.
	*/
	if(!self->reliable)
	{
		TRANSAC_NICT_TIMER_SCHEDULE(E);
	}
}

/* Trying -> (Timer E) -> Trying
*/
void tsip_transac_nict_Trying_2_Trying_X_timerE(tsip_transac_nict_t *self)
{
	//== Send the request
	tsip_transac_send(TSIP_TRANSAC(self), self->branch, self->request);

	/*	RFC 3261 - 17.1.2.2
		If timer E fires while still in this (Trying) state, the timer is reset, but this time with a value of MIN(2*T1, T2).
		When the timer fires again, it is reset to a MIN(4*T1, T2).  This process continues so that retransmissions occur with an exponentially
	    increasing interval that caps at T2.  The default value of T2 is 4s, and it represents the amount of time a non-INVITE server transaction
	    will take to respond to a request, if it does not respond immediately.  For the default values of T1 and T2, this results in
	    intervals of 500 ms, 1 s, 2 s, 4 s, 4 s, 4 s, etc.
	*/
	self->timerE.timeout = TSK_MIN(self->timerE.timeout*2, TSIP_TIMER_GET(T2));
	TRANSAC_NICT_TIMER_SCHEDULE(E);
}

/* Trying -> (Timer F) -> Terminated
*/
void tsip_transac_nict_Trying_2_Terminated_X_timerF(tsip_transac_nict_t *self)
{
	/*	RFC 3261 - 17.1.2.2
		If Timer F fires while the client transaction is still in the
		"Trying" state, the client transaction SHOULD inform the TU about the
		timeout, and then it SHOULD enter the "Terminated" state.
	*/

	/* Timers will be canceled by "tsip_transac_nict_OnTerminated" */
	
	self->dialog->callback(self->dialog, tsip_dialog_transport_error, 0);
}

/* Trying -> (Transport Error) -> Terminated
*/
void tsip_transac_nict_Trying_2_Terminated_X_transportError(tsip_transac_nict_t *self)
{
	/* Timers will be canceled by "tsip_transac_nict_OnTerminated" */

	self->dialog->callback(self->dialog, tsip_dialog_transport_error, 0);
}

/* Trying -> (1xx) -> Proceeding
*/
void tsip_transac_nict_Trying_2_Proceedding_X_1xx(tsip_transac_nict_t *self, const tsip_message_t* msg)
{
	/*	RFC 3261 - 17.1.2.2
		If a provisional response is received while in the "Trying" state, the
		response MUST be passed to the TU, and then the client transaction
		SHOULD move to the "Proceeding" state.
	*/

	/* Cancel timers */
	if(!self->reliable)
	{
		TRANSAC_TIMER_CANCEL(E);
	}
	TRANSAC_TIMER_CANCEL(F);
	
	self->dialog->callback(self->dialog, tsip_dialog_msg, msg);
}

/* Trying -> (200-699) -> Completed
*/
void tsip_transac_nict_Trying_2_Completed_X_200_to_699(tsip_transac_nict_t *self, const tsip_message_t* msg)
{
	/*	RFC 3261 - 17.1.2.2
		If a final response (status codes 200-699) is received while in the "Trying" state, the response
		MUST be passed to the TU, and the client transaction MUST transition
		to the "Completed" state.

		If Timer K fires while in this state (Completed), the client transaction MUST transition to the "Terminated" state.
	*/

	if(!self->reliable)
	{
		TRANSAC_TIMER_CANCEL(E);
	}
	TRANSAC_TIMER_CANCEL(F);

	self->dialog->callback(self->dialog, tsip_dialog_msg, msg);

	/* SCHEDULE timer K */
	TRANSAC_NICT_TIMER_SCHEDULE(K);
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
	//== Send the request
	tsip_transac_send(TSIP_TRANSAC(self), self->branch, self->request);

	/*	RFC 3261 - 17.1.2.2
		If Timer E fires while in the "Proceeding" state, the request MUST be
		passed to the transport layer for retransmission, and Timer E MUST be
		reset with a value of T2 seconds.
	*/
	self->timerE.timeout = TSK_MIN(self->timerE.timeout*2, TSIP_TIMER_GET(T2));
	TRANSAC_NICT_TIMER_SCHEDULE(E);
}

/* Proceeding -> (Timer F) -> Proceeding
*/
void tsip_transac_nict_Proceeding_2_Terminated_X_timerF(tsip_transac_nict_t *self)
{
	/*	RFC 3261 - 17.1.2.2
		If timer F fires while in the "Proceeding" state, the TU MUST be informed of a timeout, and the
		client transaction MUST transition to the terminated state.
	*/

	/* Timers will be canceled by "tsip_transac_nict_OnTerminated" */

	self->dialog->callback(self->dialog, tsip_dialog_transport_error, 0);
}

/* Proceeding -> (Transport error) -> Terminated
*/
void tsip_transac_nict_Proceeding_2_Terminated_X_transportError(tsip_transac_nict_t *self)
{
	/* Timers will be canceles by On */
	self->dialog->callback(self->dialog, tsip_dialog_transport_error, 0);
}

/* Proceeding -> (1xx) -> Proceeding
*/
void tsip_transac_nict_Proceeding_2_Proceeding_X_1xx(tsip_transac_nict_t *self, const tsip_message_t* msg)
{
	if(!self->reliable)
	{
		TRANSAC_TIMER_CANCEL(E);
	}
	self->dialog->callback(self->dialog, tsip_dialog_msg, msg);
}

/* Proceeding -> (200-699) -> Completed
*/
void tsip_transac_nict_Proceeding_2_Completed_X_200_to_699(tsip_transac_nict_t *self, const tsip_message_t* msg)
{
	/*	RFC 3261 - 17.1.2.2
		If a final response (status codes 200-699) is received while in the
		"Proceeding" state, the response MUST be passed to the TU, and the
		client transaction MUST transition to the "Completed" state.
	*/

	/*	RFC 3261 - 17.1.2.2
		Once the client transaction enters the "Completed" state, it MUST set
		Timer K to fire in T4 seconds for unreliable transports, and zero
		seconds for reliable transports.  The "Completed" state exists to
		buffer any additional response retransmissions that may be received
		(which is why the client transaction remains there only for

		unreliable transports).  T4 represents the amount of time the network
		will take to clear messages between client and server transactions.
		The default value of T4 is 5s.
	*/

	if(!self->reliable)
	{
		TRANSAC_TIMER_CANCEL(E);
	}

	self->dialog->callback(self->dialog, tsip_dialog_msg, msg);
	
	/* SCHEDULE timer K */
	TRANSAC_NICT_TIMER_SCHEDULE(K);
}

/* Completed -> (Timer K) -> Terminated
*/
void tsip_transac_nict_Completed_2_Terminated_X_timerK(tsip_transac_nict_t *self)
{
	/*	RFC 3261 - 17.1.2.2
		If Timer K fires while in this state (Completed), the client transaction
		MUST transition to the "Terminated" state.
	*/
	
	/*	RFC 3261 - 17.1.2.2
		ONCE THE TRANSACTION IS IN THE TERMINATED STATE, IT MUST BE DESTROYED IMMEDIATELY.
	*/

	/* Timers will be canceled by "tsip_transac_nict_OnTerminated" */

	self->dialog->callback(self->dialog, tsip_dialog_transac_ok, 0);
}

/* Any -> (Transport Error) -> Terminated
*/
void tsip_transac_nict_Any_2_Terminated_X_transportError(tsip_transac_nict_t *self)
{
	/* Timers will be canceled by "tsip_transac_nict_OnTerminated" */

	self->dialog->callback(self->dialog, tsip_dialog_transport_error, 0);
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
		unsigned reliable = va_arg(*app, unsigned);
		int32_t cseq_value = va_arg(*app, int32_t);
		const char *cseq_method = va_arg(*app, const char *);
		const char *callid = va_arg(*app, const char *);

		/* Initialize base class */
		tsip_transac_init(TSIP_TRANSAC(transac), stack, tsip_nict, reliable, cseq_value, cseq_method, callid);

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
		transac->running = 0;
		TSIP_REQUEST_SAFE_FREE(transac->request);

		/* DeInitialize base class */
		tsip_transac_deinit(TSIP_TRANSAC(transac));
	}
	return self;
}

static int tsip_transac_nict_cmp(const void *t1, const void *t2)
{
	return tsip_transac_cmp(t1, t2);
}

static const tsk_object_def_t tsip_transac_nict_def_s = 
{
	sizeof(tsip_transac_nict_t),
	tsip_transac_nict_create, 
	tsip_transac_nict_destroy,
	tsip_transac_nict_cmp, 
};
const void *tsip_transac_nict_def_t = &tsip_transac_nict_def_s;
