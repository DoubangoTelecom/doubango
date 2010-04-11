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

/**@file tsms_tpdu_report.h
 * @brief SMS TPDU SMS-DELIVER-REPORT and SMS-SUBMIT-REPORT message as per 3GPP TS 23.040 section 9.2.2.1a and 9.2.2.2a.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSMS_TSMS_TPDU_REPORT_H
#define TINYSMS_TSMS_TPDU_REPORT_H

#include "tinysms_config.h"

#include "tinySMS/tsms_common.h"


TSMS_BEGIN_DECLS

/** SMS TPDU SMS-DELIVER-REPORT or SMS-SUBMIT-REPORT
* message as per 3GPP TS 23.040 section 9.2.2.1a or 9.2.2.2a.
*/
typedef struct tsms_tpdu_report_s
{
	TSMS_DECLARE_TPDU_MESSAGE;

	/** Indicates whether the Report is for RP-ERROR or not (RP-ACK).*/
	tsk_bool_t error;

	/** TP-User-Data-Header-Indication (O - 1b)
	* Parameter indicating that the TP-UD field contains a Header. */
	unsigned udhi:1;
	/** TP Failure Cause (M - I)
	* Parameter indicating the reason for SMS DELIVER/SUBMIT failure. */
	unsigned fcs;
	/** TP Parameter Indicator (M - o)
	* Parameter indicating the presence of any of the optional parameters which follow. */
	uint8_t pi;
	/** TP Service Centre Time Stamp (M - 7o)
	* Parameter identifying the time when the SC received the SMS DELIVER/SUBMIT.
	* See clause 9.2.3.11. */
	uint8_t scts[7];
	/** TP Protocol Identifier (O - o)
	* See clause 9.2.3.9. */
	uint8_t pid;
	/** TP Data Coding Scheme (O - o)
	* See clause 9.2.3.10. */
	uint8_t dcs;
	/** TP User Data Length (O- o)
	* See clause 9.2.3.16. */
	uint8_t udl;
	/** TP User Data (O - v)
	* User data. */
	uint8_t* ud;
}
tsms_tpdu_report_t;

TSMS_END_DECLS

#endif /* TINYSMS_TSMS_TPDU_REPORT_H */
