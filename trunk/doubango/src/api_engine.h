/**
* @file
* @author  xxxyyyzzz <imsframework(at)gmail.com>
* @version 1.0
*
* @section LICENSE
*
*	
* This file is part of Open Source Doubango IMS Client Framework project.
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
* @section DESCRIPTION
*
*
*/
#ifndef _DOUBANGO_API_ENGINE_H_
#define _DOUBANGO_API_ENGINE_H_

#include "pref.h"
#include "api_errors.h"

PREF_NAMESPACE_START

DOUBANGO_API_C ERR engine_initialize(void);
DOUBANGO_API_C ERR engine_deinitialize(void);

DOUBANGO_API_C ERR engine_stack_create(int stack_id);
DOUBANGO_API_C ERR engine_stack_run(int stack_id);
DOUBANGO_API_C ERR engine_stack_shutdown(int stack_id);
DOUBANGO_API_C ERR engine_stack_shutdown_all(void);

void* engine_stack_find(int stack_id);
ERR engine_push_stack(void* stack_ptr);

PREF_NAMESPACE_END

#endif /* _DOUBANGO_API_ENGINE_H_ */