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
#include "tnet_proxydetect.h"

#include "tsk_string.h"
#include "tsk_timer.h"
#include "tsk_time.h"
#include "tsk_safeobj.h"
#include "tsk_debug.h"

#define kTurnTransportFriendlyName		"TURN transport"
#define kTurnTransportConnectTimeout	1500 // 1.5sec to wait until socket get connected
#define kTurnStreamChunckMaxSize		0xFFFF // max size of a chunck to form a valid STUN message. Used as a guard.
#define kTurnStreamOutMaxSize           0xFFFF // max pending data size

#define TNET_TURN_SESSION_TIMOUT_GET_BEST_SEC(u_timeout_in_sec) ((u_timeout_in_sec)*950) // add small delay for code execution
#define TNET_TURN_SESSION_TIMOUT_GET_BEST_MILLIS(u_timeout_in_millis) (((u_timeout_in_millis)*950)/1000) // add small delay for code execution

#define TNET_TURN_SESSION_TIMER_SCHEDULE_SEC(_p_self, u_id, u_timeout_in_sec) \
	(u_id) = tsk_timer_manager_schedule((_p_self)->timer.p_mgr, TNET_TURN_SESSION_TIMOUT_GET_BEST_SEC((u_timeout_in_sec)), _tnet_turn_session_timer_callback, (_p_self))
#define TNET_TURN_SESSION_TIMER_SCHEDULE_MILLIS(_p_self, u_id, u_timeout_in_millis) \
	(u_id) = tsk_timer_manager_schedule((_p_self)->timer.p_mgr, TNET_TURN_SESSION_TIMOUT_GET_BEST_MILLIS((u_timeout_in_millis)), _tnet_turn_session_timer_callback, (_p_self))

typedef tnet_stun_pkt_t tnet_turn_pkt_t;

typedef struct tnet_turn_peer_s {
    TSK_DECLARE_OBJECT;

    tnet_turn_peer_id_t id;

    uint16_t u_chan_num;
    uint32_t u_conn_id; // rfc6062 - 6.2.1.  CONNECTION-ID, For streams only
    tnet_fd_t conn_fd; // Connected FD: used for Streams only

    tnet_stun_addr_t addr_ip;
    char* p_addr_ip;
    uint16_t u_addr_port;
    tsk_bool_t b_ipv6;
    tsk_bool_t b_stream_connected;
    tsk_buffer_t *p_stream_buff_in;
    tsk_buffer_t *p_stream_buff_out;

    tnet_stun_state_t e_createperm_state;
    tnet_stun_state_t e_chanbind_state;
    tnet_stun_state_t e_connect_state;
    tnet_stun_state_t e_connbind_state;

    tnet_turn_pkt_t* p_pkt_createperm;
    tnet_stun_pkt_t* p_pkt_sendind;
    tnet_stun_pkt_t* p_pkt_chanbind;
    tnet_stun_pkt_t* p_pkt_connect; // For streams only
    tnet_stun_pkt_t* p_pkt_connbind; // For streams only

    struct {
        struct {
            struct {
                tsk_timer_id_t id;
                uint64_t u_timeout;
            } createperm;
            struct {
                tsk_timer_id_t id;
                uint64_t u_timeout;
            } chanbind;
        } fresh; // schedule refresh
        struct {
            struct {
                tsk_timer_id_t id;
                uint64_t u_timeout;
            } createperm;
            struct {
                tsk_timer_id_t id;
                uint64_t u_timeout;
            } chanbind;
        } rtt; // retransmit (UDP only, to deal with pkt loss)
    } timer;
}
tnet_turn_peer_t;
typedef tsk_list_t tnet_turn_peers_L_t;

typedef struct tnet_turn_session_s {
    TSK_DECLARE_OBJECT;

    tsk_bool_t b_prepared;
    tsk_bool_t b_started;
    tsk_bool_t b_stopping;
    enum tnet_stun_state_e e_alloc_state;
    enum tnet_stun_state_e e_refresh_state;

    enum tnet_turn_transport_e e_req_transport;

    uint32_t u_lifetime_alloc_in_sec;

    tnet_turn_pkt_t* p_pkt_alloc;
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

    struct {
        tsk_bool_t auto_detect;
        struct tnet_proxyinfo_s* info;
    }
    proxy;

    struct {
        char* path_priv;
        char* path_pub;
        char* path_ca;
        tsk_bool_t verify;
    } ssl;

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
        struct {
            struct {
                tsk_timer_id_t id;
                uint64_t u_timeout;
            } alloc;
            struct {
                tsk_timer_id_t id;
                uint64_t u_timeout;
            } refresh;
        } rtt; // retransmit (UDP only)
    } timer;

    tnet_socket_t* p_lcl_sock;
    char* p_srv_host;
    uint16_t u_srv_port;
    tsk_bool_t b_stream_connected;
    tsk_buffer_t* p_stream_buff_out; // data pending until the socket is connected
    tsk_bool_t b_stream_error; // Unrecoverable error occured
    tsk_buffer_t *p_stream_buff_in;
    struct sockaddr_storage srv_addr;
    struct tnet_transport_s* p_transport;

    tnet_turn_peers_L_t* p_list_peers;

    TSK_DECLARE_SAFEOBJ;
}
tnet_turn_session_t;

static uint16_t _tnet_turn_session_get_unique_chan_num();
static int _tnet_turn_session_send_chandata(tnet_turn_session_t* p_self, const tnet_turn_peer_t* pc_peer, const void* pc_buff_ptr, tsk_size_t u_buff_size);
static int _tnet_turn_session_send_stream_raw(tnet_turn_session_t* p_self, tnet_turn_peer_t* pc_peer, const void* pc_buff_ptr, tsk_size_t u_buff_size);
static int _tnet_turn_session_send_refresh(tnet_turn_session_t* p_self);
static int _tnet_turn_session_send_permission(struct tnet_turn_session_s* p_self, tnet_turn_peer_t *p_peer);
static int _tnet_turn_session_send_connbind(struct tnet_turn_session_s* p_self, tnet_turn_peer_t *p_peer);
static int _tnet_turn_session_send_buff_0(tnet_turn_session_t* p_self, const tnet_turn_peer_t* pc_peer, const void* pc_buff_ptr, tsk_size_t u_buff_size);
static int _tnet_turn_session_send_buff(tnet_turn_session_t* p_self, const void* pc_buff_ptr, tsk_size_t u_buff_size);
static int _tnet_turn_session_send_pkt_0(tnet_turn_session_t* p_self, const tnet_turn_peer_t* pc_peer, const tnet_turn_pkt_t *pc_pkt);
static int _tnet_turn_session_send_pkt(tnet_turn_session_t* p_self, const tnet_turn_pkt_t *pc_pkt);
static int _tnet_turn_session_process_incoming_pkt(struct tnet_turn_session_s* p_self, const tnet_turn_pkt_t *pc_pkt, tsk_bool_t *pb_processed);
static int _tnet_turn_session_process_success_connect_pkt(struct tnet_turn_session_s* p_self, tnet_turn_peer_t* pc_peer, const tnet_turn_pkt_t *pc_pkt);
static int _tnet_turn_session_process_err420_pkt(tnet_turn_pkt_t *p_pkt_req, const tnet_turn_pkt_t *pc_pkt_resp420);
static int _tnet_turn_session_transport_layer_dgram_cb(const tnet_transport_event_t* e);
static int _tnet_turn_session_transport_layer_stream_cb(const tnet_transport_event_t* e);
static int _tnet_turn_session_transport_layer_process_cb(const tnet_transport_event_t* e);
static int _tnet_turn_session_timer_callback(const void* pc_arg, tsk_timer_id_t timer_id);

static int _tnet_turn_peer_create(const char* pc_peer_ip, uint16_t u_peer_port, tsk_bool_t b_ipv6, struct tnet_turn_peer_s **pp_peer);
static int _tnet_turn_peer_find_by_xpeer(const tnet_turn_peers_L_t* pc_peers, const tnet_stun_attr_address_t* pc_xpeer, const tnet_turn_peer_t **ppc_peer);

/*** PREDICATES ***/
static int __pred_find_peer_by_id(const tsk_list_item_t *item, const void *id)
{
    if (item && item->data) {
        return (int)(((const struct tnet_turn_peer_s *)item->data)->id - *((const tnet_turn_peer_id_t*)id));
    }
    return -1;
}
static int __pred_find_peer_by_fd(const tsk_list_item_t *item, const void *fd)
{
    if (item && item->data) {
        return (int)(((const struct tnet_turn_peer_s *)item->data)->conn_fd - *((const tnet_fd_t*)fd));
    }
    return -1;
}
static int __pred_find_peer_by_channum(const tsk_list_item_t *item, const void *pu_chan_num)
{
    if (item && item->data) {
        return (int)(((const struct tnet_turn_peer_s *)item->data)->u_chan_num - *((const uint16_t*)pu_chan_num));
    }
    return -1;
}
static int __pred_find_peer_by_timer_rtt_createperm(const tsk_list_item_t *item, const void *id)
{
    if (item && item->data) {
        return (int)(((const struct tnet_turn_peer_s *)item->data)->timer.rtt.createperm.id - *((const tsk_timer_id_t*)id));
    }
    return -1;
}
static int __pred_find_peer_by_timer_fresh_createperm(const tsk_list_item_t *item, const void *id)
{
    if (item && item->data) {
        return (int)(((const struct tnet_turn_peer_s *)item->data)->timer.fresh.createperm.id - *((const tsk_timer_id_t*)id));
    }
    return -1;
}
static int __pred_find_peer_by_transacid_createperm(const tsk_list_item_t *item, const void *pc_transacid)
{
    if (item && item->data) {
        return ((const struct tnet_turn_peer_s *)item->data)->p_pkt_createperm
               ? tnet_stun_utils_transac_id_cmp(((const struct tnet_turn_peer_s *)item->data)->p_pkt_createperm->transac_id, *((const tnet_stun_transac_id_t*)pc_transacid))
               : +1;
    }
    return -1;
}
static int __pred_find_peer_by_timer_rtt_chanbind(const tsk_list_item_t *item, const void *id)
{
    if (item && item->data) {
        return (int)(((const struct tnet_turn_peer_s *)item->data)->timer.rtt.chanbind.id - *((const tsk_timer_id_t*)id));
    }
    return -1;
}
static int __pred_find_peer_by_timer_fresh_chanbind(const tsk_list_item_t *item, const void *id)
{
    if (item && item->data) {
        return (int)(((const struct tnet_turn_peer_s *)item->data)->timer.fresh.chanbind.id - *((const tsk_timer_id_t*)id));
    }
    return -1;
}
static int __pred_find_peer_by_transacid_chanbind(const tsk_list_item_t *item, const void *pc_transacid)
{
    if (item && item->data) {
        return ((const struct tnet_turn_peer_s *)item->data)->p_pkt_chanbind
               ? tnet_stun_utils_transac_id_cmp(((const struct tnet_turn_peer_s *)item->data)->p_pkt_chanbind->transac_id, *((const tnet_stun_transac_id_t*)pc_transacid))
               : +1;
    }
    return -1;
}
static int __pred_find_peer_by_transacid_connect(const tsk_list_item_t *item, const void *pc_transacid)
{
    if (item && item->data) {
        return ((const struct tnet_turn_peer_s *)item->data)->p_pkt_connect
               ? tnet_stun_utils_transac_id_cmp(((const struct tnet_turn_peer_s *)item->data)->p_pkt_connect->transac_id, *((const tnet_stun_transac_id_t*)pc_transacid))
               : +1;
    }
    return -1;
}
static int __pred_find_peer_by_transacid_connectionbind(const tsk_list_item_t *item, const void *pc_transacid)
{
    if (item && item->data) {
        return ((const struct tnet_turn_peer_s *)item->data)->p_pkt_connbind
               ? tnet_stun_utils_transac_id_cmp(((const struct tnet_turn_peer_s *)item->data)->p_pkt_connbind->transac_id, *((const tnet_stun_transac_id_t*)pc_transacid))
               : +1;
    }
    return -1;
}
static int __pred_find_peer_by_transacid_sendind(const tsk_list_item_t *item, const void *pc_transacid)
{
    if (item && item->data) {
        return ((const struct tnet_turn_peer_s *)item->data)->p_pkt_sendind
               ? tnet_stun_utils_transac_id_cmp(((const struct tnet_turn_peer_s *)item->data)->p_pkt_sendind->transac_id, *((const tnet_stun_transac_id_t*)pc_transacid))
               : +1;
    }
    return -1;
}

