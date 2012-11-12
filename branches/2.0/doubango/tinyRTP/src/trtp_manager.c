/*
* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
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

*/
#include "tinyrtp/trtp_manager.h"

#include "tinyrtp/rtp/trtp_rtp_packet.h"
#include "tinyrtp/rtcp/trtp_rtcp_packet.h"
#include "tinyrtp/rtcp/trtp_rtcp_session.h"

#include "ice/tnet_ice_candidate.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_base64.h"
#include "tsk_debug.h"

#define TRTP_TRANSPORT_NAME "RTP/RTCP Manager"

#define TRTP_DISABLE_SOCKETS_BEFORE_START	0
#define TRTP_TINY_RCVBUF					(256>>1/*Will be doubled and min on linux is 256*/) /* tiny buffer used to disable receiving */
#define TRTP_BIG_RCVBUF					0x1FFFE
#define TRTP_BIG_SNDBUF					0x1FFFE

#define TRTP_DSCP_RTP_DEFAULT           0x2e


#if !defined(TRTP_PORT_RANGE_START)
#	define TRTP_PORT_RANGE_START 1024
#endif
#if !defined(TRTP_PORT_RANGE_STOP)
#	define TRTP_PORT_RANGE_STOP 65535
#endif

static int _trtp_manager_recv_data(const trtp_manager_t* self, const uint8_t* data_ptr, tsk_size_t data_size, tnet_fd_t local_fd, const struct sockaddr_storage* remote_addr);

/* ======================= Transport callback ========================== */
static int _trtp_transport_layer_cb(const tnet_transport_event_t* e)
{
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

	return _trtp_manager_recv_data((const trtp_manager_t*)e->callback_data, e->data, e->size, e->local_fd, &e->remote_addr);
}

