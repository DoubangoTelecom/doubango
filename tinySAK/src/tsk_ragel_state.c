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

/**@file tsk_ragel_state.c
 * @brief Ragel state for SIP, HTTP and MSRP parsing.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tsk_ragel_state.h"

/**@defgroup tsk_ragel_state_group Ragel state for SIP, HTTP and MSRP parsing.
*/

/**@ingroup tsk_ragel_state_group
* Initialize/Reset the ragel state with default values.
* @param state The ragel @a state to initialize.
* @param data The @a data to parse.
* @param size The @a size of the data.
*/
void tsk_ragel_state_init(tsk_ragel_state_t *state, const char *data, tsk_size_t size)
{
	state->cs = 0;
	state->tag_start = state->p = data;
	state->eoh = state->eof = state->tag_end = state->pe = state->p + size;
}

