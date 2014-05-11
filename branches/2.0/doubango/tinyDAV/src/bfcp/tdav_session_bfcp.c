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

#include "tinybfcp/tbfcp_session.h"
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

typedef struct tdav_session_bfcp_s
{
	TMEDIA_DECLARE_SESSION_BFCP;

	struct tbfcp_session_s* p_bfcp_s;

	tsk_bool_t b_use_ipv6;

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
		else if (tsk_striequals(param->key, "local-ipver")) {
			p_bfcp->b_use_ipv6 = tsk_striequals(param->value, "ipv6");
		}
	}
	else if (param->value_type == tmedia_pvt_pobject) {
		if (tsk_striequals(param->key, "natt-ctx")) {
			TSK_OBJECT_SAFE_FREE(p_bfcp->p_natt_ctx);
			p_bfcp->p_natt_ctx = tsk_object_ref(param->value);
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

	if (p_bfcp->p_bfcp_s) {
		ret = tbfcp_session_stop(p_bfcp->p_bfcp_s);
	}

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

	if (p_self->ro_changed && p_self->M.lo){
		/* Codecs */
		tsdp_header_A_removeAll_by_field(p_self->M.lo->Attributes, "fmtp");
		tsdp_header_A_removeAll_by_field(p_self->M.lo->Attributes, "rtpmap");
		tsk_list_clear_items(p_self->M.lo->FMTs);
		
		/* QoS */
		tsdp_header_A_removeAll_by_field(p_self->M.lo->Attributes, "curr");
		tsdp_header_A_removeAll_by_field(p_self->M.lo->Attributes, "des");
		tsdp_header_A_removeAll_by_field(p_self->M.lo->Attributes, "conf");
	}

	b_changed = (p_self->ro_changed || !p_self->M.lo);

	if (!b_changed) {
		TSK_DEBUG_INFO("No changes to the BFCP session...skip SDP update");
		return p_self->M.lo;
	}

	if (b_changed && p_self->M.lo) {
		static const char* __fields[] = { "floorctrl", "setup", "connection" };
		// remove media-level attributes
		tsdp_header_A_removeAll_by_fields(p_self->M.lo->Attributes, __fields, sizeof(__fields)/sizeof(__fields[0]));
		tsk_list_clear_items(p_self->M.lo->FMTs);
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
	if ((ret = tbfcp_utils_get_setup(e_local_setup, &pc_local_setup))) {
		return tsk_null;
	}

	if (!p_self->M.lo){
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
		if ((A = tsdp_header_M_findA(m, "floorctrl"))) {
			if ((ret = tbfcp_utils_parse_role(A->value, &e_remote_role))) {
				return ret;
			}
		}
		if ((A = tsdp_header_M_findA(m, "confid"))) {
			tsk_strupdate(&p_bfcp->rfc4583.confid, A->value);
			u_remote_conf_id = (uint32_t)tsk_atoi64(p_bfcp->rfc4583.confid);
		}
		if ((A = tsdp_header_M_findA(m, "userid"))) {
			tsk_strupdate(&p_bfcp->rfc4583.userid, A->value);
			u_remote_user_id = (uint16_t)tsk_atoi64(p_bfcp->rfc4583.userid);
		}
		if ((A = tsdp_header_M_findA(m, "floorid"))) {
			char tmp_str[256];
			if (sscanf(A->value, "%255s %*s", tmp_str) != EOF) {
				char *pch, *saveptr;
				tsk_strupdate(&p_bfcp->rfc4583.floorid, tmp_str);
				u_remote_floor_id = (uint16_t)tsk_atoi64(p_bfcp->rfc4583.floorid);
				pch = tsk_strtok_r(&A->value[tsk_strlen(tmp_str) + 1], " ", &saveptr);
				while (pch) {
					if (sscanf(pch, "mstrm: %255s", tmp_str) != EOF) {
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
	}

	return p_self;
}
/* object definition */
static const tsk_object_def_t tdav_session_bfcp_def_s = 
{
	sizeof(tdav_session_bfcp_t),
	_tdav_session_bfcp_ctor, 
	_tdav_session_bfcp_dtor,
	tmedia_session_cmp, 
};
/* plugin definition*/
static const tmedia_session_plugin_def_t tdav_session_bfcp_plugin_def_s = 
{
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
