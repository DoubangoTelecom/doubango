/* Copyright (C) 2013 Mamadou DIOP
* Copyright (C) 2013 Doubango Telecom <http://www.doubango.org>
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
*/
#ifndef PLUGIN_AUDIO_DSP_CONFIG_H
#define PLUGIN_AUDIO_DSP_CONFIG_H

#ifdef __SYMBIAN32__
#undef _WIN32 /* Because of WINSCW */
#endif


// Windows (XP/Vista/7/CE and Windows Mobile) macro definition
#if defined(WIN32)|| defined(_WIN32) || defined(_WIN32_WCE)
#	define PLUGIN_AUDIO_DSP_UNDER_WINDOWS	1
#	if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP || WINAPI_FAMILY == WINAPI_FAMILY_APP)
#		define PLUGIN_AUDIO_DSP_UNDER_WINDOWS_RT		1
#	endif
#endif

#if (PLUGIN_AUDIO_DSP_UNDER_WINDOWS || defined(__SYMBIAN32__)) && defined(PLUGIN_AUDIO_DSP_EXPORTS)
# 	define PLUGIN_AUDIO_DSP_API		__declspec(dllexport)
# 	define PLUGIN_AUDIO_DSP_GEXTERN extern __declspec(dllexport)
#elif (PLUGIN_AUDIO_DSP_UNDER_WINDOWS || defined(__SYMBIAN32__)) && !defined(PLUGIN_AUDIO_DSP_IMPORTS_IGNORE)
# 	define PLUGIN_AUDIO_DSP_API __declspec(dllimport)
# 	define PLUGIN_AUDIO_DSP_GEXTERN __declspec(dllimport)
#else
#	define PLUGIN_AUDIO_DSP_API
#	define PLUGIN_AUDIO_DSP_GEXTERN	extern
#endif

// x86
#if defined(__x86_64__) || defined(__x86__) || defined(__i386__)
#	define PLUGIN_AUDIO_DSP_UNDER_X86				1
#endif

// Guards against C++ name mangling 
#ifdef __cplusplus
#	define PLUGIN_AUDIO_DSP_BEGIN_DECLS extern "C" {
#	define PLUGIN_AUDIO_DSP_END_DECLS }
#else
#	define PLUGIN_AUDIO_DSP_BEGIN_DECLS 
#	define PLUGIN_AUDIO_DSP_END_DECLS
#endif

#ifdef _MSC_VER
#	define inline __inline
#	define _CRT_SECURE_NO_WARNINGS
#	define _ALLOW_KEYWORD_MACROS
#endif

#include <stdint.h>
#ifdef __SYMBIAN32__
#include <stdlib.h>
#endif

#if HAVE_CONFIG_H
	#include <config.h>
#endif

#endif // PLUGIN_AUDIO_DSP_CONFIG_H
