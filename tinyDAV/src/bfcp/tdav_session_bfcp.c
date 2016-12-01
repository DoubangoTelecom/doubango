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
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tdav_session_bfcp.c
 * @brief The The Binary Floor Control Protocol (BFCP, rfc4582) session.
 */
#include "tinydav/bfcp/tdav_session_bfcp.h"

#if !defined(HAVE_TINYBFCP) || HAVE_TINYBFCP

#include "tinybfcp/tbfcp_session.h"
#include "tinybfcp/tbfcp_pkt.h"
#include "tinybfcp/tbfcp_utils.h"

#include "tsk_string.h"
#include "tsk_memory.h" /* TSK_FREE */
#include "tsk_debug.h"

/*
	* https://tools.ietf.org/html/rfc4574
	* https://tools.ietf.org/html/rfc4582
	* https://tools.ietf.org/html/rfc4583
	* http://tools.ietf.org/html/rfc4796
	* https://tools.ietf.org/html/draft-ietf-bfcpbis-rfc4582bis-1
*/

typedef struct tdav_session_bfcp_s {
    TMEDIA_DECLARE_SESSION_BFCP;

    struct tbfcp_session_s* p_bfcp_s;
    struct tbfcp_pkt_s* p_pkt_FloorRequest;
    struct tbfcp_pkt_s* p_pkt_FloorRelease;
    struct tbfcp_pkt_s* p_pkt_Hello;

    tsk_bool_t b_started;
    tsk_bool_t b_use_ipv6;
    tsk_bool_t b_revoked_handled;
    tsk_bool_t b_conf_idf_changed;
    tsk_bool_t b_stop_to_reconf;

    char* p_local_ip;
    //uint16_t local_port;

    /* NAT Traversal context */
    struct tnet_nat_ctx_s* p_natt_ctx;

    char* p_remote_ip;
    uint16_t u_remote_port;

    // https://tools.ietf.org/html/rfc4583 attributes
    struct {
        char* confid;
        char* floorid;
        char* mstrm;
        char* userid;
    } rfc4583;
}
tdav_session_bfcp_t;

static int _tdav_session_bfcp_notif(const struct tbfcp_session_event_xs *e);
static int _tdav_session_bfcp_send_Hello(tdav_session_bfcp_t* p_bfcp);

/* ============ Plugin interface ================= */

static int _tdav_session_bfcp_set(tmedia_session_t* p_self, const tmedia_param_t* param)
{
    int ret = 0;
    tdav_session_bfcp_t* p_bfcp;

    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    TSK_DEBUG_INFO("tdav_session_bfcp_set");

    p_bfcp = (tdav_session_bfcp_t*)p_self;

    if (param->value_type == tmedia_pvt_pchar) {
        if (tsk_striequals(param->key, "remote-ip")) {
            // only if no ip associated to the "m=" line
            if (param->value && !p_bfcp->p_remote_ip) {
                p_bfcp->p_remote_ip = tsk_strdup(param->value);
            }
        }
        else if (tsk_striequals(param->key, "local-ip")) {
            tsk_strupdate(&p_bfcp->p_local_ip, param->value);
        }
        else if (tsk_striequals(param->key, "use-ipv6")) {
            p_bfcp->b_use_ipv6 = tsk_striequals(param->value, "ipv6");
        }
    }
    else if (param->value_type == tmedia_pvt_pobject) {
        if (tsk_striequals(param->key, "natt-ctx")) {
            TSK_OBJECT_SAFE_FREE(p_bfcp->p_natt_ctx);
            p_bfcp->p_natt_ctx = tsk_object_ref(param->value);
        }
    }
    else if (param->value_type == tmedia_pvt_int32) {
        if (tsk_striequals(param->key, "stop-to-reconf")) {
            p_bfcp->b_stop_to_reconf = TSK_TO_INT32((uint8_t*)param->value) ? tsk_true : tsk_false;
        }
    }


    return ret;
}

