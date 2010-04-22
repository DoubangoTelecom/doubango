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

/**@file tsk_string.c
 * @brief Useful string functions to manipulate strings.
 * As I'm a lazy man, some comments come from <ahref="http://www.cplusplus.com">this website</a>
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_time.h"
#include "tsk_debug.h"
#include "tsk_common.h"

#include <stdarg.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_MSC_VER)
#	define snprintf		_snprintf
#	define vsnprintf	_vsnprintf
#	define strdup		_strdup
#	define stricmp		_stricmp
#	define strnicmp		_strnicmp
#else
#	if !HAVE_STRNICMP && !HAVE_STRICMP
#	define stricmp		strcasecmp
#	define strnicmp		strncasecmp
#	endif
#endif

/**@defgroup tsk_string_group String utillity functions.
*/


static char HEX[] = "0123456789abcdef";

/**@ingroup tsk_string_group
*/
tsk_string_t* tsk_string_create(const char* str)
{
	return tsk_object_new(tsk_string_def_t, str);
}

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
* Compare two Null-terminated strings (case insensitive)
* Compares the C string str1 to the C string str2.
* This function starts comparing the first character of each string. If they are equal to each other, it continues with the following pairs 
* until the characters differ or until a terminanting null-character is reached.
* @param str1 First C string to be compared. 
* @param str2 Second C string to be compared. 
* @retval Returns an integral value indicating the relationship between the strings:
* <0 : str1 less than str2.<br>
* 0  : str1 identical to str1.<br>
* >0 : str1 greater than str2.<br>
*/
int tsk_stricmp(const char * str1, const char * str2)
{
	return (str1 && str2) ? stricmp(str1, str2) : ((!str1 && !str2) ? 0 : -1);
}

/**@ingroup tsk_string_group
* Compare two Null-terminated strings (case insensitive)
* Compares the C string str1 to the C string str2.
* This function starts comparing the first character of each string. If they are equal to each other, it continues with the following pairs 
* until the characters differ or until a terminanting null-character is reached or @a n characters passed.
* @param str1 First C string to be compared. 
* @param str2 Second C string to be compared. 
* @param n The maximum number of characters to compare.
* @retval Returns an integral value indicating the relationship between the strings:
* <0 : str1 less than str2.<br>
* 0  : str1 identical to str1.<br>
* >0 : str1 greater than str2.<br>
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
* Compare two Null-terminated strings (case sensitive)
* Compares the C string str1 to the C string str2.
* This function starts comparing the first character of each string. If they are equal to each other, it continues with the following pairs 
* until the characters differ or until a terminanting null-character is reached.
* @param str1 First C string to be compared. 
* @param str2 Second C string to be compared.
* @param n The maximum number of characters to compare.
* @retval Returns an integral value indicating the relationship between the strings:
* <0 : str1 less than str2.<br>
* 0  : str1 identical to str1.<br>
* >0 : str1 greater than str2.<br>
*/
int tsk_strncmp(const char * str1, const char * str2, size_t n)
{
	return (str1 && str2) ? ((*str1 != *str2) ? -1 : strncmp(str1, str2, n)) : ((!str1 && !str2) ? 0 : -1);
}

/**@ingroup tsk_string_group
* Duplicate a Null-terminated string.
* @param s1 The string to duplicate.
* @retval The duplicated string. It's up to you to free the returned string.
*/
char* tsk_strdup(const char *s1)
{
	if(s1){
		return strdup(s1);
	}
	return tsk_null;
}

/**	Duplicates the first @a n chars of @a s1.
 * @param s1 The string to duplicate. 
 * @param n The number of characters to copy to the new string. 
 * @retval	null A copy of @a s1. 
**/
char* tsk_strndup(const char *s1, size_t n)
{
	char *ret = tsk_null;

	if(s1 && n){
		size_t len = tsk_strlen(s1);
		size_t nret = (n > len) ? (len) : (n);

		ret = tsk_calloc((nret+1), sizeof(uint8_t));
		memcpy(ret, s1, nret);
	}

	return ret;
}

/**@ingroup tsk_string_group
* Checks if @a str contains @a substring.
* @param str The master string.
* @param size The size of the master string.
* @param substring the substring.
* @retval @a tsk_true if @a str contains at least one occurence of @a substring and @a tsk_false othewise.
*/
tsk_bool_t tsk_strcontains(const char * str, size_t size, const char * substring)
{
	return (tsk_strindexOf(str, size, substring) >= 0);
}

