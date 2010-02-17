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

/**@file thttp_parser_headers.c
 * @brief HTTP headers parser.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinyhttp/parsers/thttp_parser_header.h"

//#include "tinyhttp/headers/thttp_header_Allow.h"
//#include "tinyhttp/headers/thttp_header_Allow_Events.h"
//#include "tinyhttp/headers/thttp_header_Authorization.h"
//#include "tinyhttp/headers/thttp_header_Call_ID.h"
//#include "tinyhttp/headers/thttp_header_Contact.h"
//#include "tinyhttp/headers/thttp_header_CSeq.h"
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
//#include "tinyhttp/headers/thttp_header_Service_Route.h"
//#include "tinyhttp/headers/thttp_header_Supported.h"
//#include "tinyhttp/headers/thttp_header_To.h"
//#include "tinyhttp/headers/thttp_header_User_Agent.h"
//#include "tinyhttp/headers/thttp_header_Via.h"
//#include "tinyhttp/headers/thttp_header_WWW_Authenticate.h"

#include "tsk_debug.h"

/***********************************
*	Ragel state machine.
*/
%%{
	machine thttp_machine_parser_headers;


	# /*== Accept: ==*/
	action parse_header_Accept
	{
		TSK_DEBUG_ERROR("parse_header_Accept NOT IMPLEMENTED");
	}

	# /*== Accept_Charset: ==*/
	action parse_header_Accept_Charset
	{
		TSK_DEBUG_ERROR("parse_header_Accept_Charset NOT IMPLEMENTED");
	}

	# /*== Accept_Encoding: ==*/
	action parse_header_Accept_Encoding
	{
		TSK_DEBUG_ERROR("parse_header_Accept_Encoding NOT IMPLEMENTED");
	}
	
	# /*== Accept_Language: ==*/
	action parse_header_Accept_Language
	{
		TSK_DEBUG_ERROR("parse_header_Accept_Language NOT IMPLEMENTED");
	}
	
	# /*== Allow: ==*/
	action parse_header_Allow
	{
		TSK_DEBUG_ERROR("parse_header_Allow NOT IMPLEMENTED");
	}
	
	# /*== Authorization: ==*/
	action parse_header_Authorization
	{
		TSK_DEBUG_ERROR("parse_header_Authorization NOT IMPLEMENTED");
	}
	
	# /*== Cache_Control: ==*/
	action parse_header_Cache_Control
	{
		TSK_DEBUG_ERROR("parse_header_Cache_Control NOT IMPLEMENTED");
	}
	
	# /*== Connection: ==*/
	action parse_header_Connection
	{
		TSK_DEBUG_ERROR("parse_header_Connection NOT IMPLEMENTED");
	}
	
	# /*== Content_Encoding: ==*/
	action parse_header_Content_Encoding
	{
		TSK_DEBUG_ERROR("parse_header_Content_Encoding NOT IMPLEMENTED");
	}
	
	# /*== Content_Language: ==*/
	action parse_header_Content_Language
	{
		TSK_DEBUG_ERROR("parse_header_Content_Language NOT IMPLEMENTED");
	}

	# /*== Content_Length: ==*/
	action parse_header_Content_Length
	{
		TSK_DEBUG_ERROR("parse_header_Content_Length NOT IMPLEMENTED");
	}

	# /*== Content_Location: ==*/
	action parse_header_Content_Location
	{
		TSK_DEBUG_ERROR("parse_header_Content_Location NOT IMPLEMENTED");
	}
	
	# /*== Content_MD5: ==*/
	action parse_header_Content_MD5
	{
		TSK_DEBUG_ERROR("parse_header_Content_MD5 NOT IMPLEMENTED");
	}
	
	# /*== Content_Range: ==*/
	action parse_header_Content_Range
	{
		TSK_DEBUG_ERROR("parse_header_Content_Range NOT IMPLEMENTED");
	}
	
	# /*== Content_Type: ==*/
	action parse_header_Content_Type
	{
		TSK_DEBUG_ERROR("parse_header_Content_Type NOT IMPLEMENTED");
	}

	# /*== Date: ==*/
	action parse_header_Date
	{
		TSK_DEBUG_ERROR("parse_header_Date NOT IMPLEMENTED");
	}
	
	# /*== Expect: ==*/
	action parse_header_Expect
	{
		TSK_DEBUG_ERROR("parse_header_Expect NOT IMPLEMENTED");
	}
	
	# /*== Expires: ==*/
	action parse_header_Expires
	{
		TSK_DEBUG_ERROR("parse_header_Expires NOT IMPLEMENTED");
	}
	
	# /*== From: ==*/
	action parse_header_From
	{
		TSK_DEBUG_ERROR("parse_header_From NOT IMPLEMENTED");
	}
	
	# /*== Host: ==*/
	action parse_header_Host
	{
		TSK_DEBUG_ERROR("parse_header_Host NOT IMPLEMENTED");
	}

	# /*== If_Match: ==*/
	action parse_header_If_Match
	{
		TSK_DEBUG_ERROR("parse_header_If_Match NOT IMPLEMENTED");
	}
	
	# /*== If_Modified_Since: ==*/
	action parse_header_If_Modified_Since
	{
		TSK_DEBUG_ERROR("parse_header_If_Modified_Since NOT IMPLEMENTED");
	}
	
	# /*== If_None_Match: ==*/
	action parse_header_If_None_Match
	{
		TSK_DEBUG_ERROR("parse_header_If_None_Match NOT IMPLEMENTED");
	}
	
	# /*== If_Range: ==*/
	action parse_header_If_Range
	{
		TSK_DEBUG_ERROR("parse_header_If_Range NOT IMPLEMENTED");
	}

	# /*== If_Unmodified_Since: ==*/
	action parse_header_If_Unmodified_Since
	{
		TSK_DEBUG_ERROR("parse_header_If_Unmodified_Since NOT IMPLEMENTED");
	}
	
	# /*== Last_Modified: ==*/
	action parse_header_Last_Modified
	{
		TSK_DEBUG_ERROR("parse_header_Last_Modified NOT IMPLEMENTED");
	}

	# /*== Max_Forwards: ==*/
	action parse_header_Max_Forwards
	{
		TSK_DEBUG_ERROR("parse_header_Max_Forwards NOT IMPLEMENTED");
	} 
	
	# /*== Pragma: ==*/
	action parse_header_Pragma
	{
		TSK_DEBUG_ERROR("parse_header_Pragma NOT IMPLEMENTED");
	}
	
	# /*== Proxy_Authorization: ==*/
	action parse_header_Proxy_Authorization
	{
		TSK_DEBUG_ERROR("parse_header_Proxy_Authorizations NOT IMPLEMENTED");
	}
	
	# /*== Range: ==*/
	action parse_header_Range
	{
		TSK_DEBUG_ERROR("parse_header_Range NOT IMPLEMENTED");
	}

	# /*== Referer: ==*/
	action parse_header_Referer
	{
		TSK_DEBUG_ERROR("parse_header_Referer NOT IMPLEMENTED");
	}
	
	# /*== Transfer_Encoding: ==*/
	action parse_header_Transfer_Encoding
	{
		TSK_DEBUG_ERROR("parse_header_Transfer_Encoding NOT IMPLEMENTED");
	}
	
	# /*== TE: ==*/
	action parse_header_TE
	{
		TSK_DEBUG_ERROR("parse_header_TE NOT IMPLEMENTED");
	}
	
	# /*== Trailer: ==*/
	action parse_header_Trailer
	{
		TSK_DEBUG_ERROR("parse_header_Trailer NOT IMPLEMENTED");
	}

	# /*== Upgrade: ==*/
	action parse_header_Upgrade
	{
		TSK_DEBUG_ERROR("parse_header_Upgrade NOT IMPLEMENTED");
	}
	
	# /*== User_Agent: ==*/
	action parse_header_User_Agent
	{
		TSK_DEBUG_ERROR("parse_header_User_Agent NOT IMPLEMENTED");
	}
	
	# /*== Via: ==*/
	action parse_header_Via
	{
		TSK_DEBUG_ERROR("parse_header_Via NOT IMPLEMENTED");
	}
	
	# /*== Warning: ==*/
	action parse_header_Warning
	{
		TSK_DEBUG_ERROR("parse_header_Warning NOT IMPLEMENTED");
	}
		
	# /*== extension_header: ==*/
	action parse_header_extension_header
	{
		TSK_DEBUG_ERROR("parse_header_extension_header NOT IMPLEMENTED");
	}


	# Includes
	include thttp_machine_utils "./thttp_machine_utils.rl";
	include thttp_machine_header "./thttp_machine_header.rl";

	# Entry point
	main := HEADER;
}%%

int thttp_header_parse(tsk_ragel_state_t *state, thttp_message_t *message)
{
	int cs = 0;
	const char *p = state->tag_start;
	const char *pe = state->tag_end;
	const char *eof = pe;

	%%write data;
	%%write init;
	%%write exec;

	return ( cs >= %%{ write first_final; }%% ) ? 0 : -1;
}