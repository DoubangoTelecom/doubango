#ifndef _LIBDOUBS_LIBDOUBSCONFIG_H
#define _LIBDOUBS_LIBDOUBSCONFIG_H

#if (defined(WIN32) || defined(__SYMBIAN32__)) && defined(LIBDOUBS_EXPORTS)
# 	define LIBDOUBS_API __declspec(dllexport)
#elif (defined(WIN32) || defined(__SYMBIAN32__)) && defined(LIBDOUBS_IMPORTS)
# 	define LIBDOUBS_API __declspec(dllimport)
#else
# define LIBDOUBS_API
#endif

#endif /* _LIBDOUBS_LIBDOUBSCONFIG_H */