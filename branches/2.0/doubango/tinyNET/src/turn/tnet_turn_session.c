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

typedef tnet_stun_pkt_t tnet_turn_pkt_t;

typedef struct tnet_turn_session_s
{
	TSK_DECLARE_OBJECT;

	tsk_bool_t b_prepared;
	tsk_bool_t b_started;

	uint32_t u_timeout_alloc_in_sec;
	uint8_t u_req_transport;

	tnet_turn_pkt_t* p_pkt_alloc;
	tnet_turn_pkt_t* p_pkt_createperm;

	void* p_buff_send_ptr;
	tsk_size_t u_buff_send_size;

	char* p_rel_ip;
	uint16_t u_rel_port;
	tsk_bool_t b_rel_ipv6;

	struct {
		char* p_usr_name;
		char* p_pwd;
		char* p_realm;
	} cred;

	struct {
		tnet_turn_session_callback_f f_fun;
		struct tnet_turn_session_event_xs e;
	} cb;

	tnet_socket_t* p_lcl_sock;
	struct sockaddr_storage srv_addr;
	struct tnet_transport_s* p_transport;

	TSK_DECLARE_SAFEOBJ;
}
tnet_turn_session_t;

static int _tnet_turn_session_send_buff(tnet_turn_session_t* p_self, const void* pc_buff_ptr, tsk_size_t u_buff_size);
static int _tnet_turn_session_send_pkt(tnet_turn_session_t* p_self, const tnet_turn_pkt_t *pc_pkt);
static int _tnet_turn_session_process_incoming_pkt(struct tnet_turn_session_s* p_self, const tnet_turn_pkt_t *pc_pkt);
static int _tnet_turn_session_process_err420_pkt(tnet_turn_pkt_t *p_pkt_req, const tnet_turn_pkt_t *pc_pkt_resp420);
static int _tnet_turn_session_transport_layer_dgram_cb(const tnet_transport_event_t* e);
static int _tnet_turn_session_transport_layer_stream_cb(const tnet_transport_event_t* e);

#define _tnet_turn_session_raise_event0(_p_self, _e_type) \
	if (_p_self->cb.f_fun) { \
		_p_self->cb.e.e_type = _e_type; \
		_p_self->cb.f_fun(&p_self->cb.e); \
	}
#define _tnet_turn_session_raise_event_alloc_ok(p_self) _tnet_turn_session_raise_event0((p_self), tnet_turn_session_event_type_alloc_ok)
#define _tnet_turn_session_raise_event_alloc_nok(p_self) _tnet_turn_session_raise_event0((p_self), tnet_turn_session_event_type_alloc_nok)
#define _tnet_turn_session_raise_event_perm_ok(p_self) _tnet_turn_session_raise_event0((p_self), tnet_turn_session_event_type_perm_ok)
#define _tnet_turn_session_raise_event_perm_nok(p_self) _tnet_turn_session_raise_event0((p_self), tnet_turn_session_event_type_perm_nok)

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
	TSK_OBJECT_SAFE_FREE(p_self->p_pkt_alloc);
	if ((ret = tnet_stun_pkt_create_empty(tnet_stun_pkt_type_allocate_request, &p_self->p_pkt_alloc))) {
		TSK_DEBUG_ERROR("Failed to create TURN Allocate request");
		goto bail;
	}
	// add attributes
	ret = tnet_stun_pkt_attrs_add(p_self->p_pkt_alloc,
		TNET_STUN_PKT_ATTR_ADD_LIFETIME(p_self->u_timeout_alloc_in_sec),
		TNET_STUN_PKT_ATTR_ADD_REQUESTED_TRANSPORT(p_self->u_req_transport),
		TNET_STUN_PKT_ATTR_ADD_SOFTWARE_ZT(kStunSoftware),
		TNET_STUN_PKT_ATTR_ADD_NULL());
	if (ret) {
		goto bail;
	}

	// FIXME
	_tnet_turn_session_send_pkt(p_self, p_self->p_pkt_alloc);

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

	tsk_strupdate(pp_ip, p_self->p_rel_ip);
	*pu_port = p_self->u_rel_port;
	*pb_ipv6 = p_self->b_rel_ipv6;

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

	b_ipv6 = TNET_SOCKET_TYPE_IS_IPV6(p_self->p_lcl_sock->type);
	if ((ret = tnet_stun_utils_inet_pton(b_ipv6, pc_peer_addr, &peer_addr))) {
		TSK_DEBUG_ERROR("inet_pton(%s,IPv6=%d) failed", pc_peer_addr, b_ipv6);
		goto bail;
	}

	// create CreatePermission Request
	TSK_OBJECT_SAFE_FREE(p_self->p_pkt_createperm);
	if ((ret = tnet_stun_pkt_create_empty(tnet_stun_pkt_type_createpermission_request, &p_self->p_pkt_createperm))) {
		TSK_DEBUG_ERROR("Failed to create TURN CreatePermission request");
		goto bail;
	}
	// add attributes
	ret = tnet_stun_pkt_attrs_add(p_self->p_pkt_createperm,
		TNET_STUN_PKT_ATTR_ADD_XOR_PEER_ADDRESS(b_ipv6 ? tnet_stun_address_family_ipv6 : tnet_stun_address_family_ipv4, u_peer_port, peer_addr),
		TNET_STUN_PKT_ATTR_ADD_NULL());
	if (ret) {
		goto bail;
	}

	// FIXME
	_tnet_turn_session_send_pkt(p_self, p_self->p_pkt_createperm);

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

	// free transport to force next call to prepare() to create new one with new sockets
    if (p_self->p_transport) {
        tnet_transport_shutdown(p_self->p_transport);
        TSK_OBJECT_SAFE_FREE(p_self->p_transport);
    }

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
#if 0
    if (!p_self->b_started) {
        TSK_DEBUG_ERROR("TURN session not started");
        ret = -2;
        goto bail;
    }
