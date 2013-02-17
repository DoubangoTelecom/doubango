/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tnet_dns_resolvconf.h"

#include "tnet_utils.h"

#include "dns/tnet_dns.h"

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
			TSK_DEBUG_INFO("Adding DNS server = %s:%d", ip, TNET_DNS_SERVER_PORT_DEFAULT);
			
			address = tnet_address_create(ip);
			address->family = tnet_get_family(ip, TNET_DNS_SERVER_PORT_DEFAULT);
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
	
	COMMENT = SP*<: ("#" | ";") any*;
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
	const char* tag_start = tsk_null;
	FILE* fd;
	char* buf = tsk_null;
	
	// Ragel
	int cs = 0;
	const char *p;
	const char *pe;
	const char *eof;

	%%write data;
	(void)(eof);
	(void)(tdns_machine_resolvconf_first_final);
	(void)(tdns_machine_resolvconf_error);
	(void)(tdns_machine_resolvconf_en_main);

	if(tsk_strnullORempty(fullpath)){
		fullpath = TNET_RESOLV_CONF_PATH;
	}

	/* Open the file and read all data */
	if((fd = fopen(fullpath, "r"))){
		long len;
		fseek(fd, 0L, SEEK_END);
		len = ftell(fd);
		fseek(fd, 0L, SEEK_SET);
		if(!(buf = (char*)tsk_calloc(len + 1, 1))){
			TSK_DEBUG_ERROR("Failed to allocate buffer with size = %ld", (len + 1));
			goto bail;
		}
		fread(buf, 1, len, fd);
		p = &buf[0];
		pe = p + len + 1/*hack*/;
		eof = pe;
		fclose(fd);
		
		buf[len] = '\n'; // hack to have perfect lines

		servers = tsk_list_create();
	}
	else{
#if ANDROID || defined(__APPLE__) /* TARGET_OS_IPHONE not defined for bsd libraries */
		TSK_DEBUG_INFO("Failed to open [%s]. But don't panic, we have detected that you are using Google Android/iOS Systems.\n"
			"You should look at the Progammer's Guide for more information.\n If you are not using DNS functions, don't worry about this warning.", 
			fullpath);
#else
		TSK_DEBUG_ERROR("Failed to open %s.", fullpath);
#endif
		goto bail;
	}

	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse %s.", fullpath);
		TSK_OBJECT_SAFE_FREE(servers);
	}

bail:
	TSK_FREE(buf);
	return servers;
}


