/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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

/**@file tsms_tpdu.h
 * @brief SMS TPDU encoder/decoder as per 3GPP TS 23.040.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSMS_TSMS_TPDU_H
#define TINYSMS_TSMS_TPDU_H

#include "tinysms_config.h"

#include "tinySMS/tsms_common.h"

#include "tsk_buffer.h"


TSMS_BEGIN_DECLS

typedef struct tsms_tpdu_ctx_s
{
	TSK_DECLARE_OBJECT;

	uint8_t mr;
	tsms_address_t* smsc; /**< SMSC*/
	tsms_address_t* phone; /**< Remote Phone number. */
	tsms_alphabet_t alphabet; /**< User data. */
	tsk_buffer_t* usrdata; /**< User data. */
}
tsms_tpdu_ctx_t;

typedef void tsms_tpdu_ctx_handle_t;

TINYSMS_API tsms_tpdu_ctx_handle_t* tsms_pdu_ctx_create(uint8_t mr, tsms_address_string_t smsc, tsms_address_string_t phone);
TINYSMS_API int tsms_pdu_ctx_set_alphabet(tsms_tpdu_ctx_handle_t* handle, tsms_alphabet_t alphabet);
TINYSMS_API int tsms_pdu_ctx_set_usrdata(tsms_tpdu_ctx_handle_t* handle, const void* data, size_t size);
TINYSMS_API tsk_buffer_t* tsms_pdu_ctx_getSUBMIT(tsms_tpdu_ctx_handle_t* handle);

TINYSMS_GEXTERN const tsk_object_def_t *tsms_tpdu_ctx_def_t;

TSMS_END_DECLS

#endif /* TINYSMS_TSMS_TPDU_H */
