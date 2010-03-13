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
/**@file tsdp_machine_message.rl
 * @brief Ragel file.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinySDP/parsers/tsdp_parser_message.h"

#include "tinySDP/headers/tsdp_header_B.h"
#include "tinySDP/headers/tsdp_header_C.h"
#include "tinySDP/headers/tsdp_header_Dummy.h"
#include "tinySDP/headers/tsdp_header_E.h"
#include "tinySDP/headers/tsdp_header_I.h"
#include "tinySDP/headers/tsdp_header_K.h"
#include "tinySDP/headers/tsdp_header_O.h"
#include "tinySDP/headers/tsdp_header_P.h"
#include "tinySDP/headers/tsdp_header_R.h"
#include "tinySDP/headers/tsdp_header_S.h"
#include "tinySDP/headers/tsdp_header_T.h"
#include "tinySDP/headers/tsdp_header_U.h"
#include "tinySDP/headers/tsdp_header_V.h"
#include "tinySDP/headers/tsdp_header_Z.h"

#include "tsk_debug.h"

%%{
	machine tsdp_machine_message;
	
	###########################################
	#	Includes
	###########################################
	include tsdp_machine_utils "./tsdp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	###########################################
	#	Actions
	###########################################
	action parse_header_A{
		TSK_DEBUG_INFO("Header A");
	}

	action parse_header_B{
		if((header = (tsdp_header_t*)tsdp_header_B_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header B");
	}
	
	action parse_header_C{
		if((header = (tsdp_header_t*)tsdp_header_C_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header C");
	}
	
	action parse_header_Dummy{
		if((header = (tsdp_header_t*)tsdp_header_Dummy_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header Dummy");
	}
	
	action parse_header_E{
		if((header = (tsdp_header_t*)tsdp_header_E_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header E");
	}
	
	action parse_header_I{
		if((header = (tsdp_header_t*)tsdp_header_I_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header I");
	}
	
	action parse_header_K{
		if((header = (tsdp_header_t*)tsdp_header_K_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header K");
	}
	
	action parse_header_M{
		TSK_DEBUG_INFO("Header M");
	}
	
	action parse_header_O{
		if((header = (tsdp_header_t*)tsdp_header_O_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header O");
	}
	
	action parse_header_P{
		if((header = (tsdp_header_t*)tsdp_header_P_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header P");
	}

	action parse_header_R{
		if((header = (tsdp_header_t*)tsdp_header_R_parse(tag_start, (p - tag_start)))){
			if(hdr_T){
				if(!hdr_T->repeat_fields){
					hdr_T->repeat_fields = TSK_LIST_CREATE();
				}
				tsk_list_push_back_data(hdr_T->repeat_fields, (void**)&header);
			}
			else{
				tsdp_message_add_header(sdp_msg, header);
				tsk_object_unref(header);
			}
		}
		TSK_DEBUG_INFO("Header R");
	}
	
	action parse_header_S{
		if((header = (tsdp_header_t*)tsdp_header_S_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header S");
	}
	
	action parse_header_T{
		if((hdr_T = tsdp_header_T_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, TSDP_HEADER(hdr_T));
			hdr_T = tsk_object_unref(hdr_T);
		}
		TSK_DEBUG_INFO("Header T");
	}

	action parse_header_U{
		if((header = (tsdp_header_t*)tsdp_header_U_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header U");
	}

	action parse_header_V{
		if((header = (tsdp_header_t*)tsdp_header_V_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header V");
	}

	action parse_header_Z{
		if((header = (tsdp_header_t*)tsdp_header_Z_parse(tag_start, (p - tag_start)))){
			tsdp_message_add_header(sdp_msg, header);
			tsk_object_unref(header);
		}
		TSK_DEBUG_INFO("Header Z");
	}

	###########################################
	#	Headers
	###########################################

	A = "a"i SP* "=" SP*<: any* :>CRLF  %parse_header_A;
	B = "b"i SP* "=" SP*<: any* :>CRLF  %parse_header_B;
	C = "c"i SP* "=" SP*<: any* :>CRLF  %parse_header_C;
	E = "e"i SP* "=" SP*<: any* :>CRLF  %parse_header_E;
	I = "i"i SP* "=" SP*<: any* :>CRLF  %parse_header_I;
	K = "k"i SP* "=" SP*<: any* :>CRLF  %parse_header_K;
	M = "m"i SP* "=" SP*<: any* :>CRLF  %parse_header_M;
	O = "o"i SP* "=" SP*<: any* :>CRLF  %parse_header_O;
	P = "p"i SP* "=" SP*<: any* :>CRLF  %parse_header_P;
	R = "r"i SP* "=" SP*<: any* :>CRLF  %parse_header_R;
	S = "s"i SP* "=" SP*<: any* :>CRLF  %parse_header_S;
	T = "t"i SP* "=" SP*<: any* :>CRLF  %parse_header_T;
	U = "u"i SP* "=" SP*<: any* :>CRLF  %parse_header_U;
	V = "v"i SP* "=" SP*<: any* :>CRLF  %parse_header_V;
	Z = "z"i SP* "=" SP*<: any* :>CRLF  %parse_header_Z;

	Dummy = alpha SP* "=" SP*<: any* :>CRLF  %parse_header_Dummy;

	Header = (A | B | C | E | I | K | M | O | P | R | S | T | U | V | Z)>tag @1 | (Dummy>tag) @0;


	###########################################
	#	Message
	###########################################
	SDP_message = Header*;

	###########################################
	#	Entry Point
	###########################################
	main := SDP_message;
}%%

/* Ragel data */
%% write data;

tsdp_message_t* tsdp_message_parse(const void *input, size_t size)
{
	tsdp_message_t* sdp_msg = TSDP_NULL;
	const char* tag_start = TSDP_NULL;
	tsdp_header_t *header = TSDP_NULL;
	tsdp_header_T_t *hdr_T = TSDP_NULL;

	/* Ragel variables */
	int cs = 0;
	const char* p = input;
	const char* pe = p + size;
	const char* eof = TSDP_NULL;

	if(!input || !size){
		TSK_DEBUG_ERROR("Null or empty buffer.");
		goto bail;
	}

	if(!(sdp_msg = TSDP_MESSAGE_CREATE())){
		goto bail;
	}

	/* Ragel init */
	%% write init;

	/* Ragel execute */
	%% write exec;

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