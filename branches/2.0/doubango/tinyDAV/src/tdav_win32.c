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
/**@file tdav_win32.c
 * @brief tinyDAV WIN32 helper functions.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinydav/tdav_win32.h"

#if TDAV_UNDER_WINDOWS

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <windows.h>

/*
Version Number    Description
6.1               Windows 7     / Windows 2008 R2
6.0               Windows Vista / Windows 2008
5.2               Windows 2003 
5.1               Windows XP
5.0               Windows 2000
*/
static DWORD dwMajorVersion = -1;
static DWORD dwMinorVersion = -1;

int tdav_win32_init()
{
#if !TDAV_UNDER_WINDOWS_RT
	MMRESULT result;

	// Timers accuracy
	result = timeBeginPeriod(1);
	if(result){
		TSK_DEBUG_ERROR("timeBeginPeriod(1) returned result=%u", result);
	}

	// Get OS version
	if(dwMajorVersion == -1 || dwMinorVersion == -1){
		OSVERSIONINFO osvi;
		ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&osvi);
		dwMajorVersion = osvi.dwMajorVersion;
		dwMinorVersion = osvi.dwMinorVersion;
		fprintf(stdout, "Windows dwMajorVersion=%ld, dwMinorVersion=%ld", dwMajorVersion, dwMinorVersion);
	}
#endif

	return 0;
}

int tdav_win32_get_osversion(unsigned long* version_major, unsigned long* version_minor)
{
	if(version_major){
		*version_major = dwMajorVersion;
	}
	if(version_minor){
		*version_minor = dwMinorVersion;
	}
	return 0;
}

tsk_bool_t tdav_win32_is_win7_or_later()
{
	if(dwMajorVersion == -1 || dwMinorVersion == -1){
		TSK_DEBUG_ERROR("Version numbers are invalid");
		return tsk_false;
	}
	return ( (dwMajorVersion > 6) || ( (dwMajorVersion == 6) && (dwMinorVersion >= 1) ) );
}

tsk_bool_t tdav_win32_is_winxp_or_later()
{
	if(dwMajorVersion == -1 || dwMinorVersion == -1){
		TSK_DEBUG_ERROR("Version numbers are invalid");
		return tsk_false;
	}
	return ( (dwMajorVersion > 5) || ( (dwMajorVersion == 5) && (dwMinorVersion >= 1) ) );
}

TINYDAV_API void tdav_win32_print_error(const char* func, HRESULT hr)
{
	CHAR message[1024] = {0};

#if TDAV_UNDER_WINDOWS_RT
	// FormatMessageA not allowed on the Store
	static WCHAR wBuff[1024] = {0};
	FormatMessageW(
		  FORMAT_MESSAGE_FROM_SYSTEM, 
		  tsk_null,
		  hr,
		  0,
		  wBuff, 
		  sizeof(wBuff)-1,
		  tsk_null);
	WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wBuff, wcslen(wBuff), message, sizeof(message) - 1, NULL, NULL);
#else
#ifdef _WIN32_WCE
	FormatMessage
#else
	FormatMessageA
#endif
	(
#if !TDAV_UNDER_WINDOWS_RT
	  FORMAT_MESSAGE_ALLOCATE_BUFFER | 
#endif
	  FORMAT_MESSAGE_FROM_SYSTEM, 
	  tsk_null,
	  hr,
	  0,
	  message, 
	  sizeof(message) - 1,
	  tsk_null);
#endif

	TSK_DEBUG_ERROR("%s(): %s", func, message);
}

int tdav_win32_deinit()
{
#if !TDAV_UNDER_WINDOWS_RT
	MMRESULT result;

	// Timers accuracy
	result = timeEndPeriod(1);
	if(result){
		TSK_DEBUG_ERROR("timeEndPeriod(1) returned result=%u", result);
	}
#endif

	return 0;
}

#endif /* TDAV_UNDER_WINDOWS */