/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou [at) doubango (DOT) org>
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

/**@file tinyMSRP_config.h
 * @brief Global configuration file.
 *
 * This file incude all your preferences or configuration. All specific configuration
 * must be defined in this file. You must include this file in all your header files.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */

#ifndef _TINYMSRP_H_
#define _TINYMSRP_H_

#ifdef __SYMBIAN32__
#undef _WIN32 /* Because of WINSCW */
#endif

// Windows (XP/Vista/7/CE and Windows Mobile) macro definition.
#if defined(WIN32)|| defined(_WIN32) || defined(_WIN32_WCE)
#	define TMSRP_UNDER_WINDOWS	1
#	if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP || WINAPI_FAMILY == WINAPI_FAMILY_APP)
#		define TMSRP_UNDER_WINDOWS_RT		1
#	endif
#endif

/* Used on Windows and Symbian systems to export/import public functions and global variables.
*/
#if !defined(__GNUC__) && defined(TINYMSRP_EXPORTS)
# 	define TINYMSRP_API		__declspec(dllexport)
#	define TINYMSRP_GEXTERN	extern __declspec(dllexport)
#elif !defined(__GNUC__) && !defined(TINYMSRP_IMPORTS_IGNORE)
# 	define TINYMSRP_API		__declspec(dllimport)
#	define TINYMSRP_GEXTERN	__declspec(dllimport)
#else
#	define TINYMSRP_API
#	define TINYMSRP_GEXTERN	extern
#endif

/* Guards against C++ name mangling
*/
#ifdef __cplusplus
#	define TMSRP_BEGIN_DECLS extern "C" {
#	define TMSRP_END_DECLS }
#else
#	define TMSRP_BEGIN_DECLS
#	define TMSRP_END_DECLS
#endif

/* Disable some well-known warnings
*/
#ifdef _MSC_VER
#	define _CRT_SECURE_NO_WARNINGS
#	pragma warning( disable : 4996 )
#endif

#include <stdint.h>
#include <stddef.h>


#if HAVE_CONFIG_H
#include <config.h>
#endif

#endif /* _TINYMSRP_H_ */

