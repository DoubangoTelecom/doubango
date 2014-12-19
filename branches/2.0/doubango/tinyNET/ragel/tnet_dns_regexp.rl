/*
* Copyright (C) 2010-2015 Mamadou DIOP.
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
/**@file tnet_dns_regexp.c
 * @brief DNS Regex parser for NAPTR RR.
 */
#include "tnet_dns_regexp.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_ragel_state.h"
#include "tsk_debug.h"

#include <string.h>

/* ===	Ragel state machine === */
%%{
	machine tdns_machine_regexp;

	action tag {
		tag_start = p;
	}

	action parse_prefix {
		TSK_PARSER_SET_STRING(prefix);
	}

	action cat_any{
		int len = (int)(p  - tag_start);
		if (len) {
			tsk_strncat(&ret, tag_start, len);
		}
	}

	action cat_group {
		if (prefix) {
			int prefixlen = (tsk_size_t)tsk_strlen(prefix);
			tsk_strncat(&ret, e164num + prefixlen, (e164len - prefixlen));
		}
	}

	#	http://www.itu.int/itudoc/itu-t/workshop/enum/012_pp7.ppt
	#	The format is like this: !<regexp>!<string>!
	#	* 1. Match <regexp>  on the original E.164 number
	#	* 2. Apply rewrite rule <string>
	#	* \n in <string> is replaced with group number 'n' in <regexp>
	#	
	
	regexp = (".*") | ("\\"? <:any*>tag %parse_prefix :>"(.*)");
	group = "\\" digit;
	string = (any -- "\\")*>tag %cat_any (group >tag %cat_group)? (any -- "\\")*>tag %cat_any;
	
	# Supported regexp --> !^.*$! or !^+1800(.*)! where "1800" is called the prefix
	main := "!^"<: regexp :> "$!" <: string :>"!" "i"?;
}%%

/**
* Apply @a regexp to @a e164num.
* @param e164num Original E.164 number supplied by the user (only digits or '+' are accepted).
* @param regexp  A <character-string> containing a substitution expression that is
* applied to the original string held by the client in order to
* construct the next domain name to lookup. Example: "!^.*$!sip:bob@doubango.org!i".
* @retval The final Internet address. It's up to the caller to free the string.
*/
char* tnet_dns_regex_parse(const char* e164num, const char* regexp)
{
	char* ret = tsk_null;
	char* prefix = tsk_null;
	const char* tag_start;
	tsk_size_t e164len;
		
	// Ragel
	int cs = 0;
	const char *p = tag_start = regexp;
	const char *pe;
	const char *eof;
	
	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	%%write data;
	TSK_RAGEL_DISABLE_WARNINGS_END()
	(void)(eof);
	(void)(tdns_machine_regexp_first_final);
	(void)(tdns_machine_regexp_error);
	(void)(tdns_machine_regexp_en_main);
	
	if (!e164num) {
		goto bail;
	}
	
	if (!regexp) {
		ret = tsk_strdup(e164num);
		goto bail;
	}
	
	e164len = (tsk_size_t)tsk_strlen(e164num);
	pe = p + tsk_strlen(regexp);
	eof = pe;
	
	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	%%write init;
	%%write exec;
	TSK_RAGEL_DISABLE_WARNINGS_END()
	
	if (cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("regexp substitition failed.");
		TSK_FREE(ret);
	}

bail:
	TSK_FREE(prefix);
	
	return ret;
}
