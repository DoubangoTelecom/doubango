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

#include "tsk_options.h"

_BEGIN_DECLS

typedef enum cmd_type_e
{	
	cmd_none,

	cmd_audio, /* ++audio | ++a*/
	cmd_audiovideo, /* ++audiovideo | ++av*/
	cmd_config_file, /* ++config-file | ++cf*/
	cmd_config_session, /* ++config-session | ++css */
	cmd_config_stack, /* ++config-stack | ++cst*/
	cmd_exit,	/*++exit | ++e*/
	cmd_file, /* ++file | ++f*/
	cmd_hangup, /* ++hangup | ++hp */
	cmd_help, /* ++help | ++h  */
	cmd_message, /* ++message | ++m*/
	cmd_publish, /* ++publish | ++pub*/
	cmd_quit, /* ++quit | ++q */
	cmd_register, /* ++register | ++reg */
	cmd_run, /* ++run | ++r*/
	cmd_sms,	/* ++sms */
	cmd_subscribe, /* ++subscribe | ++sub */
	cmd_video, /* ++video | ++v */
}
cmd_type_t;

typedef enum option_e
{
	opt_none,

	opt_amf,			/* --amf 0x85FF */
	opt_caps,			/* --caps +g.oma.sip-im or language=en,fr*/
	opt_dhcpv4,			/* --dhcpv4 */
	opt_dhcpv6,			/* --dhcpv6 */
	opt_dname,			/* --dname bob */
	opt_dns_naptr,		/* --dns-naptr */
	opt_from,			/* --from sip:alice@open-ims.test */
	opt_header,			/* --header Supported=norefersub */
	opt_impi,			/* --impi bob@open-ims.test */
	opt_impu,			/* --impu sip:bob@open-ims.test */
	opt_ipv6,			/* --ipv6 */
	opt_local_ip,		/* --local-ip 192.168.0.10 */
	opt_local_port,		/* --local-port 4000 */
	opt_opid,			/* --opid 0xA712F5D04B */
	opt_password,		/* --password mysecret */
	opt_pcscf_ip,		/* --pcscf-ip 192.168.0.13 */
	opt_pcscf_port,		/* --pcscf-port 5060 */
	opt_pcscf_trans,	/* --pcscf-trans udp */
	opt_realm,			/* --realm open-ims.test */
	opt_sid,			/* --sid 1234 */
	opt_sigcomp,		/* --sigcomp */
	opt_to,				/* --to sip:alice@open-ims.test */
}
option_t;


tsk_options_L_t* cmd_parse(char* buffer, cmd_type_t* cmd);
void cmd_print_help();

_END_DECLS

#endif /* TINYDEMO_CMD_H */
