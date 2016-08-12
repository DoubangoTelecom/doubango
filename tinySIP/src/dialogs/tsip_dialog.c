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

/**@file tsip_dialog.c
 * @brief SIP dialog base class as per RFC 3261 subclause 17.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/dialogs/tsip_dialog.h"

#include "tinysip/dialogs/tsip_dialog_layer.h"
#include "tinysip/transactions/tsip_transac_layer.h"
#include "tinysip/transports/tsip_transport_layer.h"

#include "tinysip/transactions/tsip_transac_nict.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tinysip/headers/tsip_header_Authorization.h"
#include "tinysip/headers/tsip_header_Contact.h"
#include "tinysip/headers/tsip_header_Dummy.h"
#include "tinysip/headers/tsip_header_Expires.h"
#include "tinysip/headers/tsip_header_P_Preferred_Identity.h"
#include "tinysip/headers/tsip_header_Proxy_Authenticate.h"
#include "tinysip/headers/tsip_header_Proxy_Authorization.h"
#include "tinysip/headers/tsip_header_Record_Route.h"
#include "tinysip/headers/tsip_header_Route.h"
#include "tinysip/headers/tsip_header_Subscription_State.h"
#include "tinysip/headers/tsip_header_WWW_Authenticate.h"

#include "tsk_debug.h"
#include "tsk_time.h"

int tsip_dialog_update_challenges(tsip_dialog_t *self, const tsip_response_t* response, tsk_bool_t acceptNewVector);
int tsip_dialog_add_session_headers(const tsip_dialog_t *self, tsip_request_t* request);
int tsip_dialog_add_common_headers(const tsip_dialog_t *self, tsip_request_t* request);

extern tsip_uri_t* tsip_stack_get_pcscf_uri(const tsip_stack_t *self, tnet_socket_type_t type, tsk_bool_t lr);
extern tsip_uri_t* tsip_stack_get_contacturi(const tsip_stack_t *self, const char* protocol);

#define TSIP_DIALOG_ADD_HEADERS(headers) {\
		const tsk_list_item_t* item;\
		tsk_list_foreach(item, headers){ \
			if(!TSK_PARAM(item->data)->tag){ \
				/* 'Route' is special header as it's used to find next destination address */ \
				if(tsk_striequals(TSK_PARAM(item->data)->name, "route")){ \
					tsip_uri_t* route_uri; \
					char* route_uri_str = tsk_strdup(TSK_PARAM(item->data)->value); \
					tsk_strunquote_2(&route_uri_str, '<', '>'); \
					route_uri = tsip_uri_parse(route_uri_str, tsk_strlen(route_uri_str)); \
					if(route_uri){ \
						tsip_message_add_headers(request, \
							TSIP_HEADER_ROUTE_VA_ARGS(route_uri), \
							tsk_null); \
						TSK_OBJECT_SAFE_FREE(route_uri); \
					} \
					TSK_FREE(route_uri_str); \
				} \
				else{ \
					TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_DUMMY_VA_ARGS(TSK_PARAM(item->data)->name, TSK_PARAM(item->data)->value)); \
				} \
			} \
		}\
	}


