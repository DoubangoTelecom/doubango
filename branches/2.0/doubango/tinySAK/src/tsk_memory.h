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

/**@file tsk_memory.h
 * @brief Useful memory management functions to handle memory.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TINYSAK_MEMORY_H_
#define _TINYSAK_MEMORY_H_

#include "tinysak_config.h"

#include <stdlib.h> /* tsk_size_t */

/**@ingroup tsk_memory_group
* @def TSK_SAFE_FREE
* Safely free the memory pointed by @a ptr.
*/
/**@ingroup tsk_memory_group
* @def TSK_FREE
* Safely free the memory pointed by @a ptr.
*/

TSK_BEGIN_DECLS

#define TSK_SAFE_FREE(ptr) (void)tsk_free((void**)(&ptr));
#define TSK_FREE(ptr) TSK_SAFE_FREE((ptr))
#define TSK_SAFE_FREE_ARRAY(pptr, count){ \
	int __i; \
	for(__i = 0; __i < (count); ++__i) \
		TSK_SAFE_FREE((pptr)[__i]); \
}
#define TSK_FREE_ARRAY(pptr, count) TSK_SAFE_FREE_ARRAY((pptr), (count))
#define TSK_SAFE_FREE_TABLE(pptr) TSK_SAFE_FREE_ARRAY((pptr), (sizeof((pptr))/sizeof((pptr)[0])))
#define TSK_FREE_TABLE(pptr) TSK_SAFE_FREE_TABLE((pptr))

TINYSAK_API void* tsk_malloc(tsk_size_t size);
TINYSAK_API void* tsk_realloc (void * ptr, tsk_size_t size);
TINYSAK_API void tsk_free(void** ptr);
TINYSAK_API void* tsk_calloc(tsk_size_t num, tsk_size_t size);

TSK_END_DECLS

#endif /* _TINYSAK_MEMORY_H_ */

