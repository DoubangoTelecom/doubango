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
* GNU General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsk_memory.h
 * @brief Useful memory management functions to handle memory.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TINYSAK_MEMORY_H_
#define _TINYSAK_MEMORY_H_

#include "tinySAK_config.h"

#include <stdlib.h> /* size_t */

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
#define TSK_FREE(ptr) TSK_SAFE_FREE(ptr)

TINYSAK_API void* tsk_malloc(size_t size);
TINYSAK_API void* tsk_realloc (void * ptr, size_t size);
TINYSAK_API void tsk_free(void** ptr);
TINYSAK_API void* tsk_calloc(size_t num, size_t size);

TSK_END_DECLS

#endif /* _TINYSAK_MEMORY_H_ */

