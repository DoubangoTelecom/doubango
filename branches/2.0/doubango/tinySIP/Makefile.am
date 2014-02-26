lib_LTLIBRARIES         = libtinySIP.la
libtinySIP_la_LIBADD = \
	../tinySAK/libtinySAK.la\
	../tinyNET/libtinyNET.la\
	../tinyHTTP/libtinyHTTP.la\
	../tinyIPSec/libtinyIPSec.la\
	../tinySDP/libtinySDP.la\
	../tinyMEDIA/libtinyMEDIA.la\
	../tinySIGCOMP/libtinySIGCOMP.la

libtinySIP_la_CPPFLAGS = \
	-I../tinySAK/src\
	-I../tinyNET/src\
	-I../tinyHTTP/include\
	-I../tinyIPSec/src\
	-I../tinySIGCOMP/src\
	-I../tinySDP/include\
	-I../tinyMEDIA/include\
	-Iinclude
	
libtinySIP_la_SOURCES = \
	src/tsip.c\
	src/tsip_action.c\
	src/tsip_event.c\
	src/tsip_message.c\
	src/tsip_ssession.c\
	src/tsip_timers.c\
	src/tsip_uri.c
	
libtinySIP_la_SOURCES += src/api/tsip_api_common.c\
	src/api/tsip_api_info.c\
	src/api/tsip_api_invite.c\
	src/api/tsip_api_message.c\
	src/api/tsip_api_options.c\
	src/api/tsip_api_publish.c\
	src/api/tsip_api_register.c\
	src/api/tsip_api_subscribe.c

libtinySIP_la_SOURCES += src/authentication/tsip_challenge.c\
	src/authentication/tsip_milenage.c\
	src/authentication/tsip_rijndael.c
	
libtinySIP_la_SOURCES += src/dialogs/tsip_dialog.c\
	src/dialogs/tsip_dialog_info.c\
	src/dialogs/tsip_dialog_invite.c\
	src/dialogs/tsip_dialog_invite.cdiv.c\
	src/dialogs/tsip_dialog_invite.client.c\
	src/dialogs/tsip_dialog_invite.conf.c\
	src/dialogs/tsip_dialog_invite.ect.c\
	src/dialogs/tsip_dialog_invite.hold.c\
	src/dialogs/tsip_dialog_invite.ice.c\
	src/dialogs/tsip_dialog_invite.qos.c\
	src/dialogs/tsip_dialog_invite.server.c\
	src/dialogs/tsip_dialog_invite.timers.c\
	src/dialogs/tsip_dialog_layer.c\
	src/dialogs/tsip_dialog_message.c\
	src/dialogs/tsip_dialog_options.c\
	src/dialogs/tsip_dialog_publish.client.c\
	src/dialogs/tsip_dialog_register.c\
	src/dialogs/tsip_dialog_register.client.c\
	src/dialogs/tsip_dialog_register.server.c\
	src/dialogs/tsip_dialog_subscribe.client.c\
	src/dialogs/tsip_dialog_subscribe.server.c
	
