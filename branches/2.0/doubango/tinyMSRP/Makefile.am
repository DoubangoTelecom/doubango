lib_LTLIBRARIES         = libtinyMSRP.la
libtinyMSRP_la_LIBADD = ../tinySAK/libtinySAK.la ../tinyNET/libtinyNET.la ../tinyHTTP/libtinyHTTP.la ../tinySDP/libtinySDP.la
libtinyMSRP_la_CPPFLAGS = -Iinclude -I../tinySAK/src -I../tinyNET/src -I../tinyHTTP/include -I../tinySDP/include
	
libtinyMSRP_la_SOURCES = \
	src/tmsrp.c\
	src/tmsrp_event.c\
	src/tmsrp_message.c\
	src/tmsrp_uri.c
	
libtinyMSRP_la_SOURCES += src/headers/tmsrp_header.c\
		src/headers/tmsrp_header_Authentication-Info.c\
		src/headers/tmsrp_header_Authorization.c\
		src/headers/tmsrp_header_Byte-Range.c\
		src/headers/tmsrp_header_Content-Type.c\
		src/headers/tmsrp_header_Dummy.c\
		src/headers/tmsrp_header_Expires.c\
		src/headers/tmsrp_header_Failure-Report.c\
		src/headers/tmsrp_header_From-Path.c\
		src/headers/tmsrp_header_Max-Expires.c\
		src/headers/tmsrp_header_Message-ID.c\
		src/headers/tmsrp_header_Min-Expires.c\
		src/headers/tmsrp_header_Status.c\
		src/headers/tmsrp_header_Success-Report.c\
		src/headers/tmsrp_header_To-Path.c\
		src/headers/tmsrp_header_Use-Path.c\
		src/headers/tmsrp_header_WWW-Authenticate.c
	
libtinyMSRP_la_SOURCES += src/parsers/tmsrp_parser_message.c\
	src/parsers/tmsrp_parser_uri.c
	
libtinyMSRP_la_SOURCES += src/session/tmsrp_config.c\
	src/session/tmsrp_data.c \
	src/session/tmsrp_receiver.c \
	src/session/tmsrp_sender.c

libtinyMSRP_la_LDFLAGS = $LDFLAGS -no-undefined
if TARGET_OS_IS_ANDROID
libtinyMSRP_la_LDFLAGS += -static
endif

_includedir = $(includedir)/tinymsrp
_include_HEADERS = include/*.h
__includedir = $(includedir)/tinymsrp/tinymsrp
__include_HEADERS = include/tinymsrp/*.h
headers_includedir = $(includedir)/tinymsrp/tinymsrp/headers
headers_include_HEADERS = include/tinymsrp/headers/*.h
parsers_includedir = $(includedir)/tinymsrp/tinymsrp/parsers
parsers_include_HEADERS = include/tinymsrp/parsers/*.h
session_includedir = $(includedir)/tinymsrp/tinymsrp/session
session_include_HEADERS = include/tinymsrp/session/*.h


pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tinyMSRP.pc