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
* GNU General Public License for more details.
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
#include "tsk_object.h"

TCOMP_BEGIN_DECLS

/**@ingroup tcomp_state_group
* Creates new SigComp state.
*/
#define TCOMP_STATE_CREATE(length, address, instruction, minimum_access_length, retention_priority)	\
	tsk_object_new(tcomp_state_def_t, length, address, instruction, minimum_access_length, retention_priority)

/**@ingroup tcomp_state_group
* @def TCOMP_PARTIAL_ID_LEN_CODE
*/
/**@ingroup tcomp_state_group
* @def TCOMP_PARTIAL_ID_LEN_VALUE
*/
#define TCOMP_PARTIAL_ID_LEN_CODE		0x01
#define TCOMP_PARTIAL_ID_LEN_VALUE		0x06

/**@ingroup tcomp_state_group
* For the purpose of calculation, each state item is considered to cost (state_length + 64) bytes.
*/
#define TCOMP_GET_STATE_SIZE(state) ( (state) ? ((state)->length + 64) : 0 )

/**@ingroup tcomp_state_group
* SigComp state.
*/
typedef struct tcomp_state_s
{
	TSK_DECLARE_OBJECT;

	tcomp_buffer_handle_t *value;		/**< State's value. */
	tcomp_buffer_handle_t *identifier;	/**< State's identifier. */
	
	uint16_t length;					/**< State's length. */
	uint16_t address;					/**< State's address. */
	uint16_t instruction;				/**< State's instruction. */
	uint16_t minimum_access_length;		/**< State's minimum access length. */
	uint16_t retention_priority;		/**< State's retention priority. */

	TSK_DECLARE_SAFEOBJ;
}
tcomp_state_t;

typedef tcomp_state_t tcomp_dictionary_t; /**< Ad dictionary is  a @ref tcomp_state_t. */

int tcomp_state_equals(const tcomp_state_t *state1, const tcomp_state_t *state2);
void tcomp_state_makeValid(tcomp_state_t*);

TINYSIGCOMP_GEXTERN const void *tcomp_state_def_t;

TCOMP_END_DECLS

#endif /* TCOMP_STATE_H */
