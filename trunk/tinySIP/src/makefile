APP := lib$(PROJECT).so

INSTALL_DIR = /system/lib

CFLAGS := $(CFLAGS)
CFLAGS += -I$(DOUBANGO_HOME)/$(PROJECT)/include
CFLAGS += -I$(DOUBANGO_HOME)/tinySAK/src
CFLAGS += -I$(DOUBANGO_HOME)/tinyNET/src
CFLAGS += -I$(DOUBANGO_HOME)/tinyHTTP/include
CFLAGS += -I$(DOUBANGO_HOME)/thirdparties/android/include/smc

LDFLAGS := $(LDFLAGS) -Wl,-soname,lib$(PROJECT).so,-Bsymbolic,-shared,--whole-archive -ltinySAK -ltinyNET -ltinyHTTP

all: $(APP)
	
OBJS = \
	tsip.o\
	tsip_event.o\
	tsip_message.o\
	tsip_operation.o\
	tsip_timers.o\
	tsip_uri.o
	###################
	## api
	###################
OBJS += api/tsip_invite.o\
	api/tsip_message.o\
	api/tsip_publish.o\
	api/tsip_register.o\
	api/tsip_subscribe.o
	###################
	## authentication
	###################
OBJS += authentication/tsip_challenge.o\
	authentication/tsip_milenage.o\
	authentication/tsip_rijndael.o
	###################
	## dialogs
	###################
OBJS += dialogs/tsip_dialog.o\
	dialogs/tsip_dialog_invite.client.o\
	dialogs/tsip_dialog_invite.server.o\
	dialogs/tsip_dialog_layer.o\
	dialogs/tsip_dialog_message.o\
	dialogs/tsip_dialog_register.client.o\
	dialogs/tsip_dialog_register.server.o\
	dialogs/tsip_dialog_subscribe.client.o\
	dialogs/tsip_dialog_subscribe.server.o
	###################
	## auth
	###################
