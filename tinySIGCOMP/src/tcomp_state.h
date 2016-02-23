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

/**@file tcomp_state.h
 * @brief  SIGCOMP state.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */
#ifndef TCOMP_STATE_H
#define TCOMP_STATE_H

#include "tinysigcomp_config.h"

#include "tcomp_buffer.h"
#include "tsk_safeobj.h"
#include "tsk_list.h"

TCOMP_BEGIN_DECLS

#define TCOMP_PARTIAL_ID_LEN_CODE		0x01
#define TCOMP_PARTIAL_ID_LEN_VALUE		0x06

/**For the purpose of calculation, each state item is considered to cost (state_length + 64) bytes.
*/
#define TCOMP_GET_STATE_SIZE(state) ( (state) ? ((state)->length + 64) : 0 )

/**SigComp state.
*/
typedef struct tcomp_state_s {
    TSK_DECLARE_OBJECT;

    tcomp_buffer_handle_t *value;		/**< State's value. */
    tcomp_buffer_handle_t *identifier;	/**< State's identifier. */

    uint32_t length;					/**< State's length. */
    uint32_t address;					/**< State's address. */
    uint32_t instruction;				/**< State's instruction. */
    uint32_t minimum_access_length;		/**< State's minimum access length. */
    uint32_t retention_priority;		/**< State's retention priority. */

    int32_t usage_count;				/**< State's usage count (to avoid duplication). */

    TSK_DECLARE_SAFEOBJ;
}
tcomp_state_t;

typedef tcomp_state_t tcomp_dictionary_t; /**< Ad dictionary is  a @ref tcomp_state_t. */

tcomp_state_t* tcomp_state_create(uint32_t length, uint32_t address, uint32_t instruction, uint32_t minimum_access_length, uint32_t retention_priority);

int tcomp_state_equals(const tcomp_state_t *state1, const tcomp_state_t *state2);
void tcomp_state_makeValid(tcomp_state_t*);
int32_t tcomp_state_inc_usage_count(tcomp_state_t*);
int32_t tcomp_state_dec_usage_count(tcomp_state_t*);

TINYSIGCOMP_GEXTERN const tsk_object_def_t *tcomp_state_def_t;

TCOMP_END_DECLS

#endif /* TCOMP_STATE_H */