/**@ingroup tsk_string_group
* Gets the first occurrence of @a substring within @a str.
* @param str The master string.
* @param size The size of the master string.
* @param substring The substring that is to be searched for within @a str.
* @retval The index of the first ocurrence of @a substring in @a str.
* If no occurrence of @a substring is found, then -1 is returned.
*/
int tsk_strindexOf(const char * str, size_t size, const char * substring)
{
	if(str && substring){
		const char* sub_start = strstr(str, substring);
		if(sub_start && (sub_start < (str + size))){
			return (sub_start - str);
		}
	}
	return -1;
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
	tsk_strncat(destination, source, tsk_strlen(source));
}

/**@ingroup tsk_string_group
*/
void tsk_strcat_2(char** destination, const char* format, ...)
{
	char* temp = tsk_null;
	int len;
	va_list ap;
	
	/* initialize variable arguments */
	va_start(ap, format);
	/* compute */
	if((len = tsk_sprintf_2(&temp, format, &ap))){
		tsk_strncat(destination, temp, len);
	}
	/* reset variable arguments */
	va_end(ap);
	TSK_FREE(temp);
}

/**@ingroup tsk_string_group
*/
void tsk_strncat(char** destination, const char* source, size_t n)
{
	size_t index = 0;
	size_t size_to_cat = (n > tsk_strlen(source)) ? tsk_strlen(source) : n;

	if(!source) return;

	if(!*destination){
		*destination = (char*)tsk_malloc(size_to_cat+1);
		strncpy(*destination, source, size_to_cat+1);
	}else{
		index = tsk_strlen(*destination);
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
* @param str Pointer to an array of char elements where the resulting C string is stored. 
* MUST be NULL.
* @param format C string that contains the text to be written to the buffer. For more information see definiton of C function @a sprintf
* @retval On success, the total number of characters written is returned. This count does not include the additional null-character automatically appended 
* at the end of the string.
* On failure, a negative number is returned.
*/
int tsk_sprintf(char** str, const char* format, ...)
{
	int len;
	va_list ap;
	
	/* initialize variable arguments */
	va_start(ap, format);
	/* compute */
	len = tsk_sprintf_2(str, format, &ap);
	/* reset variable arguments */
	va_end(ap);
	
	return len;
}

/**@ingroup tsk_string_group
*/
int tsk_sprintf_2(char** str, const char* format, va_list* ap)
{
	int len = 0;

	/* free previous value */
	if(*str){
		tsk_free((void**)str);
	}
	
	/* compute destination len for windows mobile
	*/
#if defined(_WIN32_WCE)
	{
		int n;
		len = (tsk_strlen(format)*2);
		*str = (char*)tsk_calloc(1, len+1);
		for(;;){
			if( (n = vsnprintf(*str, len, format, *ap)) >= 0 && (n<len) ){
				len = n;
				goto done;
			}
			else{
				len += 10;
				*str = tsk_realloc(*str, len+1);
			}
		}
done:
		(*str)[len] = '\0';
	}
#else
    len = vsnprintf(0, 0, format, *ap);
    *str = (char*)tsk_calloc(1, len+1);
    vsnprintf(*str, len
#if !defined(_MSC_VER) || defined(__GNUC__)
		+1
#endif
		, format, *ap);
#endif
	
	return len;
}

/**@ingroup tsk_string_group
* Updates the value of @a str.
* @param str The string to update.
* @param newval The new value of @a str.
*/
void tsk_strupdate(char** str, const char* newval)
{
	tsk_free((void**)str);
	*str = tsk_strdup(newval);
}


/**@ingroup tsk_string_group
* Removes all occurrences of white space characters from the beginning of this @a str.
* @param str The string to trim.
*/
void tsk_strtrim_left(char **str)
{
	if(str && *str){
		size_t count = 0;
		while(isspace(*((*str)+count))) count++;
		if(count){
			strcpy((*str), (*str)+count);
		}
	}
}

/**@ingroup tsk_string_group
* Removes all occurrences of white space characters from the end of @a str.
* @param str The string to trim.
*/
void tsk_strtrim_right(char **str)
{
	if(str && *str)
	{
		size_t size = tsk_strlen(*str);
		if(size){
			while(isspace(*((*str)+size-1))) size--;
			*(*str + size) = '\0';
		}
	}
}
/**@ingroup tsk_string_group
* Removes all occurrences of white space characters from the beginning and end of @a str.
* @param str The string to trim.
*/
void tsk_strtrim(char **str)
{
	// left
	tsk_strtrim_left(str);
	// right
	tsk_strtrim_right(str);
}

/**@ingroup tsk_string_group
* Adds quotes ("") to the beginning and end of @a str.<br>
* @param str The string to quote.
* Example: tsk_strquote("doubango") = ""doubango\"".
*/
void tsk_strquote(char **str)
{
	tsk_strquote_2(str, '"', '"');
}

/**@ingroup tsk_string_group
* Adds quotes to the beginning and end of @a str.
* @param str The string to quote.
* @param lquote Quote to add to the begining of @a str.
* @param rquote Quote to add to the end of @a str.
*/
void tsk_strquote_2(char **str, char lquote, char rquote)
{
	if(str && *str){
		char *result = tsk_null;
		tsk_sprintf(&result, "%c%s%c", lquote, *str, rquote);
		tsk_free((void**)str);
		*str = result;
	}
}

/**@ingroup tsk_string_group
* Removes quotes ("") from the beginning and end of @a str.<br>
* @param str The string to unquote.
* Example: tsk_strunquote(""doubango"") = "doubango".
*/
void tsk_strunquote(char **str)
{
	tsk_strunquote_2(str, '"', '"');
}

/**@ingroup tsk_string_group
* Removes quotes from the beginning and end of @a str. The string must starts with @a lquote
* and end with @a rquote.
* @param str The string to unquote.
* @param lquote Quote to remove from the begining of @a str.
* @param rquote Quote to remove from the end of @a str.
*/
void tsk_strunquote_2(char **str, char lquote, char rquote)
{
	if(str && *str)
	{
		size_t size = tsk_strlen(*str);
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

/**@ingroup tsk_string_group
*/
long tsk_atox(const char* str)
{
	long ret = 0;
	if(str){
		sscanf(str, "%x", &ret);
	}
	return ret;
}

/**@ingroup tsk_string_group
 * Generates a random string.
 *
 * @param result	A pointer to the result. 
**/
void tsk_strrandom(tsk_istr_t *result)
{
	static uint64_t __counter = 1;
	tsk_itoa((tsk_time_epoch() ^ (rand())) ^ ++__counter, result);
}

/**@ingroup tsk_string_group
 *
 * Converts hexadecimal bytes into string representation.
 *
 * @param hex	The hexadecimal bytes to convert. 
 * @param	size		The size of the hexadecimal bytes. 
 * @param str	The pointer to the result. MUST be enought large to hold the result.
 *						It is up to you to add the final '\\0'.
 * @sa @ref tsk_str_to_hex
**/
void tsk_str_from_hex(const uint8_t *hex, size_t size, char* str)
{
	static const char *TSK_HEXA_VALUES = {"0123456789abcdef"};
	size_t i;

	for (i = 0 ; i<size; i++){
		str[2*i] = TSK_HEXA_VALUES [ (*(hex+i) & 0xf0) >> 4 ];
		str[(2*i)+1] = TSK_HEXA_VALUES [ (*(hex+i) & 0x0f)		];
	}
}

/**@ingroup tsk_string_group
 * Converts string chars into hexadecimal bytes.
 *
 * @param str	If non-null, the string. 
 * @param	size		The size. 
 * @param hex	If non-null, the hexadecimal. 
**/
void tsk_str_to_hex(const char *str, size_t size, uint8_t* hex)
{
	TSK_DEBUG_FATAL("Not implemented.");
}










//=================================================================================================
//	String object definition
//
static tsk_object_t* tsk_string_ctor(tsk_object_t * self, va_list * app)
{
	tsk_string_t *string = self;
	const char *value = va_arg(*app, const char *);
	if(value){
		string->value = tsk_strdup(value);
	}
	return self;
}

static tsk_object_t* tsk_string_dtor(tsk_object_t * self)
{ 
	tsk_string_t *string = self;
	if(string){
		TSK_FREE(string->value);
	}

	return self;
}

static int tsk_string_cmp(const tsk_object_t *_s1, const tsk_object_t *_s2)
{
	const tsk_string_t *s1 = _s1;
	const tsk_string_t *s2 = _s2;

	if(s1 && s2){
		return tsk_stricmp(s1->value, s2->value);
	}
	else if(!s1 && !s2) return 0;
	else return -1;
}

static const tsk_object_def_t tsk_string_def_s = 
{
	sizeof(tsk_string_t),
	tsk_string_ctor, 
	tsk_string_dtor,
	tsk_string_cmp, 
};
const tsk_object_def_t *tsk_string_def_t = &tsk_string_def_s;

