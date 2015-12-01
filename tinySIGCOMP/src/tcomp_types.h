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

#ifndef TCOMP_TYPES_H
#define TCOMP_TYPES_H

#include "tsk_list.h"

TCOMP_BEGIN_DECLS

typedef tsk_list_t tcomp_buffers_L_t; /**< List containing @ref tcomp_buffer_handle_t elements. */
typedef tsk_list_t tcomp_states_L_t; /**< List containing @ref tcomp_state_t elements. */
typedef tsk_list_t tcomp_dictionaries_L_t; /**< List containing @ref tcomp_dictionary_t elements. */
typedef tsk_list_t tcomp_compartments_L_t; /** List containing @ref tcomp_compartment_t elements. */
typedef tsk_list_t tcomp_stream_buffer_L_t; /** List containing @ref tcomp_stream_buffer_t elements. */

TCOMP_END_DECLS

#endif /* TCOMP_TYPES_H */
