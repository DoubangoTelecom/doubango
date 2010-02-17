# Copyright (C) 2009 Mamadou Diop.
#
# Contact: Mamadou Diop <diopmamadou@yahoo.fr>
#	
# This file is part of Open Source Doubango Framework.
#
# DOUBANGO is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#	
# DOUBANGO is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#	
# You should have received a copy of the GNU General Public License
# along with DOUBANGO.

LOCAL_PATH := $(DOUBANGO_HOME)/tinySIP/source

include $(CLEAR_VARS)

LOCAL_MODULE    := tinySIP
LOCAL_SRC_FILES := \
	tsip.c\
	tsip_message.c\
	tsip_operation.c\
	tsip_timers.c\
	tsip_uri.c\
	\
	dialogs/tsip_dialog.c\
	dialogs/tsip_dialog_invite.client.c\
	dialogs/tsip_dialog_invite.server.c\
	dialogs/tsip_dialog_layer.c\
	dialogs/tsip_dialog_register.client.c\
	dialogs/tsip_dialog_register.server.c\
	dialogs/tsip_dialog_subscribe.client.c\
	dialogs/tsip_dialog_subscribe.server.c\
	\
	headers/tsip_header.c\
	headers/tsip_header_accept.c\
	headers/tsip_header_Accept_Contact.c\
	headers/tsip_header_Accept_Encoding.c\
	headers/tsip_header_Accept_Language.c\
	headers/tsip_header_Accept_Resource_Priority.c\
	headers/tsip_header_Alert_Info.c\
	headers/tsip_header_Allow.c\
	headers/tsip_header_Allow_Events.c\
	headers/tsip_header_Authentication_Info.c\
	headers/tsip_header_Authorization.c\
	headers/tsip_header_Call_ID.c\
	headers/tsip_header_Call_Info.c\
	headers/tsip_header_Contact.c\
	headers/tsip_header_Content_Disposition.c\
	headers/tsip_header_Content_Encoding.c\
	headers/tsip_header_Content_Language.c\
	headers/tsip_header_Content_Length.c\
	headers/tsip_header_Content_Type.c\
	headers/tsip_header_CSeq.c\
	headers/tsip_header_Date.c\
	headers/tsip_header_Error_Info.c\
	headers/tsip_header_Event.c\
	headers/tsip_header_Expires.c\
	headers/tsip_header_From.c\
	headers/tsip_header_History_Info.c\
	headers/tsip_header_Identity.c\
	headers/tsip_header_Identity_Info.c\
	headers/tsip_header_In_Reply_To.c\
	headers/tsip_header_Join.c\
	headers/tsip_header_Max_Forwards.c\
	headers/tsip_header_MIME_Version.c\
	headers/tsip_header_Min_Expires.c\
	headers/tsip_header_Min_SE.c\
	headers/tsip_header_Organization.c\
	headers/tsip_header_Path.c\
	headers/tsip_header_Priority.c\
	headers/tsip_header_Privacy.c\
	headers/tsip_header_Proxy_Authenticate.c\
	headers/tsip_header_Proxy_Authorization.c\
	headers/tsip_header_Proxy_Require.c\
	headers/tsip_header_P_Access_Network_Info.c\
	headers/tsip_header_P_Answer_State.c\
	headers/tsip_header_P_Asserted_Identity.c\
	headers/tsip_header_P_Associated_URI.c\
	headers/tsip_header_P_Called_Party_ID.c\
	headers/tsip_header_P_Charging_Function_Addresses.c\
	headers/tsip_header_P_Charging_Vector.c\
	headers/tsip_header_P_DCS_Billing_Info.c\
	headers/tsip_header_P_DCS_LAES.c\
	headers/tsip_header_P_DCS_OSPS.c\
	headers/tsip_header_P_DCS_Redirect.c\
	headers/tsip_header_P_DCS_Trace_Party_ID.c\
	headers/tsip_header_P_Early_Media.c\
	headers/tsip_header_P_Media_Authorization.c\
	headers/tsip_header_P_Preferred_Identity.c\
	headers/tsip_header_P_Profile_Key.c\
	headers/tsip_header_P_User_Database.c\
	headers/tsip_header_P_Visited_Network_ID.c\
	headers/tsip_header_RAck.c\
	headers/tsip_header_Reason.c\
	headers/tsip_header_Record_Route.c\
	headers/tsip_header_Referred_By.c\
	headers/tsip_header_Refer_Sub.c\
	headers/tsip_header_Refer_To.c\
	headers/tsip_header_Reject_Contact.c\
	headers/tsip_header_Replaces.c\
	headers/tsip_header_Reply_To.c\
	headers/tsip_header_Request_Disposition.c\
	headers/tsip_header_Require.c\
	headers/tsip_header_Resource_Priority.c\
	headers/tsip_header_Retry_After.c\
	headers/tsip_header_Route.c\
	headers/tsip_header_RSeq.c\
	headers/tsip_header_Security_Client.c\
	headers/tsip_header_Security_Server.c\
	headers/tsip_header_Security_Verify.c\
	headers/tsip_header_Server.c\
	headers/tsip_header_Service_Route.c\
	headers/tsip_header_Session_Expires.c\
	headers/tsip_header_SIP_ETag.c\
	headers/tsip_header_SIP_If_Match.c\
	headers/tsip_header_Subject.c\
	headers/tsip_header_Subscription_State.c\
	headers/tsip_header_Supported.c\
	headers/tsip_header_Target_Dialog.c\
	headers/tsip_header_Timestamp.c\
	headers/tsip_header_To.c\
	headers/tsip_header_Unsupported.c\
	headers/tsip_header_User_Agent.c\
	headers/tsip_header_Via.c\
	headers/tsip_header_Warning.c\
	headers/tsip_header_WWW_Authenticate.c\
	\
	parsers/tsip_parser_header.c\
	parsers/tsip_parser_message.c\
	parsers/tsip_parser_uri.c\
	parsers/tsip_ragel_state.c\
	\
	smc/tsip_dialog_register_sm.c\
	smc/tsip_transac_ict_sm.c\
	smc/tsip_transac_ist_sm.c\
	smc/tsip_transac_nict_sm.c\
	smc/tsip_transac_nist_sm.c\
	\
	transactions/tsip_transac.c\
	transactions/tsip_transac_ict.c\
	transactions/tsip_transac_ist.c\
	transactions/tsip_transac_layer.c\
	transactions/tsip_transac_nict.c\
	transactions/tsip_transac_nist.c\
	\
	transports/tsip_transport.c\
	transports/tsip_transport_layer.c
	
LOCAL_CFLAGS := \
	-I$(DOUBANGO_HOME)/thirdparties/win32/include/smc\
	-I$(DOUBANGO_HOME)/tinySAK/src\
	-I$(DOUBANGO_HOME)/tinyNET/src\
	-I$(DOUBANGO_HOME)/tinySIP/include
	

#LOCAL_SHARED_LIBRARIES := libtinySAK
LOCAL_LDLIBS := -L$(DOUBANGO_HOME)/android/libs/armeabi -ltinySAK -ltinyNET

include $(BUILD_SHARED_LIBRARY)
