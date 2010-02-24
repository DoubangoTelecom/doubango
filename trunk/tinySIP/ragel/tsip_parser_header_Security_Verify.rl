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

/**@file tsip_header_Security_Verify.c
 * @brief SIP Security-Verify header as per RFC 3329.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Security_Verify.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>

/**@defgroup tsip_header_Security_Verify_group SIP Security_Verify header.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Security_Verify;

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";
	
	action tag
	{
		tag_start = p;
	}
	
	action create_securityverify
	{
		if(!curr_securityverify)
		{
			curr_securityverify = TSIP_HEADER_SECURITY_VERIFY_CREATE_NULL();
		}
	}

	action add_securityverify
	{
		if(curr_securityverify)
		{
			tsk_list_push_back_data(hdr_securityverifies, ((void**) &curr_securityverify));
		}
	}
	
	action parse_mech
	{
		if(curr_securityverify)
		{
			TSK_PARSER_SET_STRING(curr_securityverify->mech);
		}
	}

	action parse_port_s
	{
		if(curr_securityverify)
		{
			TSK_PARSER_SET_INT(curr_securityverify->port_s);
		}
	}

	action parse_port_c
	{
		if(curr_securityverify)
		{
			TSK_PARSER_SET_INT(curr_securityverify->port_c);
		}
	}

	action parse_spi_s
	{
		if(curr_securityverify)
		{
			TSK_PARSER_SET_UINT(curr_securityverify->spi_s);
		}
	}

	action parse_spi_c
	{
		if(curr_securityverify)
		{
			TSK_PARSER_SET_UINT(curr_securityverify->spi_c);
		}
	}

	action parse_ealg
	{
		if(curr_securityverify)
		{
			TSK_PARSER_SET_STRING(curr_securityverify->ealg);
		}
	}

	action parse_alg
	{
		if(curr_securityverify)
		{
			TSK_PARSER_SET_STRING(curr_securityverify->alg);
		}
	}

	action parse_prot
	{
		if(curr_securityverify)
		{
			TSK_PARSER_SET_STRING(curr_securityverify->prot);
		}
	}

	action parse_preference
	{
		if(curr_securityverify)
		{
			TSK_PARSER_SET_DOUBLE(curr_securityverify->q);
		}
	}

	action parse_param
	{
		if(curr_securityverify)
		{
			TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_securityverify));
		}
	}



	action eob
	{
	}

	mech_extension = generic_param>tag %parse_param;
	port_s = "port-s"i EQUAL DIGIT+>tag %parse_port_s;
	port_c = "port-c"i EQUAL DIGIT+>tag %parse_port_c;
	spi_s = "spi-s"i EQUAL DIGIT+>tag %parse_spi_s;
	spi_c = "spi-c"i EQUAL DIGIT+>tag %parse_spi_c;
	ealg = "ealg"i EQUAL token>tag %parse_ealg;
	alg = "alg"i EQUAL token>tag %parse_alg;
	prot = "prot"i EQUAL token>tag %parse_prot;
	preference = "q"i EQUAL qvalue>tag %parse_preference;
	mech_parameters = (preference | prot | alg | ealg | spi_c | spi_s | port_c | port_s) @1 | mech_extension @0;
	mechanism_name = token>tag %parse_mech;
	sec_mechanism = (mechanism_name ( SEMI mech_parameters )*) >create_securityverify %add_securityverify;
	Security_Verify = "Security-Verify"i HCOLON sec_mechanism ( COMMA sec_mechanism )*;

	# Entry point
	main := Security_Verify :>CRLF @eob;

}%%

int tsip_header_Security_Verify_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Security_Verify_t *Security_Verify = header;
		int ret = 0;
		
		// ipsec-3gpp; alg=hmac-md5-96; ealg=des-ede3-cbc; spi-c=1111; spi-s=2222; port-c=5062; port-s=5064
		if(tsk_striequals(Security_Verify->mech, "ipsec-3gpp"))
		{
			tsk_buffer_appendEx(output, "%s%s%s%s%s%s%s;spi-c=%u;spi-s=%u;port-c=%u;port-s=%u", 
				Security_Verify->mech,
				
				Security_Verify->alg ? ";alg=" : "",
				Security_Verify->alg ? Security_Verify->alg : "",
				
				Security_Verify->ealg ? ";ealg=" : "",
				Security_Verify->ealg ? Security_Verify->ealg : "",

				Security_Verify->prot ? ";prot=" : "",
				Security_Verify->prot ? Security_Verify->prot : "",
				
				Security_Verify->spi_c,
				Security_Verify->spi_s,
				Security_Verify->port_c,
				Security_Verify->port_s
				);
		}
		else if(Security_Verify->mech){
			tsk_buffer_append(output, Security_Verify->mech, strlen(Security_Verify->mech));
		}

		if(Security_Verify->q >= 0){
			/* qvalue	=  	("0" [ "." 0*3DIGIT ] ) / ( "1" [ "." 0*3("0") ] ) */
			tsk_buffer_appendEx(output, ";q=%1.3f", Security_Verify->q);
		}
		
		return ret;
	}

	return -1;
}

tsip_header_Security_Verifies_L_t *tsip_header_Security_Verify_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Security_Verifies_L_t *hdr_securityverifies = TSK_LIST_CREATE();
	
	const char *tag_start;
	tsip_header_Security_Verify_t *curr_securityverify = 0;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% )
	{
		TSK_OBJECT_SAFE_FREE(curr_securityverify);
		TSK_OBJECT_SAFE_FREE(hdr_securityverifies);
	}
	
	return hdr_securityverifies;
}





//========================================================
//	Security_Verify header object definition
//

/**@ingroup tsip_header_Security_Verify_group
*/
static void* tsip_header_Security_Verify_create(void *self, va_list * app)
{
	tsip_header_Security_Verify_t *Security_Verify = self;
	if(Security_Verify)
	{

		TSIP_HEADER(Security_Verify)->type = tsip_htype_Security_Verify;
		TSIP_HEADER(Security_Verify)->tostring = tsip_header_Security_Verify_tostring;

		Security_Verify->q = -1;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Security_Verify header.");
	}
	return self;
}

/**@ingroup tsip_header_Security_Verify_group
*/
static void* tsip_header_Security_Verify_destroy(void *self)
{
	tsip_header_Security_Verify_t *Security_Verify = self;
	if(Security_Verify)
	{
		TSK_FREE(Security_Verify->mech);
		TSK_FREE(Security_Verify->alg);
		TSK_FREE(Security_Verify->prot);
		TSK_FREE(Security_Verify->mod);
		TSK_FREE(Security_Verify->ealg);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Security_Verify));
	}
	else TSK_DEBUG_ERROR("Null Security_Verify header.");

	return self;
}

static const tsk_object_def_t tsip_header_Security_Verify_def_s = 
{
	sizeof(tsip_header_Security_Verify_t),
	tsip_header_Security_Verify_create,
	tsip_header_Security_Verify_destroy,
	0
};
const void *tsip_header_Security_Verify_def_t = &tsip_header_Security_Verify_def_s;

