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
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tinyNET_config.h
 * @brief Global configuration file.
 *
 * This file incude all your preferences or configuration. All specific configuration
 * must be defined in this file. You must include this file in all your header files.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */

#ifndef _TINYNET_H_
#define _TINYNET_H_

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#ifdef __SYMBIAN32__
#undef _WIN32 /* Because of WINSCW */
#endif

/* Windows (XP/Vista/7/CE and Windows Mobile) macro definition.
*/
#if defined(WIN32)|| defined(_WIN32) || defined(_WIN32_WCE)
#	define TNET_UNDER_WINDOWS	1
#endif

/**@def  TINYNET_API
* Used on Windows and Sysbian systems to export public functions.
*/
#if !defined(__GNUC__) && defined(TINYNET_EXPORTS)
# 	define TINYNET_API __declspec(dllexport)
# 	define TINYNET_GEXTERN __declspec(dllexport)
#elif !defined(__GNUC__) /*&& defined(TINYNET_IMPORTS)*/
# 	define TINYNET_API __declspec(dllimport)
# 	define TINYNET_GEXTERN __declspec(dllimport)
#else
#	define TINYNET_API
#	define TINYNET_GEXTERN	extern
#endif

/* Guards against C++ name mangling 
*/
#ifdef __cplusplus
#	define TNET_BEGIN_DECLS extern "C" {
#	define TNET_END_DECLS }
#else
#	define TNET_BEGIN_DECLS 
#	define TNET_END_DECLS
#endif

/* Disable some well-known warnings
*/
#ifdef _MSC_VER
#	define _CRT_SECURE_NO_WARNINGS
#	pragma warning( disable : 4996 )
#endif

#if (_WIN32_WINNT>=0x0600) || (ANDROID) || defined (__APPLE__)
#	define TNET_HAVE_POLL		1
#else
#	define TNET_HAVE_POLL		0
#endif

#if TNET_UNDER_WINDOWS
#	define TNET_USE_POLL	0 /* Do not use WSAPoll event if under Vista. */
#else
#	define TNET_USE_POLL	1
#endif

#if defined(__APPLE__)
#	define TNET_HAVE_SS_LEN		1
#else
#	define TNET_HAVE_SS_LEN		0
#endif

#include <stdint.h>

#endif /* _TINYNET_H_ */

