lib_LTLIBRARIES         = libtinyIPSec.la
libtinyIPSec_la_LIBADD = ../tinySAK/libtinySAK.la
libtinyIPSec_la_CPPFLAGS = -I../tinySAK/src -Isrc

libtinyIPSec_la_SOURCES = \
	src/tipsec.c

# LDFLAGS
libtinyIPSec_la_LDFLAGS = $LDFLAGS -no-undefined
if TARGET_OS_IS_ANDROID
libtinyIPSec_la_LDFLAGS += -static
endif

# Headers file to install
libtinyIPSec_la_HEADERS = src/*.h
libtinyIPSec_ladir = $(includedir)/tinyipsec

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tinyIPSec.pc