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
	cmd_type_realm,			/* --realm open-ims.test */
	cmd_type_impi,			/* --impi bob@open-ims.test */
	cmd_type_impu,			/* --impu sip:bob@open-ims.test */
	cmd_type_pcscf_ip,		/* --pcscf_ip 192.168.0.13 */
	cmd_type_pcscf_port,	/* --pcscf_port 5060 */
	cmd_type_pcscf_trans,	/* --pcscf_trans udp */
	cmd_type_local_ip,		/* --local_ip 192.168.0.10 */
	cmd_type_local_port,	/* --local-port 4000 */
	cmd_type_ipv6,

	cmd_type_quit,			/* --quit */
	cmd_type_exit,			/* --exist */
}
cmd_type_t;


tsk_options_L_t* cmd_parse(char* buffer);

_END_DECLS

#endif /* TINYDEMO_CMD_H */
