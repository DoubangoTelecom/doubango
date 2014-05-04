/* Copyright (C) 2014 Mamadou DIOP.
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
#include "turn/tnet_turn_session.h"

#include "stun/tnet_stun_pkt.h"
#include "stun/tnet_stun_utils.h"

#include "tinynet.h"

#include "tsk_string.h"
#include "tsk_timer.h"
#include "tsk_time.h"
#include "tsk_safeobj.h"
#include "tsk_debug.h"

#define kTurnTransportFriendlyName "TURN transport"

#define TNET_TURN_SESSION_TIMOUT_GET_BEST_SEC(u_timeout_in_sec) ((u_timeout_in_sec)*990) // small delay for code execution
#define TNET_TURN_SESSION_TIMOUT_GET_BEST_MILLIS(u_timeout_in_millis) (((u_timeout_in_millis)*990)/1000) // small delay for code execution

#define TNET_TURN_SESSION_TIMER_SCHEDULE_SEC(_p_self, u_id, u_timeout_in_sec) \
	(u_id) = tsk_timer_manager_schedule((_p_self)->timer.p_mgr, TNET_TURN_SESSION_TIMOUT_GET_BEST_SEC((u_timeout_in_sec)), _tnet_turn_session_timer_callback, (_p_self))
#define TNET_TURN_SESSION_TIMER_SCHEDULE_MILLIS(_p_self, u_id, u_timeout_in_millis) \
	(u_id) = tsk_timer_manager_schedule((_p_self)->timer.p_mgr, TNET_TURN_SESSION_TIMOUT_GET_BEST_MILLIS((u_timeout_in_millis)), _tnet_turn_session_timer_callback, (_p_self))

typedef tnet_stun_pkt_t tnet_turn_pkt_t;

typedef struct tnet_turn_session_s
{
	TSK_DECLARE_OBJECT;

	tsk_bool_t b_prepared;
	tsk_bool_t b_started;
	tsk_bool_t b_chanbind_ok;
	tsk_bool_t b_alloc_ok;
	
	uint8_t u_req_transport;
	uint16_t u_chan_num;

	uint32_t u_lifetime_alloc_in_sec;
	uint32_t u_lifetime_createperm_in_sec;
	uint32_t u_lifetime_chanbind_in_sec;

	tnet_turn_pkt_t* p_pkt_alloc;
	tnet_turn_pkt_t* p_pkt_createperm;
	tnet_stun_pkt_t* p_pkt_sendind;
	tnet_stun_pkt_t* p_pkt_chanbind;
	tnet_stun_pkt_t* p_pkt_refresh;

	void* p_buff_send_ptr;
	tsk_size_t u_buff_send_size;

	void* p_buff_chandata_ptr;
	tsk_size_t u_buff_chandata_size;

	char* p_rel_ip;
	uint16_t u_rel_port;
	tsk_bool_t b_rel_ipv6;

	char* p_srflx_ip;
	uint16_t u_srflx_port;
	tsk_bool_t b_srflx_ipv6;

	tnet_stun_addr_t addr_peer_ip;
	char* p_addr_peer_ip;
	uint16_t u_addr_peer_port;
	tsk_bool_t b_peer_perm_ok;
	tsk_bool_t b_peer_ipv6;

	struct {
		char* p_usr_name;
		char* p_pwd;
	} cred;

	struct {
		tnet_turn_session_callback_f f_fun;
		struct tnet_turn_session_event_xs e;
	} cb;

	struct {
		tsk_timer_manager_handle_t *p_mgr;
		tsk_timer_id_t u_timer_id_refresh; // To refresh Alloc (Send Refresh Indication)
		tsk_timer_id_t u_timer_id_createperm;
		tsk_timer_id_t u_timer_id_chanbind;
		struct {
			struct {
				tsk_timer_id_t id;
				uint64_t u_timeout;
			} alloc;
			struct {
				tsk_timer_id_t id;
				uint64_t u_timeout;
			} createperm;
			struct {
				tsk_timer_id_t id;
				uint64_t u_timeout;
			} chanbind;
			struct {
				tsk_timer_id_t id;
				uint64_t u_timeout;
			} refresh;
		} rtt; // retransmit (UDP only)
	} timer;

	tnet_socket_t* p_lcl_sock;
	struct sockaddr_storage srv_addr;
	struct tnet_transport_s* p_transport;

	TSK_DECLARE_SAFEOBJ;
}
tnet_turn_session_t;

static uint16_t _tnet_turn_session_get_unique_chan_num();
static int _tnet_turn_session_send_chandata(tnet_turn_session_t* p_self, const void* pc_buff_ptr, tsk_size_t u_buff_size);
static int _tnet_turn_session_send_refresh(tnet_turn_session_t* p_self);
static int _tnet_turn_session_send_buff(tnet_turn_session_t* p_self, const void* pc_buff_ptr, tsk_size_t u_buff_size);
static int _tnet_turn_session_send_pkt(tnet_turn_session_t* p_self, const tnet_turn_pkt_t *pc_pkt);
static int _tnet_turn_session_process_incoming_pkt(struct tnet_turn_session_s* p_self, const tnet_turn_pkt_t *pc_pkt);
static int _tnet_turn_session_process_err420_pkt(tnet_turn_pkt_t *p_pkt_req, const tnet_turn_pkt_t *pc_pkt_resp420);
static int _tnet_turn_session_transport_layer_dgram_cb(const tnet_transport_event_t* e);
static int _tnet_turn_session_transport_layer_stream_cb(const tnet_transport_event_t* e);
static int _tnet_turn_session_timer_callback(const void* pc_arg, tsk_timer_id_t timer_id);

#define _tnet_turn_session_raise_event0(_p_self, _e_type) \
	if ((_p_self)->cb.f_fun) { \
		(_p_self)->cb.e.e_type = _e_type; \
		(_p_self)->cb.f_fun(&(_p_self)->cb.e); \
	}
#define _tnet_turn_session_raise_event1(_p_self, _e_type, _pc_data_ptr, _u_data_size) \
	if ((_p_self)->cb.f_fun) { \
		(_p_self)->cb.e.e_type = _e_type; \
		(_p_self)->cb.e.data.pc_data_ptr = (_pc_data_ptr); \
		(_p_self)->cb.e.data.u_data_size = (_u_data_size); \
		(_p_self)->cb.f_fun(&(_p_self)->cb.e); \
	}
#define _tnet_turn_session_raise_event_alloc_ok(p_self) _tnet_turn_session_raise_event0((p_self), tnet_turn_session_event_type_alloc_ok)
#define _tnet_turn_session_raise_event_alloc_nok(p_self) _tnet_turn_session_raise_event0((p_self), tnet_turn_session_event_type_alloc_nok)
#define _tnet_turn_session_raise_event_refresh_ok(p_self) _tnet_turn_session_raise_event0((p_self), tnet_turn_session_event_type_refresh_ok)
#define _tnet_turn_session_raise_event_refresh_nok(p_self) _tnet_turn_session_raise_event0((p_self), tnet_turn_session_event_type_refresh_nok)
#define _tnet_turn_session_raise_event_createperm_ok(p_self) _tnet_turn_session_raise_event0((p_self), tnet_turn_session_event_type_perm_ok)
#define _tnet_turn_session_raise_event_createperm_nok(p_self) _tnet_turn_session_raise_event0((p_self), tnet_turn_session_event_type_perm_nok)
#define _tnet_turn_session_raise_event_chanbind_ok(p_self) _tnet_turn_session_raise_event0((p_self), tnet_turn_session_event_type_chanbind_ok)
#define _tnet_turn_session_raise_event_chanbind_nok(p_self) _tnet_turn_session_raise_event0((p_self), tnet_turn_session_event_type_chanbind_nok)
#define _tnet_turn_session_raise_event_recv_data(p_self, pc_data_ptr, u_data_size) _tnet_turn_session_raise_event1((p_self), tnet_turn_session_event_type_recv_data, (pc_data_ptr), (u_data_size))


int tnet_turn_session_create(struct tnet_socket_s* p_lcl_sock, const char* pc_srv_ip, uint16_t u_srv_port, struct tnet_turn_session_s** pp_self)
{
	int ret;
	extern const tsk_object_def_t *tnet_turn_session_def_t;
	tnet_turn_session_t* p_self;
	if (!p_lcl_sock || !TNET_SOCKET_IS_VALID(p_lcl_sock) || !pc_srv_ip || !u_srv_port || !pp_self) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if (!(p_self = tsk_object_new(tnet_turn_session_def_t))) {
		TSK_DEBUG_ERROR("Failed to create 'tnet_turn_session_def_t' object");
		return -2;
	}
	if ((ret = tnet_sockaddr_init(pc_srv_ip, u_srv_port, p_lcl_sock->type, &p_self->srv_addr))) {
		TSK_DEBUG_ERROR("Invalid TURN SRV address [%s:%u]", pc_srv_ip, u_srv_port);
		goto bail;
	}
	p_self->p_lcl_sock = tsk_object_ref(p_lcl_sock);
	p_self->u_req_transport = TNET_SOCKET_TYPE_IS_DGRAM(p_self->p_lcl_sock->type) ? IPPROTO_UDP : IPPROTO_TCP;
	*pp_self = p_self;
bail:
	if (ret) {
		TSK_OBJECT_SAFE_FREE(p_self);
	}
	return ret;
}

int tnet_turn_session_create_2(const char* pc_lcl_ip, uint16_t u_lcl_port, enum tnet_socket_type_e e_lcl_type, const char* pc_srv_ip, uint16_t u_srv_port, struct tnet_turn_session_s** pp_self)
{
	tnet_socket_t* p_lcl_sock;
	int ret;
	if (!pc_srv_ip || !u_srv_port || !pp_self) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(!(p_lcl_sock = tnet_socket_create(pc_lcl_ip, u_lcl_port, e_lcl_type))) {
		TSK_DEBUG_ERROR("Failed to create socket(%s:%u$%d)", pc_lcl_ip, u_lcl_port, e_lcl_type);
		return -2;
	}
	ret = tnet_turn_session_create(p_lcl_sock, pc_srv_ip, u_srv_port, pp_self);
	TSK_OBJECT_SAFE_FREE(p_lcl_sock);
	return ret;
}

int tnet_turn_session_set_cred(tnet_turn_session_t* p_self, const char* pc_usr_name, const char* pc_pwd)
{
	if (!p_self || !pc_usr_name || !pc_pwd) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	tsk_strupdate(&p_self->cred.p_usr_name, pc_usr_name);
	tsk_strupdate(&p_self->cred.p_pwd, pc_pwd);

	return 0;
}

int tnet_turn_session_set_callback(struct tnet_turn_session_s* p_self, tnet_turn_session_callback_f f_fun, const void* pc_usr_data)
{
	if (!p_self) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	p_self->cb.f_fun = f_fun;
	p_self->cb.e.pc_usr_data = pc_usr_data;
	return 0;
}

int tnet_turn_session_prepare(tnet_turn_session_t* p_self)
{
	int ret = 0;

	if (!p_self) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(p_self);

	if (p_self->b_prepared) {
		goto bail;
	}

	p_self->timer.rtt.alloc.id = TSK_INVALID_TIMER_ID;
	p_self->timer.rtt.chanbind.id = TSK_INVALID_TIMER_ID;
	p_self->timer.rtt.createperm.id = TSK_INVALID_TIMER_ID;
	p_self->timer.rtt.refresh.id = TSK_INVALID_TIMER_ID;

	p_self->timer.u_timer_id_refresh = TSK_INVALID_TIMER_ID;
	p_self->timer.u_timer_id_createperm = TSK_INVALID_TIMER_ID;
	p_self->timer.u_timer_id_chanbind = TSK_INVALID_TIMER_ID;
	p_self->u_lifetime_alloc_in_sec = kTurnAllocationTimeOutInSec;
	p_self->u_lifetime_createperm_in_sec = kTurnPermissionTimeOutInSec;
	p_self->u_lifetime_chanbind_in_sec = kTurnChannelBindingTimeOutInSec;

	TSK_OBJECT_SAFE_FREE(p_self->p_pkt_alloc);
	TSK_OBJECT_SAFE_FREE(p_self->p_pkt_chanbind);
	TSK_OBJECT_SAFE_FREE(p_self->p_pkt_createperm);
	TSK_OBJECT_SAFE_FREE(p_self->p_pkt_refresh);
	TSK_OBJECT_SAFE_FREE(p_self->p_pkt_sendind);

	// create timer manager
	if (!p_self->timer.p_mgr && !(p_self->timer.p_mgr = tsk_timer_manager_create())) {
		TSK_DEBUG_ERROR("Failed to create timer manager");
        ret = -4;
		goto bail;
	}

	// create transport
    if (!p_self->p_transport && !(p_self->p_transport = tnet_transport_create_2(p_self->p_lcl_sock, kTurnTransportFriendlyName))) {
        TSK_DEBUG_ERROR("Failed to create %s Transport", kTurnTransportFriendlyName);
        ret = -5;
		goto bail;
    }
    // set transport callback
	if ((ret = tnet_transport_set_callback(p_self->p_transport, TNET_SOCKET_TYPE_IS_DGRAM(p_self->p_lcl_sock->type) ? _tnet_turn_session_transport_layer_dgram_cb : _tnet_turn_session_transport_layer_stream_cb, p_self))) {
        goto bail;
    }	

	p_self->b_prepared = tsk_true;
bail:
	tsk_safeobj_unlock(p_self);
	return ret;
}

int tnet_turn_session_start(tnet_turn_session_t* p_self)
{
	int ret = 0;

	if (!p_self) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(p_self);

	if (p_self->b_started) {
		goto bail;
	}
	if (!p_self->b_prepared) {
		TSK_DEBUG_ERROR("TURN session not prepared yet");
		ret = -2;
		goto bail;
	}

	// start timer manager
	if ((ret = tsk_timer_manager_start(p_self->timer.p_mgr))) {
		TSK_DEBUG_ERROR("Failed to start TURN timer manager");
		goto bail;
	}	

	// start network transport
	if ((ret = tnet_transport_start(p_self->p_transport))) {
		TSK_DEBUG_ERROR("Failed to start TURN transport");
		goto bail;
	}	

	p_self->b_started = tsk_true;
bail:
	tsk_safeobj_unlock(p_self);
	return ret;
}

int tnet_turn_session_allocate(tnet_turn_session_t* p_self)
{
	int ret = 0;

	if (!p_self) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(p_self);

	if (!p_self->b_started) {
		TSK_DEBUG_ERROR("TURN session not started yet");
		ret = -3;
		goto bail;
	}

	// create Allocate Request
	p_self->b_alloc_ok = tsk_false;
	p_self->timer.rtt.alloc.id = TSK_INVALID_TIMER_ID;
	TSK_OBJECT_SAFE_FREE(p_self->p_pkt_alloc);
	if ((ret = tnet_stun_pkt_create_empty(tnet_stun_pkt_type_allocate_request, &p_self->p_pkt_alloc))) {
		TSK_DEBUG_ERROR("Failed to create TURN Allocate request");
		goto bail;
	}
	// add attributes
	p_self->p_pkt_alloc->opt.dontfrag = 0;
	ret = tnet_stun_pkt_attrs_add(p_self->p_pkt_alloc,
		TNET_STUN_PKT_ATTR_ADD_LIFETIME(p_self->u_lifetime_alloc_in_sec),
		TNET_STUN_PKT_ATTR_ADD_REQUESTED_TRANSPORT(p_self->u_req_transport),
		TNET_STUN_PKT_ATTR_ADD_SOFTWARE_ZT(kStunSoftware), // recommended for Alloc and Refresh
		TNET_STUN_PKT_ATTR_ADD_NULL());
	if (ret) {
		goto bail;
	}
	
	if ((ret = _tnet_turn_session_send_pkt(p_self, p_self->p_pkt_alloc))) {
		goto bail;
	}
	if (p_self->u_req_transport == IPPROTO_UDP) {
		p_self->timer.rtt.alloc.u_timeout = kStunUdpRetransmitTimoutMinInMs;
		TNET_TURN_SESSION_TIMER_SCHEDULE_MILLIS(p_self, p_self->timer.rtt.alloc.id, p_self->timer.rtt.alloc.u_timeout);
	}

bail:
	tsk_safeobj_unlock(p_self);
	return ret;
}

int tnet_turn_session_get_relayed_addr(const struct tnet_turn_session_s* p_self, char** pp_ip, uint16_t *pu_port, tsk_bool_t *pb_ipv6)
{
	int ret = 0;

	if (!p_self || !pp_ip || !pu_port || !pb_ipv6) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(p_self);

	if (!p_self->b_started) {
		TSK_DEBUG_ERROR("TURN session not started yet");
		ret = -3;
		goto bail;
	}
	if (!p_self->b_alloc_ok) {
		TSK_DEBUG_ERROR("No active TURN allocation yet");
		ret = -4;
		goto bail;
	}

	tsk_strupdate(pp_ip, p_self->p_rel_ip);
	*pu_port = p_self->u_rel_port;
	*pb_ipv6 = p_self->b_rel_ipv6;

bail:
	tsk_safeobj_unlock(p_self);
	return ret;
}

int tnet_turn_session_get_srflx_addr(const tnet_turn_session_t* p_self, char** pp_ip, uint16_t *pu_port, tsk_bool_t *pb_ipv6)
{
	int ret = 0;

	if (!p_self || !pp_ip || !pu_port || !pb_ipv6) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(p_self);

	if (!p_self->b_started) {
		TSK_DEBUG_ERROR("TURN session not started yet");
		ret = -3;
		goto bail;
	}
	if (!p_self->b_alloc_ok) {
		TSK_DEBUG_ERROR("No active TURN allocation yet");
		ret = -4;
		goto bail;
	}

	tsk_strupdate(pp_ip, p_self->p_srflx_ip);
	*pu_port = p_self->u_srflx_port;
	*pb_ipv6 = p_self->b_srflx_ipv6;

bail:
	tsk_safeobj_unlock(p_self);
	return ret;
}

int tnet_turn_session_createpermission(tnet_turn_session_t* p_self, const char* pc_peer_addr, uint16_t u_peer_port)
{
	int ret = 0;
	tnet_stun_addr_t peer_addr;
	tsk_bool_t b_ipv6;

	if (!p_self || !pc_peer_addr || !u_peer_port) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(p_self);

	if (!p_self->b_started) {
		TSK_DEBUG_ERROR("TURN session not started yet");
		ret = -3;
		goto bail;
	}
	if (!p_self->b_alloc_ok) {
		TSK_DEBUG_ERROR("No active TURN allocation yet");
		ret = -4;
		goto bail;
	}

	b_ipv6 = TNET_SOCKET_TYPE_IS_IPV6(p_self->p_lcl_sock->type);
	if ((ret = tnet_stun_utils_inet_pton(b_ipv6, pc_peer_addr, &peer_addr))) {
		TSK_DEBUG_ERROR("inet_pton(%s,IPv6=%d) failed", pc_peer_addr, b_ipv6);
		goto bail;
	}

	// create CreatePermission Request
	p_self->b_peer_perm_ok = tsk_false;
	p_self->timer.rtt.createperm.id = TSK_INVALID_TIMER_ID;
	TSK_OBJECT_SAFE_FREE(p_self->p_pkt_createperm);
	if ((ret = tnet_stun_pkt_create_empty(tnet_stun_pkt_type_createpermission_request, &p_self->p_pkt_createperm))) {
		TSK_DEBUG_ERROR("Failed to create TURN CreatePermission request");
		goto bail;
	}
	// add authinfo
	tnet_stun_pkt_auth_copy(p_self->p_pkt_createperm, p_self->cred.p_usr_name, p_self->cred.p_pwd, p_self->p_pkt_alloc);
	// add attributes
	p_self->p_pkt_createperm->opt.dontfrag = 0;
	ret = tnet_stun_pkt_attrs_add(p_self->p_pkt_createperm,
		/* Must not add LIFETIME and there is no way to delete permission */
		TNET_STUN_PKT_ATTR_ADD_XOR_PEER_ADDRESS(b_ipv6 ? tnet_stun_address_family_ipv6 : tnet_stun_address_family_ipv4, u_peer_port, peer_addr),
		TNET_STUN_PKT_ATTR_ADD_NULL());
	if (ret) {
		goto bail;
	}

	p_self->b_peer_ipv6 = b_ipv6;
	p_self->u_addr_peer_port = u_peer_port;
	memcpy(p_self->addr_peer_ip, peer_addr, sizeof(peer_addr));
	tsk_strupdate(&p_self->p_addr_peer_ip, pc_peer_addr);
	TSK_OBJECT_SAFE_FREE(p_self->p_pkt_sendind); // force updating XOR-PEER-ADDRESS
	
	if ((ret = _tnet_turn_session_send_pkt(p_self, p_self->p_pkt_createperm))) {
		goto bail;
	}
	if (p_self->u_req_transport == IPPROTO_UDP) {
		p_self->timer.rtt.createperm.u_timeout = kStunUdpRetransmitTimoutMinInMs;
		TNET_TURN_SESSION_TIMER_SCHEDULE_MILLIS(p_self, p_self->timer.rtt.createperm.id, p_self->timer.rtt.createperm.u_timeout);
	}

