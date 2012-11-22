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

# CFLAGS
CFLAGS="${CFLAGS=}"

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
[CFLAGS="$CFLAGS -O0 -g3 -DDEBUG"
	AC_MSG_RESULT(yes)])

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


## checks string functions
AC_CHECK_FUNCS([stricmp strnicmp])

AH_TEMPLATE([HAVE_DNS_H], [Define if <dns.h> header exist])
AC_CHECK_HEADER([dns.h], AC_DEFINE(HAVE_DNS_H, 1), AC_DEFINE(HAVE_DNS_H, 0), [])

AH_TEMPLATE([TNET_HAVE_SS_LEN], [Define if sockaddr_storage.ss_len exists])
AC_CHECK_MEMBER([struct sockaddr_storage.ss_len], AC_DEFINE(TNET_HAVE_SS_LEN, 1), AC_DEFINE(TNET_HAVE_SS_LEN,0), [#include <sys/socket.h>])

AH_TEMPLATE([TNET_HAVE_SA_LEN], [Define if sockaddr.sa_len exists])
AC_CHECK_MEMBER([struct sockaddr.sa_len], AC_DEFINE(TNET_HAVE_SA_LEN, 1), AC_DEFINE(TNET_HAVE_SA_LEN, 0), [#include <netinet/in.h>])


AM_CONDITIONAL(USE_LIBSRTP, false)
AC_ARG_WITH(libsrtp,
[  --with-libsrtp            Link against libsrtp library],
 [AC_DEFINE_UNQUOTED(USE_LIBSRTP, , USE_LIBSRTP)
 AC_SUBST(LIBSRTP_LIBADD, "-lsrtp")
 AM_CONDITIONAL(USE_LIBSRTP, true)], 
 [ AC_SUBST(LIBSRTP_LIBADD, "")])

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