#if 0
static int _trtp_manager_enable_sockets(trtp_manager_t* self)
{
	int rcv_buf = TRTP_BIG_RCVBUF;
	int snd_buf = TRTP_BIG_SNDBUF;
	int ret;

	if(!self->socket_disabled){
		return 0;
	}

	if(!self || !self->transport){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if((ret = setsockopt(self->transport->master->fd, SOL_SOCKET, SO_RCVBUF, (char*)&rcv_buf, sizeof(rcv_buf)))){
		TNET_PRINT_LAST_ERROR("setsockopt(SOL_SOCKET, SO_RCVBUF) has failed with error code %d", ret);
		return ret;
	}
	if((ret = setsockopt(self->transport->master->fd, SOL_SOCKET, SO_SNDBUF, (char*)&snd_buf, sizeof(snd_buf)))){
		TNET_PRINT_LAST_ERROR("setsockopt(SOL_SOCKET, SO_RCVBUF) has failed with error code %d", ret);
		return ret;
	}

	self->socket_disabled = tsk_false;
	return 0;
}
#endif

static trtp_manager_t* _trtp_manager_create(tsk_bool_t use_rtcp, const char* local_ip, tsk_bool_t ipv6)
{
	trtp_manager_t* manager;

#if HAVE_SRTP
	static tsk_bool_t __strp_initialized = tsk_false;
	err_status_t srtp_err;
	if(!__strp_initialized){
		if((srtp_err = srtp_init()) != err_status_ok){
			TSK_DEBUG_ERROR("srtp_init() failed with error code = %d", srtp_err);
		}
		__strp_initialized = (srtp_err == err_status_ok);
	}
#endif

	if((manager = tsk_object_new(trtp_manager_def_t))){
		manager->use_rtcp = use_rtcp;
		manager->local_ip = tsk_strdup(local_ip);
		manager->use_ipv6 = ipv6;
		manager->rtp.payload_type = 127;
	}
	return manager;
}

static int _trtp_manager_recv_data(const trtp_manager_t* self, const uint8_t* data_ptr, tsk_size_t data_size, tnet_fd_t local_fd, const struct sockaddr_storage* remote_addr)
{
	tsk_bool_t is_rtcp, is_stun;

	// defined when RTCP-MUX is disabled and RTCP port is equal to "RTP Port + 1"
	is_rtcp = (self->rtcp.local_socket && self->rtcp.local_socket->fd == local_fd);
	if(!is_rtcp && data_size >= 2 && (data_ptr[1] & 0x80)){
		// RFC 5761
		switch((data_ptr[1] & 0x7F)){
			case 64: case 65: 
			case 72: case 73: case 74: case 75: case 76:
			case 77: case 78:
			case 79: is_rtcp = tsk_true; break;
		}
	}
	is_stun = !is_rtcp && TNET_IS_STUN2_MSG(((uint8_t*)data_ptr), data_size);

	if(is_stun){
		static tsk_bool_t role_conflict = tsk_false;
		if(self->ice_ctx){
			return tnet_ice_ctx_recv_stun_message(self->ice_ctx, data_ptr, data_size, local_fd, remote_addr, &role_conflict);
		}
		return 0;
	}
	else if(is_rtcp){
		if(self->rtcp.session){
			#if HAVE_SRTP
			if(self->srtp_ctx_neg_remote){
				if(srtp_unprotect_rtcp(self->srtp_ctx_neg_remote->session, (void*)data_ptr, (int*)&data_size) != err_status_ok){
					TSK_DEBUG_ERROR("srtp_unprotect() failed");
					return -1;
				}
			}
			#endif
			return trtp_rtcp_session_process_rtcp_in(self->rtcp.session, data_ptr, data_size);
		}
		TSK_DEBUG_WARN("No RTCP session");
		return 0;
	}
	else{
		if(self->rtp.callback){
			trtp_rtp_packet_t* packet_rtp = tsk_null;
			#if HAVE_SRTP
			err_status_t status;
			if(self->srtp_ctx_neg_remote){
				if((status = srtp_unprotect(self->srtp_ctx_neg_remote->session, (void*)data_ptr, (int*)&data_size)) != err_status_ok){
					TSK_DEBUG_ERROR("srtp_unprotect() failed with error code=%d", (int)status);
					return -1;
				}
			}
			#endif
			if((packet_rtp = trtp_rtp_packet_deserialize(data_ptr, data_size))){
				self->rtp.callback(self->rtp.callback_data, packet_rtp);
				// forward packet to the RTCP session
				if(self->rtcp.session){
					trtp_rtcp_session_process_rtp_in(self->rtcp.session, packet_rtp, data_size);
				}
				TSK_OBJECT_SAFE_FREE(packet_rtp);
				return 0;
			}
			else{
				TSK_DEBUG_ERROR("RTP packet === NOK");
				return -1;
			}
		}
		return 0;
	}
}

static int _trtp_manager_ice_init(trtp_manager_t* self)
{
	int ret;
	const tnet_ice_candidate_t *candidate_offer, *candidate_answer_src, *candidate_answer_dest;

	if(!self || !self->ice_ctx){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!self->transport){
		// get rtp nominated symetric candidates
		ret = tnet_ice_ctx_get_nominated_symetric_candidates(self->ice_ctx, TNET_ICE_CANDIDATE_COMPID_RTP,
			&candidate_offer, &candidate_answer_src, &candidate_answer_dest);
		if(ret != 0 || !candidate_offer || !candidate_answer_src || !candidate_answer_dest){
			// If this code is called this means that ICE negotiation has failed
			// This is not really an error because it could happen if the remote peer is not an ICE agent or is an ICE-lite
			// in this case, use the first offered candidate which is the best one and already used in the "c=" line
			if(!(candidate_offer = tnet_ice_ctx_get_local_candidate_at(self->ice_ctx, 0))){
				// Must never happen as we always have at least one local "host" candidate
				TSK_DEBUG_ERROR("ICE context not ready");
				return -3;
			}
		}

		// create transport
		if(!(self->transport = tnet_transport_create_2(candidate_offer->socket, TRTP_TRANSPORT_NAME))){
			TSK_DEBUG_ERROR("Failed to create transport(%s:%u)", candidate_offer->socket->ip, candidate_offer->socket->port);
			return -4;
		}
		// set rtp local and remote IPs and ports
		if(candidate_answer_dest){ // could be "null" if remote peer is ICE-lite. Use 
			tsk_strupdate(&self->rtp.remote_ip, candidate_answer_dest->connection_addr);
			self->rtp.remote_port = candidate_answer_dest->port;
			tsk_strupdate(&self->rtp.public_ip, candidate_offer->connection_addr);
			self->rtp.public_port = candidate_offer->port;
		}

		// get rtp nominated symetric candidates
		if(self->use_rtcp){
			ret = tnet_ice_ctx_get_nominated_symetric_candidates(self->ice_ctx, TNET_ICE_CANDIDATE_COMPID_RTCP,
				&candidate_offer, &candidate_answer_src, &candidate_answer_dest);
			if(ret == 0 && candidate_offer && candidate_answer_src && candidate_answer_dest){
				// set rtp local and remote IPs and ports
				tsk_strupdate(&self->rtcp.remote_ip, candidate_answer_dest->connection_addr);
				self->rtcp.remote_port = candidate_answer_dest->port;
				tsk_strupdate(&self->rtcp.public_ip, candidate_offer->connection_addr);
				self->rtcp.public_port = candidate_offer->port;
				TSK_OBJECT_SAFE_FREE(self->rtcp.local_socket);
				self->rtcp.local_socket = tsk_object_ref((tsk_object_t*)candidate_offer->socket);
			}
		}
	}

	// set callback functions
	ret = tnet_transport_set_callback(self->transport, _trtp_transport_layer_cb, self); // NetTransport to RtpManager
	ret = tnet_ice_ctx_rtp_callback(self->ice_ctx, _trtp_manager_recv_data, self); // ICE 2 RtpManager

	return ret;
}


/** Create RTP/RTCP manager */
trtp_manager_t* trtp_manager_create(tsk_bool_t use_rtcp, const char* local_ip, tsk_bool_t ipv6)
{
	trtp_manager_t* manager;
	if((manager = _trtp_manager_create(use_rtcp, local_ip, ipv6))){
	}
	return manager;
}

/** Create RTP/RTCP manager */
trtp_manager_t* trtp_manager_create_2(struct tnet_ice_ctx_s* ice_ctx)
{
	trtp_manager_t* manager;
	const char* local_ip;
	tsk_bool_t use_ipv6, use_rtcp;

	if(!ice_ctx){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	local_ip = (use_ipv6 = tnet_ice_ctx_use_ipv6(ice_ctx)) ? "::1" : "127.0.0.1";
	use_rtcp = tnet_ice_ctx_use_rtcp(ice_ctx);

	if((manager = _trtp_manager_create(use_rtcp, local_ip, use_ipv6))){
		manager->ice_ctx = tsk_object_ref(ice_ctx);
	}
	return manager;
}

int trtp_manager_set_ice_ctx(trtp_manager_t* self, struct tnet_ice_ctx_s* ice_ctx)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid ICE context");
		return -1;
	}
	TSK_OBJECT_SAFE_FREE(self->ice_ctx);
	self->ice_ctx = tsk_object_ref(ice_ctx);
	return 0;
}

