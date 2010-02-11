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
 * As I'm a lazy man, some comments come from <ahref="http://www.cplusplus.com">this website</a>
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_time.h"
#include "tsk_debug.h"

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
#	define strnicmp		_strnicmp
#elif defined(__GNUC__)
#	define stricmp		strcasecmp
#	define strnicmp		strncasecmp
#endif

/**@defgroup tsk_string_group String utils
*/


/**@page tsk_string_page String utils Tutorial
*/

static char HEX[] = "0123456789abcdef";

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
int tsk_strnicmp(const char * str1, const char * str2, size_t n)
{
	return (str1 && str2) ? strnicmp(str1, str2, n) : ((!str1 && !str2) ? 0 : -1);
}

/**@ingroup tsk_string_group
*/
int tsk_strcmp(const char * str1, const char * str2)
{
	return (str1 && str2) ? ((*str1 != *str2) ? -1 : strcmp(str1, str2)) : ((!str1 && !str2) ? 0 : -1);
}

/**@ingroup tsk_string_group
*/
int tsk_strncmp(const char * str1, const char * str2, size_t n)
{
	return (str1 && str2) ? ((*str1 != *str2) ? -1 : strncmp(str1, str2, n)) : ((!str1 && !str2) ? 0 : -1);
}

/**@ingroup tsk_string_group
* Duplicate a string
* @param heap The memory heap on which to allocate the duplicated string. Set to NULL if
* you don't want to use heap allocation mechanism.
* @param s1 The string to duplicate
* @retval The duplicated string. */
char* tsk_strdup(const char *s1)
{
	if(s1)
	{
		return strdup(s1);
	}
	return 0;
}

/**
 * @fn	char* tsk_strndup(const char *s1, size_t n)
 *
 * @brief	Duplicates the first @a n chars of @a s1.
 *
 * @author	Mamadou
 * @date	1/16/2010
 *
 * @param [in,out]	s1	The string to duplicate. 
 * @param	n			The number of chars to copy to the new string. 
 *
 * @return	null A copy of @a s1. 
**/
char* tsk_strndup(const char *s1, size_t n)
{
	char *ret = 0;
	size_t len = strlen(s1);
	size_t nret = (n > len) ? (len) : (n);

	ret = tsk_calloc((nret+1), sizeof(uint8_t));
	memcpy(ret, s1, nret);

	return ret;
}

/**@ingroup tsk_string_group
* Checks if @ref str1 contains an occurrence of @ref str2.
* @param str1 The master.
* @param str2 The string for which to search an occcurrence.
* @retval 1 if @ref str1 contains an occurrence of @ref str2 and Zero otherwise.
*/
int tsk_strcontains(const char * str1, const char * str2)
{
	return ((str1 && str2) && strstr(str1, str2));
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
	tsk_strncat(destination, source, strlen(source));
}

/**@ingroup tsk_string_group
*/
void tsk_strncat(char** destination, const char* source, size_t n)
{
	size_t index = 0;
	size_t size_to_cat = (n > strlen(source)) ? strlen(source) : n;

	if(!source) return;

	if(!*destination){
		*destination = (char*)tsk_malloc(size_to_cat+1);
		strncpy(*destination, source, size_to_cat+1);
	}else{
		index = strlen(*destination);
		*destination = tsk_realloc(*destination, index + size_to_cat+1);
		strncpy(((*destination)+index), source, size_to_cat+1);
	}
	(*destination)[index + size_to_cat] = '\0';
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
#if defined(_WIN32_WCE)
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
	tsk_strquoteex(str, '"', '"');
}

/**@ingroup tsk_string_group
*/
void tsk_strquoteex(char **str, char lquote, char rquote)
{
	if(str && *str)
	{
		char *result = 0;
		tsk_sprintf(&result, "%c%s%c", lquote, *str, rquote);
		tsk_free((void**)str);
		*str = result;
	}
}

/**@ingroup tsk_string_group
*/
void tsk_strunquote(char **str)
{
	tsk_strunquoteex(str, '"', '"');
}

/**@ingroup tsk_string_group
*/
void tsk_strunquoteex(char **str, char lquote, char rquote)
{
	if(str && *str)
	{
		size_t size = strlen(*str);
		if(size>=2 && **str == lquote && *((*str)+size-1) == rquote)
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


/**
 * @fn	void tsk_strrandom(tsk_istr_t *result)
 *
 * @brief	Generates a random string.
 *
 * @author	Mamadou
 * @date	12/27/2009
 *
 * @param [out]	result	A pointer to the result. 
**/
void tsk_strrandom(tsk_istr_t *result)
{
	uint64_t epoch = tsk_time_epoch();
	tsk_itoa(epoch, result);
}

/**
 * @fn	void tsk_str_from_hex(const uint8_t *hex, size_t size, char* str)
 *
 * @brief	Converts hexadecimal bytes into string representation.
 *
 * @author	Mamadou
 * @date	12/27/2009
 *
 * @param [in,out]	hex	The hexadecimal bytes to convert. 
 * @param	size		The size of the hexadecimal bytes. 
 * @param [in,out]	str	The pointer to the result. MUST be enought large to hold the result.
 *						It is up to you to add the final '\0'.
 * @sa @ref tsk_str_to_hex
**/
void tsk_str_from_hex(const uint8_t *hex, size_t size, char* str)
{
	static const char *TSK_HEXA_VALUES = {"0123456789abcdef"};
	size_t i;

	for (i = 0 ; i<size; i++)
	{
		str[2*i] = TSK_HEXA_VALUES [ (*(hex+i) & 0xf0) >> 4 ];
		str[(2*i)+1] = TSK_HEXA_VALUES [ (*(hex+i) & 0x0f)		];
	}
}

/**
 * @fn	void tsk_str_to_hex(const char *str, size_t size, uint8_t* hex)
 *
 * @brief	Converts string chars into hexadecimal bytes.
 *
 * @author	Mamadou
 * @date	12/27/2009
 *
 * @param [in,out]	str	If non-null, the string. 
 * @param	size		The size. 
 * @param [in,out]	hex	If non-null, the hexadecimal. 
**/
void tsk_str_to_hex(const char *str, size_t size, uint8_t* hex)
{
	TSK_DEBUG_FATAL("Not implemented.");
}










//=================================================================================================
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

static int tsk_string_cmp(const void *obj1, const void *obj2)
{
	const tsk_string_t *s1 = obj1;
	const tsk_string_t *s2 = obj2;

	if(s1 && s2)
	{
		return tsk_stricmp(s1->value, s2->value);
	}
	else if(!s1 && !s2) return 0;
	else return -1;
}

static const tsk_object_def_t tsk_string_def_s = 
{
	sizeof(tsk_string_t),
	tsk_string_create, 
	tsk_string_destroy,
	tsk_string_cmp, 
};
const void *tsk_string_def_t = &tsk_string_def_s;

