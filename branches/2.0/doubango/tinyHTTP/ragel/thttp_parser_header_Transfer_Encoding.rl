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

/**@file thttp_header_Transfer_Encoding.c
 * @brief HTTP Transfer-Encoding header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinyhttp/headers/thttp_header_Transfer_Encoding.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>

/***********************************
*	Ragel state machine.
*/
%%{
	machine thttp_machine_parser_header_Transfer_Encoding;

	# Includes
	include thttp_machine_utils "./ragel/thttp_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_encoding{
		TSK_PARSER_SET_STRING(hdr_tencoding->encoding);
	}

	action parse_param{		
		TSK_PARSER_ADD_PARAM(THTTP_HEADER_PARAMS(hdr_tencoding));
	}

	action eob{
	}

	Transfer_Encoding = ( "Transfer-Encoding"i ) HCOLON token>tag %parse_encoding :>( ";" (pname("=" pvalue )?)>tag %parse_param )*;
	
	# Entry point
	main := Transfer_Encoding :>CRLF @eob;

}%%

thttp_header_Transfer_Encoding_t* thttp_header_transfer_encoding_create(const char* encoding)
{
	return tsk_object_new(THTTP_HEADER_TRANSFER_ENCODING_VA_ARGS(encoding));
}

thttp_header_Transfer_Encoding_t* thttp_header_transfer_encoding_create_null()
{
	return thttp_header_transfer_encoding_create(tsk_null);
}


int thttp_header_Transfer_Encoding_tostring(const thttp_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const thttp_header_Transfer_Encoding_t *Transfer_Encoding = (const thttp_header_Transfer_Encoding_t*)header;
		if(Transfer_Encoding->encoding){
			return tsk_buffer_append(output, Transfer_Encoding->encoding, tsk_strlen(Transfer_Encoding->encoding));
		}
		return 0;
	}

	return -1;
}

/**@ingroup thttp_header_group
*/
thttp_header_Transfer_Encoding_t *thttp_header_Transfer_Encoding_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	thttp_header_Transfer_Encoding_t *hdr_tencoding = thttp_header_transfer_encoding_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(thttp_machine_parser_header_Transfer_Encoding_first_final);
	(void)(thttp_machine_parser_header_Transfer_Encoding_error);
	(void)(thttp_machine_parser_header_Transfer_Encoding_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse Tansfer-Encoding header.");
		TSK_OBJECT_SAFE_FREE(hdr_tencoding);
	}
	
	return hdr_tencoding;
}







//========================================================
//	Transfer_Encoding header object definition
//

static tsk_object_t* thttp_header_Transfer_Encoding_ctor(tsk_object_t *self, va_list * app)
{
	thttp_header_Transfer_Encoding_t *Transfer_Encoding = self;
	if(Transfer_Encoding){
		THTTP_HEADER(Transfer_Encoding)->type = thttp_htype_Transfer_Encoding;
		THTTP_HEADER(Transfer_Encoding)->tostring = thttp_header_Transfer_Encoding_tostring;

		Transfer_Encoding->encoding = tsk_strdup( va_arg(*app, const char*) );
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Transfer_Encoding header.");
	}
	return self;
}

static tsk_object_t* thttp_header_Transfer_Encoding_dtor(tsk_object_t* self)
{
	thttp_header_Transfer_Encoding_t *Transfer_Encoding = self;
	if(Transfer_Encoding){
		TSK_FREE(Transfer_Encoding->encoding);
		TSK_OBJECT_SAFE_FREE(THTTP_HEADER_PARAMS(Transfer_Encoding));
	}
	else{
		TSK_DEBUG_ERROR("Null Transfer_Encoding header.");
	}

	return self;
}

static const tsk_object_def_t thttp_header_Transfer_Encoding_def_s = 
{
	sizeof(thttp_header_Transfer_Encoding_t),
	thttp_header_Transfer_Encoding_ctor,
	thttp_header_Transfer_Encoding_dtor,
	tsk_null
};
const tsk_object_def_t *thttp_header_Transfer_Encoding_def_t = &thttp_header_Transfer_Encoding_def_s;
