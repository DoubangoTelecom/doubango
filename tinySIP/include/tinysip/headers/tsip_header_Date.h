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

/**@file tsip_header_Date.h
 * @brief SIP dummy header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TSIP_HEADER_DATE_H_
#define _TSIP_HEADER_DATE_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

TSIP_BEGIN_DECLS

#define TSIP_HEADER_DATE_VA_ARGS(wkday, month, day, year, h, m, s)		tsip_header_Date_def_t, (const char*)wkday, (const char*)month, (int8_t)day, (int16_t)year, (int8_t)h, (int8_t)m, (int8_t)s

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @brief	SIP Date header.
///
/// @par ABNF : Date	= 	"Date" HCOLON SIP-date
/// SIP-date	= 	rfc1123-date
/// rfc1123-date	= 	wkday "," SP date1 SP time SP "GMT"
/// date1	= 	2DIGIT SP month SP 4DIGIT
/// time	= 	2DIGIT ":" 2DIGIT ":" 2DIGIT
/// wkday	= 	"Mon" / "Tue" / "Wed" / "Thu" / "Fri" / "Sat" / "Sun"
/// month	= 	"Jan" / "Feb" / "Mar" / "Apr" / "May" / "Jun" / "Jul" / "Aug" / "Sep" / "Oct" / "Nov" / "Dec"
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Date_s
{	
	TSIP_DECLARE_HEADER;

	char *wkday;
	char *month;
	int8_t day;
	int16_t year;
	struct{
		int8_t h;
		int8_t m;
		int8_t s;
	} time;
}
tsip_header_Date_t;

TINYSIP_API tsip_header_Date_t* tsip_header_Date_create(const char* wkday, const char* month, int8_t day, int16_t year, int8_t h, int8_t m, int8_t s);
TINYSIP_API tsip_header_Date_t* tsip_header_Date_create_null();

TINYSIP_API tsip_header_Date_t *tsip_header_Date_parse(const char *data, tsk_size_t size);

TINYSIP_GEXTERN const tsk_object_def_t *tsip_header_Date_def_t;

TSIP_END_DECLS

#endif /* _TSIP_HEADER_DATE_H_ */

