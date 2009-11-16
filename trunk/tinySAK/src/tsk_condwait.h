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

/**@file tsk_condwait.h
 * @brief Pthread CondWait
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TINYSAK_CONDWAIT_H_
#define _TINYSAK_CONDWAIT_H_

#include "tinySAK_config.h"
#include "tsk_mutex.h"

/**@def TSK_CONDWAIT_CREATE
* Create and initialize a condwait.
* You MUST use @ref TSK_CONDWAIT_SAFE_FREE to free a condwait.
* @param this The @ref tsk_condwait_t object to create.
* @sa @ref TSK_CONDWAIT_SAFE_FREE.
*/
#define TSK_CONDWAIT_CREATE(this)				TSK_XXX_CREATE2(this, condwait)
/**@def TSK_CONDWAIT_SAFE_FREE
* Safely free a condwait previously created using @ref TSK_CONDWAIT_CREATE.
* @param this The @ref tsk_condwait_t object to free.
* @sa @ref TSK_CONDWAIT_CREATE.
*/
#define TSK_CONDWAIT_SAFE_FREE(this)			TSK_XXX_SAFE_FREE2(this, condwait)

/** Pthread CondWait.
*/
typedef struct tsk_condwait_s
{
	void* handle; /**< Handle pointing to the condwait */
	tsk_mutex_t* mutex;  /**< Locker*/
}
tsk_condwait_t;

TINYSAK_API void tsk_condwait_init(tsk_condwait_t* condwait);
TINYSAK_API int tsk_condwait_wait(tsk_condwait_t* condwait);
TINYSAK_API int tsk_condwait_timedwait(tsk_condwait_t* condwait, unsigned int ms);
TINYSAK_API int tsk_condwait_signal(tsk_condwait_t* condwait);
TINYSAK_API int tsk_condwait_broadcast(tsk_condwait_t* condwait);
TINYSAK_API void tsk_condwait_free(tsk_condwait_t** condwait);

#endif /* _TINYSAK_CONDWAIT_H_ */