/** Prepares the RTP/RTCP manager */
int trtp_manager_prepare(trtp_manager_t* self)
{
	const char *rtp_local_ip = tsk_null, *rtcp_local_ip = tsk_null;
	tnet_port_t rtp_local_port = 0, rtcp_local_port = 0;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(self->transport){
		TSK_DEBUG_ERROR("RTP/RTCP manager already prepared");
		return -2;
	}

	if(self->ice_ctx){
		TSK_DEBUG_INFO("ICE enabled on RTP manager");
		// Get Sockets when the transport is started
		rtp_local_ip = rtcp_local_ip = self->use_ipv6 ? "::1" : "127.0.0.1";
	}
	else{
		uint8_t retry_count = 4;
		tnet_socket_type_t socket_type = self->use_ipv6 ? tnet_socket_type_udp_ipv6 : tnet_socket_type_udp_ipv4;

		/* Creates local rtp and rtcp sockets */
		while(retry_count--){
			/* random number in the range 1024 to 65535 */
			static int counter = 0;
#if 0
			tnet_port_t local_port = 6060;
#else
			tnet_port_t local_port = (((rand() ^ ++counter) % (self->port_range.stop - self->port_range.start)) + self->port_range.start);
#endif
			local_port = (local_port & 0xFFFE); /* turn to even number */

			/* beacuse failure will cause errors in the log, print a message to alert that there is
			* nothing to worry about */
			TSK_DEBUG_INFO("RTP/RTCP manager[Begin]: Trying to bind to random ports");

			/* RTP */
			if(!(self->transport = tnet_transport_create(self->local_ip, local_port, socket_type, TRTP_TRANSPORT_NAME))){
				TSK_DEBUG_ERROR("Failed to create RTP/RTCP Transport");
				return -3;
			}

			/* RTCP */
			if(self->use_rtcp){
				if(!(self->rtcp.local_socket = tnet_socket_create(self->local_ip, local_port+1, socket_type))){
					TSK_DEBUG_WARN("Failed to bind to %d", local_port+1);
					TSK_OBJECT_SAFE_FREE(self->transport);
					continue;
				}
			}

			TSK_DEBUG_INFO("RTP/RTCP manager[End]: Trying to bind to random ports");
			break;
		}// end-of-while(retry_count)

		rtp_local_ip = self->transport->master->ip;
		rtp_local_port = self->transport->master->port;
		if(self->rtcp.local_socket){
			rtcp_local_ip = self->rtcp.local_socket->ip;
			rtcp_local_port = self->rtcp.local_socket->port;

		}
	}// end-of-else(!ice)

	tsk_strupdate(&self->rtp.public_ip, rtp_local_ip);
	self->rtp.public_port = rtp_local_port;

	tsk_strupdate(&self->rtcp.public_ip, rtcp_local_ip);
	self->rtcp.public_port = rtcp_local_port;

	if(self->transport){
		/* set callback function */
		tnet_transport_set_callback(self->transport, _trtp_transport_layer_cb, self);
		/* Disable receiving until we start the transport (To avoid buffering) */
#if TRTP_DISABLE_SOCKETS_BEFORE_START
		if(!self->socket_disabled){
			int err, optval = TRTP_TINY_RCVBUF;
			if((err = setsockopt(self->transport->master->fd, SOL_SOCKET, SO_RCVBUF, (char*)&optval, sizeof(optval)))){
				TNET_PRINT_LAST_ERROR("setsockopt(SOL_SOCKET, SO_RCVBUF) has failed with error code %d", err);
			}
			self->socket_disabled = (err == 0);
		}
#endif
	}

	/* SRTP */
#if HAVE_SRTP
	{		
		trtp_srtp_ctx_init(&self->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][0], 1, HMAC_SHA1_80, self->rtp.ssrc);
		trtp_srtp_ctx_init(&self->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][1], 2, HMAC_SHA1_32, self->rtp.ssrc);
	}
