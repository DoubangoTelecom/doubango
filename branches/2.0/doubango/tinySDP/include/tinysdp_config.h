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

#ifndef TINYSDP_CONFIG_H
#define TINYSDP_CONFIG_H

#ifdef __SYMBIAN32__
#undef _WIN32 /* Because of WINSCW */
#endif

// Windows (XP/Vista/7/CE and Windows Mobile) macro definition.
#if defined(WIN32)|| defined(_WIN32) || defined(_WIN32_WCE)
#	define TSDP_UNDER_WINDOWS	1
#	if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP || WINAPI_FAMILY == WINAPI_FAMILY_APP)
#		define TSDP_UNDER_WINDOWS_RT		1
#	endif
#endif


#if (TSDP_UNDER_WINDOWS || defined(__SYMBIAN32__)) && defined(TINYSDP_EXPORTS)
# 	define TINYSDP_API		__declspec(dllexport)
# 	define TINYSDP_GEXTERN extern __declspec(dllexport)
#elif (TSDP_UNDER_WINDOWS || defined(__SYMBIAN32__)) && !defined(TINYSDP_IMPORTS_IGNORE)
# 	define TINYSDP_API __declspec(dllimport)
# 	define TINYSDP_GEXTERN __declspec(dllimport)
#else
#	define TINYSDP_API
#	define TINYSDP_GEXTERN	extern
#endif

/* Guards against C++ name mangling 
*/
#ifdef __cplusplus
#	define TSDP_BEGIN_DECLS extern "C" {
#	define TSDP_END_DECLS }
#else
#	define TSDP_BEGIN_DECLS 
#	define TSDP_END_DECLS
#endif

/* Disable some well-known warnings
*/
#ifdef _MSC_VER
#	define _CRT_SECURE_NO_WARNINGS
#endif

/* Detecting C99 compilers
 */
#if (__STDC_VERSION__ == 199901L) && !defined(__C99__)
#	define __C99__
#endif

#include <stdint.h>
#ifdef __SYMBIAN32__
#include <stdlib.h>
#endif

#if HAVE_CONFIG_H
	#include <config.h>
#endif

#endif // TINYSDP_CONFIG_H
