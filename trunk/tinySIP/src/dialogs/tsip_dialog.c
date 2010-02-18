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

#include "tinysip/headers/tsip_header_Authorization.h"
#include "tinysip/headers/tsip_header_Contact.h"
#include "tinysip/headers/tsip_header_Expires.h"
#include "tinysip/headers/tsip_header_Proxy_Authenticate.h"
#include "tinysip/headers/tsip_header_Proxy_Authorization.h"
#include "tinysip/headers/tsip_header_Route.h"
#include "tinysip/headers/tsip_header_Subscription_State.h"
#include "tinysip/headers/tsip_header_WWW_Authenticate.h"

#include "tsk_debug.h"

int tsip_dialog_update_challenges(tsip_dialog_t *self, const tsip_response_t* response, int acceptNewVector);

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
	if(!self->routes || TSK_LIST_IS_EMPTY(self->routes))
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
		tsip_header_Contacts_L_t *hdr_contacts;
		tsk_sprintf(&contact, "m: <%s:%s@%s:%d>;expires=%d;doubs\r\n", /*self->issecure*/0?"sips":"sip", from_uri->user_name, "127.0.0.1", 5060, self->expires);
		hdr_contacts = tsip_header_Contact_parse(contact, strlen(contact));
		if(!TSK_LIST_IS_EMPTY(hdr_contacts))
		{
			request->Contact = tsk_object_ref(hdr_contacts->head->data);
		}
		TSK_OBJECT_SAFE_FREE(hdr_contacts);
		TSK_FREE(contact);
	}
	
	/* P-Access-Network-Info */
	if(TSIP_STACK(self->stack)->netinfo)
	{
		TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_P_ACCESS_NETWORK_INFO_VA_ARGS(TSIP_STACK(self->stack)->netinfo));
	}
	
	/* Update authorizations */
	if(TSK_LIST_IS_EMPTY(self->challenges))
	{
		if(tsk_striequals("REGISTER", method) && !TSIP_STACK(self->stack)->enable_earlyIMS)
		{
			/*	3GPP TS 34.229 - 5.1.1.1A === 3GPP TS 33.978 - 6.2.3.1
				On sending a REGISTER request, the UE shall populate the header fields as follows:
					a)	the Authorization header, with:
					-	the username directive, set to the value of the private user identity;
					-	the realm directive, set to the domain name of the home network;
					-	the uri directive, set to the SIP URI of the domain name of the home network;
					-	the nonce directive, set to an empty value; and
					-	the response directive, set to an empty value.
			*/
			tsip_header_t* auth_hdr = tsip_challenge_create_empty_header_authorization("sip:fixme@micromethod.com", "fixme:realm", "uri");
			tsip_message_add_header(request, auth_hdr);
			tsk_object_unref(auth_hdr), auth_hdr = 0;
		}
	}
	else
	{
		tsk_list_item_t *item;
		tsip_challenge_t *challenge;
		tsip_header_t* auth_hdr;
		tsk_list_foreach(item, self->challenges)
		{
			challenge = item->data;
			auth_hdr = tsip_challenge_create_header_authorization(challenge, request);
			if(auth_hdr)
			{
				tsip_message_add_header(request, auth_hdr);
				tsk_object_unref(auth_hdr), auth_hdr = 0;
			}
		}
	}

	/* Update CSeq */
	if(!tsk_striequals(method, "ACK") && !tsk_striequals(method, "CANCEL"))
	{
		request->CSeq->seq = ++(TSIP_DIALOG(self)->cseq_value);
	}

	/* Route generation 
		*	==> http://betelco.blogspot.com/2008/11/proxy-and-service-route-discovery-in.html
		* The dialog Routes have been copied above.
	*/
	if(!tsk_striequals("REGISTER", method))
	{	// According to the above link ==> Initial/Re/De registration do not have routes.
		tsk_list_item_t* item;
		if(copy_routes_start !=-1)
		{	/* The dialog already have routes ==> copy them. */
			if(self->state == tsip_early || self->state == tsip_established)
			{
				int32_t index = -1;
				tsk_list_foreach(item, self->routes)
				{
					const tsip_uri_t* uri = item->data;
					if(++index < copy_routes_start || !uri){
						continue;
					}
					TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_ROUTE_VA_ARGS(uri));
				}
			}
		}
		else
		{	/* No routes associated to this dialog. */
			if(self->state == tsip_initial || self->state == tsip_early)
			{
				tsip_uri_t *uri = tsip_stack_get_pcscf_uri(TSIP_DIALOG_GET_STACK(self), 1);
				// Proxy-CSCF as first route
				if(uri){
					TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_ROUTE_VA_ARGS(uri));
					TSK_OBJECT_SAFE_FREE(uri);
				}
				// Service routes
				tsk_list_foreach(item, TSIP_DIALOG_GET_STACK(self)->service_routes)
				{
					TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_ROUTE_VA_ARGS(item->data));
				}
			}
		}
	}


	TSK_OBJECT_SAFE_FREE(request_uri);
	TSK_OBJECT_SAFE_FREE(from_uri);
	TSK_OBJECT_SAFE_FREE(to_uri);

	return request;
}


