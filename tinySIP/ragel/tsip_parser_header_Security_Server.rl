
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

/**@file tsip_header_Security_Server.c
 * @brief SIP Security-Server header as per RFC 3329.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Security_Server.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Security_Server;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action create_securityserver{
		if(!curr_securityserver){
			curr_securityserver = tsip_header_Security_Server_create_null();
		}
	}

	action add_securityserver{
		if(curr_securityserver){
			tsk_list_push_back_data(hdr_securityservers, ((void**) &curr_securityserver));
		}
	}
	
	action parse_mech{
		if(curr_securityserver){
			TSK_PARSER_SET_STRING(curr_securityserver->mech);
		}
	}

	action parse_port_s{
		if(curr_securityserver){
			TSK_PARSER_SET_INT(curr_securityserver->port_s);
		}
	}

	action parse_port_c{
		if(curr_securityserver){
			TSK_PARSER_SET_INT(curr_securityserver->port_c);
		}
	}

	action parse_spi_s{
		if(curr_securityserver){
			TSK_PARSER_SET_UINT(curr_securityserver->spi_s);
		}
	}

	action parse_spi_c{
		if(curr_securityserver){
			TSK_PARSER_SET_UINT(curr_securityserver->spi_c);
		}
	}

	action parse_ealg{
		if(curr_securityserver){
			TSK_PARSER_SET_STRING(curr_securityserver->ealg);
		}
	}

	action parse_alg{
		if(curr_securityserver){
			TSK_PARSER_SET_STRING(curr_securityserver->alg);
		}
	}

	action parse_prot{
		if(curr_securityserver){
			TSK_PARSER_SET_STRING(curr_securityserver->prot);
		}
	}

	action parse_preference{
		if(curr_securityserver){
			TSK_PARSER_SET_DOUBLE(curr_securityserver->q);
		}
	}

	action parse_param{
		if(curr_securityserver){
			TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_securityserver));
		}
	}

	action eob{
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
	sec_mechanism = (mechanism_name ( SEMI mech_parameters )*) >create_securityserver %add_securityserver;
	Security_Server = "Security-Server"i HCOLON sec_mechanism ( COMMA sec_mechanism )*;

	# Entry point
	main := Security_Server :>CRLF @eob;

}%%

tsip_header_Security_Server_t* tsip_header_Security_Server_create()
{
	return tsk_object_new(TSIP_HEADER_SECURITY_SERVER_VA_ARGS());
}

tsip_header_Security_Server_t* tsip_header_Security_Server_create_null()
{
	return tsip_header_Security_Server_create();
}

int tsip_header_Security_Server_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Security_Server_t *Security_Server = (const tsip_header_Security_Server_t *)header;
		int ret = 0;
		
		// ipsec-3gpp; alg=hmac-md5-96; ealg=des-ede3-cbc; spi-c=1111; spi-s=2222; port-c=5062; port-s=5064
		if(tsk_striequals(Security_Server->mech, "ipsec-3gpp")){
			ret = tsk_buffer_append_2(output, "%s%s%s%s%s%s%s;spi-c=%u;spi-s=%u;port-c=%u;port-s=%u", 
				Security_Server->mech,
				
				Security_Server->alg ? ";alg=" : "",
				Security_Server->alg ? Security_Server->alg : "",
				
				Security_Server->ealg ? ";ealg=" : "",
				Security_Server->ealg ? Security_Server->ealg : "",

				Security_Server->prot ? ";prot=" : "",
				Security_Server->prot ? Security_Server->prot : "",
				
				Security_Server->spi_c,
				Security_Server->spi_s,
				Security_Server->port_c,
				Security_Server->port_s
				);
		}
		else if(Security_Server->mech){
			ret = tsk_buffer_append(output, Security_Server->mech, tsk_strlen(Security_Server->mech));
		}

		if(Security_Server->q >= 0){
			/* qvalue	=  	("0" [ "." 0*3DIGIT ] ) / ( "1" [ "." 0*3("0") ] ) */
			ret = tsk_buffer_append_2(output, ";q=%1.3f", Security_Server->q);
		}
		
		return ret;
	}

	return -1;
}


tsip_header_Security_Servers_L_t *tsip_header_Security_Server_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Security_Servers_L_t *hdr_securityservers = tsk_list_create();
	
	const char *tag_start = tsk_null;
	tsip_header_Security_Server_t *curr_securityserver = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_Security_Server_first_final);
	(void)(tsip_machine_parser_header_Security_Server_error);
	(void)(tsip_machine_parser_header_Security_Server_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Security-Server' header.");
		TSK_OBJECT_SAFE_FREE(curr_securityserver);
		TSK_OBJECT_SAFE_FREE(hdr_securityservers);
	}
	
	return hdr_securityservers;
}





//========================================================
//	Security_Server header object definition
//

static tsk_object_t* tsip_header_Security_Server_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Security_Server_t *Security_Server = self;
	if(Security_Server){

		TSIP_HEADER(Security_Server)->type = tsip_htype_Security_Server;
		TSIP_HEADER(Security_Server)->serialize = tsip_header_Security_Server_serialize;

		Security_Server->q = -1;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Security_Server header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Security_Server_dtor(tsk_object_t *self)
{
	tsip_header_Security_Server_t *Security_Server = self;
	if(Security_Server){
		TSK_FREE(Security_Server->mech);
		TSK_FREE(Security_Server->alg);
		TSK_FREE(Security_Server->prot);
		TSK_FREE(Security_Server->mod);
		TSK_FREE(Security_Server->ealg);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Security_Server));
	}
	else{
		TSK_DEBUG_ERROR("Null Security_Server header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Security_Server_def_s = 
{
	sizeof(tsip_header_Security_Server_t),
	tsip_header_Security_Server_ctor,
	tsip_header_Security_Server_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Security_Server_def_t = &tsip_header_Security_Server_def_s;
