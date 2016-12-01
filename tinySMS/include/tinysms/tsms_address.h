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

/**@file tsms_address.h
 * @brief SMS address encoder/decoder.
 *
 * @author Mamadou Diop <diopmamadou [at) doubango (DOT) org>
 *

 */
#ifndef TINYSMS_TSMS_ADDRESS_H
#define TINYSMS_TSMS_ADDRESS_H

#include "tinysms_config.h"

#include "tsk_buffer.h"

TSMS_BEGIN_DECLS

typedef uint8_t tsms_address_string_t[13]; /* 12 + (+) */

/** Address type
*/
typedef enum tsms_address_type_e {
    tsms_addr_oa,
    tsms_addr_da,
    tsms_addr_smsc,
}
tsms_address_type_t;

/** Type-of-number
* 3GPP TS 23.040 v910 section 9.1.2.5.
*/
typedef enum tsms_address_ton_e {
    /** "Unknown" is used when the user or network has no a priori information about the numbering plan. In
    	this case, the Address-Value field is organized according to the network dialling plan, e.g. prefix or
    	escape digits might be present.*/
    tsms_addr_ton_unknown			= 0x00, // 0b000
    /** International number
    	The international format shall be accepted also when the message is destined to a recipient in the
    	same country as the MSC. */
    tsms_addr_ton_international		= 0x01, // 0b001
    /** National number
    	Prefix or escape digits shall not be included */
    tsms_addr_ton_national			= 0x02, // 0b010
    /** "Network specific number" is used to indicate administration/service number specific to the serving
    	network, e.g. used to access an operator. */
    tsms_addr_ton_network_specific	= 0x03, // 0b011
    /** "Subscriber number" is used when a specific short number representation is stored in one or more
    	SCs as part of a higher layer application. (Note that "Subscriber number" shall only be used in
    	connection with the proper PID referring to this application). */
    tsms_addr_ton_subscriber		= 0x04, // 0b100
    /** Alphanumeric, (coded according to GSM TS 03.38 7-bit default alphabet) */
    tsms_addr_ton_alphanumeric		= 0x05, // 0b101
    /** Abbreviated number */
    tsms_addr_ton_abbreviated		= 0x06, // 0b110
    /** Reserved for extension */
    tsms_addr_ton_reserved			= 0x07, // 0b111
}
tsms_address_ton_t;

/** Numbering-Plan-Identification
* 3GPP TS 23.040 v910 section 9.1.2.5.
*/
typedef enum tsms_address_npi_e {
    /* 0000 */ tsms_addr_npi_unknown	= 0x00,	/**< Unknown */
    /* 0001 */ tsms_addr_npi_isdn		= 0x01, /**< ISDN/telephone numbering plan (E.164/E.163) */
    /* 0011 */ tsms_addr_npi_x121		= 0x03,	/**< Data numbering plan (X.121) */
    /* 0100 */ tsms_addr_npi_telex		= 0x04,	/**< Telex numbering plan */
    /* 1000 */ tsms_addr_npi_national	= 0x08,	/**< National numbering plan */
    /* 1001 */ tsms_addr_npi_private	= 0x09,	/**< Private numbering plan */
    /* 1010 */ tsms_addr_npi_ermes		= 0x0A,	/**< ERMES numbering plan (ETSI DE/PS 3 01-3) */
    /* 1111 */ tsms_addr_npi_reserved	= 0x0F,	/**< Reserved for extension */
}
tsms_address_npi_t;

/** Address
*/
typedef struct tsms_address_s {
    TSK_DECLARE_OBJECT;

    // Address Type
    tsms_address_ton_t ton; /**< Type-of-number */
    tsms_address_npi_t npi; /**< Numbering-Plan-Identification */

    tsms_address_type_t type;
    char* digits; /* BCD digits */
}
tsms_address_t;

int tsms_address_serialize(const tsms_address_t* address, tsk_buffer_t* output);
tsms_address_t* tsms_address_deserialize(const void* data, tsk_size_t size, tsms_address_type_t xtype, tsk_size_t *length);


tsms_address_t* tsms_address_create(const tsms_address_string_t digits, tsms_address_type_t type);
tsms_address_t* tsms_address_oa_create(const tsms_address_string_t digits);
tsms_address_t* tsms_address_da_create(const tsms_address_string_t digits);
tsms_address_t* tsms_address_smsc_create(const tsms_address_string_t digits);

TINYSMS_GEXTERN const tsk_object_def_t *tsms_address_def_t;

TSMS_END_DECLS

#endif /* TSMS_BEGIN_DECLS */
