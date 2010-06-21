/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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

/**@file tmsrp_media.c
 * @brief MSRP Media definition.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinymsrp/session/tmsrp_media.h"

#include "tinysdp/headers/tsdp_header_A.h"
#include "tinysdp/headers/tsdp_header_C.h"
#include "tinysdp/headers/tsdp_header_M.h"

#include "tmsrp.h"

#include "tnet_utils.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#define TMSRP_CONNECT_TIMEOUT	1500

tmsrp_session_setup_t setup_from_string(const char* setup)
{
	tmsrp_session_setup_t ret = setup_active;

	if(setup){
		if(tsk_strequals(setup, "holdconn")){
			ret = setup_holdconn;
		}
		else if(tsk_strequals(setup, "passive")){
			ret = setup_passive;
		}
		else if(tsk_strequals(setup, "actpass")){
			ret = setup_actpass;
		}
		else{
			ret = setup_active;
		}
	}
	return ret;
}

const char* setup_to_string(tmsrp_session_setup_t setup)
{
	switch(setup)
	{
	case setup_active:
		return "active";
	case setup_passive:
		return "passive";
	case setup_actpass:
		return "actpass";
	case setup_holdconn:
		return "holdconn";
	}
	return "active";
}

/* =========================== Plugin ============================= */

int	tmsrp_media_start(tmedia_t* self)
{
	int ret = -1;
	tsk_bool_t send_bodiless = tsk_false;
	tmsrp_media_t *msrp = TMSRP_MEDIA(self);
		
	if(!msrp){
		goto bail;
	}

	if(!msrp->local.M || !msrp->remote.M){
		ret = -2;
		goto bail;
	}

	if(!msrp->remote.M->C){
		ret = -3;
		goto bail;
	}
	
	// start the transport
	if((ret = tnet_transport_start(msrp->transport))){
		goto bail;
	}
	
	switch(msrp->setup){
		case setup_active:
		case setup_actpass:
			{
				//
				//	ACTIVE
				//
				if((msrp->connectedFD = tnet_transport_connectto_2(msrp->transport, msrp->remote.M->C->addr, msrp->remote.M->port)) == TNET_INVALID_FD){
					goto bail;
				}
				else{						
						if((ret = tnet_sockfd_waitUntilWritable(msrp->connectedFD, TMSRP_CONNECT_TIMEOUT))){
							TSK_DEBUG_ERROR("%d milliseconds elapsed and the socket is still not connected.", TMSRP_CONNECT_TIMEOUT);
							goto bail;
						}
						/*	draft-denis-simple-msrp-comedia-02 - 4.2.3. Setting up the connection
							Once the TCP session is established, and if the answerer was the
							active connection endpoint, it MUST send an MSRP request.  In
							particular, if it has no pending data to send, it MUST send an empty
							MSRP SEND request.  That is necessary for the other endpoint to
							authenticate this TCP session.

							...RFC 4975 - 7.1
						*/
						send_bodiless = tsk_true;
				}
				break;
			}
		default:
			{
				//
				//	PASSIVE
				//
				break;
			}
	}
	
	// create and start the receiver
	if(!msrp->receiver){
		if((msrp->receiver = tmsrp_receiver_create(msrp->config, msrp->connectedFD))){
			tnet_transport_set_callback(msrp->transport, TNET_TRANSPORT_CB_F(tmsrp_transport_layer_stream_cb), msrp->receiver);
			if((ret = tmsrp_receiver_start(msrp->receiver))){
				goto bail;
			}
		}
	}

	// create and start the sender
	if(!msrp->sender){
		if((msrp->sender = tmsrp_sender_create(msrp->config, msrp->connectedFD))){
			if((ret = tmsrp_sender_start(msrp->sender))){
				goto bail;
			}
			else if(send_bodiless){
				// ... send bodiless message
				tmsrp_request_t* BODILESS;
				if(msrp->config->To_Path && msrp->config->From_Path){
					if((BODILESS = tmsrp_create_bodiless(msrp->config->To_Path->uri, msrp->config->From_Path->uri))){
						char* str;
						if((str = tmsrp_message_tostring(BODILESS))){
							if(!tnet_sockfd_send(msrp->connectedFD, str, strlen(str), 0)){
								TSK_DEBUG_WARN("Failed to send bodiless request.");
							}
							TSK_FREE(str);
						}

						TSK_OBJECT_SAFE_FREE(BODILESS);
					}
				}
			}
		}
	}

bail:
	return ret;
}

