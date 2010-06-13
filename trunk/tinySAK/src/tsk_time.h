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

/**@file tsk_time.h
 * @brief Datetime functions.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TINYSAK_TIME_H_
#define _TINYSAK_TIME_H_

#include "tinysak_config.h"

TSK_BEGIN_DECLS

//#if defined(__SYMBIAN32__) || ANDROID /* Forward declaration */
struct timeval;
struct timezone;
//#endif

/**@ingroup tsk_time_group
*/
#define TSK_TIME_S_2_MS(S) ((S)*1000)
#define TSK_TIME_MS_2_S(MS) ((MS)/1000)

TINYSAK_API int tsk_gettimeofday(struct timeval *tv, struct timezone *tz);
TINYSAK_API uint64_t tsk_time_get_ms(struct timeval *tv);
TINYSAK_API uint64_t tsk_time_epoch();

/**@ingroup tsk_time_group
* Gets the number of milliseconds since the EPOCH.
*/
#define tsk_time_now() tsk_time_epoch()

//#ifdef _WIN32_WCE
//
//#ifndef TIMEVAL
///* On wince timeval struct is defined in "Winsock2.h" but I don't want to add it */
//struct timeval 
//{
//  long tv_sec;
//  long tv_usec;
//};
//#define TIMEVAL
//#endif TIMEVAL
//
//#endif

TSK_END_DECLS

#endif /* _TINYSAK_TIME_H_ */