bail:
	tsk_safeobj_unlock(p_self);
	return ret;
}

int tnet_turn_session_chanbind(tnet_turn_session_t* p_self)
{
	int ret = 0;

	if (!p_self) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(p_self);

	if (!p_self->b_started) {
		TSK_DEBUG_ERROR("TURN session not started yet");
		ret = -3;
		goto bail;
	}
	if (!p_self->b_alloc_ok) {
		TSK_DEBUG_ERROR("No active TURN allocation yet");
		ret = -4;
		goto bail;
	}

	// create ChannelBind Request if doesn't exist (ChannelBind refresh *must* have same id)
	p_self->b_chanbind_ok = tsk_false;
	p_self->timer.rtt.chanbind.id = TSK_INVALID_TIMER_ID;
	if (!p_self->p_pkt_chanbind) {
		p_self->u_chan_num = _tnet_turn_session_get_unique_chan_num();
		if ((ret = tnet_stun_pkt_create_empty(tnet_stun_pkt_type_channelbind_request, &p_self->p_pkt_chanbind))) {
			TSK_DEBUG_ERROR("Failed to create TURN ChannelBind request");
			goto bail;
		}
		// add authentication info
		tnet_stun_pkt_auth_copy(p_self->p_pkt_chanbind, p_self->cred.p_usr_name, p_self->cred.p_pwd, p_self->p_pkt_alloc);
		// add attributes
		p_self->p_pkt_chanbind->opt.dontfrag = 0;
		ret = tnet_stun_pkt_attrs_add(p_self->p_pkt_chanbind,
			/* Must not add LIFETIME and there is no way to delete permission */
			TNET_STUN_PKT_ATTR_ADD_XOR_PEER_ADDRESS(p_self->b_peer_ipv6 ? tnet_stun_address_family_ipv6 : tnet_stun_address_family_ipv4, p_self->u_addr_peer_port, p_self->addr_peer_ip),
			TNET_STUN_PKT_ATTR_ADD_CHANNEL_NUMBER(p_self->u_chan_num),
			TNET_STUN_PKT_ATTR_ADD_NULL());
		if (ret) {
			goto bail;
		}
		
	}
	else {
		if ((ret = tnet_stun_utils_transac_id_rand(&p_self->p_pkt_chanbind->transac_id))) {
			goto bail;
		}
	}
	
	if ((ret = _tnet_turn_session_send_pkt(p_self, p_self->p_pkt_chanbind))) {
		goto bail;
	}
	if (p_self->u_req_transport == IPPROTO_UDP) {
		p_self->timer.rtt.chanbind.u_timeout = kStunUdpRetransmitTimoutMinInMs;
		TNET_TURN_SESSION_TIMER_SCHEDULE_MILLIS(p_self, p_self->timer.rtt.chanbind.id, p_self->timer.rtt.chanbind.u_timeout);
	}

bail:
	tsk_safeobj_unlock(p_self);
	return ret;
}

