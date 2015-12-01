/* Copyright (C) 2010-2014 Mamadou DIOP
* Copyright (C) 2011-2014 Doubango Telecom <http://www.doubango.org>
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
*/
#ifndef TINYIPSEC_CONFIG_H
#define TINYIPSEC_CONFIG_H

#ifdef __SYMBIAN32__
#undef _WIN32 /* Because of WINSCW */
#endif

// Windows (XP/Vista/7/CE and Windows Mobile) macro definition
#if defined(WIN32)|| defined(_WIN32) || defined(_WIN32_WCE)
#	define TIPSEC_UNDER_WINDOWS	1
#	if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP || WINAPI_FAMILY == WINAPI_FAMILY_APP)
#		define TIPSEC_UNDER_WINDOWS_RT		1
#	endif
#endif

/* Used on Windows and Symbian systems to export/import public functions and global variables.
*/
#if !defined(__GNUC__) && defined(TINYIPSEC_EXPORTS)
# 	define TINYIPSEC_API		__declspec(dllexport)
#	define TINYIPSEC_GEXTERN	__declspec(dllexport)
#elif !defined(__GNUC__) && !defined(TINYIPSEC_IMPORTS_IGNORE)
# 	define TINYIPSEC_API		__declspec(dllimport)
#	define TINYIPSEC_GEXTERN	__declspec(dllimport)
#else
#	define TINYIPSEC_API
#	define TINYIPSEC_GEXTERN	extern
#endif

/* Guards against C++ name mangling
*/
#ifdef __cplusplus
#	define TIPSEC_BEGIN_DECLS extern "C" {
#	define TIPSEC_END_DECLS }
#else
#	define TIPSEC_BEGIN_DECLS
#	define TIPSEC_END_DECLS
#endif

/* Disable some well-known warnings
*/
#ifdef _MSC_VER
#	define _CRT_SECURE_NO_WARNINGS
#	pragma warning( disable : 4996 )
#endif

//
// IPSEC
//
#if HAVE_IPSEC
#	if (_WIN32_WINNT >= 0x0600)
#		define HAVE_IPSEC_VISTA		1
#	elif (_WIN32_WINNT >= 0x0501)
#		define HAVE_IPSEC_XP		0
#	elif HAVE_IPSEC_TOOLS
#		define HAVE_IPSEC_RACOON	1
#	endif
#endif


#if HAVE_CONFIG_H
#include <config.h>
#endif

#endif /* TINYIPSEC_CONFIG_H */