tsip_request_t *tsip_dialog_request_new(const tsip_dialog_t *self, const char* method)
{
    tsip_request_t *request = tsk_null;
    tsip_uri_t *to_uri, *from_uri, *request_uri;
    const char *call_id;
    int copy_routes_start = -1; /* NONE */
    const tsk_list_item_t* item;

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
    if(TSK_LIST_IS_EMPTY(self->record_routes)) {
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
    else {
        const tsip_uri_t *first_route = ((tsip_header_Record_Route_t*)TSK_LIST_FIRST_DATA(self->record_routes))->uri;
        if(tsk_params_have_param(first_route->params, "lr")) {
            request_uri = tsk_object_ref(self->uri_remote_target);
            copy_routes_start = 0; /* Copy all */
        }
        else {
            request_uri = tsk_object_ref((void*)first_route);
            copy_routes_start = 1; /* Copy starting at index 1. */
        }
    }

    /*=====================================================================
    */
    request = tsip_request_new(method, request_uri, from_uri, to_uri, call_id, self->cseq_value);
    request->To->tag = tsk_strdup(self->tag_remote);
    request->From->tag = tsk_strdup(self->tag_local);
    request->update = tsk_true; /* Now signal that the message should be updated by the transport layer (Contact, SigComp, IPSec, ...) */


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
    switch(request->line.request.request_type) {
    case tsip_MESSAGE:
    case tsip_PUBLISH:
    case tsip_BYE: {
        if(request->line.request.request_type == tsip_PUBLISH) {
            TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_EXPIRES_VA_ARGS(TSK_TIME_MS_2_S(self->expires)));
        }
        /* add caps in Accept-Contact headers */
        tsk_list_foreach(item, self->ss->caps) {
            const tsk_param_t* param = TSK_PARAM(item->data);
            char* value = tsk_null;
            tsk_sprintf(&value, "*;%s%s%s",
                        param->name,
                        param->value ? "=" : "",
                        param->value ? param->value : "");
            if(value) {
                TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_DUMMY_VA_ARGS("Accept-Contact", value));
                TSK_FREE(value);
            }
        }
        break;
    }

    default: {
        char* contact = tsk_null;
        tsip_header_Contacts_L_t *hdr_contacts;

        if(request->line.request.request_type == tsip_OPTIONS ||
                request->line.request.request_type == tsip_PUBLISH ||
                request->line.request.request_type == tsip_REGISTER) {
            /**** with expires */
            tsk_sprintf(&contact, "m: <%s:%s@%s:%d>;expires=%d\r\n",
                        "sip",
                        from_uri->user_name,
                        "127.0.0.1",
                        5060,

                        TSK_TIME_MS_2_S(self->expires));
        }
        else {
            /**** without expires */
            if(request->line.request.request_type == tsip_SUBSCRIBE) {
                /* RFC 3265 - 3.1.1. Subscription Duration
                	An "expires" parameter on the "Contact" header has no semantics for SUBSCRIBE and is explicitly
                	not equivalent to an "Expires" header in a SUBSCRIBE request or response.
                */
                TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_EXPIRES_VA_ARGS(TSK_TIME_MS_2_S(self->expires)));
            }
            tsk_sprintf(&contact, "m: <%s:%s@%s:%d%s%s%s%s%s%s%s%s%s>\r\n",
                        "sip",
                        from_uri->user_name,
                        "127.0.0.1",
                        5060,

                        self->ss->ws.src.host ? ";" : "",
                        self->ss->ws.src.host ? "ws-src-ip=" : "",
                        self->ss->ws.src.host ? self->ss->ws.src.host : "",
                        self->ss->ws.src.port[0] ? ";" : "",
                        self->ss->ws.src.port[0] ? "ws-src-port=" : "",
                        self->ss->ws.src.port[0] ? self->ss->ws.src.port : "",
                        self->ss->ws.src.proto ? ";" : "",
                        self->ss->ws.src.proto ? "ws-src-proto=" : "",
                        self->ss->ws.src.proto ? self->ss->ws.src.proto : ""
                       );
        }
        hdr_contacts = tsip_header_Contact_parse(contact, tsk_strlen(contact));
        if(!TSK_LIST_IS_EMPTY(hdr_contacts)) {
            request->Contact = tsk_object_ref(hdr_contacts->head->data);
        }
        TSK_OBJECT_SAFE_FREE(hdr_contacts);
        TSK_FREE(contact);

        /* Add capabilities as per RFC 3840 */
        if(request->Contact) {
            tsk_list_foreach(item, self->ss->caps) {
                tsk_params_add_param(&TSIP_HEADER(request->Contact)->params, TSK_PARAM(item->data)->name, TSK_PARAM(item->data)->value);
            }
			tsk_strupdate(&(request->Contact->uri->display_name), TSIP_DIALOG_GET_STACK(self)->identity.display_name);
        }

        break;
    }
    }

    /* Update authorizations */
    if(self->state == tsip_initial && TSK_LIST_IS_EMPTY(self->challenges)) {
        /* 3GPP TS 33.978 6.2.3.1 Procedures at the UE
        	On sending a REGISTER request in order to indicate support for early IMS security procedures, the UE shall not
        	include an Authorization header field and not include header fields or header field values as required by RFC3329.
        */
        if(TSIP_REQUEST_IS_REGISTER(request) && !TSIP_DIALOG_GET_STACK(self)->security.earlyIMS) {
            /*	3GPP TS 24.229 - 5.1.1.2.2 Initial registration using IMS AKA
            	On sending a REGISTER request, the UE shall populate the header fields as follows:
            		a) an Authorization header field, with:
            		- the "username" header field parameter, set to the value of the private user identity;
            		- the "realm" header field parameter, set to the domain name of the home network;
            		- the "uri" header field parameter, set to the SIP URI of the domain name of the home network;
            		- the "nonce" header field parameter, set to an empty value; and
            		- the "response" header field parameter, set to an empty value;
            */
            const char* realm = TSIP_DIALOG_GET_STACK(self)->network.realm ? TSIP_DIALOG_GET_STACK(self)->network.realm->host : "(null)";
            char* request_uri = tsip_uri_tostring(request->line.request.uri, tsk_false, tsk_false);
            tsip_header_t* auth_hdr = tsip_challenge_create_empty_header_authorization(TSIP_DIALOG_GET_STACK(self)->identity.impi, realm, request_uri);
            tsip_message_add_header(request, auth_hdr);
            tsk_object_unref(auth_hdr), auth_hdr = tsk_null;
            TSK_FREE(request_uri);
        }
    }
    else if(!TSK_LIST_IS_EMPTY(self->challenges)) {
        tsip_challenge_t *challenge;
        tsip_header_t* auth_hdr;
        tsk_list_foreach(item, self->challenges) {
            challenge = item->data;
            auth_hdr = tsip_challenge_create_header_authorization(challenge, request);
            if(auth_hdr) {
                tsip_message_add_header(request, auth_hdr);
                tsk_object_unref(auth_hdr), auth_hdr = tsk_null;
            }
        }
    }

    /* Update CSeq */
    /*	RFC 3261 - 13.2.2.4 2xx Responses
       Generating ACK: The sequence number of the CSeq header field MUST be
       the same as the INVITE being acknowledged, but the CSeq method MUST
       be ACK.  The ACK MUST contain the same credentials as the INVITE.  If
       the 2xx contains an offer (based on the rules above), the ACK MUST
       carry an answer in its body.
       ==> CSeq number will be added/updated by the caller of this function,
       credentials were added above.
    */
    if(!TSIP_REQUEST_IS_ACK(request) && !TSIP_REQUEST_IS_CANCEL(request)) {
        request->CSeq->seq = ++(TSIP_DIALOG(self)->cseq_value);
    }

    /* Route generation
    	*	==> http://betelco.blogspot.com/2008/11/proxy-and-service-route-discovery-in.html
    	* The dialog Routes have been copied above.

    	3GPP TS 24.229 - 5.1.2A.1 UE-originating case

    	The UE shall build a proper preloaded Route header field value for all new dialogs and standalone transactions. The UE
    	shall build a list of Route header field values made out of the following, in this order:
    	a) the P-CSCF URI containing the IP address or the FQDN learnt through the P-CSCF discovery procedures; and
    	b) the P-CSCF port based on the security mechanism in use:

    	- if IMS AKA or SIP digest with TLS is in use as a security mechanism, the protected server port learnt during
    	the registration procedure;
    	- if SIP digest without TLS, NASS-IMS bundled authentciation or GPRS-IMS-Bundled authentication is in
    	use as a security mechanism, the unprotected server port used during the registration procedure;
    	c) and the values received in the Service-Route header field saved from the 200 (OK) response to the last
    	registration or re-registration of the public user identity with associated contact address.
    */
    if(!TSIP_REQUEST_IS_REGISTER(request)) {
        // According to the above link ==> Initial/Re/De registration do not have routes.
        if(copy_routes_start != -1) {
            /* The dialog already have routes ==> copy them. */
            if(self->state == tsip_early || self->state == tsip_established) {
                int32_t index = -1;
                tsk_list_foreach(item, self->record_routes) {
                    tsip_header_Record_Route_t *record_Route = ((tsip_header_Record_Route_t*)item->data);
                    const tsip_uri_t* uri = record_Route->uri;
                    tsip_header_Route_t *route = tsk_null;
                    if(++index < copy_routes_start || !uri) {
                        continue;
                    }

                    if((route = tsip_header_Route_create(uri))) {
                        // copy parameters: see http://code.google.com/p/imsdroid/issues/detail?id=52
                        if(!TSK_LIST_IS_EMPTY(TSIP_HEADER_PARAMS(record_Route))) {
                            if(!TSIP_HEADER_PARAMS(route)) {
                                TSIP_HEADER_PARAMS(route) = tsk_list_create();
                            }
                            tsk_list_pushback_list(TSIP_HEADER_PARAMS(route), TSIP_HEADER_PARAMS(record_Route));
                        }

                        tsip_message_add_header(request, TSIP_HEADER(route));
                        TSK_OBJECT_SAFE_FREE(route);
                    }
                }
            }
        }
        else {
            /* No routes associated to this dialog. */
            if(self->state == tsip_initial || self->state == tsip_early) {
                /*	GPP TS 24.229 section 5.1.2A [Generic procedures applicable to all methods excluding the REGISTER method]:
                	The UE shall build a proper preloaded Route header field value for all new dialogs and standalone transactions. The UE
                	shall build a list of Route header field values made out of the following, in this order:
                	a) the P-CSCF URI containing the IP address or the FQDN learnt through the P-CSCF discovery procedures; and
                	b) the P-CSCF port based on the security mechanism in use:
                		- if IMS AKA or SIP digest with TLS is in use as a security mechanism, the protected server port learnt during
                		the registration procedure;
                		- if SIP digest without TLS, NASS-IMS bundled authentciation or GPRS-IMS-Bundled authentication is in
                		use as a security mechanism, the unprotected server port used during the registration procedure;
                	c) and the values received in the Service-Route header field saved from the 200 (OK) response to the last
                	registration or re-registration of the public user identity with associated contact address.
                */
#if _DEBUG && defined(SDS_HACK)/* FIXME: remove this */
                /* Ericsson SDS hack (INVITE with Proxy-CSCF as First route fail) */
#elif 0
                tsip_uri_t *uri = tsip_stack_get_pcscf_uri(TSIP_DIALOG_GET_STACK(self), tsk_true);
                // Proxy-CSCF as first route
                if(uri) {
                    TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_ROUTE_VA_ARGS(uri));
                    TSK_OBJECT_SAFE_FREE(uri);
                }
#endif
                // Service routes
                tsk_list_foreach(item, TSIP_DIALOG_GET_STACK(self)->service_routes) {
                    TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_ROUTE_VA_ARGS(item->data));
                }
            }
        }
    }

    /* Add headers associated to the session */
    tsip_dialog_add_session_headers(self, request);

    /* Add headers associated to the dialog's stack */
    TSIP_DIALOG_ADD_HEADERS(self->ss->stack->headers);

    /* Add common headers */
    tsip_dialog_add_common_headers(self, request);

    /* SigComp */
    if(self->ss->sigcomp_id) {
        /* should be added in this field instead of 'Contact' or 'Via' headers
        * it's up to the transport layer to copy it to these headers */
        request->sigcomp_id = tsk_strdup(self->ss->sigcomp_id);
    }

    /* Remote Address: Used if "Server mode" otherwise Proxy-CSCF will be used  */
    request->remote_addr = self->remote_addr;
    /* Connected FD */
    if(request->local_fd <= 0) {
        request->local_fd = self->connected_fd;
    }

    TSK_OBJECT_SAFE_FREE(request_uri);
    TSK_OBJECT_SAFE_FREE(from_uri);
    TSK_OBJECT_SAFE_FREE(to_uri);

    return request;
}


