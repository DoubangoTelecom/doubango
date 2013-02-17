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

/**@file tsip_header_Date.c
 * @brief SIP DUmmy header.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tinysip/headers/tsip_header_Date.h"

#include "tinysip/parsers/tsip_parser_uri.h"

#include "tsk_debug.h"
#include "tsk_memory.h"

#include <string.h>



/***********************************
*	Ragel state machine.
*/
%%{
	machine tsip_machine_parser_header_Date;

	# Includes
	include tsip_machine_utils "./ragel/tsip_machine_utils.rl";
	
	action tag{
		tag_start = p;
	}

	action parse_wkday{
		TSK_PARSER_SET_STRING(hdr_Date->wkday);
	}

	action parse_day{
		TSK_PARSER_SET_INTEGER(hdr_Date->day);
	}

	action parse_month{
		TSK_PARSER_SET_STRING(hdr_Date->month);
	}

	action parse_year{
		TSK_PARSER_SET_INTEGER(hdr_Date->year);
	}

	action parse_h{
		TSK_PARSER_SET_INTEGER(hdr_Date->time.h);
	}

	action parse_m{
		TSK_PARSER_SET_INTEGER(hdr_Date->time.m);
	}

	action parse_s{
		TSK_PARSER_SET_INTEGER(hdr_Date->time.s);
	}

	action eob{
	}
		
	wkday = "Mon"i | "Tue"i | "Wed"i | "Thu"i | "Fri"i | "Sat"i | "Sun"i;
	month = "Jan"i | "Feb"i | "Mar"i | "Apr"i | "May"i | "Jun"i | "Jul"i | "Aug"i | "Sep"i | "Oct"i | "Nov"i | "Dec"i;
	date1 = DIGIT{2}>tag %parse_day SP month>tag %parse_month SP DIGIT{4}>tag %parse_year;
	time = DIGIT{2}>tag %parse_h ":" DIGIT{2}>tag %parse_m ":" DIGIT{2}>tag %parse_s;
	rfc1123_date = wkday>tag %parse_wkday "," SP date1 SP time SP "GMT"i;
	SIP_date = rfc1123_date;
	Date = "Date"i HCOLON SIP_date;
	
	# Entry point
	main := Date :>CRLF @eob;

}%%

tsip_header_Date_t* tsip_header_Date_create(const char* wkday, const char* month, int8_t day, int16_t year, int8_t h, int8_t m, int8_t s)
{
	return tsk_object_new(TSIP_HEADER_DATE_VA_ARGS(wkday, month, day, year, h, m, s));
}

tsip_header_Date_t* tsip_header_Date_create_null()
{
	return tsip_header_Date_create(tsk_null, tsk_null, -1, -1, -1, -1, -1);
}

int tsip_header_Date_serialize(const tsip_header_t* header, tsk_buffer_t* output)
{
	/* Date: Wed, 28 Apr 2010 23:42:50 GMT */
	if(header){
		const tsip_header_Date_t *Date = (const tsip_header_Date_t *)header;
		if(Date->month){
			tsk_buffer_append_2(output, "%s, %d %s %d %d:%d:%d GMT", 
				Date->wkday, Date->day, Date->month, Date->year, Date->time.h, Date->time.m, Date->time.s);
		}
		return 0;
	}

	return -1;
}

tsip_header_Date_t *tsip_header_Date_parse(const char *data, tsk_size_t size)
{
	int cs = 0;
	const char *p = data;
	const char *pe = p + size;
	const char *eof = pe;
	tsip_header_Date_t *hdr_Date = tsip_header_Date_create_null();
	
	const char *tag_start = tsk_null;

	%%write data;
	(void)(eof);
	(void)(tsip_machine_parser_header_Date_first_final);
	(void)(tsip_machine_parser_header_Date_error);
	(void)(tsip_machine_parser_header_Date_en_main);
	%%write init;
	%%write exec;
	
	if( cs < %%{ write first_final; }%% ){
		TSK_DEBUG_ERROR("Failed to parse 'Date' header.");
		TSK_OBJECT_SAFE_FREE(hdr_Date);
	}
	
	return hdr_Date;
}







//========================================================
//	Date header object definition
//

static tsk_object_t* tsip_header_Date_ctor(tsk_object_t *self, va_list * app)
{
	tsip_header_Date_t *Date = self;
	if(Date){
		const char* wkday;
		const char* month;
		TSIP_HEADER(Date)->type = tsip_htype_Date;
		TSIP_HEADER(Date)->serialize = tsip_header_Date_serialize;
		Date->day = Date->time.h = Date->time.m = Date->time.s = -1;
		Date->year = -1;

		if((wkday = va_arg(*app, const char*))){
			month = va_arg(*app, const char*);

			Date->wkday = tsk_strdup(wkday);
			Date->month = tsk_strdup(month);

#if defined __GNUC__
			Date->day = (int8_t)va_arg(*app, int);
			Date->year = (int16_t)va_arg(*app, int);
			Date->time.h = (int8_t)va_arg(*app, int);
			Date->time.m = (int8_t)va_arg(*app, int);
			Date->time.s = (int8_t)va_arg(*app, int);
#else
			Date->day = va_arg(*app, int8_t);
			Date->year = va_arg(*app, int16_t);
			Date->time.h = va_arg(*app, int8_t);
			Date->time.m = va_arg(*app, int8_t);
			Date->time.s = va_arg(*app, int8_t);
#endif
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new Date header.");
	}
	return self;
}

static tsk_object_t* tsip_header_Date_dtor(tsk_object_t *self)
{
	tsip_header_Date_t *Date = self;
	if(Date){
		TSK_FREE(Date->wkday);
		TSK_FREE(Date->month);

		TSK_OBJECT_SAFE_FREE(TSIP_HEADER_PARAMS(Date));
	}
	else{
		TSK_DEBUG_ERROR("Null Date header.");
	}

	return self;
}

static const tsk_object_def_t tsip_header_Date_def_s = 
{
	sizeof(tsip_header_Date_t),
	tsip_header_Date_ctor,
	tsip_header_Date_dtor,
	tsk_null
};
const tsk_object_def_t *tsip_header_Date_def_t = &tsip_header_Date_def_s;

