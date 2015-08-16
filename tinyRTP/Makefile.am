lib_LTLIBRARIES         = libtinyRTP.la
libtinyRTP_la_LIBADD = ../tinySAK/libtinySAK.la ../tinyNET/libtinyNET.la ../tinyMEDIA/libtinyMEDIA.la
libtinyRTP_la_CPPFLAGS = -I../tinySAK/src -I../tinyNET/src -I../tinyMEDIA/include -Iinclude

if USE_SRTP
libtinyRTP_la_LIBADD += ${LIBSRTP_LIBADD}
libtinyRTP_la_CPPFLAGS += -DHAVE_SRTP=1
endif
	
libtinyRTP_la_SOURCES = \
	src/trtp.c \
	src/trtp_manager.c \
	src/trtp_srtp.c

libtinyRTP_la_SOURCES += src/rtcp/trtp_rtcp_header.c \
	src/rtcp/trtp_rtcp_packet.c \
	src/rtcp/trtp_rtcp_rblock.c \
	src/rtcp/trtp_rtcp_report.c \
	src/rtcp/trtp_rtcp_report_bye.c \
	src/rtcp/trtp_rtcp_report_fb.c \
	src/rtcp/trtp_rtcp_report_rr.c \
	src/rtcp/trtp_rtcp_report_sdes.c \
	src/rtcp/trtp_rtcp_report_sr.c \
	src/rtcp/trtp_rtcp_report_xr.c \
	src/rtcp/trtp_rtcp_sdes_chunck.c \
	src/rtcp/trtp_rtcp_sdes_item.c \
	src/rtcp/trtp_rtcp_session.c

libtinyRTP_la_SOURCES += src/rtp/trtp_rtp_header.c \
	src/rtp/trtp_rtp_packet.c \
	src/rtp/trtp_rtp_session.c


libtinyRTP_la_LDFLAGS = $LDFLAGS -no-undefined
if TARGET_OS_IS_ANDROID
libtinyRTP_la_LDFLAGS += -static
endif

_includedir = $(includedir)/tinyrtp
_include_HEADERS = include/*.h
__includedir = $(includedir)/tinyrtp/tinyrtp
__include_HEADERS = include/tinyrtp/*.h
rtcp_includedir = $(includedir)/tinyrtp/tinyrtp/rtcp
rtcp_include_HEADERS = include/tinyrtp/rtcp/*.h
rtp_includedir = $(includedir)/tinyrtp/tinyrtp/rtp
rtp_include_HEADERS = include/tinyrtp/rtp/*.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tinyRTP.pc