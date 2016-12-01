/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou [at) doubango (DOT) org>
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
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#ifndef TINYSMS_TSMS_TPDU_STATUS_REPORT_H
#define TINYSMS_TSMS_TPDU_STATUS_REPORT_H

#include "tinysms_config.h"

#include "tinysms/tsms_common.h"


TSMS_BEGIN_DECLS

/** 3GPP TS 23.040 v910 section 9.2.3.15 (TP-ST).
* Used in SMS-STATUS-REPORT message.
*/
typedef enum tsms_tpdu_status_type_e {
    /*== Short message transaction completed ==*/
    tsms_tpdu_status_received = 0x00, /**< Short message received by the SME. */
    tsms_tpdu_status_forwarded = 0x01, /**< Short message forwarded by the SC to the SME but the SC is unable to confirm delivery. */
    tsms_tpdu_status_replaced = 0x02, /**< Short message replaced by the SC. */
    /* 0000011..0001111		Reserved */
    /* 0010000..0011111		Values specific to each SC */

    /*== Temporary error, SC still trying to transfer SM ==*/
    tsms_tpdu_status_congestion = 0x20, /**< Congestion. */
    tsms_tpdu_status_busy = 0x21, /**< ME busy. */
    tsms_tpdu_status_no_response = 0x22, /**< No response from SME. */
    tsms_tpdu_status_serv_rejected = 0x23, /**< Service rejected. */
    tsms_tpdu_status_no_qos = 0x24, /**< Quality of service not available. */
    tsms_tpdu_status_error_in_sme= 0x25, /**< Error in SME. */
    /* 0100110..0101111		Reserved */
    /* 0110000..0111111		Values specific to each SC */

    /*== Permanent error, SC is not making any more transfer attempts ==*/
    tsms_tpdu_status_remote_error = 0x40, /**< Remote procedure error. */
    tsms_tpdu_status_incompatible_dest = 0x41, /**< Incompatible destination. */
    tsms_tpdu_status_conn_rejected = 0x42, /**< Connection rejected by SME. */
    tsms_tpdu_status_not_obtainable = 0x43, /**< Not obtainable. */
    tsms_tpdu_status_no_qos_2 = 0x44, /**< Quality of service not available. */
    tsms_tpdu_status_no_inter_avail = 0x45, /**< No interworking available. */
    tsms_tpdu_status_vp_expired = 0x46, /**< SM Validity Period Expired. */
    tsms_tpdu_status_deleted_by_orig = 0x47, /**< SM Deleted by originating SME. */
    tsms_tpdu_status_deleted_by_admin = 0x48, /**< SM Deleted by SC Administration. */
    tsms_tpdu_status_sm_not_exist = 0x49, /**< SM does not exist (The SM may have previously existed in the SC but the SC 	no longer has knowledge of it or the SM may never have previously existed in the SC). */
    /* 1001010..1001111		Reserved */
    /* 1010000..1011111		Values specific to each SC */

    /*== Temporary error, SC is not making any more transfer attempts ==*/
    tsms_tpdu_status_congestion_3 = 0x50, /**< Congestion. */
    tsms_tpdu_status_busy_3 = 0x51, /**< SME busy. */
    tsms_tpdu_status_no_response_3 = 0x52, /**< No response from SME. */
    tsms_tpdu_status_serv_rejected_3 = 0x53, /**< Service rejected. */
    tsms_tpdu_status_temp_no_qos_3 = 0x54, /**< Quality of service not available. */
    tsms_tpdu_status_error_in_sme_3 = 0x55, /**< Error in SME. */
    /* 1100110..1101001		Reserved */
    /* 1101010..1101111		Reserved */
    /* 1110000..1111111		Values specific to each SC */

}
tsms_tpdu_status_type_t;

