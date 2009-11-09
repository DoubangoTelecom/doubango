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
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tsk_string.h"

#include <stdarg.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_MSC_VER) || defined(WIN32)
#	define snprintf		_snprintf
#	define vsnprintf	_vsnprintf
#	define strdup		_strdup
#	define stricmp		_stricmp
#endif

#define HEAP_PUSH(heap, address) if(heap) tsk_heap_push(heap, (void*)address)
#define HEAP_POP(heap, address) if(heap) tsk_heap_pop(heap, (void*)address); else free(address)

/**@defgroup tsk_string_group String utils
*/


/**@page tsk_string_page String utils Tutorial
*/

/**@ingroup tsk_string_group
* Free allocated memory
* @param heap The memory heap from which to free @a ptr. Set to NULL if @a ptr had not been allocated using heap allocation mechanism.
* @param ptr POinter to the memory to free
*/
void tsk_free(tsk_heap_t *heap, void** ptr)
{
	if(ptr)
	{
		HEAP_POP(heap, *ptr);
		*ptr = 0;
	}
}

/**@ingroup tsk_string_group
* Duplicate a string
* @param heap The memory heap on which to allocate the duplicated string. Set to NULL if
* you don't want to use heap allocation mechanism.
* @param s1 The string to duplicate
* @retval The duplicated string. The returned pointer can be passed to free() if you are not using memory heap mechanism.
* You MUST not directly free the returned pointer if you are using heap mechanism. */
char* tsk_strdup(tsk_heap_t *heap, const char *s1)
{
	char* ret = strdup(s1);
	HEAP_PUSH(heap, ret);

	return ret;
}