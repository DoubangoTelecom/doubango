ACLOCAL_AMFLAGS = -I m4
lib_LTLIBRARIES         = libtinySAK.la

if USE_RT
libtinySAK_la_LIBADD = ${LIBRT_LIBADD}
endif

libtinySAK_la_SOURCES  = \
	src/tsk.c\
	src/tsk_base64.c\
	src/tsk_binaryutils.c\
	src/tsk_buffer.c\
	src/tsk_condwait.c\
	src/tsk_debug.c\
	src/tsk_fsm.c\
	src/tsk_hmac.c\
	src/tsk_list.c\
	src/tsk_md5.c\
	src/tsk_memory.c\
	src/tsk_mutex.c\
	src/tsk_object.c\
	src/tsk_options.c\
	src/tsk_params.c\
	src/tsk_plugin.c\
	src/tsk_ppfcs16.c\
	src/tsk_ppfcs32.c\
	src/tsk_ragel_state.c\
	src/tsk_runnable.c\
	src/tsk_safeobj.c\
	src/tsk_semaphore.c\
	src/tsk_sha1.c\
	src/tsk_string.c\
	src/tsk_thread.c\
	src/tsk_time.c\
	src/tsk_timer.c\
	src/tsk_url.c\
	src/tsk_uuid.c\
	src/tsk_xml.c
	
libtinySAK_la_LDFLAGS = $LDFLAGS -no-undefined
if TARGET_OS_IS_ANDROID
libtinySAK_la_LDFLAGS += -static
endif
libtinySAK_ladir = $(includedir)/tinysak

src_includedir = $(includedir)/tinysak
src_include_HEADERS = src/*.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tinySAK.pc