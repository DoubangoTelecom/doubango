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

/**@file tsip_header_Contact.c
 * @brief SIP Contact/m header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Contact.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Contact;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action create_contact{
		if(!curr_contact){
			curr_contact = tsip_header_Contact_create();
		}
	}

	action parse_display_name{
		if(curr_contact){
			TSK_PARSER_SET_STRING(curr_contact->display_name);
			tsk_strunquote(&curr_contact->display_name);
		}
	}

	action parse_uri{
		if(curr_contact && !curr_contact->uri){
			int len = (int)(p  - tag_start);
			if((curr_contact->uri = tsip_uri_parse(tag_start, (tsk_size_t)len)) && curr_contact->display_name){
				curr_contact->uri->display_name = tsk_strdup(curr_contact->display_name);
			}
		}
	}

	action parse_expires{
		if(curr_contact){
			TSK_PARSER_SET_INTEGER(curr_contact->expires);
		}
	}

	action parse_param{
		if(curr_contact){
			TSK_PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_contact));
		}
	}

	action add_contact{
		if(curr_contact){
			tsk_list_push_back_data(hdr_contacts, ((void**) &curr_contact));
		}
	}
	
	action eob{
	}
	
	URI = (scheme HCOLON any+)>tag %parse_uri;
	display_name = (( token LWS )+ | quoted_string)>tag %parse_display_name;
	my_name_addr = display_name? :>LAQUOT<: URI :>RAQUOT;
	
	c_p_expires = "expires"i EQUAL delta_seconds>tag %parse_expires;
	contact_extension = (generic_param)>tag %parse_param;
	contact_params = c_p_expires@1 | contact_extension@0;
	contact_param = ( (my_name_addr | URI) :> (SEMI contact_params)* ) >create_contact %add_contact;
	Contact = ( "Contact"i | "m"i ) HCOLON ( STAR | ( contact_param ( COMMA contact_param )* ) );
	
	# Entry point
	main := Contact :>CRLF @eob;

}%%


tsip_header_Contact_t* tsip_header_Contact_create()
{
	return tsk_object_new(tsip_header_Contact_def_t);
}

int tsip_header_Contact_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	if(header){
		const tsip_header_Contact_t *Contact = (const tsip_header_Contact_t *)header;
		int ret = 0;
		
		/* Uri with hacked display-name*/
		if((ret = tsip_uri_serialize(Contact->uri, tsk_true, tsk_true, output))){
			return ret;
		}

		/* Expires */
		if(Contact->expires >=0){
			tsk_buffer_append_2(output, ";expires=%lld", Contact->expires);
		}
		
		return ret;
	}

	return -1;
}


tsip_header_Contacts_L_t *tsip_header_Contact_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Contacts_L_t *hdr_contacts = tsk_list_create();
	
	const char *tag_start = tsk_null;
	tsip_header_Contact_t *curr_contact = 0;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_Contact_first_final);
	(void)(tsip_machine_parser_header_Contact_error);
	(void)(tsip_machine_parser_header_Contact_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse SIP 'Contact' header.");
		TSK_OBJECT_SAFE_FREE(curr_contact);
		TSK_OBJECT_SAFE_FREE(hdr_contacts);
	}
	
	return hdr_contacts;
}




//========================================================
//	Contact header object definition
//

static tsk_object_t* tsip_header_Contact_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Contact_t *Contact = self;
	if(Contact){
		TSIP_HEADER(Contact)->type = tsip_htype_Contact;
		TSIP_HEADER(Contact)->serialize = tsip_header_Contact_serialize;
		Contact->expires = -1;
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Contact header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Contact_dtor(tsk_object_t *self)
{
	tsip_header_Contact_t *Contact = self;
	if(Contact){
		TSK_FREE(Contact->display_name);
		TSK_OBJECT_SAFE_FREE(Contact->uri);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Contact));
	}
	else{
		TSK_DEBUG_ERROR("Null Contact header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Contact_def_s = 
{
	sizeof(tsip_header_Contact_t),
	tsip_header_Contact_ctor,
	tsip_header_Contact_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Contact_def_t = &tsip_header_Contact_def_s;


////========================================================
////	Contact object definition
////
//
///**@ingroup tsip_header_Contact_group
//*/
//static tsk_object_t* tsip_contact_ctor(tsk_object_t *self, va_list * app)
//{
//	tsip_contact_t *contact = self;
//	if(contact)
//	{
//		contact->expires = -1;
//	}
//	else
//	{
//		TSK_DEBUG_ERROR("Failed to create new Contact object.");
//	}
//	return self;
//}
//
//static tsk_object_t* tsip_contact_dtor(tsk_object_t *self)
//{
//	tsip_contact_t *contact = self;
//	if(contact)
//	{
//		TSK_FREE(contact->display_name);
//		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(contact));
//
//		TSK_OBJECT_SAFE_FREE(contact->uri);
//	}
//	else TSK_DEBUG_ERROR("Null Contact object.");
//
//	return self;
//}
//
//static const tsk_object_def_t tsip_contact_def_s = 
//{
//	sizeof(tsip_contact_t),
//	tsip_contact_ctor,
//	tsip_contact_dtor,
//	0
//};
//const void *tsip_contact_def_t = &tsip_contact_def_s;
