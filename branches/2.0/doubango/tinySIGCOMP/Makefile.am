lib_LTLIBRARIES         = libtinySIGCOMP.la
libtinySIGCOMP_la_LIBADD = ../tinySAK/libtinySAK.la
libtinySIGCOMP_la_CPPFLAGS = -I../tinySAK/src -Iinclude

libtinySIGCOMP_la_SOURCES = \
	src/adler32.c\
	src/compress.c\
	src/deflate.c\
	src/tcomp.c\
	src/tcomp_buffer.c\
	src/tcomp_compartment.c\
	src/tcomp_compressordata.c\
	src/tcomp_compressordisp.c\
	src/tcomp_compressor_deflate.c\
	src/tcomp_compressor_dummy.c\
	src/tcomp_decompressordisp.c\
	src/tcomp_deflatedata.c\
	src/tcomp_deflatedata.ghost.c\
	src/tcomp_deflatedata.zlib.c\
	src/tcomp_dicts.c\
	src/tcomp_manager.c\
	src/tcomp_message.c\
	src/tcomp_nackinfo.c\
	src/tcomp_params.c\
	src/tcomp_reqfeed.c\
	src/tcomp_result.c\
	src/tcomp_state.c\
	src/tcomp_statehandler.c\
	src/tcomp_udvm.bytecopy.c\
	src/tcomp_udvm.c\
	src/tcomp_udvm.instructions.c\
	src/tcomp_udvm.nack.c\
	src/tcomp_udvm.operands.c\
	src/tcomp_udvm.statemanagment.c\
	src/trees.c\
	src/zutil.c

libtinySIGCOMP_la_LDFLAGS = $LDFLAGS -no-undefined
if TARGET_OS_IS_ANDROID
libtinySIGCOMP_la_LDFLAGS += -static
endif
libtinySIGCOMP_la_HEADERS = src/*.h
libtinySIGCOMP_ladir = $(includedir)/tinysigcomp

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tinySIGCOMP.pc