/**
 * @fn	int tsip_dialog_request_send(const tsip_dialog_t *self, tsip_request_t* request)
 *
 * @brief	Sends a SIP/IMS request. This function is responsible for transaction creation.
 *
 * @author	Mamadou
 * @date	1/4/2010
 *
 * @param [in,out]	self	The parent dialog. All callback events will be notified to this dialog.
 * @param [in,out]	request	The request to send.
 *
 * @return	Zero if succeed and no-zero error code otherwise. 
**/
int tsip_dialog_request_send(const tsip_dialog_t *self, tsip_request_t* request)
{
	if(self && self->stack)
	{	
		const tsip_transac_layer_t *layer = tsip_stack_get_transac_layer(self->stack);
		if(layer)
		{
			/*	Create new transaction. The new transaction will be added to the dialog layer. 
				The transaction has all information to create the right transaction type (NICT or ICT).
				As this is an outgoing request ==> It shall be a client transaction (NICT or ICT).
				For server transactions creation see @ref tsip_dialog_response_send.
			*/
			/*const*/ tsip_transac_t *transac = tsip_transac_layer_new(layer, 1, request);

			/* Set the transaction's dialog. All events comming from the transaction (timeouts, errors ...) will be signaled to this dialog.
			*/
			/*TSIP_TRANSAC*/(transac)->dialog = self;
			if(transac)
			{
				switch(transac->type)
				{
				case tsip_ict:
					{
						/* Start the newly create IC transaction.
						*/
						
						break;
					}

				case tsip_nict:
					{
						/* Start the newly created NIC transaction.
						*/
						tsip_transac_start(transac, request);
						break;
					}

				default: return -1;
				}

				return 0;
			}
		}
	}
	return -1;
}

tsip_response_t *tsip_dialog_response_new(const tsip_dialog_t *self, short status, const char* phrase, const tsip_request_t* request)
{
	/* Reponse is created as per RFC 3261 subclause 8.2.6 and (headers+tags) are copied
	* as per subclause 8.2.6.2.
	*/
	tsip_response_t* response = tsip_response_new(status, phrase, request);
	return response;
}

int tsip_dialog_response_send(const tsip_dialog_t *self, tsip_response_t* response)
{
	int ret = -1;

	if(self && self->stack)
	{
		const tsip_transac_layer_t *layer = tsip_stack_get_transac_layer(self->stack);
		if(layer)
		{
			/* As this is a response ...then there should be a matching server transaction.
			*/
			const tsip_transac_t *transac = tsip_transac_layer_find_server(layer, response);
			if(transac)
			{
				ret = transac->callback(transac, tsip_transac_outgoing_msg, response);
			}
		}
	}
	return ret;
}

