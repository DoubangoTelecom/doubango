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

/**@file tsip_parser_header.c
 * @brief SIP headers parser.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/parsers/tsip_parser_header.h"

#include "tinysip/headers/tsip_headers.h"

#include "tsk_debug.h"

#undef ADD_HEADERS
#undef ADD_HEADER

#define ADD_HEADERS(headers)\
	if(headers)\
	{\
		const tsk_list_item_t *item;\
		tsk_list_foreach(item, headers){\
			tsip_header_t *hdr = tsk_object_ref((void*)item->data);\
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


// Check if we have ",CRLF" ==> See WWW-Authenticate header
// As :>CRLF is preceded by any+ ==> p will be at least (start + 1)
// p point to CR
#define prev_not_comma(p) !(p && p[-1] == ',')

/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_headers;


	# /*== Accept: ==*/
	action parse_header_Accept
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Accept NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Accept-Contact: ==*/
	action parse_header_Accept_Contact 
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Accept_Contact NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Accept-Encoding: ==*/
	action parse_header_Accept_Encoding
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Accept_Encoding NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Accept-Language: ==*/
	action parse_header_Accept_Language
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Accept_Language NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Accept-Resource-Priority : ==*/
	action parse_header_Accept_Resource_Priority 
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Accept_Resource_Priority NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Alert-Info: ==*/
	action parse_header_Alert_Info
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Alert_Info NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Allow: ==*/
	action parse_header_Allow
	{
		tsip_header_Allow_t *header = tsip_header_Allow_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== Allow-Events: ==*/
	action parse_header_Allow_Events
	{
		tsip_header_Allow_Events_t *header = tsip_header_Allow_Events_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== Authentication-Info: ==*/
	action parse_header_Authentication_Info 
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Authentication_Info NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Authorization: ==*/
	action parse_header_Authorization 
	{
		tsip_header_Authorization_t *header = tsip_header_Authorization_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== Call-ID: ==*/
	action parse_header_Call_ID
	{
		if(!message->Call_ID){
			message->Call_ID = tsip_header_Call_ID_parse(state->tag_start, (state->tag_end-state->tag_start));
		}
		else{
			tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
			ADD_HEADER(header);
			TSK_DEBUG_WARN("The message already have 'Call-ID' header.");
		}
	}

	# /*== Call-Info: ==*/
	action parse_header_Call_Info
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Call_Info NOT IMPLEMENTED. Will be added as Dummy header.");
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
				if(!message->Contact){
					message->Contact = hdr;
				}
				else{
					tsk_list_push_back_data(message->headers, ((void**) &hdr));
				}
			}

			TSK_OBJECT_SAFE_FREE(headers);
		}
	}

	# /*== Content-Disposition: ==*/
	action parse_header_Content_Disposition
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Content_Disposition NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Content-Encoding: ==*/
	action parse_header_Content_Encoding
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		TSK_DEBUG_WARN("PARSE_HEADER_ACCEPT NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Content-Language: ==*/
	action parse_header_Content_Language
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Content_Language NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Content-Length: ==*/
	action parse_header_Content_Length
	{
		if(!message->Content_Length){
			message->Content_Length = tsip_header_Content_Length_parse(state->tag_start, (state->tag_end-state->tag_start));
		}
		else{
			tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
			ADD_HEADER(header);
			TSK_DEBUG_WARN("The message already have 'Content-Length' header.");
		}
	}

	# /*== Content-Type: ==*/
	action parse_header_Content_Type
	{
		if(!message->Content_Type){
			message->Content_Type = tsip_header_Content_Type_parse(state->tag_start, (state->tag_end-state->tag_start));
		}
		else{
			tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
			ADD_HEADER(header);
			TSK_DEBUG_WARN("The message already have 'Content-Type' header.");
		}
	}

	# /*== CSeq: ==*/
	action parse_header_CSeq
	{
		if(!message->CSeq){
			message->CSeq = tsip_header_CSeq_parse(state->tag_start, (state->tag_end-state->tag_start));
		}
		else{
			tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
			ADD_HEADER(header);
			TSK_DEBUG_WARN("The message already have 'CSeq' header.");
		}
	}

	# /*== Date: ==*/
	action parse_header_Date
	{
		tsip_header_Date_t *header = tsip_header_Date_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== Error-Info: ==*/
	action parse_header_Error_Info
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Error_Info NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Event: ==*/
	action parse_header_Event
	{
		tsip_header_Event_t *header = tsip_header_Event_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== Expires: ==*/
	action parse_header_Expires
	{
		if(!message->Expires){
			message->Expires = tsip_header_Expires_parse(state->tag_start, (state->tag_end-state->tag_start));
		}
		else{
			tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
			ADD_HEADER(header);
			TSK_DEBUG_WARN("The message already have 'Expires' header.");
		}
	}

	# /*== From: ==*/
	action parse_header_From
	{
		if(!message->From){
			message->From = tsip_header_From_parse(state->tag_start, (state->tag_end-state->tag_start));
		}
		else{
			tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
			ADD_HEADER(header);
			TSK_DEBUG_WARN("The message already have 'From' header.");
		}
	}

	# /*== History-Info: ==*/
	action parse_header_History_Info 
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_History_Info NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Identity: ==*/
	action parse_header_Identity
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Identity NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Identity-Info: ==*/
	action parse_header_Identity_Info
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Identity_Info NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== In_Reply-To: ==*/
	action parse_header_In_Reply_To
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_In_Reply_To NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Join: ==*/
	action parse_header_Join
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Join NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Max-Forwards: ==*/
	action parse_header_Max_Forwards
	{
		tsip_header_Max_Forwards_t *header = tsip_header_Max_Forwards_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== MIME-Version: ==*/
	action parse_header_MIME_Version
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_MIME_Version NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Min-Expires: ==*/
	action parse_header_Min_Expires
	{
		tsip_header_Min_Expires_t *header = tsip_header_Min_Expires_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== Min-SE: ==*/
	action parse_header_Min_SE
	{
		tsip_header_Min_SE_t *header = tsip_header_Min_SE_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== Organization: ==*/
	action parse_header_Organization 
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Organization NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== P-Access-Network-Info: ==*/
	action parse_header_P_Access_Network_Info 
	{
		tsip_header_P_Access_Network_Info_t *header = tsip_header_P_Access_Network_Info_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== P-Answer-State: ==*/
	action parse_header_P_Answer_State
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_P_Answer_State NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== P-Asserted-Identity: ==*/
	action parse_header_P_Asserted_Identity 
	{
		tsip_header_P_Asserted_Identities_L_t* headers =  tsip_header_P_Asserted_Identity_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADERS(headers);
	}

	# /*== P-Associated-URI: ==*/
	action parse_header_P_Associated_URI
	{
		tsip_header_P_Associated_URIs_L_t* headers =  tsip_header_P_Associated_URI_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADERS(headers);
	}

	# /*== P-Called-Party-ID: ==*/
	action parse_header_P_Called_Party_ID
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_P_Called_Party_ID NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== P-Charging-Function-Addresses : ==*/
	action parse_header_P_Charging_Function_Addresses 
	{
		tsip_header_P_Charging_Function_Addressess_L_t* headers =  tsip_header_P_Charging_Function_Addresses_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADERS(headers);
	}

	# /*== P_Charging_Vector: ==*/
	action parse_header_P_Charging_Vector
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_P_Charging_Vector NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== P-DCS-Billing-Info: ==*/
	action parse_header_P_DCS_Billing_Info
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_P_DCS_Billing_Info NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== P-DCS-LAES: ==*/
	action parse_header_P_DCS_LAES
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_P_DCS_LAES NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== P-DCS-OSPS: ==*/
	action parse_header_P_DCS_OSPS
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_P_DCS_OSPS NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== P-DCS-Redirect: ==*/
	action parse_header_P_DCS_Redirect
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_P_DCS_Redirect NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== P-DCS-Trace-Party-ID: ==*/
	action parse_header_P_DCS_Trace_Party_ID
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_P_DCS_Trace_Party_ID NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== P-Early-Media: ==*/
	action parse_header_P_Early_Media
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_P_Early_Media NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== P-Media-Authorization: ==*/
	action parse_header_P_Media_Authorization
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_P_Media_Authorization NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== P-Preferred-Identity: ==*/
	action parse_header_P_Preferred_Identity
	{
		tsip_header_P_Preferred_Identity_t *header = tsip_header_P_Preferred_Identity_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== P-Profile-Key: ==*/
	action parse_header_P_Profile_Key
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_P_Profile_Key NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== P-User-Database: ==*/
	action parse_header_P_User_Database
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_P_User_Database NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== P-Visited-Network-ID: ==*/
	action parse_header_P_Visited_Network_ID
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_P_Visited_Network_ID NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Path: ==*/
	action parse_header_Path
	{
		tsip_header_Paths_L_t* headers =  tsip_header_Path_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADERS(headers);
	}

	# /* == Priority: ==*/
	action parse_header_Priority
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Priority NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Privacy: ==*/
	action parse_header_Privacy
	{
		tsip_header_Privacy_t *header = tsip_header_Privacy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== Authenticate: ==*/
	action parse_header_Proxy_Authenticate
	{
		tsip_header_Proxy_Authenticate_t *header = tsip_header_Proxy_Authenticate_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== Proxy-Authorization: ==*/
	action parse_header_Proxy_Authorization
	{
		tsip_header_Proxy_Authorization_t *header = tsip_header_Proxy_Authorization_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== Proxy-Require: ==*/
	action parse_header_Proxy_Require 
	{
		tsip_header_Proxy_Require_t *header = tsip_header_Proxy_Require_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== RAck: ==*/
	action parse_header_RAck
	{
		tsip_header_RAck_t *header = tsip_header_RAck_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== Reason: ==*/
	action parse_header_Reason 
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Reason NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Record-Route: ==*/
	action parse_header_Record_Route 
	{
		tsip_header_Record_Routes_L_t* headers =  tsip_header_Record_Route_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADERS(headers);
	}

	# /*== Refer-Sub: ==*/
	action parse_header_Refer_Sub 
	{
		tsip_header_Refer_Sub_t *header = tsip_header_Refer_Sub_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== Refer-To: ==*/
	action parse_header_Refer_To
	{
		tsip_header_Refer_To_t *header = tsip_header_Refer_To_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== Referred-By: ==*/
	action parse_header_Referred_By
	{
		tsip_header_Referred_By_t *header = tsip_header_Referred_By_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== Reject-Contact: ==*/
	action parse_header_Reject_Contact
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Reject_Contact NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Replaces: ==*/
	action parse_header_Replaces
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Replaces NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Reply-To: ==*/
	action parse_header_Reply_To
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Reply_To NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Request-Disposition: ==*/
	action parse_header_Request_Disposition
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Request_Disposition NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Require: ==*/
	action parse_header_Require
	{
		tsip_header_Require_t *header = tsip_header_Require_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== Resource-Priority: ==*/
	action parse_header_Resource_Priority
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Resource_Priority NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Retry-After: ==*/
	action parse_header_Retry_After
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Retry_After NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Route: ==*/
	action parse_header_Route
	{
		tsip_header_Routes_L_t* headers =  tsip_header_Route_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADERS(headers);
	}

	# /*== RSeq: ==*/
	action parse_header_RSeq
	{
		tsip_header_RSeq_t *header = tsip_header_RSeq_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== Security_Client: ==*/
	action parse_header_Security_Client
	{
		tsip_header_Security_Clients_L_t* headers =  tsip_header_Security_Client_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADERS(headers);
	}

	# /*== Security-Server: ==*/
	action parse_header_Security_Server
	{
		tsip_header_Security_Servers_L_t* headers =  tsip_header_Security_Server_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADERS(headers);
	}

	# /*== Security-Verify: ==*/
	action parse_header_Security_Verify
	{
		tsip_header_Security_Verifies_L_t* headers =  tsip_header_Security_Verify_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADERS(headers);
	}

	# /*== Server: ==*/
	action parse_header_Server
	{
		tsip_header_Server_t *header = tsip_header_Server_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== Service-Route: ==*/
	action parse_header_Service_Route
	{
		tsip_header_Service_Routes_L_t* headers =  tsip_header_Service_Route_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADERS(headers);
	}

	# /*== Session-Expires: ==*/
	action parse_header_Session_Expires
	{
		tsip_header_Session_Expires_t *header = tsip_header_Session_Expires_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== SIP-ETag: ==*/
	action parse_header_SIP_ETag
	{
		tsip_header_SIP_ETag_t *header = tsip_header_SIP_ETag_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== SIP-If-Match: ==*/
	action parse_header_SIP_If_Match
	{
		tsip_header_SIP_If_Match_t *header = tsip_header_SIP_If_Match_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== Subject: ==*/
	action parse_header_Subject
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Subject NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Subscription-State: ==*/
	action parse_header_Subscription_State
	{
		tsip_header_Subscription_State_t* header =  tsip_header_Subscription_State_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== Supported: ==*/
	action parse_header_Supported
	{
		tsip_header_Supported_t *header = tsip_header_Supported_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== Target-Dialog: ==*/
	action parse_header_Target_Dialog
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Target_Dialog NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== Timestamp: ==*/
	action parse_header_Timestamp
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Timestamp NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== To: ==*/
	action parse_header_To
	{
		if(!message->To){
			message->To = tsip_header_To_parse(state->tag_start, (state->tag_end-state->tag_start));
		}
		else{
			tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
			ADD_HEADER(header);
			TSK_DEBUG_WARN("The message already have 'To' header.");
		}
	}

	# /*== Unsupported: ==*/
	action parse_header_Unsupported
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
		//TSK_DEBUG_WARN("parse_header_Unsupported NOT IMPLEMENTED. Will be added as Dummy header.");
	}

	# /*== User-Agent: ==*/
	action parse_header_User_Agent
	{
		tsip_header_User_Agent_t *header = tsip_header_User_Agent_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	# /*== Via: ==*/
	action parse_header_Via
	{	
		tsip_header_Vias_L_t* headers =  tsip_header_Via_parse(state->tag_start, (state->tag_end-state->tag_start));
		if(headers){
			tsk_list_item_t *item;
			tsk_list_foreach(item, headers){
				tsip_header_Via_t *hdr = tsk_object_ref(item->data);
				if(!message->firstVia){
					message->firstVia = hdr;
				}
				else{
					tsk_list_push_back_data(message->headers, ((void**) &hdr));
				}
			}

			TSK_OBJECT_SAFE_FREE(headers);
		}
	}

	# /*== Warning: ==*/
	action parse_header_Warning 
	{
		tsip_header_Warnings_L_t* headers =  tsip_header_Warning_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADERS(headers);
	}

	# /*== WWW-Authenticate: ==*/
	action parse_header_WWW_Authenticate
	{
		tsip_header_WWW_Authenticate_t *header = tsip_header_WWW_Authenticate_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}
		
	# /*== extension_header: ==*/
	action parse_header_extension_header
	{
		tsip_header_Dummy_t *header = tsip_header_Dummy_parse(state->tag_start, (state->tag_end-state->tag_start));
		ADD_HEADER(header);
	}

	action prev_not_comma{
		prev_not_comma(p)
	}

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	include tsip_machine_header "./ragel/tsip_machine_header.rl";

	# Entry point
	main := HEADER;
}%%

tsk_bool_t tsip_header_parse(tsk_ragel_state_t *state, tsip_message_t *message)
{
	int cs = 0;
	const char *p = state->tag_start;
	const char *pe = state->tag_end;
	const char *eof = pe;	

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_headers_first_final);
	(void)(tsip_machine_parser_headers_error);
	(void)(tsip_machine_parser_headers_en_main);
	%%write init;
	%%write exec;
	
	return ( cs >= %%{ write first_final; }%% );
	//return (cs == tsip_machine_parser_headers_first_final);
}