#endif

	return 0;
}

/** Indicates whether the manager is already ready or not */
tsk_bool_t trtp_manager_is_ready(trtp_manager_t* self)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_false;
	}
	return (self->transport != tsk_null);
}

/** Sets NAT Traversal context */
int trtp_manager_set_natt_ctx(trtp_manager_t* self, tnet_nat_context_handle_t* natt_ctx)
{
	int ret;

	if(!self || !self->transport || !natt_ctx){
		if(self && self->ice_ctx){
			return 0; // Nat context is not needed when ICE is enabled
		}
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(!(ret = tnet_transport_set_natt_ctx(self->transport, natt_ctx))){
		tnet_ip_t public_ip = {0};
		tnet_port_t public_port = 0;
		// get RTP public IP and Port
		if(!tnet_transport_get_public_ip_n_port(self->transport, self->transport->master->fd, &public_ip, &public_port)){
			tsk_strupdate(&self->rtp.public_ip, public_ip);
			self->rtp.public_port = public_port;
		}
		// get RTCP public IP and Port
		memset(public_ip, 0, sizeof(public_ip));
		public_port = 0;
		if(self->rtcp.local_socket && !tnet_transport_get_public_ip_n_port(self->transport, self->rtcp.local_socket->fd, &public_ip, &public_port)){
			tsk_strupdate(&self->rtcp.public_ip, public_ip);
			self->rtcp.public_port = public_port;
		}
		// re-enable sockets to be able to receive STUN packets
#if 0
		_trtp_manager_enable_sockets(self);
#endif
	}
	return ret;
}

/** Sets RTP callback */
int trtp_manager_set_rtp_callback(trtp_manager_t* self, trtp_rtp_cb_f callback, const void* callback_data)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	self->rtp.callback = callback;
	self->rtp.callback_data = callback_data;

	return 0;
}

