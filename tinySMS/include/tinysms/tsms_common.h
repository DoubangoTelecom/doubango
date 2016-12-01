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

/**@file tsms_common.h
 * @brief SMS Commons (3GPP TS 23.038 and al.).
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#ifndef TINYSMS_TSMS_COMMON_H
#define TINYSMS_TSMS_COMMON_H

#include "tinysms_config.h"

#include "tinysms/tsms_address.h"

#include "tsk_buffer.h"

TSMS_BEGIN_DECLS

/** SMS alphabet values as per 3GPP TS 23.038 v911 section 4.
* Part of TP-DCS (SMS Data Coding Scheme).
*/
typedef enum tsms_alphabet_e {
    /*11*/ tsms_alpha_reserved = 0x03,
    /*00*/ tsms_alpha_7bit = 0x00,
    /*01*/ tsms_alpha_8bit = 0x01,
    /*10*/ tsms_alpha_ucs2 = 0x02
}
tsms_alphabet_t;

/* 3GPP TS 23.038 v911 section 4 */
#define TSMS_ALPHA_FROM_DCS(dcs) (((dcs) & 0x0C) >> 2) /* Bit3 and Bit2 */


/* ======================== TPDU ========================
=========================================================*/

/** 3GPP TS 23.040 - 9.2.3.1	TP Message Type Indicator (TP MTI) */
typedef enum tsms_tpdu_mti_e {
    /*0 0*/ tsms_tpdu_mti_deliver_mt		= 0x00,	/**< SMS-DELIVER (in the direction SC to MS)*/
    /*0 0*/ tsms_tpdu_mti_deliver_report_mo	= 0x00,	/**< SMS-DELIVER-REPORT (in the direction MS to SC)*/
    /*1 0*/ tsms_tpdu_mti_status_report_mt	= 0x02,	/**< SMS-STATUS-REPORT (in the direction SC to MS)*/
    /*1 0*/ tsms_tpdu_mti_command_mo		= 0x02,	/**< SMS-COMMAND (in the direction MS to SC)*/
    /*0 1*/ tsms_tpdu_mti_submit_mo			= 0x01,	/**< SMS-SUBMIT (in the direction MS to SC)*/
    /*0 1*/ tsms_tpdu_mti_submit_report_mt	= 0x01,	/**< SMS-SUBMIT-REPORT (in the direction SC to MS)*/
    /*1 1*/ tsms_tpdu_mti_reserved			= 0x03	/**< Reserved*/
}
tsms_tpdu_mti_t;

#define TSMS_TPDU_MTI_IS_RESERVED(mti)	((mti) == tsms_tpdu_mti_reserved)
#define TSMS_TPDU_MTI_IS_MO(mti)		(((mti) == tsms_tpdu_mti_deliver_report_mo) || ((mti) == tsms_tpdu_mti_command_mo) || ((mti) == tsms_tpdu_mti_submit_mo))

/** 3GPP TS 23.040 - 9.2.3.1	TP Message Type Indicator (TP MTI) */
typedef enum tsms_tpdu_vpf_e {
    // Bit4 and Bit3
    /*0	0*/	tsms_tpdu_vpf_not_present = 0x00, /**< TP VP field not present	*/
    /*1	0*/	tsms_tpdu_vpf_relative = 0x02, /**< TP VP field present - relative format*/
    /*0	1*/	tsms_tpdu_vpf_enhanced = 0x01, /**< TP-VP field present - enhanced format */
    /*1	1*/	tsms_tpdu_vpf_absolute = 0x03, /**< TP VP field present - absolute format */
}
tsms_tpdu_vpf_t;

