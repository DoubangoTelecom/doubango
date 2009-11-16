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

/**@file tsk_semaphore.h
 * @brief Pthread Semaphore.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TINYSAK_SEMAPHORE_H_
#define _TINYSAK_SEMAPHORE_H_

#include "tinySAK_config.h"

/**@def TSK_SEMAPHORE_CREATE
* Create and initialize a semaphore.
* You MUST use @ref TSK_SEMAPHORE_SAFE_FREE to free a semaphore.
* @param this The @ref tsk_semaphore_t object to create.
* @sa @ref TSK_SEMAPHORE_SAFE_FREE.
*/
#define TSK_SEMAPHORE_CREATE(this)				TSK_XXX_CREATE2(this, semaphore)
/**@def TSK_SEMAPHORE_SAFE_FREE
* Safely free a semaphore previously created using @ref TSK_SEMAPHORE_CREATE.
* @param this The @ref tsk_semaphore_t object to free.
* @sa @ref TSK_SEMAPHORE_CREATE.
*/
#define TSK_SEMAPHORE_SAFE_FREE(this)			TSK_XXX_SAFE_FREE2(this, semaphore)

/** Pthread Semaphore.
*/
typedef struct tsk_semaphore_s
{
	void* handle; /**< Handle pointing to the semaphore */
}
tsk_semaphore_t;

TINYSAK_API void tsk_semaphore_init(tsk_semaphore_t* semaphore);
TINYSAK_API int tsk_semaphore_increment(tsk_semaphore_t* semaphore);
TINYSAK_API int tsk_semaphore_decrement(tsk_semaphore_t* semaphore);
TINYSAK_API void tsk_semaphore_free(tsk_semaphore_t** semaphore);

#endif /* _TINYSAK_SEMAPHORE_H_ */