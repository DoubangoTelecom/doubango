/*
* Copyright (C) 2010-2015 Mamadou Diop.
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
/**@file tsdp_machine_message.rl
 * @brief Ragel file.
 */
#include "tinysdp/parsers/tsdp_parser_message.h"

#include "tinysdp/headers/tsdp_header_A.h"
#include "tinysdp/headers/tsdp_header_B.h"
#include "tinysdp/headers/tsdp_header_C.h"
#include "tinysdp/headers/tsdp_header_Dummy.h"
#include "tinysdp/headers/tsdp_header_E.h"
#include "tinysdp/headers/tsdp_header_I.h"
#include "tinysdp/headers/tsdp_header_K.h"
#include "tinysdp/headers/tsdp_header_M.h"
#include "tinysdp/headers/tsdp_header_O.h"
#include "tinysdp/headers/tsdp_header_P.h"
#include "tinysdp/headers/tsdp_header_R.h"
#include "tinysdp/headers/tsdp_header_S.h"
#include "tinysdp/headers/tsdp_header_T.h"
#include "tinysdp/headers/tsdp_header_U.h"
#include "tinysdp/headers/tsdp_header_V.h"
#include "tinysdp/headers/tsdp_header_Z.h"

#include "tsk_debug.h"

%%{
	machine tsdp_machine_message;
	
	###########################################
	#	Includes
	###########################################
	include tsdp_machine_utils "./ragel/tsdp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	###########################################
	#	Actions
	###########################################
	action parse_header_A{
		if(hdr_M){
			if(!hdr_M->Attributes){
				hdr_M->Attributes = tsk_list_create();
			}
			if((header = (tsdp_header_t*)tsdp_header_A_parse(tag_start, (p - tag_start)))){
				tsk_list_push_back_data(hdr_M->Attributes, (void**)&header);
			}
		}
		else if((header = (tsdp_header_t*)tsdp_header_A_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
	}

	action parse_header_B{
		if(hdr_M){
			if(!hdr_M->Bandwidths){
				hdr_M->Bandwidths = tsk_list_create();
			}
			if((header = (tsdp_header_t*)tsdp_header_B_parse(tag_start, (p - tag_start)))){
				tsk_list_push_back_data(hdr_M->Bandwidths, (void**)&header);
			}
		}
		else if((header = (tsdp_header_t*)tsdp_header_B_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
	}
	
	action parse_header_C{
		if(hdr_M && !hdr_M->C){
			hdr_M->C = tsdp_header_C_parse(tag_start, (p - tag_start));
		}
		else if((header = (tsdp_header_t*)tsdp_header_C_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
	}
	
	action parse_header_Dummy{
		if((header = (tsdp_header_t*)tsdp_header_Dummy_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
	}
	
	action parse_header_E{
		if((header = (tsdp_header_t*)tsdp_header_E_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
	}
	
	action parse_header_I{
		if(hdr_M && !hdr_M->I){
			hdr_M->I = tsdp_header_I_parse(tag_start, (p - tag_start));
		}
		else if((header = (tsdp_header_t*)tsdp_header_I_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
	}
	
	action parse_header_K{
		if(hdr_M && !hdr_M->K){
			hdr_M->K = tsdp_header_K_parse(tag_start, (p - tag_start));
		}
		else if((header = (tsdp_header_t*)tsdp_header_K_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
	}
	
	action parse_header_M{
		if((hdr_M = tsdp_header_M_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, TSDP_HEADER(hdr_M));
			hdr_M = tsk_object_unref(hdr_M);
		}
	}
	
	action parse_header_O{
		if((header = (tsdp_header_t*)tsdp_header_O_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
	}
	
	action parse_header_P{
		if((header = (tsdp_header_t*)tsdp_header_P_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
	}

	action parse_header_R{
		if((header = (tsdp_header_t*)tsdp_header_R_parse(tag_start, (p - tag_start)))){
			if(hdr_T){
				if(!hdr_T->repeat_fields){
					hdr_T->repeat_fields = tsk_list_create();
				}
				tsk_list_push_back_data(hdr_T->repeat_fields, (void**)&header);
			}
			else{
				tsdp_message_add_header(sdp_msg, header);
				tsk_object_unref(header);
			}
		}
	}
	
	action parse_header_S{
		if((header = (tsdp_header_t*)tsdp_header_S_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
	}
	
	action parse_header_T{
		if((hdr_T = tsdp_header_T_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, TSDP_HEADER(hdr_T));
			hdr_T = tsk_object_unref(hdr_T);
		}
	}

	action parse_header_U{
		if((header = (tsdp_header_t*)tsdp_header_U_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
	}

	action parse_header_V{
		if((header = (tsdp_header_t*)tsdp_header_V_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
	}

	action parse_header_Z{
		if((header = (tsdp_header_t*)tsdp_header_Z_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
	}

	###########################################
	#	Headers
	###########################################

	A = "a"i SP* "=" SP*<: any* %parse_header_A :>CRLF;
	B = "b"i SP* "=" SP*<: any* %parse_header_B :>CRLF;
	C = "c"i SP* "=" SP*<: any* %parse_header_C :>CRLF;
	E = "e"i SP* "=" SP*<: any* %parse_header_E :>CRLF;
	I = "i"i SP* "=" SP*<: any* %parse_header_I :>CRLF;
	K = "k"i SP* "=" SP*<: any* %parse_header_K :>CRLF;
	M = "m"i SP* "=" SP*<: any* %parse_header_M :>CRLF;
	O = "o"i SP* "=" SP*<: any* %parse_header_O :>CRLF;
	P = "p"i SP* "=" SP*<: any* %parse_header_P :>CRLF;
	R = "r"i SP* "=" SP*<: any* %parse_header_R :>CRLF;
	S = "s"i SP* "=" SP*<: any* %parse_header_S :>CRLF;
	T = "t"i SP* "=" SP*<: any* %parse_header_T :>CRLF;
	U = "u"i SP* "=" SP*<: any* %parse_header_U :>CRLF;
	V = "v"i SP* "=" SP*<: any* %parse_header_V :>CRLF;
	Z = "z"i SP* "=" SP*<: any* %parse_header_Z :>CRLF;

	Dummy = alpha SP* "=" SP*<: any* %parse_header_Dummy :>CRLF;

	Header = (A | B | C | E | I | K | M | O | P | R | S | T | U | V | Z)>tag >1 | (Dummy>tag) >0;


	###########################################
	#	Message
	###########################################
	SDP_message = Header*;

	###########################################
	#	Entry Point
	###########################################
	main := SDP_message;
}%%

TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
/* Ragel data */
%% write data;
TSK_RAGEL_DISABLE_WARNINGS_END()

tsdp_message_t* tsdp_message_parse(const void *input, tsk_size_t size)
{
	tsdp_message_t* sdp_msg = tsk_null;
	const char* tag_start = tsk_null;
	tsdp_header_t *header = tsk_null;
	tsdp_header_T_t *hdr_T = tsk_null;
	tsdp_header_M_t *hdr_M = tsk_null;

	/* Ragel variables */
	int cs = 0;
	const char* p = input;
	const char* pe = p + size;
	const char* eof = tsk_null;

	(void)(eof);

	if(!input || !size){
		TSK_DEBUG_ERROR("Null or empty buffer.");
		goto bail;
	}

	if(!(sdp_msg = tsdp_message_create())){
		goto bail;
	}

	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	/* Ragel init */
	%% write init;

	/* Ragel execute */
	%% write exec;
	TSK_RAGEL_DISABLE_WARNINGS_END()

	/* Check result */
	if( cs < %%{ write first_final; }%% )
	{
		TSK_DEBUG_ERROR("Failed to parse SDP message.");
		TSK_OBJECT_SAFE_FREE(sdp_msg);
		goto bail;
	}
	
bail:
	return sdp_msg;
}
