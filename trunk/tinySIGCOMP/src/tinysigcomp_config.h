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

#ifndef TINYSIGCOMP_CONFIG_H
#define TINYSIGCOMP_CONFIG_H

#if HAVE_CONFIG_H
	#include "config.h"
#endif

#ifdef __SYMBIAN32__
#undef _WIN32 /* Because of WINSCW */
#endif

/* Windows (XP/Vista/7/CE and Windows Mobile) macro definition.
*/
#if defined(WIN32)|| defined(_WIN32) || defined(_WIN32_WCE)
#	define TCOMP_UNDER_WINDOWS	1
#endif

#if !defined(__GNUC__) && defined(TINYSIGCOMP_EXPORTS)
# 	define TINYSIGCOMP_API		__declspec(dllexport)
# 	define TINYSIGCOMP_GEXTERN	__declspec(dllexport)
#elif !defined(__GNUC__) /*&& defined(TINYSIGCOMP_IMPORTS)*/
# 	define TINYSIGCOMP_API		__declspec(dllimport)
# 	define TINYSIGCOMP_GEXTERN	__declspec(dllimport)
#else
#	define TINYSIGCOMP_API
#	define TINYSIGCOMP_GEXTERN	extern
#endif

/* Guards against C++ name mangling 
*/
#ifdef __cplusplus
#	define TCOMP_BEGIN_DECLS extern "C" {
#	define TCOMP_END_DECLS }
#else
#	define TCOMP_BEGIN_DECLS 
#	define TCOMP_END_DECLS
#endif

/*	DEFLATE block type 01 (data compressed with fixed Huffman codes)
*/
#ifndef FORCE_STATIC
#	define FORCE_STATIC /*zlib*/
#endif

// avoid linking in the crc code
#define NO_GZIP

//
//	Nack - RFC 4077
//
#define NACK_VERSION			0x01
#define NACK_MAX_HISTORY_SIZE	0x0A

//
//	Feedbacks
//
#define USE_ONLY_ACKED_STATES	1

/* Disable some well-known warnings
*/
#ifdef _MSC_VER
#	define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdint.h>

#endif // TINYSIGCOMP_CONFIG_H