/** SM-TL base type as per 3GPP TS 23.040 section 9.2.
*/
typedef struct tsms_tpdu_message_s {
    TSK_DECLARE_OBJECT;

    tsk_bool_t MobOrig;

    /** TP Message Type Indicator (TP MTI) as per TS 23.040 section 9.2.3.1. 2-bit field. */
    tsms_tpdu_mti_t mti;
    /** TP Protocol Identifier (M - o)
    * Parameter identifying the above layer protocol, if any. */
    uint8_t pid;
    /** TP Data Coding Scheme (M - o)
    * Parameter identifying the coding scheme within the TP-User-Data. */
    uint8_t dcs;
    /** TP User Data Length (M - I)
    * Parameter indicating the length of the TP User Data field to follow. */
    uint8_t udl;
    /** TP User Data (O - v)
    * User data. */
    tsk_buffer_t* ud;
}
tsms_tpdu_message_t;

#define TSMS_DECLARE_TPDU_MESSAGE tsms_tpdu_message_t tpdu
#define TSMS_TPDU_MESSAGE(self) ((tsms_tpdu_message_t*)(self))

/**@ingroup tsms_tpdu_group
* @def tsms_tpdu_message_serialize_mo
* Serialize an outgoing (MS to SC) @a TP-Message as binary content.
* @param self The @a TP-Message (any SMS-*) to serialize.
* @param output A pointer to the @a output buffer.
* @retval Zero if succeed and non-zero error code otherwise.
*/
/**@ingroup tsms_tpdu_group
* @def tsms_tpdu_message_serialize_mt
* Serialize an incoming (SC to MS) @a TP-Message as binary content.
* @param self The @a TP-Message (SMS-SUBMIT, SMS-DELIVER, SMS-COMMAND ...) to serialize.
* @param output A pointer to the @a output buffer.
* @retval Zero if succeed and non-zero error code otherwise.
*/
/**@ingroup tsms_tpdu_group
* @def tsms_tpdu_message_deserialize_mo
* Deserialize the outgoing (MO to SC) binary content as a @a TP-Message.
* @param data A pointer to the binary data.
* @param size The size of the buffer holding the binary data.
* @retval @ref tsms_tpdu_message_t if succeed and Null otherwise.
*/
/**@ingroup tsms_tpdu_group
* @def tsms_tpdu_message_deserialize_mt
* Deserialize the incoming (SC to MS) binary content as a @a TP-Message.
* @param data A pointer to the binary data.
* @param size The size of the buffer holding the binary data.
* @retval @ref tsms_tpdu_message_t if succeed and Null otherwise.
*/

int tsms_tpdu_message_init(tsms_tpdu_message_t* self, tsms_tpdu_mti_t mti);
TINYSMS_API int tsms_tpdu_message_serialize(const tsms_tpdu_message_t* self, tsk_buffer_t* output, tsk_bool_t MobOrig);
#define tsms_tpdu_message_serialize_mo(self, output) tsms_tpdu_message_serialize(self, output, tsk_true)
#define tsms_tpdu_message_serialize_mt(self, output) tsms_tpdu_message_serialize(self, output, tsk_false)
TINYSMS_API tsms_tpdu_message_t* tsms_tpdu_message_deserialize(const void* data, tsk_size_t size, tsk_bool_t MobOrig);
#define tsms_tpdu_message_deserialize_mo(data, size) tsms_tpdu_message_deserialize(data, size, tsk_true)
#define tsms_tpdu_message_deserialize_mt(data, size) tsms_tpdu_message_deserialize(data, size, tsk_false)
TINYSMS_API char* tsms_tpdu_message_tostring(const tsms_tpdu_message_t* self, tsk_bool_t MobOrig);
TINYSMS_API char* tsms_tpdu_message_tohexastring(const tsms_tpdu_message_t* self, tsk_bool_t MobOrig);
TINYSMS_API char* tsms_tpdu_message_get_payload(const tsms_tpdu_message_t* self);
TINYSMS_API int tsms_tpdu_message_set_userdata(tsms_tpdu_message_t* self, const tsk_buffer_t* udata, tsms_alphabet_t alpha);
int tsms_tpdu_message_deinit(tsms_tpdu_message_t* self);