int	tmsrp_media_pause(tmedia_t* self)
{
	tmsrp_media_t *msrp = TMSRP_MEDIA(self);
	TSK_DEBUG_INFO("tmsrp_media_pause");

	return 0;
}

int	tmsrp_media_stop(tmedia_t* self)
{
	tmsrp_media_t *msrp = TMSRP_MEDIA(self);
	
	if(msrp->sender){
		tmsrp_sender_stop(msrp->sender);
	}
	if(msrp->receiver){
		tmsrp_receiver_stop(msrp->receiver);
	}

	if(msrp->transport){
		tnet_transport_shutdown(msrp->transport);
	}
	
	return 0;
}

const tsdp_header_M_t* tmsrp_media_get_local_offer(tmedia_t* self, va_list *app)
{
	tmsrp_media_t *msrp = TMSRP_MEDIA(self);
	const tsdp_header_A_t* A;
	const char* proto = "TCP/MSRP";
	const char* sheme = "msrp";
	tsk_bool_t answer;
	tsk_bool_t ipv6;
	tsk_istr_t sessionid;
	tnet_socket_type_t type;
	tnet_ip_t ip = "127.0.0.1";
	tnet_port_t port = 0;

	if(!msrp || !msrp->transport){
		goto bail;
	}

	// get socket type
	type = tnet_transport_get_type(msrp->transport);
	// get ip and port
	tnet_transport_get_ip_n_port_2(msrp->transport, &ip, &port); 
	// answer or initial offer?
	answer = (msrp->remote.M != tsk_null);
	// using ipv6?
	ipv6 = TNET_SOCKET_TYPE_IS_IPV6(type);
	
	if(TNET_SOCKET_TYPE_IS_TLS(type)){
		proto = "TCP/TLS/MSRP";
		sheme = "msrps";
	}

	if(!msrp->local.M){
		char* path = tsk_null;
		tmsrp_uri_t* uri;
		const tsk_object_def_t* objdef;
		tsdp_header_t* header;

		tsk_strrandom(&sessionid);
		tsk_sprintf(&path, "%s://%s:%u/%s;tcp", sheme, ip, port, sessionid); //tcp is ok even if tls is used.

		if((msrp->local.M = tsdp_header_M_create(self->plugin->media, port, proto))){
			tsdp_header_M_add_headers(msrp->local.M,
				TSDP_FMT_VA_ARGS("*"),
				TSDP_HEADER_C_VA_ARGS("IN", ipv6?"IP6":"IP4", ip),
				
				TSDP_HEADER_A_VA_ARGS("path", path),
				
				tsk_null
				);
			
			if((uri = tmsrp_uri_parse(path, path?strlen(path):0))){
				if(msrp->config->From_Path){
					TSK_OBJECT_SAFE_FREE(msrp->config->From_Path);
				}
				msrp->config->From_Path = tmsrp_header_From_Path_create(uri);
				TSK_OBJECT_SAFE_FREE(uri);
			}
			TSK_FREE(path);

			//
			// Add user headers
			//
			while((objdef = va_arg(*app, const tsk_object_def_t*))){
				if((header = tsk_object_new_2(objdef, app))){
					tsdp_header_M_add(msrp->local.M, header);
					TSK_OBJECT_SAFE_FREE(header);
				}
				else break;
			}


			if(answer){ /* We are about to send 2xx INVITE(sdp) */
				/*	RFC 4145 - 4.1.  The Setup Attribute in the Offer/Answer Model
					Offer      Answer
					________________
					active     passive / holdconn
					passive    active / holdconn
					actpass    active / passive / holdconn
					holdconn   holdconn
				*/
				if((A = tsdp_header_M_findA(msrp->remote.M, "setup"))){
					tmsrp_session_setup_t setup = setup_from_string(A->value);
					switch(setup){
						case setup_actpass:
						case setup_passive:
							msrp->setup = setup_active;
							break;
						case setup_active:
							msrp->setup = setup_passive;
							break;
					}
					tsdp_header_M_add_headers(msrp->local.M,
						TSDP_HEADER_A_VA_ARGS("accept-types", "text/plain"), // FIXME: match with the offer
						TSDP_HEADER_A_VA_ARGS("connection", "new"),

						tsk_null
					);
				}
			}
			else{ /* We are about to send INVITE(sdp) */
				tsdp_header_M_add_headers(msrp->local.M,
					TSDP_HEADER_A_VA_ARGS("accept-types", "text/plain"),
					TSDP_HEADER_A_VA_ARGS("connection", "new"),
				
					tsk_null
				);
			}
		}
		
		// Common headers
		tsdp_header_M_add_headers(msrp->local.M,
			TSDP_HEADER_A_VA_ARGS("setup", setup_to_string(msrp->setup)),
			
			tsk_null
		);
	}

bail:
	return msrp->local.M;
}

