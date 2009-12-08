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

/**@file tsip_header.h
 * @brief Defines a SIP header (field-name: field-value).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSIP_HEADER_H
#define TINYSIP_HEADER_H

#include "tinysip_config.h"

#include "tinysip/parsers/tsip_ragel_state.h"

#include "tsk_params.h"
#include "tsk_object.h"
#include "tsk_safeobj.h"
#include "tsk_memory.h"
#include "tsk_string.h"

#define TSIP_HEADER(hdr)	((tsip_header_t*)(hdr))
#define TSIP_HEADER_CREATE()

/**
 * @enum	tsip_header_type_e
 *
 * @brief	List of all supported headers.
**/
typedef enum tsip_header_type_e
{
	tsip_htype_Accept,
	tsip_htype_Accept_Contact,
	tsip_htype_Accept_Encoding,
	tsip_htype_Accept_Language,
	tsip_htype_Accept_Resource_Priority,
	tsip_htype_Alert_Info,
	tsip_htype_Allow,
	tsip_htype_Allow_Events,
	tsip_htype_Authentication_Info,
	tsip_htype_Authorization,
	tsip_htype_Call_ID,
	tsip_htype_Call_Info,
	tsip_htype_Contact,
	tsip_htype_Content_Disposition,
	tsip_htype_Content_Encoding,
	tsip_htype_Content_Language,
	tsip_htype_Content_Length,
	tsip_htype_Content_Type,
	tsip_htype_CSeq,
	tsip_htype_Date,
	tsip_htype_Error_Info,
	tsip_htype_Event,
	tsip_htype_Expires,
	tsip_htype_From,
	tsip_htype_History_Info,
	tsip_htype_Identity,
	tsip_htype_Identity_Info,
	tsip_htype_In_Reply_To,
	tsip_htype_Join,
	tsip_htype_Max_Forwards,
	tsip_htype_MIME_Version,
	tsip_htype_Min_Expires,
	tsip_htype_Min_SE,
	tsip_htype_Organization,
	tsip_htype_Path,
	tsip_htype_Priority,
	tsip_htype_Privacy,
	tsip_htype_Proxy_Authenticate,
	tsip_htype_Proxy_Authorization,
	tsip_htype_Proxy_Require,
	tsip_htype_RAck,
	tsip_htype_Reason,
	tsip_htype_Record_Route,
	tsip_htype_Refer_Sub,
	tsip_htype_Refer_To,
	tsip_htype_Referred_By,
	tsip_htype_Reject_Contact,
	tsip_htype_Replaces,
	tsip_htype_Reply_To,
	tsip_htype_Request_Disposition,
	tsip_htype_Require,
	tsip_htype_Resource_Priority,
	tsip_htype_Retry_After,
	tsip_htype_Route,
	tsip_htype_RSeq,
	tsip_htype_Security_Client,
	tsip_htype_Security_Server,
	tsip_htype_Security_Verify,
	tsip_htype_Server,
	tsip_htype_Service_Route,
	tsip_htype_Session_Expires,
	tsip_htype_SIP_ETag,
	tsip_htype_SIP_If_Match,
	tsip_htype_Subject,
	tsip_htype_Subscription_State,
	tsip_htype_Supported,
	tsip_htype_Target_Dialog,
	tsip_htype_Timestamp,
	tsip_htype_To,
	tsip_htype_Unsupported,
	tsip_htype_User_Agent,
	tsip_htype_Via,
	tsip_htype_Warning,
	tsip_htype_WWW_Authenticate,
	tsip_htype_P_Access_Network_Info,
	tsip_htype_P_Answer_State,
	tsip_htype_P_Asserted_Identity,
	tsip_htype_P_Associated_URI,
	tsip_htype_P_Called_Party_ID,
	tsip_htype_P_Charging_Function_Addresses,
	tsip_htype_P_Charging_Vector,
	tsip_htype_P_DCS_Billing_Info,
	tsip_htype_P_DCS_LAES,
	tsip_htype_P_DCS_OSPS,
	tsip_htype_P_DCS_Redirect,
	tsip_htype_P_DCS_Trace_Party_ID,
	tsip_htype_P_Early_Media,
	tsip_htype_P_Media_Authorization,
	tsip_htype_P_Preferred_Identity,
	tsip_htype_P_Profile_Key,
	tsip_htype_P_User_Database,
	tsip_htype_P_Visited_Network_ID
}
tsip_header_type_t;

#define TSIP_DECLARE_HEADER struct { \
	TSK_DECLARE_OBJECT; \
	tsip_header_type_t type; \
	tsk_params_L_t *params; \
}

typedef TSIP_DECLARE_HEADER tsip_header_t;

TINYSIP_API const char *tsip_header_get_name(tsip_header_type_t type);
TINYSIP_API void tsip_header_add_param(tsip_header_t *header, const char *name, const char *value);

#define TSIP_HEADER_HAS_PARAM(header, name)					tsk_params_has_param(header ? header->params : 0, name)
#define TSIP_HEADER_GET_PARAM_BY_NAME(header, name)			tsk_params_get_param_by_name(header ? header->params : 0, name)
#define TSIP_HEADER_GET_PARAM_VALUE(header, name)			tsk_params_get_param_value(header ? header->params : 0, name)
#define TSIP_HEADER_GET_PARAM_VALUE_AS_INT(header, name)	tsk_params_get_param_value_as_int(header ? header->params : 0, name)


#endif /* TINYSIP_HEADERS_H */