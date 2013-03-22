/* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
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
#ifndef AUDIO_OPENSLES_CONFIG_H
#define AUDIO_OPENSLES_CONFIG_H

#ifdef __SYMBIAN32__
#undef _WIN32 /* Because of WINSCW */
#endif

// Windows (XP/Vista/7/CE and Windows Mobile) macro definition
#if defined(WIN32)|| defined(_WIN32) || defined(_WIN32_WCE)
#	define AUDIO_OPENSLES_UNDER_WINDOWS			1
#endif

// OS X or iOS
#if defined(__APPLE__)
#	define AUDIO_OPENSLES_UNDER_APPLE				1
#endif
#if TARGET_OS_MAC
#	define AUDIO_OPENSLES_UNDER_MAC				1
#endif
#if TARGET_OS_IPHONE
#	define AUDIO_OPENSLES_UNDER_IPHONE			1
#endif
#if TARGET_IPHONE_SIMULATOR
#	define AUDIO_OPENSLES_UNDER_IPHONE_SIMULATOR	1
#endif

#if defined(ANDROID)
#	define AUDIO_OPENSLES_UNDER_ANDROID				1
#endif

// x86
#if AUDIO_OPENSLES_UNDER_WINDOWS || defined(__x86_64__) || defined(__x86__) || defined(__i386__)
#	define AUDIO_OPENSLES_UNDER_X86				1
#endif

// Mobile
#if defined(_WIN32_WCE) || defined(ANDROID) // iOS (not true)=> || defined(IOS)
#	define AUDIO_OPENSLES_UNDER_MOBILE	1
#endif

#if (AUDIO_OPENSLES_UNDER_WINDOWS || defined(__SYMBIAN32__)) && defined(AUDIO_OPENSLES_EXPORTS)
# 	define AUDIO_OPENSLES_API		__declspec(dllexport)
# 	define AUDIO_OPENSLES_GEXTERN __declspec(dllexport)
#elif (AUDIO_OPENSLES_UNDER_WINDOWS || defined(__SYMBIAN32__))
# 	define AUDIO_OPENSLES_API __declspec(dllimport)
# 	define AUDIO_OPENSLES_GEXTERN __declspec(dllimport)
#else
#	define AUDIO_OPENSLES_API
#	define AUDIO_OPENSLES_GEXTERN	extern
#endif

// Guards against C++ name mangling 
#ifdef __cplusplus
#	define AUDIO_OPENSLES_BEGIN_DECLS extern "C" {
#	define AUDIO_OPENSLES_END_DECLS }
#else
#	define AUDIO_OPENSLES_BEGIN_DECLS 
#	define AUDIO_OPENSLES_END_DECLS
#endif

#ifdef _MSC_VER
#if HAVE_FFMPEG // FFMPeg warnings (treated as errors)
#	pragma warning (disable:4244) 
#endif
#	define inline __inline
#	define _CRT_SECURE_NO_WARNINGS
#endif

// Detecting C99 compilers
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

#if AUDIO_OPENSLES_UNDER_ANDROID
#	include <android/log.h>

#	define ANDROID_DEBUG_TAG "plugin_audio_opensles" // DDMS log tag when using eclise
#	define AUDIO_OPENSLES_DEBUG_INFO(FMT, ...)  __android_log_print(ANDROID_LOG_INFO, ANDROID_DEBUG_TAG, FMT, ##__VA_ARGS__)
#	define AUDIO_OPENSLES_DEBUG_WARN(FMT, ...)  __android_log_print(ANDROID_LOG_WARN, ANDROID_DEBUG_TAG, "***WARN: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__)
#	define AUDIO_OPENSLES_DEBUG_ERROR(FMT, ...)  __android_log_print(ANDROID_LOG_ERROR, ANDROID_DEBUG_TAG, "***ERROR: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__)
#	define AUDIO_OPENSLES_DEBUG_FATAL(FMT, ...)  __android_log_print(ANDROID_LOG_FATAL, ANDROID_DEBUG_TAG, "***FATAL: function: \"%s()\" \nfile: \"%s\" \nline: \"%u\" \nMSG: " FMT "\n", __FUNCTION__,  __FILE__, __LINE__, ##__VA_ARGS__)
#else
#	include "tsk_debug.h"
#	define AUDIO_OPENSLES_DEBUG_INFO(FMT, ...) TSK_DEBUG_INFO(FMT, ##__VA_ARGS__)
#	define AUDIO_OPENSLES_DEBUG_WARN(FMT, ...) TSK_DEBUG_WARN(FMT, ##__VA_ARGS__)
#	define AUDIO_OPENSLES_DEBUG_ERROR(FMT, ...) TSK_DEBUG_ERROR(FMT, ##__VA_ARGS__)
#	define AUDIO_OPENSLES_DEBUG_FATAL(FMT, ...) TSK_DEBUG_FATAL(FMT, ##__VA_ARGS__)
#endif /* AUDIO_OPENSLES_UNDER_ANDROID */

#endif // AUDIO_OPENSLES_CONFIG_H