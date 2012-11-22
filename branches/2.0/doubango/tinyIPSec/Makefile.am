lib_LTLIBRARIES         = libtinyIPSec.la
libtinyIPSec_la_LIBADD = ../tinySAK/libtinySAK.la
libtinyIPSec_la_CPPFLAGS = -I../tinySAK/src -Isrc

libtinyIPSec_la_SOURCES = \
	src/tipsec.c\
	src/tipsec_common.c\
	src/tipsec_racoon.c\
	src/tipsec_vista.c\
	src/tipsec_xp.c

# LDFLAGS
libtinyIPSec_la_LDFLAGS = $LDFLAGS -no-undefined

# Headers file to install
libtinyIPSec_la_HEADERS = src/*.h
libtinyIPSec_ladir = $(includedir)/tinyipsec
