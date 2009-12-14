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

/**@file tsk_string.c
 * @brief Useful string functions to manipulate strings.
 * As I'm a lazy man, some definition come from this <ahref="http://www.cplusplus.com">this website</a>
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tsk_string.h"
#include "tsk_memory.h"

#include <stdarg.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_MSC_VER) || TSK_UNDER_WINDOWS
#	define snprintf		_snprintf
#	define vsnprintf	_vsnprintf
#	define strdup		_strdup
#	define stricmp		_stricmp
#endif

/**@defgroup tsk_string_group String utils
*/


/**@page tsk_string_page String utils Tutorial
*/

static char HEX[] = "0123456789ABCDEF";

/**@ingroup tsk_string_group
* From base 10 to base 16
* @param c the base 10 char to convert to base 16
* @retval The base 16 value
*/
char tsk_b10tob16(char c)
{
	return HEX[c & 15];
}

/**@ingroup tsk_string_group
* From base 16 to base 10
* @param c The base 16 char to convert to base 10
* @retval The base 10 value
*/
char tsk_b16tob10(char c)
{
	return isdigit(c) ? c - '0' : tolower(c) - 'a' + 10;
}

/**@ingroup tsk_string_group
* Compare two strings (case insensitive)
* Compares the C string str1 to the C string str2.
* This function starts comparing the first character of each string. If they are equal to each other, it continues with the following pairs 
* until the characters differ or until a terminanting null-character is reached.
* @param str1 C string to be compared. 
* @param str2 C string to be compared. 
* @retval Returns an integral value indicating the relationship between the strings:
* A zero value indicates that both strings are equal.
* A value greater than zero indicates that the first character that does not match has a greater value in str1 than in str2; And a value less than zero indicates the opposite.
*/
int tsk_stricmp(const char * str1, const char * str2)
{
	return (str1 && str2) ? stricmp(str1, str2) : ((!str1 && !str2) ? 0 : -1);
}

/**@ingroup tsk_string_group
*/
int tsk_strcmp(const char * str1, const char * str2)
{
	return (str1 && str2) ? ((*str1 != *str2) ? -1 : strcmp(str1, str2)) : ((!str1 && !str2) ? 0 : -1);
}

/**@ingroup tsk_string_group
* Duplicate a string
* @param heap The memory heap on which to allocate the duplicated string. Set to NULL if
* you don't want to use heap allocation mechanism.
* @param s1 The string to duplicate
* @retval The duplicated string. */
char* tsk_strdup(const char *s1)
{
	char* ret = 0;
	if(s1)
	{
		ret = strdup(s1);
	}

	return ret;
}

/**@ingroup tsk_string_group
* Appends a copy of the source string to the destination string. The terminating null character in destination is overwritten by the first character of source, 
* and a new null-character is appended at the end of the new string formed by the concatenation of both in destination. If the destination is NULL then new 
* memory will allocated and filled with source value.
* @param destination Pointer de the destination array containing the new string.
* @param source C string to be appended. This should not overlap destination. If NULL then nothing is done.
*/
void tsk_strcat(char** destination, const char* source)
{
	size_t index = 0;

	if(!source) return;

	if(!*destination){
		*destination = (char*)tsk_malloc(strlen(source)+1);
		strncpy(*destination, source, strlen(source)+1);
	}else{
		index = strlen(*destination);
		*destination = tsk_realloc(*destination, index + strlen(source)+1);
		strncpy(((*destination)+index), source, strlen(source)+1);
	}
}

/**@ingroup tsk_string_group
* Writes into the array pointed by str a C string consisting on a sequence of data formatted as the format argument specifies. After the format parameter, 
* the function expects at least as many additional arguments as specified in format.
* This function behaves exactly as printf does, but writing its results to a string instead of stdout. The size of the array passed as str should be enough to 
* contain the entire formatted string.
* @param heap The memory heap on which to allocate the concatened string. Set to NULL if
* you don't want to use heap allocation mechanism.
* @param str Pointer to an array of char elements where the resulting C string is stored. 
* MUST be NULL.
* @param format C string that contains the text to be written to the buffer. For more information see definiton of C function @a sprintf
* @retval On success, the total number of characters written is returned. This count does not include the additional null-character automatically appended 
* at the end of the string.
* On failure, a negative number is returned.
*/
int tsk_sprintf(char** str, const char* format, ...)
{
	int len = 0;
	va_list list;
	
	/* initialize variable arguments */
	va_start(list, format);

	/* free previous value */
	if(*str) tsk_free((void**)str);
	
	/* compute destination len for windows mobile
	*/
#ifdef _WIN32_WCE
	{
		int n;
		len = (strlen(format)*2);
		*str = (char*)tsk_calloc(1, len+1);
		for(;;)
		{
			if( (n = vsnprintf(*str, len, format, list)) >= 0 && (n<len) )
			{
				len = n;
				goto done;
			}
			else
			{
				len += 5;
				*str = tsk_realloc(*str, len+1);
			}
		}
done:
		(*str)[len] = '\0';
	}
#else
    len = vsnprintf(0, 0, format, list);
    *str = (char*)tsk_calloc(1, len+1);
    vsnprintf(*str, len
#if !defined(_MSC_VER) || defined(__GNUC__)
		+1
#endif
		, format, list);
#endif
	
	/* reset variable arguments */
	va_end( list );
	
	return len;
}

/**@ingroup tsk_string_group
*/
void tsk_strupdate(char** str, const char* newval)
{
	tsk_free((void**)str);
	*str = tsk_strdup(newval);
}


/**@ingroup tsk_string_group
*/
void tsk_strtrim_left(char **str)
{
	if(str && *str)
	{
		size_t count = 0;
		while(isspace(*((*str)+count))) count++;
		if(count)
		{
			strcpy((*str), (*str)+count);
		}
	}
}

/**@ingroup tsk_string_group
*/
void tsk_strtrim_right(char **str)
{
	if(str && *str)
	{
		size_t size = strlen(*str);
		if(size)
		{
			while(isspace(*((*str)+size-1))) size--;
			*(*str + size) = '\0';
		}
	}
}

/**@ingroup tsk_string_group
*/
void tsk_strquote(char **str)
{
	if(str && *str)
	{
		char *result = 0;
		tsk_sprintf(&result, "\"%s\"", *str);
		tsk_free(str);
		*str = result;
	}
}

/**@ingroup tsk_string_group
*/
void tsk_strunquote(char **str)
{
	if(str && *str)
	{
		size_t size = strlen(*str);
		if(size>=2 && **str == '"' && *((*str)+size-1) == '"')
		{
			strcpy((*str), (*str)+1);
			*((*str)+size-2) = '\0';
		}
	}
}

/**@ingroup tsk_string_group
* Conversts an integer to string.
* @param i The integer number to convert to a string.
* @param result Pointer to the string where to copy the result.
*/
void tsk_itoa(int64_t i, tsk_istr_t *result)
{
	memset(result, 0, sizeof(*result));
    sprintf(*result,"%lld",i);
}










//========================================================
//	String object definition
//
static void* tsk_string_create(void * self, va_list * app)
{
	tsk_string_t *string = self;
	const char *value = va_arg(*app, const char *);
	if(value)
	{
		string->value = tsk_strdup(value);
	}
	return self;
}

static void* tsk_string_destroy(void * self)
{ 
	tsk_string_t *string = self;
	free(string->value), string->value = 0;

	return self;
}

static const tsk_object_def_t tsk_string_def_s = 
{
	sizeof(tsk_string_t),
	tsk_string_create, 
	tsk_string_destroy,
	0, 
};
const void *tsk_string_def_t = &tsk_string_def_s;
