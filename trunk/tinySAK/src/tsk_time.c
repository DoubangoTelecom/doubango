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

/**@file tsk_time.c
* @brief Useful datetime functions.
*
* @author Mamadou Diop <diopmamadou(at)yahoo.fr>
*
* @date Created: Sat Nov 8 16:54:58 2009 mdiop
*/
#include "tsk_time.h"

#if TSK_UNDER_WINDOWS
#	ifdef _WIN32_WCE
#		include <Winsock2.h> // timeval
#	endif
#	include <windows.h>
#elif defined(__SYMBIAN32__)
#	include <_timeval.h> 
#else
#	include <sys/time.h>
#endif

#include <time.h>

/**@defgroup tsk_time_group Useful datetime functions
*/

#if !HAVE_GETTIMEOFDAY
#if TSK_UNDER_WINDOWS

/* Thanks to "http://www.cpp-programming.net/c-tidbits/gettimeofday-function-for-windows" */
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

	if (tz)
	{   
		if (!tzflag)  
		{    
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

/**
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

/**
* Gets the number of milliseconds since the EPOCH.
* @retval The number of milliseconds since EPOCH.
*/
uint64_t tsk_time_epoch()
{
	struct timeval tv;
	static const uint64_t thousand = 1000;
	gettimeofday(&tv, 0); 
	
	return (((uint64_t)tv.tv_sec)*thousand) + (((uint64_t)tv.tv_usec)/thousand);
}

