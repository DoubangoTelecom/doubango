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

/**@file tsip_dialog.c
 * @brief SIP dialog base class as per RFC 3261 subclause 17.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/dialogs/tsip_dialog.h"

#include "tinysip/dialogs/tsip_dialog_layer.h"
#include "tinysip/transactions/tsip_transac_layer.h"

#include "tinysip/transactions/tsip_transac_nict.h"


tsip_request_t *tsip_dialog_request_new(const tsip_dialog_t *self, const char* method)
{
	tsip_request_t *request = 0;
	tsip_uri_t *to_uri, *from_uri, *request_uri;
	const char *call_id;
	int copy_routes_start = -1; /* NONE */

	/*
	RFC 3261 - 12.2.1.1 Generating the Request

	The Call-ID of the request MUST be set to the Call-ID of the dialog.
	*/
	call_id = self->callid;

	/*
	RFC 3261 - 12.2.1.1 Generating the Request

	Requests within a dialog MUST contain strictly monotonically
	increasing and contiguous CSeq sequence numbers (increasing-by-one)
	in each direction (excepting ACK and CANCEL of course, whose numbers
	equal the requests being acknowledged or cancelled).  Therefore, if
	the local sequence number is not empty, the value of the local
	sequence number MUST be incremented by one, and this value MUST be
	placed into the CSeq header field.
	*/
	/*if(!tsk_striequals(method, "ACK") && !tsk_striequals(method, "CANCEL"))
	{
		TSIP_DIALOG(self)->cseq_value +=1;
	}
	===> See send method (cseq will be incremented before sending the request)
	*/
	

	/*
	RFC 3261 - 12.2.1.1 Generating the Request

	The URI in the To field of the request MUST be set to the remote URI
	from the dialog state.  The tag in the To header field of the request
	MUST be set to the remote tag of the dialog ID.  The From URI of the
	request MUST be set to the local URI from the dialog state.  The tag
	in the From header field of the request MUST be set to the local tag
	of the dialog ID.  If the value of the remote or local tags is null,
	the tag parameter MUST be omitted from the To or From header fields,
	respectively.
	*/
	to_uri = tsk_object_ref((void*)self->uri_remote);
	from_uri = tsk_object_ref((void*)self->uri_local);


	/*
	RFC 3261 - 12.2.1.1 Generating the Request

	If the route set is empty, the UAC MUST place the remote target URI
	into the Request-URI.  The UAC MUST NOT add a Route header field to
	the request.
	*/
	if(TSK_LIST_IS_EMPTY(self->routes))
	{
		request_uri = tsk_object_ref((void*)self->uri_remote_target);
	}

	/*
	RFC 3261 - 12.2.1.1 Generating the Request

	If the route set is not empty, and the first URI in the route set
	contains the lr parameter (see Section 19.1.1), the UAC MUST place
	the remote target URI into the Request-URI and MUST include a Route
	header field containing the route set values in order, including all
	parameters.

	If the route set is not empty, and its first URI does not contain the
	lr parameter, the UAC MUST place the first URI from the route set
	into the Request-URI, stripping any parameters that are not allowed
	in a Request-URI.  The UAC MUST add a Route header field containing
	the remainder of the route set values in order, including all
	parameters.  The UAC MUST then place the remote target URI into the
	Route header field as the last value.

	For example, if the remote target is sip:user@remoteua and the route
	set contains:

	<sip:proxy1>,<sip:proxy2>,<sip:proxy3;lr>,<sip:proxy4>
	*/
	else
	{
		tsip_uri_t *first_route = self->routes->head->data;
		if(tsk_params_has_param(first_route->params, "lr"))
		{
			request_uri = tsk_object_ref(self->uri_remote_target);
			copy_routes_start = 0; /* Copy all */
		}
		else
		{
			request_uri = tsk_object_ref(first_route);
			copy_routes_start = 1; /* Copy starting at index 1. */
		}
	}

	/*=====================================================================
	*/
	request = tsip_request_new(method, request_uri, from_uri, to_uri, call_id, self->cseq_value);
	request->To->tag = tsk_strdup(self->tag_remote);
	request->From->tag = tsk_strdup(self->tag_local);


	/*
	RFC 3261 - 12.2.1.1 Generating the Request

	A UAC SHOULD include a Contact header field in any target refresh
	requests within a dialog, and unless there is a need to change it,
	the URI SHOULD be the same as used in previous requests within the
	dialog.  If the "secure" flag is true, that URI MUST be a SIPS URI.
	As discussed in Section 12.2.2, a Contact header field in a target
	refresh request updates the remote target URI.  This allows a UA to
	provide a new contact address, should its address change during the
	duration of the dialog.
	*/
	{
		char* contact = 0;
		tsk_sprintf(&contact, "m: <%s:%s@%s:%d>;expires=%d;doubs\r\n", /*self->issecure*/0?"sips":"sip", from_uri->user_name, "127.0.0.1", 5060, self->expires);
		request->Contact = tsip_header_Contact_parse(contact, strlen(contact));
		TSK_FREE(contact);
	}




	if(copy_routes_start !=-1)
	{
		// FIXME:
	}

	
	/* P-Access-Network-Info */
	if(TSIP_STACK(self->stack)->netinfo)
	{
		TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_P_ACCESS_NETWORK_INFO_VA_ARGS(TSIP_STACK(self->stack)->netinfo));
	}


	TSIP_URI_SAFE_FREE(request_uri);
	TSIP_URI_SAFE_FREE(from_uri);
	TSIP_URI_SAFE_FREE(to_uri);

	return request;
}


