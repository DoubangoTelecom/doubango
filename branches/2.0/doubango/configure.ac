AC_PREREQ([2.0])
AC_INIT(libdoubango, 0.1, diopmamadou(at)doubango(DOT)org)
AM_INIT_AUTOMAKE
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([tinySAK/src/tsk.c])

dnl find suitable C and C++ compilers
AC_PROG_CC
AC_PROG_CXX

AC_PROG_INSTALL

AC_PROG_LIBTOOL
AC_LIBTOOL_WIN32_DLL

AC_PROG_SED

AC_PROG_MAKE_SET

## Are we using Windows?
#dnl detecting WIN32
#case "$host" in
#	*cygwin* | *mingw*)
#		AC_DEFINE([WIN32], [], "Using Windows as target os.")
#		;;
#	*)
#		AC_MSG_RESULT(${host} is not Windows... I will do my best.)
#		;;
#esac

#echo Setting up build environment for ${target_cpu}${target_os}

### ENABLE-DEBUG
AC_MSG_CHECKING(whether to enable debugging)
debug_default="yes"
AC_ARG_ENABLE(debug, 
[  --enable-debug[=no/yes] turn on/off debugging 
                       [[default=$debug_default]]],
[ if test "x$enableval" = "xyes" ; then
	CFLAGS="$CFLAGS -O0 -g3 -DDEBUG"
	AC_MSG_RESULT(yes)
  else
	CFLAGS="$CFLAGS -O3 -NDEBUG"
	AC_MSG_RESULT(no)
  fi
],
[])

### DEBUG-LEVEL
AC_ARG_WITH(debug-level, 
[  --with-debug-level[=level] Set debug-level to level [[default=info]]
                       info: informational
                       warn: warnings
                       error [default]: errors
                       fatal: fatal],
[ if test "x$withval" = "xinfo" ; then
	AC_DEFINE(DEBUG_LEVEL, DEBUG_LEVEL_INFO, [info debug])
	AC_MSG_RESULT(info)
  elif test "x$withval" = "xwarn" ; then
	AC_DEFINE(DEBUG_LEVEL, DEBUG_LEVEL_INFO, [warn debug])
	AC_MSG_RESULT(warn)
  elif test "x$withval" = "xerror" ; then
	AC_DEFINE(DEBUG_LEVEL, DEBUG_LEVEL_ERROR, [error debug])
	AC_MSG_RESULT(error)
  elif test "x$withval" = "xfatal" ; then
	AC_DEFINE(DEBUG_LEVEL, DEBUG_LEVEL_FATAL, [fatal debug])
	AC_MSG_RESULT(fatal)
  fi
])

AC_CHECK_FUNCS([stricmp strnicmp])
AC_DEFINE(USE_POLL, 1, [Setting USE_POLL to 1 for backward compatibility])
AC_CHECK_FUNCS([poll])

AC_CHECK_HEADERS([net/if_types.h net/if_dl.h poll.h])

AC_CHECK_FUNC(getifaddrs, AC_DEFINE(HAVE_GETIFADDRS, 1 ,[Define to 1 if you have the 'getifaddrs' function]))
AH_TEMPLATE([HAVE_IFADDRS_H], [Define if <ifaddrs.h> header exist])
AC_CHECK_HEADER([ifaddrs.h], AC_DEFINE(HAVE_IFADDRS_H, 1), AC_DEFINE(HAVE_IFADDRS_H, 0), [])

AH_TEMPLATE([HAVE_NET_IF_DL_H], [Define if <net/if_dl.h> header exist])
AC_CHECK_HEADER([net/if_dl.h], AC_DEFINE(HAVE_NET_IF_DL_H, 1), AC_DEFINE(HAVE_NET_IF_DL_H, 0), [])

AH_TEMPLATE([HAVE_NET_IF_TYPES_H], [Define if <net/if_types> header exist])
AC_CHECK_HEADER([net/if_types.h], AC_DEFINE(HAVE_NET_IF_TYPES_H, 1), AC_DEFINE(HAVE_NET_IF_TYPES_H, 0), [])

AH_TEMPLATE([HAVE_NETPACKET_PACKET_H], [Define if <netpacket/packet.h> header exist])
AC_CHECK_HEADER([netpacket/packet.h], AC_DEFINE(HAVE_NETPACKET_PACKET_H, 1), AC_DEFINE(HAVE_NETPACKET_PACKET_H, 0), [])

