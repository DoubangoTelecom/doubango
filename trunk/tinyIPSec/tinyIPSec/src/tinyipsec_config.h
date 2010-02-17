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

#ifndef TINYIPSEC_CONFIG_H
#define TINYIPSEC_CONFIG_H

#ifdef WIN32
//#include <windows.h>
#include <ws2tcpip.h>
#endif

#if (defined(WIN32) || defined(__SYMBIAN32__)) && defined(TINYIPSEC_EXPORTS)
# 	define TINYIPSEC_API __declspec(dllexport)
#elif (defined(WIN32) || defined(__SYMBIAN32__)) && defined(TINYIPSEC_IMPORTS)
# 	define TINYIPSEC_API __declspec(dllimport)
#else
# define TINYIPSEC_API
#endif

//
// Disable some well-known warnings
//
#ifdef _MSC_VER
#	define _CRT_SECURE_NO_WARNINGS
#endif

//
// IPSEC
//
#if (_WIN32_WINNT >= 0x0600 || WINVER >= 0x0600)
#	define HAVE_WIN32_IPSEC 1
#elif HAVE_IPSEC_TOOLS
#	define HAVE_LINUX_IPSEC 1
#endif /* IPSEC */

#endif // TINYIPSEC_CONFIG_H
