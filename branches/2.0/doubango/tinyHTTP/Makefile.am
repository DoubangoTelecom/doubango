lib_LTLIBRARIES         = libtinyHTTP.la
libtinyHTTP_la_LIBADD = ../tinySAK/libtinySAK.la ../tinyNET/libtinyNET.la
libtinyHTTP_la_CPPFLAGS = -Iinclude -I../tinySAK/src -I../tinyNET/src

libtinyHTTP_la_SOURCES = \
	src/thttp.c\
	src/thttp_action.c\
	src/thttp_dialog.c\
	src/thttp_event.c\
	src/thttp_message.c\
	src/thttp_session.c\
	src/thttp_url.c\
	src/thttp_proxy_node_plugin.c
	
libtinyHTTP_la_SOURCES +=	src/auth/thttp_auth.c\
	src/auth/thttp_challenge.c
	
libtinyHTTP_la_SOURCES +=	src/headers/thttp_header.c\
	src/headers/thttp_header_Authorization.c\
	src/headers/thttp_header_Content_Length.c\
	src/headers/thttp_header_Content_Type.c\
	src/headers/thttp_header_Dummy.c\
	src/headers/thttp_header_ETag.c\
	src/headers/thttp_header_Sec_WebSocket_Accept.c\
	src/headers/thttp_header_Sec_WebSocket_Key.c\
	src/headers/thttp_header_Sec_WebSocket_Protocol.c\
	src/headers/thttp_header_Sec_WebSocket_Version.c\
	src/headers/thttp_header_Transfer_Encoding.c\
	src/headers/thttp_header_WWW_Authenticate.c
	
libtinyHTTP_la_SOURCES +=	src/parsers/thttp_parser_header.c\
	src/parsers/thttp_parser_message.c\
	src/parsers/thttp_parser_url.c

libtinyHTTP_la_LDFLAGS = $LDFLAGS -no-undefined
if TARGET_OS_IS_ANDROID
libtinyHTTP_la_LDFLAGS += -static
endif

_includedir = $(includedir)/tinyhttp
_include_HEADERS = include/*.h
__includedir = $(includedir)/tinyhttp/tinyhttp
__include_HEADERS = include/tinyhttp/*.h
auth_includedir = $(includedir)/tinyhttp/tinyhttp/auth
auth_include_HEADERS = include/tinyhttp/auth/*.h
headers_includedir = $(includedir)/tinyhttp/tinyhttp/headers
headers_include_HEADERS = include/tinyhttp/headers/*.h
parsers_includedir = $(includedir)/tinyhttp/tinyhttp/parsers
parsers_include_HEADERS = include/tinyhttp/parsers/*.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tinyHTTP.pc
