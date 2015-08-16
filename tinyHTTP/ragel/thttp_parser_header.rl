/*
* Copyright (C) 2010-2015 Mamadou Diop.
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

/**@file thttp_parser_header.c
 * @brief HTTP headers parser.
 */
#include "tinyhttp/parsers/thttp_parser_header.h"

//#include "tinyhttp/headers/thttp_header_Allow.h"
//#include "tinyhttp/headers/thttp_header_Allow_Events.h"
#include "tinyhttp/headers/thttp_header_Authorization.h"
//#include "tinyhttp/headers/thttp_header_Call_ID.h"
//#include "tinyhttp/headers/thttp_header_Contact.h"
//#include "tinyhttp/headers/thttp_header_CSeq.h"
#include "tinyhttp/headers/thttp_header_Dummy.h"
#include "tinyhttp/headers/thttp_header_ETag.h"
//#include "tinyhttp/headers/thttp_header_Expires.h"
//#include "tinyhttp/headers/thttp_header_From.h"
//#include "tinyhttp/headers/thttp_header_Max_Forwards.h"
//#include "tinyhttp/headers/thttp_header_Min_Expires.h"
//#include "tinyhttp/headers/thttp_header_Path.h"
//#include "tinyhttp/headers/thttp_header_P_Access_Network_Info.h" 
//#include "tinyhttp/headers/thttp_header_P_Preferred_Identity.h"
//#include "tinyhttp/headers/thttp_header_Privacy.h"
//#include "tinyhttp/headers/thttp_header_Proxy_Authenticate.h"
//#include "tinyhttp/headers/thttp_header_Proxy_Authorization.h"
//#include "tinyhttp/headers/thttp_header_Record_Route.h"
//#include "tinyhttp/headers/thttp_header_Require.h"
//#include "tinyhttp/headers/thttp_header_Route.h"
#include "tinyhttp/headers/thttp_header_Sec_WebSocket_Accept.h"
#include "tinyhttp/headers/thttp_header_Sec_WebSocket_Key.h"
#include "tinyhttp/headers/thttp_header_Sec_WebSocket_Protocol.h"
#include "tinyhttp/headers/thttp_header_Sec_WebSocket_Version.h"
//#include "tinyhttp/headers/thttp_header_Service_Route.h"
//#include "tinyhttp/headers/thttp_header_Supported.h"
#include "tinyhttp/headers/thttp_header_Transfer_Encoding.h"
//#include "tinyhttp/headers/thttp_header_User_Agent.h"
//#include "tinyhttp/headers/thttp_header_Via.h"
#include "tinyhttp/headers/thttp_header_WWW_Authenticate.h"

#include "tsk_debug.h"

#undef ADD_HEADERS
#undef ADD_HEADER

#define ADD_HEADERS(headers)\
	if(headers)\
	{\
		tsk_list_item_t *item;\
		tsk_list_foreach(item, headers)\
		{\
			thttp_header_t *hdr = tsk_object_ref(item->data);\
			tsk_list_push_back_data(message->headers, ((void**) &hdr));\
		}\
		\
		TSK_OBJECT_SAFE_FREE(headers);\
	}
#define ADD_HEADER(header)\
	if(header)\
	{\
		tsk_list_push_back_data(message->headers, ((void**) &header));\
	}

