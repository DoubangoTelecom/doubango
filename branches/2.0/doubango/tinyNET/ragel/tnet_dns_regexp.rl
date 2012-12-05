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
/**@file tnet_dns_regexp.h
 * @brief DNS Regex parser for NAPTR RR.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

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

	action tag{
		tag_start = p;
	}

	action parse_prefix{
		TSK_PARSER_SET_STRING(prefix);
	}

	action cat_any{
		int len = (int)(p  - tag_start);
		if(len){
			tsk_strncat(&ret, tag_start, len);
		}
	}

	action cat_group{
		if(prefix){
			int prefixlen = tsk_strlen(prefix);
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
	
	%%write data;
	(void)(eof);
	(void)(tdns_machine_regexp_first_final);
	(void)(tdns_machine_regexp_error);
	(void)(tdns_machine_regexp_en_main);
	
	if(!e164num){
		goto bail;
	}
	
	if(!regexp){
		ret = tsk_strdup(e164num);
		goto bail;
	}
	
	e164len = tsk_strlen(e164num);
	pe = p + tsk_strlen(regexp);
	eof = pe;
	
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("regexp substitition failed.");
		TSK_FREE(ret);
	}

bail:
	TSK_FREE(prefix);
	
	return ret;
}













///*
//* Copyright (C) 2010-2011 Mamadou Diop.
//*
//* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
//*	
//* This file is part of Open Source Doubango Framework.
//*
//* DOUBANGO is free software: you can redistribute it and/or modify
//* it under the terms of the GNU General Public License as published by
//* the Free Software Foundation, either version 3 of the License, or
//* (at your option) any later version.
//*	
//* DOUBANGO is distributed in the hope that it will be useful,
//* but WITHOUT ANY WARRANTY; without even the implied warranty of
//* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//* GNU General Public License for more details.
//*	
//* You should have received a copy of the GNU General Public License
//* along with DOUBANGO.
//*
//*/
///**@file tnet_dns_regexp.h
// * @brief DNS Regex parser for NAPTR RR.
// *
// * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
// *
//
// */
//#include "tnet_dns_regexp.h"
//
//#include "tsk_debug.h"
//#include "tsk_memory.h"
//#include "tsk_string.h"
//#include "tsk_list.h"
//
//#include <string.h>
//
//const tsk_object_def_t *group_def_t;
//typedef tsk_list_t groups_L_t;
//
//#define CREATE_GROUP(id)	tsk_object_new(group_def_t, (int)id)
//typedef struct group_s
//{
//	TSK_DECLARE_OBJECT;
//
//	int id;
//	tsk_size_t start;
//	tsk_size_t end;
//}
//group_t;
//
///* ===	Ragel state machine === */
//%%{
//	machine tdns_machine_regexp;
//
//	action tag{
//		tag_start = p;
//	}
//
//	action inc_index{
//		index++;
//		TSK_DEBUG_INFO("inc_index %d", index);
//	}
//	
//	action create_group{
//		if(curr_group){
//			TSK_OBJECT_SAFE_FREE(curr_group);
//		}
//		curr_group = CREATE_GROUP(++group_id);
//		curr_group->start = index;
//		TSK_DEBUG_INFO("Create groupe");
//	}
//	
//	action add_group{
//		curr_group->end = index;
//		tsk_list_push_back_data(groups, (void**)&curr_group);
//		TSK_DEBUG_INFO("Create groupe");
//	}
//
//	#	http://www.itu.int/itudoc/itu-t/workshop/enum/012_pp7.ppt
//	#	The format is like this: !<regexp>!<string>!
//	#	* 1. Match <regexp>  on the original E.164 number
//	#	* 2. Apply rewrite rule <string>
//	#	* \n in <string> is replaced with group number 'n' in <regexp>
//	#	
//
//	# Match beginning
//	m_begin = "^" any* $inc_index;
//	
//	# Match end
//	m_end = "$";
//	
//	# Math any character
//	m_any_c = ".";
//	
//	# Match any number of any character
//	m_any_string = ".*" | ".+";
//	
//	# Grouping
//	group_n = "\\" digit+;
//	group_begin = "(" >create_group;
//	group_end = ")" >add_group;
//	group = group_begin<: any* :>group_end;
//	
//	# Escaped char
//	escaped_char = "\\" alpha;
//	
//	regexp = 
//		m_begin<:
//		m_any_c | m_any_string | group
//		:>m_end;
//	
//	string = any*;
//	
//	main := "!"<: regexp :> "!" <: string :> "!";
//}%%
//
///**
//* Apply @a regexp to @a e164num.
//* @param e164num Original E.164 number supplied by the user (only digits or '+' are accepted).
//* @param regexp  A <character-string> containing a substitution expression that is
//* applied to the original string held by the client in order to
//* construct the next domain name to lookup. Example: "!^.*$!sip:bob@doubango.org!i".
//* @retval The final Internet address. It's up to the caller to free the string.
//*/
//char* tnet_dns_regex_parse(const char* e164num, const char* regexp)
//{
//	char* ret = tsk_null;
//	groups_L_t* groups = tsk_list_create();
//	group_t* curr_group = tsk_null;
//	tsk_size_t index = 0;
//	int group_id = 0;
//	
//	// Ragel
//	int cs = 0;
//	const char *p = regexp;
//	const char *pe;
//	const char *eof;
//	
//	%%write data;
//	
//	if(!e164num){
//		goto bail;
//	}
//	
//	if(!regexp){
//		ret = tsk_strdup(e164num);
//		goto bail;
//	}
//	
//	pe = p + tsk_strlen(regexp);
//	eof = pe;
//	
//	%%write init;
//	%%write exec;
//	
//	if( cs < %%{ write first_final; }%% ){
//		TSK_DEBUG_ERROR("regexp substitition failed.");
//		TSK_FREE(ret);
//	}
//
//bail:
//	TSK_OBJECT_SAFE_FREE(groups);
//	if(curr_group){
//		TSK_OBJECT_SAFE_FREE(curr_group);
//	}
//	
//	return ret;
//}
//
////=================================================================================================
////	String object definition
////
//static tsk_object_t* group_create(tsk_object_t * self, va_list * app)
//{
//	group_t *g = self;
//	if(g){
//		g->id = va_arg(*app, int);
//	}
//	return self;
//}
//
//static tsk_object_t* group_destroy(tsk_object_t * self)
//{ 
//	group_t *g = self;
//	if(g){
//	}
//
//	return self;
//}
//
//static int group_cmp(const tsk_object_t *_g1, const tsk_object_t *_g2)
//{
//	const group_t *g1 = _g1;
//	const group_t *g2 = _g2;
//
//	if(g1 && g22){
//		return (g1->id - g2->id);
//	}
//	else if(!g1 && !g2) return 0;
//	else return -1;
//}
//
//static const tsk_object_def_t group_def_s = 
//{
//	sizeof(group_t),
//	group_create, 
//	group_destroy,
//	group_cmp, 
//};
//const tsk_object_def_t *group_def_t = &group_def_s;