static int _tdav_session_bfcp_get(tmedia_session_t* p_self, tmedia_param_t* param)
{
    int ret = 0;
    tdav_session_bfcp_t* p_bfcp;

    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    TSK_DEBUG_INFO("_tdav_session_bfcp_get");

    p_bfcp = (tdav_session_bfcp_t*)p_self;

    return ret;
}

static int _tdav_session_bfcp_prepare(tmedia_session_t* p_self)
{
    int ret = 0;
    tdav_session_bfcp_t* p_bfcp;

    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    TSK_DEBUG_INFO("_tdav_session_bfcp_prepare");

    p_bfcp = (tdav_session_bfcp_t*)p_self;

    if (!p_bfcp->p_bfcp_s) {
        enum tnet_socket_type_e e_socket_type = kBfcpTransportDefault;
        if ((ret = tbfcp_session_create(e_socket_type, p_bfcp->p_local_ip, &p_bfcp->p_bfcp_s))) {
            return ret;
        }
    }
    if ((ret = tbfcp_session_set_natt_ctx(p_bfcp->p_bfcp_s, p_bfcp->p_natt_ctx))) {
        return ret;
    }
    if ((ret = tbfcp_session_prepare(p_bfcp->p_bfcp_s))) {
        return ret;
    }
    if ((ret = tbfcp_session_set_callback(p_bfcp->p_bfcp_s, _tdav_session_bfcp_notif, p_bfcp))) {
        return ret;
    }
    TSK_OBJECT_SAFE_FREE(p_bfcp->p_pkt_Hello);
    TSK_OBJECT_SAFE_FREE(p_bfcp->p_pkt_FloorRequest);
    TSK_OBJECT_SAFE_FREE(p_bfcp->p_pkt_FloorRelease);
    p_bfcp->b_revoked_handled = tsk_false;

    return ret;
}

static int _tdav_session_bfcp_start(tmedia_session_t* p_self)
{
    int ret = 0;
    tdav_session_bfcp_t* p_bfcp;

    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    TSK_DEBUG_INFO("_tdav_session_bfcp_start");

    p_bfcp = (tdav_session_bfcp_t*)p_self;

    if ((ret = tbfcp_session_set_remote_address(p_bfcp->p_bfcp_s, p_bfcp->p_remote_ip, p_bfcp->u_remote_port))) {
        return ret;
    }
    if ((ret = tbfcp_session_start(p_bfcp->p_bfcp_s))) {
        return ret;
    }
    if ((ret = _tdav_session_bfcp_send_Hello(p_bfcp))) {
        return ret;
    }

    p_bfcp->b_started = tsk_true;

    return ret;
}

static int _tdav_session_bfcp_pause(tmedia_session_t* p_self)
{
    int ret = 0;
    tdav_session_bfcp_t* p_bfcp;

    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    TSK_DEBUG_INFO("_tdav_session_bfcp_pause");

    p_bfcp = (tdav_session_bfcp_t*)p_self;

    if (p_bfcp->p_bfcp_s && (ret = tbfcp_session_pause(p_bfcp->p_bfcp_s))) {
        return ret;
    }

    return ret;
}