/** Sets RTCP callback */
int trtp_manager_set_rtcp_callback(trtp_manager_t* self, trtp_rtcp_cb_f callback, const void* callback_data)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	self->rtcp.callback = callback;
	self->rtcp.callback_data = callback_data;
	if(self->rtcp.session){
		return trtp_rtcp_session_set_callback(self->rtcp.session, callback, callback_data);
	}

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

int trtp_manager_set_rtp_dscp(trtp_manager_t* self, int32_t dscp)
{
    if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	self->rtp.dscp = dscp;
	return 0;
}

/** Sets remote parameters for rtp session */
int trtp_manager_set_rtp_remote(trtp_manager_t* self, const char* remote_ip, tnet_port_t remote_port)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	// if ICE is enabled then, these values will be updated when the manager start()s and call ice_init()
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
	if(self->ice_ctx){
		TSK_DEBUG_WARN("Manually setting RTCP remote IP and Port while ICE is enabled");
	}
	tsk_strupdate(&self->rtcp.remote_ip, remote_ip);
	self->rtcp.remote_port = remote_port;
	return 0;
}

int trtp_manager_set_port_range(trtp_manager_t* self, uint16_t start, uint16_t stop)
{
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	self->port_range.start = start;
	self->port_range.stop = stop;
	return 0;
}

