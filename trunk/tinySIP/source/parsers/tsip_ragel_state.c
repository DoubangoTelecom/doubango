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

/**@file tsip_ragel_state.c.
 * @brief Ragel state for sip parsing.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinysip/parsers/tsip_ragel_state.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn	void tsip_ragel_state_init(tsip_ragel_state_t *state, const char *data, size_t size)
///
/// @brief	Initialize/Reset the ragel state with default values. 
///
/// @author	Mamadou
/// @date	12/4/2009
///
/// @param [in,out]	state	The ragel state to initialize. 
/// @param [in,out]	data	The data to parse. 
/// @param	size			The size of the data. 
///
////////////////////////////////////////////////////////////////////////////////////////////////////
void tsip_ragel_state_init(tsip_ragel_state_t *state, const char *data, size_t size)
{
	state->cs = 0;
	state->p = data;
	state->pe = state->p + size;
	state->eof = 0;
	
	state->tag_start = 0;
	state->tag_end = 0;
}
