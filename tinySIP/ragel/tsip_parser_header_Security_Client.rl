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

/**@file tsip_header_Security_Client.c
 * @brief SIP Security-Client header as per RFC 3329.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Security_Client.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_time.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Security_Client;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action create_securityclient{
		if(!curr_securityclient){
			curr_securityclient = tsip_header_Security_Client_create_null();
		}
	}

	action add_securityclient{
		if(curr_securityclient){
			tsk_list_push_back_data(hdr_securityclients, ((void**) &curr_securityclient));
		}
	}
	
	action parse_mech{
		if(curr_securityclient){
			TSK_PARSER_SET_STRING(curr_securityclient->mech);
		}
	}

	action parse_port_s{
		if(curr_securityclient){
			TSK_PARSER_SET_INT(curr_securityclient->port_s);
		}
	}

	action parse_port_c{
		if(curr_securityclient){
			TSK_PARSER_SET_INT(curr_securityclient->port_c);
		}
	}

	action parse_spi_s{
		if(curr_securityclient){
			TSK_PARSER_SET_UINT(curr_securityclient->spi_s);
		}
	}

	action parse_spi_c{
		if(curr_securityclient){
			TSK_PARSER_SET_UINT(curr_securityclient->spi_c);
		}
	}

	action parse_ealg{
		if(curr_securityclient){
			TSK_PARSER_SET_STRING(curr_securityclient->ealg);
		}
	}

	action parse_alg{
		if(curr_securityclient){
			TSK_PARSER_SET_STRING(curr_securityclient->alg);
		}
	}

	action parse_prot{
		if(curr_securityclient){
			TSK_PARSER_SET_STRING(curr_securityclient->prot);
		}
	}

	action parse_preference{
		if(curr_securityclient){
			TSK_PARSER_SET_DOUBLE(curr_securityclient->q);
		}
	}

	action parse_param{
		if(curr_securityclient){
			TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_securityclient));
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
	mech_parameters = (preference |prot | alg | ealg | spi_c | spi_s | port_c | port_s) @1 | mech_extension @0;
	mechanism_name = token>tag %parse_mech;
	sec_mechanism = (mechanism_name ( SEMI mech_parameters )*) >create_securityclient %add_securityclient;
	Security_Client = "Security-Client"i HCOLON sec_mechanism ( COMMA sec_mechanism )*;

	# Entry point
	main := Security_Client :>CRLF @eob;

}%%

tsip_header_Security_Client_t* tsip_header_Security_Client_create(const char* mech, const char* alg, const char* prot, const char* mod, const char* ealg, tnet_port_t port_c, tnet_port_t port_s, uint32_t spi_c, uint32_t spi_s)
{
	return tsk_object_new(TSIP_HEADER_SECURITY_CLIENT_VA_ARGS(mech, alg, prot, mod, ealg, port_c, port_s, spi_c, spi_s));
}

tsip_header_Security_Client_t* tsip_header_Security_Client_create_null()
{
	return tsip_header_Security_Client_create(tsk_null, tsk_null, tsk_null, tsk_null, tsk_null, 0, 0, 0, 0);
}


int tsip_header_Security_Client_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Security_Client_t *Security_Client = (const tsip_header_Security_Client_t *)header;
		int ret = 0;
		
		// ipsec-3gpp; alg=hmac-md5-96; ealg=des-ede3-cbc; spi-c=1111; spi-s=2222; port-c=5062; port-s=5064
		if(tsk_striequals(Security_Client->mech, "ipsec-3gpp")){
			ret = tsk_buffer_append_2(output, "%s%s%s%s%s%s%s;spi-c=%u;spi-s=%u;port-c=%u;port-s=%u", 
				Security_Client->mech,
				
				Security_Client->alg ? ";alg=" : "",
				Security_Client->alg ? Security_Client->alg : "",
				
				Security_Client->ealg ? ";ealg=" : "",
				Security_Client->ealg ? Security_Client->ealg : "",

				Security_Client->prot ? ";prot=" : "",
				Security_Client->prot ? Security_Client->prot : "",
				
				Security_Client->spi_c,
				Security_Client->spi_s,
				Security_Client->port_c,
				Security_Client->port_s
				);
		}
		else if(Security_Client->mech){
			ret = tsk_buffer_append(output, Security_Client->mech, tsk_strlen(Security_Client->mech));
		}

		if(Security_Client->q >= 0){
			/* qvalue	=  	("0" [ "." 0*3DIGIT ] ) / ( "1" [ "." 0*3("0") ] ) */
			ret = tsk_buffer_append_2(output, ";q=%1.3f", Security_Client->q);
		}
		
		return ret;
	}

	return -1;
}

tsip_header_Security_Clients_L_t *tsip_header_Security_Client_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Security_Clients_L_t *hdr_securityclients = tsk_list_create();
	
	const char *tag_start = tsk_null;
	tsip_header_Security_Client_t *curr_securityclient = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_Security_Client_first_final);
	(void)(tsip_machine_parser_header_Security_Client_error);
	(void)(tsip_machine_parser_header_Security_Client_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Security-Client' header.");
		TSK_OBJECT_SAFE_FREE(curr_securityclient);
		TSK_OBJECT_SAFE_FREE(hdr_securityclients);
	}
	
	return hdr_securityclients;
}





//========================================================
//	Security_Client header object definition
//

static tsk_object_t* tsip_header_Security_Client_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Security_Client_t *Security_Client = self;
	if(Security_Client){
		const char* mech = va_arg(*app, const char*);

		TSIP_HEADER(Security_Client)->type = tsip_htype_Security_Client;
		TSIP_HEADER(Security_Client)->serialize = tsip_header_Security_Client_serialize;

		Security_Client->q = -1;

		if(mech){
			Security_Client->mech = tsk_strdup(mech);
			Security_Client->alg = tsk_strdup(va_arg(*app, const char*));
			Security_Client->prot = tsk_strdup(va_arg(*app, const char*));
			Security_Client->mod = tsk_strdup(va_arg(*app, const char*));
			Security_Client->ealg = tsk_strdup(va_arg(*app, const char*));
#if defined(__GNUC__)
			Security_Client->port_c = (tnet_port_t)va_arg(*app, unsigned);
			Security_Client->port_s = (tnet_port_t)va_arg(*app, unsigned);
#else
			Security_Client->port_c = va_arg(*app, tnet_port_t);
			Security_Client->port_s = va_arg(*app, tnet_port_t);
#endif
			Security_Client->spi_c = va_arg(*app, uint32_t);
			Security_Client->spi_s = va_arg(*app, uint32_t);
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Security_Client header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Security_Client_dtor(tsk_object_t *self)
{
	tsip_header_Security_Client_t *Security_Client = self;
	if(Security_Client){
		TSK_FREE(Security_Client->mech);
		TSK_FREE(Security_Client->alg);
		TSK_FREE(Security_Client->prot);
		TSK_FREE(Security_Client->mod);
		TSK_FREE(Security_Client->ealg);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Security_Client));
	}
	else{
		TSK_DEBUG_ERROR("Null Security_Client header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Security_Client_def_s = 
{
	sizeof(tsip_header_Security_Client_t),
	tsip_header_Security_Client_ctor,
	tsip_header_Security_Client_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Security_Client_def_t = &tsip_header_Security_Client_def_s;
