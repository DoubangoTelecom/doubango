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

/**@file tsip_header_Subscription_State.c
 * @brief SIP Subscription_State header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Subscription_State.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Subscription_State;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_state{
		TSK_PARSER_SET_STRING(hdr_Subscription_State->state);
	}

	action parse_reason{
		TSK_PARSER_SET_STRING(hdr_Subscription_State->reason);
	}

	action parse_expires{
		TSK_PARSER_SET_INTEGER(hdr_Subscription_State->expires);
	}

	action parse_retry_after{
		TSK_PARSER_SET_INTEGER(hdr_Subscription_State->retry_after);
	}

	action parse_param{
		TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(hdr_Subscription_State));
	}

	action eob{
	}
	
	subexp_params = (( "reason"i EQUAL token>tag %parse_reason ) | ( "expires"i EQUAL delta_seconds>tag %parse_expires ) | ( "retry-after"i EQUAL delta_seconds>tag %parse_retry_after ))@1 | generic_param>tag %parse_param @0;
	Subscription_State = ( "Subscription-State"i ) HCOLON token>tag %parse_state ( SEMI subexp_params )*;
	
	# Entry point
	main := Subscription_State :>CRLF @eob;

}%%

tsip_header_Subscription_State_t* tsip_header_Subscription_State_create()
{
	return tsk_object_new(tsip_header_Subscription_State_def_t);
}


int tsip_header_Subscription_State_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Subscription_State_t *Subscription_State = (const tsip_header_Subscription_State_t *)header;
		int ret;
		
		ret = tsk_buffer_append_2(output, "%s%s%s", 
			Subscription_State->state,
			
			Subscription_State->reason ? ";reason=" : "",
			Subscription_State->reason ? Subscription_State->reason : ""				
			);
		if(!ret && Subscription_State->expires>=0){
			ret = tsk_buffer_append_2(output, ";expires=%d", Subscription_State->expires);
		}
		if(!ret && Subscription_State->retry_after>=0){
			ret = tsk_buffer_append_2(output, ";retry-after=%d", Subscription_State->retry_after);
		}

		return ret;
	}

	return -1;
}

tsip_header_Subscription_State_t *tsip_header_Subscription_State_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Subscription_State_t *hdr_Subscription_State = tsip_header_Subscription_State_create();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_Subscription_State_first_final);
	(void)(tsip_machine_parser_header_Subscription_State_error);
	(void)(tsip_machine_parser_header_Subscription_State_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Subscription-State' header.");
		TSK_OBJECT_SAFE_FREE(hdr_Subscription_State);
	}
	
	return hdr_Subscription_State;
}







//========================================================
//	Subscription_State header object definition
//

static tsk_object_t* tsip_header_Subscription_State_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Subscription_State_t *Subscription_State = self;
	if(Subscription_State){
		TSIP_HEADER(Subscription_State)->type = tsip_htype_Subscription_State;
		TSIP_HEADER(Subscription_State)->serialize = tsip_header_Subscription_State_serialize;

		Subscription_State->expires = -1;
		Subscription_State->retry_after = -1;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Subscription_State header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Subscription_State_dtor(tsk_object_t *self)
{
	tsip_header_Subscription_State_t *Subscription_State = self;
	if(Subscription_State){
		TSK_FREE(Subscription_State->state);
		TSK_FREE(Subscription_State->reason);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Subscription_State));
	}
	else{
		TSK_DEBUG_ERROR("Null Subscription_State header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Subscription_State_def_s = 
{
	sizeof(tsip_header_Subscription_State_t),
	tsip_header_Subscription_State_ctor,
	tsip_header_Subscription_State_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Subscription_State_def_t = &tsip_header_Subscription_State_def_s;
