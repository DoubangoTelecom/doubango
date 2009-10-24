/****************************************************************************
			 _             _                             
			| |           | |                            
		  _ | | ___  _   _| | _   ____ ____   ____  ___  
		 / || |/ _ \| | | | || \ / _  |  _ \ / _  |/ _ \ 
		( (_| | |_| | |_| | |_) | ( | | | | ( ( | | |_| |
		 \____|\___/ \____|____/ \_||_|_| |_|\_|| |\___/ 
											(_____|   
	
	Copyright (C) 2009 xxxyyyzzz <imsframework(at)gmail.com>

	This file is part of Open Source Doubango IMS Client Framework project.

    DOUBANGO is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
	
    DOUBANGO is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
	
    You should have received a copy of the GNU General Public License
    along with DOUBANGO.
****************************************************************************/
#include "ipsec_utils.h"

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdio.h>

#if defined(_MSC_VER) || defined(WIN32)
#	define snprintf		_snprintf
#	define vsnprintf	_vsnprintf
#	define strdup		_strdup
#	define stricmp		_stricmp
#endif

/* My own sprintf implementation */
/* ATTENTION: use 'IPSEC_SAFE_FREE' macro to free the returned string */
void ipsec_strcat(char** destination, const char* source)
{
	size_t index = 0;

	if(!source) return;

	if(!*destination){
		*destination = (char*)malloc(strlen(source)+1);
		strncpy(*destination, source, strlen(source)+1);
	}else{
		index = strlen(*destination);
		*destination = realloc(*destination, index + strlen(source)+1);
		strncpy(((*destination)+index), source, strlen(source)+1);
	}
}

/* My own sprintf implementation */
/* ATTENTION: use 'IPSEC_SAFE_FREE' macro to free the returned string */
int ipsec_sprintf(char** str, const char* format, ...)
{
    int len = 0;
	va_list list;
	
	/* initialize variable arguments */
	va_start(list, format);
	
	/* compute destination len */
    len = vsnprintf(0, 0, format, list);
    *str = (char*)malloc(len+1);
    vsnprintf(*str, len, format, list);
	(*str)[len] = '\0';
	
	/* reset variable arguments */
	va_end( list );
	
	return len;
}

/* My own strdup implementation */
/* ATTENTION: use 'IPSEC_SAFE_FREE' macro to free the returned string */
char* ipsec_strdup(const char *s1)
{
	return strdup(s1);
}

/* update @str value with @newval */
void ipsec_strupdate(char** str, const char* newval)
{
	IPSEC_SAFE_FREE(*str);
	*str = ipsec_strdup(newval);
}

/* My own stricmp implementation */
int ipsec_stricmp(const char* s1, const char* s2)
{
	return stricmp(s1, s2);
}