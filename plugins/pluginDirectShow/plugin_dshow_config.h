/* Copyright (C) 2011-2013 Doubango Telecom <http://www.doubango.org>
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

#ifndef PLUGIN_DSHOW_CONFIG_H
#define PLUGIN_DSHOW_CONFIG_H

#ifdef __SYMBIAN32__
#undef _WIN32 /* Because of WINSCW */
#endif

// Windows (XP/Vista/7/CE and Windows Mobile) macro definition
#if defined(WIN32)|| defined(_WIN32) || defined(_WIN32_WCE)
#	define PLUGIN_DSHOW_UNDER_WINDOWS	1
#	if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP || WINAPI_FAMILY == WINAPI_FAMILY_APP)
#		define PLUGIN_DSHOW_UNDER_WINDOWS_RT		1
#	endif
#endif

#if (PLUGIN_DSHOW_UNDER_WINDOWS || defined(__SYMBIAN32__)) && defined(PLUGIN_DSHOW_EXPORTS)
# 	define PLUGIN_DSHOW_API		__declspec(dllexport)
# 	define PLUGIN_DSHOW_GEXTERN extern __declspec(dllexport)
#elif (PLUGIN_DSHOW_UNDER_WINDOWS || defined(__SYMBIAN32__)) && !defined(PLUGIN_DSHOW_IMPORTS_IGNORE)
# 	define PLUGIN_DSHOW_API __declspec(dllimport)
# 	define PLUGIN_DSHOW_GEXTERN __declspec(dllimport)
#else
#	define PLUGIN_DSHOW_API
#	define PLUGIN_DSHOW_GEXTERN	extern
#endif

/* Guards against C++ name mangling
*/
#ifdef __cplusplus
#	define PLUGIN_DSHOW_BEGIN_DECLS extern "C" {
#	define PLUGIN_DSHOW_END_DECLS }
#else
#	define PLUGIN_DSHOW_BEGIN_DECLS
#	define PLUGIN_DSHOW_END_DECLS
#endif

/* Disable some well-known warnings
*/
#ifdef _MSC_VER
#	pragma warning (disable:4995 4996)
#	define _CRT_SECURE_NO_WARNINGS
#endif

/* Detecting C99 compilers
 */
#if (__STDC_VERSION__ == 199901L) && !defined(__C99__)
#	define __C99__
#endif

#if PLUGIN_DSHOW_UNDER_WINDOWS
#include <windows.h>
#endif

#include <stdint.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#ifdef __SYMBIAN32__
#include <stdlib.h>
#endif

#if HAVE_CONFIG_H
#include <config.h>
#endif

#if !defined(TDSHOW_DEFINE_GUID) && !defined(_WIN32_WCE)
#define TDSHOW_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        EXTERN_C const GUID DECLSPEC_SELECTANY name \
                = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
#elif !defined(TDSHOW_DEFINE_GUID) && defined(_WIN32_WCE)
#define TDSHOW_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        EXTERN_C const GUID __declspec(selectany) name \
                = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
#endif

#if 1 // workaround for "Cannot open include file: 'dxtrans.h': No such file or directory"
#pragma include_alias( "dxtrans.h", "qedit.h" )
#define __IDxtCompositor_INTERFACE_DEFINED__
#define __IDxtAlphaSetter_INTERFACE_DEFINED__
#define __IDxtJpeg_INTERFACE_DEFINED__
#define __IDxtKey_INTERFACE_DEFINED__
#endif

#endif // PLUGIN_DSHOW_CONFIG_H
