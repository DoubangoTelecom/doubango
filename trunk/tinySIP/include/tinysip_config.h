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

#ifndef TINYSIP_CONFIG_H
#define TINYSIP_CONFIG_H

#if (defined(WIN32) || defined(_WIN32_WCE) || defined(__SYMBIAN32__)) && defined(TINYSIP_EXPORTS)
# 	define TINYSIP_API __declspec(dllexport)
#elif (defined(WIN32) || defined(_WIN32_WCE) || defined(__SYMBIAN32__)) && defined(TINYSIP_IMPORTS)
# 	define TINYSIP_API __declspec(dllimport)
#else
# define TINYSIP_API
#endif


//
// Disable some well-known warnings
//
#ifdef _MSC_VER
#	define _CRT_SECURE_NO_WARNINGS
#endif

#ifdef __SYMBIAN32__
#undef _WIN32 /* Because of WINSCW */
#endif

#include <stdint.h>
#ifdef __SYMBIAN32__
#include <stdlib.h>
#endif

/* FIXME */
#define TINYSAK_IMPORTS
#define TINYNET_IMPORTS

/* FIXME */
#define TSIP_TRUE	1
#define TSIP_FALSE	0
typedef int TSIP_BOOLEAN;

#endif // TINYSIP_CONFIG_H
