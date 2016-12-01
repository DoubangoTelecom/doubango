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

/**@file tsms_tpdu_deliver.h
 * @brief SMS TPDU SMS-DELIVER message as per 3GPP TS 23.040 section 9.2.2.1.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#ifndef TINYSMS_TSMS_TPDU_DELIVER_H
#define TINYSMS_TSMS_TPDU_DELIVER_H

#include "tinysms_config.h"

#include "tinysms/tsms_common.h"


TSMS_BEGIN_DECLS

/** SMS TPDU SMS-DELIVER message as per 3GPP TS 23.040 section 9.2.2.1.
*/
typedef struct tsms_tpdu_deliver_s {
    TSMS_DECLARE_TPDU_MESSAGE;

    tsms_address_t* smsc;

    /** TP More Messages to Send (M - 1b)
    * Parameter indicating whether or not there are more messages to send. */
    unsigned mms:1;
    /** TP-Loop-Prevention (O - 1b)
    * Parameter indicating that SMS applications should inhibit forwarding or automatic message generation that could cause infinite looping. */
    unsigned lp:2;
    /** TP Reply Path (M - 1b)
    * Parameter indicating the request for Reply Path.
    Section 9.2.3.28 */
    unsigned rp:1;
    /** TP User Data Header Indicator (O - 1b)
    * Parameter indicating that the TP UD field contains a Header. */
    unsigned udhi:1;
    /** TP Status Report Indicator (O - 1b)
    * Parameter indicating if the SME has requested a status report. */
    unsigned sri:1;
    /** TP Originating Address (M - 2-12o)
    * Address of the originating SME. */
    tsms_address_t* oa;
    /** TP Protocol Identifier (M - o)
    * Parameter identifying the above layer protocol, if any. */
    //(base)uint8_t pid;
    /** TP Data Coding Scheme (M - o)
    * Parameter identifying the coding scheme within the TP-User-Data. */
    //(base)uint8_t dcs;
    /** TP Service Centre Time Stamp (M - 7o)
    * Parameter identifying time when the SC received the message. */
    uint8_t scts[7];
    /** TP User Data Length (M - I)
    * Parameter indicating the length of the TP User Data field to follow. */
    //(base)uint8_t udl;
    /** TP User Data (O - v)
    * User data. */
    //(base)tsk_buffer_t* ud;
}
tsms_tpdu_deliver_t;

/** cast any pointer to @ref tsms_tpdu_deliver_t* */
#define TSMS_TPDU_DELIVER(self) ((tsms_tpdu_deliver_t*)(self))

TINYSMS_API tsms_tpdu_deliver_t* tsms_tpdu_deliver_create(const tsms_address_string_t smsc, const tsms_address_string_t orig);

/**@ingroup tsms_tpdu_group
* @def tsms_tpdu_deliver_serialize
* Serializes a @a SMS-DELIVER message as binary content.
* @param self A pointer to the @a SMS-DELIVER (@ref tsms_tpdu_deliver_t) message to serialize.
* @param output A pointer to the output buffer. Should be valid.
* @retval Zero if succeed and non-zero error code otherwise.
*
* See For more information, see @ref tsms_tpdu_group_DELIVER "SMS-DELIVER".
*/
/**@ingroup tsms_tpdu_group
* @def tsms_tpdu_deliver_tohexastring
* Serializes a @a SMS-DELIVER message as hexa-string content.
* @param self A pointer to the @a SMS-DELIVER (@ref tsms_tpdu_deliver_t) message to serialize.
* @retval A pointer to the hexa-string if succeed and Null otherwise.
*
* See For more information, see @ref tsms_tpdu_group_DELIVER "SMS-DELIVER".
*/
/**@ingroup tsms_tpdu_group
* @def tsms_tpdu_deliver_set_userdata
* Sets the content of the @a SMS-DELIVER message.
* @param self A pointer to the @a SMS-DELIVER (@ref tsms_tpdu_deliver_t).
* @param udata A pointer to the content.
* @param alpha The alphabet used to encode the content.
* @retval Zero if succeed and non-zero error code otherwise.
*
* See For more information, see @ref tsms_tpdu_group_DELIVER "SMS-DELIVER".
*/
#define tsms_tpdu_deliver_serialize(self, output) tsms_tpdu_message_serialize(TSMS_TPDU_MESSAGE(self), output, tsk_false)
#define tsms_tpdu_deliver_tostring(self) tsms_tpdu_message_tostring(TSMS_TPDU_MESSAGE(self), tsk_false)
#define tsms_tpdu_deliver_tohexastring(self) tsms_tpdu_message_tohexastring(TSMS_TPDU_MESSAGE(self), tsk_false)
#define tsms_tpdu_deliver_set_userdata(self, udata, alpha) tsms_tpdu_message_set_userdata(TSMS_TPDU_MESSAGE(self), udata, alpha)

TINYSMS_GEXTERN const tsk_object_def_t *tsms_tpdu_deliver_def_t;

TSMS_END_DECLS

#endif /* TINYSMS_TSMS_TPDU_DELIVER_H */
