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

/**@file tsip_header.c
 * @brief Defines a SIP header (field-name: field-value).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header.h"

#include "tinysip/headers/tsip_header_Dummy.h"


#include "tsk_debug.h"

/* Compact headers: http://www.cs.columbia.edu/sip/compact.html
Abbreviation 	Header 					defined by 					origin (mnemonic)
a 				Accept-Contact 			draft-ietf-sip-callerprefs 	--
b 				Referred-By 			-refer- 					"by"
c 				Content-Type 			RFC 3261
d				Request-Disposition
e 				Content-Encoding 		RFC 3261
f 				From 					RFC 3261
i 				Call-ID 				RFC 3261
j				Reject-Contact
k 				Supported 				RFC 3261 					"know"
l 				Content-Length 			RFC 3261
m 				Contact 				RFC 3261 					"moved"
n				Identity-Info
o 				Event 					-event- 					"occurance"
r 				Refer-To 				-refer-
s 				Subject 				RFC 3261
t 				To 						RFC 3261
u 				Allow-Events 			-events- 					"understand"
v 				Via 					RFC 3261
y				Identity
*/

#if TSIP_COMPACT_HEADERS
#	define _Accept_Contact "a"
#	define _Referred_By  "b"
#	define _Content_Type  "c"
#	define _Request_Disposition "d"
#	define _Content_Encoding  "e"
#	define _From  "f"
#	define _Call_ID  "i"
#	define _Reject_Contact "j";
#	define _Supported  "k"
#	define _Content_Length  "l"
#	define _Contact  "m"
#	define _Identity_Info  "n"
#	define _Event  "o"
#	define _Refer_To  "r"
#	define _Subject  "s"
#	define _To  "t";
#	define _Allow_Events  "u"
#	define _Via  "v"
#	define _Session_Expires "x"
#	define _Identity  "y"
#else
#	define _Accept_Contact "Accept-Contact"
#	define _Referred_By  "Referred-By"
#	define _Content_Type  "Content-Type"
#	define _Request_Disposition "Request-Disposition"
#	define _Content_Encoding  "Content-Encoding"
#	define _From  "From"
#	define _Call_ID  "Call-ID"
#	define _Reject_Contact "Reject-Contact"
#	define _Supported  "Supported"
#	define _Content_Length  "Content-Length"
#	define _Contact  "Contact"
#	define _Identity_Info  "Identity-Info"
#	define _Event  "Event"
#	define _Refer_To  "Refer-To"
#	define _Subject  "Subject"
#	define _To  "To"
#	define _Allow_Events  "Allow-Events"
#	define _Via  "Via"
#	define _Session_Expires "Session-Expires"
#	define _Identity  "Identity"
#endif