/***********************************
*	Ragel state machine.
*/
%%{
	machine thttp_machine_parser_headers;


	# /*== Accept: ==*/
	action parse_header_Accept
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Accept NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Accept_Charset: ==*/
	action parse_header_Accept_Charset
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Accept_Charset NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Accept_Encoding: ==*/
	action parse_header_Accept_Encoding
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Accept_Encoding NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Accept_Language: ==*/
	action parse_header_Accept_Language
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Accept_Language NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Allow: ==*/
	action parse_header_Allow
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Allow NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Authorization: ==*/
	action parse_header_Authorization
	{
		thttp_header_Authorization_t *header = thttp_header_Authorization_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}
	
	# /*== Cache_Control: ==*/
	action parse_header_Cache_Control
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Cache_Control NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Connection: ==*/
	action parse_header_Connection
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Connection NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Content_Encoding: ==*/
	action parse_header_Content_Encoding
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Content_Encoding NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Content_Language: ==*/
	action parse_header_Content_Language
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Content_Language NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Content_Length: ==*/
	action parse_header_Content_Length
	{
		if(!message->Content_Length){
			message->Content_Length = thttp_header_Content_Length_parse(state->tag_start, (state->tag_end-state->tag_start));
		}
		else{
			thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
			ADD_HEADER(header);
			//TSK_DEBUG_WARN("The message already have 'Content-Length' header.");
		}
	}

	# /*== Content_Location: ==*/
	action parse_header_Content_Location
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Content_Location NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Content_MD5: ==*/
	action parse_header_Content_MD5
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Content_MD5 NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Content_Range: ==*/
	action parse_header_Content_Range
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Content_Range NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Content_Type: ==*/
	action parse_header_Content_Type
	{
		if(!message->Content_Type){
			message->Content_Type = thttp_header_Content_Type_parse(state->tag_start, (state->tag_end-state->tag_start));
		}
		else{
			thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
			ADD_HEADER(header);
			//TSK_DEBUG_WARN("The message already have 'Content-Type' header.");
		}
	}

	# /*== Date: ==*/
	action parse_header_Date
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Date NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== ETag: ==*/
	action parse_header_ETag
	{
		thttp_header_ETag_t *header = thttp_header_ETag_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Expires NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Expect: ==*/
	action parse_header_Expect
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Expect NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Expires: ==*/
	action parse_header_Expires
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Expires NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== From: ==*/
	action parse_header_From
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_From NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Host: ==*/
	action parse_header_Host
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Host NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== If_Match: ==*/
	action parse_header_If_Match
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_If_Match NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== If_Modified_Since: ==*/
	action parse_header_If_Modified_Since
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_If_Modified_Since NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== If_None_Match: ==*/
	action parse_header_If_None_Match
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_If_None_Match NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== If_Range: ==*/
	action parse_header_If_Range
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_If_Range NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== If_Unmodified_Since: ==*/
	action parse_header_If_Unmodified_Since
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_If_Unmodified_Since NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Last_Modified: ==*/
	action parse_header_Last_Modified
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Last_Modified NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Max_Forwards: ==*/
	action parse_header_Max_Forwards
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Max_Forwards NOT IMPLEMENTED. Will be added as Dummy header.");
	} 
	
	# /*== Pragma: ==*/
	action parse_header_Pragma
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Pragma NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Proxy_Authenticate: ==*/
	action parse_header_Proxy_Authenticate
	{
		thttp_header_Proxy_Authenticate_t *header = thttp_header_Proxy_Authenticate_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== Proxy_Authorization: ==*/
	action parse_header_Proxy_Authorization
	{
		thttp_header_Proxy_Authorization_t *header = thttp_header_Proxy_Authorization_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}
	
	# /*== Range: ==*/
	action parse_header_Range
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Range NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Referer: ==*/
	action parse_header_Referer
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Referer NOT IMPLEMENTED. Will be added as Dummy header.");
	}	
	
	# /* == Sec-WebSocket-Accept == */
	action parse_header_Sec_WebSocket_Accept
	{
		thttp_header_Sec_WebSocket_Accept_t* header = thttp_header_Sec_WebSocket_Accept_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /* == Sec-WebSocket-Key == */
	action parse_header_Sec_WebSocket_Key
	{
		thttp_header_Sec_WebSocket_Key_t* header = thttp_header_Sec_WebSocket_Key_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /* == Sec-WebSocket-Protocol == */
	action parse_header_Sec_WebSocket_Protocol
	{
		thttp_header_Sec_WebSocket_Protocol_t* header = thttp_header_Sec_WebSocket_Protocol_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}


	# /* == Sec-WebSocket-Version == */
	action parse_header_Sec_WebSocket_Version
	{
		thttp_header_Sec_WebSocket_Version_t* header = thttp_header_Sec_WebSocket_Version_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== TE: ==*/
	action parse_header_TE
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_TE NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Trailer: ==*/
	action parse_header_Trailer
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Trailer NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Transfer_Encoding: ==*/
	action parse_header_Transfer_Encoding
	{
		thttp_header_Transfer_Encoding_t *header = thttp_header_Transfer_Encoding_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Trailer NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Upgrade: ==*/
	action parse_header_Upgrade
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Upgrade NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== User_Agent: ==*/
	action parse_header_User_Agent
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_User_Agent NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Via: ==*/
	action parse_header_Via
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Via NOT IMPLEMENTED. Will be added as Dummy header.");
	}
	
	# /*== Warning: ==*/
	action parse_header_Warning
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_Warning NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== WWW-Authenticate: ==*/
	action parse_header_WWW_Authenticate
	{
		thttp_header_WWW_Authenticate_t *header = thttp_header_WWW_Authenticate_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}
		
	# /*== extension_header: ==*/
	action parse_header_extension_header
	{
		thttp_header_Dummy_t *header = thttp_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);

		//TSK_DEBUG_WARN("parse_header_extension_header NOT IMPLEMENTED. Will be added as Dummy header.");
	}


	# Includes
	include thttp_machine_utils "./ragel/thttp_machine_utils.rl";
	include thttp_machine_header "./ragel/thttp_machine_header.rl";

	# Entry point
	main := HEADER;
}%%

int thttp_header_parse(tsk_ragel_state_t *state, thttp_message_t *message)
{
	int cs = 0;
	const char *p = state->tag_start;
	const char *pe = state->tag_end;
	const char *eof = pe;

	TSK_RAGEL_DISABLE_WARNINGS_BEGIN()
	%%write data;
	(void)(eof);
	(void)(thttp_machine_parser_headers_first_final);
	(void)(thttp_machine_parser_headers_error);
	(void)(thttp_machine_parser_headers_en_main);
	%%write init;
	%%write exec;
	TSK_RAGEL_DISABLE_WARNINGS_END()

	return ( cs >= %%{ write first_final; }%% ) ? 0 : -1;
}
