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

/**@file tsms_common.h
 * @brief SMS Commons (3GPP TS 23.038 and al.).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSMS_TSMS_COMMON_H
#define TINYSMS_TSMS_COMMON_H

#include "tinysms_config.h"

#include "tsk_object.h"

TSMS_BEGIN_DECLS

/** SMS alphabet values as per 3GPP TS 23.038 v911 section 4. 
* Part of TP-DCS (SMS Data Coding Scheme).
*/
typedef enum tsms_alphabet_e
{

}
tsms_alphabet_t;

/** 3GPP TS 23.040 - 9.2.3.1	TP Message Type Indicator (TP MTI)
*/
typedef enum tsms_tpdu_mti_e
{
	/*0 0*/ tsms_tpdu_mti_deliver_mt		= 0x00,	/**< SMS-DELIVER (in the direction SC to MS)*/
	/*0 0*/ tsms_tpdu_mti_deliver_report_mo	= 0x00,	/**< SMS-DELIVER-REPORT (in the direction MS to SC)*/
	/*1 0*/ tsms_tpdu_mti_status_report_mt	= 0x02,	/**< SMS-STATUS-REPORT (in the direction SC to MS)*/
	/*1 0*/ tsms_tpdu_mti_command_mo		= 0x02,	/**< SMS-COMMAND (in the direction MS to SC)*/
	/*0 1*/ tsms_tpdu_mti_submit_mo			= 0x01,	/**< SMS-SUBMIT (in the direction MS to SC)*/
	/*0 1*/ tsms_tpdu_mti_submit_report_mt	= 0x01,	/**< SMS-SUBMIT-REPORT (in the direction SC to MS)*/
	/*1 1*/ tsms_tpdu_mti_reserved			= 0x03	/**< Reserved*/
}
tsms_tpdu_mti_t;

/** SM-TL base type as per 3GPP TS 23.040 section 9.2.
*/
typedef struct tsms_tpdu_message_s
{
	TSK_DECLARE_OBJECT;
	tsms_tpdu_mti_t mti; /**< TP Message Type Indicator (TP MTI) as per TS 23.040 section 9.2.3.1. 2-bit field. */
}
tsms_tpdu_message_t;

#define TSMS_DECLARE_TPDU_MESSAGE tsms_tpdu_message_t tpdu

TSMS_END_DECLS

#endif /* TINYSMS_TSMS_COMMON_H */