int tsip_dialog_request_send(const tsip_dialog_t *self, tsip_request_t* request)
{
	if(self && self->stack /*&& TSIP_MESSAGE_IS_REQUEST(request)*/)
	{	
		tsip_transac_layer_t *layer = tsip_stack_get_transac_layer(self->stack);
		if(layer)
		{
			const tsip_transac_t *transac = tsip_transac_layer_new(layer, request);
			if(transac)
			{
				/* Set transaction's dialog */
				TSIP_TRANSAC(transac)->dialog = self;

				/* Increment the CSeq value*/
				if(!tsk_striequals(request->line_request.method, "ACK") && !tsk_striequals(request->line_request.method, "CANCEL"))
				{
					request->CSeq->seq = ++(TSIP_DIALOG(self)->cseq_value);
				}

				switch(transac->type)
				{
				case tsip_ict:
					{
						
						break;
					}

				case tsip_nict:
					{
						tsip_transac_nict_start(TSIP_TRANSAC_NICT(transac), request);
						break;
					}

				default:
					{
						return -2;
					}
				}

				return 0;
			}
		}
	}
	return -1;
}

int tsip_dialog_response_send(const tsip_dialog_t *self, tsip_response_t* response)
{
	if(self && self->stack)
	{
		return 0;
	}
	return -1;
}

int tsip_dialog_get_newdelay(tsip_dialog_t *self, const tsip_response_t* response)
{
	int expires = self->expires;
	int newdelay = expires;	/* default value */

	/* 
	*	NOTIFY with subscription-state header with expires parameter. 
	*/
	if(response->CSeq && tsk_striequals(response->CSeq->method, "NOTIFY"))
	{
		// FIXME:
		//expires = tsk_params_get_asint("expires");
		goto compute;
	}

	/*
	*	Exipires header or contact header with expires param?
	*/
	if((expires = tsip_message_getExpires(response)) >0)
	{
		goto compute;
	}

	/*
	*	3GPP TS 24.229 - 
	*
	*	The UE shall reregister the public user identity either 600 seconds before the expiration time if the initial 
	*	registration was for greater than 1200 seconds, or when half of the time has expired if the initial registration 
	*	was for 1200 seconds or less.
	*/
compute:
	newdelay = (expires > 1200) ? (expires-600) : (expires/2);

	return newdelay;
}

int tsip_dialog_update(tsip_dialog_t *self, const tsip_response_t* response)
{
	if(self && TSIP_MESSAGE_IS_RESPONSE(response) && response->To)
	{
		short code = response->line_status.status_code;
		const char *tag = response->To->tag;
		int isRegister = response->CSeq ? tsk_striequals(response->CSeq->method, "REGISTER") : 0;

		/* 
		*	1xx (!100) or 2xx 
		*/
		if(100 <= code || code >= 300)
		{
			tsip_dialog_state_t state = self->state;

			/* 1xx */
			if(response->line_status.status_code <= 199)
			{
				if(tsk_strempty(response->To->tag)) return -1;
				state = tsip_early;
			}
			/* 2xx */
			else
			{
				state = tsip_established;
			}

			/* Remote target */
			if(!isRegister && response->Contact && !TSK_LIST_IS_EMPTY(response->Contact->contacts))
			{
				tsip_contact_t *contact = response->Contact->contacts->head->data;
				TSIP_URI_SAFE_FREE(self->uri_remote_target);
				if(contact->uri)
				{
					self->uri_remote_target = tsip_uri_clone(contact->uri, 0, 0);
				}
			}

			/* Route sets + cseq + tags + ... */
			if(self->state == tsip_established && tsk_striequals(self->tag_remote, tag))
			{
				return 0;
			}
			else
			{
				// FIXME: copy routes (reversed)
				if(!isRegister)
				{

				}

				tsk_strupdate(&self->tag_remote, tag);
				self->cseq_value = response->CSeq ? response->CSeq->seq : self->cseq_value;
			}

			self->state = state;
		}
		
		/*
		*	401 or 407 or 421 or 494
		*/
		else if(code == 401 || code == 407 || code == 421 || code == 494)
		{
			// FIXME:
		}

	}
	return -1;
}

int tsip_dialog_init(tsip_dialog_t *self, tsip_dialog_type_t type, const tsip_stack_handle_t * stack, const char* call_id, const tsip_operation_handle_t* operation)
{
	if(self)
	{
		self->type = type;
		self->stack = stack;
		if(!self->routes)self->routes = TSK_LIST_CREATE();
		self->expires = TSIP_DIALOG_EXPIRES_DEFAULT;
		
		if(call_id)
		{
			tsk_strupdate(&self->callid, call_id);
		}
		else
		{
			tsk_istr_t cid;
			tsip_header_Call_ID_random(&cid);
			tsk_strupdate(&self->callid, cid);
		}

		self->operation = operation;

		/* Local tag */
		{
			tsk_istr_t tag;
			tsk_strrandom(&tag);
			tsk_strupdate(&self->tag_local, tag);
		}

		/* CSeq */
		self->cseq_value = rand();

		return 0;
	}
	return -1;
}

int tsip_dialog_deinit(tsip_dialog_t *self)
{
	if(self && self->stack)
	{
		TSIP_URI_SAFE_FREE(self->uri_local);
		TSK_FREE(self->tag_local);
		TSIP_URI_SAFE_FREE(self->uri_remote);
		TSK_FREE(self->tag_remote);

		TSIP_URI_SAFE_FREE(self->uri_remote_target);
		
		TSK_FREE(self->cseq_method);
		TSK_FREE(self->callid);

		TSK_LIST_SAFE_FREE(self->routes);
		
		return 0;
	}
	return -1;
}

