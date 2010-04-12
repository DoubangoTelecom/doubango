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

/**@file tsms_tpdu_status_report.h
 * @brief SMS TPDU SMS-STATUS-REPORT message as per 3GPP TS 23.040 section 9.2.2.3.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef TINYSMS_TSMS_TPDU_STATUS_REPORT_H
#define TINYSMS_TSMS_TPDU_STATUS_REPORT_H

#include "tinysms_config.h"

#include "tinySMS/tsms_common.h"


TSMS_BEGIN_DECLS

/** SMS TPDU SMS-STATUS-REPORT message as per 3GPP TS 23.040 section 9.2.2.3.
*/
typedef struct tsms_tpdu_status_report_s
{
	TSMS_DECLARE_TPDU_MESSAGE;
	
	/** TP User Data Header Indicator (O - 1b)
	* Parameter indicating that the TP UD field contains a Header. */
	unsigned udhi:1;
	/** TP More Messages to Send (M - 1b)
	* Parameter indicating whether or not there are more messages to send. */
	unsigned mms:1;
	/** TP-Loop-Prevention (O - 1b)
	* Parameter indicating that SMS applications should inhibit forwarding or automatic message generation that could cause infinite looping. */
	unsigned lp:1;
	/** TP Status Report Qualifier (M - 1b)
	* Parameter indicating whether the previously submitted TPDU was an SMS-SUBMIT or an SMS COMMAND. */
	unsigned srq:1;
	/** TP Message Reference (M - I)
	* Parameter identifying the previously submitted SMS SUBMIT or SMS COMMAND. */
	unsigned mr;
	/** TP Recipient Address (M - 2-12o)
	* Address of the recipient of the previously submitted mobile originated short message. */
	tsms_address_t ra;
	/** TP Service Centre Time Stamp (M - 7o)
	* Parameter identifying time when the SC received the previously sent SMS SUBMIT. */
	uint8_t scts[7];
	/** TP Discharge Time (M 7o)
	* Parameter identifying the time associated with a particular TP ST outcome. */
	uint8_t dt[7];
	/** TP Status (M - o)
	* Parameter identifying the status of the previously sent mobile originated short message. */
	uint8_t st;
	/** TP-Parameter-Indicator (O - o)
	* Parameter indicating the presence of any of the optional parameters which follow. */
	uint8_t pi;
	/** TP Protocol Identifier (O - o)
	* See clause 9.2.3.9. TP-PID of original SMS-SUBMIT. */
	uint8_t pid;
	/** TP Data Coding Scheme (O - o)
	* See clause 9.2.3.10. */
	uint8_t dcs;
	/** TP User Data Length (O- o)
	* See clause 9.2.3.16. */
	uint8_t udl;
	/** TP User Data (O - v)
	* User data. See clause 9.2.3.24. */
	uint8_t* ud;
}
tsms_tpdu_status_report_t;


TSMS_END_DECLS


#endif /* TINYSMS_TSMS_TPDU_STATUS_REPORT_H */