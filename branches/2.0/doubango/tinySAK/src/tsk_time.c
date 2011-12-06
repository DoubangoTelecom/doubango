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

/**@file tsk_time.c
* @brief Datetime functions.
*
* @author Mamadou Diop <diopmamadou(at)doubango.org>
*
* @date Created: Sat Nov 8 16:54:58 2009 mdiop
*/
#include "tsk_time.h"

#include "tsk_debug.h"

#if TSK_UNDER_WINDOWS
//#	ifdef _WIN32_WCE
#		include <Winsock2.h> // timeval
//#	endif
#	include <windows.h>
#elif defined(__SYMBIAN32__)
#	include <_timeval.h> 
#else
#	include <sys/time.h>
#endif

#include <time.h>
#if defined (__APPLE__)
#	include <mach/mach.h>
#	include <mach/mach_time.h>
#endif

/**@defgroup tsk_time_group Datetime functions.
*/

#if !HAVE_GETTIMEOFDAY
#if TSK_UNDER_WINDOWS

/* For windows implementation of "gettimeofday" Thanks to "http://www.cpp-programming.net/c-tidbits/gettimeofday-function-for-windows" */
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS 11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS 11644473600000000ULL
#endif

struct timezone 
{  
	int  tz_minuteswest; /* minutes W of Greenwich */  
	int  tz_dsttime;     /* type of dst correction */
}; 

int gettimeofday(struct timeval *tv, struct timezone *tz) 
{  
	FILETIME ft;
	uint64_t tmpres = 0;  
	static int tzflag = 0; 

	if(tv)   
	{    
#ifdef _WIN32_WCE
		SYSTEMTIME st;
		GetSystemTime(&st);
		SystemTimeToFileTime(&st, &ft);
#else
		GetSystemTimeAsFileTime(&ft);
#endif

		tmpres |= ft.dwHighDateTime;   
		tmpres <<= 32; 
		tmpres |= ft.dwLowDateTime;

		/*converting file time to unix epoch*/   
		tmpres /= 10;  /*convert into microseconds*/  
		tmpres -= DELTA_EPOCH_IN_MICROSECS;  
		tv->tv_sec = (long)(tmpres / 1000000UL); 
		tv->tv_usec = (long)(tmpres % 1000000UL); 
	}

	if (tz){   
		if (!tzflag){    
			_tzset();   
			tzflag++;  
		}   
		tz->tz_minuteswest = _timezone / 60;
		tz->tz_dsttime = _daylight;
	}

	return 0; 
}

#else
#pragma error "You MUST provide an implement for 'gettimeofday'"
#endif /* WIN32 */

#endif /* !HAVE_GETTIMEOFDAY */

/**@ingroup tsk_time_group
* The tsk_gettimeofday() function shall obtain the current time, expressed as seconds and microseconds since EPOCH (00:00:00 UTC on 1 January 1970). 
* The resolution of the system clock is unspecified.
* @param tv The current time, expressed as seconds and microseconds since EPOCH(00:00:00 UTC on 1 January 1970).
* @param tz The timezone.
* @retval The tsk_gettimeofday() function shall return 0 and no value shall be reserved to indicate an error.
*/
int tsk_gettimeofday(struct timeval *tv, struct timezone *tz)
{
	return gettimeofday(tv, tz);
}

/**@ingroup tsk_time_group
* Gets the number of milliseconds in @a tv
* @retval The number of milliseconds
*/
uint64_t tsk_time_get_ms(const struct timeval* tv)
{
	if(!tv){
		TSK_DEBUG_ERROR("Invalid parameter");
		return 0;
	}
	return (((uint64_t)tv->tv_sec)*(uint64_t)1000) + (((uint64_t)tv->tv_usec)/(uint64_t)1000);
}

/**@ingroup tsk_time_group
* Gets the number of milliseconds since the EPOCH.
* @retval The number of milliseconds since EPOCH.
*/
uint64_t tsk_time_epoch()
{
	struct timeval tv;
	gettimeofday(&tv, 0); 
	return (((uint64_t)tv.tv_sec)*(uint64_t)1000) + (((uint64_t)tv.tv_usec)/(uint64_t)1000);
}

uint64_t tsk_time_now()
{
#if TSK_UNDER_WINDOWS
	static LARGE_INTEGER __liFrequency = {0};
	LARGE_INTEGER liPerformanceCount;
	if(!__liFrequency.QuadPart){
		QueryPerformanceFrequency(&__liFrequency);
	}
	QueryPerformanceCounter(&liPerformanceCount);
	return (uint64_t)(((double)liPerformanceCount.QuadPart/(double)__liFrequency.QuadPart)*1000.0);
#elif HAVE_CLOCK_GETTIME || _POSIX_TIMERS > 0
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (((uint64_t)ts.tv_sec)*(uint64_t)1000) + (((uint64_t)ts.tv_nsec)/(uint64_t)1000000);
#elif defined(__APPLE__)
    static mach_timebase_info_data_t __apple_timebase_info = {0, 0};
    if (__apple_timebase_info.denom == 0) {
        (void) mach_timebase_info(&__apple_timebase_info);
    }
    return (uint64_t)((mach_absolute_time() * __apple_timebase_info.numer) / (1e+6 * __apple_timebase_info.denom));
#else
	struct timeval tv;
	gettimeofday(&tv, 0); 
	return (((uint64_t)tv.tv_sec)*(uint64_t)1000) + (((uint64_t)tv.tv_usec)/(uint64_t)1000);
#endif
}