/** Starts the RTP/RTCP manager */
int trtp_manager_start(trtp_manager_t* self)
{
	int ret = 0;
	int rcv_buf = TRTP_BIG_RCVBUF;
	int snd_buf = TRTP_BIG_SNDBUF;
    int32_t dscp_rtp;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(self->is_started){
		return 0;
	}

	// Initialize transport with ICE context
	if(self->ice_ctx && (ret = _trtp_manager_ice_init(self)) != 0){
		TSK_DEBUG_ERROR("_trtp_manager_ice_init() failed");
		return ret;
	}

	if(!self->transport && (ret = trtp_manager_prepare(self))){
		TSK_DEBUG_ERROR("Failed to prepare RTP/RTCP mamanger");
		return ret;
	}

	if(!self->transport || !self->transport->master){
		TSK_DEBUG_ERROR("RTP/RTCP manager not prepared");
		return -2;
	}

	/* Flush buffers and re-enable sockets */
	if(self->transport->master && self->socket_disabled){
		static char buff[TRTP_BIG_RCVBUF];
		tsk_size_t guard_count = 0;
#if 0
		// re-enable sockets
		_trtp_manager_enable_sockets(self);
#endif

		TSK_DEBUG_INFO("Start flushing RTP socket...");
		// Buffer should be empty ...but who know?
		// rcv() should never block() as we are always using non-blocking sockets
		while ((ret = recv(self->transport->master->fd, buff, sizeof(buff), 0)) > 0 && ++guard_count < 0xF0){
			TSK_DEBUG_INFO("Flushing RTP Buffer %d", ret);
		}
		TSK_DEBUG_INFO("End flushing RTP socket");
	}

	/* enlarge socket buffer */
	if((ret = setsockopt(self->transport->master->fd, SOL_SOCKET, SO_RCVBUF, (char*)&rcv_buf, sizeof(rcv_buf)))){
		TNET_PRINT_LAST_ERROR("setsockopt(SOL_SOCKET, SO_RCVBUF) has failed with error code %d", ret);
	}
	if((ret = setsockopt(self->transport->master->fd, SOL_SOCKET, SO_SNDBUF, (char*)&snd_buf, sizeof(snd_buf)))){
		TNET_PRINT_LAST_ERROR("setsockopt(SOL_SOCKET, SO_RCVBUF) has failed with error code %d", ret);
	}
    dscp_rtp = (self->rtp.dscp << 2);
    if((ret = setsockopt(self->transport->master->fd, IPPROTO_IP, IP_TOS, (char*)&dscp_rtp, sizeof(dscp_rtp)))){
		TNET_PRINT_LAST_ERROR("setsockopt(SOL_SOCKET, SO_RCVBUF) has failed with error code %d", ret);
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
	if(self->use_rtcp){
		tnet_fd_t local_rtcp_fd = self->rtcp.local_socket ? self->rtcp.local_socket->fd : -1;
		if(local_rtcp_fd < 0 || self->use_rtcpmux){ // use RTP local port to send RTCP packets
			local_rtcp_fd = self->transport->master->fd;
		}
		
		if(!self->rtcp.remote_ip){
			self->rtcp.remote_ip = tsk_strdup(self->rtcp.remote_ip ? self->rtcp.remote_ip : self->rtp.remote_ip);
		}
		if(!self->rtcp.remote_port){
			self->rtcp.remote_port = self->rtcp.remote_port ? self->rtcp.remote_port : (self->use_rtcpmux ? self->rtp.remote_port : (self->rtp.remote_port + 1));
		}

		if((ret = tnet_sockaddr_init(self->rtcp.remote_ip, self->rtcp.remote_port, self->transport->master->type, &self->rtcp.remote_addr))){
			TSK_DEBUG_ERROR("Invalid RTCP host:port [%s:%u]", self->rtcp.remote_ip, self->rtcp.remote_port);
			/* do not exit */
		}

		/* add RTCP socket to the transport */
		if(self->rtcp.local_socket){
			if(ret == 0 && (ret = tnet_transport_add_socket(self->transport, self->rtcp.local_socket->fd, self->rtcp.local_socket->type, tsk_false, tsk_true/* only Meaningful for tls*/))){
				TSK_DEBUG_ERROR("Failed to add RTCP socket");
				/* do not exit */
			}
		}
		/* create and start RTCP session */
		if(!self->rtcp.session && ret == 0){
			self->rtcp.session = trtp_rtcp_session_create(self->rtp.ssrc);
		}
		if(self->rtcp.session){
			ret = trtp_rtcp_session_set_callback(self->rtcp.session, self->rtcp.callback, self->rtcp.callback_data);
			if((ret = trtp_rtcp_session_start(self->rtcp.session, local_rtcp_fd, (const struct sockaddr *)&self->rtcp.remote_addr))){
				TSK_DEBUG_ERROR("Failed to start RTCP session");
				return ret;
			}
		}
	}

	/*SRTP*/
#if HAVE_SRTP
	{
		const trtp_srtp_ctx_xt* ctx_remote = &self->srtp_contexts[TRTP_SRTP_LINE_IDX_REMOTE][0];
		const trtp_srtp_ctx_xt* ctx_local = &self->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][0];

		if(ctx_remote->initialized){
			self->srtp_ctx_neg_remote = ctx_remote;
			if(ctx_local[0].crypto_type == ctx_remote->crypto_type){
				self->srtp_ctx_neg_local = &ctx_local[0];
			}
			else if(ctx_local[1].crypto_type == ctx_remote->crypto_type){
				self->srtp_ctx_neg_local = &ctx_local[1];
			}
		}
		else{
			self->srtp_ctx_neg_local = tsk_null;
			self->srtp_ctx_neg_remote = tsk_null;
		}
		// Pass SRTP session to the RTCP session manager
		trtp_rtcp_session_set_srtp_sess(self->rtcp.session, self->srtp_ctx_neg_local ? &self->srtp_ctx_neg_local->session : tsk_null);
	}
#endif /* HAVE_SRTP */

	self->is_started = tsk_true;

	return 0;
}

