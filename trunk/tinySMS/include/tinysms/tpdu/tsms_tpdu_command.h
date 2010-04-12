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

#define TSMS_TPDU_COMMAND_CREATE()				tsk_object_new(tsms_tpdu_command_def_t)

/** SMS TPDU SMS-COMMAND message as per 3GPP TS 23.040 section 9.2.2.4.
*/
typedef struct tsms_tpdu_command_s
{
	TSMS_DECLARE_TPDU_MESSAGE;
	
	/** TP-User-Data-Header-Indication (O - 1b)
	* Parameter indicating that the TP-CD field contains a Header. */
	unsigned udhi:1;
	/** TP Status Report  Request (O - 1b)
	* Parameter indicating if the SMS Command is requesting a status report. */
	unsigned srr:1;
	/** TP Message Reference (M - I)
	* Parameter identifying the SMS COMMAND. */
	unsigned mr;
	/** TP Protocol  Identifier (M - o)
	* Parameter identifying the above layer protocol, if any. */
	uint8_t pid;
	/** TP Command Type (M - o)
	* Parameter specifying which operation is to be performed on a SM. */
	uint8_t ct;
	/** TP Message Number (M - o)
	* Parameter indicating which SM in the SC to operate on. */
	uint8_t mn;
	/** TP Destination Address (M - 2-12o)
	* Parameter indicating the Destination Address to which the TP Command refers. */
	tsms_address_t da;
	/** TP Command Data Length (M - o)
	* Parameter indicating the length of the TP-CD field in octets. */
	uint8_t cdl;
	/** TP Command Data (O - o)
	* Parameter containing user data. */
	uint8_t cd;
}
tsms_tpdu_command_t;

TINYSMS_GEXTERN const tsk_object_def_t *tsms_tpdu_command_def_t;

#endif /* TINYSMS_TSMS_TPDU_COMMAND_H */
