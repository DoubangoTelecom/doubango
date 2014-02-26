lib_LTLIBRARIES         = libtinyMEDIA.la
libtinyMEDIA_la_LIBADD = ../tinySAK/libtinySAK.la ../tinyNET/libtinyNET.la ../tinySDP/libtinySDP.la
libtinyMEDIA_la_CPPFLAGS = -Iinclude -I../tinySAK/src -I../tinyNET/src -I../tinySDP/include
	
libtinyMEDIA_la_SOURCES = \
	src/tmedia.c \
	src/tmedia_codec.c \
	src/tmedia_codec_dummy.c \
	src/tmedia_common.c \
	src/tmedia_consumer.c \
	src/tmedia_converter_video.c \
	src/tmedia_defaults.c \
	src/tmedia_denoise.c \
	src/tmedia_imageattr.c \
	src/tmedia_jitterbuffer.c \
	src/tmedia_params.c \
	src/tmedia_producer.c \
	src/tmedia_qos.c \
	src/tmedia_resampler.c \
	src/tmedia_session.c \
	src/tmedia_session_dummy.c \
	src/tmedia_session_ghost.c
	
libtinyMEDIA_la_SOURCES += \
	src/content/tmedia_content.c \
	src/content/tmedia_content_cpim.c \
	src/content/tmedia_content_multipart.c \
	src/content/tmedia_content_sip_frag.c

libtinyMEDIA_la_LDFLAGS = $LDFLAGS -no-undefined
if TARGET_OS_IS_ANDROID
libtinyMEDIA_la_LDFLAGS += -static
endif

_includedir = $(includedir)/tinymedia
_include_HEADERS = include/*.h
__includedir = $(includedir)/tinymedia/tinymedia
__include_HEADERS = include/tinymedia/*.h
content_includedir = $(includedir)/tinymedia/tinymedia/content
content_include_HEADERS = include/tinymedia/content/*.h


pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tinyMEDIA.pc