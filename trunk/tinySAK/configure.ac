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

#LDFLAGS=$LDFLAGS -Wl,-soname,libtinySAK.so,-Bsymbolic,-shared,--whole-archive

echo Setting up build environment for ${target_cpu}${target_os}

## checks string functions
AC_CHECK_FUNCS([stricmp strnicmp])


AC_OUTPUT(
Makefile
src/Makefile
)