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

/**@file tsms_tpdu_command.h
 * @brief SMS TPDU SMS-COMMAND message as per 3GPP TS 23.040 section 9.2.2.4.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSMS_TSMS_TPDU_COMMAND_H
#define TINYSMS_TSMS_TPDU_COMMAND_H

#include "tinysms_config.h"

#include "tinySMS/tsms_common.h"

TSMS_BEGIN_DECLS

/** TP-Command-Type (TP-CT) as per 3GPP TS 23.040 section 9.2.3.19. 
* An 8-bit field.
*/
typedef enum tsms_tpdu_cmd_e
{
	tsms_tpdu_cmd_rel = 0x00, /**< Enquiry relating to previously submitted short message. */
	tsms_tpdu_cmd_cancel = 0x01, /**< Cancel Status Report Request relating to previously submitted short message. */
	tsms_tpdu_cmd_delete = 0x02, /**< Delete previously submitted Short Message. */
	tsms_tpdu_cmd_enable_status = 0x03, /**< Enable Status Report Request relating to previously submitted short message. */
	/* 00000100..00011111 ==> Reserved */
	/* 11100000..11111111 ==> Values specific for each SC */
}
tsms_tpdu_cmd_t;


/** SMS TPDU SMS-COMMAND message as per 3GPP TS 23.040 section 9.2.2.4.
*/
typedef struct tsms_tpdu_command_s
{
	TSMS_DECLARE_TPDU_MESSAGE;
	
	tsms_address_t* smsc;

	/** TP-User-Data-Header-Indication (O - 1b)
	* Parameter indicating that the TP-CD field contains a Header. */
	unsigned udhi:1;
	/** TP Status Report  Request (O - 1b)
	* Parameter indicating if the SMS Command is requesting a status report. */
	unsigned srr:1;
	/** TP Message Reference (M - I)
	* Parameter identifying the SMS COMMAND. */
	uint8_t mr;
	/** TP Protocol  Identifier (M - o)
	* Parameter identifying the above layer protocol, if any. */
	//(base)uint8_t pid;
	/** TP Command Type (M - o)
	* Parameter specifying which operation is to be performed on a SM. 
	* See section 9.2.3.19 */
	tsms_tpdu_cmd_t ct;
	/** TP Message Number (M - o)
	* Parameter indicating which SM in the SC to operate on. 
	* See section 9.2.3.18 */
	uint8_t mn;
	/** TP Destination Address (M - 2-12o)
	* Parameter indicating the Destination Address to which the TP Command refers. */
	tsms_address_t* da;
	/** TP Command Data Length (M - o)
	* Parameter indicating the length of the TP-CD field in octets. */
	//(base)uint8_t cdl;
	/** TP Command Data (O - o)
	* Parameter containing user data. */
	//(base)uint8_t cd;
}
tsms_tpdu_command_t;

typedef void tsms_tpdu_command_handle_t;

TINYSMS_API tsms_tpdu_command_handle_t* tsms_tpdu_command_create(uint8_t mr, tsms_address_string_t smsc, tsms_address_string_t dest, uint8_t msg_num, tsms_tpdu_cmd_t cmd);

#define tsms_tpdu_command_serialize(self, output) tsms_tpdu_message_serialize(TSMS_TPDU_MESSAGE(self), output, tsk_true)
#define tsms_tpdu_command_tostring(self) tsms_tpdu_message_tostring(TSMS_TPDU_MESSAGE(self), tsk_true)
#define tsms_tpdu_command_tohexastring(self) tsms_tpdu_message_tohexastring(TSMS_TPDU_MESSAGE(self), tsk_true)
#define tsms_tpdu_command_set_cmddata(self, cdata) tsms_tpdu_message_set_userdata(TSMS_TPDU_MESSAGE(self), cdata, tsms_alpha_8bit)

TINYSMS_GEXTERN const tsk_object_def_t *tsms_tpdu_command_def_t;

#endif /* TINYSMS_TSMS_TPDU_COMMAND_H */
