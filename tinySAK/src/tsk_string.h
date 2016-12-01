/*
* Copyright (C) 2010-2015 Mamadou DIOP.
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

/**@file tsk_string.h
 * @brief Useful string functions to manipulate strings.
 */
#ifndef _TINYSAK_STRING_H_
#define _TINYSAK_STRING_H_

#include "tinysak_config.h"

#include "tsk_object.h"
#include "tsk_list.h"

/**@ingroup tsk_string_group
* @def TSK_STRING_STR
* Gets the internal string pointer.
*/
TSK_BEGIN_DECLS

#define TSK_STRING_STR(self)				((self) ? ((tsk_string_t*)(self))->value : tsk_null)

typedef char tsk_istr_t[21]; /**< Integer number as string value. */

TINYSAK_API char tsk_b10tob16(char c);
TINYSAK_API char tsk_b16tob10(char c);

TINYSAK_API int tsk_stricmp(const char * str1, const char * str2);
TINYSAK_API int tsk_strnicmp(const char * str1, const char * str2, tsk_size_t n);
TINYSAK_API int tsk_strcmp(const char * str1, const char * str2);
TINYSAK_API int tsk_strncmp(const char * str1, const char * str2, tsk_size_t n);
TINYSAK_API char* tsk_strdup(const char *s1);
TINYSAK_API char* tsk_strndup(const char *s1, tsk_size_t n);
TINYSAK_API tsk_bool_t tsk_strcontains(const char * str, tsk_size_t size, const char * substring);
TINYSAK_API int tsk_strindexOf(const char * str, tsk_size_t size, const char * substring);
TINYSAK_API int tsk_strLastIndexOf(const char * str, tsk_size_t size, const char * substring);
TINYSAK_API void tsk_strcat(char** destination, const char* source);
TINYSAK_API void tsk_strcat_2(char** destination, const char* format, ...);
TINYSAK_API void tsk_strncat(char** destination, const char* source, tsk_size_t n);
TINYSAK_API int tsk_sprintf(char** str, const char* format, ...);
TINYSAK_API int tsk_sprintf_2(char** str, const char* format, va_list* ap);
TINYSAK_API void tsk_strupdate(char** str, const char* newval);
TINYSAK_API void tsk_strtrim_left(char **str);
TINYSAK_API void tsk_strtrim_right(char **str);
TINYSAK_API void tsk_strtrim(char **str);
TINYSAK_API void tsk_strquote(char **str);
TINYSAK_API void tsk_strquote_2(char **str, char lquote, char rquote);
TINYSAK_API void tsk_strunquote(char **str);
TINYSAK_API void tsk_strunquote_2(char **str, char lquote, char rquote);
TINYSAK_API void tsk_itoa(int64_t i, tsk_istr_t *result);
TINYSAK_API int64_t tsk_atoll(const char*);
TINYSAK_API long tsk_atox(const char*);
TINYSAK_API void tsk_strrandom(tsk_istr_t *result);
TINYSAK_API void tsk_str_from_hex(const uint8_t *hex, tsk_size_t size, char* str);
TINYSAK_API void tsk_str_to_hex(const char *str, tsk_size_t size, uint8_t* hex);

/**@ingroup tsk_string_group
* @def tsk_strtrim_both
*/
/**@ingroup tsk_string_group
* @def tsk_strempty
*/
/**@ingroup tsk_string_group
* @def tsk_striequals
*/
/**@ingroup tsk_string_group
* @def tsk_strniequals
*/
/**@ingroup tsk_string_group
* @def tsk_strequals
*/
/**@ingroup tsk_string_group
* @def tsk_strnequals
*/
#define tsk_strtrim_both(str) tsk_strtrim_left(str), tsk_strtrim_right(str);
#define tsk_strempty(s) (*(s) == '\0')
#define tsk_strnullORempty(s) (!(s) || tsk_strempty((s)))
#define tsk_striequals(s1, s2) (tsk_stricmp((const char*)(s1), (const char*)(s2)) ? tsk_false : tsk_true)
#define tsk_strniequals(s1, s2, n) (tsk_strnicmp((const char*)(s1), (const char*)(s2), n) ? tsk_false : tsk_true)
#define tsk_strequals(s1, s2) (tsk_strcmp((const char*)(s1), (const char*)(s2)) ? tsk_false : tsk_true)
#define tsk_strnequals(s1, s2, n) (tsk_strncmp((const char*)(s1), (const char*)(s2), n) ? tsk_false : tsk_true)
#define tsk_strlen(s) ((s) ? strlen((s)) : 0)
#if defined(_MSC_VER) || HAVE_STRTOK_S
#	define tsk_strtok_r(str, delim, saveptr) strtok_s((str), (delim), (saveptr))
#elif HAVE_STRTOK_R || TSK_UNDER_APPLE || TSK_UNDER_ANDROID
#	define tsk_strtok_r strtok_r
#else
#	define tsk_strtok_r(str, delim, saveptr) strtok(str, delim)
#endif
#if defined(_MSC_VER)
#	define tsk_atoi64	_atoi64
#else
#	define tsk_atoi64	atoll
#endif


/**@ingroup tsk_string_group
* String object.
*/
typedef struct tsk_string_s {
    TSK_DECLARE_OBJECT;

    char *value; /**< The value of the string object. */
}
tsk_string_t;

typedef tsk_list_t tsk_strings_L_t;

TINYSAK_API tsk_string_t* tsk_string_create(const char* str);
TINYSAK_API int tsk_string_pred_icmp(const struct tsk_list_item_s* item, const void* str);
TINYSAK_API int tsk_string_pred_cmp(const struct tsk_list_item_s* item, const void* str);

TINYSAK_GEXTERN const tsk_object_def_t *tsk_string_def_t;

TSK_END_DECLS

#endif /* _TINYSAK_STRING_H_ */

