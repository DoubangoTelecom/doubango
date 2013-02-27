/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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

/**@file tsip_transac_layer.c
 * @brief SIP transaction layer.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/transactions/tsip_transac_layer.h"

#include "tinysip/transactions/tsip_transac_ict.h"
#include "tinysip/transactions/tsip_transac_ist.h"
#include "tinysip/transactions/tsip_transac_nict.h"
#include "tinysip/transactions/tsip_transac_nist.h"

#include "tsk_string.h"
#include "tsk_debug.h"

tsip_transac_layer_t* tsip_transac_layer_create(tsip_stack_t* stack)
{
	return tsk_object_new(tsip_transac_layer_def_t, stack);
}

tsip_transac_t* tsip_transac_layer_new(const tsip_transac_layer_t *self, tsk_bool_t isCT, const tsip_message_t* msg, tsip_transac_dst_t* dst)
{
	tsip_transac_t *ret = tsk_null;
	tsip_transac_t *transac = tsk_null;

	tsk_safeobj_lock(self);

	if(self && msg)
	{
		if(TSIP_MESSAGE_IS_REQUEST(msg))
		{
			if(isCT)	/* Client transaction */
			{
				if(TSIP_REQUEST_IS_INVITE(msg)){
					// INVITE Client transaction (ICT)
					transac = (tsip_transac_t *)tsip_transac_ict_create(msg->CSeq->seq, msg->Call_ID->value, dst);
				}
				else{
					// NON-INVITE Client transaction (NICT)
					transac = (tsip_transac_t *)tsip_transac_nict_create(msg->CSeq->seq, msg->CSeq->method, msg->Call_ID->value, dst);
				}
			}
			else	/* Server transaction */
			{
				if(TSIP_REQUEST_IS_INVITE(msg)){
					// INVITE Server transaction (IST)
					transac = (tsip_transac_t *)tsip_transac_ist_create(msg->CSeq->seq, msg->Call_ID->value, dst);
				}
				else{
					// NON-INVITE Server transaction (NIST)
					transac = (tsip_transac_t *)tsip_transac_nist_create(msg->CSeq->seq, msg->CSeq->method, msg->Call_ID->value, dst);
				}
				
				if(transac){ /* Copy branch from the message */
					transac->branch = tsk_strdup(msg->firstVia->branch);
				}
			}
			
			/* Add new transaction */
			if(transac){
				ret = tsk_object_ref(transac);
				tsk_list_push_back_data(self->transactions, (void**)&transac);
			}
		}
	}

	tsk_safeobj_unlock(self);

	return ret;
}

int tsip_transac_layer_remove(tsip_transac_layer_t *self, const tsip_transac_t *transac)
{
	if(transac && self){
		tsk_safeobj_lock(self);
		tsk_list_remove_item_by_data(self->transactions, transac);
		tsk_safeobj_unlock(self);

		return 0;
	}

	return -1;
}

/* cancel all transactions related to this dialog */
int tsip_transac_layer_cancel_by_dialog(tsip_transac_layer_t *self, const struct tsip_dialog_s* dialog)
{
	tsk_list_item_t *item;
	int ret = 0; /* Perhaps there is zero transaction */


	if(!self || !dialog){
		TSK_DEBUG_WARN("Invalid parameter.");
		return -1;
	}
	
	tsk_safeobj_lock(self);
again:
	tsk_list_foreach(item, self->transactions){
		if(tsk_object_cmp(dialog, TSIP_TRANSAC_GET_DIALOG(item->data)) == 0){
			if((ret = tsip_transac_fsm_act(TSIP_TRANSAC(item->data), tsip_atype_cancel, tsk_null))){ /* will call tsip_transac_layer_remove() if succeed */
				/* break; */
			}
			else{
				/* we cannot continue because an item has been removed from the list while we are looping through  */
				goto again;
			}
		}
	}
	tsk_safeobj_unlock(self);
	
	return 0;
}

tsip_transac_t* tsip_transac_layer_find_client(const tsip_transac_layer_t *self, const tsip_response_t* response)
{
   /*
   RFC 3261 - 17.1.3 Matching Responses to Client Transactions

   When the transport layer in the client receives a response, it has to
   determine which client transaction will handle the response, so that
   the processing of Sections 17.1.1 and 17.1.2 can take place.  The
   branch parameter in the top Via header field is used for this
   purpose.  A response matches a client transaction under two
   conditions:

      1.  If the response has the same value of the branch parameter in
          the top Via header field as the branch parameter in the top
          Via header field of the request that created the transaction.

      2.  If the method parameter in the CSeq header field matches the
          method of the request that created the transaction.  The
          method is needed since a CANCEL request constitutes a
          different transaction, but shares the same value of the branch
          parameter.
	*/
	tsip_transac_t *ret = tsk_null;
	tsip_transac_t *transac;
	tsk_list_item_t *item;

	/*	Check first Via/CSeq validity.
	*/
	if(!response->firstVia || !response->CSeq){
		return tsk_null;
	}

	tsk_safeobj_lock(self);

	tsk_list_foreach(item, self->transactions){
		transac = item->data;
		if( tsk_strequals(transac->branch, response->firstVia->branch) 
			&& tsk_strequals(transac->cseq_method, response->CSeq->method)
			)
		{
			ret = tsk_object_ref(transac);
			break;
		}
	}

	tsk_safeobj_unlock(self);

	return ret;
}

