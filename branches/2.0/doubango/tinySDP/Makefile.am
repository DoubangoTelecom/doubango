lib_LTLIBRARIES         = libtinySDP.la
libtinySDP_la_LIBADD = ../tinySAK/libtinySAK.la
libtinySDP_la_CPPFLAGS = -I../tinySAK/src -Iinclude

libtinySDP_la_SOURCES = \
	src/tsdp.c\
	src/tsdp_message.c
	
libtinySDP_la_SOURCES += src/headers/tsdp_header.c\
	src/headers/tsdp_header_A.c\
	src/headers/tsdp_header_B.c\
	src/headers/tsdp_header_C.c\
	src/headers/tsdp_header_Dummy.c\
	src/headers/tsdp_header_E.c\
	src/headers/tsdp_header_I.c\
	src/headers/tsdp_header_K.c\
	src/headers/tsdp_header_M.c\
	src/headers/tsdp_header_O.c\
	src/headers/tsdp_header_P.c\
	src/headers/tsdp_header_R.c\
	src/headers/tsdp_header_S.c\
	src/headers/tsdp_header_T.c\
	src/headers/tsdp_header_U.c\
	src/headers/tsdp_header_V.c\
	src/headers/tsdp_header_Z.c
	
libtinySDP_la_SOURCES += src/parsers/tsdp_parser_message.c


libtinySDP_la_LDFLAGS = $LDFLAGS -no-undefined
if TARGET_OS_IS_ANDROID
libtinySDP_la_LDFLAGS += -static
endif


_includedir = $(includedir)/tinysdp
_include_HEADERS = include/*.h
__includedir = $(includedir)/tinysdp/tinysdp
__include_HEADERS = include/tinysdp/*.h
headers_includedir = $(includedir)/tinysdp/tinysdp/headers
headers_include_HEADERS = include/tinysdp/headers/*.h
parsers_includedir = $(includedir)/tinysdp/tinysdp/parsers
parsers_include_HEADERS = include/tinysdp/parsers/*.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tinySDP.pc