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

#ifndef TINYDAV_CONFIG_H
#define TINYDAV_CONFIG_H

#ifdef __SYMBIAN32__
#undef _WIN32 /* Because of WINSCW */
#endif

/* Windows (XP/Vista/7/CE and Windows Mobile) macro definition.
*/
#if defined(WIN32)|| defined(_WIN32) || defined(_WIN32_WCE)
#	define TDAV_UNDER_WINDOWS	1
#endif

#if (TDAV_UNDER_WINDOWS || defined(__SYMBIAN32__)) && defined(TINYDAV_EXPORTS)
# 	define TINYDAV_API		__declspec(dllexport)
# 	define TINYDAV_GEXTERN __declspec(dllexport)
#elif (TDAV_UNDER_WINDOWS || defined(__SYMBIAN32__)) /*&& defined(TINYDAV_IMPORTS)*/
# 	define TINYDAV_API __declspec(dllimport)
# 	define TINYDAV_GEXTERN __declspec(dllimport)
#else
#	define TINYDAV_API
#	define TINYDAV_GEXTERN	extern
#endif

/* Guards against C++ name mangling 
*/
#ifdef __cplusplus
#	define TDAV_BEGIN_DECLS extern "C" {
#	define TDAV_END_DECLS }
#else
#	define TDAV_BEGIN_DECLS 
#	define TDAV_END_DECLS
#endif

#ifdef _MSC_VER
#if HAVE_FFMPEG // FFMPeg warnings (treated as errors)
#	pragma warning (disable:4244) 
#endif
#	define inline __inline
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
	#include "../config.h"
#endif

#endif // TINYDAV_CONFIG_H
