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

/**@file tsip_header.c
 * @brief Defines a SIP header (field-name: field-value).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/headers/tsip_header.h"

#include "tinysip/headers/tsip_header_Dummy.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn	const char *tsip_header_get_name(tsip_header_type_t type)
///
/// @brief	Retrieves a header name by type. 
///
/// @author	Mamadou
/// @date	12/2/2009
///
/// @param	type	The type of the headers. 
///
/// @return	The name of the header. 
////////////////////////////////////////////////////////////////////////////////////////////////////
const char *tsip_header_get_name(tsip_header_type_t type)
{
	switch(type)
	{
		case tsip_htype_Accept: return "Accept";
		case tsip_htype_Accept_Contact: return "Accept-Contact";
		case tsip_htype_Accept_Encoding: return "Accept-Encoding";
		case tsip_htype_Accept_Language: return "Accept-Language";
		case tsip_htype_Accept_Resource_Priority: return "Accept-Resource-Priority";
		case tsip_htype_Alert_Info: return "Alert-Info";
		case tsip_htype_Allow: return "Allow";
		case tsip_htype_Allow_Events: return "Allow-Events";
		case tsip_htype_Authentication_Info: return "Authentication-Info";
		case tsip_htype_Authorization: return "Authorization";
		case tsip_htype_Call_ID: return "Call-ID";
		case tsip_htype_Call_Info: return "Call-Info";
		case tsip_htype_Contact: return "Contact";
		case tsip_htype_Content_Disposition: return "Content-Disposition";
		case tsip_htype_Content_Encoding: return "Content-Encoding";
		case tsip_htype_Content_Language: return "Content-Language";
		case tsip_htype_Content_Length: return "Content-Length";
		case tsip_htype_Content_Type: return "Content-Type";
		case tsip_htype_CSeq: return "CSeq";
		case tsip_htype_Date: return "Date";
		case tsip_htype_Error_Info: return "Error-Info";
		case tsip_htype_Event: return "Event";
		case tsip_htype_Expires: return "Expires";
		case tsip_htype_From: return "From";
		case tsip_htype_History_Info: return "History-Info";
		case tsip_htype_Identity: return "Identity";
		case tsip_htype_Identity_Info: return "Identity-Info";
		case tsip_htype_In_Reply_To: return "In-Reply-To";
		case tsip_htype_Join: return "Join";
		case tsip_htype_Max_Forwards: return "Max-Forwards";
		case tsip_htype_MIME_Version: return "MIME-Version";
		case tsip_htype_Min_Expires: return "Min-Expires";
		case tsip_htype_Min_SE: return "Min-SE";
		case tsip_htype_Organization: return "Organization";
		case tsip_htype_Path: return "Path";
		case tsip_htype_Priority: return "Priority";
		case tsip_htype_Privacy: return "Privacy";
		case tsip_htype_Proxy_Authenticate: return "Proxy-Authenticate";
		case tsip_htype_Proxy_Authorization: return "Proxy-Authorization";
		case tsip_htype_Proxy_Require: return "Proxy-Require";
		case tsip_htype_RAck: return "RAck";
		case tsip_htype_Reason: return "Reason";
		case tsip_htype_Record_Route: return "Record-Route";
		case tsip_htype_Refer_Sub: return "Refer-Sub";
		case tsip_htype_Refer_To: return "Refer-To";
		case tsip_htype_Referred_By: return "Referred-By";
		case tsip_htype_Reject_Contact: return "Reject-Contact";
		case tsip_htype_Replaces: return "Replaces";
		case tsip_htype_Reply_To: return "Reply-To";
		case tsip_htype_Request_Disposition: return "Request-Disposition";
		case tsip_htype_Require: return "Require";
		case tsip_htype_Resource_Priority: return "Resource-Priority";
		case tsip_htype_Retry_After: return "Retry-After";
		case tsip_htype_Route: return "Route";
		case tsip_htype_RSeq: return "RSeq";
		case tsip_htype_Security_Client: return "Security-Client";
		case tsip_htype_Security_Server: return "Security-Server";
		case tsip_htype_Security_Verify: return "Security-Verify";
		case tsip_htype_Server: return "Server";
		case tsip_htype_Service_Route: return "Service-Route";
		case tsip_htype_Session_Expires: return "Session-Expires";
		case tsip_htype_SIP_ETag: return "SIP-ETag";
		case tsip_htype_SIP_If_Match: return "SIP-If-Match";
		case tsip_htype_Subject: return "Subject";
		case tsip_htype_Subscription_State: return "Subscription-State";
		case tsip_htype_Supported: return "Supported";
		case tsip_htype_Target_Dialog: return "Target-Dialog";
		case tsip_htype_Timestamp: return "Timestamp";
		case tsip_htype_To: return "To";
		case tsip_htype_Unsupported: return "Unsupported";
		case tsip_htype_User_Agent: return "User-Agent";
		case tsip_htype_Via: return "Via";
		case tsip_htype_Warning: return "Warning";
		case tsip_htype_WWW_Authenticate: return "WWW-Authenticate";
		case tsip_htype_P_Access_Network_Info: return "P-Access-Network-Info";
		case tsip_htype_P_Answer_State: return "P-Answer-State";
		case tsip_htype_P_Asserted_Identity: return "P-Asserted-Identity";
		case tsip_htype_P_Associated_URI: return "P-Associated-URI";
		case tsip_htype_P_Called_Party_ID: return "P-Called-Party-ID";
		case tsip_htype_P_Charging_Function_Addresses: return "P-Charging-Function-Addresses";
		case tsip_htype_P_Charging_Vector: return "P-Charging-Vector";
		case tsip_htype_P_DCS_Billing_Info: return "P-DCS-Billing-Info";
		case tsip_htype_P_DCS_LAES: return "P-DCS-LAES";
		case tsip_htype_P_DCS_OSPS: return "P-DCS-OSPS";
		case tsip_htype_P_DCS_Redirect: return "P-DCS-Redirect";
		case tsip_htype_P_DCS_Trace_Party_ID: return "P-DCS-Trace-Party-ID";
		case tsip_htype_P_Early_Media: return "P-Early-Media";
		case tsip_htype_P_Media_Authorization: return "P-Media-Authorization";
		case tsip_htype_P_Preferred_Identity: return "P-Preferred-Identity";
		case tsip_htype_P_Profile_Key: return "P-Profile-Key";
		case tsip_htype_P_User_Database: return "P-User-Database";
		case tsip_htype_P_Visited_Network_ID: return "P-Visited-Network-ID";

		default: return "unknown-header";
	}
}

const char *tsip_header_get_nameex(const tsip_header_t *self)
{
	if(self){
		if(self->type == tsip_htype_Dummy){
			return ((tsip_header_Dummy_t*)self)->name;
		}
		else{
			return tsip_header_get_name(self->type);
		}
	}
	return "unknown-header";
}

char tsip_header_get_param_separator(const tsip_header_t *self)
{
	if(self)
	{
		switch(self->type)
		{
		case tsip_htype_Authorization:
		case tsip_htype_Proxy_Authorization:
		case tsip_htype_Proxy_Authenticate:
		case tsip_htype_WWW_Authenticate:
			{
				return ',';
			}

		default:
			{
				return ';';
			}
		}
	}
	return 0;
}

int tsip_header_tostring(const tsip_header_t *self, tsk_buffer_t *output)
{
	int ret = -1;
	static const char* hname;
	static char separator;

	if(self && TSIP_HEADER(self)->tostring)
	{
		tsk_list_item_t *item;
		
		hname = tsip_header_get_nameex(self);
		ret = 0; // for empty lists

		/* Header name */
		tsk_buffer_appendEx(output, "%s: ", hname);

		/*  Header value.*/
		if((ret = TSIP_HEADER(self)->tostring(self, output))){
			// CHECK all headers return value!
			//return ret;
		}

		/* Parameters */
		tsk_list_foreach(item, self->params)
		{
			tsk_param_t* param = item->data;
			separator = tsip_header_get_param_separator(self);
			if(ret = tsk_buffer_appendEx(output, param->value?"%c%s=%s":"%c%s", separator, param->name, param->value)){
				return ret;
			}
		}

		/* CRLF */
		tsk_buffer_append(output, "\r\n", 2);
	}
	return ret;
}
