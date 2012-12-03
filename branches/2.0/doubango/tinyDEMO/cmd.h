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
#if !defined(TINYDEMO_CMD_H)
#define TINYDEMO_CMD_H

#include "demo_config.h"

#include "tsk_params.h"

_BEGIN_DECLS

typedef enum cmd_type_e
{	
	cmd_none,

	cmd_audio, /* ++audio | ++a*/
	cmd_audiovideo, /* ++audiovideo | ++av*/
	cmd_config_session, /* ++config-session | ++css */
	cmd_config_stack, /* ++config-stack | ++cst*/
	cmd_dtmf, /*++dtmf*/
	cmd_dump, /*++dump | ++d*/
	cmd_ect, /*++ect*/
	cmd_exit,	/*++exit | ++e | ++quit | ++q*/
	cmd_file, /* ++file | ++f*/
	cmd_hangup, /* ++hangup | ++hu */
	cmd_help, /* ++help | ++h  */
	cmd_hold, /* ++hold | ++ho  */
	cmd_large_message, /* ++large-message | ++lm */
	cmd_message, /* ++message | ++m*/
	cmd_options, /* ++options | ++opt*/
	cmd_publish, /* ++publish | ++pub*/
	cmd_register, /* ++register | ++reg */
	cmd_resume, /* ++resume | ++res */
	cmd_run, /* ++run | ++r*/
	cmd_scenario, /* ++scenario | ++sn*/
	cmd_sleep, /* ++sleep */
	cmd_sms,	/* ++sms */
	cmd_stop, /* ++stop */
	cmd_subscribe, /* ++subscribe | ++sub */
	cmd_video, /* ++video | ++v */
}
cmd_type_t;

typedef enum opt_type_e
{
	opt_none,

	opt_amf,			/* --amf 0x85FF */
	opt_caps,			/* --caps +g.oma.sip-im or language=en,fr*/
	opt_dhcpv4,			/* --dhcpv4 */
	opt_dhcpv6,			/* --dhcpv6 */
	opt_dname,			/* --dname bob */
	opt_dns_naptr,		/* --dns-naptr */
	opt_from,			/* --from sip:alice@open-ims.test */
	opt_event,			/* --event 2 */
	opt_expires,		/* --expires|--xp 30 */
	opt_header,			/* --header Supported=norefersub */
	opt_impi,			/* --impi bob@open-ims.test */
	opt_impu,			/* --impu sip:bob@open-ims.test */
	opt_ipv6,			/* --ipv6 */
	opt_local_ip,		/* --local-ip 192.168.0.10 */
	opt_local_port,		/* --local-port 5060 */
	opt_opid,			/* --opid 0xA712F5D04B */
	opt_password,		/* --password|--pwd mysecret */
	opt_path,			/* --path /cygdrive/c/Projects/sample.cfg */
	opt_payload,		/* --payload|--pay hello world! */
	opt_pcscf_ip,		/* --pcscf-ip 192.168.0.13 */
	opt_pcscf_port,		/* --pcscf-port 5060 */
	opt_pcscf_trans,	/* --pcscf-trans udp */
	opt_realm,			/* --realm open-ims.test */
	opt_sec,			/* --sec 1800 */
	opt_sid,			/* --sid 1234 */
	opt_sigcomp_id,		/* --sigcomp-id urn:uuid:2e5fdc76-00be-4314-8202-1116fa82a473 */
	opt_silent,			/* --silent */
	opt_smsc,			/* --smsc +3315245856 */
	opt_stun_ip,		/* --stun-ip numb.viagenie.ca */
	opt_stun_pwd,		/* --stun-pwd mysecret */
	opt_stun_port,		/* --stun-port 3478 */
	opt_stun_usr,		/* --stun-usr bob@open-ims.test */
	opt_to,				/* --to sip:alice@open-ims.test */
}
opt_type_t;

typedef enum lv_e
{
	lv_none,

	lv_stack,	/* @@stack | @@st */
	lv_session,	/* @@session | @@ss */
	lv_action	/* @@action | @@request | @@a | @@r*/
}
lv_t;

typedef struct opt_s
{
	TSK_DECLARE_OBJECT;

	opt_type_t type;
	lv_t lv;
	char* value;
}
opt_t;
typedef tsk_list_t opts_L_t;

typedef struct cmd_s
{
	TSK_DECLARE_OBJECT;

	cmd_type_t type;
	opts_L_t *opts;
	char* sidparam;
}
cmd_t;

cmd_t* cmd_create(cmd_type_t );
#define cmd_create_null() cmd_create(cmd_none)

cmd_t* cmd_parse(const char* buffer, tsk_size_t size, tsk_bool_t *comment, tsk_params_L_t* params);
void cmd_print_help();

opt_t* opt_create(opt_type_t, lv_t, const char*);
const opt_t* opt_get_by_type(const opts_L_t* , opt_type_t);
#define opt_create_null() opt_create(opt_none, lv_none, tsk_null)

const tsk_object_def_t *opt_def_t;
const tsk_object_def_t *cmd_def_t;

_END_DECLS

#endif /* TINYDEMO_CMD_H */
