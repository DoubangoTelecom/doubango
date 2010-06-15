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
/**@file trtp_manager.c
 * @brief RTP/RTCP manager.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyrtp/trtp_manager.h"

#include "tinyrtp/rtp/trtp_rtp_packet.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

// TODO: Add support for outbound DTMF (http://www.ietf.org/rfc/rfc2833.txt)

/* ======================= Transport callback ========================== */
static int trtp_transport_layer_cb(const tnet_transport_event_t* e)
{
	int ret = -1;
	const trtp_manager_t *manager = e->callback_data;
	trtp_rtp_packet_t* packet = tsk_null;

	switch(e->type){
		case event_data: {
				break;
			}
		case event_closed:
		case event_connected:
		default:{
				return 0;
			}
	}

	//
	//	RTCP
	//
	if(manager->rtcp.local_socket && manager->rtcp.local_socket->fd == e->fd){
		TSK_DEBUG_INFO("RTCP packet");
	}
	//
	// RTP
	//
	else if(manager->transport->master && (manager->transport->master->fd == e->fd)){
		if(manager->rtp.callback){
			if((packet = trtp_rtp_packet_deserialize(e->data, e->size))){
				manager->rtp.callback(manager->rtp.callback_data, packet);
				TSK_OBJECT_SAFE_FREE(packet);
			}
			else{
				TSK_DEBUG_ERROR("RTP packet === NOK");
				goto bail;
			}
		}
	}
	//
	// UNKNOWN
	//
	else{
		TSK_DEBUG_INFO("XXXX packet");
		goto bail;
	}

bail:

	return ret;
}



/** Create RTP/RTCP manager */
trtp_manager_t* trtp_manager_create(tsk_bool_t enable_rtcp, const char* local_ip, tsk_bool_t ipv6)
{
	trtp_manager_t* manager;
	if((manager = tsk_object_new(trtp_manager_def_t))){
		manager->enable_rtcp = enable_rtcp;
		manager->local_ip = tsk_strdup(local_ip);
		manager->ipv6 = ipv6;
		manager->rtp.payload_type = 127;
	}
	return manager;
}

/** Prepares the RTP/RTCP manager */
int trtp_manager_prepare(trtp_manager_t* self)
{
	uint8_t retry_count = 7;
	tnet_socket_type_t socket_type;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(self->transport){
		TSK_DEBUG_ERROR("RTP/RTCP manager already prepared");
		return -2;
	}

	socket_type = self->ipv6 ? tnet_socket_type_udp_ipv6 : tnet_socket_type_udp_ipv4;
	
	/* Creates local rtp and rtcp sockets */
	while(retry_count--){
		/* random number in the range 1024 to 65535 */
		tnet_port_t local_port = ((rand() % 64510) + 1025);
		local_port = (local_port & 0xFFFE); /* turn to even number */
		
		/* beacuse failure will cause errors in the log, print a message to alert that there is
		* nothing to worry about */
		TSK_DEBUG_INFO("RTP/RTCP manager[Begin]: Trying to bind to random ports");
		
		/* RTP */
		if((self->transport = tnet_transport_create(self->local_ip, local_port, socket_type, "RTP/RTCP Manager"))){
			/* set callback function */
			tnet_transport_set_callback(self->transport, trtp_transport_layer_cb, self);
		}
		else {
			TSK_DEBUG_ERROR("Failed to create RTP/RTCP Transport");
			return -3;
		}

		/* RTCP */
		if(self->enable_rtcp){
			if(!(self->rtcp.local_socket = tnet_socket_create(self->local_ip, local_port+1, socket_type))){
				TSK_DEBUG_WARN("Failed to bind to %d", local_port+1);
				TSK_OBJECT_SAFE_FREE(self->transport);
				continue;
			}
		}
	
		TSK_DEBUG_INFO("RTP/RTCP manager[End]: Trying to bind to random ports");
		break;
	}

	return 0;
}

/** Indicates whether the manager is already prepared or not */
tsk_bool_t trtp_manager_is_prepared(trtp_manager_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_false;
	}
	return self->transport == tsk_null ? tsk_false : tsk_true;
}

/** Sets RTP callback */
int trtp_manager_set_rtp_callback(trtp_manager_t* self, trtp_manager_rtp_cb_f callback, const void* callback_data)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	self->rtp.callback = callback;
	self->rtp.callback_data = callback_data;

	return 0;
}

/** Sets the payload type */
int trtp_manager_set_payload_type(trtp_manager_t* self, uint8_t payload_type)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	self->rtp.payload_type = payload_type;
	return 0;
}

/** Sets remote parameters for rtp session */
int trtp_manager_set_rtp_remote(trtp_manager_t* self, const char* remote_ip, tnet_port_t remote_port)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	tsk_strupdate(&self->rtp.remote_ip, remote_ip);
	self->rtp.remote_port = remote_port;
	return 0;
}

/** Sets remote parameters for rtcp session */
int trtp_manager_set_rtcp_remote(trtp_manager_t* self, const char* remote_ip, tnet_port_t remote_port)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	tsk_strupdate(&self->rtcp.remote_ip, remote_ip);
	self->rtcp.remote_port = remote_port;
	return 0;
}

