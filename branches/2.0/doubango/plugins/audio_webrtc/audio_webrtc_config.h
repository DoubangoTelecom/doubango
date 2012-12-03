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

#ifndef DOUBANGO_AUDIO_WEBRTC_CONFIG_H
#define DOUBANGO_AUDIO_WEBRTC_CONFIG_H

#ifdef __SYMBIAN32__
#undef _WIN32 /* Because of WINSCW */
#endif

// Windows (XP/Vista/7/CE and Windows Mobile) macro definition
#if defined(WIN32)|| defined(_WIN32) || defined(_WIN32_WCE)
#	define DOUBANGO_AUDIO_WEBRTC_UNDER_WINDOWS			1
#endif

// OS X or iOS
#if defined(__APPLE__)
#	define DOUBANGO_AUDIO_WEBRTC_UNDER_APPLE				1
#endif
#if TARGET_OS_MAC
#	define DOUBANGO_AUDIO_WEBRTC_UNDER_MAC				1
#endif
#if TARGET_OS_IPHONE
#	define DOUBANGO_AUDIO_WEBRTC_UNDER_IPHONE			1
#endif
#if TARGET_IPHONE_SIMULATOR
#	define DOUBANGO_AUDIO_WEBRTC_UNDER_IPHONE_SIMULATOR	1
#endif

#if defined(ANDROID)
#	define DOUBANGO_AUDIO_WEBRTC_UNDER_ANDROID				1
#endif

// x86
#if DOUBANGO_AUDIO_WEBRTC_UNDER_WINDOWS || defined(__x86_64__) || defined(__x86__) || defined(__i386__)
#	define DOUBANGO_AUDIO_WEBRTC_UNDER_X86				1
#endif

// Mobile
#if defined(_WIN32_WCE) || defined(ANDROID) // iOS (not true)=> || defined(IOS)
#	define DOUBANGO_AUDIO_WEBRTC_UNDER_MOBILE	1
#endif

#if (DOUBANGO_AUDIO_WEBRTC_UNDER_WINDOWS || defined(__SYMBIAN32__)) && defined(DOUBANGO_AUDIO_WEBRTC_EXPORTS)
# 	define DOUBANGO_AUDIO_WEBRTC_API		__declspec(dllexport)
# 	define DOUBANGO_AUDIO_WEBRTC_GEXTERN __declspec(dllexport)
#elif (DOUBANGO_AUDIO_WEBRTC_UNDER_WINDOWS || defined(__SYMBIAN32__))
# 	define DOUBANGO_AUDIO_WEBRTC_API __declspec(dllimport)
# 	define DOUBANGO_AUDIO_WEBRTC_GEXTERN __declspec(dllimport)
#else
#	define DOUBANGO_AUDIO_WEBRTC_API
#	define DOUBANGO_AUDIO_WEBRTC_GEXTERN	extern
#endif

/* Guards against C++ name mangling  */
#ifdef __cplusplus
#	define DOUBANGO_AUDIO_WEBRTC_BEGIN_DECLS extern "C" {
#	define DOUBANGO_AUDIO_WEBRTC_END_DECLS }
#else
#	define DOUBANGO_AUDIO_WEBRTC_BEGIN_DECLS 
#	define DOUBANGO_AUDIO_WEBRTC_END_DECLS
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

#if DOUBANGO_AUDIO_WEBRTC_UNDER_WINDOWS
#	define DOUBANGO_AUDIO_WEBRTC_DEVICE_DEFAULT	AudioDeviceModule::kDefaultCommunicationDevice
#else
#	define DOUBANGO_AUDIO_WEBRTC_DEVICE_DEFAULT	0
#endif

#if DOUBANGO_AUDIO_WEBRTC_UNDER_ANDROID
#include <android/log.h>
#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"
#define ANDROID_DEBUG_TAG "plugin_audio_webrtc" // DDMS log tag when using eclise
static void DOUBANGO_AUDIO_WEBRTC_DEBUG_ANY(int level, const char* fmt, ...)
{
	char* message = tsk_null;
	va_list ap;
	va_start(ap, fmt);
	tsk_sprintf_2(&message, fmt, &ap);

	if(message){
		switch(level){
			case DEBUG_LEVEL_INFO: __android_log_write(ANDROID_LOG_INFO, ANDROID_DEBUG_TAG, message); break;
			case DEBUG_LEVEL_WARN: __android_log_write(ANDROID_LOG_WARN, ANDROID_DEBUG_TAG, message); break;
			case DEBUG_LEVEL_ERROR: __android_log_write(ANDROID_LOG_ERROR, ANDROID_DEBUG_TAG, message); break;
			case DEBUG_LEVEL_FATAL: __android_log_write(ANDROID_LOG_FATAL, ANDROID_DEBUG_TAG, message); break;
		}
		TSK_FREE(message);
	}

	va_end(ap);
}
#define DOUBANGO_AUDIO_WEBRTC_DEBUG_INFO(FMT, ...) DOUBANGO_AUDIO_WEBRTC_DEBUG_ANY(DEBUG_LEVEL_INFO, FMT, ##__VA_ARGS__)
#define DOUBANGO_AUDIO_WEBRTC_DEBUG_WARN(FMT, ...) DOUBANGO_AUDIO_WEBRTC_DEBUG_ANY(DEBUG_LEVEL_WARN, FMT, ##__VA_ARGS__)
#define DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR(FMT, ...) DOUBANGO_AUDIO_WEBRTC_DEBUG_ANY(DEBUG_LEVEL_ERROR, FMT, ##__VA_ARGS__)
#define DOUBANGO_AUDIO_WEBRTC_DEBUG_FATAL(FMT, ...) DOUBANGO_AUDIO_WEBRTC_DEBUG_ANY(DEBUG_LEVEL_FATAL, FMT, ##__VA_ARGS__)
#else
#define DOUBANGO_AUDIO_WEBRTC_DEBUG_INFO(FMT, ...) TSK_DEBUG_INFO(FMT, ##__VA_ARGS__)
#define DOUBANGO_AUDIO_WEBRTC_DEBUG_WARN(FMT, ...) TSK_DEBUG_WARN(FMT, ##__VA_ARGS__)
#define DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR(FMT, ...) TSK_DEBUG_ERROR(FMT, ##__VA_ARGS__)
#define DOUBANGO_AUDIO_WEBRTC_DEBUG_FATAL(FMT, ...) TSK_DEBUG_FATAL(FMT, ##__VA_ARGS__)
#endif /* DOUBANGO_AUDIO_WEBRTC_UNDER_ANDROID */

#endif // DOUBANGO_AUDIO_WEBRTC_CONFIG_H
