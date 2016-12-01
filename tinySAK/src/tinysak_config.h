/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
*
* This file is part of Open Source Doubango Framework.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* DOUBANGO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tinysak_config.h
 * @brief Global configuration file.
 *
 * This file incude all your preferences or configuration. All specific configuration
 * must be defined in this file. You must include this file in all your header files.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */

#ifndef _TINYSAK_H_
#define _TINYSAK_H_

#ifdef __SYMBIAN32__
#undef _WIN32 /* Because of WINSCW */
#endif

// Windows (XP/Vista/7/CE and Windows Mobile) macro definition.
#if defined(WIN32)|| defined(_WIN32) || defined(_WIN32_WCE)
#	define TSK_UNDER_WINDOWS	1
#	if defined(_WIN32_WCE) || defined(UNDER_CE)
#		define TSK_UNDER_WINDOWS_CE	1
#		define TSK_STDCALL			__cdecl
#	else
#		define TSK_STDCALL __stdcall
#	endif
#	if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP || WINAPI_FAMILY == WINAPI_FAMILY_APP)
#		define TSK_UNDER_WINDOWS_RT		1
#	endif
#else
#	define TSK_STDCALL
#endif

// OS X or iOS
#if defined(__APPLE__)
#	define TSK_UNDER_APPLE				1
#   include <TargetConditionals.h>
#   include <Availability.h>
#endif
#if TARGET_OS_MAC
#	define TSK_UNDER_MAC				1
#endif
#if TARGET_OS_IPHONE
#	define TSK_UNDER_IPHONE			1
#endif
#if TARGET_IPHONE_SIMULATOR
#	define TSK_UNDER_IPHONE_SIMULATOR	1
#endif

/* Used on Windows and Symbian systems to export/import public functions and global variables.
*/
#if !defined(__GNUC__) && defined(TINYSAK_EXPORTS)
# 	define TINYSAK_API		__declspec(dllexport)
#	define TINYSAK_GEXTERN	extern __declspec(dllexport)
#elif !defined(__GNUC__) && !defined(TINYSAK_IMPORTS_IGNORE)
# 	define TINYSAK_API		__declspec(dllimport)
#	define TINYSAK_GEXTERN	__declspec(dllimport)
#else
#	define TINYSAK_API
#	define TINYSAK_GEXTERN	extern
#endif

/* Guards against C++ name mangling */
#ifdef __cplusplus
#	define TSK_BEGIN_DECLS extern "C" {
#	define TSK_END_DECLS }
#else
#	define TSK_BEGIN_DECLS
#	define TSK_END_DECLS
#endif

#if defined(_MSC_VER)
#	define TSK_INLINE	__forceinline
#elif defined(__GNUC__) && !defined(__APPLE__)
#	define TSK_INLINE	__inline
#else
#	define TSK_INLINE
#endif

/* Disable some well-known warnings */
#if defined(_MSC_VER)
#	if !defined(_CRT_SECURE_NO_WARNINGS)
#		define _CRT_SECURE_NO_WARNINGS
#	endif /* _CRT_SECURE_NO_WARNINGS */
#	pragma warning( disable : 4996 )
#	define TSK_VS_DISABLE_WARNINGS_BEGIN(nn) \
		__pragma(warning( push )) \
		__pragma(warning( disable : ##nn ))
#	define TSK_VS_DISABLE_WARNINGS_END() \
		__pragma(warning( pop ))
#else
#	define TSK_VS_DISABLE_WARNINGS_BEGIN(nn)
#	define TSK_VS_DISABLE_WARNINGS_END()
#endif /* _MSC_VER */
#if defined(__GNUC__)
#	define TSK_GCC_DO_PRAGMA(x) _Pragma (#x)
#	define TSK_GCC_DISABLE_WARNINGS_BEGIN(w) \
		TSK_GCC_DO_PRAGMA(GCC diagnostic push) \
		TSK_GCC_DO_PRAGMA(GCC diagnostic ignored w)
#	define TSK_GCC_DISABLE_WARNINGS_END() \
		TSK_GCC_DO_PRAGMA(GCC diagnostic pop)
#else
#	define TSK_GCC_DISABLE_WARNINGS_BEGIN(tt)
#	define TSK_GCC_DISABLE_WARNINGS_END()
#endif /* __GNUC__ */

/*	Features */
#if !defined (HAVE_GETTIMEOFDAY)
#if TSK_UNDER_WINDOWS
#	define HAVE_GETTIMEOFDAY				0
#else
#	define HAVE_GETTIMEOFDAY				1
#endif
#endif /* HAVE_GETTIMEOFDAY */

#if defined(__ANDROID__) || defined(ANDROID)
#	define TSK_UNDER_ANDROID				1
#	define HAVE_CLOCK_GETTIME				1
#endif

#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#include "tsk_common.h"


#if HAVE_CONFIG_H
#	include <config.h>
#endif

#endif /* _TINYSAK_H_ */