int tnet_turn_session_send_data(tnet_turn_session_t* p_self, const void* pc_data_ptr, uint16_t u_data_size)
{
	int ret = 0;

	if (!p_self || !pc_data_ptr || !u_data_size) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(p_self);

	if (!p_self->b_started) {
		TSK_DEBUG_ERROR("TURN session not started yet");
		ret = -3;
		goto bail;
	}
	if (!p_self->b_alloc_ok) {
		TSK_DEBUG_ERROR("No active TURN allocation yet");
		ret = -5;
		goto bail;
	}
	if (!p_self->b_peer_perm_ok) {
		TSK_DEBUG_ERROR("No active TURN permission for the remote peer");
		ret = -4;
		goto bail;
	}

	/*** ChannelData ***/
	if ((p_self->b_chanbind_ok)) {
		ret = _tnet_turn_session_send_chandata(p_self, pc_data_ptr, u_data_size);
		goto bail;
	}

	/*** Send indication ***/
	if (!p_self->p_pkt_sendind) {
		if ((ret = tnet_stun_pkt_create_empty(tnet_stun_pkt_type_send_indication, &p_self->p_pkt_sendind))) {
			TSK_DEBUG_ERROR("Failed to create TURN SendIndication request");
			goto bail;
		}
		// add authinfo
		tnet_stun_pkt_auth_copy(p_self->p_pkt_sendind, p_self->cred.p_usr_name, p_self->cred.p_pwd, p_self->p_pkt_alloc);
		// add attributes
		ret = tnet_stun_pkt_attrs_add(p_self->p_pkt_sendind,
			TNET_STUN_PKT_ATTR_ADD_XOR_PEER_ADDRESS(p_self->b_peer_ipv6 ? tnet_stun_address_family_ipv6 : tnet_stun_address_family_ipv4, p_self->u_addr_peer_port, p_self->addr_peer_ip),
			TNET_STUN_PKT_ATTR_ADD_DATA(pc_data_ptr, u_data_size),
			TNET_STUN_PKT_ATTR_ADD_NULL());
		if (ret) {
			goto bail;
		}
	}
	else {
		const tnet_stun_attr_vdata_t *pc_attr_data;
		if ((ret = tnet_stun_pkt_attr_find_first(p_self->p_pkt_sendind, tnet_stun_attr_type_data, (const tnet_stun_attr_t**)&pc_attr_data))) {
			goto bail;
		}
		if (!pc_attr_data) {
			ret = tnet_stun_pkt_attrs_add(p_self->p_pkt_sendind,
				TNET_STUN_PKT_ATTR_ADD_DATA(pc_data_ptr, u_data_size),
				TNET_STUN_PKT_ATTR_ADD_NULL());
			if (ret) {
				goto bail;
			}
		}
		else {
			if ((ret = tnet_stun_attr_vdata_update((tnet_stun_attr_vdata_t*)pc_attr_data, pc_data_ptr, u_data_size))) {
				goto bail;
			}
		}
		if ((ret = tnet_stun_utils_transac_id_rand(&p_self->p_pkt_sendind->transac_id))) {
			goto bail;
		}
	}
	if ((ret = _tnet_turn_session_send_pkt(p_self, p_self->p_pkt_sendind))) {
		goto bail;
	}
	
bail:
	tsk_safeobj_unlock(p_self);
	return ret;
}

