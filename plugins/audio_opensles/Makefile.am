lib_LTLIBRARIES         = libplugin_audio_opensles.la
libplugin_audio_opensles_la_LIBADD = \
	../../tinySAK/libtinySAK.la \
	../../tinyNET/libtinyNET.la \
	../../tinyMEDIA/libtinyMEDIA.la \
	../../tinySDP/libtinySDP.la
libplugin_audio_opensles_la_CPPFLAGS = \
	-I.. \
	-I../../tinySAK/src \
	-I../../tinyNET/src \
	-I../../tinySDP/include \
	-I../../tinyMEDIA/include \
	-I../../tinyRTP/include \
	-I../../tinyDAV/include

libplugin_audio_opensles_la_LDFLAGS = ${LDFLAGS}

if TARGET_OS_IS_ANDROID
libplugin_audio_opensles_la_CPPFLAGS += -DANDROID=1 -fno-rtti -fno-exceptions -I${NDK}/platforms/android-9/arch-${TARGET_ARCH}/usr/include
libplugin_audio_opensles_la_LDFLAGS +=  -Wl,-shared,-Bsymbolic,--no-undefined,--whole-archive -L${NDK}/platforms/android-9/arch-${TARGET_ARCH}/usr/lib -lOpenSLES -lm -lstdc++ -lgcc -llog -ldl
else
libplugin_audio_opensles_la_LDFLAGS += 
endif

if USE_SSL
libplugin_audio_opensles_la_LIBADD += ${LIBSSL_LIBADD}
endif

libplugin_audio_opensles_la_SOURCES  = \
	audio_opensles.cxx \
	audio_opensles_consumer.cxx \
	audio_opensles_device_impl.cxx \
	audio_opensles_producer.cxx \
	audio_opensles_device.cxx \
	../../tinyDAV/src/audio/tdav_consumer_audio.c \
	../../tinyDAV/src/audio/tdav_producer_audio.c

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = plugin_audio_opensles.pc
