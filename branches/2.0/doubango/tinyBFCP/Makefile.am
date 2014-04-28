lib_LTLIBRARIES         = libtinyBFCP.la
libtinyBFCP_la_LIBADD = ../tinySAK/libtinySAK.la ../tinyNET/libtinyNET.la
libtinyBFCP_la_CPPFLAGS = -Iinclude -I../tinySAK/src -I../tinyNET/src

libtinyBFCP_la_SOURCES = \
	src/tbfcp_attr.c\
	src/tbfcp_pkt.c\
	src/tbfcp_session.c\
	src/tbfcp_utils.c

libtinyBFCP_la_LDFLAGS = $LDFLAGS -no-undefined
if TARGET_OS_IS_ANDROID
libtinyBFCP_la_LDFLAGS += -static
endif

_includedir = $(includedir)/tinybfcp
_include_HEADERS = include/*.h
__includedir = $(includedir)/tinybfcp/tinybfcp
__include_HEADERS = include/tinybfcp/*.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tinyBFCP.pc