tsip_transac_t* tsip_transac_layer_find_server(const tsip_transac_layer_t *self, const tsip_message_t* message)
{
	/*
	   RFC 3261 - 17.2.3 Matching Requests to Server Transactions

	   When a request is received from the network by the server, it has to
	   be matched to an existing transaction.  This is accomplished in the
	   following manner.

	   The branch parameter in the topmost Via header field of the request
	   is examined.  If it is present and begins with the magic cookie
	   "z9hG4bK", the request was generated by a client transaction
	   compliant to this specification.  Therefore, the branch parameter
	   will be unique across all transactions sent by that client.  The
	   request matches a transaction if:

		  1. the branch parameter in the request is equal to the one in the
			 top Via header field of the request that created the
			 transaction, and

		  2. the sent-by value in the top Via of the request is equal to the
			 one in the request that created the transaction, and

		  3. the method of the request matches the one that created the
			 transaction, except for ACK, where the method of the request
			 that created the transaction is INVITE.
	*/
	tsip_transac_t *ret = tsk_null;
	tsip_transac_t *transac;
	tsk_list_item_t *item;
	//const char* sent_by;

	/*	Check first Via/CSeq validity */
	if(!message->firstVia || !message->CSeq){
		return tsk_null;
	}

	tsk_safeobj_lock(self);

	tsk_list_foreach(item, self->transactions){
		transac = item->data;
		if(TSIP_REQUEST_IS_ACK(message) && tsk_strequals(transac->callid, message->Call_ID->value)){ /* 1. ACK branch won't match INVITE's but they MUST have the same CSeq/CallId values */
			// [transac->type == tsip_transac_type_ist] is used to avoid looping in webrtc2sip mode (e.g. browser <->(breaker)<->browser)
			// (browser-1) -> INVITE -> (breaker) -> INVITE - (server) -> INVITE -> (breaker) -> (browser-2)
			// the breaker will have two transactions (IST and ICT) with same cseq value and call-id (if not changed by the server)
			if(transac->type == tsip_transac_type_ist && tsk_striequals(transac->cseq_method, "INVITE") && message->CSeq->seq == transac->cseq_value){
				ret = tsk_object_ref(transac);
				break;
			}
		}
		else if(tsk_strequals(transac->branch, message->firstVia->branch) /* 2. Compare branches*/
			&& (1 == 1) /* FIXME: compare host:ip */
			){
			if(tsk_strequals(transac->cseq_method, message->CSeq->method)){
				ret = tsk_object_ref(transac);
				break;
			}
			else if(TSIP_REQUEST_IS_CANCEL(message) || TSIP_RESPONSE_IS_TO_CANCEL(message)){
				ret = tsk_object_ref(transac);
				break;
			}
		}
	}

	tsk_safeobj_unlock(self);

	return ret;
}



/**
 * @fn	int tsip_transac_layer_handle_incoming_msg(const tsip_transac_layer_t *self, const tsip_message_t* message)
 *
 * @brief	Handles SIP/IMS message incoming from the transport layer.
 *
 * @author	Mamadou
 * @date	1/8/2010
 *
 * @param [in,out]	self	The transaction layer.
 * @param [in,out]	message	The SIP/IMS message to handle.
 *
 * @return	Zero if a matching transaction have been found and non-zero result code otherwise.
**/
int tsip_transac_layer_handle_incoming_msg(const tsip_transac_layer_t *self, const tsip_message_t* message)
{
	int ret = -1;
	tsip_transac_t *transac = tsk_null;

	if(!message){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	//tsk_safeobj_lock(self);

	if(TSIP_MESSAGE_IS_REQUEST(message)){
		transac = tsip_transac_layer_find_server(self, /*TSIP_MESSAGE_AS_REQUEST*/(message));
	}
	else{
		transac = tsip_transac_layer_find_client(self, /*TSIP_MESSAGE_AS_RESPONSE*/(message));
	}

	//tsk_safeobj_unlock(self);
	
	if(transac){
		ret = transac->callback(transac, tsip_transac_incoming_msg, message);
		tsk_object_unref(transac);
	}

	return ret;
}











//========================================================
//	Transaction layer object definition
//
static tsk_object_t* tsip_transac_layer_ctor(tsk_object_t * self, va_list * app)
{
	tsip_transac_layer_t *layer = self;
	if(layer){
		layer->stack = va_arg(*app, const tsip_stack_handle_t *);
		layer->transactions = tsk_list_create();

		tsk_safeobj_init(layer);
	}
	return self;
}

static tsk_object_t* tsip_transac_layer_dtor(tsk_object_t * self)
{ 
	tsip_transac_layer_t *layer = self;
	if(layer){
		TSK_OBJECT_SAFE_FREE(layer->transactions);

		tsk_safeobj_deinit(layer);

		TSK_DEBUG_INFO("*** Transaction Layer destroyed ***");
	}
	return self;
}

static int tsip_transac_layer_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	return -1;
}

static const tsk_object_def_t tsip_transac_layer_def_s = 
{
	sizeof(tsip_transac_layer_t),
	tsip_transac_layer_ctor, 
	tsip_transac_layer_dtor,
	tsip_transac_layer_cmp, 
};
const tsk_object_def_t *tsip_transac_layer_def_t = &tsip_transac_layer_def_s;
