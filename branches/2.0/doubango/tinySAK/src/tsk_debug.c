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

/**@file tsk_debug.c
 * @brief Utility functions for debugging purpose.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tsk_debug.h"

/**@defgroup tsk_debug_group Utility functions for debugging purpose.
*/

#if TSK_HAVE_DEBUG_H
// Nothing to do --> all is up to the end-user application
#else

static const void* tsk_debug_arg_data = tsk_null;
static tsk_debug_f tsk_debug_info_cb = tsk_null;
static tsk_debug_f tsk_debug_warn_cb = tsk_null;
static tsk_debug_f tsk_debug_error_cb = tsk_null;
static tsk_debug_f tsk_debug_fatal_cb = tsk_null;
static tsk_bool_t tsk_debug_enabled = tsk_true;

void tsk_debug_set_arg_data(const void* arg_data){
	tsk_debug_arg_data = arg_data;
}
const void* tsk_debug_get_arg_data(){
	return tsk_debug_arg_data;
}
void tsk_debug_set_info_cb(tsk_debug_f cb){
	tsk_debug_info_cb = cb;
}
tsk_debug_f tsk_debug_get_info_cb(){
	return tsk_debug_info_cb;
}
void tsk_debug_set_warn_cb(tsk_debug_f cb){
	tsk_debug_warn_cb = cb;
}
tsk_debug_f tsk_debug_get_warn_cb(){
	return tsk_debug_warn_cb;
}
void tsk_debug_set_error_cb(tsk_debug_f cb){
	tsk_debug_error_cb = cb;
}
tsk_debug_f tsk_debug_get_error_cb(){
	return tsk_debug_error_cb;
}
void tsk_debug_set_fatal_cb(tsk_debug_f cb){
	tsk_debug_fatal_cb = cb;
}
tsk_debug_f tsk_debug_get_fatal_cb(){
	return tsk_debug_fatal_cb;
}
void tsk_debug_set_enabled(tsk_bool_t enabled){
	tsk_debug_enabled = enabled;
}
tsk_bool_t tsk_debug_get_enabled(){
	return tsk_debug_enabled;
}

#endif /* TSK_HAVE_DEBUG_H */
