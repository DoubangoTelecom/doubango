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
		TSK_DEBUG_INFO("CONTACT:TAG");
		tag_start = p;
	}

	action create_contact
	{
		if(!curr_contact)
		{
			curr_contact = TSIP_CONTACT_CREATE();
			TSK_DEBUG_INFO("CONTACT:CREATE_CONTACT");
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
		if(curr_contact)
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
			PARSER_ADD_PARAM(curr_contact->params);
		}
	}

	action add_contact
	{
		if(curr_contact)
		{
			tsk_list_push_back_data(hdr_contact->contacts, ((void**) &curr_contact));
			TSK_DEBUG_INFO("CONTACT:ADD_CONTACT");
		}
	}

	action eob
	{
		TSK_DEBUG_INFO("CONTACT:EOB");
	}
	
	URI = (scheme HCOLON any+)>tag %parse_uri;
	display_name = (( token LWS )+ | quoted_string)>tag %parse_display_name;
	my_name_addr = display_name? :>LAQUOT<: URI :>RAQUOT;
	
	c_p_expires = "expires"i EQUAL delta_seconds>tag %parse_expires;
	contact_extension = generic_param>tag %parse_param;
	contact_params = c_p_expires | contact_extension;
	contact_param = (( my_name_addr | URI ) ( SEMI contact_params )*) >create_contact %add_contact;
	Contact = ( "Contact"i | "m"i ) HCOLON ( STAR | ( contact_param ( COMMA contact_param )* ) );
	
	# Entry point
	main := Contact :>CRLF @eob;

}%%


int tsip_header_Contact_tostring(const void* header, tsk_buffer_t* output)
{
	if(header)
	{
		const tsip_header_Contact_t *Contact = header;
		tsip_contact_t *contact;
		tsk_list_item_t *item;
		int ret = 0;

		tsk_list_foreach(item, Contact->contacts)
		{
			contact = item->data;

			/* Separator */
			if(item != Contact->contacts->head)
			{
				tsk_buffer_append(output, ",", 1);
			}

			/* Display name */
			if(contact->display_name)
			{
				tsk_buffer_appendEx(output, "\"%s\"", contact->display_name);
			}

			/* Uri */
			if(ret=tsip_uri_tostring(contact->uri, 1, 1, output))
			{
				return ret;
			}

			/* Expires */
			if(contact->expires >=0)
			{
				tsk_buffer_appendEx(output, ";expires=%d", contact->expires);
			}

			/* Params */
			if(ret=tsk_params_tostring(contact->params, ';', output))
			{
				return ret;
			}
		}

		return ret;
	}

	return -1;
}


tsip_header_Contact_t *tsip_header_Contact_parse(const char *data, size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Contact_t *hdr_contact = TSIP_HEADER_CONTACT_CREATE();
	
	const char *tag_start;
	tsip_contact_t *curr_contact = 0;

	%%write data;
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% )
	{
		TSIP_HEADER_CONTACT_SAFE_FREE(hdr_contact);
	}
	else
	{
		if(curr_contact)
		{
			tsk_list_push_back_data(hdr_contact->contacts, ((void**) &curr_contact));
		}
	}
	
	return hdr_contact;
}



const tsip_contact_t *tsip_header_Contact_get_ContactAt(tsip_header_Contact_t *hdr_contact, size_t index)
{
	size_t pos = 0;
	tsk_list_item_t *item = 0;
	if(hdr_contact)
	{
		tsk_list_foreach(item, hdr_contact->contacts)
		{
			//if(!pred_find_header_by_type(item, &type))
			{
				if(pos++ >= index)
				{
					break;
				}
			}
		}
	}
	return item ? item->data : 0;
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
		Contact->type = tsip_htype_Contact;
		Contact->tostring = tsip_header_Contact_tostring;
		Contact->contacts = TSK_LIST_CREATE();
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
		TSK_LIST_SAFE_FREE(Contact->contacts);
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


//========================================================
//	Contact object definition
//

/**@ingroup tsip_header_Contact_group
*/
static void* tsip_contact_create(void *self, va_list * app)
{
	tsip_contact_t *contact = self;
	if(contact)
	{
		contact->expires = -1;
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new Contact object.");
	}
	return self;
}

/**@ingroup tsip_header_Contact_group
*/
static void* tsip_contact_destroy(void *self)
{
	tsip_contact_t *contact = self;
	if(contact)
	{
		TSK_FREE(contact->display_name);
		TSK_LIST_SAFE_FREE(contact->params);

		TSIP_URI_SAFE_FREE(contact->uri);
	}
	else TSK_DEBUG_ERROR("Null Contact object.");

	return self;
}

static const tsk_object_def_t tsip_contact_def_s = 
{
	sizeof(tsip_contact_t),
	tsip_contact_create,
	tsip_contact_destroy,
	0
};
const void *tsip_contact_def_t = &tsip_contact_def_s;