AH_TEMPLATE([HAVE_NET_ROUTE_H], [Define if <net/route.h> header exist])
AC_CHECK_HEADER([net/route.h], AC_DEFINE(HAVE_NET_ROUTE_H, 1), AC_DEFINE(HAVE_NET_ROUTE_H, 0), [])
AC_CHECK_TYPES([struct rt_metrics], [], [],
[
#if HAVE_NET_ROUTE_H
# include <net/route.h>
#endif
])
AC_CHECK_TYPES([struct sockaddr_dl], [], [],
[
#if HAVE_NET_IF_DL_H
# include <net/if_dl.h>
#endif
])

AH_TEMPLATE([HAVE_DNS_H], [Define if <dns.h> header exist])
AC_CHECK_HEADER([dns.h], AC_DEFINE(HAVE_DNS_H, 1), AC_DEFINE(HAVE_DNS_H, 0), [])
AM_CONDITIONAL(USE_RESOLV, false)
AC_CHECK_LIB([resolv], [dns_search], 
    AC_SUBST(LIBRESOLV_LIBADD, "-lresolv")
    AM_CONDITIONAL(USE_RESOLV, true), 
    AC_SUBST(LIBRESOLV_LIBADD, "")
    )

AH_TEMPLATE([TNET_HAVE_SS_LEN], [Define if sockaddr_storage.ss_len exists])
AC_CHECK_MEMBER([struct sockaddr_storage.ss_len], AC_DEFINE(TNET_HAVE_SS_LEN, 1), AC_DEFINE(TNET_HAVE_SS_LEN,0), [#include <sys/socket.h>])

AH_TEMPLATE([TNET_HAVE_SA_LEN], [Define if sockaddr.sa_len exists])
AC_CHECK_MEMBER([struct sockaddr.sa_len], AC_DEFINE(TNET_HAVE_SA_LEN, 1), AC_DEFINE(TNET_HAVE_SA_LEN, 0), [#include <netinet/in.h>])


AM_CONDITIONAL(USE_SRTP, false)
AC_ARG_WITH(srtp,
[  --with-srtp            Link against libsrtp library],
 [AC_DEFINE_UNQUOTED(HAVE_SRTP, 1, HAVE_SRTP)
 AC_SUBST(LIBSRTP_LIBADD, "-lsrtp")
 AM_CONDITIONAL(USE_SRTP, true)], 
 [ AC_SUBST(LIBSRTP_LIBADD, "")])

AM_CONDITIONAL(USE_SSL, false)
AC_ARG_WITH(ssl,
[  --with-ssl            Link against OpenSSL libraries],
 [AC_DEFINE_UNQUOTED(HAVE_OPENSSL, 1, HAVE_OPENSSL)
 AC_SUBST(LIBSSL_LIBADD, "-lssl -lcrypto")
 AM_CONDITIONAL(USE_SSL, true)], 
 [ AC_SUBST(LIBSSL_LIBADD, "")])


AM_CONDITIONAL(USE_FFMPEG, false)
AC_ARG_WITH(ffmpeg,
[  --with-ffmpeg            Link against ffmpeg libraries with H.263 and MP4V-ES enabled],
 [AC_DEFINE_UNQUOTED(HAVE_FFMPEG, 1, HAVE_FFMPEG)
 AC_SUBST(LIBFFMPEG_LIBADD, "-lavutil -lswscale -lavcodec")
 AM_CONDITIONAL(USE_FFMPEG, true)], 
 [ AC_SUBST(LIBFFMPEG_LIBADD, "")])

AM_CONDITIONAL(USE_H264, false)
AC_ARG_WITH(h264,
[  --with-h264            Link against x264. Requires ffmpeg libraries with H.264 enabled],
 [AC_DEFINE_UNQUOTED(HAVE_H264, 1, HAVE_H264)
 AC_SUBST(LIBH264_LIBADD, "-lx264")
 AM_CONDITIONAL(USE_H264, true)], 
 [ AC_SUBST(LIBH264_LIBADD, "")])

AM_CONDITIONAL(USE_VPX, false)
AC_ARG_WITH(vpx,
[  --with-vpx            Link against libvpx library],
 [AC_DEFINE_UNQUOTED(HAVE_LIBVPX, 1, HAVE_LIBVPX)
 AC_SUBST(LIBVPX_LIBADD, "-lvpx")
 AM_CONDITIONAL(USE_VPX, true)], 
 [ AC_SUBST(LIBVPX_LIBADD, "")])

AM_CONDITIONAL(USE_YUV, false)
AC_ARG_WITH(yuv,
[  --with-yuv            Link against libyuv library],
 [AC_DEFINE_UNQUOTED(HAVE_LIBYUV, 1, HAVE_LIBYUV)
 AC_SUBST(LIBYUV_LIBADD, "-lyuv")
 AM_CONDITIONAL(USE_YUV, true)], 
 [ AC_SUBST(LIBYUV_LIBADD, "")])

AM_CONDITIONAL(USE_AMR, false)
AC_ARG_WITH(amr,
[  --with-amr            Link against libopencore-amrnb library],
 [AC_DEFINE_UNQUOTED(HAVE_OPENCORE_AMR, 1, HAVE_OPENCORE_AMR)
 AC_SUBST(LIBAMR_LIBADD, "-lopencore-amrnb")
 AM_CONDITIONAL(USE_AMR, true)], 
 [ AC_SUBST(LIBAMR_LIBADD, "")])

AM_CONDITIONAL(USE_SPEEX, false)
AC_ARG_WITH(speex,
[  --with-speex            Link against libspeex library],
 [AC_DEFINE_UNQUOTED(HAVE_LIB_SPEEX, 1, HAVE_LIB_SPEEX)
 AC_SUBST(LIBSPEEX_LIBADD, "-lspeex")
 AM_CONDITIONAL(USE_SPEEX, true)], 
 [ AC_SUBST(LIBSPEEX_LIBADD, "")])

AM_CONDITIONAL(USE_SPEEXDSP, false)
AC_ARG_WITH(speexdsp,
[  --with-speexdsp            Link against libspeexdsp library],
 [AC_DEFINE_UNQUOTED(HAVE_SPEEX_DSP, 1, HAVE_SPEEX_DSP)
 AC_SUBST(LIBSPEEXDSP_LIBADD, "-lspeexdsp")
 AM_CONDITIONAL(USE_SPEEXDSP, true)], 
 [ AC_SUBST(LIBSPEEXDSP_LIBADD, "")])
AC_ARG_ENABLE(speexresampler,
[  --enable-speexresampler           Enable SPEEX RESAMPLER support (requires libspeexdsp)],
 [AC_DEFINE_UNQUOTED(HAVE_SPEEX_RESAMPLER, 1, HAVE_SPEEX_RESAMPLER)],  )
AC_ARG_ENABLE(speexjb,
[  --enable-speexjb           Enable SPEEX JB support (requires libspeexdsp)],
 [AC_DEFINE_UNQUOTED(HAVE_SPEEX_JB, 1, HAVE_SPEEX_JB)],  )
AC_ARG_ENABLE(speexdenoiser,
[  --enable-speexdenoiser           Enable SPEEX DENOISER support (requires libspeexdsp)],
 [AC_DEFINE_UNQUOTED(HAVE_SPEEX_DENOISE, 1, HAVE_SPEEX_DENOISE)],  )


AM_CONDITIONAL(USE_GSM, false)
AC_ARG_WITH(gsm,
[  --with-gsm            Link against libgsm library],
 [AC_DEFINE_UNQUOTED(HAVE_LIBGSM, 1, HAVE_LIBGSM)
 AC_SUBST(LIBGSM_LIBADD, "-lgsm")
 AM_CONDITIONAL(USE_GSM, true)], 
 [ AC_SUBST(LIBGSM_LIBADD, "")])


AM_CONDITIONAL(USE_G729, false)
AC_ARG_WITH(g729,
[  --with-g729            Link against libg729b library],
 [AC_DEFINE_UNQUOTED(HAVE_G729, 1, HAVE_G729)
 AC_SUBST(LIBG729_LIBADD, "-lg729b")
 AM_CONDITIONAL(USE_G729, true)], 
 [ AC_SUBST(LIBG729_LIBADD, "")])

AM_CONDITIONAL(USE_ILBC, false)
AC_ARG_WITH(ilbc,
[  --with-ilbc            Link against libiLBC library],
 [AC_DEFINE_UNQUOTED(HAVE_ILBC, 1, HAVE_ILBC)
 AC_SUBST(LIBILBC_LIBADD, "-liLBC")
 AM_CONDITIONAL(USE_ILBC, true)], 
 [ AC_SUBST(LIBILBC_LIBADD, "")])

AC_OUTPUT(
Makefile
tinySAK/Makefile
tinyNET/Makefile
tinyHTTP/Makefile
tinyXCAP/Makefile
tinyIPSec/Makefile
tinySMS/Makefile
tinySIGCOMP/Makefile
tinySDP/Makefile
tinyRTP/Makefile
tinyMSRP/Makefile
tinyMEDIA/Makefile
tinyDAV/Makefile
tinySIP/Makefile
)