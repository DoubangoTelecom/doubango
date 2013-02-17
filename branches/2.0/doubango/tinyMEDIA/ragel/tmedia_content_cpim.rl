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

/**@file tmedia_content_cpim.c
 * @brief Common Presence and Instant Messaging (CPIM): Message Format (RFC 3862)
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *
 */
#include "tinymedia/content/tmedia_content_cpim.h"

#include "tsk_debug.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_ragel_state.h"

#include <string.h>

/*	RFC 3862 - 2. Overall Message Structure
A complete message looks something like this:

      m: Content-type: Message/CPIM
      s:
      h: (message-metadata-headers)
      s:
      e: (encapsulated MIME message-body)

   The end of the message body is defined by the framing mechanism of
   the protocol used.  The tags 'm:', 's:', 'h:', 'e:', and 'x:' are not
   part of the message format and are used here to indicate the
   different parts of the message, thus:

      m:  MIME headers for the overall message
      s:  a blank separator line
      h:  message headers
      e:  encapsulated MIME object containing the message content
      x:  MIME security multipart message wrapper
*/





/***********************************
*	Ragel state machine.
*/
%%{
	machine tmedia_machine_content_cpim;

	# Includes
	include tmedia_machine_utils "./ragel/tmedia_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}
	
	action is_parsing_mime_headers{
		parsing_mime_headers = tsk_true;
	}

	action is_parsing_message_headers{
		parsing_mime_headers = tsk_false;
	}

	action parse_hname{
		TSK_PARSER_SET_STRING(hname);
	}
	
	action parse_hvalue{
		tmedia_content_header_t* header;
		TSK_PARSER_SET_STRING(hvalue);
		header = tmedia_content_header_create(hname, hvalue);
		TSK_FREE(hname); TSK_FREE(hvalue);

		if(parsing_mime_headers){
			if(!TMEDIA_CONTENT_CPIM(self)->m_headers){
				TMEDIA_CONTENT_CPIM(self)->m_headers = tsk_list_create();
			}
			tsk_list_push_back_data(TMEDIA_CONTENT_CPIM(self)->m_headers, (void**)&header);
		}
		else{
			if(!TMEDIA_CONTENT_CPIM(self)->h_headers){
				TMEDIA_CONTENT_CPIM(self)->h_headers = tsk_list_create();
			}
			tsk_list_push_back_data(TMEDIA_CONTENT_CPIM(self)->h_headers, (void**)&header);
		}
	}

	action parse_e{
		int len = (int)(p  - tag_start);
		if(len && tag_start){
			if(TMEDIA_CONTENT_CPIM(self)->e){
				TSK_OBJECT_SAFE_FREE(TMEDIA_CONTENT_CPIM(self)->e); \
			}
			TMEDIA_CONTENT_CPIM(self)->e = tsk_buffer_create(tag_start, len);
		}
	}
	
	hname = token>tag %parse_hname;
	hvalue = any*>tag %parse_hvalue;

	Header = hname :>SP*:> ":" SP*<: hvalue :> CRLF;
	m = Header+ >is_parsing_mime_headers;
	s = CRLF;	
	h = Header+ >is_parsing_message_headers;
	e = any*>tag %parse_e;
	
	# Entry point
	main := m s h s e;
}%%


static int tmedia_content_cpim_parse(tmedia_content_t* self, const void* in_data, tsk_size_t in_size)
{
	int cs = 0;
	const char *p = in_data;
	const char *pe = p + in_size;
	const char *eof = pe;
	
	const char *tag_start = tsk_null;

	char* hname = tsk_null;
	char* hvalue = tsk_null;
	tsk_bool_t parsing_mime_headers = tsk_true;

	%%write data;
	(void)(eof);
	(void)(tmedia_machine_content_cpim_first_final);
	(void)(tmedia_machine_content_cpim_error);
	(void)(tmedia_machine_content_cpim_en_main);
	%%write init;
	%%write exec;
	
	TSK_FREE(hname);
	TSK_FREE(hvalue);

	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse CPIM content");
		return -1;
	}
	
	return 0;
}

static tsk_buffer_t* tmedia_content_cpim_get_data(tmedia_content_t* self)
{
	tsk_buffer_t* data = tsk_buffer_create_null();
	tmedia_content_cpim_t *cpim = TMEDIA_CONTENT_CPIM(self);
	const tsk_list_item_t* item;
	  /*
		  m: Content-type: Message/CPIM
		  s:
		  h: (message-metadata-headers)
		  s:
		  e: (encapsulated MIME message-body)
		  x:  MIME security multipart message wrapper
	  */
	if(cpim->m_headers){
		tsk_list_foreach(item, cpim->m_headers){
			char* hstring = tmedia_content_header_tostring(TMEDIA_CONTENT_HEADER(item->data));
			tsk_buffer_append_2(data, TSK_LIST_IS_LAST(cpim->m_headers, item) ? "%s\r\n\r\n" : "%s\r\n", hstring);
			TSK_FREE(hstring);
		}
	}
	if(cpim->h_headers){
		tsk_list_foreach(item, cpim->h_headers){
			char* hstring = tmedia_content_header_tostring(TMEDIA_CONTENT_HEADER(item->data));
			tsk_buffer_append_2(data, TSK_LIST_IS_LAST(cpim->h_headers, item) ? "%s\r\n\r\n" : "%s\r\n", hstring);
			TSK_FREE(hstring);
		}
	}
	if(cpim->e){
		tsk_buffer_append(data, TSK_BUFFER_DATA(cpim->e), TSK_BUFFER_SIZE(cpim->e));
	}
	if(cpim->x){
		tsk_buffer_append(data, TSK_BUFFER_DATA(cpim->x), TSK_BUFFER_SIZE(cpim->x));
	}

	return data;
}

//=================================================================================================
//	object/plugin definitions
//
/* constructor */
static tsk_object_t* tmedia_content_cpim_ctor(tsk_object_t * self, va_list * app)
{
	tmedia_content_cpim_t *cpim = self;
	if(cpim){
		/* init base: called by tmedia_content_create() */
		/* init self */
	}
	return self;
}
/* destructor */
static tsk_object_t* tmedia_content_cpim_dtor(tsk_object_t * self)
{ 
	tmedia_content_cpim_t *cpim = self;
	if(cpim){
		/* deinit base */
		tmedia_content_deinit(TMEDIA_CONTENT(cpim));
		/* deinit self */
		TSK_OBJECT_SAFE_FREE(cpim->m_headers);
		TSK_OBJECT_SAFE_FREE(cpim->h_headers);
		TSK_OBJECT_SAFE_FREE(cpim->e);
		TSK_OBJECT_SAFE_FREE(cpim->x);
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tmedia_content_cpim_def_s = 
{
	sizeof(tmedia_content_cpim_t),
	tmedia_content_cpim_ctor, 
	tmedia_content_cpim_dtor,
	tsk_null, 
};
/* plugin definition*/
static const tmedia_content_plugin_def_t tmedia_content_cpim_plugin_def_s = 
{
	&tmedia_content_cpim_def_s,

	TMEDIA_CONTENT_CPIM_TYPE,
	tmedia_content_cpim_parse,
	tmedia_content_cpim_get_data
};
const tmedia_content_plugin_def_t *tmedia_content_cpim_plugin_def_t = &tmedia_content_cpim_plugin_def_s;