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

/**@file tsms_rpdu.h
 * @brief SMS RPDU encoder/decoder as per 3GPP TS 23.011.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSMS_TSMS_RPDU_H
#define TINYSMS_TSMS_RPDU_H

#include "tinysms_config.h"

#include "tinySMS/tsms_common.h"

TSMS_BEGIN_DECLS

/** RP-DATA as per 3GPP TS 24.011 section 7.3.1. */
typedef struct tsms_rpdu_data_s
{
	TSMS_DECLARE_RPDU_MESSAGE;

	// section 8.2.5.1
	tsms_address_t *oa;
	//section 8.2.5.2
	tsms_address_t *da;
	// section 8.2.5.3
	tsk_buffer_t* udata;
}
tsms_rpdu_data_t;

#define TSMS_RPDU_DATA(self) ((tsms_rpdu_data_t*)(self))

TINYSMS_API tsms_rpdu_data_t* tsms_rpdu_data_create(uint8_t mr, const tsms_address_string_t smsc, const tsms_tpdu_message_t* tpdu, tsk_bool_t MobOrig);
#define tsms_rpdu_data_create_mo(mr, smsc, tpdu) tsms_rpdu_data_create(mr, smsc, tpdu, tsk_true)
#define tsms_rpdu_data_create_mt(mr, smsc, tpdu) tsms_rpdu_data_create(mr, smsc, tpdu, tsk_false)
#define tsms_rpdu_data_serialize(self, output) tsms_rpdu_message_serialize(TSMS_RPDU_MESSAGE(self), output)

// set tpdu

TINYSMS_GEXTERN const tsk_object_def_t *tsms_rpdu_data_def_t;

/** RP-SMMA as per 3GPP TS 24.011 section 7.3.2. */
typedef struct tsms_rpdu_smma_s
{
	TSMS_DECLARE_RPDU_MESSAGE;
}
tsms_rpdu_smma_t;

#define TSMS_RPDU_SMMA(self) ((tsms_rpdu_smma_t*)(self))

TINYSMS_API tsms_rpdu_smma_t* tsms_rpdu_smma_create(uint8_t mr);
#define tsms_rpdu_smma_serialize(self, output) tsms_rpdu_message_serialize(TSMS_RPDU_MESSAGE(self), output)

TINYSMS_GEXTERN const tsk_object_def_t *tsms_rpdu_smma_def_t;

/** RP-ACK as per 3GPP TS 24.011 section 7.3.3. */
typedef struct tsms_rpdu_ack_s
{
	TSMS_DECLARE_RPDU_MESSAGE;

	// section 8.2.5.3
	tsk_buffer_t* udata;
}
tsms_rpdu_ack_t;

#define TSMS_RPDU_ACK(self) ((tsms_rpdu_ack_t*)(self))

TINYSMS_API tsms_rpdu_ack_t* tsms_rpdu_ack_create(uint8_t mr, const tsms_tpdu_message_t* tpdu, tsk_bool_t MobOrig);
#define tsms_rpdu_ack_create_mo(mr, tpdu) tsms_rpdu_ack_create(mr, tpdu, tsk_true)
#define tsms_rpdu_ack_create_mt(mr, tpdu) tsms_rpdu_ack_create(mr, tpdu, tsk_false)
#define tsms_rpdu_ack_serialize(self, output) tsms_rpdu_message_serialize(TSMS_RPDU_MESSAGE(self), output)

TINYSMS_GEXTERN const tsk_object_def_t *tsms_rpdu_ack_def_t;

/** RP-ERROR as per 3GPP TS 24.011 section 7.3.4. */
typedef struct tsms_rpdu_error_s
{
	TSMS_DECLARE_RPDU_MESSAGE;
	
	// section 8.2.5.4
	uint8_t cause [3]; //2-3o
	// section 8.2.5.3
	tsk_buffer_t* udata;
}
tsms_rpdu_error_t;

#define TSMS_RPDU_ERROR(self) ((tsms_rpdu_error_t*)(self))

TINYSMS_API tsms_rpdu_error_t* tsms_rpdu_error_create(uint8_t mr, const tsms_tpdu_message_t* tpdu, uint8_t cause, tsk_bool_t MobOrig);
#define tsms_rpdu_error_create_mo(mr, tpdu, cause) tsms_rpdu_error_create(mr, tpdu, cause, tsk_true)
#define tsms_rpdu_error_create_mt(mr, tpdu, cause) tsms_rpdu_error_create(mr, tpdu, cause, tsk_false)
#define tsms_rpdu_error_serialize(self, output) tsms_rpdu_message_serialize(TSMS_RPDU_MESSAGE(self), output)

TINYSMS_GEXTERN const tsk_object_def_t *tsms_rpdu_error_def_t;


TSMS_END_DECLS

#endif /* TINYSMS_TSMS_RPDU_H */
