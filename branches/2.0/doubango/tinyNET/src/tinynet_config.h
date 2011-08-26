/*
* Copyright (C) 2010-2011 Mamadou Diop.
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

/**@file tinynet_config.h
 * @brief Global configuration file.
 *
 * This file incude all your preferences or configuration. All specific configuration
 * must be defined in this file. You must include this file in all your header files.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */

#ifndef _TINYNET_H_
#define _TINYNET_H_

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

#if defined(_MSC_VER)
#	define TNET_INLINE	__forceinline
#elif defined(__GNUC__) && !defined(__APPLE__)
#	define TNET_INLINE	__inline
#else
#	define TNET_INLINE	
#endif

/* have poll()? */
#if (_WIN32_WINNT>=0x0600) || (ANDROID) || defined(__APPLE__)
#	define TNET_HAVE_POLL		1
#else
#	define TNET_HAVE_POLL		0
#endif

/* whether to use poll() */
#if TNET_UNDER_WINDOWS
#	define TNET_USE_POLL		0 /* Do not use WSAPoll event if under Vista. */
#else
#	define TNET_USE_POLL		1
#endif

#if defined(__APPLE__)
#	define HAVE_IFADDRS			1
#   define HAVE_DNS_H			1
#	define TNET_HAVE_SS_LEN		1
#	define TNET_HAVE_SA_LEN		0
#else
#	define HAVE_IFADDRS			0
#   define HAVE_DNS_H			0
#	define TNET_HAVE_SS_LEN		0
#	define TNET_HAVE_SA_LEN		0
#endif

/* Used in TURN/STUN2 attributes. */
#define TNET_SOFTWARE	"IM-client/OMA1.0 doubango/v0.0.0"
#define TNET_IANA_PEN		35368 /**< PEN number assigned by the IANA.
	The list of assigned numbers could be found here http://www.iana.org/assignments/enterprise-numbers. */
#define TNET_RESOLV_CONF_PATH "/etc/resolv.conf" /**< Path to "/resolv.conf". */

#include <stdint.h>


#ifdef HAVE_CONFIG_H
#	include "../config.h"
#endif

#endif /* _TINYNET_H_ */


