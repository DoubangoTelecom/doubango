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
/**@file tnet_dns_resolvconf.c
 * @brief Parser for "/etc/resolv.conf" file to retrive DNS servers.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_dns_resolvconf.h"

#include "tnet_utils.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_ragel_state.h"
#include "tsk_debug.h"

#include <string.h>


/* ===	Ragel state machine === */
%%{
	machine tdns_machine_resolvconf;

	action tag{
		tag_start = p;
	}

	action add_dns{
		int len = (int)(p  - tag_start);
		if(len && len<=sizeof(ip)){
			tnet_address_t *address;
			memset(ip, '\0', sizeof(ip));
			memcpy(ip, tag_start, len);
			
			address = tnet_address_create(ip);
			address->family = tnet_get_family(ip);
			address->dnsserver = 1;
			tsk_list_push_ascending_data(servers, (void**)&address);
		}
	}

	SP = " ";
	LF = "\n";
	CR = "\r";
	CRLF = CR LF;
	ENDL = (LF | CR | CRLF);

	o_name = any+;
	o_value = any+;
	
	COMMENT = SP*<: "#" any*;
	OPTION_ANY = SP*<: o_name :>SP+<: o_value :>SP*;
	OPTION_DNS = SP*<: "nameserver"i :>SP+<: o_value>tag %add_dns :>SP*;
	LINE = (OPTION_DNS)@100 | (OPTION_ANY)@0 | (COMMENT)@50;
	
	main := ((LINE :>ENDL) | ENDL)* %3  any* %2;
}%%

/** Gets list of DNS servers from a conf file.
* @param path Path of the conf file from which to retrieve the DNS servers.
* should be @a "/etc/resolv.conf". You can adjust the value by modifying @ref TNET_RESOLV_CONF_PATH.<br>
* If you are using <b>Android</b> and the resolv.conf file is missing, then run the following line in a command window: <br>
* <i>ln -s /private/var/run/resolv.conf /etc/resolv.conf</i><br> If this fails, then try to manually add the file.
* @retval List of DNS servers.
*/
tnet_addresses_L_t * tnet_dns_resolvconf_parse(const char* path)
{
	tnet_addresses_L_t* servers = tsk_null;
	tnet_ip_t ip;
	const char* fullpath = path;
	const char* tag_start;
	FILE* fd;
	char buf[4092];
	
	// Ragel
	int cs = 0;
	const char *p;
	const char *pe;
	const char *eof;

	%%write data;

	if(!fullpath || tsk_strempty(fullpath)){
		fullpath = TNET_RESOLV_CONF_PATH;
	}

	/* Open the file and read all data */
	if((fd = fopen(fullpath, "r"))){
		int len = fread(buf, sizeof(uint8_t), sizeof(buf)-2, fd);
		p = &buf[0];
		pe = p + len + 1/*hack*/;
		eof = pe;
		fclose(fd);
		
		buf[len] = '\n'; // hack to have perfect lines

		servers = tsk_list_create();
	}
	else{
		TSK_DEBUG_ERROR("Failed to open %s.", fullpath);
		goto bail;
	}

	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse %s.", fullpath);
		TSK_OBJECT_SAFE_FREE(servers);
	}

bail:
	return servers;
}


