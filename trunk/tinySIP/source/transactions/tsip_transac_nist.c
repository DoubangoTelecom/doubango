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
								  |Request received
                                  |pass to TU
                                  V
                            +-----------+
                            |           |
                            | Trying    |-------------+
                            |           |             |
                            +-----------+             |200-699 from TU
                                  |                   |send response
                                  |1xx from TU        |
                                  |send response      |
                                  |                   |
               Request            V      1xx from TU  |
               send response+-----------+send response|
                   +--------|           |--------+    |
                   |        | Proceeding|        |    |
                   +------->|           |<-------+    |
            +<--------------|           |             |
            |Trnsprt Err    +-----------+             |
            |Inform TU            |                   |
            |                     |                   |
            |                     |200-699 from TU    |
            |                     |send response      |
            |  Request            V                   |
            |  send response+-----------+             |
            |      +--------|           |             |
            |      |        | Completed |<------------+
            |      +------->|           |
            +<--------------|           |
            |Trnsprt Err    +-----------+
            |Inform TU            |
            |                     |Timer J fires
            |                     |-
            |                     |
            |                     V
            |               +-----------+
            |               |           |
            +-------------->| Terminated|
                            |           |
                            +-----------+ 

=============================================================================*/

/**@file tsip_transac_nist.c
 * @brief SIP Non-INVITE Server Transaction as per RFC 3261 subclause 17.2.2.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/transactions/tsip_transac_nist.h"

#include "tsk_debug.h"

#define DEBUG_STATE_MACHINE						1

#define TRANSAC_NIST_TIMER_SCHEDULE(TX)			TRANSAC_TIMER_SCHEDULE(nist, TX)

int tsip_transac_nist_event_callback(const tsip_transac_nist_t *self, tsip_transac_event_type_t type, const tsip_message_t *msg)
{
	TSIP_TRANSAC_SYNC_BEGIN(self);

	switch(type)
	{
	case tsip_transac_incoming_msg:
		{
			if(msg && TSIP_MESSAGE_IS_REQUEST(msg))
			{
				tsip_transac_nistContext_sm_recv_request(&(TSIP_TRANSAC_NIST(self)->_fsm), msg);
			}
			break;
		}

	case tsip_transac_outgoing_msg:
		{
			if(msg && TSIP_MESSAGE_IS_RESPONSE(msg))
			{
				
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

	TSIP_TRANSAC_SYNC_END(self);

	return 0;
}

int tsip_transac_nist_timer_callback(const tsip_transac_nist_t* self, tsk_timer_id_t timer_id)
{
	int ret = -1;

	if(self)
	{
		TSIP_TRANSAC_SYNC_BEGIN(self);


		TSIP_TRANSAC_SYNC_END(self);
	}

	return ret;
}

void tsip_transac_nist_init(tsip_transac_nist_t *self)
{
	/* Initialize the state machine.
	*/
	tsip_transac_nistContext_Init(&self->_fsm, self);

	/* Set callback function to call when new messages arrive or errors happen in
		the transport layer.
	*/
	TSIP_TRANSAC(self)->callback = tsip_transac_nist_event_callback;

#if defined(_DEBUG) || defined(DEBUG)
	 setDebugFlag(&(self->_fsm), DEBUG_STATE_MACHINE);
#endif

	 self->timerJ.timeout = TSIP_TRANSAC(self)->reliable ? 0 : TSIP_TIMER_GET(J); /* RFC 3261 - 17.2.2*/
}

int tsip_transac_nist_start(tsip_transac_nist_t *self)
{
	return -1;
}


/* Started --> (INCOMING REQUEST) --> Trying
*/
void tsip_transac_nist_Started_2_Trying_X_recv_request(tsip_transac_nist_t *self, const tsip_request_t* request)
{
	TSIP_TRANSAC(self)->dialog->callback(TSIP_TRANSAC(self)->dialog, tsip_dialog_msg, request);
}
												
void tsip_transac_nist_Trying_2_Proceeding_X_send_1xx(tsip_transac_nist_t *self, const tsip_request_t* request)
{

}

void tsip_transac_nist_Trying_2_Completed_X_send_200_to_699(tsip_transac_nist_t *self, const tsip_request_t* request)
{

}

void tsip_transac_nist_Proceeding_2_Proceeding_X_send_1xx(tsip_transac_nist_t *self, const tsip_request_t* request)
{

}

void tsip_transac_nist_Proceeding_2_Completed_X_send_200_to_699(tsip_transac_nist_t *self, const tsip_request_t* request)
{

}

/* Proceeding -> (INCOMING REQUEST) -> Proceeding
*/
void tsip_transac_nist_Proceeding_2_Proceeding_X_recv_request(tsip_transac_nist_t *self, const tsip_request_t* request)
{
	TSIP_TRANSAC(self)->dialog->callback(TSIP_TRANSAC(self)->dialog, tsip_dialog_msg, request);
}

/* Completed -> (INCOMING REQUEST) -> Completed
*/
void tsip_transac_nist_Completed_2_Completed_X_recv_request(tsip_transac_nist_t *self, const tsip_request_t* request)
{
	TSIP_TRANSAC(self)->dialog->callback(TSIP_TRANSAC(self)->dialog, tsip_dialog_msg, request);
}

void tsip_transac_nist_Completed_2_Terminated_X_tirmerJ(tsip_transac_nist_t *self)
{

}

void tsip_transac_nist_Any_2_Terminated_X_transportError(tsip_transac_nist_t *self)
{

}



void tsip_transac_nist_OnTerminated(tsip_transac_nist_t *self)
{
}





















//========================================================
//	NIST object definition
//
static void* tsip_transac_nist_create(void * self, va_list * app)
{
	tsip_transac_nist_t *transac = self;
	if(transac)
	{
		const tsip_stack_handle_t *stack = va_arg(*app, const tsip_stack_handle_t *);
		unsigned reliable = va_arg(*app, unsigned);
		int32_t cseq_value = va_arg(*app, int32_t);
		const char *cseq_method = va_arg(*app, const char *);
		const char *callid = va_arg(*app, const char *);

		/* Initialize base class */
		tsip_transac_init(TSIP_TRANSAC(transac), stack, tsip_nist, reliable, cseq_value, cseq_method, callid);

		/* Initialize NICT object */
		tsip_transac_nist_init(transac);
	}
	return self;
}

static void* tsip_transac_nist_destroy(void * self)
{ 
	tsip_transac_nist_t *transac = self;
	if(transac)
	{
		TSIP_TRANSAC(transac)->running = 0;
		TSIP_RESPONSE_SAFE_FREE(transac->response);

		/* DeInitialize base class */
		tsip_transac_deinit(TSIP_TRANSAC(transac));
	}
	return self;
}

static int tsip_transac_nist_cmp(const void *t1, const void *t2)
{
	return tsip_transac_cmp(t1, t2);
}

static const tsk_object_def_t tsip_transac_nist_def_s = 
{
	sizeof(tsip_transac_nist_t),
	tsip_transac_nist_create, 
	tsip_transac_nist_destroy,
	tsip_transac_nist_cmp, 
};
const void *tsip_transac_nist_def_t = &tsip_transac_nist_def_s;