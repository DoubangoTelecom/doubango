lib_LTLIBRARIES         = libtinySMS.la
libtinySMS_la_LIBADD = ../tinySAK/libtinySAK.la
libtinySMS_la_CPPFLAGS = -I../tinySAK/src -Iinclude

libtinySMS_la_SOURCES = \
	src/tsms.c\
	src/tsms_address.c\
	src/tsms_common.c\
	src/tsms_packing.c
	
libtinySMS_la_SOURCES += src/rpdu/tsms_rpdu.c
	
libtinySMS_la_SOURCES += src/tpdu/tsms_tpdu_command.c\
	src/tpdu/tsms_tpdu_deliver.c\
	src/tpdu/tsms_tpdu_report.c\
	src/tpdu/tsms_tpdu_status_report.c\
	src/tpdu/tsms_tpdu_submit.c

libtinySMS_la_LDFLAGS = $LDFLAGS -no-undefined
if TARGET_OS_IS_ANDROID
libtinySMS_la_LDFLAGS += -static
endif

_includedir = $(includedir)/tinysms
_include_HEADERS = include/*.h
__includedir = $(includedir)/tinysms/tinysms
__include_HEADERS = include/tinysms/*.h
rpdu_includedir = $(includedir)/tinysms/tinysms/rpdu
rpdu_include_HEADERS = include/tinysms/rpdu/*.h
tpdu_includedir = $(includedir)/tinysms/tinysms/tpdu
tpdu_include_HEADERS = include/tinysms/tpdu/*.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tinySMS.pc