const tsdp_header_M_t* tmsrp_media_get_negotiated_offer(tmedia_t* self)
{
	tmsrp_media_t *msrp = TMSRP_MEDIA(self);
	TSK_DEBUG_INFO("tmsrp_media_get_negotiated_offer");

	return tsk_null;
}

int tmsrp_media_set_remote_offer(tmedia_t* self, const tsdp_message_t* offer)
{
	tmsrp_media_t *msrp = TMSRP_MEDIA(self);
	const tsdp_header_C_t* C;
	const tsdp_header_A_t* A;
	const tsdp_header_M_t* M;
	int ret = -1;
	tsk_size_t index;
	tsk_bool_t found = tsk_false;
	tsk_bool_t answer;

	if(!offer || !msrp){
		goto bail;
	}
	
	// answer or initial offer?
	answer = (msrp ->local.M != tsk_null);
	
	// Find header M associated to our "media"
	for(index = 0; (M = (const tsdp_header_M_t*)tsdp_message_get_headerAt(offer, tsdp_htype_M, index)); index++){
		if(tsk_strequals(M->media, self->plugin->media)){
			found = tsk_true;
			break;
		}
	}

	// media found?
	if(found){
		TSK_OBJECT_SAFE_FREE(msrp->remote.M);
		if((msrp->remote.M = (tsdp_header_M_t*)tsdp_header_clone(TSDP_HEADER(M)))){
			// Find remote connection (C)
			if(!msrp->remote.M->C){ // Get Session Level Connection
				if((C = (const tsdp_header_C_t*)tsdp_message_get_header(offer, tsdp_htype_C))){
					msrp->remote.M->C = (tsdp_header_C_t*)tsdp_header_clone(TSDP_HEADER(C));
				}
			}
		}
	}
	else{
		TSK_DEBUG_WARN("Failed to match media(%s)", self->plugin->media);
		goto bail;
	}

	/* To-Path */
	if((A = tsdp_header_M_findA(msrp->remote.M, "path"))){
		tmsrp_uri_t* uri;
		if((uri = tmsrp_uri_parse(A->value, A->value?strlen(A->value):0))){
			if(msrp->config->To_Path){
				TSK_OBJECT_SAFE_FREE(msrp->config->To_Path);
			}
			msrp->config->To_Path = tmsrp_header_To_Path_create(uri);
			TSK_OBJECT_SAFE_FREE(uri);
		}
	}

	if(answer){ /* We are about to receive 2xx INVITE(sdp) */
	}
	else{ /* We are about to receive INVITE(sdp) */
		msrp->setup = setup_passive; // default value
		// OMA-TS-SIMPLE_IM-V1_0-20080903-C - 5.8.1 Negotiate direction of the MSRP connection setup
		if((A = tsdp_header_M_findA(msrp->remote.M, "setup"))){
			tmsrp_session_setup_t setup = setup_from_string(A->value);
			switch(setup){
				case setup_actpass:
				case setup_passive:
					msrp->setup = setup_active;
					break;
				case setup_active:
					msrp->setup = setup_passive;
					break;
			}
		}
	}

bail:
	return ret;
}


