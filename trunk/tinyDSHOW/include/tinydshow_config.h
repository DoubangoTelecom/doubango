/*
* Copyright (C) 2009 Mamadou Diop.
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

#ifndef TINYDSHOW_CONFIG_H
#define TINYDSHOW_CONFIG_H

#include <windows.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


#if defined(TINYDSHOW_EXPORTS)
# 	define TINYDSHOW_API		__declspec(dllexport)
# 	define TINYDSHOW_GEXTERN __declspec(dllexport)
#else
# 	define TINYDSHOW_API __declspec(dllimport)
# 	define TINYDSHOW_GEXTERN __declspec(dllimport)
#endif

/* Guards against C++ name mangling  */
#ifdef __cplusplus
#	define TDSHOW_BEGIN_DECLS extern "C" {
#	define TDSHOW_END_DECLS }
#else
#	define TDSHOW_BEGIN_DECLS 
#	define TDSHOW_END_DECLS
#endif

/* Disable some well-known warnings */
#ifdef _MSC_VER
#	pragma warning (disable:4995) 
#	define _CRT_SECURE_NO_WARNINGS
#endif

#if HAVE_CONFIG_H
	#include "../config.h"
#endif

#endif // TINYDSHOW_CONFIG_H