OBJS += headers/tsip_header.o\
	headers/tsip_header_accept.o\
	headers/tsip_header_Accept_Contact.o\
	headers/tsip_header_Accept_Encoding.o\
	headers/tsip_header_Accept_Language.o\
	headers/tsip_header_Accept_Resource_Priority.o\
	headers/tsip_header_Alert_Info.o\
	headers/tsip_header_Allow.o\
	headers/tsip_header_Allow_Events.o\
	headers/tsip_header_Authentication_Info.o\
	headers/tsip_header_Authorization.o\
	headers/tsip_header_Call_ID.o\
	headers/tsip_header_Call_Info.o\
	headers/tsip_header_Contact.o\
	headers/tsip_header_Content_Disposition.o\
	headers/tsip_header_Content_Encoding.o\
	headers/tsip_header_Content_Language.o\
	headers/tsip_header_Content_Length.o\
	headers/tsip_header_Content_Type.o\
	headers/tsip_header_CSeq.o\
	headers/tsip_header_Date.o\
	headers/tsip_header_Error_Info.o\
	headers/tsip_header_Event.o\
	headers/tsip_header_Expires.o\
	headers/tsip_header_From.o\
	headers/tsip_header_History_Info.o\
	headers/tsip_header_Identity.o\
	headers/tsip_header_Identity_Info.o\
	headers/tsip_header_In_Reply_To.o\
	headers/tsip_header_Join.o\
	headers/tsip_header_Max_Forwards.o\
	headers/tsip_header_MIME_Version.o\
	headers/tsip_header_Min_Expires.o\
	headers/tsip_header_Min_SE.o\
	headers/tsip_header_Organization.o\
	headers/tsip_header_Path.o\
	headers/tsip_header_Priority.o\
	headers/tsip_header_Privacy.o\
	headers/tsip_header_Proxy_Authenticate.o\
	headers/tsip_header_Proxy_Authorization.o\
	headers/tsip_header_Proxy_Require.o\
	headers/tsip_header_P_Access_Network_Info.o\
	headers/tsip_header_P_Answer_State.o\
	headers/tsip_header_P_Asserted_Identity.o\
	headers/tsip_header_P_Associated_URI.o\
	headers/tsip_header_P_Called_Party_ID.o\
	headers/tsip_header_P_Charging_Function_Addresses.o\
	headers/tsip_header_P_Charging_Vector.o\
	headers/tsip_header_P_DCS_Billing_Info.o\
	headers/tsip_header_P_DCS_LAES.o\
	headers/tsip_header_P_DCS_OSPS.o\
	headers/tsip_header_P_DCS_Redirect.o\
	headers/tsip_header_P_DCS_Trace_Party_ID.o\
	headers/tsip_header_P_Early_Media.o\
	headers/tsip_header_P_Media_Authorization.o\
	headers/tsip_header_P_Preferred_Identity.o\
	headers/tsip_header_P_Profile_Key.o\
	headers/tsip_header_P_User_Database.o\
	headers/tsip_header_P_Visited_Network_ID.o\
	headers/tsip_header_RAck.o\
	headers/tsip_header_Reason.o\
	headers/tsip_header_Record_Route.o\
	headers/tsip_header_Referred_By.o\
	headers/tsip_header_Refer_Sub.o\
	headers/tsip_header_Refer_To.o\
	headers/tsip_header_Reject_Contact.o\
	headers/tsip_header_Replaces.o\
	headers/tsip_header_Reply_To.o\
	headers/tsip_header_Request_Disposition.o\
	headers/tsip_header_Require.o\
	headers/tsip_header_Resource_Priority.o\
	headers/tsip_header_Retry_After.o\
	headers/tsip_header_Route.o\
	headers/tsip_header_RSeq.o\
	headers/tsip_header_Security_Client.o\
	headers/tsip_header_Security_Server.o\
	headers/tsip_header_Security_Verify.o\
	headers/tsip_header_Server.o\
	headers/tsip_header_Service_Route.o\
	headers/tsip_header_Session_Expires.o\
	headers/tsip_header_SIP_ETag.o\
	headers/tsip_header_SIP_If_Match.o\
	headers/tsip_header_Subject.o\
	headers/tsip_header_Subscription_State.o\
	headers/tsip_header_Supported.o\
	headers/tsip_header_Target_Dialog.o\
	headers/tsip_header_Timestamp.o\
	headers/tsip_header_To.o\
	headers/tsip_header_Unsupported.o\
	headers/tsip_header_User_Agent.o\
	headers/tsip_header_Via.o\
	headers/tsip_header_Warning.o\
	headers/tsip_header_WWW_Authenticate.o
	###################
	## parsers
	###################
OBJS += parsers/tsip_parser_header.o\
	parsers/tsip_parser_message.o\
	parsers/tsip_parser_uri.o\
	parsers/tsip_ragel_state.o
	###################
	## smc
	###################
OBJS += smc/tsip_dialog_register_sm.o\
	smc/tsip_dialog_message_sm.o\
	smc/tsip_transac_ict_sm.o\
	smc/tsip_transac_ist_sm.o\
	smc/tsip_transac_nict_sm.o\
	smc/tsip_transac_nist_sm.o
	###################
	## transactions
	###################
OBJS += transactions/tsip_transac.o\
	transactions/tsip_transac_ict.o\
	transactions/tsip_transac_ist.o\
	transactions/tsip_transac_layer.o\
	transactions/tsip_transac_nict.o\
	transactions/tsip_transac_nist.o
	###################
	## transport
	###################
OBJS += transports/tsip_transport.o\
	transports/tsip_transport_layer.o

$(APP): $(OBJS)
	$(CPP) $(LDFLAGS) -o $@ $^ 

%.o: %.c
	$(CC) -c $(INCLUDE) $(CFLAGS) $< -o $@
	
install: $(APP)
	$(ANDROID_SDK_ROOT)/tools/adb remount
	$(ANDROID_SDK_ROOT)/tools/adb push $(APP) $(INSTALL_DIR)/$(APP)
	$(ANDROID_SDK_ROOT)/tools/adb shell chmod 777 $(INSTALL_DIR)/$(APP)
	
clean:
	@rm -f $(OBJS) $(APP)