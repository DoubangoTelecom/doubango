/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
* @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
*
* @date Created: Sat Nov 8 16:54:58 2009 mdiop
*/
#include "tsk_time.h"

#include "tsk_debug.h"

#if TSK_UNDER_WINDOWS
#	include <Winsock2.h> // timeval
#	include <windows.h>
#	if TSK_UNDER_WINDOWS_CE && HAVE_WCECOMPAT
#		include <sys/time.h>
#	endif
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

#if TSK_UNDER_WINDOWS_CE
#endif

struct timezone {
    int  tz_minuteswest; // minutes W of Greenwich
    int  tz_dsttime;     // type of dst correction
};

static int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    FILETIME ft;
    uint64_t tmpres = 0;
    static int tzflag = 0;

    if(tv) {
#if TSK_UNDER_WINDOWS_CE
        SYSTEMTIME st;
        GetSystemTime(&st);
        SystemTimeToFileTime(&st, &ft);
#else
        GetSystemTimeAsFileTime(&ft);
#endif /* TSK_UNDER_WINDOWS_CE */

        tmpres |= ft.dwHighDateTime;
        tmpres <<= 32;
        tmpres |= ft.dwLowDateTime;

        /*converting file time to unix epoch*/
        tmpres /= 10;  /*convert into microseconds*/
        tmpres -= DELTA_EPOCH_IN_MICROSECS;
        tv->tv_sec = (long)(tmpres / 1000000UL);
        tv->tv_usec = (long)(tmpres % 1000000UL);
    }

#if !TSK_UNDER_WINDOWS_CE
    if (tz) {
        if (!tzflag) {
#if !TSK_UNDER_WINDOWS_RT
            _tzset();
#endif /* TSK_UNDER_WINDOWS_RT */
            tzflag++;
        }
        tz->tz_minuteswest = _timezone / 60;
        tz->tz_dsttime = _daylight;
    }
#endif /* TSK_UNDER_WINDOWS_CE */

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
*/
uint64_t tsk_gettimeofday_ms()
{
    struct timeval tv;
    tsk_gettimeofday(&tv, tsk_null);
    return (((uint64_t)tv.tv_sec)*(uint64_t)1000) + (((uint64_t)tv.tv_usec)/(uint64_t)1000);
}

/**@ingroup tsk_time_group
* Gets the number of milliseconds in @a tv
* @retval The number of milliseconds
*/
uint64_t tsk_time_get_ms(const struct timeval* tv)
{
    if(!tv) {
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
    gettimeofday(&tv, (struct timezone *)tsk_null);
    return (((uint64_t)tv.tv_sec)*(uint64_t)1000) + (((uint64_t)tv.tv_usec)/(uint64_t)1000);
}

// /!\ NOT CURRENT TIME
// only make sense when comparing two values (e.g. for duration)
uint64_t tsk_time_now()
{
#if TSK_UNDER_WINDOWS
    static int __cpu_count = 0;
    if(__cpu_count == 0) {
        SYSTEM_INFO SystemInfo;
#	if TSK_UNDER_WINDOWS_RT
        GetNativeSystemInfo(&SystemInfo);
#	else
        GetSystemInfo(&SystemInfo);
#	endif
        __cpu_count = SystemInfo.dwNumberOfProcessors;
    }
    if(__cpu_count == 1) {
        static LARGE_INTEGER __liFrequency = {0};
        LARGE_INTEGER liPerformanceCount;
        if(!__liFrequency.QuadPart) {
            QueryPerformanceFrequency(&__liFrequency);
        }
        QueryPerformanceCounter(&liPerformanceCount);
        return (uint64_t)(((double)liPerformanceCount.QuadPart/(double)__liFrequency.QuadPart)*1000.0);
    }
    else {
#	if TSK_UNDER_WINDOWS_RT
        return tsk_time_epoch();
#	else
        return timeGetTime();
#	endif
    }
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
    gettimeofday(&tv, tsk_null);
    return (((uint64_t)tv.tv_sec)*(uint64_t)1000) + (((uint64_t)tv.tv_usec)/(uint64_t)1000);
#endif
}

// http://en.wikipedia.org/wiki/Network_Time_Protocol
uint64_t tsk_time_ntp()
{
    struct timeval tv;
    gettimeofday(&tv, (struct timezone *)tsk_null);
    return tsk_time_get_ntp_ms(&tv);
}

uint64_t tsk_time_get_ntp_ms(const struct timeval *tv)
{
    static const unsigned long __epoch = 2208988800UL;
    static const double __ntp_scale_frac = 4294967295.0;

    uint64_t tv_ntp;
    uint64_t tv_usecs;

    if(!tv) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    tv_ntp = tv->tv_sec + __epoch;
#if 0 // ARM floating point calc issue (__aeabi_d2uiz)
    tv_usecs = (tv->tv_usec * 1e-6) * __ntp_scale_frac;
    return ((tv_ntp << 32) | (uint32_t)tv_usecs);
#else
    tv_usecs = ((uint64_t)tv->tv_usec * (uint64_t)__ntp_scale_frac) / (uint64_t)1000000;
    return ((((uint64_t)tv_ntp) << 32) | (uint32_t)tv_usecs);
#endif
}