libtinySIP_la_SOURCES += src/headers/tsip_header.c\
	src/headers/tsip_header_accept.c\
	src/headers/tsip_header_Accept_Contact.c\
	src/headers/tsip_header_Accept_Encoding.c\
	src/headers/tsip_header_Accept_Language.c\
	src/headers/tsip_header_Accept_Resource_Priority.c\
	src/headers/tsip_header_Alert_Info.c\
	src/headers/tsip_header_Allow.c\
	src/headers/tsip_header_Allow_Events.c\
	src/headers/tsip_header_Authentication_Info.c\
	src/headers/tsip_header_Authorization.c\
	src/headers/tsip_header_Call_ID.c\
	src/headers/tsip_header_Call_Info.c\
	src/headers/tsip_header_Contact.c\
	src/headers/tsip_header_Content_Disposition.c\
	src/headers/tsip_header_Content_Encoding.c\
	src/headers/tsip_header_Content_Language.c\
	src/headers/tsip_header_Content_Length.c\
	src/headers/tsip_header_Content_Type.c\
	src/headers/tsip_header_CSeq.c\
	src/headers/tsip_header_Date.c\
	src/headers/tsip_header_Dummy.c\
	src/headers/tsip_header_Error_Info.c\
	src/headers/tsip_header_Event.c\
	src/headers/tsip_header_Expires.c\
	src/headers/tsip_header_From.c\
	src/headers/tsip_header_History_Info.c\
	src/headers/tsip_header_Identity.c\
	src/headers/tsip_header_Identity_Info.c\
	src/headers/tsip_header_In_Reply_To.c\
	src/headers/tsip_header_Join.c\
	src/headers/tsip_header_Max_Forwards.c\
	src/headers/tsip_header_MIME_Version.c\
	src/headers/tsip_header_Min_Expires.c\
	src/headers/tsip_header_Min_SE.c\
	src/headers/tsip_header_Organization.c\
	src/headers/tsip_header_Path.c\
	src/headers/tsip_header_Priority.c\
	src/headers/tsip_header_Privacy.c\
	src/headers/tsip_header_Proxy_Authenticate.c\
	src/headers/tsip_header_Proxy_Authorization.c\
	src/headers/tsip_header_Proxy_Require.c\
	src/headers/tsip_header_P_Access_Network_Info.c\
	src/headers/tsip_header_P_Answer_State.c\
	src/headers/tsip_header_P_Asserted_Identity.c\
	src/headers/tsip_header_P_Associated_URI.c\
	src/headers/tsip_header_P_Called_Party_ID.c\
	src/headers/tsip_header_P_Charging_Function_Addresses.c\
	src/headers/tsip_header_P_Charging_Vector.c\
	src/headers/tsip_header_P_DCS_Billing_Info.c\
	src/headers/tsip_header_P_DCS_LAES.c\
	src/headers/tsip_header_P_DCS_OSPS.c\
	src/headers/tsip_header_P_DCS_Redirect.c\
	src/headers/tsip_header_P_DCS_Trace_Party_ID.c\
	src/headers/tsip_header_P_Early_Media.c\
	src/headers/tsip_header_P_Media_Authorization.c\
	src/headers/tsip_header_P_Preferred_Identity.c\
	src/headers/tsip_header_P_Profile_Key.c\
	src/headers/tsip_header_P_User_Database.c\
	src/headers/tsip_header_P_Visited_Network_ID.c\
	src/headers/tsip_header_RAck.c\
	src/headers/tsip_header_Reason.c\
	src/headers/tsip_header_Record_Route.c\
	src/headers/tsip_header_Referred_By.c\
	src/headers/tsip_header_Refer_Sub.c\
	src/headers/tsip_header_Refer_To.c\
	src/headers/tsip_header_Reject_Contact.c\
	src/headers/tsip_header_Replaces.c\
	src/headers/tsip_header_Reply_To.c\
	src/headers/tsip_header_Request_Disposition.c\
	src/headers/tsip_header_Require.c\
	src/headers/tsip_header_Resource_Priority.c\
	src/headers/tsip_header_Retry_After.c\
	src/headers/tsip_header_Route.c\
	src/headers/tsip_header_RSeq.c\
	src/headers/tsip_header_Security_Client.c\
	src/headers/tsip_header_Security_Server.c\
	src/headers/tsip_header_Security_Verify.c\
	src/headers/tsip_header_Server.c\
	src/headers/tsip_header_Service_Route.c\
	src/headers/tsip_header_Session_Expires.c\
	src/headers/tsip_header_SIP_ETag.c\
	src/headers/tsip_header_SIP_If_Match.c\
	src/headers/tsip_header_Subject.c\
	src/headers/tsip_header_Subscription_State.c\
	src/headers/tsip_header_Supported.c\
	src/headers/tsip_header_Target_Dialog.c\
	src/headers/tsip_header_Timestamp.c\
	src/headers/tsip_header_To.c\
	src/headers/tsip_header_Unsupported.c\
	src/headers/tsip_header_User_Agent.c\
	src/headers/tsip_header_Via.c\
	src/headers/tsip_header_Warning.c\
	src/headers/tsip_header_WWW_Authenticate.c
	
libtinySIP_la_SOURCES += src/parsers/tsip_parser_header.c\
	src/parsers/tsip_parser_message.c\
	src/parsers/tsip_parser_uri.c
	
libtinySIP_la_SOURCES += src/sigcomp/tsip_sigcomp.c
	
libtinySIP_la_SOURCES += src/transactions/tsip_transac.c\
	src/transactions/tsip_transac_ict.c\
	src/transactions/tsip_transac_ist.c\
	src/transactions/tsip_transac_layer.c\
	src/transactions/tsip_transac_nict.c\
	src/transactions/tsip_transac_nist.c
	
libtinySIP_la_SOURCES += src/transports/tsip_transport.c\
	src/transports/tsip_transport_ipsec.c\
	src/transports/tsip_transport_layer.c\
	src/transports/tsip_transport_tls.c


libtinySIP_la_LDFLAGS = $LDFLAGS -no-undefined
if TARGET_OS_IS_ANDROID
libtinySIP_la_LDFLAGS += -static
endif

_includedir = $(includedir)/tinysip
_include_HEADERS = include/*.h
__includedir = $(includedir)/tinysip/tinysip
__include_HEADERS = include/tinysip/*.h
api_includedir = $(includedir)/tinysip/tinysip/api
api_include_HEADERS = include/tinysip/api/*.h
authentication_includedir = $(includedir)/tinysip/tinysip/authentication
authentication_include_HEADERS = include/tinysip/authentication/*.h
dialogs_includedir = $(includedir)/tinysip/tinysip/dialogs
dialogs_include_HEADERS = include/tinysip/dialogs/*.h
headers_includedir = $(includedir)/tinysip/tinysip/headers
headers_include_HEADERS = include/tinysip/headers/*.h
parsers_includedir = $(includedir)/tinysip/tinysip/parsers
parsers_include_HEADERS = include/tinysip/parsers/*.h
sigcomp_includedir = $(includedir)/tinysip/tinysip/sigcomp
sigcomp_include_HEADERS = include/tinysip/sigcomp/*.h
transactions_includedir = $(includedir)/tinysip/tinysip/transactions
transactions_include_HEADERS = include/tinysip/transactions/*.h
transports_includedir = $(includedir)/tinysip/tinysip/transports
transports_include_HEADERS = include/tinysip/transports/*.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tinySIP.pc



