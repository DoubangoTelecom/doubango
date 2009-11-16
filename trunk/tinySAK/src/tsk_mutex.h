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

/**@file tsk_mutex.h
 * @brief Pthread Mutex.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TINYSAK_MUTEX_H_
#define _TINYSAK_MUTEX_H_

#include "tinySAK_config.h"

/**@def TSK_MUTEX_CREATE
* Create and initialize a mutex.
* You MUST use @ref TSK_MUTEX_SAFE_FREE to free a mutex.
* @param this The @ref tsk_mutex_t object to create.
* @sa @ref TSK_MUTEX_SAFE_FREE.
*/
#define TSK_MUTEX_CREATE(this)				TSK_XXX_CREATE2(this, mutex)
/**@def TSK_MUTEX_SAFE_FREE
* Safely free a mutex previously created using @ref TSK_MUTEX_CREATE.
* @param this The @ref tsk_mutex_t object to free.
* @sa @ref TSK_MUTEX_CREATE.
*/
#define TSK_MUTEX_SAFE_FREE(this)			TSK_XXX_SAFE_FREE2(this, mutex)

/** Pthread Mutex.
*/
typedef struct tsk_mutex_s
{
	void* handle; /**< Handle pointing to the mutex */
}
tsk_mutex_t;

TINYSAK_API void tsk_mutex_init(tsk_mutex_t* mutex);
TINYSAK_API int tsk_mutex_lock(tsk_mutex_t* mutex);
TINYSAK_API int tsk_mutex_unlock(tsk_mutex_t* mutex);
TINYSAK_API void tsk_mutex_free(tsk_mutex_t** mutex);

#endif /* _TINYSAK_MUTEX_H_ */