static int _tdav_session_bfcp_stop(tmedia_session_t* p_self)
{
    int ret = 0;
    tdav_session_bfcp_t* p_bfcp;

    if (!p_self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    TSK_DEBUG_INFO("_tdav_session_bfcp_stop");

    p_bfcp = (tdav_session_bfcp_t*)p_self;

    if (!p_bfcp->b_stop_to_reconf) { // If stop-to-reconf then do not release the FloorRequest but reuse it
        if (p_bfcp->b_started) {
            /*if (p_bfcp->p_bfcp_s)*/ {
                /*if (!p_bfcp->p_pkt_FloorRelease) {
                	ret = tbfcp_session_create_pkt_FloorRelease(p_bfcp->p_bfcp_s, &p_bfcp->p_pkt_FloorRelease);
                }
                if (ret == 0 && p_bfcp->p_pkt_FloorRelease && (ret = tbfcp_session_send_pkt(p_bfcp->p_bfcp_s, p_bfcp->p_pkt_FloorRelease))) {
                	//!\ do not exit
                }*/
            }
        }
        tsk_strupdate(&p_bfcp->rfc4583.confid, "");
    }

    if (p_bfcp->p_bfcp_s) {
        ret = tbfcp_session_stop(p_bfcp->p_bfcp_s);
    }

    p_bfcp->b_started = tsk_false;
    p_bfcp->b_stop_to_reconf = tsk_false; // reset

    return ret;
}

static const tsdp_header_M_t* _tdav_session_bfcp_get_lo(tmedia_session_t* p_self)
{
    tdav_session_bfcp_t* p_bfcp;
    tsk_bool_t b_changed = tsk_false;
    const char *pc_local_ip, *pc_local_profile, *pc_local_role, *pc_local_setup;
    tnet_port_t u_local_port;
    enum tbfcp_role_e e_local_role;
    enum tbfcp_setup_e e_local_setup;
    int ret;

    TSK_DEBUG_INFO("_tdav_session_bfcp_get_lo");

    if (!p_self || !p_self->plugin) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }

    p_bfcp = (tdav_session_bfcp_t*)p_self;

    b_changed = (p_self->ro_changed || !p_self->M.lo);

    if (!b_changed) {
        TSK_DEBUG_INFO("No changes to the BFCP session...skip SDP update");
        return p_self->M.lo;
    }

    if (b_changed && p_self->M.lo) {
        static const char* __fields[] = { "floorctrl", "setup", "connection", "curr", "des", "conf" };
        // remove media-level attributes
        tsdp_header_A_removeAll_by_fields(p_self->M.lo->Attributes, __fields, sizeof(__fields)/sizeof(__fields[0]));
        // Codec list never change and FMTs always a single star (*) value. Radvision TelePresence System reject a BFCP session whithout the single FMT (*)
        // The Codecs and formats are never rebuilt which means we must not clear them
#if 0
        tsk_list_clear_items(p_self->M.lo->FMTs);
#endif
    }

    // get local address
    if ((ret = tbfcp_session_get_local_address(p_bfcp->p_bfcp_s, &pc_local_ip, &u_local_port))) {
        TSK_DEBUG_ERROR("Failed to get local address from BFCP session");
        return tsk_null;
    }
    // get local profile
    if ((ret = tbfcp_session_get_profile(p_bfcp->p_bfcp_s, &pc_local_profile))) {
        TSK_DEBUG_ERROR("Failed to get local profile from BFCP session");
        return tsk_null;
    }
    // get local role
    if ((ret = tbfcp_session_get_local_role(p_bfcp->p_bfcp_s, &e_local_role))) {
        TSK_DEBUG_ERROR("Failed to get local role from BFCP session");
        return tsk_null;
    }
    if ((ret = tbfcp_utils_get_role(e_local_role, &pc_local_role))) {
        return tsk_null;
    }
    // get local setup
    if ((ret = tbfcp_session_get_local_setup(p_bfcp->p_bfcp_s, &e_local_setup))) {
        TSK_DEBUG_ERROR("Failed to get local setup from BFCP session");
        return tsk_null;
    }
    if ((ret = tbfcp_utils_get_setup(e_local_role, &pc_local_setup))) {
        return tsk_null;
    }

    if (!p_self->M.lo) {
        if (!(p_self->M.lo = tsdp_header_M_create(p_self->plugin->media, u_local_port, pc_local_profile))) {
            TSK_DEBUG_ERROR("Failed to create BFCP SDP media header");
            return tsk_null;
        }
        tsdp_header_M_add_headers(p_self->M.lo,
                                  TSDP_FMT_VA_ARGS("*"),
                                  TSDP_HEADER_A_VA_ARGS("connection", "new"),
                                  TSDP_HEADER_A_VA_ARGS("lib", "tinyBFCP"),
                                  tsk_null);
        // If NATT is active, do not rely on the global IP address Connection line
        if (p_bfcp->p_natt_ctx)  {
            tsdp_header_M_add_headers(p_self->M.lo,
                                      TSDP_HEADER_C_VA_ARGS("IN", p_bfcp->b_use_ipv6 ? "IP6" : "IP4", pc_local_ip),
                                      tsk_null);
        }
    }
    else {
        p_self->M.lo->port = u_local_port;
        tsk_strupdate(&p_self->M.lo->proto, pc_local_profile);
    }

    // add "floorctrl" and "setup" attributes
    tsdp_header_M_add_headers(p_self->M.lo,
                              TSDP_HEADER_A_VA_ARGS("connection", "new"),
                              TSDP_HEADER_A_VA_ARGS("floorctrl", pc_local_role),
                              TSDP_HEADER_A_VA_ARGS("setup", pc_local_setup),
                              tsk_null);

    return p_self->M.lo;
}

