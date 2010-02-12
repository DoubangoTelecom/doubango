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

/**@file tsip_parser_headers.c
 * @brief SIP headers parser.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/parsers/tsip_parser_header.h"

#include "tinysip/headers/tsip_header_Allow.h"
#include "tinysip/headers/tsip_header_Allow_Events.h"
#include "tinysip/headers/tsip_header_Authorization.h"
#include "tinysip/headers/tsip_header_Call_ID.h"
#include "tinysip/headers/tsip_header_Contact.h"
#include "tinysip/headers/tsip_header_CSeq.h"
#include "tinysip/headers/tsip_header_Expires.h"
#include "tinysip/headers/tsip_header_From.h"
#include "tinysip/headers/tsip_header_Max_Forwards.h"
#include "tinysip/headers/tsip_header_Min_Expires.h"
#include "tinysip/headers/tsip_header_Path.h"
#include "tinysip/headers/tsip_header_P_Access_Network_Info.h" 
#include "tinysip/headers/tsip_header_P_Preferred_Identity.h"
#include "tinysip/headers/tsip_header_Privacy.h"
#include "tinysip/headers/tsip_header_Proxy_Authenticate.h"
#include "tinysip/headers/tsip_header_Proxy_Authorization.h"
#include "tinysip/headers/tsip_header_Record_Route.h"
#include "tinysip/headers/tsip_header_Require.h"
#include "tinysip/headers/tsip_header_Route.h"
#include "tinysip/headers/tsip_header_Service_Route.h"
#include "tinysip/headers/tsip_header_Supported.h"
#include "tinysip/headers/tsip_header_To.h"
#include "tinysip/headers/tsip_header_User_Agent.h"
#include "tinysip/headers/tsip_header_Via.h"
#include "tinysip/headers/tsip_header_WWW_Authenticate.h"

#include "tsk_debug.h"

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_headers;


	# /*== Accept: ==*/
	action parse_header_Accept
	{
		TSK_DEBUG_ERROR("parse_header_Accept NOT IMPLEMENTED");
	}

	# /*== Accept-Contact: ==*/
	action parse_header_Accept_Contact 
	{
		TSK_DEBUG_ERROR("parse_header_Accept_Contact NOT IMPLEMENTED");
	}

	# /*== Accept-Encoding: ==*/
	action parse_header_Accept_Encoding
	{
		TSK_DEBUG_ERROR("parse_header_Accept_Encoding NOT IMPLEMENTED");
	}

	# /*== Accept-Language: ==*/
	action parse_header_Accept_Language
	{
		TSK_DEBUG_ERROR("parse_header_Accept_Language NOT IMPLEMENTED");
	}

	# /*== Accept-Resource-Priority : ==*/
	action parse_header_Accept_Resource_Priority 
	{
		TSK_DEBUG_ERROR("parse_header_Accept_Resource_Priority NOT IMPLEMENTED");
	}

	# /*== Alert-Info: ==*/
	action parse_header_Alert_Info
	{
		TSK_DEBUG_ERROR("parse_header_Alert_Info NOT IMPLEMENTED");
	}

	# /*== Allow: ==*/
	action parse_header_Allow
	{
		tsip_header_Allow_t *header = tsip_header_Allow_parse(state->tag_start, (state->tag_end-state->tag_start));
		if(header)
		{
			tsk_list_push_back_data(message->headers, ((void**) &header));
		}
	}

	# /*== Allow-Events: ==*/
	action parse_header_Allow_Events
	{
		tsip_header_Allow_Events_t *header = tsip_header_Allow_Events_parse(state->tag_start, (state->tag_end-state->tag_start));
		if(header)
		{
			tsk_list_push_back_data(message->headers, ((void**) &header));
		}
	}

	# /*== Authentication-Info: ==*/
	action parse_header_Authentication_Info 
	{
		TSK_DEBUG_ERROR("parse_header_Authentication_Info NOT IMPLEMENTED");
	}

	# /*== Authorization: ==*/
	action parse_header_Authorization 
	{
		tsip_header_Authorization_t *header = tsip_header_Authorization_parse(state->tag_start, (state->tag_end-state->tag_start));
		if(header)
		{
			tsk_list_push_back_data(message->headers, ((void**) &header));
		}
	}

	# /*== Call-ID: ==*/
	action parse_header_Call_ID
	{
		if(!message->Call_ID)
		{
			message->Call_ID = tsip_header_Call_ID_parse(state->tag_start, (state->tag_end-state->tag_start));
		}
	}

	# /*== Call-Info: ==*/
	action parse_header_Call_Info
	{
		TSK_DEBUG_ERROR("parse_header_Call_Info NOT IMPLEMENTED");
	}

	# /*== Contact: ==*/
	action parse_header_Contact 
	{
		tsip_header_Contacts_L_t* headers =  tsip_header_Contact_parse(state->tag_start, (state->tag_end-state->tag_start));
		if(headers)
		{
			tsk_list_item_t *item;
			tsk_list_foreach(item, headers)
			{
				tsip_header_Contact_t *hdr = tsk_object_ref(item->data);
				if(!message->Contact)
				{
					message->Contact = hdr;
				}
				else
				{
					tsk_list_push_back_data(message->headers, ((void**) &hdr));
				}
			}

			TSK_OBJECT_SAFE_FREE(headers);
		}
	}

	# /*== Content-Disposition: ==*/
	action parse_header_Content_Disposition
	{
		TSK_DEBUG_ERROR("parse_header_Content_Disposition NOT IMPLEMENTED");
	}

	# /*== Content-Encoding: ==*/
	action parse_header_Content_Encoding
	{
		TSK_DEBUG_ERROR("PARSE_HEADER_ACCEPT NOT IMPLEMENTED");
	}

	# /*== Content-Language: ==*/
	action parse_header_Content_Language
	{
		TSK_DEBUG_ERROR("parse_header_Content_Language NOT IMPLEMENTED");
	}

	# /*== Content-Length: ==*/
	action parse_header_Content_Length
	{
		if(!message->Content_Length)
		{
			message->Content_Length = tsip_header_Content_Length_parse(state->tag_start, (state->tag_end-state->tag_start));
		}
	}

	# /*== Content-Type: ==*/
	action parse_header_Content_Type
	{
		if(!message->Content_Type)
		{
			message->Content_Type = tsip_header_Content_Type_parse(state->tag_start, (state->tag_end-state->tag_start));
		}
	}

	# /*== CSeq: ==*/
	action parse_header_CSeq
	{
		if(!message->CSeq)
		{
			message->CSeq = tsip_header_CSeq_parse(state->tag_start, (state->tag_end-state->tag_start));
		}
	}

	# /*== Date: ==*/
	action parse_header_Date
	{
		TSK_DEBUG_ERROR("parse_header_Date NOT IMPLEMENTED");
	}

	# /*== Error-Info: ==*/
	action parse_header_Error_Info
	{
		TSK_DEBUG_ERROR("parse_header_Error_Info NOT IMPLEMENTED");
	}

	# /*== Event: ==*/
	action parse_header_Event
	{
		TSK_DEBUG_ERROR("parse_header_Event NOT IMPLEMENTED");
	}

	# /*== Expires: ==*/
	action parse_header_Expires
	{
		if(!message->Expires)
		{
			message->Expires = tsip_header_Expires_parse(state->tag_start, (state->tag_end-state->tag_start));
		}
	}

	# /*== From: ==*/
	action parse_header_From
	{
		if(!message->From)
		{
			message->From = tsip_header_From_parse(state->tag_start, (state->tag_end-state->tag_start));
		}
	}

	# /*== History-Info: ==*/
	action parse_header_History_Info 
	{
		TSK_DEBUG_ERROR("parse_header_History_Info NOT IMPLEMENTED");
	}

	# /*== Identity: ==*/
	action parse_header_Identity
	{
		TSK_DEBUG_ERROR("parse_header_Identity NOT IMPLEMENTED");
	}

	# /*== Identity-Info: ==*/
	action parse_header_Identity_Info
	{
		TSK_DEBUG_ERROR("parse_header_Identity_Info NOT IMPLEMENTED");
	}

	# /*== In_Reply-To: ==*/
	action parse_header_In_Reply_To
	{
		TSK_DEBUG_ERROR("parse_header_In_Reply_To NOT IMPLEMENTED");
	}

	# /*== Join: ==*/
	action parse_header_Join
	{
		TSK_DEBUG_ERROR("parse_header_Join NOT IMPLEMENTED");
	}

	# /*== Max-Forwards: ==*/
	action parse_header_Max_Forwards
	{
		tsip_header_Max_Forwards_t *header = tsip_header_Max_Forwards_parse(state->tag_start, (state->tag_end-state->tag_start));
		if(header)
		{
			tsk_list_push_back_data(message->headers, ((void**) &header));
		}
	}

	# /*== MIME-Version: ==*/
	action parse_header_MIME_Version
	{
		TSK_DEBUG_ERROR("parse_header_MIME_Version NOT IMPLEMENTED");
	}

	# /*== Min-Expires: ==*/
	action parse_header_Min_Expires
	{
		tsip_header_Min_Expires_t *header = tsip_header_Min_Expires_parse(state->tag_start, (state->tag_end-state->tag_start));
		if(header)
		{
			tsk_list_push_back_data(message->headers, ((void**) &header));
		}
	}

	# /*== Min-SE: ==*/
	action parse_header_Min_SE
	{
		TSK_DEBUG_ERROR("parse_header_Min_SE NOT IMPLEMENTED");
	}

	# /*== Organization: ==*/
	action parse_header_Organization 
	{
		TSK_DEBUG_ERROR("parse_header_Organization NOT IMPLEMENTED");
	}

	# /*== P-Access-Network-Info: ==*/
	action parse_header_P_Access_Network_Info 
	{
		tsip_header_P_Access_Network_Info_t *header = tsip_header_P_Access_Network_Info_parse(state->tag_start, (state->tag_end-state->tag_start));
		if(header)
		{
			tsk_list_push_back_data(message->headers, ((void**) &header));
		}
	}

	# /*== P-Answer-State: ==*/
	action parse_header_P_Answer_State
	{
		TSK_DEBUG_ERROR("parse_header_P_Answer_State NOT IMPLEMENTED");
	}

	# /*== P-Asserted-Identity: ==*/
	action parse_header_P_Asserted_Identity 
	{
		TSK_DEBUG_ERROR("parse_header_P_Asserted_Identity NOT IMPLEMENTED");
	}

	# /*== P-Associated-URI: ==*/
	action parse_header_P_Associated_URI 
	{
		TSK_DEBUG_ERROR("parse_header_P_Associated_URI NOT IMPLEMENTED");
	}

	# /*== P-Called-Party-ID: ==*/
	action parse_header_P_Called_Party_ID
	{
		TSK_DEBUG_ERROR("parse_header_P_Called_Party_ID NOT IMPLEMENTED");
	}

	# /*== P-Charging-Function-Addresses : ==*/
	action parse_header_P_Charging_Function_Addresses 
	{
		TSK_DEBUG_ERROR("parse_header_P_Charging_Function_Addresses NOT IMPLEMENTED");
	}

	# /*== P_Charging_Vector: ==*/
	action parse_header_P_Charging_Vector
	{
		TSK_DEBUG_ERROR("parse_header_P_Charging_Vector NOT IMPLEMENTED");
	}

	# /*== P-DCS-Billing-Info: ==*/
	action parse_header_P_DCS_Billing_Info
	{
		TSK_DEBUG_ERROR("parse_header_P_DCS_Billing_Info NOT IMPLEMENTED");
	}

	# /*== P-DCS-LAES: ==*/
	action parse_header_P_DCS_LAES
	{
		TSK_DEBUG_ERROR("parse_header_P_DCS_LAES NOT IMPLEMENTED");
	}

	# /*== P-DCS-OSPS: ==*/
	action parse_header_P_DCS_OSPS
	{
		TSK_DEBUG_ERROR("parse_header_P_DCS_OSPS NOT IMPLEMENTED");
	}

	# /*== P-DCS-Redirect: ==*/
	action parse_header_P_DCS_Redirect
	{
		TSK_DEBUG_ERROR("parse_header_P_DCS_Redirect NOT IMPLEMENTED");
	}

	# /*== P-DCS-Trace-Party-ID: ==*/
	action parse_header_P_DCS_Trace_Party_ID
	{
		TSK_DEBUG_ERROR("parse_header_P_DCS_Trace_Party_ID NOT IMPLEMENTED");
	}

	# /*== P-Early-Media: ==*/
	action parse_header_P_Early_Media
	{
		TSK_DEBUG_ERROR("parse_header_P_Early_Media NOT IMPLEMENTED");
	}

	# /*== P-Media-Authorization: ==*/
	action parse_header_P_Media_Authorization
	{
		TSK_DEBUG_ERROR("parse_header_P_Media_Authorization NOT IMPLEMENTED");
	}

	# /*== P-Preferred-Identity: ==*/
	action parse_header_P_Preferred_Identity
	{
		tsip_header_P_Preferred_Identity_t *header = tsip_header_P_Preferred_Identity_parse(state->tag_start, (state->tag_end-state->tag_start));
		if(header)
		{
			tsk_list_push_back_data(message->headers, ((void**) &header));
		}
	}

	# /*== P-Profile-Key: ==*/
	action parse_header_P_Profile_Key
	{
		TSK_DEBUG_ERROR("parse_header_P_Profile_Key NOT IMPLEMENTED");
	}

	# /*== P-User-Database: ==*/
	action parse_header_P_User_Database
	{
		TSK_DEBUG_ERROR("parse_header_P_User_Database NOT IMPLEMENTED");
	}

	# /*== P-Visited-Network-ID: ==*/
	action parse_header_P_Visited_Network_ID
	{
		TSK_DEBUG_ERROR("parse_header_P_Visited_Network_ID NOT IMPLEMENTED");
	}

	# /*== Path: ==*/
	action parse_header_Path
	{
		tsip_header_Paths_L_t* headers =  tsip_header_Path_parse(state->tag_start, (state->tag_end-state->tag_start));
		if(headers)
		{
			tsk_list_item_t *item;
			tsk_list_foreach(item, headers)
			{
				tsip_header_Route_t *hdr = tsk_object_ref(item->data);
				tsk_list_push_back_data(message->headers, ((void**) &hdr));
			}

			TSK_OBJECT_SAFE_FREE(headers);
		}
	}

	# /* == Priority: ==*/
	action parse_header_Priority
	{
		TSK_DEBUG_ERROR("parse_header_Priority NOT IMPLEMENTED");
	}

	# /*== Privacy: ==*/
	action parse_header_Privacy
	{
		tsip_header_Privacy_t *header = tsip_header_Privacy_parse(state->tag_start, (state->tag_end-state->tag_start));
		if(header)
		{
			tsk_list_push_back_data(message->headers, ((void**) &header));
		}
	}

	# /*== Authenticate: ==*/
	action parse_header_Proxy_Authenticate
	{
		tsip_header_Proxy_Authenticate_t *header = tsip_header_Proxy_Authenticate_parse(state->tag_start, (state->tag_end-state->tag_start));
		if(header)
		{
			tsk_list_push_back_data(message->headers, ((void**) &header));
		}
	}

	# /*== Proxy-Authorization: ==*/
	action parse_header_Proxy_Authorization
	{
		tsip_header_Proxy_Authorization_t *header = tsip_header_Proxy_Authorization_parse(state->tag_start, (state->tag_end-state->tag_start));
		if(header)
		{
			tsk_list_push_back_data(message->headers, ((void**) &header));
		}
	}

	# /*== Proxy-Require: ==*/
	action parse_header_Proxy_Require 
	{
		TSK_DEBUG_ERROR("parse_header_Proxy_Require NOT IMPLEMENTED");
	}

	# /*== RAck: ==*/
	action parse_header_RAck
	{
		TSK_DEBUG_ERROR("parse_header_RAck NOT IMPLEMENTED");
	}

	# /*== Reason: ==*/
	action parse_header_Reason 
	{
		TSK_DEBUG_ERROR("parse_header_Reason NOT IMPLEMENTED");
	}

	# /*== Record-Route: ==*/
	action parse_header_Record_Route 
	{
		tsip_header_Record_Route_t *header = tsip_header_Record_Route_parse(state->tag_start, (state->tag_end-state->tag_start));
		if(header)
		{
			tsk_list_push_back_data(message->headers, ((void**) &header));
		}
	}

	# /*== Refer-Sub: ==*/
	action parse_header_Refer_Sub 
	{
		TSK_DEBUG_ERROR("parse_header_Refer_Sub NOT IMPLEMENTED");
	}

	# /*== Refer-To: ==*/
	action parse_header_Refer_To
	{
		TSK_DEBUG_ERROR("parse_header_Refer_To NOT IMPLEMENTED");
	}

	# /*== Referred-By: ==*/
	action parse_header_Referred_By
	{
		TSK_DEBUG_ERROR("parse_header_Referred_By NOT IMPLEMENTED");
	}

	# /*== Reject-Contact: ==*/
	action parse_header_Reject_Contact
	{
		TSK_DEBUG_ERROR("parse_header_Reject_Contact NOT IMPLEMENTED");
	}

	# /*== Replaces: ==*/
	action parse_header_Replaces
	{
		TSK_DEBUG_ERROR("parse_header_Replaces NOT IMPLEMENTED");
	}

	# /*== Reply-To: ==*/
	action parse_header_Reply_To
	{
		TSK_DEBUG_ERROR("parse_header_Reply_To NOT IMPLEMENTED");
	}

	# /*== Request-Disposition: ==*/
	action parse_header_Request_Disposition
	{
		TSK_DEBUG_ERROR("parse_header_Request_Disposition NOT IMPLEMENTED");
	}

	# /*== Require: ==*/
	action parse_header_Require
	{
		tsip_header_Require_t *header = tsip_header_Require_parse(state->tag_start, (state->tag_end-state->tag_start));
		if(header)
		{
			tsk_list_push_back_data(message->headers, ((void**) &header));
		}
	}

	# /*== Resource-Priority: ==*/
	action parse_header_Resource_Priority
	{
		TSK_DEBUG_ERROR("parse_header_Resource_Priority NOT IMPLEMENTED");
	}

	# /*== Retry-After: ==*/
	action parse_header_Retry_After
	{
		TSK_DEBUG_ERROR("parse_header_Retry_After NOT IMPLEMENTED");
	}

	# /*== Route: ==*/
	action parse_header_Route
	{
		tsip_header_Routes_L_t* headers =  tsip_header_Route_parse(state->tag_start, (state->tag_end-state->tag_start));
		if(headers)
		{
			tsk_list_item_t *item;
			tsk_list_foreach(item, headers)
			{
				tsip_header_Route_t *hdr = tsk_object_ref(item->data);
				tsk_list_push_back_data(message->headers, ((void**) &hdr));
			}

			TSK_OBJECT_SAFE_FREE(headers);
		}
	}

	# /*== RSeq: ==*/
	action parse_header_RSeq
	{
		TSK_DEBUG_ERROR("parse_header_RSeq NOT IMPLEMENTED");
	}

	# /*== Security_Client: ==*/
	action parse_header_Security_Client
	{
		TSK_DEBUG_ERROR("parse_header_Security_Client NOT IMPLEMENTED");
	}

	# /*== Security-Server: ==*/
	action parse_header_Security_Server
	{
		TSK_DEBUG_ERROR("parse_header_Security_Server NOT IMPLEMENTED");
	}

	# /*== Security-Verify: ==*/
	action parse_header_Security_Verify
	{
		TSK_DEBUG_ERROR("parse_header_Security_Verify NOT IMPLEMENTED");
	}

	# /*== Server: ==*/
	action parse_header_Server
	{
		TSK_DEBUG_ERROR("parse_header_Server NOT IMPLEMENTED");
	}

	# /*== Service-Route: ==*/
	action parse_header_Service_Route
	{
		tsip_header_Service_Routes_L_t* headers =  tsip_header_Service_Route_parse(state->tag_start, (state->tag_end-state->tag_start));
		if(headers)
		{
			tsk_list_item_t *item;
			tsk_list_foreach(item, headers)
			{
				tsip_header_Service_Route_t *hdr = tsk_object_ref(item->data);
				tsk_list_push_back_data(message->headers, ((void**) &hdr));
			}

			TSK_OBJECT_SAFE_FREE(headers);
		}
	}

	# /*== Session-Expires: ==*/
	action parse_header_Session_Expires
	{
		TSK_DEBUG_ERROR("parse_header_Session_Expires NOT IMPLEMENTED");
	}

	# /*== SIP-ETag: ==*/
	action parse_header_SIP_ETag
	{
		TSK_DEBUG_ERROR("parse_header_SIP_ETag NOT IMPLEMENTED");
	}

	# /*== SIP-If-Match: ==*/
	action parse_header_SIP_If_Match
	{
		TSK_DEBUG_ERROR("parse_header_SIP_If_Match NOT IMPLEMENTED");
	}

	# /*== Subject: ==*/
	action parse_header_Subject
	{
		TSK_DEBUG_ERROR("parse_header_Subject NOT IMPLEMENTED");
	}

	# /*== Subscription-State: ==*/
	action parse_header_Subscription_State
	{
		TSK_DEBUG_ERROR("PARSE_HEADER_ACCEPT NOT IMPLEMENTED");
	}

	# /*== Supported: ==*/
	action parse_header_Supported
	{
		tsip_header_Supported_t *header = tsip_header_Supported_parse(state->tag_start, (state->tag_end-state->tag_start));
		if(header)
		{
			tsk_list_push_back_data(message->headers, ((void**) &header));
		}
	}

	# /*== Target-Dialog: ==*/
	action parse_header_Target_Dialog
	{
		TSK_DEBUG_ERROR("parse_header_Target_Dialog NOT IMPLEMENTED");
	}

	# /*== Timestamp: ==*/
	action parse_header_Timestamp
	{
		TSK_DEBUG_ERROR("parse_header_Timestamp NOT IMPLEMENTED");
	}

	# /*== To: ==*/
	action parse_header_To
	{
		if(!message->To)
		{
			message->To = tsip_header_To_parse(state->tag_start, (state->tag_end-state->tag_start));
		}
	}

	# /*== Unsupported: ==*/
	action parse_header_Unsupported
	{
		TSK_DEBUG_ERROR("parse_header_Unsupported NOT IMPLEMENTED");
	}

	# /*== User-Agent: ==*/
	action parse_header_User_Agent
	{
		tsip_header_User_Agent_t *header = tsip_header_User_Agent_parse(state->tag_start, (state->tag_end-state->tag_start));
		if(header)
		{
			tsk_list_push_back_data(message->headers, ((void**) &header));
		}
	}

	# /*== Via: ==*/
	action parse_header_Via
	{		
		if(!message->firstVia)
		{
			message->firstVia = tsip_header_Via_parse(state->tag_start, (state->tag_end-state->tag_start));
		}
		else
		{
			tsip_header_Via_t *header = tsip_header_Via_parse(state->tag_start, (state->tag_end-state->tag_start));
			if(header)
			{
				tsk_list_push_back_data(message->headers, ((void**) &header));
			}
		}
	}

	# /*== Warning: ==*/
	action parse_header_Warning 
	{
		TSK_DEBUG_ERROR("parse_header_Warning NOT IMPLEMENTED");
	}

	# /*== WWW-Authenticate: ==*/
	action parse_header_WWW_Authenticate
	{
		tsip_header_WWW_Authenticate_t *header = tsip_header_WWW_Authenticate_parse(state->tag_start, (state->tag_end-state->tag_start));
		if(header)
		{
			tsk_list_push_back_data(message->headers, ((void**) &header));
		}
	}
		
	# /*== extension_header: ==*/
	action parse_header_extension_header
	{
		TSK_DEBUG_ERROR("parse_header_extension_header NOT IMPLEMENTED");
	}


	# Includes
	include tsip_machine_utils "./tsip_machine_utils.rl";
	include tsip_machine_header "./tsip_machine_header.rl";

	# Entry point
	main := HEADER;
}%%

TSIP_BOOLEAN tsip_header_parse(tsip_ragel_state_t *state, tsip_message_t *message)
{
	int cs = 0;
	const char *p = state->tag_start;
	const char *pe = state->tag_end;
	const char *eof = pe;

	%%write data;
	%%write init;
	%%write exec;
	
	return (cs == tsip_machine_parser_headers_first_final);
}