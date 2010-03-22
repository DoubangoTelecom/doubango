/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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

/**@file tinySAK_config.h
 * @brief Global configuration file.
 *
 * This file incude all your preferences or configuration. All specific configuration
 * must be defined in this file. You must include this file in all your header files.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#ifndef _TINYSAK_H_
#define _TINYSAK_H_

#if HAVE_CONFIG_H
	#include "config.h"
#endif

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

/* Guards against C++ name mangling 
*/
#ifdef __cplusplus
#	define TSK_BEGIN_DECLS extern "C" {
#	define TSK_END_DECLS }
#else
#	define TSK_BEGIN_DECLS 
#	define TSK_END_DECLS
#endif

/* Disable some well-known warnings
*/
#ifdef _MSC_VER
#	define _CRT_SECURE_NO_WARNINGS
#	pragma warning( disable : 4996 )
#endif

/*	Features
*/
#if TSK_UNDER_WINDOWS
#	define HAVE_GETTIMEOFDAY				0
#elif !HAVE_CONFIG_H
#	define HAVE_GETTIMEOFDAY				1
#endif

#if defined(__SYMBIAN32__)
#	define USE_GNUC_VA_ARGS		1
#else
#	define USE_GNUC_VA_ARGS		0
#endif

#ifndef TSK_MIN
#define TSK_MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif /* TSK_MIN */

#ifndef TSK_MAX
#define TSK_MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif /* TSK_MAX */

#include <stdint.h>
#include <stddef.h>
#include "tsk_common.h"

#endif /* _TINYSAK_H_ */