#endif

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
			{
				uint16_t u_code;
				tnet_turn_pkt_t *pc_pkt_req = tsk_null;
				
				// Find request
				if (tnet_stun_utils_transac_id_cmp(p_self->p_pkt_alloc->transac_id, pc_pkt->transac_id) == 0) {
					pc_pkt_req = p_self->p_pkt_alloc;
				}
				else if (tnet_stun_utils_transac_id_cmp(p_self->p_pkt_createperm->transac_id, pc_pkt->transac_id) == 0) {
					pc_pkt_req = p_self->p_pkt_createperm;
				}
				
				if (!pc_pkt_req) {
					TSK_DEBUG_ERROR("No matching request[TID=%s]", pc_pkt->transac_id);
					ret = -3;
					goto bail;
				}
				
				/*** SUCCESS ***/
				if (TNET_STUN_PKT_RESP_IS_SUCCESS(pc_pkt)) {
					const tnet_stun_attr_address_t *pc_attr_rel;
					tnet_ip_t ip_rel;
					// --- ALLOC --- //
					if (pc_pkt_req == p_self->p_pkt_alloc) {
						if ((ret = tnet_stun_pkt_attr_find_first(pc_pkt, tnet_stun_attr_type_xor_relayed_address, (const tnet_stun_attr_t**)&pc_attr_rel))) {
							_tnet_turn_session_raise_event_alloc_nok(p_self);
							goto bail;
						}
						if (!pc_attr_rel) {
							TSK_DEBUG_ERROR("XOR-RELAYED-ADDRESS missing in success response for Allocate");
							ret = -4;
							_tnet_turn_session_raise_event_alloc_nok(p_self);
							goto bail;
						}
						if ((ret = tnet_stun_utils_inet_ntop((pc_attr_rel->e_family == tnet_stun_address_family_ipv6), &pc_attr_rel->address, &ip_rel))) {
							_tnet_turn_session_raise_event_alloc_nok(p_self);
							goto bail;
						}
						p_self->b_rel_ipv6 = (pc_attr_rel->e_family == tnet_stun_address_family_ipv6);
						p_self->u_rel_port = pc_attr_rel->u_port;
						tsk_strupdate(&p_self->p_rel_ip, ip_rel);
						_tnet_turn_session_raise_event_alloc_ok(p_self);
					}
					// --- CREATE-PERMISSION --- //
					else if (pc_pkt_req == p_self->p_pkt_createperm) {
					}
				}
				/*** ERROR ***/
				else {
					if ((ret = tnet_stun_pkt_get_errorcode(pc_pkt, &u_code))) {
						goto bail;
					}
					if (u_code == kStunErrCodeUnauthorized || u_code == kStunErrCodeStaleNonce) {
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
        default:
			break;
	}

bail:
    // unlock()
    tsk_safeobj_unlock(p_self);
    return ret;
}

static int _tnet_turn_session_transport_layer_dgram_cb(const tnet_transport_event_t* e)
{
    tnet_turn_session_t* p_ss = (tnet_turn_session_t*)e->callback_data;
	int ret;
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
		// FIXME: callback (incoming_data)
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

static tsk_object_t* tnet_turn_session_ctor(tsk_object_t * self, va_list * app)
{
    tnet_turn_session_t *p_ss = (tnet_turn_session_t *)self;
    if (p_ss) {
		p_ss->u_timeout_alloc_in_sec = kTurnAllocationTimeOutInSec;
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
		// cred.free()
		TSK_FREE(p_ss->cred.p_usr_name);
		TSK_FREE(p_ss->cred.p_pwd);
		TSK_FREE(p_ss->cred.p_realm);
		// others.free()
		TSK_OBJECT_SAFE_FREE(p_ss->p_pkt_alloc);
		TSK_OBJECT_SAFE_FREE(p_ss->p_pkt_createperm);
		TSK_OBJECT_SAFE_FREE(p_ss->p_lcl_sock);

		TSK_FREE(p_ss->p_buff_send_ptr);
		TSK_FREE(p_ss->p_rel_ip);

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