/** Sends a SIP/IMS request. This function is responsible for transaction creation.
 *
 * @param self	The parent dialog. All callback events will be notified to this dialog.
 * @param request	The request to send.
 *
 * @return	Zero if succeed and no-zero error code otherwise.
**/
int tsip_dialog_request_send(const tsip_dialog_t *self, tsip_request_t* request)
{
    int ret = -1;

    if(self && TSIP_DIALOG_GET_STACK(self)) {
        const tsip_transac_layer_t *layer = TSIP_DIALOG_GET_STACK(self)->layer_transac;
        if(layer) {
            /*	Create new transaction. The new transaction will be added to the transaction layer.
            	The transaction has all information to create the right transaction type (NICT or ICT).
            	As this is an outgoing request ==> It shall be a client transaction (NICT or ICT).
            	For server transactions creation see @ref tsip_dialog_response_send.
            */
            static const tsk_bool_t isCT = tsk_true;
            tsip_transac_t* transac;
            tsip_transac_dst_t* dst;


            if(TSIP_STACK_MODE_IS_CLIENT(TSIP_DIALOG_GET_STACK(self))) {
                const tsip_transport_t* transport = tsip_transport_layer_find_by_idx(TSIP_DIALOG_GET_STACK(self)->layer_transport, TSIP_DIALOG_GET_STACK(self)->network.transport_idx_default);
                if(!transport) {
                    TSK_DEBUG_ERROR("Failed to find a valid default transport [%d]", TSIP_DIALOG_GET_STACK(self)->network.transport_idx_default);
                }
                else {
                    request->dst_net_type = transport->type;
                }
            }
            dst = tsip_transac_dst_dialog_create(TSIP_DIALOG(self));
            transac = tsip_transac_layer_new(
                          layer,
                          isCT,
                          request,
                          dst
                      );
            TSK_OBJECT_SAFE_FREE(dst);

            /* Set the transaction's dialog. All events comming from the transaction (timeouts, errors ...) will be signaled to this dialog */
            if(transac) {
                switch(transac->type) {
                case tsip_transac_type_ict:
                case tsip_transac_type_nict: {
                    /* Start the newly create IC/NIC transaction */
                    ret = tsip_transac_start(transac, request);
                    break;
                }
                default:
                    break;
                }
                TSK_OBJECT_SAFE_FREE(transac);
            }
        }
    }
    return ret;
}

