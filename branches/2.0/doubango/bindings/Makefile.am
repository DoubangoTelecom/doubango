lib_LTLIBRARIES         = libtinyWRAP.la
libtinyWRAP_la_LIBADD = \
	../tinySAK/libtinySAK.la \
	../tinyHTTP/libtinyHTTP.la \
	../tinyXCAP/libtinyXCAP.la \
	../tinyIPSec/libtinyIPSec.la \
	../tinySIGCOMP/libtinySIGCOMP.la \
	../tinySMS/libtinySMS.la \
	../tinyNET/libtinyNET.la \
	../tinySDP/libtinySDP.la \
	../tinyRTP/libtinyRTP.la \
	../tinyMEDIA/libtinyMEDIA.la \
	../tinyMSRP/libtinyMSRP.la \
	../tinyDAV/libtinyDAV.la \
	../tinySIP/libtinySIP.la
libtinyWRAP_la_CPPFLAGS = \
	-I_common \
	-I. \
	-I../tinySAK/src \
	-I../tinyNET/src \
	-I../tinyHTTP/include\
	-I../tinyXCAP/include \
	-I../tinySMS/include \
	-I../tinySDP/include \
	-I../tinyMEDIA/include \
	-I../tinyMSRP/include \
	-I../tinyDAV/include \
	-I../tinySIP/include

libtinyWRAP_la_LDFLAGS = ${LDFLAGS}

if TARGET_OS_IS_ANDROID
libtinyWRAP_la_CPPFLAGS += -DANDROID=1 -Iandroid -fno-rtti -fno-exceptions
libtinyWRAP_la_LDFLAGS +=  -Wl,-shared,-Bsymbolic,--no-undefined,--whole-archive -lm -lstdc++ -lgcc -llog -ldl
else
libtinyWRAP_la_LDFLAGS += 
endif


if USE_SSL
libtinyWRAP_la_LIBADD += ${LIBSSL_LIBADD}
endif

if USE_FFMPEG
libtinyWRAP_la_LIBADD += ${LIBFFMPEG_LIBADD}
endif

if USE_H264
libtinyWRAP_la_LIBADD += ${LIBH264_LIBADD}
endif

if USE_VPX
libtinyWRAP_la_LIBADD += ${LIBVPX_LIBADD}
endif

if USE_YUV
libtinyWRAP_la_LIBADD += ${LIBYUV_LIB}
endif

if USE_OPUS
libtinyWRAP_la_LIBADD += ${LIBOPUS_LIBADD}
endif

if USE_AMR
libtinyWRAP_la_LIBADD += ${LIBAMR_LIBADD}
endif

if USE_SPEEX
libtinyWRAP_la_LIBADD += ${LIBSPEEX_LIBADD}
endif

if USE_SPEEXDSP
libtinyWRAP_la_LIBADD += ${LIBSPEEXDSP_LIBADD}
endif
	
if USE_GSM
libtinyWRAP_la_LIBADD += ${LIBGSM_LIBADD}
endif

if USE_G729
libtinyWRAP_la_LIBADD += ${LIBG729_LIBADD}
endif

if USE_ILBC
libtinyWRAP_la_LIBADD += ${LIBILBC_LIBADD}
endif

libtinyWRAP_la_SOURCES  = \
	_common/ActionConfig.cxx \
	_common/AudioResampler.cxx \
	_common/DDebug.cxx \
	_common/MediaContent.cxx \
	_common/MediaSessionMgr.cxx \
	_common/Msrp.cxx \
	_common/ProxyConsumer.cxx \
	_common/ProxyPluginMgr.cxx \
	_common/ProxyProducer.cxx \
	_common/SafeObject.cxx \
	_common/SipCallback.cxx \
	_common/SipEvent.cxx \
	_common/SipMessage.cxx \
	_common/SipSession.cxx \
	_common/SipStack.cxx \
	_common/SipUri.cxx \
	_common/SMSEncoder.cxx \
	_common/Xcap.cxx

if TARGET_OS_IS_ANDROID
libtinyWRAP_la_SOURCES  +=  java/android/tinyWRAP_wrap.cxx
endif

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tinyWRAP.pc
