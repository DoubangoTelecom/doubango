AC_PREREQ([2.52])
AC_INIT(libtinySAK, 0.1, diopmamadou@doubango.org)
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([src/tsk.c])

dnl find suitable C++ compiler
AC_PROG_CC
AC_PROG_INSTALL


AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

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


AC_OUTPUT(
Makefile
src/Makefile
test/Makefile
)