static int _tdav_session_bfcp_set_ro(tmedia_session_t* p_self, const tsdp_header_M_t* m)
{
    int ret = 0;
    const tsdp_header_A_t* A;
    tdav_session_bfcp_t* p_bfcp;
    enum tbfcp_role_e e_remote_role = tbfcp_role_c_s;
    uint32_t u_remote_conf_id = 0xFFFF;
    uint16_t u_remote_user_id = 0xFFFF, u_remote_floor_id = 0xFFFF;

    if (!p_self || !m) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    TSK_DEBUG_INFO("_tdav_session_bfcp_set_ro");

    p_bfcp = (tdav_session_bfcp_t*)p_self;

    /* update remote offer */
    TSK_OBJECT_SAFE_FREE(p_self->M.ro);
    p_self->M.ro = tsk_object_ref(TSK_OBJECT(m));


    // https://tools.ietf.org/html/rfc4583
    {
        p_bfcp->b_conf_idf_changed = tsk_false;
        if ((A = tsdp_header_M_findA(m, "floorctrl"))) {
            if ((ret = tbfcp_utils_parse_role(A->value, &e_remote_role))) {
                return ret;
            }
        }
        if ((A = tsdp_header_M_findA(m, "confid"))) {
            p_bfcp->b_conf_idf_changed |= !tsk_striequals(p_bfcp->rfc4583.confid, A->value);
            tsk_strupdate(&p_bfcp->rfc4583.confid, A->value);
            u_remote_conf_id = (uint32_t)tsk_atoi64(p_bfcp->rfc4583.confid);
        }
        if ((A = tsdp_header_M_findA(m, "userid"))) {
            p_bfcp->b_conf_idf_changed |= !tsk_striequals(p_bfcp->rfc4583.userid, A->value);
            tsk_strupdate(&p_bfcp->rfc4583.userid, A->value);
            u_remote_user_id = (uint16_t)tsk_atoi64(p_bfcp->rfc4583.userid);
        }
        if ((A = tsdp_header_M_findA(m, "floorid"))) {
            char tmp_str[256];
            if (sscanf(A->value, "%255s %*s", tmp_str) != EOF) {
                char *pch, *saveptr = NULL;
                p_bfcp->b_conf_idf_changed |= !tsk_striequals(p_bfcp->rfc4583.floorid, tmp_str);
                tsk_strupdate(&p_bfcp->rfc4583.floorid, tmp_str);
                u_remote_floor_id = (uint16_t)tsk_atoi64(p_bfcp->rfc4583.floorid);
                pch = tsk_strtok_r(&A->value[tsk_strlen(tmp_str) + 1], " ", &saveptr);
                while (pch) {
                    if (sscanf(pch, "mstrm: %255s", tmp_str) != EOF) {
                        p_bfcp->b_conf_idf_changed |= !tsk_striequals(p_bfcp->rfc4583.mstrm, tmp_str);
                        tsk_strupdate(&p_bfcp->rfc4583.mstrm, tmp_str);
                        break;
                    }
                    pch = tsk_strtok_r(tsk_null, " ", &saveptr);
                }
            }
        }
        // set remote role
        if ((ret = tbfcp_session_set_remote_role(p_bfcp->p_bfcp_s, e_remote_role))) {
            return ret;
        }
        if ((e_remote_role & tbfcp_role_s_only)) {
            // local = client
            if ((ret = tbfcp_session_set_conf_ids(p_bfcp->p_bfcp_s, u_remote_conf_id, u_remote_user_id, u_remote_floor_id))) {
                return ret;
            }
        }
        else {
            // local = remote: Not supported yet and will never happen
        }
    }//end-of-rfc4583

    /* get connection associated to this media line
    * If the connnection is global, then the manager will call tdav_session_audio_set() */
    if (m->C && m->C->addr) {
        tsk_strupdate(&p_bfcp->p_remote_ip, m->C->addr);
        p_bfcp->b_use_ipv6 = tsk_striequals(m->C->addrtype, "IP6");
    }
    /* set remote port */
    p_bfcp->u_remote_port = m->port;

    return ret;
}

