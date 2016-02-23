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

/**@file tsk_mutex.h
 * @brief Pthread Mutex.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#ifndef _TINYSAK_MUTEX_H_
#define _TINYSAK_MUTEX_H_

#include "tinysak_config.h"

TSK_BEGIN_DECLS

/**@ingroup tsk_mutex_group
* Mutex handle.
*/
typedef void tsk_mutex_handle_t;

TINYSAK_API tsk_mutex_handle_t* tsk_mutex_create();
TINYSAK_API tsk_mutex_handle_t* tsk_mutex_create_2(tsk_bool_t recursive);
TINYSAK_API int tsk_mutex_lock(tsk_mutex_handle_t* handle);
TINYSAK_API int tsk_mutex_unlock(tsk_mutex_handle_t* handle);
TINYSAK_API void tsk_mutex_destroy(tsk_mutex_handle_t** handle);

TSK_END_DECLS

#endif /* _TINYSAK_MUTEX_H_ */

