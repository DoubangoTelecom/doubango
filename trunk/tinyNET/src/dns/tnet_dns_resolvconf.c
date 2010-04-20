
/* #line 1 "./ragel/tnet_dns_resolvconf.rl" */
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

/* #line 78 "./ragel/tnet_dns_resolvconf.rl" */


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

	
/* #line 72 "./src/dns/tnet_dns_resolvconf.c" */
static const char _tdns_machine_resolvconf_actions[] = {
	0, 1, 0, 1, 1
};

static const char _tdns_machine_resolvconf_key_offsets[] = {
	0, 6, 7, 8, 11, 14, 14, 20, 
	22, 25, 28, 31, 34, 37, 40, 43, 
	46, 49, 50, 51, 54
};

static const char _tdns_machine_resolvconf_trans_keys[] = {
	10, 13, 32, 35, 78, 110, 32, 32, 
	10, 13, 32, 10, 13, 32, 10, 13, 
	32, 35, 78, 110, 10, 13, 32, 65, 
	97, 32, 77, 109, 32, 69, 101, 32, 
	83, 115, 32, 69, 101, 32, 82, 114, 
	32, 86, 118, 32, 69, 101, 32, 82, 
	114, 32, 32, 10, 13, 32, 32, 35, 
	78, 110, 0
};

static const char _tdns_machine_resolvconf_single_lengths[] = {
	6, 1, 1, 3, 3, 0, 6, 2, 
	3, 3, 3, 3, 3, 3, 3, 3, 
	3, 1, 1, 3, 4
};

static const char _tdns_machine_resolvconf_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0
};

static const char _tdns_machine_resolvconf_index_offsets[] = {
	0, 7, 9, 11, 15, 19, 20, 27, 
	30, 34, 38, 42, 46, 50, 54, 58, 
	62, 66, 68, 70, 74
};

static const char _tdns_machine_resolvconf_indicies[] = {
	1, 1, 2, 3, 4, 4, 0, 5, 
	0, 5, 6, 7, 7, 8, 6, 7, 
	7, 8, 9, 9, 1, 1, 5, 3, 
	4, 4, 0, 7, 7, 3, 5, 10, 
	10, 0, 5, 11, 11, 0, 5, 12, 
	12, 0, 5, 13, 13, 0, 5, 14, 
	14, 0, 5, 15, 15, 0, 5, 16, 
	16, 0, 5, 17, 17, 0, 5, 18, 
	18, 0, 19, 0, 19, 20, 22, 22, 
	23, 21, 2, 3, 4, 4, 0, 0
};

static const char _tdns_machine_resolvconf_trans_targs[] = {
	1, 6, 20, 7, 8, 2, 3, 0, 
	4, 5, 9, 10, 11, 12, 13, 14, 
	15, 16, 17, 18, 19, 19, 0, 4
};

static const char _tdns_machine_resolvconf_trans_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 1, 0, 3, 3
};

static const int tdns_machine_resolvconf_start = 0;
static const int tdns_machine_resolvconf_first_final = 0;
static const int tdns_machine_resolvconf_error = -1;

static const int tdns_machine_resolvconf_en_main = 0;


/* #line 103 "./ragel/tnet_dns_resolvconf.rl" */

	if(tsk_strnullORempty(fullpath)){
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

	
/* #line 168 "./src/dns/tnet_dns_resolvconf.c" */
	{
	cs = tdns_machine_resolvconf_start;
	}

/* #line 126 "./ragel/tnet_dns_resolvconf.rl" */
	
/* #line 175 "./src/dns/tnet_dns_resolvconf.c" */
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
_resume:
	_keys = _tdns_machine_resolvconf_trans_keys + _tdns_machine_resolvconf_key_offsets[cs];
	_trans = _tdns_machine_resolvconf_index_offsets[cs];

	_klen = _tdns_machine_resolvconf_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _tdns_machine_resolvconf_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += ((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _tdns_machine_resolvconf_indicies[_trans];
	cs = _tdns_machine_resolvconf_trans_targs[_trans];

	if ( _tdns_machine_resolvconf_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _tdns_machine_resolvconf_actions + _tdns_machine_resolvconf_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
/* #line 45 "./ragel/tnet_dns_resolvconf.rl" */
	{
		tag_start = p;
	}
	break;
	case 1:
/* #line 49 "./ragel/tnet_dns_resolvconf.rl" */
	{
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
	break;
/* #line 269 "./src/dns/tnet_dns_resolvconf.c" */
		}
	}

_again:
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	}

/* #line 127 "./ragel/tnet_dns_resolvconf.rl" */
	
	if( cs < 
/* #line 282 "./src/dns/tnet_dns_resolvconf.c" */
0
/* #line 128 "./ragel/tnet_dns_resolvconf.rl" */
 ){
		TSK_DEBUG_ERROR("Failed to parse %s.", fullpath);
		TSK_OBJECT_SAFE_FREE(servers);
	}

bail:
	return servers;
}


