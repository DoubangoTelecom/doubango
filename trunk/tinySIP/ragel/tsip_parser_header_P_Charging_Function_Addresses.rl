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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsip_header_P_Charging_Function_Addresses.c
 * @brief SIP P-Charging-Function-Addresses header as per RFC 3455.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_P_Charging_Function_Addresses.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_P_Charging_Function_Addresses_group SIP P_Charging_Function_Addresses header.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_P_Charging_Function_Addresses;

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";
	
	action tag
	{
		tag_start = p;
	}
	
	action create_p_charging_function_addresses
	{
		if(!curr_p_charging_function_addresses)
		{
			curr_p_charging_function_addresses = TSIP_HEADER_P_CHARGING_FUNCTION_ADDRESSES_CREATE();
		}
	}

	action parse_ccf
	{
		if(!curr_p_charging_function_addresses->ccf)
		{
			TSK_PARSER_SET_STRING(curr_p_charging_function_addresses->ccf);
		}
	}

	action parse_ecf
	{
		if(!curr_p_charging_function_addresses->ecf)
		{
			TSK_PARSER_SET_STRING(curr_p_charging_function_addresses->ecf);
		}
	}

	action parse_param
	{
		if(curr_p_charging_function_addresses)
		{
			TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_p_charging_function_addresses));
		}
	}

	action add_p_charging_function_addresses
	{
		if(curr_p_charging_function_addresses)
		{
			tsk_list_push_back_data(hdr_p_charging_function_addressess, ((void**) &curr_p_charging_function_addresses));
		}
	}

	action eob
	{
	}
	
	ccf = "ccf"i EQUAL gen_value;
	ecf = "ecf"i EQUAL gen_value;
	charge_addr_params = (ccf>tag %parse_ccf | ecf>tag %parse_ecf | generic_param>tag %parse_param) >create_p_charging_function_addresses %add_p_charging_function_addresses;
	P_Charging_Addr = "P-Charging-Function-Addresses"i HCOLON charge_addr_params ( SEMI charge_addr_params )*;
	P_Charging_Function_Addresses = P_Charging_Addr;

	# Entry point
	main := P_Charging_Function_Addresses :>CRLF @eob;

}%%

int tsip_header_P_Charging_Function_Addresses_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_P_Charging_Function_Addresses_t *P_Charging_Function_Addresses = header;		
		return tsk_buffer_appendEx(output, "%s%s%s%s%s", 
			P_Charging_Function_Addresses->ecf ? "ecf=" : "",
			P_Charging_Function_Addresses->ecf ? P_Charging_Function_Addresses->ecf : "",

			(P_Charging_Function_Addresses->ecf && P_Charging_Function_Addresses->ccf) ? ";" : "",

			P_Charging_Function_Addresses->ccf ? "ccf=" : "",
			P_Charging_Function_Addresses->ccf ? P_Charging_Function_Addresses->ccf : ""
				);
	}

	return -1;
}

tsip_header_P_Charging_Function_Addressess_L_t *tsip_header_P_Charging_Function_Addresses_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_P_Charging_Function_Addressess_L_t *hdr_p_charging_function_addressess = TSK_LIST_CREATE();
	
	const char *tag_start;
	tsip_header_P_Charging_Function_Addresses_t *curr_p_charging_function_addresses = 0;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% )
	{
		TSK_OBJECT_SAFE_FREE(curr_p_charging_function_addresses);
		TSK_OBJECT_SAFE_FREE(hdr_p_charging_function_addressess);
	}
	
	return hdr_p_charging_function_addressess;
}





//========================================================
//	P_Charging_Function_Addresses header object definition
//

/**@ingroup tsip_header_P_Charging_Function_Addresses_group
*/
static void* tsip_header_P_Charging_Function_Addresses_create(void *self, va_list * app)
{
	tsip_header_P_Charging_Function_Addresses_t *P_Charging_Function_Addresses = self;
	if(P_Charging_Function_Addresses)
	{
		TSIP_HEADER(P_Charging_Function_Addresses)->type = tsip_htype_P_Charging_Function_Addresses;
		TSIP_HEADER(P_Charging_Function_Addresses)->tostring = tsip_header_P_Charging_Function_Addresses_tostring;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new P_Charging_Function_Addresses header.");
	}
	return self;
}

/**@ingroup tsip_header_P_Charging_Function_Addresses_group
*/
static void* tsip_header_P_Charging_Function_Addresses_destroy(void *self)
{
	tsip_header_P_Charging_Function_Addresses_t *P_Charging_Function_Addresses = self;
	if(P_Charging_Function_Addresses)
	{
		TSK_FREE(P_Charging_Function_Addresses->ecf);
		TSK_FREE(P_Charging_Function_Addresses->ccf);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(P_Charging_Function_Addresses));
	}
	else TSK_DEBUG_ERROR("Null P_Charging_Function_Addresses header.");

	return self;
}

static const tsk_object_def_t tsip_header_P_Charging_Function_Addresses_def_s = 
{
	sizeof(tsip_header_P_Charging_Function_Addresses_t),
	tsip_header_P_Charging_Function_Addresses_create,
	tsip_header_P_Charging_Function_Addresses_destroy,
	0
};
const void *tsip_header_P_Charging_Function_Addresses_def_t = &tsip_header_P_Charging_Function_Addresses_def_s;