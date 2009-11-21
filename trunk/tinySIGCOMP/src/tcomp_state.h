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

/**@file tcomp_state.h
 * @brief  SIGCOMP state.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TCOMP_STATE_H
#define TCOMP_STATE_H

#include "tinysigcomp_config.h"
#include "tcomp_buffer.h"
#include "tsk_safeobj.h"

typedef struct tcomp_state_s
{
	tcomp_buffer_handle_t *value;		/**< State's value. */
	tcomp_buffer_handle_t *identifier;	/**< State's identifier. */
	
	uint16_t length;					/**< State's length. */
	uint16_t address;					/**< State's address. */
	uint16_t instruction;				/**< State's instruction. */
	uint16_t minimum_access_length;		/**< State's minimum access length. */
	uint16_t retention_priority;		/**< State's retention priority. */

	TSK_SAFEOBJ_DECLARE;
}
tcomp_state_t;


tcomp_state_t* tcomp_state_create(uint16_t length, uint16_t address, uint16_t instruction, uint16_t minimum_access_length, uint16_t retention_priority);

int tcomp_state_equals(const tcomp_state_t *state1, const tcomp_state_t *state2);

void tcomp_state_makeValid(tcomp_state_t*);

void tcomp_state_destroy(tcomp_state_t**);

#endif /* TCOMP_STATE_H */