#define _tnet_turn_session_raise_event0(_p_self, _e_type, _u_peer_id) \
	if ((_p_self)->cb.f_fun) { \
		(_p_self)->cb.e.e_type = _e_type; \
		(_p_self)->cb.e.u_peer_id = _u_peer_id; \
		(_p_self)->cb.f_fun(&(_p_self)->cb.e); \
	}
#define _tnet_turn_session_raise_event1(_p_self, _e_type, _u_peer_id, _pc_data_ptr, _u_data_size) \
	if ((_p_self)->cb.f_fun) { \
		(_p_self)->cb.e.e_type = _e_type; \
		(_p_self)->cb.e.u_peer_id = _u_peer_id; \
		(_p_self)->cb.e.data.pc_data_ptr = _pc_data_ptr; \
		(_p_self)->cb.e.data.u_data_size = _u_data_size; \
		(_p_self)->cb.f_fun(&(_p_self)->cb.e); \
	}
#define _tnet_turn_session_raise_event_alloc_ok(p_self) _tnet_turn_session_raise_event0((p_self), tnet_turn_session_event_type_alloc_ok, kTurnPeerIdInvalid)
#define _tnet_turn_session_raise_event_alloc_nok(p_self) _tnet_turn_session_raise_event0((p_self), tnet_turn_session_event_type_alloc_nok, kTurnPeerIdInvalid)
#define _tnet_turn_session_raise_event_refresh_ok(p_self) _tnet_turn_session_raise_event0((p_self), tnet_turn_session_event_type_refresh_ok, kTurnPeerIdInvalid)
#define _tnet_turn_session_raise_event_refresh_nok(p_self) _tnet_turn_session_raise_event0((p_self), tnet_turn_session_event_type_refresh_nok, kTurnPeerIdInvalid)
#define _tnet_turn_session_raise_event_createperm_ok(p_self, u_peer_id) _tnet_turn_session_raise_event0((p_self), tnet_turn_session_event_type_perm_ok, (u_peer_id))
#define _tnet_turn_session_raise_event_createperm_nok(p_self, u_peer_id) _tnet_turn_session_raise_event0((p_self), tnet_turn_session_event_type_perm_nok, (u_peer_id))
#define _tnet_turn_session_raise_event_chanbind_ok(p_self, u_peer_id) _tnet_turn_session_raise_event0((p_self), tnet_turn_session_event_type_chanbind_ok, (u_peer_id))
#define _tnet_turn_session_raise_event_chanbind_nok(p_self, u_peer_id) _tnet_turn_session_raise_event0((p_self), tnet_turn_session_event_type_chanbind_nok, (u_peer_id))
#define _tnet_turn_session_raise_event_connect_ok(p_self, u_peer_id) _tnet_turn_session_raise_event0((p_self), tnet_turn_session_event_type_connect_ok, (u_peer_id))
#define _tnet_turn_session_raise_event_connect_nok(p_self, u_peer_id) _tnet_turn_session_raise_event0((p_self), tnet_turn_session_event_type_connect_nok, (u_peer_id))
#define _tnet_turn_session_raise_event_connectionbind_ok(p_self, u_peer_id) _tnet_turn_session_raise_event0((p_self), tnet_turn_session_event_type_connectionbind_ok, (u_peer_id))
#define _tnet_turn_session_raise_event_connectionbind_nok(p_self, u_peer_id) _tnet_turn_session_raise_event0((p_self), tnet_turn_session_event_type_connectionbind_nok, (u_peer_id))
#define _tnet_turn_session_raise_event_recv_data(p_self, u_peer_id, pc_data_ptr, u_data_size) _tnet_turn_session_raise_event1((p_self), tnet_turn_session_event_type_recv_data, (u_peer_id), (pc_data_ptr), (u_data_size))


int tnet_turn_session_create(struct tnet_socket_s* p_lcl_sock, enum tnet_turn_transport_e e_req_transport, const char* pc_srv_host, uint16_t u_srv_port, struct tnet_turn_session_s** pp_self)
{
    int ret;
    extern const tsk_object_def_t *tnet_turn_session_def_t;
    tnet_turn_session_t* p_self;
    if (!p_lcl_sock || !TNET_SOCKET_IS_VALID(p_lcl_sock) || !pc_srv_host || !u_srv_port || !pp_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!(p_self = tsk_object_new(tnet_turn_session_def_t))) {
        TSK_DEBUG_ERROR("Failed to create 'tnet_turn_session_def_t' object");
        return -2;
    }
    if (!(p_self->p_list_peers = tsk_list_create())) {
        TSK_DEBUG_ERROR("Failed to create list");
        ret = -3;
        goto bail;
    }
    if (TNET_SOCKET_TYPE_IS_STREAM(p_lcl_sock->type) && !(p_self->p_stream_buff_in = tsk_buffer_create_null())) {
        TSK_DEBUG_ERROR("Failed to stream buffer");
        ret = -4;
        goto bail;
    }
    if ((ret = tnet_sockaddr_init(pc_srv_host, u_srv_port, p_lcl_sock->type, &p_self->srv_addr))) {
        TSK_DEBUG_ERROR("Invalid TURN SRV address [%s:%u]", pc_srv_host, u_srv_port);
        goto bail;
    }
    tsk_strupdate(&p_self->p_srv_host, pc_srv_host);
    p_self->u_srv_port = u_srv_port;
    p_self->p_lcl_sock = tsk_object_ref(p_lcl_sock);
    p_self->e_req_transport = e_req_transport;
    p_self->cb.e.pc_session = p_self;
    *pp_self = p_self;
bail:
    if (ret) {
        TSK_OBJECT_SAFE_FREE(p_self);
    }
    return ret;
}