static int _tdav_session_bfcp_send_Hello(tdav_session_bfcp_t* p_bfcp)
{
    int ret = 0;
    if (!p_bfcp) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if (!p_bfcp->p_pkt_Hello && (ret = tbfcp_session_create_pkt_Hello(p_bfcp->p_bfcp_s, &p_bfcp->p_pkt_Hello))) {
        return ret;
    }
    if ((ret = tbfcp_session_send_pkt(p_bfcp->p_bfcp_s, p_bfcp->p_pkt_Hello))) {
        return ret;
    }
    return ret;
}

static int _tdav_session_bfcp_notif(const struct tbfcp_session_event_xs *e)
{
    tdav_session_bfcp_t* p_bfcp = tsk_object_ref(TSK_OBJECT(e->pc_usr_data));
    int ret = 0;
    static const char* kErrTextGlobalError = "Global error";
    static const int kErrCodeGlobalError = -56;
    static const char* kErrTextTimeout = "Timeout";
    static const int kErrCodeTimeout = -57;
    static const char* kErrTextUnExpectedIncomingMsg = "Unexpected incoming BFCP message";
    static const int kErrCodeUnExpectedIncomingMsg = -58;
    static const char* kErrTextBadRequest = "Bad Request";
    static const int kErrCodeBadRequest = -59;
    static const char* kInfoTextFloorReqStatus = "FloorRequestStatus";

#define _RAISE_ERR_AND_GOTO_BAIL(_code, _reason) \
		if (TMEDIA_SESSION(p_bfcp)->bfcp_cb.fun) { \
			tmedia_session_bfcp_evt_xt e; \
			e.type = tmedia_session_bfcp_evt_type_err; e.err.code = _code; e.reason = _reason; \
			TMEDIA_SESSION(p_bfcp)->bfcp_cb.fun(TMEDIA_SESSION(p_bfcp)->bfcp_cb.usrdata, TMEDIA_SESSION(p_bfcp), &e); \
		} \
		ret = _code; goto bail;
#define _RAISE_FLREQ(_status, _reason) \
		if (TMEDIA_SESSION(p_bfcp)->bfcp_cb.fun) { \
			tmedia_session_bfcp_evt_xt e; \
			e.type = tmedia_session_bfcp_evt_type_flreq_status; e.flreq.status = _status; e.reason = _reason; \
			TMEDIA_SESSION(p_bfcp)->bfcp_cb.fun(TMEDIA_SESSION(p_bfcp)->bfcp_cb.usrdata, TMEDIA_SESSION(p_bfcp), &e); \
		} \
 
    switch (e->e_type) {
    case tbfcp_session_event_type_inf_inc_msg: {
        if (p_bfcp->p_pkt_Hello && p_bfcp->p_pkt_Hello->hdr.transac_id == e->pc_pkt->hdr.transac_id && p_bfcp->p_pkt_Hello->hdr.user_id == e->pc_pkt->hdr.user_id && p_bfcp->p_pkt_Hello->hdr.conf_id == e->pc_pkt->hdr.conf_id) {
            TSK_OBJECT_SAFE_FREE(p_bfcp->p_pkt_Hello);
            if (e->pc_pkt->hdr.primitive == tbfcp_primitive_HelloAck) {
                if (!p_bfcp->p_pkt_FloorRequest) {
                    if (p_bfcp->b_conf_idf_changed || 0) {
                        // Create the "FloorRelease" for this "FloorRequest"
                        TSK_OBJECT_SAFE_FREE(p_bfcp->p_pkt_FloorRelease);
                        if ((ret = tbfcp_session_create_pkt_FloorRelease(p_bfcp->p_bfcp_s, &p_bfcp->p_pkt_FloorRelease))) {
                            goto raise_err;
                        }
                        if ((ret = tbfcp_session_create_pkt_FloorRequest(p_bfcp->p_bfcp_s, &p_bfcp->p_pkt_FloorRequest))) {
                            goto raise_err;
                        }
                        if ((ret = tbfcp_session_send_pkt(p_bfcp->p_bfcp_s, p_bfcp->p_pkt_FloorRequest))) {
                            goto raise_err;
                        }
                    }
                    else {
                        TSK_DEBUG_INFO("No change to BFCP session... do not send FloorRequest");
                    }
                }
            }
            else {
                TSK_DEBUG_ERROR("%s", kErrTextUnExpectedIncomingMsg);
                _RAISE_ERR_AND_GOTO_BAIL(kErrCodeUnExpectedIncomingMsg, kErrTextUnExpectedIncomingMsg);
            }
        }
        else if(p_bfcp->p_pkt_FloorRequest /*&& p_bfcp->p_pkt_FloorRequest->hdr.transac_id == e->pc_pkt->hdr.transac_id*/ && p_bfcp->p_pkt_FloorRequest->hdr.user_id == e->pc_pkt->hdr.user_id && p_bfcp->p_pkt_FloorRequest->hdr.conf_id == e->pc_pkt->hdr.conf_id) {
            tsk_bool_t transac_id_matched = (p_bfcp->p_pkt_FloorRequest->hdr.transac_id == e->pc_pkt->hdr.transac_id);
            if (e->pc_pkt->hdr.primitive == tbfcp_primitive_FloorRequestStatus || e->pc_pkt->hdr.primitive == tbfcp_primitive_FloorStatus) {
                tsk_size_t u_index0, u_index1, u_index2, u_index3;
                const tbfcp_attr_grouped_t *pc_attr_FloorRequestInformation = tsk_null,
                                            *pc_attr_FloorRequestStatus = tsk_null,
                                             *pc_attr_OverallRequestStatus = tsk_null;
                const tbfcp_attr_octetstring16_t *pc_attr_RequestStatus = tsk_null;

                u_index0 = 0;
                // Find "FloorRequestInformation"
                while ((ret = tbfcp_pkt_attr_find_at(e->pc_pkt, tbfcp_attribute_format_Grouped, u_index0++, (const tbfcp_attr_t **)&pc_attr_FloorRequestInformation)) == 0 && pc_attr_FloorRequestInformation) {
                    if (TBFCP_ATTR(pc_attr_FloorRequestInformation)->hdr.type != tbfcp_attribute_type_FLOOR_REQUEST_INFORMATION) {
                        continue;
                    }
                    // Find "FloorRequestStatus"
                    u_index1 = 0;
                    while ((ret = tbfcp_attr_grouped_find_at(pc_attr_FloorRequestInformation, tbfcp_attribute_format_Grouped, u_index1++, (const tbfcp_attr_t **)&pc_attr_FloorRequestStatus)) == 0 && pc_attr_FloorRequestStatus) {
                        if (TBFCP_ATTR(pc_attr_FloorRequestStatus)->hdr.type != tbfcp_attribute_type_FLOOR_REQUEST_STATUS) {
                            continue;
                        }
                        if (pc_attr_FloorRequestStatus->extra_hdr.FloorID != atoi(p_bfcp->rfc4583.floorid)) {
                            continue;
                        }
                        break;
                    }
                    if (!pc_attr_FloorRequestStatus) {
                        continue;
                    }
                    // Find "OverallRequestStatus"
                    u_index2 = 0;
                    while ((ret = tbfcp_attr_grouped_find_at(pc_attr_FloorRequestInformation, tbfcp_attribute_format_Grouped, u_index2++, (const tbfcp_attr_t **)&pc_attr_OverallRequestStatus)) == 0 && pc_attr_OverallRequestStatus) {
                        if (TBFCP_ATTR(pc_attr_OverallRequestStatus)->hdr.type != tbfcp_attribute_type_OVERALL_REQUEST_STATUS) {
                            continue;
                        }

                        // Find "RequestStatus"
                        u_index3 = 0;
                        while ((ret = tbfcp_attr_grouped_find_at(pc_attr_OverallRequestStatus, tbfcp_attribute_format_OctetString16, u_index3++, (const tbfcp_attr_t **)&pc_attr_RequestStatus)) == 0 && pc_attr_RequestStatus) {
                            if (TBFCP_ATTR(pc_attr_RequestStatus)->hdr.type != tbfcp_attribute_type_REQUEST_STATUS) {
                                continue;
                            }
                            break;
                        }
                    }
                    if (pc_attr_RequestStatus) {
                        break;
                    }
                }

                if (pc_attr_RequestStatus) {
                    // https://tools.ietf.org/html/rfc4582#section-5.2.5
                    uint16_t u_status = pc_attr_RequestStatus->OctetString16[0] + (pc_attr_RequestStatus->OctetString16[1] << 8);
                    if (transac_id_matched) {
                        if (u_status == tbfcp_reqstatus_Revoked && !p_bfcp->b_revoked_handled) { // revoked
                            TSK_OBJECT_SAFE_FREE(p_bfcp->p_pkt_FloorRequest); // free the FloorRequest and ask new one once HelloAck is received
                            // Radvision sends a Revoke after a reINVITE to ask for new negotiation.
                            if (p_bfcp->p_pkt_FloorRelease) {
                                if ((ret = tbfcp_session_send_pkt(p_bfcp->p_bfcp_s, p_bfcp->p_pkt_FloorRelease))) {
                                    goto raise_err;
                                }
                            }
                            if ((ret = _tdav_session_bfcp_send_Hello(p_bfcp))) {
                                goto raise_err;
                            }
                            p_bfcp->b_revoked_handled = tsk_true;
                        }
                        else {
                            _RAISE_FLREQ(u_status, kInfoTextFloorReqStatus);
                        }
                    }
                    else { //!transac_id_matched
                        // Status from old FloorRequest
                        tbfcp_pkt_t* p_pkt = tsk_null;
                        TSK_DEBUG_INFO("Status from old Request");
                        if (u_status == tbfcp_reqstatus_Pending || u_status == tbfcp_reqstatus_Accepted || u_status == tbfcp_reqstatus_Granted) {
                            if ((ret = tbfcp_pkt_create_FloorRelease_2(e->pc_pkt->hdr.conf_id, e->pc_pkt->hdr.transac_id, e->pc_pkt->hdr.user_id, pc_attr_FloorRequestStatus->extra_hdr.FloorID, &p_pkt))) {
                                goto raise_err;
                            }
                            ret = tbfcp_session_send_pkt(p_bfcp->p_bfcp_s, p_pkt);
                            TSK_OBJECT_SAFE_FREE(p_pkt);
                            if (ret) {
                                goto raise_err;
                            }
                        }
                    }
                }
                else {
                    /* /!\ No RequestStatus attribute in FloorRequestStatus */
                    TSK_OBJECT_SAFE_FREE(p_bfcp->p_pkt_FloorRequest);
                    TSK_DEBUG_ERROR("%s", kErrTextBadRequest);
                    _RAISE_ERR_AND_GOTO_BAIL(kErrCodeBadRequest, kErrTextBadRequest);
                }
            }
            else {
                switch (e->pc_pkt->hdr.primitive) {
                case tbfcp_primitive_Hello:
                    break; // already handled in "_tbfcp_session_process_incoming_pkt()"
                default: {
                    TSK_DEBUG_ERROR("%s", kErrTextUnExpectedIncomingMsg);
                    _RAISE_ERR_AND_GOTO_BAIL(kErrCodeUnExpectedIncomingMsg, kErrTextUnExpectedIncomingMsg);
                    break;
                }
                }
            }
        }
        break;
    }
    case tbfcp_session_event_type_err_send_timedout: {
        /* /!\ Sending BFCP message timedout */
        TSK_DEBUG_ERROR("%s", kErrTextTimeout);
        _RAISE_ERR_AND_GOTO_BAIL(kErrCodeTimeout, kErrTextTimeout);
        break;
    }
    }
raise_err:
    if (ret) {
        TSK_DEBUG_ERROR("%s", kErrTextGlobalError);
        _RAISE_ERR_AND_GOTO_BAIL(kErrCodeGlobalError, kErrTextGlobalError);
    }
bail:

    TSK_OBJECT_SAFE_FREE(p_bfcp);
    return ret;
}


