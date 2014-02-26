lib_LTLIBRARIES         = libtinyXCAP.la
libtinyXCAP_la_LIBADD = ../tinySAK/libtinySAK.la ../tinyNET/libtinyNET.la ../tinyHTTP/libtinyHTTP.la
libtinyXCAP_la_CPPFLAGS = -I../tinySAK/src -I../tinyNET/src -I../tinyHTTP/include -Iinclude

libtinyXCAP_la_SOURCES = \
	src/txcap.c\
	src/txcap_auid.c\
	src/txcap_document.c\
	src/txcap_node.c\
	src/txcap_selector.c\
	src/txcap_action.c


libtinyXCAP_la_LDFLAGS = $LDFLAGS -no-undefined
if TARGET_OS_IS_ANDROID
libtinyXCAP_la_LDFLAGS += -static
endif

_includedir = $(includedir)/tinyxcap
_include_HEADERS = include/*.h
__includedir = $(includedir)/tinyxcap/tinyxcap
__include_HEADERS = include/tinyxcap/*.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tinyXCAP.pc