#define TSMS_TPDU_DEFAULT_PID			0x00					/**< 3GPP TS 23.040 section 9.2.3.9 - TP-Protocol-Identifier (TP-PID) */
#define TSMS_TPDU_DEFAULT_DCS			0x00					/**< 3GPP TS 23.040 section  9.2.3.10 - TP-Data-Coding-Scheme (TP-DCS) (default class, 7 bit message) + GSM 03.38*/
#define TSMS_TPDU_DEFAULT_VP			0xAA					/**< 3GPP TS 23.040 section 9.2.3.12 - TP-Validity-Period */
#define TSMS_TPDU_DEFAULT_VPF			tsms_tpdu_vpf_relative	/**< 3GPP TS 23.040 section 9.2.3.3 - TP Validity Period Format (TP VPF) */
#define TSMS_TPDU_DEFAULT_FCS			0xFF					/**< 3GPP TS 23.040 section  9.2.3.22 - TP-Failure-Cause (TP-FCS) */
#define TSMS_TPDU_DEFAULT_PI			0x00					/**< 3GPP TS 23.040 section  9.2.3.27 - TP-Parameter-Indicator (TP-PI) */
#define TSMS_TPDU_DEFAULT_SCTS			"00000000000000"		/**< 3GPP TS 23.040 section  9.2.3.11 - TP-Service-Centre-Time-Stamp (TP-SCTS) */
#define TSMS_TPDU_DEFAULT_MMS			0x01					/**< 3GPP TS 23.040 section  9.2.3.2 - TP More Messages to Send (TP-MMS) */

/**< Indicates whether to append SMSC address at the begining of the TPDU content.
*/
#define TSMS_TPDU_APPEND_SMSC	0

/* ======================== RPDU ========================
=========================================================*/

/** RP-MTI types as per 3GPP TS 24.011 section 8.2.2
* 3bit field located in the first octet of all RP-Messages. */
typedef enum tsms_rpdu_type_e {
    /*000*/ tsms_rpdu_type_data_mo		= 0x00, /**< RP-DATA message ms->n */
    /*001*/ tsms_rpdu_type_data_mt		= 0x01, /**< RP-DATA message n->ms */
    /*010*/ tsms_rpdu_type_ack_mo		= 0x02, /**< RP-ACK message ms->n */
    /*011*/ tsms_rpdu_type_ack_mt		= 0x03, /**< RP-ACK message n->ms */
    /*100*/ tsms_rpdu_type_error_mo		= 0x04, /**< RP-ERROR message ms->n */
    /*101*/ tsms_rpdu_type_error_mt		= 0x05, /**< RP-ERROR message n->ms */
    /*110*/ tsms_rpdu_type_smma_mo		= 0x06, /**< RP-SMMA message ms->n */
}
tsms_rpdu_type_t;

#define TSMS_RPDU_TYPE_IS_MO(type) (((type) == tsms_rpdu_type_data_mo) \
									|| ((type) == tsms_rpdu_type_ack_mo) \
									|| ((type) == tsms_rpdu_type_error_mo) \
									|| ((type) == tsms_rpdu_type_smma_mo))

typedef struct tsms_rpdu_message_s {
    TSK_DECLARE_OBJECT;

    tsms_rpdu_type_t mti;
    uint8_t mr; /**< Message Reference. */
}
tsms_rpdu_message_t;
#define TSMS_DECLARE_RPDU_MESSAGE tsms_rpdu_message_t rpdu
#define TSMS_RPDU_MESSAGE(self) ((tsms_rpdu_message_t*)(self))

TINYSMS_API int tsms_rpdu_message_serialize(const tsms_rpdu_message_t* self, tsk_buffer_t* output);
TINYSMS_API tsms_rpdu_message_t* tsms_rpdu_message_deserialize(const void* data, tsk_size_t size);
TINYSMS_API char* tsms_rpdu_message_tohexastring(const tsms_rpdu_message_t* self);

TSMS_END_DECLS

#endif /* TINYSMS_TSMS_COMMON_H */
