/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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


/**@file tsdp_header_B.c
 * @brief SDP "m=" header (Media Descriptions).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Iat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySDP/headers/tsdp_header_M.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsdp_machine_parser_header_M;

	# Includes
	include tsdp_machine_utils "./tsdp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action parse_media{
		TSK_PARSER_SET_STRING(hdr_M->media);
	}
	
	action parse_port{
		TSK_PARSER_SET_UINT(hdr_M->port);
	}
	
	action parse_nports{
		TSK_PARSER_SET_UINT(hdr_M->nports);
	}
	
	action parse_proto{
		TSK_PARSER_SET_STRING(hdr_M->proto);
	}
	
	action parse_fmt{
		TSK_PARSER_ADD_STRING(hdr_M->FMTs);
	}
	
	media = token>tag %parse_media;
	port = DIGIT+>tag %parse_port;
	nports = DIGIT+>tag %parse_port;
	proto = (token ("/" token)*)>tag %parse_proto;
	fmt = token>tag %parse_fmt;
	
	#// media SP port ["/" integer] SP proto 1*(SP fmt)
	M = 'm' SP* "=" SP*<: media SP port ("/" nports)? SP proto (SP fmt)+;
	
	# Entry point
	main := M :>CRLF?;
}%%

int tsdp_header_M_tostring(const tsdp_header_t* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsdp_header_M_t *M = (const tsdp_header_M_t *)header;
		const tsk_list_item_t* item;
		tsk_istr_t nports;

		tsk_itoa(M->nports, &nports);
		
		/*	IMPORTANT: Keep the order.
			
			m=  (media name and transport address)
			i=* (media title)
			c=* (connection information -- optional if included at
				 session level)
			b=* (zero or more bandwidth information lines)
			k=* (encryption key)
			a=* (zero or more media attribute lines)
		*/
		tsk_buffer_appendEx(output, "%s %u%s%s %s",
			M->media,
			M->port,
			
			M->nports ? "/" : "",
			M->nports ? nports : "",

			M->proto
			);
		// FMTs
		tsk_list_foreach(item, M->FMTs){
			tsk_buffer_appendEx(output, " %s", TSK_STRING_STR(item->data));
		}
		tsk_buffer_append(output, "\r\n", 2); // close the "m=" line.
		// i=* (media title)
		if(M->I){
			tsdp_header_tostring(TSDP_HEADER(M->I), output);
		}
		// c=* (connection information -- optional if included at session level)
		if(M->C){
			tsdp_header_tostring(TSDP_HEADER(M->C), output);
		}
		// b=* (zero or more bandwidth information lines)
		if(M->Bandwidths){
			tsk_list_foreach(item, M->Bandwidths){
				tsdp_header_tostring(TSDP_HEADER(item->data), output);
			}
		}
		// k=* (encryption key)
		if(M->K){
			tsdp_header_tostring(TSDP_HEADER(M->K), output);
		}
		// a=* (zero or more media attribute lines)
		if(M->Attributes){
			tsk_list_foreach(item, M->Attributes){
				tsdp_header_tostring(TSDP_HEADER(item->data), output);
			}
		}
	}

	return -1;
}

tsdp_header_M_t *tsdp_header_M_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsdp_header_M_t *hdr_M = TSDP_HEADER_M_CREATE_NULL();
	
	const char *tag_start;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse \"m=\" header.");
		TSK_OBJECT_SAFE_FREE(hdr_M);
	}
	
	return hdr_M;
}







//========================================================
//	M header object definition
//

static void* tsdp_header_M_create(void *self, va_list * app)
{
	tsdp_header_M_t *M = self;
	if(M)
	{
		TSDP_HEADER(M)->type = tsdp_htype_M;
		TSDP_HEADER(M)->tostring = tsdp_header_M_tostring;
		TSDP_HEADER(M)->rank = TSDP_HTYPE_M_RANK;
		
		M->FMTs = TSK_LIST_CREATE(); // Becuase there is at least one fmt.

		M->media = tsk_strdup(va_arg(*app, const char*));
		M->port = va_arg(*app, uint32_t);
		M->proto = tsk_strdup(va_arg(*app, const char*));
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new M header.");
	}
	return self;
}

static void* tsdp_header_M_destroy(void *self)
{
	tsdp_header_M_t *M = self;
	if(M){
		TSK_FREE(M->media);
		TSK_FREE(M->proto);
		TSK_OBJECT_SAFE_FREE(M->FMTs);
		
		TSK_OBJECT_SAFE_FREE(M->I);
		TSK_OBJECT_SAFE_FREE(M->C);
		TSK_OBJECT_SAFE_FREE(M->Bandwidths);
		TSK_OBJECT_SAFE_FREE(M->K);
		TSK_OBJECT_SAFE_FREE(M->Attributes);
	}
	else{
		TSK_DEBUG_ERROR("Null M header.");
	}

	return self;
}
static int tsdp_header_M_cmp(const tsk_object_t *obj1, const tsk_object_t *obj2)
{
	if(obj1 && obj2){
		return tsdp_header_rank_cmp(obj1, obj2);
	}
	else{
		return -1;
	}
}

static const tsk_object_def_t tsdp_header_M_def_s = 
{
	sizeof(tsdp_header_M_t),
	tsdp_header_M_create,
	tsdp_header_M_destroy,
	tsdp_header_M_cmp
};

const void *tsdp_header_M_def_t = &tsdp_header_M_def_s;
