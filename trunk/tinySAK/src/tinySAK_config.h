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

/**@def  TINYSAK_API
* Used on Windows and Sysbian systems to export public functions.
*/
#if (defined(WIN32) || defined(_WIN32_WCE) || defined(__SYMBIAN32__)) && defined(TINYSAK_EXPORTS)
# 	define TINYSAK_API __declspec(dllexport)
#elif (defined(WIN32) || defined(_WIN32_WCE) || defined(__SYMBIAN32__)) && defined(TINYSAK_IMPORTS)
# 	define TINYSAK_API __declspec(dllimport)
#else
# define TINYSAK_API
#endif

//
// Disable some well-known warnings
//
#ifdef _MSC_VER
#	define _CRT_SECURE_NO_WARNINGS
#endif

#endif /* _TINYSAK_H_ */