int tmsrp_media_perform(tmedia_t* self, tmedia_action_t action, const tsk_params_L_t* params)
{
	tmsrp_media_t *msrp = TMSRP_MEDIA(self);
	int ret = -1;

	if(!msrp || !msrp->sender){
		return -1;
	}

	switch(action){
		case tma_msrp_send_data:
			{
				const char* content;
				if((content = tsk_params_get_param_value(params, "content"))){
					ret = tsmrp_sender_send_data(msrp->sender, content, strlen(content), tsk_null);
				}
				else{
					TSK_DEBUG_ERROR("%s param not found.", "content");
				}
				break;
			}

		case tma_msrp_send_file:
			{
				const char* filepath;
				if((filepath = tsk_params_get_param_value(params, "path"))){
					ret = tsmrp_sender_send_file(msrp->sender, filepath);
				}
				else{
					TSK_DEBUG_ERROR("%s param not found.", "path");
				}
				break;
			}
	}
		
	return ret;
}
/* ======================================================== */







int tmsrp_send_file(tmsrp_media_t* self, const char* path)
{
	if(!self){
		return -1;
	}

	return 0;
}

tmsrp_send_data(tmsrp_media_t* self, const void* data, tsk_size_t size, const char* ctype)
{
	if(!self || !data || !size || !self->sender){
		return -1;
	}
	else{
		return tsmrp_sender_send_data(self->sender, data, size, ctype);
	}
}





//========================================================
//	Dummy media object definition
//
static void* tmsrp_media_ctor(tsk_object_t *self, va_list * app)
{
	tmsrp_media_t *msrp = self;
	if(msrp)
	{
		tnet_host_t local;

		// Parameters MUST appear in this order
		const char* name = va_arg(*app, const char*);
		const char* host = va_arg(*app, const char*);
		tnet_socket_type_t socket_type = va_arg(*app, tnet_socket_type_t);
		
		// init base
		tmedia_init(TMEDIA(msrp), name);

		msrp->config = tmsrp_config_create();
		msrp->setup = setup_actpass; // draft-denis-simple-msrp-comedia-02 - 4.1.1. Sending the offer
		TMEDIA(msrp)->protocol = tsk_strdup("TCP/MSRP");

		if(host == TNET_SOCKET_HOST_ANY){
			// Because wa cannot use bestsource (no dest)
			// if this is not used then the host address will be equal to "0.0.0.0" or "::"
			// when used with SIP, the stack will provide a routable IP (e.g. 192.168.16.104)
			tnet_gethostname(&local);
			msrp->transport = tnet_transport_create(local, TNET_SOCKET_PORT_ANY, socket_type, "MSRP/MSRPS transport");
		}
		else{
			msrp->transport = tnet_transport_create(host, TNET_SOCKET_PORT_ANY, socket_type, "MSRP/MSRPS transport");
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new dummy media.");
	}
	return self;
}

static void* tmsrp_media_dtor(tsk_object_t *self)
{
	tmsrp_media_t *msrp = self;
	if(msrp){
		tmedia_deinit(TMEDIA(msrp));

		TSK_OBJECT_SAFE_FREE(msrp->config);
		
		// local
		TSK_OBJECT_SAFE_FREE(msrp->local.M);
		// remote
		TSK_OBJECT_SAFE_FREE(msrp->remote.M);
		//TSK_OBJECT_SAFE_FREE(msrp->remote.C);
		// negociated
		TSK_OBJECT_SAFE_FREE(msrp->negociated.M);

		// sender
		TSK_OBJECT_SAFE_FREE(msrp->sender);
		// receiver
		TSK_OBJECT_SAFE_FREE(msrp->receiver);

		// transrport
		TSK_OBJECT_SAFE_FREE(msrp->transport);
		
		// do not close the connectedFD ...it's uo to the transport
	}
	else{
		TSK_DEBUG_ERROR("Null dummy media.");
	}

	return self;
}

static const tsk_object_def_t tmsrp_media_def_s = 
{
	sizeof(tmsrp_media_t),
	tmsrp_media_ctor,
	tmsrp_media_dtor,
	tsk_null
};

const tsk_object_def_t *tmsrp_media_def_t = &tmsrp_media_def_s;

//========================================================
//	Dummy media plugin definition
//
static const tmedia_plugin_def_t tmsrp_media_plugin_def_s = 
{
	&tmsrp_media_def_s,
	"msrp",
	"message",

	tmsrp_media_start,
	tmsrp_media_pause,
	tmsrp_media_stop,

	tmsrp_media_get_local_offer,
	tmsrp_media_get_negotiated_offer,
	tmsrp_media_set_remote_offer,

	tmsrp_media_perform
};
const tmedia_plugin_def_t *tmsrp_media_plugin_def_t = &tmsrp_media_plugin_def_s;

