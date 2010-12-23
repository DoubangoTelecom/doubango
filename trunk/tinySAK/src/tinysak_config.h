/*
* Copyright (C) 2009-2010 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#ifndef _TINYSAK_H_
#define _TINYSAK_H_

#ifdef __SYMBIAN32__
#undef _WIN32 /* Because of WINSCW */
#endif

/* Windows (XP/Vista/7/CE and Windows Mobile) macro definition.
*/
#if defined(WIN32)|| defined(_WIN32) || defined(_WIN32_WCE)
#	define TSK_UNDER_WINDOWS	1
#endif

/* Used on Windows and Symbian systems to export/import public functions and global variables.
*/
#if !defined(__GNUC__) && defined(TINYSAK_EXPORTS)
# 	define TINYSAK_API		__declspec(dllexport)
#	define TINYSAK_GEXTERN	__declspec(dllexport)
#elif !defined(__GNUC__) /*&& defined(TINYSAK_IMPORTS)*/
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


/* Disable some well-known warnings for M$ Visual Studio*/
#ifdef _MSC_VER
#	define _CRT_SECURE_NO_WARNINGS
#	pragma warning( disable : 4996 )
#endif

/*	Features */
#if TSK_UNDER_WINDOWS
#	define HAVE_GETTIMEOFDAY				0
#else
#	define HAVE_GETTIMEOFDAY				1
#endif

#if defined(ANDROID)
#	define HAVE_CLOCK_GETTIME	1
#endif

#include <stdint.h>
#include <stddef.h>
#include "tsk_common.h"


#if HAVE_CONFIG_H
#	include "../config.h"
#endif

#endif /* _TINYSAK_H_ */

