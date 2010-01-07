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

/**@file tsip_header_Contact.c
 * @brief SIP Contact/m header.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header_Contact.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

/**@defgroup tsip_header_Contact_group SIP Contact header.
*/

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Contact;

	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";
	
	action tag
	{
		tag_start = p;
	}

	action create_contact
	{
		if(!curr_contact)
		{
			curr_contact = TSIP_HEADER_CONTACT_CREATE();
		}
	}

	action parse_display_name
	{
		if(curr_contact)
		{
			PARSER_SET_STRING(curr_contact->display_name);
		}
	}

	action parse_uri
	{
		if(curr_contact && !curr_contact->uri)
		{
			int len = (int)(p  - tag_start);
			curr_contact->uri = tsip_uri_parse(tag_start, (size_t)len);
		}
	}

	action parse_expires
	{
		if(curr_contact)
		{
			PARSER_SET_INTEGER(curr_contact->expires);
		}
	}

	action parse_param
	{
		if(curr_contact)
		{
			PARSER_ADD_PARAM(TSIP_HEADER_PARAMS(curr_contact));
		}
	}

	action add_contact
	{
		if(curr_contact)
		{
			tsk_list_push_back_data(hdr_contacts, ((void**) &curr_contact));
		}
	}
	
	action eob
	{
	}
	
	URI = (scheme HCOLON any+)>tag %parse_uri;
	display_name = (( token LWS )+ | quoted_string)>tag %parse_display_name;
	my_name_addr = display_name? :>LAQUOT<: URI :>RAQUOT;
	
	c_p_expires = "expires"i EQUAL delta_seconds>tag %parse_expires;
	contact_extension = (generic_param)>tag %parse_param;
	contact_params = c_p_expires>1 | contact_extension>0;
	contact_param = (( my_name_addr | URI ) <: ( SEMI contact_params )*) >create_contact %add_contact;
	Contact = ( "Contact"i | "m"i ) HCOLON ( STAR | ( contact_param ( COMMA contact_param )* ) );
	
	# Entry point
	main := Contact :>CRLF @eob;

}%%


int tsip_header_Contact_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Contact_t *Contact = header;
		int ret = 0;

		{
			/* Display name */
			if(Contact->display_name)
			{
				tsk_buffer_appendEx(output, "\"%s\"", Contact->display_name);
			}

			/* Uri */
			if(ret=tsip_uri_tostring(Contact->uri, 1, 1, output))
			{
				return ret;
			}

			/* Expires */
			if(Contact->expires >=0)
			{
				tsk_buffer_appendEx(output, ";expires=%d", Contact->expires);
			}
		}

		return ret;
	}

	return -1;
}


tsip_header_Contacts_L_t *tsip_header_Contact_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Contacts_L_t *hdr_contacts = TSK_LIST_CREATE();
	
	const char *tag_start;
	tsip_header_Contact_t *curr_contact = 0;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% )
	{
		TSIP_HEADER_CONTACT_SAFE_FREE(curr_contact);
		TSK_LIST_SAFE_FREE(hdr_contacts);
	}
	
	return hdr_contacts;
}




//========================================================
//	Contact header object definition
//

/**@ingroup tsip_header_Contact_group
*/
static void* tsip_header_Contact_create(void *self, va_list * app)
{
	tsip_header_Contact_t *Contact = self;
	if(Contact)
	{
		TSIP_HEADER(Contact)->type = tsip_htype_Contact;
		TSIP_HEADER(Contact)->tostring = tsip_header_Contact_tostring;
		Contact->expires = -1;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Contact header.");
	}
	return self;
}

/**@ingroup tsip_header_Contact_group
*/
static void* tsip_header_Contact_destroy(void *self)
{
	tsip_header_Contact_t *Contact = self;
	if(Contact)
	{
		TSK_FREE(Contact->display_name);
		TSIP_URI_SAFE_FREE(Contact->uri);

		TSK_LIST_SAFE_FREE(TSIP_HEADER_PARAMS(Contact));
	}
	else TSK_DEBUG_ERROR("Null Contact header.");

	return self;
}

static const tsk_object_def_t tsip_header_Contact_def_s = 
{
	sizeof(tsip_header_Contact_t),
	tsip_header_Contact_create,
	tsip_header_Contact_destroy,
	0
};
const void *tsip_header_Contact_def_t = &tsip_header_Contact_def_s;


////========================================================
////	Contact object definition
////
//
///**@ingroup tsip_header_Contact_group
//*/
//static void* tsip_contact_create(void *self, va_list * app)
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
///**@ingroup tsip_header_Contact_group
//*/
//static void* tsip_contact_destroy(void *self)
//{
//	tsip_contact_t *contact = self;
//	if(contact)
//	{
//		TSK_FREE(contact->display_name);
//		TSK_LIST_SAFE_FREE(TSIP_HEADER_PARAMS(contact));
//
//		TSIP_URI_SAFE_FREE(contact->uri);
//	}
//	else TSK_DEBUG_ERROR("Null Contact object.");
//
//	return self;
//}
//
//static const tsk_object_def_t tsip_contact_def_s = 
//{
//	sizeof(tsip_contact_t),
//	tsip_contact_create,
//	tsip_contact_destroy,
//	0
//};
//const void *tsip_contact_def_t = &tsip_contact_def_s;