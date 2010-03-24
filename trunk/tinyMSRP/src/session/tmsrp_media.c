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

#include "tnet_utils.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

/* =========================== Plugin ============================= */

int tmsrp_media_set_params(tmedia_t* self, const tsk_params_L_t* params)
{
	const tsk_param_t* param;
	tmsrp_media_t *msrp = TMSRP_MEDIA(self);
	TSK_DEBUG_INFO("tmsrp_media_set_params");
	
	// setup
	if((param = tsk_params_get_param_by_name(params, "msrp/setup"))){
		if(tsk_strequals(param->name, "holdconn")){
			msrp->setup = setup_holdconn;
		}
		else if(tsk_strequals(param->name, "passive")){
			msrp->setup = setup_passive;
		}
		else if(tsk_strequals(param->name, "actpass")){
			msrp->setup = setup_actpass;
		}
		else{
			msrp->setup = setup_active;
		}
	}

	return 0;
}

int	tmsrp_media_start(tmedia_t* self)
{
	tmsrp_media_t *msrp = TMSRP_MEDIA(self);
	TSK_DEBUG_INFO("tmsrp_media_start");

	return 0;
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
	TSK_DEBUG_INFO("tmsrp_media_stop");

	return 0;
}

const tsdp_header_M_t* tmsrp_media_get_local_offer(tmedia_t* self)
{
	tmsrp_media_t *msrp = TMSRP_MEDIA(self);
	const char* proto = "TCP/MSRP";
	const char* sheme = "msrp";
	tsk_bool_t answer;
	tsk_bool_t ipv6;
	tsk_istr_t sessionid;

	if(!msrp || !msrp->localSocket || msrp->localSocket->fd <= 0){
		goto bail;
	}

	// answer or initial offer?
	answer = (msrp->remote != tsk_null);
	// using ipv6?
	ipv6 = TNET_SOCKET_TYPE_IS_IPV6(msrp->localSocket->type);
	
	if(TNET_SOCKET_TYPE_IS_TLS(msrp->localSocket->type)){
		proto = "TCP/TLS/MSRP";
		sheme = "msrps";
	}

	if(!msrp->local){
		char* path = tsk_null;
		
		tsk_strrandom(&sessionid);
		tsk_sprintf(&path, "%s://%s:%u/%s;tcp", sheme, msrp->localSocket->ip, msrp->localSocket->port, sessionid); //tcp is ok even if tls is used.

		if((msrp->local = TSDP_HEADER_M_CREATE(self->plugin->media, msrp->localSocket->port, proto))){
			tsdp_header_M_add_headers(msrp->local,
				TSDP_FMT_VA_ARGS("*"),
				TSDP_HEADER_C_VA_ARGS("IN", ipv6?"IP6":"IP4", &msrp->localSocket->ip),

				TSDP_HEADER_A_VA_ARGS("sendrecv", tsk_null),
				TSDP_HEADER_A_VA_ARGS("path", path),
				
				
				tsk_null
				);
			TSK_FREE(path);

			if(!answer){
				tsdp_header_M_add_headers(msrp->local,
					TSDP_HEADER_A_VA_ARGS("accept-types", "text/plain"),
					TSDP_HEADER_A_VA_ARGS("setup", "actpass"),
					TSDP_HEADER_A_VA_ARGS("connection", "new"),
				
					tsk_null
				);
			}
		}
	}

bail:
	return msrp->local;
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
	TSK_DEBUG_INFO("tmsrp_media_set_remote_offer");

	return 0;
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

		tmedia_init(TMEDIA(msrp), name);

		TMEDIA(msrp)->protocol = tsk_strdup("TCP/MSRP");
		msrp->sdp_ctx = TSDP_CTX_CREATE();

		if(host == TNET_SOCKET_HOST_ANY){
			// Because wa cannot use bestsource (no dest)
			// if this is not used then the host address will be equal to "0.0.0.0" or "::"
			// when used with SIP, the stack will provide a routable IP (e.g. 192.168.16.104)
			tnet_gethostname(&local);
			msrp->localSocket = TNET_SOCKET_CREATE(local, TNET_SOCKET_PORT_ANY, socket_type);
		}
		else{
			msrp->localSocket = TNET_SOCKET_CREATE(host, TNET_SOCKET_PORT_ANY, socket_type);
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
		tmedia_deinit(TMEDIA(msrp));
		TSK_OBJECT_SAFE_FREE(msrp->sdp_ctx);

		TSK_OBJECT_SAFE_FREE(msrp->local);
		TSK_OBJECT_SAFE_FREE(msrp->remote);
		TSK_OBJECT_SAFE_FREE(msrp->negociated);

		tnet_sockfd_close(&msrp->connectedFD);
		TSK_OBJECT_SAFE_FREE(msrp->localSocket);
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