/** Gets the name of the SIP header with a type equal to @a type.
 * @param	type	The @a type of the header for which to retrieve the name.
 *
 * @return	The name of the header.
**/
const char *tsip_header_get_name(tsip_header_type_t type)
{
    switch(type) {
    case tsip_htype_Accept:
        return "Accept";
    case tsip_htype_Accept_Contact:
        return _Accept_Contact;
    case tsip_htype_Accept_Encoding:
        return "Accept-Encoding";
    case tsip_htype_Accept_Language:
        return "Accept-Language";
    case tsip_htype_Accept_Resource_Priority:
        return "Accept-Resource-Priority";
    case tsip_htype_Alert_Info:
        return "Alert-Info";
    case tsip_htype_Allow:
        return "Allow";
    case tsip_htype_Allow_Events:
        return _Allow_Events;
    case tsip_htype_Authentication_Info:
        return "Authentication-Info";
    case tsip_htype_Authorization:
        return "Authorization";
    case tsip_htype_Call_ID:
        return _Call_ID;
    case tsip_htype_Call_Info:
        return "Call-Info";
    case tsip_htype_Contact:
        return _Contact;
    case tsip_htype_Content_Disposition:
        return "Content-Disposition";
    case tsip_htype_Content_Encoding:
        return _Content_Encoding;
    case tsip_htype_Content_Language:
        return "Content-Language";
    case tsip_htype_Content_Length:
        return _Content_Length;
    case tsip_htype_Content_Type:
        return _Content_Type;
    case tsip_htype_CSeq:
        return "CSeq";
    case tsip_htype_Date:
        return "Date";
    case tsip_htype_Error_Info:
        return "Error-Info";
    case tsip_htype_Event:
        return _Event;
    case tsip_htype_Expires:
        return "Expires";
    case tsip_htype_From:
        return _From;
    case tsip_htype_History_Info:
        return "History-Info";
    case tsip_htype_Identity:
        return _Identity;
    case tsip_htype_Identity_Info:
        return _Identity_Info;
    case tsip_htype_In_Reply_To:
        return "In-Reply-To";
    case tsip_htype_Join:
        return "Join";
    case tsip_htype_Max_Forwards:
        return "Max-Forwards";
    case tsip_htype_MIME_Version:
        return "MIME-Version";
    case tsip_htype_Min_Expires:
        return "Min-Expires";
    case tsip_htype_Min_SE:
        return "Min-SE";
    case tsip_htype_Organization:
        return "Organization";
    case tsip_htype_Path:
        return "Path";
    case tsip_htype_Priority:
        return "Priority";
    case tsip_htype_Privacy:
        return "Privacy";
    case tsip_htype_Proxy_Authenticate:
        return "Proxy-Authenticate";
    case tsip_htype_Proxy_Authorization:
        return "Proxy-Authorization";
    case tsip_htype_Proxy_Require:
        return "Proxy-Require";
    case tsip_htype_RAck:
        return "RAck";
    case tsip_htype_Reason:
        return "Reason";
    case tsip_htype_Record_Route:
        return "Record-Route";
    case tsip_htype_Refer_Sub:
        return "Refer-Sub";
    case tsip_htype_Refer_To:
        return _Refer_To;
    case tsip_htype_Referred_By:
        return _Referred_By;
    case tsip_htype_Reject_Contact:
        return _Reject_Contact;
    case tsip_htype_Replaces:
        return "Replaces";
    case tsip_htype_Reply_To:
        return "Reply-To";
    case tsip_htype_Request_Disposition:
        return _Request_Disposition;
    case tsip_htype_Require:
        return "Require";
    case tsip_htype_Resource_Priority:
        return "Resource-Priority";
    case tsip_htype_Retry_After:
        return "Retry-After";
    case tsip_htype_Route:
        return "Route";
    case tsip_htype_RSeq:
        return "RSeq";
    case tsip_htype_Security_Client:
        return "Security-Client";
    case tsip_htype_Security_Server:
        return "Security-Server";
    case tsip_htype_Security_Verify:
        return "Security-Verify";
    case tsip_htype_Server:
        return "Server";
    case tsip_htype_Service_Route:
        return "Service-Route";
    case tsip_htype_Session_Expires:
        return _Session_Expires;
    case tsip_htype_SIP_ETag:
        return "SIP-ETag";
    case tsip_htype_SIP_If_Match:
        return "SIP-If-Match";
    case tsip_htype_Subject:
        return _Subject;
    case tsip_htype_Subscription_State:
        return "Subscription-State";
    case tsip_htype_Supported:
        return _Supported;
    case tsip_htype_Target_Dialog:
        return "Target-Dialog";
    case tsip_htype_Timestamp:
        return "Timestamp";
    case tsip_htype_To:
        return _To;
    case tsip_htype_Unsupported:
        return "Unsupported";
    case tsip_htype_User_Agent:
        return "User-Agent";
    case tsip_htype_Via:
        return _Via;
    case tsip_htype_Warning:
        return "Warning";
    case tsip_htype_WWW_Authenticate:
        return "WWW-Authenticate";
    case tsip_htype_P_Access_Network_Info:
        return "P-Access-Network-Info";
    case tsip_htype_P_Answer_State:
        return "P-Answer-State";
    case tsip_htype_P_Asserted_Identity:
        return "P-Asserted-Identity";
    case tsip_htype_P_Associated_URI:
        return "P-Associated-URI";
    case tsip_htype_P_Called_Party_ID:
        return "P-Called-Party-ID";
    case tsip_htype_P_Charging_Function_Addresses:
        return "P-Charging-Function-Addresses";
    case tsip_htype_P_Charging_Vector:
        return "P-Charging-Vector";
    case tsip_htype_P_DCS_Billing_Info:
        return "P-DCS-Billing-Info";
    case tsip_htype_P_DCS_LAES:
        return "P-DCS-LAES";
    case tsip_htype_P_DCS_OSPS:
        return "P-DCS-OSPS";
    case tsip_htype_P_DCS_Redirect:
        return "P-DCS-Redirect";
    case tsip_htype_P_DCS_Trace_Party_ID:
        return "P-DCS-Trace-Party-ID";
    case tsip_htype_P_Early_Media:
        return "P-Early-Media";
    case tsip_htype_P_Media_Authorization:
        return "P-Media-Authorization";
    case tsip_htype_P_Preferred_Identity:
        return "P-Preferred-Identity";
    case tsip_htype_P_Profile_Key:
        return "P-Profile-Key";
    case tsip_htype_P_User_Database:
        return "P-User-Database";
    case tsip_htype_P_Visited_Network_ID:
        return "P-Visited-Network-ID";

    default:
        return "unknown-header";
    }
}

