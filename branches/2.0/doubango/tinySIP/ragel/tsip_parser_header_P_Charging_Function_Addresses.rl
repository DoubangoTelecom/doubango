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

/**@file tsip_header_P_Charging_Function_Addresses.c
 * @brief SIP P-Charging-Function-Addresses header as per RFC 3455.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_P_Charging_Function_Addresses.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_P_Charging_Function_Addresses;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action create_p_charging_function_addresses{
		if(!curr_p_charging_function_addresses){
			curr_p_charging_function_addresses = tsip_header_P_Charging_Function_Addresses_create();
		}
	}

	action parse_ccf{
		if(!curr_p_charging_function_addresses->ccf){
			TSK_PARSER_SET_STRING(curr_p_charging_function_addresses->ccf);
		}
	}

	action parse_ecf{
		if(!curr_p_charging_function_addresses->ecf){
			TSK_PARSER_SET_STRING(curr_p_charging_function_addresses->ecf);
		}
	}

	action parse_param{
		if(curr_p_charging_function_addresses){
			TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_p_charging_function_addresses));
		}
	}

	action add_p_charging_function_addresses{
		if(curr_p_charging_function_addresses){
			tsk_list_push_back_data(hdr_p_charging_function_addressess, ((void**) &curr_p_charging_function_addresses));
		}
	}

	action eob{
	}
	
	ccf = "ccf"i EQUAL gen_value;
	ecf = "ecf"i EQUAL gen_value;
	charge_addr_params = (ccf>tag %parse_ccf | ecf>tag %parse_ecf | generic_param>tag %parse_param) >create_p_charging_function_addresses %add_p_charging_function_addresses;
	P_Charging_Addr = "P-Charging-Function-Addresses"i HCOLON charge_addr_params ( SEMI charge_addr_params )*;
	P_Charging_Function_Addresses = P_Charging_Addr;

	# Entry point
	main := P_Charging_Function_Addresses :>CRLF @eob;

}%%


tsip_header_P_Charging_Function_Addresses_t* tsip_header_P_Charging_Function_Addresses_create()
{
	return tsk_object_new(tsip_header_P_Charging_Function_Addresses_def_t);
}

int tsip_header_P_Charging_Function_Addresses_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_P_Charging_Function_Addresses_t *P_Charging_Function_Addresses = (const tsip_header_P_Charging_Function_Addresses_t *)header;		
		return tsk_buffer_append_2(output, "%s%s%s%s%s", 
			P_Charging_Function_Addresses->ecf ? "ecf=" : "",
			P_Charging_Function_Addresses->ecf ? P_Charging_Function_Addresses->ecf : "",

			(P_Charging_Function_Addresses->ecf && P_Charging_Function_Addresses->ccf) ? ";" : "",

			P_Charging_Function_Addresses->ccf ? "ccf=" : "",
			P_Charging_Function_Addresses->ccf ? P_Charging_Function_Addresses->ccf : ""
				);
	}

	return -1;
}

tsip_header_P_Charging_Function_Addressess_L_t *tsip_header_P_Charging_Function_Addresses_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_P_Charging_Function_Addressess_L_t *hdr_p_charging_function_addressess = tsk_list_create();
	
	const char *tag_start = tsk_null;
	tsip_header_P_Charging_Function_Addresses_t *curr_p_charging_function_addresses = 0;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_P_Charging_Function_Addresses_first_final);
	(void)(tsip_machine_parser_header_P_Charging_Function_Addresses_error);
	(void)(tsip_machine_parser_header_P_Charging_Function_Addresses_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'P-Charging-Function-Addresses' header.");
		TSK_OBJECT_SAFE_FREE(curr_p_charging_function_addresses);
		TSK_OBJECT_SAFE_FREE(hdr_p_charging_function_addressess);
	}
	
	return hdr_p_charging_function_addressess;
}





//========================================================
//	P_Charging_Function_Addresses header object definition
//

static tsk_object_t* tsip_header_P_Charging_Function_Addresses_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_P_Charging_Function_Addresses_t *P_Charging_Function_Addresses = self;
	if(P_Charging_Function_Addresses){
		TSIP_HEADER(P_Charging_Function_Addresses)->type = tsip_htype_P_Charging_Function_Addresses;
		TSIP_HEADER(P_Charging_Function_Addresses)->serialize = tsip_header_P_Charging_Function_Addresses_serialize;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new P_Charging_Function_Addresses header.");
	}
	return self;
}

static tsk_object_t* tsip_header_P_Charging_Function_Addresses_dtor(tsk_object_t *self)
{
	tsip_header_P_Charging_Function_Addresses_t *P_Charging_Function_Addresses = self;
	if(P_Charging_Function_Addresses){
		TSK_FREE(P_Charging_Function_Addresses->ecf);
		TSK_FREE(P_Charging_Function_Addresses->ccf);
		
		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(P_Charging_Function_Addresses));
	}
	else{
		TSK_DEBUG_ERROR("Null P_Charging_Function_Addresses header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_P_Charging_Function_Addresses_def_s = 
{
	sizeof(tsip_header_P_Charging_Function_Addresses_t),
	tsip_header_P_Charging_Function_Addresses_ctor,
	tsip_header_P_Charging_Function_Addresses_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_P_Charging_Function_Addresses_def_t = &tsip_header_P_Charging_Function_Addresses_def_s;
