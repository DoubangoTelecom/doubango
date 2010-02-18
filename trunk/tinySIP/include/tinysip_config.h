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

#ifndef TINYSIP_CONFIG_H
#define TINYSIP_CONFIG_H

#if HAVE_CONFIG_H
	#include "config.h"
#endif

#ifdef __SYMBIAN32__
#undef _WIN32 /* Because of WINSCW */
#endif

/* Windows (XP/Vista/7/CE and Windows Mobile) macro definition.
*/
#if defined(WIN32)|| defined(_WIN32) || defined(_WIN32_WCE)
#	define TSIP_UNDER_WINDOWS	1
#endif

#if !defined(__GNUC__) && defined(TINYSIP_EXPORTS)
# 	define TINYSIP_API		__declspec(dllexport)
# 	define TINYSIP_GEXTERN	__declspec(dllexport)
#elif !defined(__GNUC__) /*&& defined(TINYSIP_IMPORTS)*/
# 	define TINYSIP_API		__declspec(dllimport)
# 	define TINYSIP_GEXTERN	__declspec(dllimport)
#else
#	define TINYSIP_API
#	define TINYSIP_GEXTERN	extern
#endif


/* Guards against C++ name mangling 
*/
#ifdef __cplusplus
#	define TSIP_BEGIN_DECLS extern "C" {
#	define TSIP_END_DECLS }
#else
#	define TSIP_BEGIN_DECLS 
#	define TSIP_END_DECLS
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

/* FIXME */
#define TSIP_TRUE	1
#define TSIP_FALSE	0
typedef int TSIP_BOOLEAN;
#define TSIP_NULL 0

#endif // TINYSIP_CONFIG_H