/** Starts the RTP/RTCP manager */
int trtp_manager_start(trtp_manager_t* self)
{
	int ret = 0;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(self->started){
		TSK_DEBUG_WARN("RTP/RTCP manager already started");
		return 0;
	}

	if(!self->transport || !self->transport->master){
		TSK_DEBUG_ERROR("RTP/RTCP manager not prepared");
		return -2;
	}

	/* start the transport */
	if((ret = tnet_transport_start(self->transport))){
		TSK_DEBUG_ERROR("Failed to start the RTP/RTCP transport");
		return ret;
	}
	
	/* RTP */
	if((ret = tnet_sockaddr_init(self->rtp.remote_ip, self->rtp.remote_port, self->transport->master->type, &self->rtp.remote_addr))){
		tnet_transport_shutdown(self->transport);
		TSK_DEBUG_ERROR("Invalid RTP host:port [%s:%u]", self->rtp.remote_ip, self->rtp.remote_port);
		return ret;
	}

	/* RTCP */
	if(self->enable_rtcp){
		if(!self->rtcp.remote_ip){
			self->rtcp.remote_ip = tsk_strdup(self->rtp.remote_ip);
		}
		if(!self->rtcp.remote_port){
			self->rtcp.remote_port = self->rtp.remote_port;
		}
	}
	if(self->enable_rtcp && (ret = tnet_sockaddr_init(self->rtcp.remote_ip, self->rtcp.remote_port, self->rtcp.local_socket->type, &self->rtp.remote_addr))){
		TSK_DEBUG_ERROR("Invalid RTCP host:port [%s:%u]", self->rtcp.remote_ip, self->rtcp.remote_port);
		/* do not exit */
	}
	
	/* add RTCP socket to the transport */
	if(self->enable_rtcp && (ret = tnet_transport_add_socket(self->transport, self->rtcp.local_socket->fd, self->rtcp.local_socket->type, tsk_false, tsk_true/* only Meaningful for tls*/))){
		TSK_DEBUG_ERROR("Failed to add RTCP socket");
		/* do not exit */
	}

	self->started = tsk_true;

	return 0;
}

/* Encapsulate raw data into RTP packet and send it over the network */
int trtp_manager_send_rtp(trtp_manager_t* self, const void* data, tsk_size_t size, uint32_t duration, tsk_bool_t marker)
{
	trtp_rtp_packet_t* packet;
	tsk_buffer_t* buffer;
	int ret = -1;

	if(!self || !data || !size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!self->started){
		TSK_DEBUG_ERROR("RTP/RTCP manager should be started before trying to send data");
		return -2;
	}
	
	/* create packet with header */
	if(!(packet = trtp_rtp_packet_create(self->rtp.ssrc, self->rtp.seq_num++, 
		(self->rtp.timestamp += duration), self->rtp.payload_type, marker))){
		return -3;
	}

	/* set data */
	if((packet->payload.data = tsk_calloc(size, sizeof(uint8_t)))){
		memcpy(packet->payload.data, data, size);
		packet->payload.size = size;
	}

	/* serialize and send over the network */
	if((buffer = trtp_rtp_packet_serialize(packet))){
		if(/* number of bytes sent */tnet_sockfd_sendto(self->transport->master->fd, (const struct sockaddr *)&self->rtp.remote_addr, buffer->data, buffer->size)){
			ret = 0;
		}
		TSK_OBJECT_SAFE_FREE(buffer);
	}
	else{
		TSK_DEBUG_ERROR("Failed to serialize RTP packet");
		ret = -5;
		goto bail;
	}

bail:
	TSK_OBJECT_SAFE_FREE(packet);
	return ret;
}

/** Stops the RTP/RTCP manager */
int trtp_manager_stop(trtp_manager_t* self)
{
	int ret;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!self->started){
		TSK_DEBUG_WARN("RTP/RTCP manager not started");
		return 0;
	}

	if(!self->transport){
		TSK_DEBUG_ERROR("RTP/RTCP manager not prepared");
		return -2;
	}

	if(!(ret = tnet_transport_shutdown(self->transport))){
		self->started = tsk_false;
	}

	return ret;
}




//=================================================================================================
//	RTP packet object definition
//
static tsk_object_t* trtp_manager_ctor(tsk_object_t * self, va_list * app)
{
	trtp_manager_t *manager = self;
	if(manager){
		/* rtp */
		manager->rtp.timestamp = rand()^rand();
		manager->rtp.seq_num = rand()^rand();
		manager->rtp.ssrc = rand()^rand();

		/* rtcp */
	}
	return self;
}

static tsk_object_t* trtp_manager_dtor(tsk_object_t * self)
{ 
	trtp_manager_t *manager = self;
	if(manager){
		/* stop */
		if(manager->started){
			trtp_manager_stop(manager);
		}

		/* rtp */
		TSK_FREE(manager->rtp.remote_ip);

		/* rtcp */
		TSK_FREE(manager->rtcp.remote_ip);
		TSK_OBJECT_SAFE_FREE(manager->rtcp.local_socket);

		TSK_FREE(manager->local_ip);
		TSK_OBJECT_SAFE_FREE(manager->transport);
	}

	return self;
}

static const tsk_object_def_t trtp_manager_def_s = 
{
	sizeof(trtp_manager_t),
	trtp_manager_ctor, 
	trtp_manager_dtor,
	tsk_null, 
};
const tsk_object_def_t *trtp_manager_def_t = &trtp_manager_def_s;
