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

/**@file tmsrp_media.c
 * @brief MSRP Media definition.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyMSRP/session/tmsrp_media.h"

#include "tinySDP/headers/tsdp_header_A.h"
#include "tinySDP/headers/tsdp_header_C.h"
#include "tinySDP/headers/tsdp_header_M.h"

#include "tnet_utils.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#define TMSRP_CONNECT_TIMEOUT	2500

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

int tmsrp_media_set_params(tmedia_t* self, const tsk_params_L_t* params)
{
	const tsk_param_t* param;
	tmsrp_media_t *msrp = TMSRP_MEDIA(self);
	TSK_DEBUG_INFO("tmsrp_media_set_params");
	
	// setup
	if((param = tsk_params_get_param_by_name(params, "msrp/setup"))){
		msrp->setup = setup_from_string(param->value);
	}

	return 0;
}

int	tmsrp_media_start(tmedia_t* self)
{
	int ret = -1;
	tmsrp_media_t *msrp = TMSRP_MEDIA(self);
	struct sockaddr_storage to;
	
	if(!msrp || !msrp->local.socket || msrp->local.socket->fd <= 0){
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

	switch(msrp->setup){
		case setup_active:
		case setup_actpass:
			{
				//
				//	ACTIVE
				//
				if((ret = tnet_sockaddr_init(msrp->remote.M->C->addr, msrp->remote.M->port, msrp->local.socket->type, &to))){
					goto bail;
				}
				if((ret = tnet_sockfd_connetto(msrp->local.socket->fd, &to))){
					goto bail;
				}
				else{
					msrp->connectedFD = msrp->local.socket->fd;
					if((ret = tnet_sockfd_waitUntilWritable(msrp->connectedFD, TMSRP_CONNECT_TIMEOUT))){
						TSK_DEBUG_ERROR("%d milliseconds elapsed and the socket is still not connected.", TMSRP_CONNECT_TIMEOUT);
						goto bail;
					}
					/*		draft-denis-simple-msrp-comedia-02 - 4.2.3. Setting up the connection
						   Once the TCP session is established, and if the answerer was the
						   active connection endpoint, it MUST send an MSRP request.  In
						   particular, if it has no pending data to send, it MUST send an empty
						   MSRP SEND request.  That is necessary for the other endpoint to
						   authenticate this TCP session.

						   ...RFC 4975 - 7.1
				   */
					// ... send bodiless message
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

	// create and start the sender
	if(!msrp->sender){
		if((msrp->sender = TMSRP_SENDER_CREATE(msrp->remote.M->C->addr, msrp->remote.M->port))){
			if((ret = tmsrp_sender_start(msrp->sender))){
				goto bail;
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
	
	return 0;
}

const tsdp_header_M_t* tmsrp_media_get_local_offer(tmedia_t* self)
{
	tmsrp_media_t *msrp = TMSRP_MEDIA(self);
	const tsdp_header_A_t* A;
	const char* proto = "TCP/MSRP";
	const char* sheme = "msrp";
	tsk_bool_t answer;
	tsk_bool_t ipv6;
	tsk_istr_t sessionid;

	if(!msrp || !msrp->local.socket || msrp->local.socket->fd <= 0){
		goto bail;
	}

	// answer or initial offer?
	answer = (msrp->remote.M != tsk_null);
	// using ipv6?
	ipv6 = TNET_SOCKET_TYPE_IS_IPV6(msrp->local.socket->type);
	
	if(TNET_SOCKET_TYPE_IS_TLS(msrp->local.socket->type)){
		proto = "TCP/TLS/MSRP";
		sheme = "msrps";
	}

	if(!msrp->local.M){
		char* path = tsk_null;
		
		tsk_strrandom(&sessionid);
		tsk_sprintf(&path, "%s://%s:%u/%s;tcp", sheme, msrp->local.socket->ip, msrp->local.socket->port, sessionid); //tcp is ok even if tls is used.

		if((msrp->local.M = TSDP_HEADER_M_CREATE(self->plugin->media, msrp->local.socket->port, proto))){
			tsdp_header_M_add_headers(msrp->local.M,
				TSDP_FMT_VA_ARGS("*"),
				TSDP_HEADER_C_VA_ARGS("IN", ipv6?"IP6":"IP4", &msrp->local.socket->ip),

				TSDP_HEADER_A_VA_ARGS("sendrecv", tsk_null),
				TSDP_HEADER_A_VA_ARGS("path", path),
				
				
				tsk_null
				);
			TSK_FREE(path);

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
	size_t index;
	tsk_bool_t found = tsk_false;
	tsk_bool_t answer;

	if(!offer || !msrp || !msrp->local.socket || msrp->local.socket->fd <= 0){
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
/* ======================================================== */







int tmsrp_send_file(tmsrp_media_t* self, const char* path)
{
	if(!self){
		return -1;
	}

	return 0;
}

int tmsrp_send_text(tmsrp_media_t* self, const char* text, const char* ctype)
{
	if(!self){
		return -1;
	}

	return 0;
}





//========================================================
//	Dummy media object definition
//
static void* tmsrp_media_create(tsk_object_t *self, va_list * app)
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

		msrp->setup = setup_actpass; // draft-denis-simple-msrp-comedia-02 - 4.1.1. Sending the offer
		TMEDIA(msrp)->protocol = tsk_strdup("TCP/MSRP");

		if(host == TNET_SOCKET_HOST_ANY){
			// Because wa cannot use bestsource (no dest)
			// if this is not used then the host address will be equal to "0.0.0.0" or "::"
			// when used with SIP, the stack will provide a routable IP (e.g. 192.168.16.104)
			tnet_gethostname(&local);
			msrp->local.socket = TNET_SOCKET_CREATE(local, TNET_SOCKET_PORT_ANY, socket_type);
		}
		else{
			msrp->local.socket = TNET_SOCKET_CREATE(host, TNET_SOCKET_PORT_ANY, socket_type);
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new dummy media.");
	}
	return self;
}

static void* tmsrp_media_destroy(tsk_object_t *self)
{
	tmsrp_media_t *msrp = self;
	if(msrp){
		tsk_bool_t closeFD = (msrp->local.socket && msrp->local.socket->fd != msrp->connectedFD);
		tmedia_deinit(TMEDIA(msrp));
		
		// local
		TSK_OBJECT_SAFE_FREE(msrp->local.M);
		TSK_OBJECT_SAFE_FREE(msrp->local.socket);
		// remote
		TSK_OBJECT_SAFE_FREE(msrp->remote.M);
		//TSK_OBJECT_SAFE_FREE(msrp->remote.C);
		// negociated
		TSK_OBJECT_SAFE_FREE(msrp->negociated.M);

		// sender
		TSK_OBJECT_SAFE_FREE(msrp->sender);
		
		if(closeFD){
			tnet_sockfd_close(&msrp->connectedFD);
		}
	}
	else{
		TSK_DEBUG_ERROR("Null dummy media.");
	}

	return self;
}
static int tmsrp_media_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	return -1;
}

static const tsk_object_def_t tmsrp_media_def_s = 
{
	sizeof(tmsrp_media_t),
	tmsrp_media_create,
	tmsrp_media_destroy,
	tmsrp_media_cmp
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

	tmsrp_media_set_params,

	tmsrp_media_start,
	tmsrp_media_pause,
	tmsrp_media_stop,

	tmsrp_media_get_local_offer,
	tmsrp_media_get_negotiated_offer,
	tmsrp_media_set_remote_offer
};
const tmedia_plugin_def_t *tmsrp_media_plugin_def_t = &tmsrp_media_plugin_def_s;