/**
 * @fn	int tsip_dialog_get_newdelay(tsip_dialog_t *self, const tsip_response_t* response)
 *
 * @brief	Gets the number of milliseconds to wait before retransmission.
 *			e.g. ==> delay before refreshing registrations (REGISTER), subscribtions (SUBSCRIBE), publication (PUBLISH) ...
 *
 * @author	Mamadou
 * @date	1/4/2010
 *
 * @param [in,out]	self		The calling dialog.
 * @param [in,out]	response	The SIP/IMS response containing the new delay (expires, subscription-state ...).
 *
 * @return	Zero if succeed and no-zero error code otherwise. 
**/
int tsip_dialog_get_newdelay(tsip_dialog_t *self, const tsip_response_t* response)
{
	int expires = self->expires;
	int newdelay = expires;	/* default value */
	const tsip_header_t* hdr;
	size_t i;

	/*== NOTIFY with subscription-state header with expires parameter. 
	*/
	if(response->CSeq && tsk_striequals(response->CSeq->method, "NOTIFY")){
		const tsip_header_Subscription_State_t *hdr_state;
		if((hdr_state = (const tsip_header_Subscription_State_t*)tsip_message_get_header(response, tsip_htype_Subscription_State))){
			if(hdr_state->expires >0){
				expires = hdr_state->expires;
				goto compute;
			}
		}
	}

	/*== Expires header.
	*/
	if((hdr = tsip_message_get_header(response, tsip_htype_Expires))){
		expires = ((const tsip_header_Expires_t*)hdr)->delta_seconds;
		goto compute;
	}

	/*== Contact header.
	*/
	for(i=0; (hdr = tsip_message_get_headerAt(response, tsip_htype_Contact, i)); i++){
		const tsip_header_Contact_t* contact = (const tsip_header_Contact_t*)hdr;
		if(contact && contact->uri)
		{
			tsip_uri_t* contactUri = tsip_stack_get_contacturi(self->stack, tsk_params_get_param_value(contact->uri->params, "transport"));
			if(contactUri)
			{
				if(tsk_strequals(contact->uri->user_name, contactUri->user_name)
					&& tsk_strequals(contact->uri->host, contactUri->host)
					&& contact->uri->port == contactUri->port)
				{
					if(contact->expires>=0){ /* No expires parameter ==> -1*/
						expires = contact->expires;

						TSK_OBJECT_SAFE_FREE(contactUri);
						goto compute;
					}
				}
				TSK_OBJECT_SAFE_FREE(contactUri);
			}
		}
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

/**
 * @fn	int tsip_dialog_update(tsip_dialog_t *self, const tsip_response_t* response)
 *
 * @brief	Updates the dialog state:
 *			- Authorizations (using challenges from the @ref response message)
 *			- State (early, established, disconnected, ...)
 *			- Routes (and Service-Route)
 *			- Target (remote)
 *			- ...
 *
 * @author	Mamadou
 * @date	1/4/2010
 *
 * @param [in,out]	self		The calling dialog.
 * @param [in,out]	response	The SIP/IMS response from which to get the new information. 
 *
 * @return	Zero if succeed and no-zero error code otherwise. 
**/
int tsip_dialog_update(tsip_dialog_t *self, const tsip_response_t* response)
{
	if(self && TSIP_MESSAGE_IS_RESPONSE(response) && response->To)
	{
		short code = response->status_code;
		const char *tag = response->To->tag;
		int isRegister = response->CSeq ? tsk_striequals(response->CSeq->method, "REGISTER") : 0;

		/* 
		*	1xx (!100) or 2xx 
		*/
		/*
		*	401 or 407 or 421 or 494
		*/
		if(code == 401 || code == 407 || code == 421 || code == 494)
		{
			int acceptNewVector;

			/* 3GPP IMS - Each authentication vector is used only once.
			*	==> Re-registration/De-registration ==> Allow 401/407 challenge.
			*/
			acceptNewVector = (isRegister && self->state == tsip_established);
			return tsip_dialog_update_challenges(self, response, acceptNewVector);
		}
		else if(100 < code && code < 300)
		{
			tsip_dialog_state_t state = self->state;

			/* 1xx */
			if(TSIP_RESPONSE_CODE(response) <= 199)
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
			if(!isRegister && response->Contact)
			{
				TSK_OBJECT_SAFE_FREE(self->uri_remote_target);
				if(response->Contact->uri)
				{
					self->uri_remote_target = tsip_uri_clone(response->Contact->uri, 0, 0);
				}
			}

			/* Route sets 
			*/
			{
				size_t index;
				const tsip_header_t *hdr;

				TSK_OBJECT_SAFE_FREE(self->routes);

				for(index = 0; (hdr = tsip_message_get_headerAt(response, tsip_htype_Record_Route, index)); index++){
					if(!self->routes){
						self->routes = TSK_LIST_CREATE();
					}
					hdr = tsk_object_ref((void*)hdr);
					tsk_list_push_front_data(self->routes, (void**)&hdr); /* Copy reversed. */
				}
			}
			

			/* cseq + tags + ... */
			if(self->state == tsip_established && tsk_striequals(self->tag_remote, tag)){
				return 0;
			}
			else{
				if(!isRegister){

				}

				tsk_strupdate(&self->tag_remote, tag);
				self->cseq_value = response->CSeq ? response->CSeq->seq : self->cseq_value;
			}

			self->state = state;
			return 0;
		}
	}
	return -1;
}

int tsip_dialog_update_challenges(tsip_dialog_t *self, const tsip_response_t* response, int acceptNewVector)
{
	int ret = -1;
	size_t i;

	tsk_list_item_t *item;

	tsip_challenge_t *challenge;
	
	const tsip_header_WWW_Authenticate_t *WWW_Authenticate;
	const tsip_header_Proxy_Authenticate_t *Proxy_Authenticate;

	/* RFC 2617 - Digest Operation

	*	(A) The client response to a WWW-Authenticate challenge for a protection
		space starts an authentication session with that protection space.
		The authentication session lasts until the client receives another
		WWW-Authenticate challenge from any server in the protection space.

		(B) The server may return a 401 response with a new nonce value, causing the client
		to retry the request; by specifying stale=TRUE with this response,
		the server tells the client to retry with the new nonce, but without
		prompting for a new username and password.
	*/

	/* FIXME: As we perform the same task ==> Use only one loop.
	*/

	for(i =0; (WWW_Authenticate = (const tsip_header_WWW_Authenticate_t*)tsip_message_get_headerAt(response, tsip_htype_WWW_Authenticate, i)); i++)
	{
		int isnew = 1;

		tsk_list_foreach(item, self->challenges)
		{
			challenge = item->data;
			if(challenge->isproxy) continue;
			
			if(tsk_strequals(challenge->realm, WWW_Authenticate->realm) && (WWW_Authenticate->stale || acceptNewVector))
			{
				/*== (B) ==*/
				if((ret = tsip_challenge_update(challenge, 
					WWW_Authenticate->scheme, 
					WWW_Authenticate->realm, 
					WWW_Authenticate->nonce, 
					WWW_Authenticate->opaque, 
					WWW_Authenticate->algorithm, 
					WWW_Authenticate->qop)))
				{
					return ret;
				}
				else
				{
					isnew = 0;
					continue;
				}
			}
			else return -1;
		}

		if(isnew)
		{
			if((challenge = TSIP_CHALLENGE_CREATE(self->stack,
					0, 
					WWW_Authenticate->scheme, 
					WWW_Authenticate->realm, 
					WWW_Authenticate->nonce, 
					WWW_Authenticate->opaque, 
					WWW_Authenticate->algorithm, 
					WWW_Authenticate->qop)))
			{
				tsk_list_push_back_data(self->challenges, (void**)&challenge);
			}
			else return -1;
		}
	}
	
	for(i=0; (Proxy_Authenticate = (const tsip_header_Proxy_Authenticate_t*)tsip_message_get_headerAt(response, tsip_htype_Proxy_Authenticate, i)); i++)
	{
		int isnew = 1;

		tsk_list_foreach(item, self->challenges)
		{
			challenge = item->data;
			if(!challenge->isproxy) continue;
			
			if(tsk_strequals(challenge->realm, Proxy_Authenticate->realm) && (Proxy_Authenticate->stale || acceptNewVector))
			{
				/*== (B) ==*/
				if((ret = tsip_challenge_update(challenge, 
					Proxy_Authenticate->scheme, 
					Proxy_Authenticate->realm, 
					Proxy_Authenticate->nonce, 
					Proxy_Authenticate->opaque, 
					Proxy_Authenticate->algorithm, 
					Proxy_Authenticate->qop)))
				{
					return ret;
				}
				else
				{
					isnew = 0;
					continue;
				}
			}
			else return -1;
		}

		if(isnew)
		{
			if((challenge = TSIP_CHALLENGE_CREATE(self->stack,
					1, 
					Proxy_Authenticate->scheme, 
					Proxy_Authenticate->realm, 
					Proxy_Authenticate->nonce, 
					Proxy_Authenticate->opaque, 
					Proxy_Authenticate->algorithm, 
					Proxy_Authenticate->qop)))
			{
				tsk_list_push_back_data(self->challenges, (void**)&challenge);
			}
			else return -1;
		}
	}	
	return 0;

}

int tsip_dialog_init(tsip_dialog_t *self, tsip_dialog_type_t type, tsip_stack_handle_t * stack, const char* call_id, tsip_operation_handle_t* operation)
{
	if(self)
	{
		const tsk_param_t* param;

		if(self->initialized)
		{
			TSK_DEBUG_WARN("Dialog already initialized.");
			return -2;
		}

		self->state = tsip_initial;
		self->type = type;
		self->stack = tsk_object_ref(stack);
		if(!self->routes){
			self->routes = TSK_LIST_CREATE();
		}
		if(!self->challenges){
			self->challenges = TSK_LIST_CREATE();
		}
		self->expires = TSIP_DIALOG_EXPIRES_DEFAULT;
		
		if(call_id){
			tsk_strupdate(&self->callid, call_id);
		}
		else{
			tsk_uuidstring_t uuid; /* Call-id is a random UUID */
			tsip_header_Call_ID_random(&uuid);
			tsk_strupdate(&self->callid, uuid);
		}

		self->operation = tsk_object_ref(operation);

		/* Local tag */
		{
			tsk_istr_t tag;
			tsk_strrandom(&tag);
			tsk_strupdate(&self->tag_local, tag);
		}

		/* CSeq */
		self->cseq_value = rand();

		/* Expires */
		if((param = tsip_operation_get_param(TSIP_DIALOG(self)->operation, "expires"))){
			self->expires = atoi(param->value);
		}
		else{
			self->expires = TSIP_DIALOG_EXPIRES_DEFAULT;
		}

		self->initialized = 1;
		return 0;
	}
	return -1;
}

int tsip_dialog_hangup(tsip_dialog_t *self)
{
	if(self)
	{
		return self->callback(self, tsip_dialog_hang_up, 0);
	}
	return -1;
}

int tsip_dialog_remove(const tsip_dialog_t* self)
{
	return tsip_dialog_layer_remove(TSIP_STACK(self->stack)->layer_dialog, TSIP_DIALOG(self));
}

int tsip_dialog_cmp(const tsip_dialog_t *d1, const tsip_dialog_t *d2)
{
	if(d1 && d2)
	{
		if(
			tsk_strequals(d1->callid, d2->callid) 
			&& (tsk_strequals(d1->tag_local, d2->tag_local))
			&& (tsk_strequals(d1->tag_remote, d2->tag_remote))
			)
		{
			return 0;
		}
	}
	return -1;
}

int tsip_dialog_deinit(tsip_dialog_t *self)
{
	if(self)
	{
		if(!self->initialized)
		{
			TSK_DEBUG_WARN("Dialog not initialized.");
			return -2;
		}

		tsk_object_unref(self->stack);
		tsk_object_unref(self->operation);

		TSK_OBJECT_SAFE_FREE(self->uri_local);
		TSK_FREE(self->tag_local);
		TSK_OBJECT_SAFE_FREE(self->uri_remote);
		TSK_FREE(self->tag_remote);

		TSK_OBJECT_SAFE_FREE(self->uri_remote_target);

		TSK_FREE(self->cseq_method);
		TSK_FREE(self->callid);

		TSK_OBJECT_SAFE_FREE(self->routes);
		TSK_OBJECT_SAFE_FREE(self->challenges);
		
		self->initialized = 0;

		return 0;
	}
	return -1;
}