/* ============ Public functions ================= */





//=================================================================================================
//	Session MSRp Plugin object definition
//
/* constructor */
static tsk_object_t* _tdav_session_bfcp_ctor(tsk_object_t * p_self, va_list * app)
{
    tdav_session_bfcp_t *p_session = (tdav_session_bfcp_t *)p_self;
    if (p_session) {
        /* init base: called by tmedia_session_create() */
        /* init self */
        // TMEDIA_SESSION_BFCP(session)->send_file = tdav_session_bfcp_send_file;
        // TMEDIA_SESSION_BFCP(session)->send_message = tdav_session_bfcp_send_message;

        // session->config = tbfcp_config_create();
        // session->setup = bfcp_setup_actpass;
        // session->dir = tdav_bfcp_dir_none;
    }
    return p_self;
}
/* destructor */
static tsk_object_t* _tdav_session_bfcp_dtor(tsk_object_t * p_self)
{
    tdav_session_bfcp_t *p_session = (tdav_session_bfcp_t *)p_self;
    if (p_session) {
        /* deinit self */

        TSK_OBJECT_SAFE_FREE(p_session->p_bfcp_s);
        TSK_OBJECT_SAFE_FREE(p_session->p_pkt_FloorRequest);
        TSK_OBJECT_SAFE_FREE(p_session->p_pkt_FloorRelease);
        TSK_OBJECT_SAFE_FREE(p_session->p_pkt_Hello);

        TSK_FREE(p_session->p_local_ip);
        TSK_FREE(p_session->p_remote_ip);

        /* rfc4583 */
        TSK_FREE(p_session->rfc4583.confid);
        TSK_FREE(p_session->rfc4583.floorid);
        TSK_FREE(p_session->rfc4583.mstrm);
        TSK_FREE(p_session->rfc4583.userid);

        /* NAT Traversal context */
        TSK_OBJECT_SAFE_FREE(p_session->p_natt_ctx);

        /* deinit base */
        tmedia_session_deinit(p_self);

        TSK_DEBUG_INFO("*** tdav_session_bfcp_t destroyed ***");
    }

    return p_self;
}
/* object definition */
static const tsk_object_def_t tdav_session_bfcp_def_s = {
    sizeof(tdav_session_bfcp_t),
    _tdav_session_bfcp_ctor,
    _tdav_session_bfcp_dtor,
    tmedia_session_cmp,
};
/* plugin definition*/
static const tmedia_session_plugin_def_t tdav_session_bfcp_plugin_def_s = {
    &tdav_session_bfcp_def_s,

    tmedia_bfcp,
    "application",

    _tdav_session_bfcp_set,
    _tdav_session_bfcp_get,
    _tdav_session_bfcp_prepare,
    _tdav_session_bfcp_start,
    _tdav_session_bfcp_pause,
    _tdav_session_bfcp_stop,

    /* Audio part */
    { tsk_null },

    _tdav_session_bfcp_get_lo,
    _tdav_session_bfcp_set_ro
};
const tmedia_session_plugin_def_t *tdav_session_bfcp_plugin_def_t = &tdav_session_bfcp_plugin_def_s;

#endif /* !defined(HAVE_TINYBFCP) || HAVE_TINYBFCP */