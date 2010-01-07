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

/**@file tsip_transac_layer.c
 * @brief SIP transaction layer.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/transactions/tsip_transac_layer.h"

#include "tinysip/transactions/tsip_transac_ict.h"
#include "tinysip/transactions/tsip_transac_ist.h"
#include "tinysip/transactions/tsip_transac_nict.h"
#include "tinysip/transactions/tsip_transac_nist.h"

#include "tsk_string.h"


const tsip_transac_t* tsip_transac_layer_new(const tsip_transac_layer_t *self, int isCT, const tsip_message_t* msg)
{
	tsip_transac_t *ret = 0;

	tsk_safeobj_lock(self);

	if(self && msg)
	{
		if(TSIP_MESSAGE_IS_REQUEST(msg))
		{
			if(isCT)
			{
				if(tsk_striequals(TSIP_MESSAGE_AS_REQUEST(msg)->line_request.method, "INVITE"))
				{
					// INVITE Client transaction (ICT)
				}
				else
				{
					// NON-INVITE Client transaction (NICT)
					tsip_transac_nict_t *transac = TSIP_TRANSAC_NICT_CREATE(self->stack, 0, msg->CSeq->seq, msg->CSeq->method, msg->Call_ID->value);
					ret = TSIP_TRANSAC(transac);

					tsk_list_push_back_data(self->transactions, (void**)&transac);
				}
			}
			else
			{
				if(tsk_striequals(TSIP_MESSAGE_AS_REQUEST(msg)->line_request.method, "INVITE"))
				{
					// INVITE Server transaction (IST)
				}
				else
				{
					// NON-INVITE Server transaction (NIST)
					tsip_transac_nist_t *transac = TSIP_TRANSAC_NIST_CREATE(self->stack, 0, msg->CSeq->seq, msg->CSeq->method, msg->Call_ID->value);
					ret = TSIP_TRANSAC(transac);
					
					tsk_list_push_back_data(self->transactions, (void**)&transac);
				}

				if(ret)
				{
					ret->branch = tsk_strdup(msg->firstVia->branch);
				}
			}
		}
	}

	tsk_safeobj_unlock(self);

	return ret;
}

int tsip_transac_layer_remove(tsip_transac_layer_t *self, const tsip_transac_t *transac)
{
	if(transac && self)
	{
		tsk_safeobj_lock(self);
		tsk_list_remove_item_by_data(self->transactions, transac);
		tsk_safeobj_unlock(self);

		return 0;
	}

	return -1;
}

const tsip_transac_t* tsip_transac_layer_find_client(const tsip_transac_layer_t *self, const tsip_response_t* response)
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
	tsip_transac_t *ret = 0;
	tsip_transac_t *transac;
	tsk_list_item_t *item;

	/*	Check first Via/CSeq validity.
	*/
	if(!response->firstVia || !response->CSeq)
	{
		return 0;
	}

	//tsk_safeobj_lock(self);

	tsk_list_foreach(item, self->transactions)
	{
		transac = item->data;
		if( tsk_strequals(transac->branch, response->firstVia->branch) 
			&& tsk_strequals(transac->cseq_method, response->CSeq->method)
			)
		{
			ret = transac;
			break;
		}
	}

	//tsk_safeobj_unlock(self);

	return ret;
}

const tsip_transac_t* tsip_transac_layer_find_server(const tsip_transac_layer_t *self, const tsip_request_t* request)
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
	tsip_transac_t *ret = 0;
	tsip_transac_t *transac;
	tsk_list_item_t *item;
	//const char* sent_by;

	/*	Check first Via/CSeq validity.
	*/
	if(!request->firstVia || !request->CSeq)
	{
		return 0;
	}

	//tsk_safeobj_lock(self);


	tsk_list_foreach(item, self->transactions)
	{
		transac = item->data;
		if(tsk_strequals(transac->branch, request->firstVia->branch) 
			&& (1 == 1) /* FIXME: compare host:ip */
			)
		{
			if(tsk_strequals(transac->cseq_method, TSIP_REQUEST_METHOD(request)))
			{
				ret = transac;
				break;
			}
			else if(tsk_strequals("ACK", TSIP_REQUEST_METHOD(request)) || tsk_strequals("CANCEL", TSIP_REQUEST_METHOD(request)))
			{
				ret = transac;
				break;
			}
		}
	}

	//tsk_safeobj_unlock(self);

	return ret;
}



int tsip_transac_layer_handle_msg(const tsip_transac_layer_t *self, int incoming, const tsip_message_t* message)
{
	int ret = -1;
	const tsip_transac_t *transac = 0;

	tsk_safeobj_lock(self);

	if(TSIP_MESSAGE_IS_REQUEST(message))
	{
		transac = tsip_transac_layer_find_server(self, TSIP_MESSAGE_AS_REQUEST(message));
	}
	else
	{
		transac = tsip_transac_layer_find_client(self, TSIP_MESSAGE_AS_RESPONSE(message));
	}

	if(transac)
	{
		transac->callback(transac, incoming?tsip_transac_incoming_msg:tsip_transac_outgoing_msg, message);
		ret = 0;
	}

	tsk_safeobj_unlock(self);

	return ret;
}











//========================================================
//	Transaction layer object definition
//
static void* tsip_transac_layer_create(void * self, va_list * app)
{
	tsip_transac_layer_t *layer = self;
	if(layer)
	{
		layer->stack = va_arg(*app, const tsip_stack_handle_t *);
		layer->transactions = TSK_LIST_CREATE();

		tsk_safeobj_init(layer);
	}
	return self;
}

static void* tsip_transac_layer_destroy(void * self)
{ 
	tsip_transac_layer_t *layer = self;
	if(layer)
	{
		TSK_LIST_SAFE_FREE(layer->transactions);

		tsk_safeobj_deinit(layer);
	}
	return self;
}

static int tsip_transac_layer_cmp(const void *obj1, const void *obj2)
{
	return -1;
}

static const tsk_object_def_t tsip_transac_layer_def_s = 
{
	sizeof(tsip_transac_layer_t),
	tsip_transac_layer_create, 
	tsip_transac_layer_destroy,
	tsip_transac_layer_cmp, 
};
const void *tsip_transac_layer_def_t = &tsip_transac_layer_def_s;
