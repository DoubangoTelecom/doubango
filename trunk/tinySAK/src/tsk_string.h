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

/**@file tsk_string.h
 * @brief Useful string functions to manipulate strings.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TINYSAK_STRING_H_
#define _TINYSAK_STRING_H_

#include "tinySAK_config.h"
#include "tsk_heap.h"
#include "tsk_object.h"
#include "tsk_list.h"

#define TSK_STRING_CREATE(str)				tsk_object_new(tsk_string_def_t, str)
#define TSK_STRING_SAFE_FREE(self)			tsk_object_unref(self), self = 0
#define TSK_STRING_STR(self)				((tsk_string_t*)self)->value

typedef char tsk_istr_t[21]; /**< Integer number as string value. */

TINYSAK_API char tsk_b10tob16(char c);
TINYSAK_API char tsk_b16tob10(char c);

TINYSAK_API int tsk_stricmp(const char * str1, const char * str2);
TINYSAK_API int tsk_strnicmp(const char * str1, const char * str2, size_t n);
TINYSAK_API int tsk_strcmp(const char * str1, const char * str2);
TINYSAK_API int tsk_strncmp(const char * str1, const char * str2, size_t n);
TINYSAK_API char* tsk_strdup(const char *s1);
TINYSAK_API void tsk_strcat(char** destination, const char* source);
TINYSAK_API int tsk_sprintf(char** str, const char* format, ...);
TINYSAK_API void tsk_strupdate(char** str, const char* newval);
TINYSAK_API void tsk_strtrim_left(char **str);
TINYSAK_API void tsk_strtrim_right(char **str);
TINYSAK_API void tsk_strquote(char **str);
TINYSAK_API void tsk_strunquote(char **str);
TINYSAK_API void tsk_itoa(int64_t i, tsk_istr_t *result);
TINYSAK_API void tsk_strrandom(tsk_istr_t *result);
TINYSAK_API void tsk_str_from_hex(const uint8_t *hex, size_t size, char* str);
TINYSAK_API void tsk_str_to_hex(const char *str, size_t size, uint8_t* hex);

#define tsk_strtrim_both(str) tsk_strtrim_left(str), tsk_strtrim_right(str);
#define tsk_strempty(s) (*s == '\0')
#define tsk_striequals(s1, s2) (tsk_stricmp((const char*)(s1), (const char*)(s2)) ? 0 : 1)
#define tsk_strniequals(s1, s2, n) (tsk_strnicmp((const char*)(s1), (const char*)(s2), n) ? 0 : 1)
#define tsk_strequals(s1, s2) (tsk_strcmp((const char*)(s1), (const char*)(s2)) ? 0 : 1)
#define tsk_strnequals(s1, s2, n) (tsk_strncmp((const char*)(s1), (const char*)(s2), n) ? 0 : 1)

typedef struct tsk_string_s
{
	TSK_DECLARE_OBJECT;

	char *value;
}
tsk_string_t;

typedef tsk_list_t tsk_strings_L_t;

TINYSAK_API const void *tsk_string_def_t;

#endif /* _TINYSAK_STRING_H_ */