const char *tsip_header_get_name_2(const tsip_header_t *self)
{
    if(self) {
        if(self->type == tsip_htype_Dummy) {
            return ((tsip_header_Dummy_t*)self)->name;
        }
        else {
            return tsip_header_get_name(self->type);
        }
    }
    return "unknown-header";
}

char tsip_header_get_param_separator(const tsip_header_t *self)
{
    if(self) {
        switch(self->type) {
        case tsip_htype_Authorization:
        case tsip_htype_Proxy_Authorization:
        case tsip_htype_Proxy_Authenticate:
        case tsip_htype_WWW_Authenticate: {
            return ',';
        }

        default: {
            return ';';
        }
        }
    }
    return 0;
}

int tsip_header_serialize(const tsip_header_t *self, tsk_buffer_t *output)
{
    int ret = -1;
    static const char* hname;
    static char separator;

    if(self && TSIP_HEADER(self)->serialize) {
        tsk_list_item_t *item;

        hname = tsip_header_get_name_2(self);
        ret = 0; // for empty lists

        /* Header name */
        tsk_buffer_append_2(output, "%s: ", hname);

        /*  Header value (likes calling tsip_header_value_serialize() ) */
        if((ret = TSIP_HEADER(self)->serialize(self, output))) {
            // CHECK all headers return value!
            //return ret;
        }

        /* Parameters */
        tsk_list_foreach(item, self->params) {
            tsk_param_t* param = item->data;
            separator = tsip_header_get_param_separator(self);
            if((ret = tsk_buffer_append_2(output, param->value?"%c%s=%s":"%c%s", separator, param->name, param->value))) {
                return ret;
            }
        }

        /* CRLF */
        tsk_buffer_append(output, "\r\n", 2);
    }
    return ret;
}

char* tsip_header_tostring(const tsip_header_t *self)
{
    tsk_buffer_t *output;
    char* ret = tsk_null;
    if(self && (output = tsk_buffer_create_null())) {
        if(!tsip_header_serialize(self, output)) {
            ret = tsk_strndup(output->data, output->size);
        }
        TSK_OBJECT_SAFE_FREE(output);
    }
    return ret;
}

int tsip_header_value_serialize(const tsip_header_t *self, tsk_buffer_t *output)
{
    if(self && output) {
        return TSIP_HEADER(self)->serialize(self, output);
    }
    else {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
}

char* tsip_header_value_tostring(const tsip_header_t *self)
{
    tsk_buffer_t *output;
    char* ret = tsk_null;
    if(self && (output = tsk_buffer_create_null())) {
        if(!tsip_header_value_serialize(self, output)) {
            ret = tsk_strndup(output->data, output->size);
        }
        TSK_OBJECT_SAFE_FREE(output);
    }
    return ret;
}

char* tsip_header_get_param_value(const tsip_header_t *self, const char* pname)
{
    const tsk_param_t* _param;
    char* value = tsk_null;

    if(!self || !pname) {
        return tsk_null;
    }

    if(self->get_special_param_value && (value = self->get_special_param_value(self, pname))) {
        return value;
    }

    if((_param = tsk_params_get_param_by_name(self->params, pname))) {
        return tsk_strdup(_param->value);
    }

    return tsk_null;
}
