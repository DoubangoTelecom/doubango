lib_LTLIBRARIES         = libtinyDAV.la
libtinyDAV_la_LIBADD = \
	../tinySAK/libtinySAK.la\
	../tinyNET/libtinyNET.la\
	../tinySDP/libtinySDP.la\
	../tinyBFCP/libtinyBFCP.la\
	../tinyRTP/libtinyRTP.la\
	../tinyMEDIA/libtinyMEDIA.la\
	../tinyMSRP/libtinyMSRP.la\
	../tinyIPSec/libtinyIPSec.la

libtinyDAV_la_CPPFLAGS = -I../tinySAK/src\
	-I../tinyNET/src\
	-I../tinyIPSec/src\
	-I../tinySDP/include\
	-I../tinyBFCP/include\
	-I../tinyRTP/include\
	-I../tinyMEDIA/include\
	-I../tinyMSRP/include\
	-Iinclude

if USE_FFMPEG
libtinyDAV_la_LIBADD += ${LIBFFMPEG_LIBADD}
endif

if USE_H264
libtinyDAV_la_LIBADD += ${LIBH264_LIBADD}
endif

if USE_VPX
libtinyDAV_la_LIBADD += ${LIBVPX_LIBADD}
endif

if USE_YUV
libtinyDAV_la_LIBADD += ${LIBYUV_LIB}
libtinyDAV_la_CPPFLAGS += ${LIBYUV_INCLUDE}
endif

if USE_OPUS
libtinyDAV_la_LIBADD += ${LIBOPUS_LIBADD}
endif

if USE_AMR
libtinyDAV_la_LIBADD += ${LIBAMR_LIBADD}
endif

if USE_SPEEX
libtinyDAV_la_LIBADD += ${LIBSPEEX_LIBADD}
endif

if USE_SPEEXDSP
libtinyDAV_la_LIBADD += ${LIBSPEEXDSP_LIBADD}
endif
	
if USE_GSM
libtinyDAV_la_LIBADD += ${LIBGSM_LIBADD}
endif

if USE_G729
libtinyDAV_la_LIBADD += ${LIBG729_LIBADD}
endif

if USE_ILBC
libtinyDAV_la_LIBADD += ${LIBILBC_LIBADD}
endif

if USE_WEBRTC
libtinyDAV_la_LIBADD += ${LIBWEBRTC_LIBADD}
endif

libtinyDAV_la_SOURCES = \
	src/tdav.c \
	src/tdav_session_av.c
	
libtinyDAV_la_SOURCES += src/audio/tdav_consumer_audio.c \
	src/audio/tdav_speakup_jitterbuffer.c \
	src/audio/tdav_jitterbuffer.c \
	src/audio/tdav_producer_audio.c \
    	src/audio/tdav_session_audio.c \
    	src/audio/tdav_speex_denoise.c \
    	src/audio/tdav_speex_jitterbuffer.c \
    	src/audio/tdav_speex_resampler.c \
    	src/audio/tdav_webrtc_denoise.c
     
libtinyDAV_la_SOURCES += src/video/tdav_consumer_video.c \
	src/video/tdav_converter_video.cxx \
	src/video/tdav_runnable_video.c \
	src/video/tdav_session_video.c \
	src/video/jb/tdav_video_frame.c \
	src/video/jb/tdav_video_jb.c

libtinyDAV_la_SOURCES += src/bfcp/tdav_session_bfcp.c
	
libtinyDAV_la_SOURCES += src/t140/tdav_consumer_t140.c \
	src/t140/tdav_producer_t140.c \
	src/t140/tdav_session_t140.c
	
libtinyDAV_la_SOURCES += src/msrp/tdav_consumer_msrp.c \
	src/msrp/tdav_producer_msrp.c \
	src/msrp/tdav_session_msrp.c

libtinyDAV_la_SOURCES += src/codecs/amr/tdav_codec_amr.c

libtinyDAV_la_SOURCES += src/codecs/bfcp/tdav_codec_bfcp.c

libtinyDAV_la_SOURCES += src/codecs/opus/tdav_codec_opus.c

libtinyDAV_la_SOURCES += src/codecs/g711/g711.c \
	src/codecs/g711/tdav_codec_g711.c
	
libtinyDAV_la_SOURCES += src/codecs/g722/g722_decode.c \
		src/codecs/g722/g722_encode.c \
		src/codecs/g722/tdav_codec_g722.c
	
libtinyDAV_la_SOURCES += src/codecs/g729/tdav_codec_g729.c
	
libtinyDAV_la_SOURCES += src/codecs/gsm/tdav_codec_gsm.c
	
libtinyDAV_la_SOURCES += src/codecs/bv/tdav_codec_bv16.c \
	src/codecs/bv/tdav_codec_bv32.c
	
libtinyDAV_la_SOURCES += src/codecs/ilbc/tdav_codec_ilbc.c

libtinyDAV_la_SOURCES += src/codecs/speex/tdav_codec_speex.c

libtinyDAV_la_SOURCES += src/codecs/dtmf/tdav_codec_dtmf.c

libtinyDAV_la_SOURCES += src/codecs/fec/tdav_codec_red.c \
	src/codecs/fec/tdav_codec_ulpfec.c

libtinyDAV_la_SOURCES += src/codecs/h261/tdav_codec_h261.c

libtinyDAV_la_SOURCES += src/codecs/h263/tdav_codec_h263.c

libtinyDAV_la_SOURCES += src/codecs/h264/tdav_codec_h264.c \
		src/codecs/h264/tdav_codec_h264_rtp.c
		
libtinyDAV_la_SOURCES += src/codecs/theora/tdav_codec_theora.c

libtinyDAV_la_SOURCES += src/codecs/vpx/tdav_codec_vp8.c

libtinyDAV_la_SOURCES += src/codecs/mp4ves/tdav_codec_mp4ves.c
	
libtinyDAV_la_SOURCES += src/codecs/msrp/tdav_codec_msrp.c

libtinyDAV_la_SOURCES += src/codecs/t140/tdav_codec_t140.c


libtinyDAV_la_LDFLAGS = $LDFLAGS -no-undefined
if TARGET_OS_IS_ANDROID
libtinyDAV_la_LDFLAGS += -static
endif

_includedir = $(includedir)/tinydav
_include_HEADERS = include/*.h
__includedir = $(includedir)/tinydav/tinydav
__include_HEADERS = include/tinydav/*.h
audio_includedir = $(includedir)/tinydav/tinydav/audio
audio_include_HEADERS = include/tinydav/audio/*.h
video_includedir = $(includedir)/tinydav/tinydav/video
video_include_HEADERS = include/tinydav/video/*.h


pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tinyDAV.pc