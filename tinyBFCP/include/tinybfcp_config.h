/* Copyright (C) 2014 Mamadou DIOP.
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
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/
#ifndef TINYBFCP_CONFIG_H
#define TINYBFCP_CONFIG_H

#ifdef __SYMBIAN32__
#undef _WIN32 /* Because of WINSCW */
#endif

// Windows (XP/Vista/7/CE and Windows Mobile) macro definition
#if defined(WIN32)|| defined(_WIN32) || defined(_WIN32_WCE)
#	define TBFCP_UNDER_WINDOWS	1
#	if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP || WINAPI_FAMILY == WINAPI_FAMILY_APP)
#		define TBFCP_UNDER_WINDOWS_RT		1
#	endif
#endif

#if (TBFCP_UNDER_WINDOWS || defined(__SYMBIAN32__)) && defined(TINYBFCP_EXPORTS)
# 	define TINYBFCP_API		__declspec(dllexport)
# 	define TINYBFCP_GEXTERN extern __declspec(dllexport)
#elif (TBFCP_UNDER_WINDOWS || defined(__SYMBIAN32__)) && !defined(TINYBFCP_IMPORTS_IGNORE)
# 	define TINYBFCP_API __declspec(dllimport)
# 	define TINYBFCP_GEXTERN __declspec(dllimport)
#else
#	define TINYBFCP_API
#	define TINYBFCP_GEXTERN	extern
#endif

/* Guards against C++ name mangling
*/
#ifdef __cplusplus
#	define TBFCP_BEGIN_DECLS extern "C" {
#	define TBFCP_END_DECLS }
#else
#	define TBFCP_BEGIN_DECLS
#	define TBFCP_END_DECLS
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

#if !defined(TBFCP_VERSION)
#	define TBFCP_VERSION 1
#endif /* TBFCP_VERSION */

#endif // TINYBFCP_CONFIG_H