/* Encapsulate raw data into RTP packet and send it over the network 
* Very IMPORTANT: For voice packets, the marker bits indicates the beginning of a talkspurt */
tsk_size_t trtp_manager_send_rtp(trtp_manager_t* self, const void* data, tsk_size_t size, uint32_t duration, tsk_bool_t marker, tsk_bool_t last_packet)
{
	trtp_rtp_packet_t* packet;
	tsk_size_t ret;

	if(!self || !self->transport || !self->transport->master || !data || !size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	if(!self->is_started || !self->transport->master){
		//--TSK_DEBUG_ERROR("RTP/RTCP manager should be started before trying to send data");
		return 0;
	}

	/* create packet with header */
	if(!(packet = trtp_rtp_packet_create(self->rtp.ssrc, self->rtp.seq_num++, self->rtp.timestamp, self->rtp.payload_type, marker))){
		return 0;
	}
	if(last_packet){
		self->rtp.timestamp += duration;
	}

	/* set data */
#if 0
	if((packet->payload.data = tsk_calloc(size, sizeof(uint8_t)))){
		memcpy(packet->payload.data, data, size);
		packet->payload.size = size;
	}
#else
	packet->payload.data_const = data;
	packet->payload.size = size;
#endif

	ret = trtp_manager_send_rtp_packet(self, packet, tsk_false);
	TSK_OBJECT_SAFE_FREE(packet);
	return ret;
}

// serialize, encrypt then send the data
tsk_size_t trtp_manager_send_rtp_packet(trtp_manager_t* self, const struct trtp_rtp_packet_s* packet, tsk_bool_t bypass_encrypt)
{
	int ret = 0;
	tsk_size_t rtp_buff_pad_count = 0;
	tsk_size_t xsize;

#if HAVE_SRTP
	if(self->srtp_ctx_neg_local && !bypass_encrypt){
		rtp_buff_pad_count = (SRTP_MAX_TRAILER_LEN + 0x04);
	}
#endif

	if(!self || !packet){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}

	tsk_safeobj_lock(self);

	xsize = (trtp_rtp_packet_guess_serialbuff_size(packet) + rtp_buff_pad_count);
	if(self->rtp.serial_buffer.size < xsize){
		if(!(self->rtp.serial_buffer.ptr = tsk_realloc(self->rtp.serial_buffer.ptr, xsize))){
			TSK_DEBUG_ERROR("Failed to allocate buffer with size = %d", xsize);
			self->rtp.serial_buffer.size = 0;
			goto bail;
		}
		self->rtp.serial_buffer.size = xsize;
	}

	/* serialize and send over the network */
	if((ret = trtp_rtp_packet_serialize_to(packet, self->rtp.serial_buffer.ptr, xsize))){
		void* data_ptr = self->rtp.serial_buffer.ptr;
		int data_size = ret;
#if HAVE_SRTP
		err_status_t status;
		if(self->srtp_ctx_neg_local && !bypass_encrypt){
			if((status = srtp_protect(self->srtp_ctx_neg_local->session, data_ptr, &data_size)) != err_status_ok){
				TSK_DEBUG_ERROR("srtp_protect() failed with error code =%d", (int)status);
				goto bail;
			}
		}
#endif
		if(/* number of bytes sent */(ret = tnet_sockfd_sendto(self->transport->master->fd, (const struct sockaddr *)&self->rtp.remote_addr, data_ptr, data_size)) > 0){
			// forward packet to the RTCP session
			if(self->rtcp.session){
				trtp_rtcp_session_process_rtp_out(self->rtcp.session, packet, data_size);
			}
		}
		else ret = 0; 
	}
	else{
		TSK_DEBUG_ERROR("Failed to serialize RTP packet");
	}

bail:
	tsk_safeobj_unlock(self);
	return ret;
}

// send raw data "as is" without adding any RTP header or SRTP encryption
tsk_size_t trtp_manager_send_rtp_raw(trtp_manager_t* self, const void* data, tsk_size_t size)
{
	tsk_size_t ret;

	if(!self || !self->transport || !self->transport->master || !data || !size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	tsk_safeobj_lock(self);
	ret = tnet_sockfd_sendto(self->transport->master->fd, (const struct sockaddr *)&self->rtp.remote_addr, data, size);
	tsk_safeobj_unlock(self);
	return 0;
}

int trtp_manager_signal_pkt_loss(trtp_manager_t* self, uint32_t ssrc_media, const uint16_t* seq_nums, tsk_size_t count)
{
	if(self && self->rtcp.session){
		return trtp_rtcp_session_signal_pkt_loss(self->rtcp.session, ssrc_media, seq_nums, count);
	}
	return -1;
}
int trtp_manager_signal_frame_corrupted(trtp_manager_t* self, uint32_t ssrc_media)
{
	if(self && self->rtcp.session){
		return trtp_rtcp_session_signal_frame_corrupted(self->rtcp.session, ssrc_media);
	}
	return -1;
}

int trtp_manager_signal_jb_error(trtp_manager_t* self, uint32_t ssrc_media)
{
	if(self && self->rtcp.session){
		return trtp_rtcp_session_signal_jb_error(self->rtcp.session, ssrc_media);
	}
	return -1;
}

/** Stops the RTP/RTCP manager */
int trtp_manager_stop(trtp_manager_t* self)
{
	int ret = 0;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(self->ice_ctx){
		ret = tnet_ice_ctx_stop(self->ice_ctx);
	}

	// Stop the RTCP session first (will send BYE)
	if(self->rtcp.session){
		ret = trtp_rtcp_session_stop(self->rtcp.session);
	}

	// Free transport to force next call to start() to create new one with new sockets
	TSK_OBJECT_SAFE_FREE(self->transport);

	self->is_started = tsk_false;

	return ret;
}




//=================================================================================================
//	RTP manager object definition
//
static tsk_object_t* trtp_manager_ctor(tsk_object_t * self, va_list * app)
{
	trtp_manager_t *manager = self;
	if(manager){
		manager->port_range.start = TRTP_PORT_RANGE_START;
		manager->port_range.stop = TRTP_PORT_RANGE_STOP;

		/* rtp */
		manager->rtp.timestamp = rand()^rand();
		manager->rtp.seq_num = rand()^rand();
		manager->rtp.ssrc = rand()^rand()^(int)tsk_time_epoch();
        manager->rtp.dscp = TRTP_DSCP_RTP_DEFAULT;

		/* rtcp */

		tsk_safeobj_init(manager);
	}
	return self;
}

static tsk_object_t* trtp_manager_dtor(tsk_object_t * self)
{ 
	trtp_manager_t *manager = self;
	if(manager){
		/* stop */
		if(manager->is_started){
			trtp_manager_stop(manager);
		}

		TSK_OBJECT_SAFE_FREE(manager->transport);

		TSK_FREE(manager->local_ip);
		
		/* rtp */
		TSK_FREE(manager->rtp.remote_ip);
		TSK_FREE(manager->rtp.public_ip);
		TSK_FREE(manager->rtp.serial_buffer.ptr);

		/* rtcp */
		TSK_OBJECT_SAFE_FREE(manager->rtcp.session);
		TSK_FREE(manager->rtcp.remote_ip);
		TSK_FREE(manager->rtcp.public_ip);
		TSK_OBJECT_SAFE_FREE(manager->rtcp.local_socket);

		/* SRTP */
#if HAVE_SRTP
		{
			int i;
			for(i = 0; i < 2; ++i){
				trtp_srtp_ctx_deinit(&manager->srtp_contexts[TRTP_SRTP_LINE_IDX_LOCAL][i]);
				trtp_srtp_ctx_deinit(&manager->srtp_contexts[TRTP_SRTP_LINE_IDX_REMOTE][i]);
			}
		}
#endif

		/* ICE */
		if(manager->ice_ctx){
			tnet_ice_ctx_rtp_callback(manager->ice_ctx, tsk_null, tsk_null);
			TSK_OBJECT_SAFE_FREE(manager->ice_ctx);
		}

		tsk_safeobj_deinit(manager);
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