tsip_response_t *tsip_dialog_response_new(tsip_dialog_t *self, short status, const char* phrase, const tsip_request_t* request)
{
    /* Reponse is created as per RFC 3261 subclause 8.2.6 and (headers+tags) are copied
    * as per subclause 8.2.6.2.
    */
    tsip_response_t* response;
    if((response = tsip_response_new(status, phrase, request))) {
        switch(request->line.request.request_type) {
        case tsip_MESSAGE:
        case tsip_PUBLISH:
            break;
        default:
            /* Is there a To tag?  */
            if(response->To && !response->To->tag) {
                response->To->tag = tsk_strdup(self->tag_local);
            }
            /* Contact Header (for 101-299 reponses) */
            if(self->uri_local && TSIP_RESPONSE_CODE(response) >= 101 && TSIP_RESPONSE_CODE(response) <= 299) {
                char* contact = tsk_null;
                tsip_header_Contacts_L_t *hdr_contacts;

                tsk_sprintf(&contact, "m: <%s:%s@%s:%d>\r\n", "sip", self->uri_local->user_name, "127.0.0.1", 5060);
                hdr_contacts = tsip_header_Contact_parse(contact, tsk_strlen(contact));
                if(!TSK_LIST_IS_EMPTY(hdr_contacts)) {
                    response->Contact = tsk_object_ref(hdr_contacts->head->data);
                    response->update = tsk_true; /* Now signal that the message should be updated by the transport layer (Contact header) */
                }
                TSK_OBJECT_SAFE_FREE(hdr_contacts);
                TSK_FREE(contact);
            }
            break;
        }

        /* SigComp */
        if(self->ss->sigcomp_id) {
            /* should be added in this field instead of 'Contact' or 'Via' headers
            * it's up to the transport layer to copy it to these headers */
            response->sigcomp_id = tsk_strdup(self->ss->sigcomp_id);
        }
        /* Connected FD */
        if(response->local_fd <= 0) {
            response->local_fd = self->connected_fd;
        }
        /* Remote Addr: used to send requests if "Server Mode" otherwise Proxy-CSCF address will be used */
        self->remote_addr = request->remote_addr;
    }
    return response;
}

int tsip_dialog_response_send(const tsip_dialog_t *self, tsip_response_t* response)
{
    int ret = -1;

    if(self && TSIP_DIALOG_GET_STACK(self)) {
        const tsip_transac_layer_t *layer = TSIP_DIALOG_GET_STACK(self)->layer_transac;
        if(layer) {
            /* As this is a response ...then use the associate server transaction */
            tsip_transac_t *transac = tsip_transac_layer_find_server(layer, response);
            if(transac) {
                ret = transac->callback(transac, tsip_transac_outgoing_msg, response);
                tsk_object_unref(transac);
            }
            else {
                TSK_DEBUG_ERROR("Failed to find associated server transaction.");
                // Send "408 Request Timeout" (should be done by the transaction layer)?
            }
        }
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
    }
    return ret;
}

int tsip_dialog_apply_action(tsip_message_t* message, const tsip_action_t* action)
{
    const tsk_list_item_t* item;

    if(!message || !action) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* SIP headers */
    tsk_list_foreach(item, action->headers) {
        TSIP_MESSAGE_ADD_HEADER(message, TSIP_HEADER_DUMMY_VA_ARGS(TSK_PARAM(item->data)->name, TSK_PARAM(item->data)->value));
    }
    /* Payload */
    if(action->payload) {
        tsip_message_add_content(message, tsk_null, TSK_BUFFER_DATA(action->payload), TSK_BUFFER_SIZE(action->payload));
    }

    return 0;
}

