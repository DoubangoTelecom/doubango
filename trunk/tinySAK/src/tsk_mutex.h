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

TSK_BEGIN_DECLS

/**
* Pthread Mutex handle.
*/
typedef void tsk_mutex_handle_t;

TINYSAK_API tsk_mutex_handle_t* tsk_mutex_create();
TINYSAK_API int tsk_mutex_lock(tsk_mutex_handle_t* handle);
TINYSAK_API int tsk_mutex_unlock(tsk_mutex_handle_t* handle);
TINYSAK_API void tsk_mutex_destroy(tsk_mutex_handle_t** handle);

TSK_END_DECLS

#endif /* _TINYSAK_MUTEX_H_ */