int tnet_turn_session_stop(tnet_turn_session_t* p_self)
{
	int ret = 0;

	if (!p_self) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(p_self);

	if (p_self->timer.p_mgr) {
		ret = tsk_timer_manager_stop(p_self->timer.p_mgr);
	}

	// free transport to force next call to prepare() to create new one with new sockets
    if (p_self->p_transport) {
        tnet_transport_shutdown(p_self->p_transport);
        TSK_OBJECT_SAFE_FREE(p_self->p_transport);
    }

	p_self->b_prepared = tsk_false;
	p_self->b_started = tsk_false;

	tsk_safeobj_unlock(p_self);
	return ret;
}

static uint16_t _tnet_turn_session_get_unique_chan_num()
{
	// rfc5766 - The channel number is in the range 0x4000 through 0x7FFE (inclusive)
	static long __l_chan_num = 0;
	tsk_atomic_inc(&__l_chan_num);
	return (__l_chan_num % (0x7FFE - 0x4000)) + 0x4000;
}

static int _tnet_turn_session_send_chandata(tnet_turn_session_t* p_self, const void* pc_buff_ptr, tsk_size_t u_buff_size)
{
	int ret = 0;
	tsk_size_t PadSize, NeededSize;
	uint8_t* _p_buff_chandata_ptr;
    if (!p_self || !pc_buff_ptr || !u_buff_size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    // lock()
    tsk_safeobj_lock(p_self);

    if (!p_self->b_started) {
        TSK_DEBUG_ERROR("TURN session not started");
        ret = -2;
        goto bail;
    }
	if (!p_self->b_chanbind_ok) {
        TSK_DEBUG_ERROR("No active TURN data channel");
        ret = -3;
        goto bail;
    }

	// rfc5766 - 11.5.  Sending a ChannelData Message
	if (p_self->u_req_transport == IPPROTO_UDP) {
		// Over UDP, the padding is not required but MAY be included.
		PadSize = 0;
	}
	else {
		// Over TCP and TLS-over-TCP, the ChannelData message MUST be padded to
		// a multiple of four bytes in order to ensure the alignment of subsequent messages.
		PadSize = (u_buff_size & 0x03) ? (4 - (u_buff_size & 0x03)) : 0;
	}
	NeededSize = kStunChannelDataHdrSizeInOctets + u_buff_size + PadSize;

	if (p_self->u_buff_chandata_size < NeededSize) {
		if (!(p_self->p_buff_chandata_ptr = tsk_realloc(p_self->p_buff_chandata_ptr, NeededSize))) {
			p_self->u_buff_chandata_size = 0;
			ret = -4;
			goto bail;
		}
		p_self->u_buff_chandata_size = NeededSize;
	}
	_p_buff_chandata_ptr = (uint8_t*)p_self->p_buff_chandata_ptr;

	*((uint16_t*)&_p_buff_chandata_ptr[0]) = tnet_htons(p_self->u_chan_num); // Channel Number
	*((uint16_t*)&_p_buff_chandata_ptr[2]) = tnet_htons((uint16_t)u_buff_size); // Length
	memcpy(&_p_buff_chandata_ptr[kStunChannelDataHdrSizeInOctets], pc_buff_ptr, u_buff_size); // Application Data
	if (PadSize) {
		memset(&_p_buff_chandata_ptr[kStunChannelDataHdrSizeInOctets + u_buff_size], 0, PadSize); // Set padding bytes to zero (not required but ease debugging)
	}

	if ((ret = _tnet_turn_session_send_buff(p_self, p_self->p_buff_chandata_ptr, NeededSize))) {
		goto bail;
	}
	
bail:
    // unlock()
    tsk_safeobj_unlock(p_self);
    return ret;
}

static int _tnet_turn_session_send_refresh(tnet_turn_session_t* p_self)
{
	int ret = 0;

	if (!p_self) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(p_self);

	if (!p_self->b_started) {
		TSK_DEBUG_ERROR("TURN session not started yet");
		ret = -3;
		goto bail;
	}
	if (!p_self->b_alloc_ok) {
		TSK_DEBUG_ERROR("No active TURN allocation yet");
		ret = -4;
		goto bail;
	}

	// create RefreshIndication Request
	p_self->timer.rtt.refresh.id = TSK_INVALID_TIMER_ID;
	TSK_OBJECT_SAFE_FREE(p_self->p_pkt_refresh);
	if ((ret = tnet_stun_pkt_create_empty(tnet_stun_pkt_type_refresh_indication, &p_self->p_pkt_refresh))) {
		TSK_DEBUG_ERROR("Failed to create TURN RefreshIndication request");
		goto bail;
	}
	// add authentication info
	tnet_stun_pkt_auth_copy(p_self->p_pkt_refresh, p_self->cred.p_usr_name, p_self->cred.p_pwd, p_self->p_pkt_alloc);
	// add attributes
	ret = tnet_stun_pkt_attrs_add(p_self->p_pkt_refresh,
		TNET_STUN_PKT_ATTR_ADD_LIFETIME(p_self->u_lifetime_alloc_in_sec),
		TNET_STUN_PKT_ATTR_ADD_SOFTWARE_ZT(kStunSoftware),  // recommended for Alloc and Refresh
		TNET_STUN_PKT_ATTR_ADD_NULL());
	if (ret) {
		goto bail;
	}
	p_self->p_pkt_refresh->opt.dontfrag = 0;
	p_self->p_pkt_refresh->opt.fingerprint = 0;
	if ((ret = _tnet_turn_session_send_pkt(p_self, p_self->p_pkt_refresh))) {
		goto bail;
	}
	if (p_self->u_req_transport == IPPROTO_UDP) {
		p_self->timer.rtt.refresh.u_timeout = kStunUdpRetransmitTimoutMinInMs;
		TNET_TURN_SESSION_TIMER_SCHEDULE_MILLIS(p_self, p_self->timer.rtt.refresh.id, p_self->timer.rtt.refresh.u_timeout);
	}

bail:
	tsk_safeobj_unlock(p_self);
	return ret;
}

static int _tnet_turn_session_send_buff(tnet_turn_session_t* p_self, const void* pc_buff_ptr, tsk_size_t u_buff_size)
{
    int ret = 0;
	 tsk_size_t u_sent_bytes = 0;
    if (!p_self || !pc_buff_ptr || !u_buff_size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    // lock()
    tsk_safeobj_lock(p_self);

    if (!p_self->b_started) {
        TSK_DEBUG_ERROR("TURN session not started");
        ret = -2;
        goto bail;
    }

	if (p_self->u_req_transport == IPPROTO_UDP) {
		u_sent_bytes = tnet_sockfd_sendto(p_self->p_lcl_sock->fd, (const struct sockaddr *)&p_self->srv_addr, pc_buff_ptr, u_buff_size);
    }
    else {
		TSK_DEBUG_ERROR("Not implemented yet");
		ret = -3;
		goto bail;
    }
	if (u_sent_bytes != u_buff_size) {
        TSK_DEBUG_ERROR("Failed to send %u bytes. Only %u sent", u_buff_size, u_sent_bytes);
        ret = -2;
        goto bail;
    }

bail:
    // unlock()
    tsk_safeobj_unlock(p_self);
    return ret;
}

static int _tnet_turn_session_send_pkt(tnet_turn_session_t* p_self, const tnet_turn_pkt_t *pc_pkt)
{
	int ret;
	tsk_size_t u_min_size;
	if (!p_self || !pc_pkt) {
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(p_self);

	if ((ret = tnet_stun_pkt_get_size_in_octetunits_with_padding(pc_pkt, &u_min_size))) {
        goto bail;
    }
    u_min_size += kStunBuffMinPad;
    if (p_self->u_buff_send_size < u_min_size) {
        if (!(p_self->p_buff_send_ptr = tsk_realloc(p_self->p_buff_send_ptr, u_min_size))) {
            TSK_DEBUG_ERROR("Failed to allocate buffer with size = %u", u_min_size);
            ret = -3;
            p_self->u_buff_send_size = 0;
            goto bail;
        }
        p_self->u_buff_send_size = u_min_size;
    }

    if ((ret = tnet_stun_pkt_write_with_padding(pc_pkt, p_self->p_buff_send_ptr, p_self->u_buff_send_size, &u_min_size))) {
        goto bail;
    }
    if ((ret = _tnet_turn_session_send_buff(p_self, p_self->p_buff_send_ptr, u_min_size))) {
        goto bail;
    }

bail:
	tsk_safeobj_unlock(p_self);
	return ret;
}

// 420 = Unknown Attribute
int _tnet_turn_session_process_err420_pkt(tnet_turn_pkt_t *p_pkt_req, const tnet_turn_pkt_t *pc_pkt_resp420)
{
	const tnet_stun_attr_vdata_t* pc_attr;
	uint16_t u16;
	int ret;
	tsk_bool_t b_done = tsk_false;
	if (!p_pkt_req || !pc_pkt_resp420) {
		TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
	}
	if ((ret = tnet_stun_pkt_attr_find_first(pc_pkt_resp420, tnet_stun_attr_type_unknown_attrs, (const tnet_stun_attr_t**)&pc_attr))) {
        goto bail;
    }
    if (!pc_attr || !pc_attr->p_data_ptr || (pc_attr->u_data_size & 1)) {
        TSK_DEBUG_ERROR("Invalid NONCE in 401");
        ret = -3;
        goto bail;
    }
	for (u16 = 0; u16 < pc_attr->u_data_size; u16+=2) {
		switch (*((uint16_t*)&pc_attr->p_data_ptr[u16])) {
			case tnet_stun_attr_type_dont_fragment:
				{
					p_pkt_req->opt.dontfrag = 0;
					b_done = tsk_true;
					break;
				}
			case tnet_stun_attr_type_fingerprint:
				{
					p_pkt_req->opt.fingerprint = 0;
					b_done = tsk_true;
					break;
				}
		}
	}

	if (b_done) {
		// TRANSACTION-ID
		if ((ret = tnet_stun_utils_transac_id_rand(&p_pkt_req->transac_id))) {
			goto bail;
		}
	}

bail:
	return ret;
}

static int _tnet_turn_session_process_incoming_pkt(struct tnet_turn_session_s* p_self, const tnet_turn_pkt_t *pc_pkt)
{
	int ret = 0;
    if (!p_self || !pc_pkt) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    // lock()
    tsk_safeobj_lock(p_self);

	switch (pc_pkt->e_type) {
		case tnet_stun_pkt_type_allocate_success_response:
		case tnet_stun_pkt_type_allocate_error_response:
		case tnet_stun_pkt_type_createpermission_success_response:
		case tnet_stun_pkt_type_createpermission_error_response:
		case tnet_stun_pkt_type_channelbind_success_response:
		case tnet_stun_pkt_type_channelbind_error_response:
		case tnet_stun_pkt_type_refresh_success_response:
		case tnet_stun_pkt_type_refresh_error_response:
			{
				uint16_t u_code;
				tnet_turn_pkt_t *pc_pkt_req = tsk_null;
				
#define CANCEL_TIMER(which) \
	if (TSK_TIMER_ID_IS_VALID(p_self->timer.rtt.##which.id)) { \
		tsk_timer_manager_cancel(p_self->timer.p_mgr, p_self->timer.rtt.##which.id); \
		p_self->timer.rtt.##which.id = TSK_INVALID_TIMER_ID; \
	}

				// Find request
				if (p_self->p_pkt_alloc && tnet_stun_utils_transac_id_cmp(p_self->p_pkt_alloc->transac_id, pc_pkt->transac_id) == 0) {
					pc_pkt_req = p_self->p_pkt_alloc;
					CANCEL_TIMER(alloc);
					CANCEL_TIMER(refresh);
				}
				else if (p_self->p_pkt_createperm && tnet_stun_utils_transac_id_cmp(p_self->p_pkt_createperm->transac_id, pc_pkt->transac_id) == 0) {
					pc_pkt_req = p_self->p_pkt_createperm;
					CANCEL_TIMER(createperm);
				}
				else if (p_self->p_pkt_sendind && tnet_stun_utils_transac_id_cmp(p_self->p_pkt_sendind->transac_id, pc_pkt->transac_id) == 0) {
					pc_pkt_req = p_self->p_pkt_sendind;
				}
				else if (p_self->p_pkt_chanbind && tnet_stun_utils_transac_id_cmp(p_self->p_pkt_chanbind->transac_id, pc_pkt->transac_id) == 0) {
					pc_pkt_req = p_self->p_pkt_chanbind;
					CANCEL_TIMER(chanbind);
				}
				else if (p_self->p_pkt_refresh && tnet_stun_utils_transac_id_cmp(p_self->p_pkt_refresh->transac_id, pc_pkt->transac_id) == 0) {
					pc_pkt_req = p_self->p_pkt_refresh;
					CANCEL_TIMER(refresh);
				}
				
				if (!pc_pkt_req) {
					TSK_DEBUG_ERROR("No matching request[TID=%s]", pc_pkt->transac_id);
					ret = -3;
					goto bail;
				}
				
				/*** SUCCESS ***/
				if (TNET_STUN_PKT_RESP_IS_SUCCESS(pc_pkt)) {
					// --- ALLOC --- //
					if (pc_pkt_req == p_self->p_pkt_alloc) {
						const tnet_stun_attr_address_t *pc_attr_addr;
						tnet_ip_t ip_addr;
						const tnet_stun_attr_vdata_t *pc_attr_lifetime;
						// XOR-MAPPED-ADDRESS (optional)
						if ((ret = tnet_stun_pkt_attr_find_first(pc_pkt, tnet_stun_attr_type_xor_mapped_address, (const tnet_stun_attr_t**)&pc_attr_addr)) == 0 && pc_attr_addr) {
							if ((ret = tnet_stun_utils_inet_ntop((pc_attr_addr->e_family == tnet_stun_address_family_ipv6), &pc_attr_addr->address, &ip_addr)) == 0) {
								p_self->b_srflx_ipv6 = (pc_attr_addr->e_family == tnet_stun_address_family_ipv6);
								p_self->u_srflx_port = pc_attr_addr->u_port;
								tsk_strupdate(&p_self->p_srflx_ip, ip_addr);
							}
						}
						// LIFETIME (optional)
						if ((ret = tnet_stun_pkt_attr_find_first(pc_pkt, tnet_stun_attr_type_lifetime, (const tnet_stun_attr_t**)&pc_attr_lifetime)) == 0 && pc_attr_lifetime && pc_attr_lifetime->u_data_size == 4) {
							p_self->u_lifetime_alloc_in_sec = TSK_TO_UINT32(pc_attr_lifetime->p_data_ptr);
						}
						// XOR-RELAYED-ADDRESS (required)
						if ((ret = tnet_stun_pkt_attr_find_first(pc_pkt, tnet_stun_attr_type_xor_relayed_address, (const tnet_stun_attr_t**)&pc_attr_addr))) {
							_tnet_turn_session_raise_event_alloc_nok(p_self);
							goto bail;
						}
						if (!pc_attr_addr) {
							TSK_DEBUG_ERROR("XOR-RELAYED-ADDRESS missing in success response for Allocate");
							ret = -4;
							_tnet_turn_session_raise_event_alloc_nok(p_self);
							goto bail;
						}
						if ((ret = tnet_stun_utils_inet_ntop((pc_attr_addr->e_family == tnet_stun_address_family_ipv6), &pc_attr_addr->address, &ip_addr))) {
							_tnet_turn_session_raise_event_alloc_nok(p_self);
							goto bail;
						}

						// Uncomment to test [TURN Client] <-> [Turn Server] <-> [Any Client]
#if 0
						{
							struct sockaddr_storage to;
							tnet_sockaddr_init(ip_addr, pc_attr_addr->u_port, p_self->p_lcl_sock->type, &to);
							tnet_sockfd_sendto(p_self->p_lcl_sock->fd, (const struct sockaddr*)&to, "pinhole", 7); // open pinhole to allow incoming data
							tnet_sockfd_sendto(p_self->p_lcl_sock->fd, (const struct sockaddr*)&to, "pinhole", 7); // open pinhole to allow incoming data
						}
#endif
						// Schedule refresh
						TNET_TURN_SESSION_TIMER_SCHEDULE_SEC(p_self, p_self->timer.u_timer_id_refresh, p_self->u_lifetime_alloc_in_sec);

						p_self->b_alloc_ok = tsk_true;
						p_self->b_rel_ipv6 = (pc_attr_addr->e_family == tnet_stun_address_family_ipv6);
						p_self->u_rel_port = pc_attr_addr->u_port;
						tsk_strupdate(&p_self->p_rel_ip, ip_addr);

						_tnet_turn_session_raise_event_alloc_ok(p_self);
					}
					// --- CREATE-PERMISSION --- //
					else if (pc_pkt_req == p_self->p_pkt_createperm) {
						p_self->b_peer_perm_ok = tsk_true;
						TNET_TURN_SESSION_TIMER_SCHEDULE_SEC(p_self, p_self->timer.u_timer_id_createperm, p_self->u_lifetime_createperm_in_sec);
						_tnet_turn_session_raise_event_createperm_ok(p_self);
					}
					// --- CHANNEL-BIND --- //
					else if (pc_pkt_req == p_self->p_pkt_chanbind) {
						p_self->b_chanbind_ok = tsk_true;
						TNET_TURN_SESSION_TIMER_SCHEDULE_SEC(p_self, p_self->timer.u_timer_id_chanbind, p_self->u_lifetime_chanbind_in_sec);
						_tnet_turn_session_raise_event_chanbind_ok(p_self);
					}
					// --- REFRESH --- //
					else if (pc_pkt_req == p_self->p_pkt_refresh) {
						const tnet_stun_attr_vdata_t *pc_attr_lifetime;
						if ((ret = tnet_stun_pkt_attr_find_first(pc_pkt, tnet_stun_attr_type_lifetime, (const tnet_stun_attr_t**)&pc_attr_lifetime)) == 0 && pc_attr_lifetime && pc_attr_lifetime->u_data_size == 4) {
							p_self->u_lifetime_alloc_in_sec = TSK_TO_UINT32(pc_attr_lifetime->p_data_ptr);
						}
						TNET_TURN_SESSION_TIMER_SCHEDULE_SEC(p_self, p_self->timer.u_timer_id_refresh, p_self->u_lifetime_alloc_in_sec);
						_tnet_turn_session_raise_event_refresh_ok(p_self);
					}
				}
				/*** ERROR ***/
				else {
					if ((ret = tnet_stun_pkt_get_errorcode(pc_pkt, &u_code))) {
						goto bail;
					}
					if (u_code == kStunErrCodeUnauthorized || u_code == kStunErrCodeStaleNonce) {
						if (u_code == kStunErrCodeUnauthorized) {
							// Make sure this is not an authentication failure (#2 401)
							// Do not send another req to avoid endless messages
							if ((tnet_stun_pkt_attr_exists(pc_pkt_req, tnet_stun_attr_type_message_integrity))) { // already has a MESSAGE-INTEGRITY?
								TSK_DEBUG_ERROR("TURN authentication failed");
								if (pc_pkt_req == p_self->p_pkt_alloc) { 
									_tnet_turn_session_raise_event_alloc_nok(p_self);
								}
								else if (pc_pkt_req == p_self->p_pkt_chanbind) {
									_tnet_turn_session_raise_event_chanbind_nok(p_self);
								}
								else if (pc_pkt_req == p_self->p_pkt_createperm) {
									_tnet_turn_session_raise_event_createperm_nok(p_self);
								}
								else if (pc_pkt_req == p_self->p_pkt_refresh) {
									_tnet_turn_session_raise_event_refresh_nok(p_self);
								}
								// INDICATION messages cannot receive 401 responses
								goto bail;
							}
						}
						if ((ret = tnet_stun_pkt_auth_prepare_2(pc_pkt_req, p_self->cred.p_usr_name, p_self->cred.p_pwd, pc_pkt))) {
							goto bail;
						}
						if ((ret = _tnet_turn_session_send_pkt(p_self, pc_pkt_req))) {
							goto bail;
						}
					}
					else if (u_code == kStunErrCodeUnknownAttributes) {
						if((ret = _tnet_turn_session_process_err420_pkt(pc_pkt_req, pc_pkt))) {
							goto bail;
						}
						if ((ret = _tnet_turn_session_send_pkt(p_self, pc_pkt_req))) {
							goto bail;
						}
					}
				}
				
				break;
			}
		case tnet_stun_pkt_type_data_indication:
			{
				const tnet_stun_attr_vdata_t *pc_attr_data;
				// DATA (required)
				if ((ret = tnet_stun_pkt_attr_find_first(pc_pkt, tnet_stun_attr_type_data, (const tnet_stun_attr_t**)&pc_attr_data))) {
					goto bail;
				}
				if (!pc_attr_data) {
					TSK_DEBUG_ERROR("DATA missing Data Indicaion");
					ret = -4;
					goto bail;
				}
				_tnet_turn_session_raise_event_recv_data(p_self, pc_attr_data->p_data_ptr, pc_attr_data->u_data_size);
				break;
			}			
        default:
			{
				break;
			}
	}

bail:
    // unlock()
    tsk_safeobj_unlock(p_self);
    return ret;
}

static int _tnet_turn_session_transport_layer_dgram_cb(const tnet_transport_event_t* e)
{
    tnet_turn_session_t* p_ss = (tnet_turn_session_t*)e->callback_data;
	int ret = 0;
	tnet_turn_pkt_t* p_pkt = tsk_null;
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

	if (!TNET_STUN_PKT_IS_STUN2(((const uint8_t*)e->data), e->size)) {
		// ChannelData ?
		if (p_ss->b_chanbind_ok && TNET_STUN_PKT_IS_CHANNEL_DATA(((const uint8_t*)e->data), e->size)) {
			const uint8_t* _p_data = (const uint8_t*)e->data;
			// Channel Number in [0x4000 - 0x7FFF]
			//rfc5766 - 11.6.  Receiving a ChannelData Message
			// If  the message uses a value in the reserved range (0x8000 through 0xFFFF), then the message is silently discarded
			uint16_t u_chan_num = tnet_ntohs_2(&_p_data[0]);
			if (u_chan_num == p_ss->u_chan_num) {
				uint16_t u_len = tnet_ntohs_2(&_p_data[2]);
				if (u_len <= (e->size - 4)) {
					_tnet_turn_session_raise_event_recv_data(p_ss, &_p_data[4], u_len);
					return 0;
				}
			}
		}
		_tnet_turn_session_raise_event_recv_data(p_ss, e->data, e->size);
		return 0;
	}

	if ((ret = tnet_stun_pkt_read(e->data, e->size, &p_pkt))) {
		goto bail;
	}
	if (p_pkt && (ret = _tnet_turn_session_process_incoming_pkt(p_ss, p_pkt))) {
		goto bail;
	}

bail:
	TSK_OBJECT_SAFE_FREE(p_pkt);
    return ret;
}

static int _tnet_turn_session_transport_layer_stream_cb(const tnet_transport_event_t* e)
{
	tnet_turn_session_t* p_ss = (tnet_turn_session_t*)e->callback_data;
	TSK_DEBUG_ERROR("Not implemented yet");
	return -1;
}

static int _tnet_turn_session_timer_callback(const void* pc_arg, tsk_timer_id_t timer_id)
{
#define RETRANSMIT(which) \
	p_ss->timer.rtt.##which.u_timeout <<= 1; \
	if (p_ss->timer.rtt.##which.u_timeout <= kStunUdpRetransmitTimoutMaxInMs) { \
		if ((ret = _tnet_turn_session_send_pkt(p_ss, p_ss->p_pkt_##which))) { \
			_tnet_turn_session_raise_event_##which##_nok(p_ss); \
			goto bail; \
		} \
		p_ss->timer.rtt.##which.id = tsk_timer_manager_schedule(p_ss->timer.p_mgr, p_ss->timer.rtt.##which.u_timeout, _tnet_turn_session_timer_callback, p_ss); \
	} else { \
		_tnet_turn_session_raise_event_##which##_nok(p_ss); \
	}

    tnet_turn_session_t* p_ss = (tnet_turn_session_t*)pc_arg;
	int ret = 0;
    tsk_safeobj_lock(p_ss); // must
	if (p_ss->timer.rtt.alloc.id == timer_id) {
		RETRANSMIT(alloc);
    }
	else if (p_ss->timer.rtt.chanbind.id == timer_id) {
		RETRANSMIT(chanbind);
    }
	else if (p_ss->timer.rtt.createperm.id == timer_id) {
		RETRANSMIT(createperm);
    }
	else if (p_ss->timer.rtt.refresh.id == timer_id) {
		RETRANSMIT(refresh);
    }
	else if (p_ss->timer.u_timer_id_refresh == timer_id) {
		if ((ret = _tnet_turn_session_send_refresh(p_ss))) {
			goto bail;
		}
    }
	else if (p_ss->timer.u_timer_id_chanbind == timer_id) {
		if ((ret = tnet_turn_session_chanbind(p_ss))) {
			goto bail;
		}
    }
	else if (p_ss->timer.u_timer_id_createperm == timer_id) {
		if ((ret = tnet_turn_session_createpermission(p_ss, p_ss->p_addr_peer_ip, p_ss->u_addr_peer_port))) {
			goto bail;
		}
    }
bail:
    tsk_safeobj_unlock(p_ss);
    return ret;
}


static tsk_object_t* tnet_turn_session_ctor(tsk_object_t * self, va_list * app)
{
    tnet_turn_session_t *p_ss = (tnet_turn_session_t *)self;
    if (p_ss) {
		tsk_safeobj_init(p_ss);
    }
    return self;
}
static tsk_object_t* tnet_turn_session_dtor(tsk_object_t * self)
{
    tnet_turn_session_t *p_ss = (tnet_turn_session_t *)self;
    if (p_ss) {
		// stop()
		tnet_turn_session_stop(p_ss);
		// timer.free()
		TSK_OBJECT_SAFE_FREE(p_ss->timer.p_mgr);
		// cred.free()
		TSK_FREE(p_ss->cred.p_usr_name);
		TSK_FREE(p_ss->cred.p_pwd);
		// others.free()
		TSK_OBJECT_SAFE_FREE(p_ss->p_pkt_alloc);
		TSK_OBJECT_SAFE_FREE(p_ss->p_pkt_createperm);
		TSK_OBJECT_SAFE_FREE(p_ss->p_pkt_sendind);
		TSK_OBJECT_SAFE_FREE(p_ss->p_pkt_chanbind);
		TSK_OBJECT_SAFE_FREE(p_ss->p_pkt_refresh);
		TSK_OBJECT_SAFE_FREE(p_ss->p_lcl_sock);

		TSK_FREE(p_ss->p_addr_peer_ip);
		TSK_FREE(p_ss->p_buff_chandata_ptr);
		TSK_FREE(p_ss->p_buff_send_ptr);
		TSK_FREE(p_ss->p_rel_ip);
		TSK_FREE(p_ss->p_srflx_ip);

		if (p_ss->p_transport) {
			tnet_transport_shutdown(p_ss->p_transport);
			TSK_OBJECT_SAFE_FREE(p_ss->p_transport);
		}
		tsk_safeobj_deinit(p_ss);
		TSK_DEBUG_INFO("*** STUN Session destroyed ***");
    }
    return self;
}
static int tnet_turn_session_cmp(const tsk_object_t *_ss1, const tsk_object_t *_ss2)
{
    const tnet_turn_session_t *pc_ss1 = (const tnet_turn_session_t *)_ss1;
    const tnet_turn_session_t *pc_ss2 = (const tnet_turn_session_t *)_ss2;

    return (int)(pc_ss1 - pc_ss2);
}
static const tsk_object_def_t tnet_turn_session_def_s = {
    sizeof(tnet_turn_session_t),
    tnet_turn_session_ctor,
    tnet_turn_session_dtor,
    tnet_turn_session_cmp,
};
const tsk_object_def_t *tnet_turn_session_def_t = &tnet_turn_session_def_s;