int tnet_turn_session_create_2(const char* pc_lcl_ip, uint16_t u_lcl_port, enum tnet_socket_type_e e_lcl_type, enum tnet_turn_transport_e e_req_transport, const char* pc_srv_host, uint16_t u_srv_port, struct tnet_turn_session_s** pp_self)
{
    tnet_socket_t* p_lcl_sock;
    int ret;
    if (!pc_srv_host || !u_srv_port || !pp_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if(!(p_lcl_sock = tnet_socket_create(pc_lcl_ip, u_lcl_port, e_lcl_type))) {
        TSK_DEBUG_ERROR("Failed to create socket(%s:%u$%d)", pc_lcl_ip, u_lcl_port, e_lcl_type);
        return -2;
    }
    ret = tnet_turn_session_create(p_lcl_sock, e_req_transport, pc_srv_host, u_srv_port, pp_self);
    TSK_OBJECT_SAFE_FREE(p_lcl_sock);
    return ret;
}

int tnet_turn_session_create_4(struct tnet_socket_s* p_lcl_sock, enum tnet_turn_transport_e e_req_transport, const char* pc_srv_host, uint16_t u_srv_port, enum tnet_socket_type_e e_srv_type, struct tnet_turn_session_s** pp_self)
{
    struct tnet_socket_s* _p_lcl_sock;
    int ret;

    if (TNET_SOCKET_TYPE_IS_STREAM(e_srv_type)) {
        // For stream the socket will be connected to the server and this is why we need to create a new socket
        if (!(_p_lcl_sock = tnet_socket_create(p_lcl_sock->ip, TNET_SOCKET_PORT_ANY/*p_lcl_sock->port*/, e_srv_type))) {
            return -2;
        }
    }
    else {
        // For UDP, use the socket socket
        _p_lcl_sock = tsk_object_ref(p_lcl_sock);
    }
    ret = tnet_turn_session_create(_p_lcl_sock, e_req_transport, pc_srv_host, u_srv_port, pp_self);
    TSK_OBJECT_SAFE_FREE(_p_lcl_sock);
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

int tnet_turn_session_set_ssl_certs(struct tnet_turn_session_s* p_self, const char* path_priv, const char* path_pub, const char* path_ca, tsk_bool_t verify)
{
    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsk_safeobj_lock(p_self);
    tsk_strupdate(&p_self->ssl.path_priv, path_priv);
    tsk_strupdate(&p_self->ssl.path_pub, path_pub);
    tsk_strupdate(&p_self->ssl.path_ca, path_ca);
    p_self->ssl.verify = verify;
    tsk_safeobj_unlock(p_self);
    return 0;
}

int tnet_turn_session_set_proxy_auto_detect(struct tnet_turn_session_s* p_self, tsk_bool_t auto_detect)
{
    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsk_safeobj_lock(p_self);
    p_self->proxy.auto_detect = auto_detect;
    tsk_safeobj_unlock(p_self);
    return 0;
}

int tnet_turn_session_set_proxy_info(struct tnet_turn_session_s* p_self, struct tnet_proxyinfo_s* info)
{
    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsk_safeobj_lock(p_self);
    TSK_OBJECT_SAFE_FREE(p_self->proxy.info);
    p_self->proxy.info = tsk_object_ref(info);
    tsk_safeobj_unlock(p_self);
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

    p_self->e_alloc_state = tnet_stun_state_none;
    p_self->e_refresh_state = tnet_stun_state_none;

    p_self->timer.rtt.alloc.id = TSK_INVALID_TIMER_ID;
    p_self->timer.rtt.refresh.id = TSK_INVALID_TIMER_ID;

    p_self->timer.u_timer_id_refresh = TSK_INVALID_TIMER_ID;
    p_self->u_lifetime_alloc_in_sec = kTurnAllocationTimeOutInSec;

    TSK_OBJECT_SAFE_FREE(p_self->p_pkt_alloc);
    TSK_OBJECT_SAFE_FREE(p_self->p_pkt_refresh);

    // create timer manager
    if (!p_self->timer.p_mgr && !(p_self->timer.p_mgr = tsk_timer_manager_create())) {
        TSK_DEBUG_ERROR("Failed to create timer manager");
        ret = -4;
        goto bail;
    }

    // create transport
    if (!p_self->p_transport) {
        if (TNET_SOCKET_TYPE_IS_DGRAM(p_self->p_lcl_sock->type)) {
            // Use the DGram socket as master
            p_self->p_transport = tnet_transport_create_2(p_self->p_lcl_sock, kTurnTransportFriendlyName);
        }
        else {
            // Use the Stream socket later and connect it
            p_self->p_transport = tnet_transport_create(p_self->p_lcl_sock->ip, TNET_SOCKET_PORT_ANY, p_self->p_lcl_sock->type, kTurnTransportFriendlyName);
        }
        if (!p_self->p_transport) {
            TSK_DEBUG_ERROR("Failed to create %s Transport", kTurnTransportFriendlyName);
            ret = -5;
            goto bail;
        }
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

    // set SSL certificates
    if (TNET_SOCKET_TYPE_IS_TLS(p_self->p_lcl_sock->type) || TNET_SOCKET_TYPE_IS_WSS(p_self->p_lcl_sock->type)) {
        if ((ret = tnet_transport_tls_set_certs(p_self->p_transport, p_self->ssl.path_ca, p_self->ssl.path_pub, p_self->ssl.path_priv, p_self->ssl.verify))) {
            TSK_DEBUG_ERROR("Failed to set SSL certificates: '%s', '%s', '%s'", p_self->ssl.path_ca, p_self->ssl.path_pub, p_self->ssl.path_priv);
            goto bail;
        }
    }

    // Proxy info
    if ((ret = tnet_transport_set_proxy_auto_detect(p_self->p_transport, p_self->proxy.auto_detect))) {
        TSK_DEBUG_ERROR("Failed to set proxy autodetect option");
        goto bail;
    }
    if (p_self->proxy.info) {
        if ((ret = tnet_transport_set_proxy_info(p_self->p_transport, p_self->proxy.info->type, p_self->proxy.info->hostname, p_self->proxy.info->port, p_self->proxy.info->username, p_self->proxy.info->password))) {
            TSK_DEBUG_ERROR("Failed to set proxy info");
            goto bail;
        }
    }

    // start network transport
    if ((ret = tnet_transport_start(p_self->p_transport))) {
        TSK_DEBUG_ERROR("Failed to start TURN transport");
        goto bail;
    }

    // Connect to the server
    if (TNET_SOCKET_TYPE_IS_STREAM(p_self->p_lcl_sock->type)) {
        tnet_fd_t fd;
        p_self->b_stream_connected = tsk_false;
        p_self->b_stream_error = tsk_false;
        fd = tnet_transport_connectto_3(p_self->p_transport, p_self->p_lcl_sock, p_self->p_srv_host, p_self->u_srv_port, p_self->p_lcl_sock->type);
        if (fd != p_self->p_lcl_sock->fd) {
            TSK_DEBUG_ERROR("Failed to connect to TURN server(%s:%d)", p_self->p_srv_host, p_self->u_srv_port);
            ret = -3;
            goto bail;
        }
        TSK_DEBUG_INFO("TURN server connection fd = %d", p_self->p_lcl_sock->fd);
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
    p_self->e_alloc_state = tnet_stun_state_none;
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
                                  TNET_STUN_PKT_ATTR_ADD_REQUESTED_TRANSPORT(p_self->e_req_transport),
                                  TNET_STUN_PKT_ATTR_ADD_SOFTWARE_ZT(kStunSoftware), // recommended for Alloc and Refresh
                                  TNET_STUN_PKT_ATTR_ADD_NULL());
    if (ret) {
        goto bail;
    }

    if ((ret = _tnet_turn_session_send_pkt(p_self, p_self->p_pkt_alloc))) {
        goto bail;
    }
    if (TNET_SOCKET_TYPE_IS_DGRAM(p_self->p_lcl_sock->type)) {
        p_self->timer.rtt.alloc.u_timeout = kStunUdpRetransmitTimoutMinInMs;
        TNET_TURN_SESSION_TIMER_SCHEDULE_MILLIS(p_self, p_self->timer.rtt.alloc.id, p_self->timer.rtt.alloc.u_timeout);
    }
    p_self->e_alloc_state = tnet_stun_state_trying;

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
    if (p_self->e_alloc_state != tnet_stun_state_ok) {
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
    if (p_self->e_alloc_state != tnet_stun_state_ok) {
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

int tnet_turn_session_get_state_alloc(const struct tnet_turn_session_s* pc_self, enum tnet_stun_state_e *pe_state)
{
    if (!pc_self || !pe_state) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    *pe_state = pc_self->e_alloc_state;
    return 0;
}

int tnet_turn_session_get_socket_local(struct tnet_turn_session_s* p_self, struct tnet_socket_s** pp_lcl_sock)
{
    if (!p_self || !pp_lcl_sock) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsk_safeobj_lock(p_self);
    *pp_lcl_sock = (struct tnet_socket_s*)tsk_object_ref(p_self->p_lcl_sock);
    tsk_safeobj_unlock(p_self);
    return 0;
}

int tnet_turn_session_get_state_createperm(const struct tnet_turn_session_s* pc_self, tnet_turn_peer_id_t u_peer_id, enum tnet_stun_state_e *pe_state)
{
    const tnet_turn_peer_t *pc_peer;
    if (!pc_self || !pe_state) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsk_safeobj_lock(pc_self);
    if ((pc_peer = tsk_list_find_object_by_pred(pc_self->p_list_peers, __pred_find_peer_by_id, &u_peer_id))) {
        *pe_state = pc_peer->e_createperm_state;
    }
    else {
        *pe_state = tnet_stun_state_none;
        if (u_peer_id != kTurnPeerIdInvalid) {
            TSK_DEBUG_WARN("TURN peer with id =%ld doesn't exist", u_peer_id);
        }
    }
    tsk_safeobj_unlock(pc_self);
    return 0;
}

int tnet_turn_session_get_state_connbind(const struct tnet_turn_session_s* pc_self, tnet_turn_peer_id_t u_peer_id, enum tnet_stun_state_e *pe_state)
{
    const tnet_turn_peer_t *pc_peer;
    if (!pc_self || !pe_state) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsk_safeobj_lock(pc_self);
    if ((pc_peer = tsk_list_find_object_by_pred(pc_self->p_list_peers, __pred_find_peer_by_id, &u_peer_id))) {
        *pe_state = pc_peer->e_connbind_state;
    }
    else {
        *pe_state = tnet_stun_state_none;
        if (u_peer_id != kTurnPeerIdInvalid) {
            TSK_DEBUG_WARN("TURN peer with id =%ld doesn't exist", u_peer_id);
        }
    }
    tsk_safeobj_unlock(pc_self);
    return 0;
}

int tnet_turn_session_get_req_transport(const struct tnet_turn_session_s* pc_self, enum tnet_turn_transport_e *pe_transport)
{
    if (!pc_self || !pe_transport) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsk_safeobj_lock(pc_self);
    *pe_transport = pc_self->e_req_transport;
    tsk_safeobj_unlock(pc_self);
    return 0;
}

int tnet_turn_session_get_bytes_count(const struct tnet_turn_session_s* pc_self, uint64_t* bytes_in, uint64_t* bytes_out)
{
    if (!pc_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    return tnet_transport_get_bytes_count(pc_self->p_transport, bytes_in, bytes_out);
}

int tnet_turn_session_createpermission(struct tnet_turn_session_s* p_self, const char* pc_peer_addr, uint16_t u_peer_port, tnet_turn_peer_id_t* pu_id)
{
    int ret = 0;
    tnet_turn_peer_t *p_peer = tsk_null;

    if (!p_self || !pc_peer_addr || !u_peer_port || !pu_id) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(p_self);

    if (!p_self->b_started) {
        TSK_DEBUG_ERROR("TURN session not started yet");
        ret = -3;
        goto bail;
    }
    if (p_self->e_alloc_state != tnet_stun_state_ok) {
        TSK_DEBUG_ERROR("No active TURN allocation yet");
        ret = -4;
        goto bail;
    }
    if ((ret = _tnet_turn_peer_create(pc_peer_addr, u_peer_port, TNET_SOCKET_TYPE_IS_IPV6(p_self->p_lcl_sock->type), &p_peer))) {
        goto bail;
    }
    if (TNET_SOCKET_TYPE_IS_STREAM(p_self->p_lcl_sock->type)) {
        if (!p_peer->p_stream_buff_in && !(p_peer->p_stream_buff_in = tsk_buffer_create_null())) {
            TSK_DEBUG_ERROR("Failed to create stream buffer for peer with id=%ld", p_peer->id);
            ret = -5;
            goto bail;
        }
    }
    if ((ret = _tnet_turn_session_send_permission(p_self, p_peer))) {
        goto bail;
    }
    *pu_id = p_peer->id;
    tsk_list_push_back_data(p_self->p_list_peers, (void**)&p_peer);

bail:
    TSK_OBJECT_SAFE_FREE(p_peer);
    tsk_safeobj_unlock(p_self);
    return ret;
}

int tnet_turn_session_deletepermission(struct tnet_turn_session_s* p_self, tnet_turn_peer_id_t u_id)
{
    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsk_safeobj_lock(p_self);
    tsk_list_remove_item_by_pred(p_self->p_list_peers, __pred_find_peer_by_id, &u_id);
    tsk_safeobj_unlock(p_self);
    return 0;
}

int tnet_turn_session_chanbind(tnet_turn_session_t* p_self, tnet_turn_peer_id_t u_peer_id)
{
    int ret = 0;
    tnet_turn_peer_t *pc_peer;

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
    if (p_self->e_alloc_state != tnet_stun_state_ok) {
        TSK_DEBUG_ERROR("No active TURN allocation yet");
        ret = -4;
        goto bail;
    }
    if (!(pc_peer = (tnet_turn_peer_t *)tsk_list_find_object_by_pred(p_self->p_list_peers, __pred_find_peer_by_id, &u_peer_id))) {
        TSK_DEBUG_ERROR("Cannot find TURN peer with id = %ld", u_peer_id);
        ret = -5;
        goto bail;
    }

    // ChannelBind must not be used for streams (e.g. TCP, TLS....)
    if (p_self->e_req_transport != tnet_turn_transport_udp) {
        TSK_DEBUG_ERROR("TURN ChannelBind not supported for stream");
        ret = -6;
        goto bail;
    }

    // create ChannelBind Request if doesn't exist (ChannelBind refresh *must* have same id)
    pc_peer->e_chanbind_state = tnet_stun_state_none;
    pc_peer->timer.rtt.chanbind.id = TSK_INVALID_TIMER_ID;
    if (!pc_peer->p_pkt_chanbind) {
        pc_peer->u_chan_num = _tnet_turn_session_get_unique_chan_num();
        if ((ret = tnet_stun_pkt_create_empty(tnet_stun_pkt_type_channelbind_request, &pc_peer->p_pkt_chanbind))) {
            TSK_DEBUG_ERROR("Failed to create TURN ChannelBind request");
            goto bail;
        }
        // add authentication info
        tnet_stun_pkt_auth_copy(pc_peer->p_pkt_chanbind, p_self->cred.p_usr_name, p_self->cred.p_pwd, p_self->p_pkt_alloc);
        // add attributes
        pc_peer->p_pkt_chanbind->opt.dontfrag = 0;
        ret = tnet_stun_pkt_attrs_add(pc_peer->p_pkt_chanbind,
                                      /* Must not add LIFETIME and there is no way to delete permission */
                                      TNET_STUN_PKT_ATTR_ADD_XOR_PEER_ADDRESS(pc_peer->b_ipv6 ? tnet_stun_address_family_ipv6 : tnet_stun_address_family_ipv4, pc_peer->u_addr_port, &pc_peer->addr_ip),
                                      TNET_STUN_PKT_ATTR_ADD_CHANNEL_NUMBER(pc_peer->u_chan_num),
                                      TNET_STUN_PKT_ATTR_ADD_NULL());
        if (ret) {
            goto bail;
        }

    }
    else {
        if ((ret = tnet_stun_utils_transac_id_rand(&pc_peer->p_pkt_chanbind->transac_id))) {
            goto bail;
        }
    }

    if ((ret = _tnet_turn_session_send_pkt(p_self, pc_peer->p_pkt_chanbind))) {
        goto bail;
    }
    if (TNET_SOCKET_TYPE_IS_DGRAM(p_self->p_lcl_sock->type)) {
        pc_peer->timer.rtt.chanbind.u_timeout = kStunUdpRetransmitTimoutMinInMs;
        TNET_TURN_SESSION_TIMER_SCHEDULE_MILLIS(p_self, pc_peer->timer.rtt.chanbind.id, pc_peer->timer.rtt.chanbind.u_timeout);
    }
    pc_peer->e_chanbind_state = tnet_stun_state_trying;

bail:
    tsk_safeobj_unlock(p_self);
    return ret;
}

// TCP-Connect rfc6062 - 4.3.  Initiating a Connection
int tnet_turn_session_connect(struct tnet_turn_session_s* p_self, tnet_turn_peer_id_t u_peer_id)
{
    int ret = 0;
    tnet_turn_peer_t *pc_peer;

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
    if (p_self->e_alloc_state != tnet_stun_state_ok) {
        TSK_DEBUG_ERROR("No active TURN allocation yet");
        ret = -4;
        goto bail;
    }
    if (!(pc_peer = (tnet_turn_peer_t *)tsk_list_find_object_by_pred(p_self->p_list_peers, __pred_find_peer_by_id, &u_peer_id))) {
        TSK_DEBUG_ERROR("Cannot find TURN peer with id = %ld", u_peer_id);
        ret = -5;
        goto bail;
    }

    // Connect must be used for streams (e.g. TCP, TLS....) only
    if (p_self->e_req_transport != tnet_turn_transport_tcp) {
        TSK_DEBUG_ERROR("TURN Connect not supported for UDP relay");
        ret = -6;
        goto bail;
    }

    // create Connect Request if doesn't exist (Connect refresh *must* have same id)
    pc_peer->e_connect_state = tnet_stun_state_none;
    pc_peer->e_connbind_state = tnet_stun_state_none;
    if (!pc_peer->p_pkt_connect) {
        if ((ret = tnet_stun_pkt_create_empty(tnet_stun_pkt_type_connect_request, &pc_peer->p_pkt_connect))) {
            TSK_DEBUG_ERROR("Failed to create TURN Connect request");
            goto bail;
        }
        // add authentication info
        tnet_stun_pkt_auth_copy(pc_peer->p_pkt_connect, p_self->cred.p_usr_name, p_self->cred.p_pwd, p_self->p_pkt_alloc);
        // add attributes
        pc_peer->p_pkt_connect->opt.dontfrag = 0;
        ret = tnet_stun_pkt_attrs_add(pc_peer->p_pkt_connect,
                                      /* Must not add LIFETIME and there is no way to delete permission */
                                      TNET_STUN_PKT_ATTR_ADD_XOR_PEER_ADDRESS(pc_peer->b_ipv6 ? tnet_stun_address_family_ipv6 : tnet_stun_address_family_ipv4, pc_peer->u_addr_port, &pc_peer->addr_ip),
                                      TNET_STUN_PKT_ATTR_ADD_NULL());
        if (ret) {
            goto bail;
        }

    }
    else {
        if ((ret = tnet_stun_utils_transac_id_rand(&pc_peer->p_pkt_connect->transac_id))) {
            goto bail;
        }
    }

    if ((ret = _tnet_turn_session_send_pkt(p_self, pc_peer->p_pkt_connect))) {
        goto bail;
    }
    pc_peer->e_connect_state = tnet_stun_state_trying;

bail:
    tsk_safeobj_unlock(p_self);
    return ret;
}

int tnet_turn_session_send_data(tnet_turn_session_t* p_self, tnet_turn_peer_id_t u_peer_id, const void* pc_data_ptr, uint16_t u_data_size)
{
    int ret = 0;
    tnet_turn_peer_t* pc_peer;

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
    if (p_self->e_alloc_state != tnet_stun_state_ok) {
        TSK_DEBUG_ERROR("No active TURN allocation yet");
        ret = -3;
        goto bail;
    }
    if (!(pc_peer = (tnet_turn_peer_t *)tsk_list_find_object_by_pred(p_self->p_list_peers, __pred_find_peer_by_id, &u_peer_id))) {
        TSK_DEBUG_ERROR("Cannot find TURN peer with id = %ld", u_peer_id);
        ret = -4;
        goto bail;
    }
    if (pc_peer->e_createperm_state != tnet_stun_state_ok) {
        TSK_DEBUG_ERROR("No active TURN permission for the remote peer");
        ret = -5;
        goto bail;
    }

    /** Send Stream **/
    if (TNET_SOCKET_TYPE_IS_STREAM(p_self->p_lcl_sock->type) && p_self->e_req_transport == tnet_turn_transport_tcp) {
        ret = _tnet_turn_session_send_stream_raw(p_self, pc_peer, pc_data_ptr, u_data_size);
        goto bail;
    }

    /*** ChannelData ***/
    if (pc_peer->e_chanbind_state == tnet_stun_state_ok) {
        ret = _tnet_turn_session_send_chandata(p_self, pc_peer, pc_data_ptr, u_data_size);
        goto bail;
    }

    /*** Send indication ***/
    if (!pc_peer->p_pkt_sendind) {
        if ((ret = tnet_stun_pkt_create_empty(tnet_stun_pkt_type_send_indication, &pc_peer->p_pkt_sendind))) {
            TSK_DEBUG_ERROR("Failed to create TURN SendIndication request");
            goto bail;
        }
        pc_peer->p_pkt_sendind->opt.dontfrag = 0;
        // add authinfo
        tnet_stun_pkt_auth_copy(pc_peer->p_pkt_sendind, p_self->cred.p_usr_name, p_self->cred.p_pwd, p_self->p_pkt_alloc);
        // add attributes
        ret = tnet_stun_pkt_attrs_add(pc_peer->p_pkt_sendind,
                                      TNET_STUN_PKT_ATTR_ADD_XOR_PEER_ADDRESS(pc_peer->b_ipv6 ? tnet_stun_address_family_ipv6 : tnet_stun_address_family_ipv4, pc_peer->u_addr_port, &pc_peer->addr_ip),
                                      TNET_STUN_PKT_ATTR_ADD_DATA(pc_data_ptr, u_data_size),
                                      TNET_STUN_PKT_ATTR_ADD_NULL());
        if (ret) {
            goto bail;
        }
    }
    else {
        const tnet_stun_attr_vdata_t *pc_attr_data;
        if ((ret = tnet_stun_pkt_attr_find_first(pc_peer->p_pkt_sendind, tnet_stun_attr_type_data, (const tnet_stun_attr_t**)&pc_attr_data))) {
            goto bail;
        }
        if (!pc_attr_data) {
            ret = tnet_stun_pkt_attrs_add(pc_peer->p_pkt_sendind,
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
        if ((ret = tnet_stun_utils_transac_id_rand(&pc_peer->p_pkt_sendind->transac_id))) {
            goto bail;
        }
    }
    if ((ret = _tnet_turn_session_send_pkt_0(p_self, pc_peer, pc_peer->p_pkt_sendind))) {
        goto bail;
    }

bail:
    tsk_safeobj_unlock(p_self);
    return ret;
}

int tnet_turn_session_is_active(const struct tnet_turn_session_s* pc_self, tnet_turn_peer_id_t u_peer_id, tsk_bool_t *pb_active)
{
    if (!pc_self || !pb_active) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsk_safeobj_lock(pc_self);
    *pb_active = pc_self->b_started
                 && (pc_self->e_alloc_state == tnet_stun_state_ok);
    if (*pb_active) {
        const tnet_turn_peer_t* pc_peer;
        if ((pc_peer = (const tnet_turn_peer_t *)tsk_list_find_object_by_pred(pc_self->p_list_peers, __pred_find_peer_by_id, &u_peer_id))) {
            *pb_active = (pc_peer->e_createperm_state == tnet_stun_state_ok);
        }
        else {
            *pb_active = tsk_false;
        }
    }
    tsk_safeobj_unlock(pc_self);
    return 0;
}

int tnet_turn_session_is_stream(const struct tnet_turn_session_s* pc_self, tsk_bool_t *pb_stream)
{
    if (!pc_self || !pb_stream) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsk_safeobj_lock(pc_self);
    *pb_stream = TNET_SOCKET_TYPE_IS_STREAM(pc_self->p_lcl_sock->type) ? tsk_true : tsk_false;
    tsk_safeobj_unlock(pc_self);
    return 0;
}

// Check "ConnectionBind" sent and underlaying socket is connected
int tnet_turn_session_is_stream_connected(const struct tnet_turn_session_s* pc_self, tnet_turn_peer_id_t u_peer_id, tsk_bool_t *pb_connected)
{
    if (!pc_self || !pb_connected) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsk_safeobj_lock(pc_self);
    *pb_connected = pc_self->b_started
                    && (pc_self->e_alloc_state == tnet_stun_state_ok);
    if (*pb_connected) {
        const tnet_turn_peer_t* pc_peer;
        if ((pc_peer = (const tnet_turn_peer_t *)tsk_list_find_object_by_pred(pc_self->p_list_peers, __pred_find_peer_by_id, &u_peer_id))) {
            *pb_connected = (pc_peer->conn_fd != TNET_INVALID_FD && pc_peer->b_stream_connected && pc_peer->e_connbind_state == tnet_stun_state_ok);
        }
        else {
            *pb_connected = tsk_false;
        }
    }
    tsk_safeobj_unlock(pc_self);
    return 0;
}

int tnet_turn_session_stop(tnet_turn_session_t* p_self)
{
    int ret = 0;

    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    // FIXME
    // tsk_safeobj_lock(p_self);

    p_self->b_stopping = tsk_true;

    if (p_self->e_alloc_state == tnet_stun_state_ok) {
        // UnAlloc
        p_self->u_lifetime_alloc_in_sec = 0;
        _tnet_turn_session_send_refresh(p_self);
    }

    if (p_self->timer.p_mgr) {
        ret = tsk_timer_manager_stop(p_self->timer.p_mgr);
    }

    // free transport to force next call to prepare() to create new one with new sockets
    if (p_self->p_transport) {
        tnet_transport_shutdown(p_self->p_transport);
        TSK_OBJECT_SAFE_FREE(p_self->p_transport);
    }

    // clear peers
    tsk_list_clear_items(p_self->p_list_peers);

    p_self->b_prepared = tsk_false;
    p_self->b_started = tsk_false;
    p_self->b_stopping = tsk_false;

    // tsk_safeobj_unlock(p_self);

    return ret;
}

static int _tnet_turn_session_peer_find_by_id(const tnet_turn_session_t* pc_self, tnet_turn_peer_id_t id, const struct tnet_turn_peer_s **ppc_peer)
{
    const tsk_list_item_t *pc_item;
    const struct tnet_turn_peer_s *pc_peer;
    if (!pc_self || !ppc_peer) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    *ppc_peer = tsk_null;
    tsk_list_foreach(pc_item, pc_self->p_list_peers) {
        if (!(pc_peer = pc_item->data)) {
            continue;
        }
        if (pc_peer->id == id) {
            *ppc_peer = pc_peer;
            break;
        }
    }
    return 0;
}

static int _tnet_turn_session_peer_find_by_timer(const tnet_turn_session_t* pc_self, tsk_timer_id_t id, const struct tnet_turn_peer_s **ppc_peer)
{
    const tsk_list_item_t *pc_item;
    const struct tnet_turn_peer_s *pc_peer;
    if (!pc_self || !ppc_peer) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    *ppc_peer = tsk_null;
    tsk_list_foreach(pc_item, pc_self->p_list_peers) {
        if (!(pc_peer = pc_item->data)) {
            continue;
        }
        if (pc_peer->timer.rtt.chanbind.id == id || pc_peer->timer.rtt.createperm.id == id) {
            *ppc_peer = pc_peer;
            break;
        }
    }
    return 0;
}

static uint16_t _tnet_turn_session_get_unique_chan_num()
{
    // rfc5766 - The channel number is in the range 0x4000 through 0x7FFE (inclusive)
    static long __l_chan_num = 0;
    tsk_atomic_inc(&__l_chan_num);
    return (__l_chan_num % (0x7FFE - 0x4000)) + 0x4000;
}

static int _tnet_turn_session_send_chandata(tnet_turn_session_t* p_self, const tnet_turn_peer_t* pc_peer, const void* pc_buff_ptr, tsk_size_t u_buff_size)
{
    int ret = 0;
    tsk_size_t PadSize, NeededSize;
    uint8_t* _p_buff_chandata_ptr;
    if (!p_self || !pc_peer || !pc_buff_ptr || !u_buff_size) {
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
    if (pc_peer->e_chanbind_state != tnet_stun_state_ok) {
        TSK_DEBUG_ERROR("No active TURN data channel for peer id = %ld", pc_peer->id);
        ret = -3;
        goto bail;
    }

    // rfc5766 - 11.5.  Sending a ChannelData Message
    if (TNET_SOCKET_TYPE_IS_DGRAM(p_self->p_lcl_sock->type)) {
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

    *((uint16_t*)&_p_buff_chandata_ptr[0]) = tnet_htons(pc_peer->u_chan_num); // Channel Number
    *((uint16_t*)&_p_buff_chandata_ptr[2]) = tnet_htons((uint16_t)u_buff_size); // Length
    memcpy(&_p_buff_chandata_ptr[kStunChannelDataHdrSizeInOctets], pc_buff_ptr, u_buff_size); // Application Data
    if (PadSize) {
        memset(&_p_buff_chandata_ptr[kStunChannelDataHdrSizeInOctets + u_buff_size], 0, PadSize); // Set padding bytes to zero (not required but ease debugging)
    }

    if ((ret = _tnet_turn_session_send_buff_0(p_self, pc_peer, p_self->p_buff_chandata_ptr, NeededSize))) {
        goto bail;
    }

bail:
    // unlock()
    tsk_safeobj_unlock(p_self);
    return ret;
}

static int _tnet_turn_session_send_stream_raw(tnet_turn_session_t* p_self, tnet_turn_peer_t* pc_peer, const void* pc_buff_ptr, tsk_size_t u_buff_size)
{
    int ret = 0;
    if (!p_self || !pc_peer || !pc_buff_ptr || !u_buff_size) {
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
    if (TNET_SOCKET_TYPE_IS_DGRAM(p_self->p_lcl_sock->type)) {
        TSK_DEBUG_ERROR("Must not call this function for non-stream transports");
        ret = -3;
        goto bail;
    }
    if (pc_peer->e_connbind_state != tnet_stun_state_ok) {
        TSK_DEBUG_ERROR("No active TURN TCP connection for peer id = %ld", pc_peer->id);
        ret = -4;
        goto bail;
    }

    if ((ret = _tnet_turn_session_send_buff_0(p_self, pc_peer, pc_buff_ptr, u_buff_size))) {
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
    if (p_self->e_alloc_state != tnet_stun_state_ok) {
        TSK_DEBUG_ERROR("No active TURN allocation yet");
        ret = -4;
        goto bail;
    }

    p_self->e_refresh_state = tnet_stun_state_none;
    // create RefreshIndication Request
    p_self->timer.rtt.refresh.id = TSK_INVALID_TIMER_ID;
    TSK_OBJECT_SAFE_FREE(p_self->p_pkt_refresh);
    if ((ret = tnet_stun_pkt_create_empty(tnet_stun_pkt_type_refresh_request, &p_self->p_pkt_refresh))) {
        TSK_DEBUG_ERROR("Failed to create TURN RefreshIndication request");
        goto bail;
    }
    // add authentication info
    tnet_stun_pkt_auth_copy(p_self->p_pkt_refresh, p_self->cred.p_usr_name, p_self->cred.p_pwd, p_self->p_pkt_alloc);
#if 0
    if (p_self->u_lifetime_alloc_in_sec == 0) {
        const tnet_stun_attr_vdata_t *pc_attr_nonce = tsk_null;
        tnet_stun_pkt_attr_find_first(p_self->p_pkt_refresh, tnet_stun_attr_type_nonce, (const tnet_stun_attr_t**)&pc_attr_nonce);
        if (pc_attr_nonce) {
            pc_attr_nonce->p_data_ptr[0] = 'a';
        }
    }
#endif
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
    if (TNET_SOCKET_TYPE_IS_DGRAM(p_self->p_lcl_sock->type)) {
        p_self->timer.rtt.refresh.u_timeout = kStunUdpRetransmitTimoutMinInMs;
        TNET_TURN_SESSION_TIMER_SCHEDULE_MILLIS(p_self, p_self->timer.rtt.refresh.id, p_self->timer.rtt.refresh.u_timeout);
    }
    p_self->e_refresh_state = tnet_stun_state_trying;

bail:
    tsk_safeobj_unlock(p_self);
    return ret;
}

static int _tnet_turn_session_send_permission(struct tnet_turn_session_s* p_self, tnet_turn_peer_t *p_peer)
{
    int ret = 0;
    if (!p_self || !p_peer) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(p_self);

    if (!p_self->b_started) {
        TSK_DEBUG_ERROR("TURN session not started yet");
        ret = -3;
        goto bail;
    }
    if (p_self->e_alloc_state != tnet_stun_state_ok) {
        TSK_DEBUG_ERROR("No active TURN allocation yet");
        ret = -4;
        goto bail;
    }

    // create CreatePermission Request
    p_peer->e_createperm_state = tnet_stun_state_none;
    p_peer->timer.rtt.createperm.id = TSK_INVALID_TIMER_ID;
    TSK_OBJECT_SAFE_FREE(p_peer->p_pkt_createperm);
    if ((ret = tnet_stun_pkt_create_empty(tnet_stun_pkt_type_createpermission_request, &p_peer->p_pkt_createperm))) {
        TSK_DEBUG_ERROR("Failed to create TURN CreatePermission request");
        goto bail;
    }
    // add authinfo
    tnet_stun_pkt_auth_copy(p_peer->p_pkt_createperm, p_self->cred.p_usr_name, p_self->cred.p_pwd, p_self->p_pkt_alloc);
    // add attributes
    p_peer->p_pkt_createperm->opt.dontfrag = 0;
    ret = tnet_stun_pkt_attrs_add(p_peer->p_pkt_createperm,
                                  /* Must not add LIFETIME and there is no way to delete permission */
                                  TNET_STUN_PKT_ATTR_ADD_XOR_PEER_ADDRESS(p_peer->b_ipv6 ? tnet_stun_address_family_ipv6 : tnet_stun_address_family_ipv4, p_peer->u_addr_port, &p_peer->addr_ip),
                                  TNET_STUN_PKT_ATTR_ADD_NULL());
    if (ret) {
        goto bail;
    }

    if ((ret = _tnet_turn_session_send_pkt(p_self, p_peer->p_pkt_createperm))) {
        goto bail;
    }
    if (TNET_SOCKET_TYPE_IS_DGRAM(p_self->p_lcl_sock->type)) {
        p_peer->timer.rtt.createperm.u_timeout = kStunUdpRetransmitTimoutMinInMs;
        TNET_TURN_SESSION_TIMER_SCHEDULE_MILLIS(p_self, p_peer->timer.rtt.createperm.id, p_peer->timer.rtt.createperm.u_timeout);
    }
    p_peer->e_createperm_state = tnet_stun_state_trying;

bail:
    tsk_safeobj_unlock(p_self);
    return ret;
}

// TCP-Connect rfc6062 - 4.3.  Initiating a Connection
static int _tnet_turn_session_send_connbind(struct tnet_turn_session_s* p_self, tnet_turn_peer_t *p_peer)
{
    int ret = 0;

    if (!p_self || !p_peer) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(p_self);

    if (!p_self->b_started) {
        TSK_DEBUG_ERROR("TURN session not started yet");
        ret = -3;
        goto bail;
    }
    if (p_self->e_alloc_state != tnet_stun_state_ok) {
        TSK_DEBUG_ERROR("No active TURN allocation yet");
        ret = -4;
        goto bail;
    }

    // ConnectionBind must be sent after Connect
    if (p_peer->e_connect_state != tnet_stun_state_ok) {
        TSK_DEBUG_ERROR("No active TURN connection yet");
        ret = -6;
        goto bail;
    }

    // Connect must be used for streams (e.g. TCP, TLS....) only
    if (p_self->e_req_transport != tnet_turn_transport_tcp) {
        TSK_DEBUG_ERROR("TURN ConnectionBind not supported for UDP transport");
        ret = -7;
        goto bail;
    }

    // create Connect Request if doesn't exist (Connect refresh *must* have same id)
    p_peer->e_connbind_state = tnet_stun_state_none;
    if (!p_peer->p_pkt_connbind) {
        if ((ret = tnet_stun_pkt_create_empty(tnet_stun_pkt_type_connectionbind_request, &p_peer->p_pkt_connbind))) {
            TSK_DEBUG_ERROR("Failed to create TURN ConnectionBind request");
            goto bail;
        }
        // add authentication info
        tnet_stun_pkt_auth_copy(p_peer->p_pkt_connbind, p_self->cred.p_usr_name, p_self->cred.p_pwd, p_self->p_pkt_alloc);
        // add attributes
        p_peer->p_pkt_connbind->opt.dontfrag = 0;
        ret = tnet_stun_pkt_attrs_add(p_peer->p_pkt_connbind,
                                      // Error "420" when XOR-PEER-ADDRESS attribute is added to ConnectionBind request
                                      /* TNET_STUN_PKT_ATTR_ADD_XOR_PEER_ADDRESS(p_peer->b_ipv6 ? tnet_stun_address_family_ipv6 : tnet_stun_address_family_ipv4, p_peer->u_addr_port, &p_peer->addr_ip), */
                                      TNET_STUN_PKT_ATTR_ADD_CONNECTION_ID(p_peer->u_conn_id),
                                      TNET_STUN_PKT_ATTR_ADD_NULL());
        if (ret) {
            goto bail;
        }

    }
    else {
        if ((ret = tnet_stun_utils_transac_id_rand(&p_peer->p_pkt_connbind->transac_id))) {
            goto bail;
        }
    }

    if ((ret = _tnet_turn_session_send_pkt_0(p_self, p_peer, p_peer->p_pkt_connbind))) {
        goto bail;
    }
    p_peer->e_connbind_state = tnet_stun_state_trying;

bail:
    tsk_safeobj_unlock(p_self);
    return ret;
}

static int _tnet_turn_session_send_buff_0(tnet_turn_session_t* p_self, const tnet_turn_peer_t* pc_peer, const void* pc_buff_ptr, tsk_size_t u_buff_size)
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

    if (TNET_SOCKET_TYPE_IS_DGRAM(p_self->p_lcl_sock->type)) {
#if 1
        u_sent_bytes = tnet_transport_sendto(p_self->p_transport, p_self->p_lcl_sock->fd, (const struct sockaddr *)&p_self->srv_addr, pc_buff_ptr, u_buff_size);
#else
        u_sent_bytes = tnet_sockfd_sendto(p_self->p_lcl_sock->fd, (const struct sockaddr *)&p_self->srv_addr, pc_buff_ptr, u_buff_size);
#endif
    }
    else {
        if (pc_peer && pc_peer->b_stream_connected && pc_peer->conn_fd != TNET_INVALID_FD) {
            // Send using Peer connection if connected
            // Should never be called because for now requested transport is always equal to UDP
#if 1
            u_sent_bytes = tnet_transport_send(p_self->p_transport, pc_peer->conn_fd, pc_buff_ptr, u_buff_size);
#else
            u_sent_bytes = tnet_sockfd_send(pc_peer->conn_fd, pc_buff_ptr, u_buff_size, 0);
#endif
        }
        else {
            tsk_bool_t b_delay_send = tsk_false;
            // Connect if not already done
            if (!p_self->b_stream_connected) {
                ret = tnet_sockfd_waitUntilWritable(p_self->p_lcl_sock->fd, kTurnTransportConnectTimeout);
                if (ret == 0) { // socket is valid but not connected (e.g. Proxy handshaking not completed yet)
                    TSK_DEBUG_INFO("Saving %u TURN bytes and waiting for 'connected' event before sending", (unsigned)u_buff_size);
                    if (!p_self->p_stream_buff_out && !(p_self->p_stream_buff_out = tsk_buffer_create_null())) {
                        TSK_DEBUG_ERROR("Failed to create buffer");
                        ret = -3;
                        goto bail;
                    }
                    if ((p_self->p_stream_buff_out->size + u_buff_size) > kTurnStreamOutMaxSize) {
                        TSK_DEBUG_ERROR("To much pending data");
                        ret = -5;
                        goto bail;
                    }
                    tsk_buffer_append(p_self->p_stream_buff_out, pc_buff_ptr, u_buff_size);
                    b_delay_send = tsk_true;
                    u_sent_bytes = u_buff_size;
                }
                else {
                    TSK_DEBUG_ERROR("Socket in invalid state");
                    ret = -6;
                    goto bail;
                }
            }
            if (!b_delay_send && p_self->b_stream_connected) {
#if 1
                u_sent_bytes = tnet_transport_send(p_self->p_transport, p_self->p_lcl_sock->fd, pc_buff_ptr, u_buff_size);
#else
                u_sent_bytes = tnet_socket_send_stream(p_self->p_lcl_sock, pc_buff_ptr, u_buff_size);
#endif
            }
        }
    }
    if (u_sent_bytes != u_buff_size) {
        TSK_DEBUG_ERROR("Failed to send %u bytes. Only %u sent", (unsigned)u_buff_size, (unsigned)u_sent_bytes);
        ret = -2;
        goto bail;
    }

bail:
    // unlock()
    tsk_safeobj_unlock(p_self);
    return ret;
}

static int _tnet_turn_session_send_buff(tnet_turn_session_t* p_self, const void* pc_buff_ptr, tsk_size_t u_buff_size)
{
    return _tnet_turn_session_send_buff_0(p_self, tsk_null/*peer*/, pc_buff_ptr, u_buff_size);
}

static int _tnet_turn_session_send_pkt_0(tnet_turn_session_t* p_self, const tnet_turn_peer_t* pc_peer, const tnet_turn_pkt_t *pc_pkt)
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
            TSK_DEBUG_ERROR("Failed to allocate buffer with size = %u", (unsigned)u_min_size);
            ret = -3;
            p_self->u_buff_send_size = 0;
            goto bail;
        }
        p_self->u_buff_send_size = u_min_size;
    }

    if ((ret = tnet_stun_pkt_write_with_padding(pc_pkt, p_self->p_buff_send_ptr, p_self->u_buff_send_size, &u_min_size))) {
        goto bail;
    }
    if ((ret = _tnet_turn_session_send_buff_0(p_self, pc_peer, p_self->p_buff_send_ptr, u_min_size))) {
        goto bail;
    }

bail:
    tsk_safeobj_unlock(p_self);
    return ret;
}

static int _tnet_turn_session_send_pkt(tnet_turn_session_t* p_self, const tnet_turn_pkt_t *pc_pkt)
{
    return _tnet_turn_session_send_pkt_0(p_self, tsk_null/*peer*/, pc_pkt);
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
        TSK_DEBUG_ERROR("UNKNOWN-ATTRIBUTES missing in 420");
        ret = -3;
        goto bail;
    }
    for (u16 = 0; u16 < pc_attr->u_data_size; u16+=2) {
        switch (*((uint16_t*)&pc_attr->p_data_ptr[u16])) {
        case tnet_stun_attr_type_dont_fragment: {
            p_pkt_req->opt.dontfrag = 0;
            b_done = tsk_true;
            break;
        }
        case tnet_stun_attr_type_fingerprint: {
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

static int _tnet_turn_session_process_incoming_pkt(struct tnet_turn_session_s* p_self, const tnet_turn_pkt_t *pc_pkt, tsk_bool_t *pb_processed)
{
    int ret = 0;
    if (!p_self || !pc_pkt || !pb_processed) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    *pb_processed = tsk_false;

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
    case tnet_stun_pkt_type_connect_success_response:
    case tnet_stun_pkt_type_connect_error_response:
    case tnet_stun_pkt_type_connectionbind_success_response:
    case tnet_stun_pkt_type_connectionbind_error_response: {
        const tnet_stun_attr_error_code_t* pc_attr_err = tsk_null;
        uint16_t u_code = 0;
        tnet_turn_pkt_t *pc_pkt_req = tsk_null;
        tnet_turn_peer_t* pc_peer = tsk_null;

#define CANCEL_TIMER(parent, which) \
	if (TSK_TIMER_ID_IS_VALID(parent->timer.rtt.which.id)) { \
		tsk_timer_manager_cancel(p_self->timer.p_mgr, parent->timer.rtt.which.id); \
		parent->timer.rtt.which.id = TSK_INVALID_TIMER_ID; \
	}

        // Find request
        if (p_self->p_pkt_alloc && tnet_stun_utils_transac_id_cmp(p_self->p_pkt_alloc->transac_id, pc_pkt->transac_id) == 0) {
            pc_pkt_req = p_self->p_pkt_alloc;
            CANCEL_TIMER(p_self, alloc);
            CANCEL_TIMER(p_self, refresh);
        }
        else if ((pc_peer = (tnet_turn_peer_t*)tsk_list_find_object_by_pred(p_self->p_list_peers, __pred_find_peer_by_transacid_createperm, &pc_pkt->transac_id))) {
            pc_pkt_req = pc_peer->p_pkt_createperm;
            CANCEL_TIMER(pc_peer, createperm);
        }
        else if ((pc_peer = (tnet_turn_peer_t*)tsk_list_find_object_by_pred(p_self->p_list_peers, __pred_find_peer_by_transacid_sendind, &pc_pkt->transac_id))) {
            pc_pkt_req = pc_peer->p_pkt_sendind;
        }
        else if ((pc_peer = (tnet_turn_peer_t*)tsk_list_find_object_by_pred(p_self->p_list_peers, __pred_find_peer_by_transacid_chanbind, &pc_pkt->transac_id))) {
            pc_pkt_req = pc_peer->p_pkt_chanbind;
            CANCEL_TIMER(pc_peer, chanbind);
        }
        else if ((pc_peer = (tnet_turn_peer_t*)tsk_list_find_object_by_pred(p_self->p_list_peers, __pred_find_peer_by_transacid_connect, &pc_pkt->transac_id))) {
            pc_pkt_req = pc_peer->p_pkt_connect;
            // TCP: no timer
        }
        else if ((pc_peer = (tnet_turn_peer_t*)tsk_list_find_object_by_pred(p_self->p_list_peers, __pred_find_peer_by_transacid_connectionbind, &pc_pkt->transac_id))) {
            pc_pkt_req = pc_peer->p_pkt_connbind;
            // TCP: no timer
        }
        else if (p_self->p_pkt_refresh && tnet_stun_utils_transac_id_cmp(p_self->p_pkt_refresh->transac_id, pc_pkt->transac_id) == 0) {
            pc_pkt_req = p_self->p_pkt_refresh;
            CANCEL_TIMER(p_self, refresh);
        }

        if (!pc_pkt_req) {
            TSK_DEBUG_INFO("No matching request[TID=%s]", pc_pkt->transac_id);
            // Not an error as the "fd" could be shared by several processes (e.g. ICE)
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

                p_self->e_alloc_state = tnet_stun_state_ok;
                p_self->b_rel_ipv6 = (pc_attr_addr->e_family == tnet_stun_address_family_ipv6);
                p_self->u_rel_port = pc_attr_addr->u_port;
                tsk_strupdate(&p_self->p_rel_ip, ip_addr);

                _tnet_turn_session_raise_event_alloc_ok(p_self);
            }
            // --- CREATE-PERMISSION --- //
            else if (pc_pkt_req->e_type == tnet_stun_pkt_type_createpermission_request) {
                TSK_DEBUG_INFO("TURN 'CREATE-PERMISSION' OK for peer-id = %ld", pc_peer->id);
                pc_peer->e_createperm_state = tnet_stun_state_ok;
                TNET_TURN_SESSION_TIMER_SCHEDULE_MILLIS(p_self, pc_peer->timer.fresh.createperm.id, pc_peer->timer.fresh.createperm.u_timeout);
                _tnet_turn_session_raise_event_createperm_ok(p_self, pc_peer->id);
            }
            // --- CHANNEL-BIND --- //
            else if (pc_pkt_req->e_type == tnet_stun_pkt_type_channelbind_request) {
                TSK_DEBUG_INFO("TURN 'CHANNEL-BIND' OK for peer-id = %ld", pc_peer->id);
                pc_peer->e_chanbind_state = tnet_stun_state_ok;
                TNET_TURN_SESSION_TIMER_SCHEDULE_MILLIS(p_self, pc_peer->timer.fresh.chanbind.id, pc_peer->timer.fresh.chanbind.u_timeout);
                _tnet_turn_session_raise_event_chanbind_ok(p_self, pc_peer->id);
            }
            // --- CONNECT --- //
            else if (pc_pkt_req->e_type == tnet_stun_pkt_type_connect_request) {
                TSK_DEBUG_INFO("TURN 'CONNECT' OK for peer-id = %ld", pc_peer->id);
                if ((ret = _tnet_turn_session_process_success_connect_pkt(p_self, pc_peer, pc_pkt))) {
                    goto bail;
                }
            }
            // --- CONNECTION-BIND --- //
            else if (pc_pkt_req->e_type == tnet_stun_pkt_type_connectionbind_request) {
                TSK_DEBUG_INFO("TURN 'CONNECTION-BIND' OK for peer-id = %ld", pc_peer->id);
                pc_peer->e_connbind_state = tnet_stun_state_ok;
                _tnet_turn_session_raise_event_connectionbind_ok(p_self, pc_peer->id);
            }
            // --- REFRESH --- //
            else if (pc_pkt_req == p_self->p_pkt_refresh) {
                const tnet_stun_attr_vdata_t *pc_attr_lifetime;
                TSK_DEBUG_INFO("TURN 'REFRESH' OK for peer-id = %ld", pc_peer ? pc_peer->id : -1);
                p_self->e_refresh_state = tnet_stun_state_ok;
                if ((ret = tnet_stun_pkt_attr_find_first(pc_pkt, tnet_stun_attr_type_lifetime, (const tnet_stun_attr_t**)&pc_attr_lifetime)) == 0 && pc_attr_lifetime && pc_attr_lifetime->u_data_size == 4) {
                    p_self->u_lifetime_alloc_in_sec = TSK_TO_UINT32(pc_attr_lifetime->p_data_ptr);
                }
                TNET_TURN_SESSION_TIMER_SCHEDULE_SEC(p_self, p_self->timer.u_timer_id_refresh, p_self->u_lifetime_alloc_in_sec);
                _tnet_turn_session_raise_event_refresh_ok(p_self);
            }
        }
        /*** ERROR ***/
        else {
            tsk_bool_t b_nok = tsk_true;
            if ((ret = tnet_stun_pkt_get_errorcode(pc_pkt, &u_code))) {
                goto bail;
            }
            if (u_code > 299) {
                tnet_stun_pkt_attr_find_first(pc_pkt, tnet_stun_attr_type_error_code, (const tnet_stun_attr_t**)&pc_attr_err);
            }
            if (u_code == kStunErrCodeUnauthorized || u_code == kStunErrCodeStaleNonce) {
                if (u_code == kStunErrCodeUnauthorized) {
                    // Make sure this is not an authentication failure (#2 401)
                    // Do not send another req to avoid endless messages
                    if ((tnet_stun_pkt_attr_exists(pc_pkt_req, tnet_stun_attr_type_message_integrity))) { // already has a MESSAGE-INTEGRITY?
                        TSK_DEBUG_ERROR("TURN authentication failed");
                        goto check_nok;
                        // INDICATION messages cannot receive 401 responses
                    }
                }
                if ((ret = tnet_stun_pkt_auth_prepare_2(pc_pkt_req, p_self->cred.p_usr_name, p_self->cred.p_pwd, pc_pkt))) {
                    goto check_nok;
                }
                if ((ret = _tnet_turn_session_send_pkt_0(p_self, pc_peer, pc_pkt_req)) == 0) {
                    b_nok = tsk_false;
                    goto check_nok;
                }
            }
            else if (u_code == kStunErrCodeUnknownAttributes) {
                if((ret = _tnet_turn_session_process_err420_pkt(pc_pkt_req, pc_pkt))) {
                    goto check_nok;
                }
                if ((ret = _tnet_turn_session_send_pkt_0(p_self, pc_peer, pc_pkt_req)) == 0) {
                    b_nok = tsk_false;
                    goto check_nok;
                }
            }

check_nok:
            if (b_nok) {
                TSK_DEBUG_INFO("--- TURN response code = %hu, phrase = %s, peer-id=%ld, ---",
                               u_code, pc_attr_err ? pc_attr_err->p_reason_phrase : "null", pc_peer ? pc_peer->id : -1);
                if (pc_pkt_req == p_self->p_pkt_alloc) {
                    p_self->e_alloc_state = tnet_stun_state_nok;
                    _tnet_turn_session_raise_event_alloc_nok(p_self);
                }
                else if (pc_pkt_req->e_type == tnet_stun_pkt_type_channelbind_request) {
                    pc_peer->e_chanbind_state = tnet_stun_state_nok;
                    _tnet_turn_session_raise_event_chanbind_nok(p_self, pc_peer->id);
                }
                else if (pc_pkt_req->e_type == tnet_stun_pkt_type_createpermission_request) {
                    pc_peer->e_createperm_state = tnet_stun_state_nok;
                    _tnet_turn_session_raise_event_createperm_nok(p_self, pc_peer->id);
                }
                else if (pc_pkt_req == p_self->p_pkt_refresh) {
                    p_self->e_refresh_state = tnet_stun_state_nok;
                    _tnet_turn_session_raise_event_refresh_nok(p_self);
                }
            }
        }

        break;
    }
    case tnet_stun_pkt_type_connectionattempt_indication: {
        // Indicates we already have an active connection
        const tnet_stun_attr_address_t *pc_attr_xor_peer_addr;
        tnet_turn_peer_t* pc_peer = tsk_null;
        // XOR-PEER-ADDRESS
        if ((ret = tnet_stun_pkt_attr_find_first(pc_pkt, tnet_stun_attr_type_xor_peer_address, (const tnet_stun_attr_t**)&pc_attr_xor_peer_addr)) == 0 && pc_attr_xor_peer_addr) {
            if ((ret = _tnet_turn_peer_find_by_xpeer(p_self->p_list_peers, pc_attr_xor_peer_addr, (const tnet_turn_peer_t**)&pc_peer)) == 0 && pc_peer) {
                if ((ret = _tnet_turn_session_process_success_connect_pkt(p_self, pc_peer, pc_pkt))) {
                    goto bail;
                }
            }
        }
        break;
    }
    case tnet_stun_pkt_type_data_indication: {
        TSK_DEBUG_ERROR("Unexpected code called"); // --> DATA-INDICATION must be handled in net-event
        break;
    }
    default: {
        goto bail;
    }
    }

    *pb_processed = tsk_true;

bail:
    // unlock()
    tsk_safeobj_unlock(p_self);
    return ret;
}

// "pc_pkt" could be "200 response" or "CONNECTATTEMP-INDICATION"
int _tnet_turn_session_process_success_connect_pkt(struct tnet_turn_session_s* p_self, tnet_turn_peer_t* pc_peer, const tnet_turn_pkt_t *pc_pkt)
{
    int ret = 0;
    const tnet_stun_attr_vdata_t *pc_attr_connection_id;

    if (!p_self || !pc_peer || !pc_pkt) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (pc_pkt->e_type != tnet_stun_pkt_type_connectionattempt_indication && !TNET_STUN_PKT_RESP_IS_SUCCESS(pc_pkt)) {
        TSK_DEBUG_ERROR("Invalid packet type");
        ret = -2;
        goto bail;
    }

    // lock()
    tsk_safeobj_lock(p_self);

    if ((ret = tnet_stun_pkt_attr_find_first(pc_pkt, tnet_stun_attr_type_connection_id, (const tnet_stun_attr_t**)&pc_attr_connection_id)) == 0 && pc_attr_connection_id && pc_attr_connection_id->u_data_size == 4) {
        pc_peer->u_conn_id = TSK_TO_UINT32(pc_attr_connection_id->p_data_ptr);
        pc_peer->e_connect_state = tnet_stun_state_ok;
        _tnet_turn_session_raise_event_connect_ok(p_self, pc_peer->id);
        // Connect to the server and send "ConnectionBind" request using the "CONNECTION-ID" once connection is completed
        pc_peer->conn_fd = tnet_transport_connectto(p_self->p_transport, p_self->p_srv_host, p_self->u_srv_port, p_self->p_transport->type);
        if (pc_peer->conn_fd == TNET_INVALID_FD) {
            TSK_DEBUG_ERROR("Failed to connect to %s:%d", p_self->p_srv_host, p_self->u_srv_port);
            ret = -3;
            goto bail;
        }
    }

bail:
    tsk_safeobj_unlock(p_self);
    return ret;
}

static int _tnet_turn_session_transport_layer_dgram_cb(const tnet_transport_event_t* e)
{
    return _tnet_turn_session_transport_layer_process_cb(e);
}

static int _tnet_turn_session_transport_layer_stream_cb(const tnet_transport_event_t* e)
{
    return _tnet_turn_session_transport_layer_process_cb(e);
}

static int _tnet_turn_session_transport_layer_process_cb(const tnet_transport_event_t* e)
{
    tnet_turn_session_t* p_ss = (tnet_turn_session_t*)e->callback_data;
    int ret = 0;
    tnet_turn_pkt_t* p_pkt = tsk_null;
    tsk_buffer_t* p_stream_buff_in = tsk_null;
    const void* pc_data = tsk_null;
    tsk_size_t u_data_size = 0;
    tsk_bool_t b_stream = tsk_false, b_stream_appended = tsk_false, b_pkt_is_complete = tsk_false, b_got_msg= tsk_false;
    tnet_turn_peer_t* pc_peer = tsk_null;
    switch(e->type) {
    case event_data:
        break;
    case event_brokenpipe:
        tsk_safeobj_lock(p_ss);
        if (p_ss->p_lcl_sock && e->local_fd == p_ss->p_lcl_sock->fd) {
            tnet_fd_t broken_fd = e->local_fd;
            tsk_bool_t registered_fd = !!tnet_transport_have_socket(p_ss->p_transport, broken_fd);
            if (registered_fd) {
                tnet_transport_remove_socket(p_ss->p_transport, &broken_fd);
            }
            if (tnet_socket_handle_brokenpipe(p_ss->p_lcl_sock) == 0) {
                if (registered_fd) {
                    tnet_transport_add_socket(p_ss->p_transport, p_ss->p_lcl_sock->fd, p_ss->p_lcl_sock->type, tsk_false/* do not take ownership */, tsk_true/* only Meaningful for tls*/, tsk_null);
                }
            }
        }
        tsk_safeobj_unlock(p_ss);
        return 0;
    case event_connected:
        if (p_ss->p_lcl_sock && p_ss->p_lcl_sock->fd == e->local_fd) {
            tsk_safeobj_lock(p_ss);
            p_ss->b_stream_connected = tsk_true;
            if (p_ss->p_stream_buff_out && p_ss->p_stream_buff_out->size > 0) {
                TSK_DEBUG_INFO("Sending %u TURN pending bytes", (unsigned)p_ss->p_stream_buff_out->size);
                _tnet_turn_session_send_buff(p_ss, p_ss->p_stream_buff_out->data, (uint16_t)p_ss->p_stream_buff_out->size);
                TSK_OBJECT_SAFE_FREE(p_ss->p_stream_buff_out);
            }
            tsk_safeobj_unlock(p_ss);
        }
        else {
            tsk_safeobj_lock(p_ss);
            pc_peer = (tnet_turn_peer_t*)tsk_list_find_object_by_pred(p_ss->p_list_peers, __pred_find_peer_by_fd, &e->local_fd);
            if (pc_peer) {
                pc_peer->b_stream_connected = tsk_true;
                // Send ConnectionBind
                if ((ret = _tnet_turn_session_send_connbind(p_ss, pc_peer))) {
                    goto bail;
                }
            }
            tsk_safeobj_unlock(p_ss);
        }
        return 0;
    case event_error:
    case event_closed:
    case event_removed:
        /* case event_removed: */
        if (p_ss->p_lcl_sock && p_ss->p_lcl_sock->fd == e->local_fd) {
            tsk_safeobj_lock(p_ss);
            p_ss->b_stream_connected = tsk_false;
            p_ss->b_stream_error = (e->type == event_error);
            TSK_OBJECT_SAFE_FREE(p_ss->p_stream_buff_out);
            tsk_safeobj_unlock(p_ss);
        }
        else {
            tsk_safeobj_lock(p_ss);
            pc_peer = (tnet_turn_peer_t*)tsk_list_find_object_by_pred(p_ss->p_list_peers, __pred_find_peer_by_fd, &e->local_fd);
            if (pc_peer) {
                pc_peer->b_stream_connected = tsk_false;
                pc_peer->conn_fd = TNET_INVALID_FD;
            }
            tsk_safeobj_unlock(p_ss);
        }
        return 0;
    default:
        return 0;
    }

    /*** If the code reach this line it means we have to deal with a "NET-DATA-EVENT" ***/

    p_ss->cb.e.pc_enet = e;
    pc_data = e->data;
    u_data_size = e->size;
    b_stream = TNET_SOCKET_TYPE_IS_STREAM(p_ss->p_lcl_sock->type);
    b_stream_appended = tsk_false;

handle_data:
    b_got_msg = tsk_false;

    // TODO: /!\
    // - When the requested transport is TCP/TLS then we will receive RAW data on the connected peer socket which means no way to guess the end-of-the message
    // - No issue for when requested transport is UDP because we'll always have the data encapsulated in CHANNEL-DATA or DATA-INDICATION.
    if (b_stream) {
        if (!b_stream_appended) {
            tsk_safeobj_lock(p_ss);
            pc_peer = (tnet_turn_peer_t*)tsk_list_find_object_by_pred(p_ss->p_list_peers, __pred_find_peer_by_fd, &e->local_fd);
            if (pc_peer) {
                p_stream_buff_in = tsk_object_ref(pc_peer->p_stream_buff_in);
            }
            else {
                p_stream_buff_in = tsk_object_ref(p_ss->p_stream_buff_in);
            }
            tsk_safeobj_unlock(p_ss);

            if ((ret = tsk_buffer_append(p_stream_buff_in, e->data, e->size))) {
                goto bail;
            }
            b_stream_appended = tsk_true;
        }
        // Guard
        if (p_stream_buff_in->size > kTurnStreamChunckMaxSize) {
            TSK_DEBUG_ERROR("Too much data in the stream buffer: %u", (unsigned)p_stream_buff_in->size);
            tsk_buffer_cleanup(p_stream_buff_in);
            goto bail;
        }
        pc_data = p_stream_buff_in->data;
        u_data_size = p_stream_buff_in->size;
    }

    if (!TNET_STUN_BUFF_IS_STUN2(((const uint8_t*)pc_data), u_data_size)) {
        // ChannelData ?
        if (TNET_STUN_BUFF_IS_CHANNEL_DATA(((const uint8_t*)pc_data), u_data_size)) {
            const uint8_t* _p_data = (const uint8_t*)pc_data;
            // Channel Number in [0x4000 - 0x7FFF]
            // rfc5766 - 11.6.  Receiving a ChannelData Message
            // rfc5766 - 11.4.  The ChannelData Message
            // If  the message uses a value in the reserved range (0x8000 through 0xFFFF), then the message is silently discarded
            static const tsk_size_t kChannelDataHdrSize = 4; // Channel Number(2 bytes) + Length (2 bytes)
            uint16_t u_chan_num = tnet_ntohs_2(&_p_data[0]);
            if ((pc_peer = (tnet_turn_peer_t*)tsk_list_find_object_by_pred(p_ss->p_list_peers, __pred_find_peer_by_channum, &u_chan_num))) {
                uint16_t u_len = tnet_ntohs_2(&_p_data[2]);
                if (u_len <= (u_data_size - kChannelDataHdrSize)) {
                    b_got_msg = tsk_true;
                    _tnet_turn_session_raise_event_recv_data(p_ss, pc_peer->id, &_p_data[kChannelDataHdrSize], u_len);
                    if (p_stream_buff_in) {
                        /* The padding is not reflected in the length
                           field of the ChannelData message, so the actual size of a ChannelData
                           message (including padding) is (4 + Length) rounded up to the nearest
                           multiple of 4.  Over UDP, the padding is not required but MAY be
                           included.
                        */
                        tsk_size_t u_pad_len = (u_len & 3) ? (4 - (u_len & 3)) : 0;
                        tsk_buffer_remove(p_stream_buff_in, 0, kChannelDataHdrSize + u_len + u_pad_len);
                    }
                }
                else {
                    TSK_DEBUG_INFO("TURN channel data too short");
                }
            }
            goto bail;
        } // if (TNET_STUN_BUFF_IS_CHANNEL_DATA...

        // Must never happen: Data must be received via CHANNEL-DATA or DATA-INDICATION
        TSK_DEBUG_INFO("Calling unexpected code :(");

        if (b_stream) {
            tsk_safeobj_lock(p_ss);
            pc_peer = (tnet_turn_peer_t*)tsk_list_find_object_by_pred(p_ss->p_list_peers, __pred_find_peer_by_fd, &e->local_fd);
            tsk_safeobj_unlock(p_ss);
        }
        _tnet_turn_session_raise_event_recv_data(p_ss, pc_peer ? pc_peer->id : kTurnPeerIdInvalid, pc_data, u_data_size);
        if (p_stream_buff_in) {
            tsk_buffer_cleanup(p_stream_buff_in);
        }
        goto bail;
    } // if (!TNET_STUN_BUFF_IS_STUN2...

    /*** At this step it means we have a STUN packet in the buffer ***/

    // Make sure we have a complete packet in the buffer
    if (b_stream) {
        if ((ret = tnet_stun_pkt_is_complete(pc_data, u_data_size, &b_pkt_is_complete))) {
            goto bail;
        }
        if (!b_pkt_is_complete) {
            TSK_DEBUG_INFO("The stream length is too short to contains a complete STUN/TURN packet");
            goto bail;
        }
    }

    // Parse the packet
    if ((ret = tnet_stun_pkt_read(pc_data, u_data_size, &p_pkt))) {
        goto bail;
    }

    // Remove the parsed data
    if (p_stream_buff_in && p_pkt) {
        tsk_size_t u_pad_len = (p_pkt->u_length & 3) ? (4 - (p_pkt->u_length & 3)) : 0;
        tsk_buffer_remove(p_stream_buff_in, 0, kStunPktHdrSizeInOctets + p_pkt->u_length + u_pad_len);
    }

    if (p_pkt) {
        b_got_msg = tsk_true;
        if (p_pkt->e_type == tnet_stun_pkt_type_data_indication) {
            const tnet_stun_attr_vdata_t *pc_attr_data;
            const tnet_stun_attr_address_t *pc_attr_xpeer;
            const tnet_turn_peer_t *pc_peer;
            // XOR-PEER-ADDRESS (required)
            if ((ret = tnet_stun_pkt_attr_find_first(p_pkt, tnet_stun_attr_type_xor_peer_address, (const tnet_stun_attr_t**)&pc_attr_xpeer)) || !pc_attr_xpeer) {
                TSK_DEBUG_ERROR("XOR-PEER-ADDRESS missing in DATA-INDICATION");
                ret = -4;
                goto bail;
            }
            tsk_safeobj_lock(p_ss); // lock to make sure the list will not be modified
            if ((ret = _tnet_turn_peer_find_by_xpeer(p_ss->p_list_peers, pc_attr_xpeer, &pc_peer))) {
                tsk_safeobj_unlock(p_ss);
                goto bail;
            }
            tsk_safeobj_unlock(p_ss); // unlock()
            // DATA (required)
            if ((ret = tnet_stun_pkt_attr_find_first(p_pkt, tnet_stun_attr_type_data, (const tnet_stun_attr_t**)&pc_attr_data)) || !pc_attr_data) {
                TSK_DEBUG_ERROR("DATA missing in DATA-INDICAION");
                ret = -4;
                goto bail;
            }

            _tnet_turn_session_raise_event_recv_data(p_ss, pc_peer ? pc_peer->id : kTurnPeerIdInvalid, pc_attr_data->p_data_ptr, pc_attr_data->u_data_size);
        }
        else {
            tsk_bool_t b_processed;
            if ((ret = _tnet_turn_session_process_incoming_pkt(p_ss, p_pkt, &b_processed))) {
                goto bail;
            }
            if (!b_processed) {
                const tnet_turn_peer_t *pc_peer;
                tsk_safeobj_lock(p_ss);
                pc_peer = (const tnet_turn_peer_t*)tsk_list_find_object_by_pred(p_ss->p_list_peers, __pred_find_peer_by_fd, &e->local_fd);
                tsk_safeobj_unlock(p_ss); // unlock()
                // Forward to listeners (e.g. ICE context)
                _tnet_turn_session_raise_event_recv_data(p_ss, pc_peer ? pc_peer->id : kTurnPeerIdInvalid, pc_data, u_data_size);
            }
        }
    }

bail:
    // Handle next message in the stream buffer
    if (ret == 0 && b_got_msg && b_stream && p_stream_buff_in && p_stream_buff_in->size > 0) {
        goto handle_data;
    }

    TSK_OBJECT_SAFE_FREE(p_pkt);
    TSK_OBJECT_SAFE_FREE(p_stream_buff_in);
    p_ss->cb.e.pc_enet = tsk_null;
    return ret;
}

static int _tnet_turn_session_timer_callback(const void* pc_arg, tsk_timer_id_t timer_id)
{
#undef _tnet_turn_session_raise_event_alloc_ok
#define _tnet_turn_session_raise_event_alloc_ok(_self, _u_peer_id) _tnet_turn_session_raise_event0((_self), tnet_turn_session_event_type_alloc_ok, kTurnPeerIdInvalid)
#undef _tnet_turn_session_raise_event_alloc_nok
#define _tnet_turn_session_raise_event_alloc_nok(_self, _u_peer_id) _tnet_turn_session_raise_event0((_self), tnet_turn_session_event_type_alloc_nok, kTurnPeerIdInvalid)
#undef _tnet_turn_session_raise_event_refresh_ok
#define _tnet_turn_session_raise_event_refresh_ok(_self, _u_peer_id) _tnet_turn_session_raise_event0((_self), tnet_turn_session_event_type_refresh_ok, kTurnPeerIdInvalid)
#undef _tnet_turn_session_raise_event_refresh_nok
#define _tnet_turn_session_raise_event_refresh_nok(_self, _u_peer_id) _tnet_turn_session_raise_event0((_self), tnet_turn_session_event_type_refresh_nok, kTurnPeerIdInvalid)
#define RETRANSMIT(parent, which, u_peer_id) \
	parent->timer.rtt.which.u_timeout <<= 1; \
	if (parent->timer.rtt.which.u_timeout <= kStunUdpRetransmitTimoutMaxInMs) { \
		if ((ret = _tnet_turn_session_send_pkt(p_ss, parent->p_pkt_##which))) { \
			parent->e_##which##_state = tnet_stun_state_nok; \
			_tnet_turn_session_raise_event_##which##_nok(p_ss, u_peer_id); \
			goto bail; \
		} \
		parent->timer.rtt.which.id = tsk_timer_manager_schedule(p_ss->timer.p_mgr, parent->timer.rtt.which.u_timeout, _tnet_turn_session_timer_callback, p_ss); \
	} else { \
		_tnet_turn_session_raise_event_##which##_nok(p_ss, u_peer_id); \
	}

    tnet_turn_session_t* p_ss = (tnet_turn_session_t*)pc_arg;
    tnet_turn_peer_t* pc_peer;
    int ret = 0;
    tsk_safeobj_lock(p_ss); // must
    if (p_ss->timer.rtt.alloc.id == timer_id) {
        RETRANSMIT(p_ss, alloc, kTurnPeerIdInvalid);
    }
    else if ((pc_peer = (tnet_turn_peer_t*)tsk_list_find_object_by_pred(p_ss->p_list_peers, __pred_find_peer_by_timer_rtt_chanbind, &timer_id))) {
        RETRANSMIT(pc_peer, chanbind, pc_peer->id);
    }
    else if ((pc_peer = (tnet_turn_peer_t*)tsk_list_find_object_by_pred(p_ss->p_list_peers, __pred_find_peer_by_timer_rtt_createperm, &timer_id))) {
        RETRANSMIT(pc_peer, createperm, pc_peer->id);
    }
    else if (p_ss->timer.rtt.refresh.id == timer_id) {
        RETRANSMIT(p_ss, refresh, kTurnPeerIdInvalid);
    }
    else if (p_ss->timer.u_timer_id_refresh == timer_id) {
        if ((ret = _tnet_turn_session_send_refresh(p_ss))) {
            goto bail;
        }
    }
    else if ((pc_peer = (tnet_turn_peer_t*)tsk_list_find_object_by_pred(p_ss->p_list_peers, __pred_find_peer_by_timer_fresh_chanbind, &timer_id))) {
        if ((ret = tnet_turn_session_chanbind(p_ss, pc_peer->id))) {
            goto bail;
        }
    }
    else if ((pc_peer = (tnet_turn_peer_t*)tsk_list_find_object_by_pred(p_ss->p_list_peers, __pred_find_peer_by_timer_fresh_createperm, &timer_id))) {
        if ((ret = _tnet_turn_session_send_permission(p_ss, pc_peer))) {
            goto bail;
        }
    }

bail:
    tsk_safeobj_unlock(p_ss);
    return ret;
}

static int _tnet_turn_peer_create(const char* pc_peer_ip, uint16_t u_peer_port, tsk_bool_t b_ipv6, struct tnet_turn_peer_s **pp_peer)
{
    extern const tsk_object_def_t *tnet_turn_peer_def_t;
    static tnet_turn_peer_id_t __l_peer_id = 0;
    tnet_stun_addr_t peer_addr;
    int ret;
    if (!pc_peer_ip || !u_peer_port || !pp_peer) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if ((ret = tnet_stun_utils_inet_pton(b_ipv6, pc_peer_ip, &peer_addr))) {
        TSK_DEBUG_ERROR("inet_pton(%s,IPv6=%d) failed", pc_peer_ip, b_ipv6);
        goto bail;
    }
    if (!(*pp_peer = tsk_object_new(tnet_turn_peer_def_t))) {
        TSK_DEBUG_ERROR("Failed to create TURN peer object");
        return -2;
    }
    (*pp_peer)->p_addr_ip = tsk_strdup(pc_peer_ip);
    memcpy((*pp_peer)->addr_ip, peer_addr, sizeof(peer_addr));
    (*pp_peer)->u_addr_port = u_peer_port;
    (*pp_peer)->b_ipv6 = b_ipv6;
    tsk_atomic_inc(&__l_peer_id);
    (*pp_peer)->id = __l_peer_id;
    (*pp_peer)->timer.rtt.chanbind.id = TSK_INVALID_TIMER_ID;
    (*pp_peer)->timer.rtt.createperm.id = TSK_INVALID_TIMER_ID;
    (*pp_peer)->timer.rtt.chanbind.u_timeout = kStunUdpRetransmitTimoutMinInMs;
    (*pp_peer)->timer.rtt.createperm.u_timeout = kStunUdpRetransmitTimoutMinInMs;
    (*pp_peer)->timer.fresh.chanbind.id = TSK_INVALID_TIMER_ID;
    (*pp_peer)->timer.fresh.createperm.id = TSK_INVALID_TIMER_ID;
    (*pp_peer)->timer.fresh.chanbind.u_timeout = kTurnChannelBindingTimeOutInSec * 1000;
    (*pp_peer)->timer.fresh.createperm.u_timeout = kTurnPermissionTimeOutInSec * 1000;
    (*pp_peer)->e_chanbind_state = tnet_stun_state_none;
    (*pp_peer)->e_createperm_state = tnet_stun_state_none;

bail:
    return ret;
}

static int _tnet_turn_peer_find_by_xpeer(const tnet_turn_peers_L_t* pc_peers, const tnet_stun_attr_address_t* pc_xpeer, const tnet_turn_peer_t **ppc_peer)
{
    const tsk_list_item_t *pc_item;
    const tnet_turn_peer_t *pc_peer;
    if (!pc_peers || !pc_xpeer || !ppc_peer) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    *ppc_peer = tsk_null;
    tsk_list_foreach(pc_item, pc_peers) {
        pc_peer = (const tnet_turn_peer_t *)pc_item->data;
        if (pc_peer->u_addr_port == pc_xpeer->u_port) {
            if (tnet_stun_utils_buff_cmp(
                        pc_peer->addr_ip, pc_peer->b_ipv6 ? 16 : 4,
                        pc_xpeer->address, (pc_xpeer->e_family == tnet_stun_address_family_ipv6) ? 16 : 4) == 0) {
                *ppc_peer = pc_peer;
                return 0;
            }
        }
    }
    return 0;
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
        TSK_OBJECT_SAFE_FREE(p_ss->p_pkt_refresh);
        TSK_OBJECT_SAFE_FREE(p_ss->p_lcl_sock);

        TSK_FREE(p_ss->p_buff_chandata_ptr);
        TSK_FREE(p_ss->p_buff_send_ptr);
        TSK_FREE(p_ss->p_rel_ip);
        TSK_FREE(p_ss->p_srflx_ip);
        TSK_FREE(p_ss->p_srv_host);

        if (p_ss->p_transport) {
            tnet_transport_shutdown(p_ss->p_transport);
            TSK_OBJECT_SAFE_FREE(p_ss->p_transport);
        }

        TSK_OBJECT_SAFE_FREE(p_ss->p_list_peers);
        TSK_OBJECT_SAFE_FREE(p_ss->p_stream_buff_in);
        TSK_OBJECT_SAFE_FREE(p_ss->p_stream_buff_out);

        TSK_OBJECT_SAFE_FREE(p_ss->proxy.info);

        TSK_FREE(p_ss->ssl.path_priv);
        TSK_FREE(p_ss->ssl.path_pub);
        TSK_FREE(p_ss->ssl.path_ca);

        tsk_safeobj_deinit(p_ss);
        TSK_DEBUG_INFO("*** TURN Session destroyed ***");
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


static tsk_object_t* tnet_turn_peer_ctor(tsk_object_t * self, va_list * app)
{
    tnet_turn_peer_t *p_peer = (tnet_turn_peer_t *)self;
    if (p_peer) {

    }
    return self;
}
static tsk_object_t* tnet_turn_peer_dtor(tsk_object_t * self)
{
    tnet_turn_peer_t *p_peer = (tnet_turn_peer_t *)self;
    if (p_peer) {
        TSK_FREE(p_peer->p_addr_ip);
        TSK_OBJECT_SAFE_FREE(p_peer->p_pkt_chanbind);
        TSK_OBJECT_SAFE_FREE(p_peer->p_pkt_createperm);
        TSK_OBJECT_SAFE_FREE(p_peer->p_pkt_sendind);
        TSK_OBJECT_SAFE_FREE(p_peer->p_pkt_connect);
        TSK_OBJECT_SAFE_FREE(p_peer->p_pkt_connbind);
        TSK_OBJECT_SAFE_FREE(p_peer->p_stream_buff_in);
        TSK_OBJECT_SAFE_FREE(p_peer->p_stream_buff_out);
#if PRINT_DESTROYED_MSG || 1
        TSK_DEBUG_INFO("*** TURN peer destroyed ***");
#endif
    }
    return self;
}
static const tsk_object_def_t tnet_turn_peer_def_s = {
    sizeof(tnet_turn_peer_t),
    tnet_turn_peer_ctor,
    tnet_turn_peer_dtor,
    tsk_null,
};
const tsk_object_def_t *tnet_turn_peer_def_t = &tnet_turn_peer_def_s;