/** SMS TPDU SMS-STATUS-REPORT message as per 3GPP TS 23.040 section 9.2.2.3.
*/
typedef struct tsms_tpdu_status_report_s {
    TSMS_DECLARE_TPDU_MESSAGE;

    tsms_address_t* smsc;

    /** TP User Data Header Indicator (O - 1b)
    * Parameter indicating that the TP UD field contains a Header. */
    unsigned udhi:1;
    /** TP More Messages to Send (M - 1b)
    * Parameter indicating whether or not there are more messages to send. */
    unsigned mms:1;
    /** TP-Loop-Prevention (O - 1b)
    * Parameter indicating that SMS applications should inhibit forwarding or automatic message generation that could cause infinite looping.
    Section 9.2.3.28 */
    unsigned lp:1;
    /** TP Status Report Qualifier (M - 1b)
    * Parameter indicating whether the previously submitted TPDU was an SMS-SUBMIT or an SMS COMMAND. */
    unsigned srq:1;
    /** TP Message Reference (M - I)
    * Parameter identifying the previously submitted SMS SUBMIT or SMS COMMAND.
    * See section 9.2.3.26. */
    unsigned mr;
    /** TP Recipient Address (M - 2-12o)
    * Address of the recipient of the previously submitted mobile originated short message. */
    tsms_address_t* ra;
    /** TP Service Centre Time Stamp (M - 7o)
    * Parameter identifying time when the SC received the previously sent SMS SUBMIT. */
    uint8_t scts[7];
    /** TP Discharge Time (M 7o)
    * Parameter identifying the time associated with a particular TP ST outcome. */
    uint8_t dt[7];
    /** TP Status (M - o)
    * Parameter identifying the status of the previously sent mobile originated short message. */
    tsms_tpdu_status_type_t st;
    /** TP-Parameter-Indicator (O - o)
    * Parameter indicating the presence of any of the optional parameters which follow. */
    uint8_t pi;
    /** TP Protocol Identifier (O - o)
    * See clause 9.2.3.9. TP-PID of original SMS-SUBMIT. */
    //(base)uint8_t pid;
    /** TP Data Coding Scheme (O - o)
    * See clause 9.2.3.10. */
    //(base)uint8_t dcs;
    /** TP User Data Length (O- o)
    * See clause 9.2.3.16. */
    //(base)uint8_t udl;
    /** TP User Data (O - v)
    * User data. See clause 9.2.3.24. */
    //(base)uint8_t* ud;
}
tsms_tpdu_status_report_t;

/** cast any pointer to @ref tsms_tpdu_status_report_t* */
#define TSMS_TPDU_STATUS_REPORT(self) ((tsms_tpdu_status_report_t*)(self))

TINYSMS_API tsms_tpdu_status_report_t* tsms_tpdu_status_report_create(uint8_t mr, const tsms_address_string_t smsc, const tsms_address_string_t recipient, tsms_tpdu_status_type_t status, tsk_bool_t submit);

/**@ingroup tsms_tpdu_group
* @def tsms_tpdu_status_report_serialize
* Serializes a @a SMS-STATUS-REPORT message as binary content.
* @param self A pointer to the @a SMS-STATUS-REPORT (@ref tsms_tpdu_status_report_t) message to serialize.
* @param output A pointer to the output buffer. Should be valid.
* @retval Zero if succeed and non-zero error code otherwise.
*
* See For more information, see @ref tsms_tpdu_group_STATUS_REPORT  "SMS-STATUS-REPORT".
*/
/**@ingroup tsms_tpdu_group
* @def tsms_tpdu_status_report_tohexastring
* Serializes a @a SMS-STATUS-REPORT message as hexa-string content.
* @param self A pointer to the @a SMS-STATUS-REPORT (@ref tsms_tpdu_status_report_t) message to serialize.
* @retval A pointer to the hexa-string if succeed and Null otherwise.
*
* See For more information, see @ref tsms_tpdu_group_STATUS_REPORT  "SMS-STATUS-REPORT".
*/
/**@ingroup tsms_tpdu_group
* @def tsms_tpdu_status_report_set_userdata
* Sets the content of the @a SMS-STATUS-REPORT message.
* @param self A pointer to the @a SMS-STATUS-REPORT (@ref tsms_tpdu_status_report_t).
* @param udata A pointer to the content.
* @param alpha The alphabet used to encode the content.
* @retval Zero if succeed and non-zero error code otherwise.
*
* See For more information, see @ref tsms_tpdu_group_STATUS_REPORT  "SMS-STATUS-REPORT".
*/
#define tsms_tpdu_status_report_serialize(self, output) tsms_tpdu_message_serialize(TSMS_TPDU_MESSAGE(self), output, tsk_false)
#define tsms_tpdu_status_report_tostring(self) tsms_tpdu_message_tostring(TSMS_TPDU_MESSAGE(self), tsk_false)
#define tsms_tpdu_status_report_tohexastring(self) tsms_tpdu_message_tohexastring(TSMS_TPDU_MESSAGE(self), tsk_false)
#define tsms_tpdu_status_report_set_userdata(self, udata, alpha) tsms_tpdu_message_set_userdata(TSMS_TPDU_MESSAGE(self), udata, alpha)

TINYSMS_GEXTERN const tsk_object_def_t *tsms_tpdu_status_report_def_t;

TSMS_END_DECLS


#endif /* TINYSMS_TSMS_TPDU_STATUS_REPORT_H */
