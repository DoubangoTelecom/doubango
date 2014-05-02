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

typedef tnet_stun_pkt_t tnet_turn_pkt_t;

typedef struct tnet_turn_session_s
{
	TSK_DECLARE_OBJECT;

	tsk_bool_t b_prepared;
	tsk_bool_t b_started;

	uint32_t u_timeout_alloc_in_sec;
	uint8_t u_req_transport;

	tnet_turn_pkt_t* p_pkt_alloc;

	void* p_buff_send_ptr;
	tsk_size_t u_buff_send_size;

	struct {
		char* p_usr_name;
		char* p_pwd;
		char* p_realm;
	} cred;

	tnet_socket_t* p_lcl_sock;
	struct sockaddr_storage srv_addr;

	TSK_DECLARE_SAFEOBJ;
}
tnet_turn_session_t;

static int _tnet_turn_session_send_buff(tnet_turn_session_t* p_self, const void* pc_buff_ptr, tsk_size_t u_buff_size);
static int _tnet_turn_session_send_pkt(tnet_turn_session_t* p_self, const tnet_turn_pkt_t *pc_pkt);

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
	TSK_OBJECT_SAFE_FREE(p_self->p_pkt_alloc);

	// create Allocate Request
	if ((ret = tnet_stun_pkt_create_empty(tnet_stun_pkt_type_allocate_request, &p_self->p_pkt_alloc))) {
		TSK_DEBUG_ERROR("Failed to create TURN Allocate request");
		goto bail;
	}
	// add attributes
	ret = tnet_stun_pkt_attrs_add(p_self->p_pkt_alloc,
		TNET_STUN_PKT_ATTR_ADD_LIFETIME(p_self->u_timeout_alloc_in_sec),
		TNET_STUN_PKT_ATTR_ADD_REQUESTED_TRANSPORT(p_self->u_req_transport),
#if 0 // "numb.viagenie.ca" -> 420 Unknown attribute
		TNET_STUN_PKT_ATTR_ADD_DONT_FRAGMENT(),
#endif
		TNET_STUN_PKT_ATTR_ADD_SOFTWARE_ZT(kStunSoftware),
		TNET_STUN_PKT_ATTR_ADD_NULL());
	if (ret) {
		goto bail;
	}

	// FIXME
	_tnet_turn_session_send_pkt(p_self, p_self->p_pkt_alloc);

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

	p_self->b_started = tsk_true;
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
        TSK_DEBUG_INFO("*** STUN Session destroyed ***");
		// cred
		TSK_FREE(p_ss->cred.p_usr_name);
		TSK_FREE(p_ss->cred.p_pwd);
		TSK_FREE(p_ss->cred.p_realm);
		// others
		TSK_OBJECT_SAFE_FREE(p_ss->p_pkt_alloc);
		TSK_OBJECT_SAFE_FREE(p_ss->p_lcl_sock);

		TSK_FREE(p_ss->p_buff_send_ptr);

		tsk_safeobj_deinit(p_ss);
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