/**
 * Gets the number of milliseconds to wait before retransmission.
 *			e.g. ==> delay before refreshing registrations (REGISTER), subscribtions (SUBSCRIBE), publication (PUBLISH) ...
 *
 *
 * @param [in,out]	self		The calling dialog.
 * @param [in,out]	response	The SIP/IMS response containing the new delay (expires, subscription-state ...).
 *
 * @return	Zero if succeed and no-zero error code otherwise.
**/
int64_t tsip_dialog_get_newdelay(tsip_dialog_t *self, const tsip_message_t* message)
{
    int64_t expires = self->expires;
    int64_t newdelay = expires;	/* default value */
    const tsip_header_t* hdr;
    tsk_size_t i;

    /*== NOTIFY with subscription-state header with expires parameter.
    */
    if(TSIP_REQUEST_IS_NOTIFY(message)) {
        const tsip_header_Subscription_State_t *hdr_state;
        if((hdr_state = (const tsip_header_Subscription_State_t*)tsip_message_get_header(message, tsip_htype_Subscription_State))) {
            if(hdr_state->expires >0) {
                expires = TSK_TIME_S_2_MS(hdr_state->expires);
                goto compute;
            }
        }
    }

    /*== Expires header.
    */
    if((hdr = tsip_message_get_header(message, tsip_htype_Expires))) {
        expires = TSK_TIME_S_2_MS(((const tsip_header_Expires_t*)hdr)->delta_seconds);
        goto compute;
    }

    /*== Contact header.
    */
    for(i=0; (hdr = tsip_message_get_headerAt(message, tsip_htype_Contact, i)); i++) {
        const tsip_header_Contact_t* contact = (const tsip_header_Contact_t*)hdr;
        if(contact && contact->uri) {
            const char* transport = tsk_params_get_param_value(contact->uri->params, "transport");
            tsip_uri_t* contactUri = tsip_stack_get_contacturi(TSIP_DIALOG_GET_STACK(self), transport ? transport : "udp");
            if(contactUri) {
                if(tsk_strequals(contact->uri->user_name, contactUri->user_name)
                        && tsk_strequals(contact->uri->host, contactUri->host)
                        && contact->uri->port == contactUri->port) {
                    if(contact->expires>=0) { /* No expires parameter ==> -1*/
                        expires = TSK_TIME_S_2_MS(contact->expires);

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
    expires = TSK_TIME_MS_2_S(expires);
    newdelay = (expires > 1200) ? (expires - 600) : (expires/2);

    return TSK_TIME_S_2_MS(newdelay);
}

/**
 *
 * Updates the dialog state:
 *			- Authorizations (using challenges from the @a response message)
 *			- State (early, established, disconnected, ...)
 *			- Routes (and Service-Route)
 *			- Target (remote)
 *			- ...
 *
 * @param [in,out]	self		The calling dialog.
 * @param [in,out]	response	The SIP/IMS response from which to get the new information.
 *
 * @return	Zero if succeed and no-zero error code otherwise.
**/
int tsip_dialog_update(tsip_dialog_t *self, const tsip_response_t* response)
{
    if(self && TSIP_MESSAGE_IS_RESPONSE(response) && response->To) {
        short code = TSIP_RESPONSE_CODE(response);
        const char *tag = response->To->tag;

        /*
        *	1xx (!100) or 2xx
        */
        /*
        *	401 or 407 or 421 or 494
        */
        if(code == 401 || code == 407 || code == 421 || code == 494) {
            tsk_bool_t acceptNewVector;

            /* 3GPP IMS - Each authentication vector is used only once.
            *	==> Re-registration/De-registration ==> Allow 401/407 challenge.
            */
            acceptNewVector = (TSIP_RESPONSE_IS_TO_REGISTER(response) && self->state == tsip_established);
            return tsip_dialog_update_challenges(self, response, acceptNewVector);
        }
        else if(100 < code && code < 300) {
            tsip_dialog_state_t state = self->state;

            /* 1xx */
            if(code <= 199) {
                if(tsk_strnullORempty(response->To->tag)) {
                    TSK_DEBUG_WARN("Invalid tag  parameter");
                    return 0;
                }
                state = tsip_early;
            }
            /* 2xx */
            else {
                state = tsip_established;
            }

            /* Remote target */
            {
                /*	RFC 3261 12.2.1.2 Processing the Responses
                	When a UAC receives a 2xx response to a target refresh request, it
                	MUST replace the dialog's remote target URI with the URI from the
                	Contact header field in that response, if present.

                	FIXME: Because PRACK/UPDATE sent before the session is established MUST have
                	the rigth target URI to be delivered to the UAS ==> Do not not check that we are connected
                */
                if(!TSIP_RESPONSE_IS_TO_REGISTER(response) && response->Contact && response->Contact->uri) {
                    TSK_OBJECT_SAFE_FREE(self->uri_remote_target);
                    self->uri_remote_target = tsip_uri_clone(response->Contact->uri, tsk_true, tsk_false);
                }
            }

            /* Route sets */
            {
                tsk_size_t index;
                const tsip_header_Record_Route_t *recordRoute;
                tsip_header_Record_Route_t *route;

                TSK_OBJECT_SAFE_FREE(self->record_routes);

                for(index = 0; (recordRoute = (const tsip_header_Record_Route_t *)tsip_message_get_headerAt(response, tsip_htype_Record_Route, index)); index++) {
                    if(!self->record_routes) {
                        self->record_routes = tsk_list_create();
                    }
                    if((route = tsk_object_ref((void*)recordRoute))) {
                        tsk_list_push_front_data(self->record_routes, (void**)&route); /* Copy reversed. */
                    }
                }
            }


            /* cseq + tags + ... */
            if(self->state == tsip_established && tsk_striequals(self->tag_remote, tag)) {
                return 0;
            }
            else {
                if(!TSIP_RESPONSE_IS_TO_REGISTER(response) && !TSIP_RESPONSE_IS_TO_PUBLISH(response)) { /* REGISTER and PUBLISH don't establish dialog */
                    tsk_strupdate(&self->tag_remote, tag);
                }
#if 0			// PRACK and BYE will have same CSeq value ==> Let CSeq value to be incremented by "tsip_dialog_request_new()"
                self->cseq_value = response->CSeq ? response->CSeq->seq : self->cseq_value;
#endif
            }

            self->state = state;
            return 0;
        }
    }
    return 0;
}

int tsip_dialog_update_2(tsip_dialog_t *self, const tsip_request_t* invite)
{
    if(!self || !invite) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* Remote target */
    if(invite->Contact && invite->Contact->uri) {
        TSK_OBJECT_SAFE_FREE(self->uri_remote_target);
        self->uri_remote_target = tsip_uri_clone(invite->Contact->uri, tsk_true, tsk_false);
    }

    /* cseq + tags + remote-uri */
    tsk_strupdate(&self->tag_remote, invite->From?invite->From->tag:"doubango");
    /* self->cseq_value = invite->CSeq ? invite->CSeq->seq : self->cseq_value; */
    if(invite->From && invite->From->uri) {
        TSK_OBJECT_SAFE_FREE(self->uri_remote);
        self->uri_remote = tsk_object_ref(invite->From->uri);
    }

    /* Route sets */
    {
        tsk_size_t index;
        const tsip_header_Record_Route_t *recordRoute;
        tsip_header_Record_Route_t* route;

        TSK_OBJECT_SAFE_FREE(self->record_routes);

        for(index = 0; (recordRoute = (const tsip_header_Record_Route_t *)tsip_message_get_headerAt(invite, tsip_htype_Record_Route, index)); index++) {
            if(!self->record_routes) {
                self->record_routes = tsk_list_create();
            }
            if((route = tsk_object_ref((void*)recordRoute))) {
                tsk_list_push_back_data(self->record_routes, (void**)&route); /* Copy non-reversed. */
            }
        }
    }

    self->state = tsip_established;

    return 0;
}

int tsip_dialog_getCKIK(tsip_dialog_t *self, AKA_CK_T *ck, AKA_IK_T *ik)
{
    tsk_list_item_t *item;
    tsip_challenge_t *challenge;

    if(!self) {
        return -1;
    }

    tsk_list_foreach(item, self->challenges) {
        if((challenge = item->data)) {
            memcpy(*ck, challenge->ck, AKA_CK_SIZE);
            memcpy(*ik, challenge->ik, AKA_IK_SIZE);
            return 0;
        }
    }
    TSK_DEBUG_ERROR("No challenge found. Fail to set IK and CK.");
    return -2;
}

int tsip_dialog_update_challenges(tsip_dialog_t *self, const tsip_response_t* response, int acceptNewVector)
{
    int ret = -1;
    tsk_size_t i;

    tsk_list_item_t *item;

    tsip_challenge_t *challenge;

    const tsip_header_WWW_Authenticate_t *WWW_Authenticate;
    const tsip_header_Proxy_Authenticate_t *Proxy_Authenticate;

    /* RFC 2617 - HTTP Digest Session

    *	(A) The client response to a WWW-Authenticate challenge for a protection
    	space starts an authentication session with that protection space.
    	The authentication session lasts until the client receives another
    	WWW-Authenticate challenge from any server in the protection space.

    	(B) The server may return a 401 response with a new nonce value, causing the client
    	to retry the request; by specifying stale=TRUE with this response,
    	the server tells the client to retry with the new nonce, but without
    	prompting for a new username and password.
    */
    /* RFC 2617 - 1.2 Access Authentication Framework
    	The realm directive (case-insensitive) is required for all authentication schemes that issue a challenge.
    */

    /* FIXME: As we perform the same task ==> Use only one loop.
    */

    for(i =0; (WWW_Authenticate = (const tsip_header_WWW_Authenticate_t*)tsip_message_get_headerAt(response, tsip_htype_WWW_Authenticate, i)); i++) {
        tsk_bool_t isnew = tsk_true;

        tsk_list_foreach(item, self->challenges) {
            challenge = item->data;
            if(challenge->isproxy) {
                continue;
            }

            if(tsk_striequals(challenge->realm, WWW_Authenticate->realm) && (WWW_Authenticate->stale || acceptNewVector)) {
                /*== (B) ==*/
                if((ret = tsip_challenge_update(challenge,
                                                WWW_Authenticate->scheme,
                                                WWW_Authenticate->realm,
                                                WWW_Authenticate->nonce,
                                                WWW_Authenticate->opaque,
                                                WWW_Authenticate->algorithm,
                                                WWW_Authenticate->qop))) {
                    return ret;
                }
                else {
                    isnew = tsk_false;
                    continue;
                }
            }
            else {
                TSK_DEBUG_ERROR("Failed to handle new challenge");
                return -1;
            }
        }

        if(isnew) {
            if((challenge = tsip_challenge_create(TSIP_DIALOG_GET_STACK(self),
                                                  tsk_false,
                                                  WWW_Authenticate->scheme,
                                                  WWW_Authenticate->realm,
                                                  WWW_Authenticate->nonce,
                                                  WWW_Authenticate->opaque,
                                                  WWW_Authenticate->algorithm,
                                                  WWW_Authenticate->qop))) {
                if(TSIP_DIALOG_GET_SS(self)->auth_ha1 && TSIP_DIALOG_GET_SS(self)->auth_impi) {
                    tsip_challenge_set_cred(challenge, TSIP_DIALOG_GET_SS(self)->auth_impi, TSIP_DIALOG_GET_SS(self)->auth_ha1);
                }
                tsk_list_push_back_data(self->challenges, (void**)&challenge);
            }
            else {
                TSK_DEBUG_ERROR("Failed to handle new challenge");
                return -1;
            }
        }
    }

    for(i=0; (Proxy_Authenticate = (const tsip_header_Proxy_Authenticate_t*)tsip_message_get_headerAt(response, tsip_htype_Proxy_Authenticate, i)); i++) {
        tsk_bool_t isnew = tsk_true;

        tsk_list_foreach(item, self->challenges) {
            challenge = item->data;
            if(!challenge->isproxy) {
                continue;
            }

            if(tsk_striequals(challenge->realm, Proxy_Authenticate->realm) && (Proxy_Authenticate->stale || acceptNewVector)) {
                /*== (B) ==*/
                if((ret = tsip_challenge_update(challenge,
                                                Proxy_Authenticate->scheme,
                                                Proxy_Authenticate->realm,
                                                Proxy_Authenticate->nonce,
                                                Proxy_Authenticate->opaque,
                                                Proxy_Authenticate->algorithm,
                                                Proxy_Authenticate->qop))) {
                    return ret;
                }
                else {
                    isnew = tsk_false;
                    continue;
                }
            }
            else {
                TSK_DEBUG_ERROR("Failed to handle new challenge");
                return -1;
            }
        }

        if(isnew) {
            if((challenge = tsip_challenge_create(TSIP_DIALOG_GET_STACK(self),
                                                  tsk_true,
                                                  Proxy_Authenticate->scheme,
                                                  Proxy_Authenticate->realm,
                                                  Proxy_Authenticate->nonce,
                                                  Proxy_Authenticate->opaque,
                                                  Proxy_Authenticate->algorithm,
                                                  Proxy_Authenticate->qop))) {
                if(TSIP_DIALOG_GET_SS(self)->auth_ha1 && TSIP_DIALOG_GET_SS(self)->auth_impi) {
                    tsip_challenge_set_cred(challenge, TSIP_DIALOG_GET_SS(self)->auth_impi, TSIP_DIALOG_GET_SS(self)->auth_ha1);
                }
                tsk_list_push_back_data(self->challenges, (void**)&challenge);
            }
            else {
                TSK_DEBUG_ERROR("Failed to handle new challenge");
                return -1;
            }
        }
    }
    return 0;
}

int tsip_dialog_add_session_headers(const tsip_dialog_t *self, tsip_request_t* request)
{
    if(!self || !request) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    TSIP_DIALOG_ADD_HEADERS(self->ss->headers);
    return 0;
}

int tsip_dialog_add_common_headers(const tsip_dialog_t *self, tsip_request_t* request)
{
    tsk_bool_t earlyIMS = tsk_false;
    const tsip_uri_t* preferred_identity = tsk_null;
    const char* netinfo = tsk_null;

    if(!self || !request) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    earlyIMS = TSIP_DIALOG_GET_STACK(self)->security.earlyIMS;
    preferred_identity = TSIP_DIALOG_GET_STACK(self)->identity.preferred;

    //
    //	P-Preferred-Identity
    //
    if(preferred_identity && TSIP_STACK_MODE_IS_CLIENT(TSIP_DIALOG_GET_STACK(self))) {
        /*	3GPP TS 33.978 6.2.3.1 Procedures at the UE
        	The UE shall use the temporary public user identity (IMSI-derived IMPU, cf. section 6.1.2) only in registration
        	messages (i.e. initial registration, re-registration or de-registration), but not in any other type of SIP requests.
        */
        switch(request->line.request.request_type) {
        case tsip_BYE:
        case tsip_INVITE:
        case tsip_OPTIONS:
        case tsip_SUBSCRIBE:
        case tsip_NOTIFY:
        case tsip_REFER:
        case tsip_MESSAGE:
        case tsip_PUBLISH:
        case tsip_REGISTER: {
            if(!earlyIMS || (earlyIMS && TSIP_REQUEST_IS_REGISTER(request))) {
                TSIP_MESSAGE_ADD_HEADER(request,
                                        TSIP_HEADER_P_PREFERRED_IDENTITY_VA_ARGS(preferred_identity)
                                       );
            }
            break;
        }
        default:
            break;
        }
    }

    //
    //	P-Access-Network-Info
    //
    if(netinfo) {
        switch(request->line.request.request_type) {
        case tsip_BYE:
        case tsip_INVITE:
        case tsip_OPTIONS:
        case tsip_REGISTER:
        case tsip_SUBSCRIBE:
        case tsip_NOTIFY:
        case tsip_PRACK:
        case tsip_INFO:
        case tsip_UPDATE:
        case tsip_REFER:
        case tsip_MESSAGE:
        case tsip_PUBLISH: {
            TSIP_MESSAGE_ADD_HEADER(request, TSIP_HEADER_P_ACCESS_NETWORK_INFO_VA_ARGS(netinfo));
            break;
        }
        default:
            break;
        }
    }

    return 0;
}

int tsip_dialog_init(tsip_dialog_t *self, tsip_dialog_type_t type, const char* call_id, tsip_ssession_t* ss, tsk_fsm_state_id curr, tsk_fsm_state_id term)
{
    static tsip_dialog_id_t unique_id = 0;
    if(self) {
        if(self->initialized) {
            TSK_DEBUG_WARN("Dialog already initialized.");
            return -2;
        }

        self->state = tsip_initial;
        self->type = type;
        self->id = ++unique_id;
        self->connected_fd = TNET_INVALID_FD;
        if(!self->record_routes) {
            self->record_routes = tsk_list_create();
        }
        if(!self->challenges) {
            self->challenges = tsk_list_create();
        }

        /* Sets some defalt values */
        self->expires = TSIP_SSESSION_EXPIRES_DEFAULT;

        if(call_id) {
            /* "server-side" session */
            tsk_strupdate(&self->callid, call_id);
        }
        else {
            tsk_uuidstring_t uuid; /* Call-id is a random UUID */
            tsip_header_Call_ID_random(&uuid);
            tsk_strupdate(&self->callid, uuid);
        }

        /* ref SIP session */
        self->ss = tsk_object_ref(ss);

        /* Local tag */{
            tsk_istr_t tag;
            tsk_strrandom(&tag);
            tsk_strupdate(&self->tag_local, tag);
        }

        /* CSeq */
        self->cseq_value = (rand() + 1);

        /* FSM */
        self->fsm = tsk_fsm_create(curr, term);

        /*=== SIP Session ===*/
        if(self->ss != TSIP_SSESSION_INVALID_HANDLE) {

            /* Expires */
            self->expires = ss->expires;

            /* From */
            self->uri_local = tsk_object_ref(call_id/* "server-side" */ ? ss->to : ss->from);

            /* To */
            if(ss->to) {
                self->uri_remote = tsk_object_ref(ss->to);
                self->uri_remote_target = tsk_object_ref(ss->to); /* Request-URI. */
            }
            else {
                self->uri_remote = tsk_object_ref(ss->from);
                self->uri_remote_target = tsk_object_ref((void*)TSIP_DIALOG_GET_STACK(self)->network.realm);
            }
        }
        else {
            TSK_DEBUG_ERROR("Invalid SIP Session id.");
        }

        tsk_safeobj_init(self);

        self->initialized = tsk_true;
        return 0;
    }
    return -1;
}

int tsip_dialog_fsm_act(tsip_dialog_t* self, tsk_fsm_action_id action_id, const tsip_message_t* message, const tsip_action_handle_t* action)
{
    int ret;
    tsip_dialog_t* copy;
    if(!self || !self->fsm) {
        TSK_DEBUG_ERROR("Invalid parameter.");
        return -1;
    }

    tsk_safeobj_lock(self);
    copy = tsk_object_ref(self); /* keep a copy because tsk_fsm_act() could destroy the dialog */
    ret = tsip_dialog_set_curr_action(copy, action);
    ret = tsk_fsm_act(copy->fsm, action_id, copy, message, copy, message, action);
    tsk_safeobj_unlock(copy);
    tsk_object_unref(copy);

    return ret;
}

/*
This function is used to know if we need to keep the same action handle after receiving a response to our last action.
*/
tsk_bool_t tsip_dialog_keep_action(const tsip_dialog_t* self, const tsip_response_t *response)
{
    if(self && response) {
        const short code = TSIP_RESPONSE_CODE(response);
        return
            TSIP_RESPONSE_IS_1XX(response) ||
            (code == 401 || code == 407 || code == 421 || code == 494) ||
            (code == 422 || code == 423);
    }
    return tsk_false;
}

int tsip_dialog_set_connected_fd(tsip_dialog_t* self, tnet_fd_t fd)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    self->connected_fd = fd;
    return 0;
}

int tsip_dialog_set_curr_action(tsip_dialog_t* self, const tsip_action_t* action)
{
    tsip_action_t* new_action;
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter.");
        return -1;
    }

    new_action = tsk_object_ref((void*)action);
    TSK_OBJECT_SAFE_FREE(self->curr_action);
    self->curr_action = new_action;
    return 0;
}

int tsip_dialog_set_lasterror_2(tsip_dialog_t* self, const char* phrase, short code, const tsip_message_t *message)
{
    if(!self || tsk_strnullORempty(phrase)) {
        TSK_DEBUG_ERROR("Invalid parameter.");
        return -1;
    }

    tsk_strupdate(&self->last_error.phrase, phrase);
    self->last_error.code = code;
    TSK_OBJECT_SAFE_FREE(self->last_error.message);
    if(message) {
        self->last_error.message = (tsip_message_t*)tsk_object_ref((void*)message);
    }
    return 0;
}

int tsip_dialog_set_lasterror(tsip_dialog_t* self, const char* phrase, short code)
{
    return tsip_dialog_set_lasterror_2(self, phrase, code, tsk_null);
}

int tsip_dialog_get_lasterror(const tsip_dialog_t* self, short *code, const char** phrase, const tsip_message_t **message)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter.");
        return -1;
    }

    if(code) {
        *code = self->last_error.code;
    }
    if(phrase) {
        *phrase = self->last_error.phrase;
    }

    if(message) {
        *message = self->last_error.message;
    }

    return 0;
}

int tsip_dialog_hangup(tsip_dialog_t *self, const tsip_action_t* action)
{
    if(self) {
        // CANCEL should only be sent for INVITE dialog
        if(self->type != tsip_dialog_INVITE || self->state == tsip_established) {
            return tsip_dialog_fsm_act(self, tsip_atype_hangup, tsk_null, action);
        }
        else {
            return tsip_dialog_fsm_act(self, tsip_atype_cancel, tsk_null, action);
        }
    }
    TSK_DEBUG_ERROR("Invalid parameter");
    return -1;
}

int tsip_dialog_shutdown(tsip_dialog_t *self, const tsip_action_t* action)
{
    if(self) {
        return tsip_dialog_fsm_act(self, tsip_atype_shutdown, tsk_null, action);
    }
    TSK_DEBUG_ERROR("Invalid parameter");
    return -1;
}

int tsip_dialog_signal_transport_error(tsip_dialog_t *self)
{
    if(self) {
        return tsip_dialog_fsm_act(self, tsip_atype_transport_error, tsk_null, tsk_null);
    }
    TSK_DEBUG_ERROR("Invalid parameter");
    return -1;
}

int tsip_dialog_remove(const tsip_dialog_t* self)
{
    return tsip_dialog_layer_remove(TSIP_DIALOG_GET_STACK(self)->layer_dialog, TSIP_DIALOG(self));
}

int tsip_dialog_cmp(const tsip_dialog_t *d1, const tsip_dialog_t *d2)
{
    if(d1 && d2) {
        if(
            tsk_strequals(d1->callid, d2->callid)
            && (tsk_strequals(d1->tag_local, d2->tag_local))
            && (tsk_strequals(d1->tag_remote, d2->tag_remote))
        ) {
            return 0;
        }
    }
    return -1;
}

int tsip_dialog_deinit(tsip_dialog_t *self)
{
    if(self) {
        if(!self->initialized) {
            TSK_DEBUG_WARN("Dialog not initialized.");
            return -2;
        }

        /* Cancel all transactions associated to this dialog (do it here before the dialog becomes unsafe) */
        tsip_transac_layer_cancel_by_dialog(TSIP_DIALOG_GET_STACK(self)->layer_transac, self);

        /* Remove the dialog from the Stream peers */
        tsip_dialog_layer_remove_callid_from_stream_peers(TSIP_DIALOG_GET_STACK(self)->layer_dialog, self->callid);

        TSK_OBJECT_SAFE_FREE(self->ss);
        TSK_OBJECT_SAFE_FREE(self->curr_action);

        TSK_OBJECT_SAFE_FREE(self->uri_local);
        TSK_FREE(self->tag_local);
        TSK_OBJECT_SAFE_FREE(self->uri_remote);
        TSK_FREE(self->tag_remote);

        TSK_OBJECT_SAFE_FREE(self->uri_remote_target);

        TSK_FREE(self->cseq_method);
        TSK_FREE(self->callid);

        TSK_FREE(self->last_error.phrase);
        TSK_OBJECT_SAFE_FREE(self->last_error.message);

        TSK_OBJECT_SAFE_FREE(self->record_routes);
        TSK_OBJECT_SAFE_FREE(self->challenges);

        TSK_OBJECT_SAFE_FREE(self->fsm);

        tsk_safeobj_deinit(self);

        self->initialized = 0;

        return 0;
    }
    return -1;
}

