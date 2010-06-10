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

/**@file tsk_time.c
* @brief Datetime functions.
*
* @author Mamadou Diop <diopmamadou(at)doubango.org>
*
* @date Created: Sat Nov 8 16:54:58 2009 mdiop
*/
#include "tsk_time.h"

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

/**@defgroup tsk_time_group Datetime functions.
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

#include <sys/timeb.h>
int gettimeofday(struct timeval *tv, struct timezone *tz) 
{  
	#ifdef _WIN32_WCE
		struct timeb tb;
		ftime (&tb);
#elif defined(__MINGW32__)
		//struct __timeb64 tb;
		//_ftime64 (&tb);
		struct _timeb tb;
		_ftime (&tb);
#else
		struct __timeb64 tb;
		_ftime64_s (&tb);
#endif

		tv->tv_sec = (long)tb.time; // Fix: tv_sec wraps year 2038 (tv.time is ok though)
		tv->tv_usec = tb.millitm * 1000L;
		if( tz ){
			tz->tz_minuteswest = tb.timezone;	/* minutes west of Greenwich  */
			tz->tz_dsttime = tb.dstflag;	/* type of dst correction  */
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
* Gets the number of milliseconds since the EPOCH.
* @retval The number of milliseconds since EPOCH.
*/
uint64_t tsk_time_epoch()
{
	struct timeval tv;
	static const uint64_t thousand = 1000;
	gettimeofday(&tv, tsk_null); 
	
	return (((uint64_t)tv.tv_sec)*thousand) + (((uint64_t)tv.tv_usec)/thousand);
}

