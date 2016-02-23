/* Copyright (C) 2014 Mamadou DIOP.
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
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/
#ifndef TNET_STUN_TYPES_H
#define TNET_STUN_TYPES_H

#include "tinynet_config.h"

TNET_BEGIN_DECLS

typedef uint8_t tnet_stun_addr_t[16]; // IPv4(32bits) or IPv6(128bits)
typedef uint64_t tnet_stun_binding_id_t;
typedef long tnet_turn_peer_id_t;

/**@ingroup tnet_stun_group
 * Checks if the pointer to the buffer hold a STUN header by checking that it starts with 0b00 and contain the magic cookie.
 *			As per RFC 5389 subclause 19: Explicitly point out that the most significant 2 bits of STUN are
 *			0b00, allowing easy differentiation with RTP packets when used with ICE.
 *			As per RFC 5389 subclause 6: The magic cookie field MUST contain the fixed value 0x2112A442 in
 *			network byte order.
 *
 * @param	PU8	The pointer to the buffer holding the STUN raw data.
**/
#define TNET_STUN_BUFF_IS_STUN2(PU8, SIZE)	\
	( \
		((PU8)) && \
		((SIZE) >= kStunPktHdrSizeInOctets) && \
		(((PU8)[0] & 0xc0) == 0x00) && \
		( PU8[4] == 0x21 && PU8[5] == 0x12 && PU8[6] == 0xA4 && PU8[7] == 0x42 ) \
	)

// rfc5766 - 11.  Channels
#define TNET_STUN_BUFF_IS_CHANNEL_DATA(PU8, SIZE)	\
	( \
		((PU8)) && \
		((SIZE) >= kStunChannelDataHdrSizeInOctets) && \
		(((PU8)[0] & 0xc0) == 0x40)  \
	)

#define kStunOptFingerPrint			1
#define kStunOptDontFragment		1

#define kStunErrCodeUnauthorized		401
#define kStunErrCodeUnknownAttributes	420
#define kStunErrCodeStaleNonce			438
#define kStunErrCodeIceConflict			487

// Estimate of the round-trip time (RTT) in millisecond.
#define kStunRTO			500

// Number of retransmission for UDP retransmission in millisecond.
//	7.2.1.  Sending over UDP
//	Rc SHOULD be configurable and SHOULD have a default of 7.
#define kStunRC				/*7*/4/* 7 is too hight */

#define kStunBindingInvalidId				0

#if !defined(kStunBuffMinPad)
#	define kStunBuffMinPad	40 // to make the buffer kasher
#endif /* kStunBuffMinPad */

#if !defined(kStunSoftware)
#	define kStunSoftware TNET_SOFTWARE
#endif /* kStunSoftware */

#if !defined(kStunPortDefaultTcpUdp)
#	define kStunPortDefaultTcpUdp 3478
#endif /* kStunPortDefaultTcpUdp */

#if !defined(kStunPortDefaultTls)
#	define kStunPortDefaultTls 5349
#endif /* kStunPortDefaultTls */

// rfc5389 - 15.  STUN Attributes
#if !defined(kStunAttrHdrSizeInOctets)
#	define kStunAttrHdrSizeInOctets 4
#endif /* kStunAttrHdrSizeInOctets */

// rfc5389 - 6.  STUN Message Structure
#if !defined(kStunPktHdrSizeInOctets)
#	define kStunPktHdrSizeInOctets 20
#endif /* kStunPktHdrSizeInOctets */

// STUN2 magic cookie value in network byte order as per RFC 5389 subclause 6.
#if !defined(kStunMagicCookieLong)
#	define kStunMagicCookieLong 0x2112A442
#endif /* kStunMagicCookieLong */
#if !defined(kStunMagicCookie)
#	define kStunMagicCookie kStunMagicCookieLong
#endif /* kStunMagicCookie */
#if !defined(kStunMagicCookieShort)
#	define kStunMagicCookieShort 0x2112
#endif /* kStunMagicCookieShort */

#if !defined (kStunFingerprintXorConst)
#	define kStunFingerprintXorConst 0x5354554e
#endif /* kStunFingerprintXorConst */

// STUN trasactionn ID size (96bits = 12bytes)
#if !defined(kStunTransacIdSize)
#	define kStunTransacIdSize		12
#endif
typedef uint8_t tnet_stun_transac_id_t[kStunTransacIdSize];

/**@ingroup tnet_stun_group
 * List of all supported STUN classes as per RFC 5389 subcaluse 6.
**/
typedef enum tnet_stun_class_e {
    tnet_stun_class_request = 0x00,			/**< Request class: 0b00 */
    tnet_stun_class_indication = 0x01,			/**< Indication class: 0b01 */
    tnet_stun_class_success_response = 0x02,	/**< Success response class: 0b10 */
    tnet_stun_class_error_response = 0x03,		/**< Error/failure response class: 0b11 */
}
tnet_stun_class_t;

/**@ingroup tnet_stun_group
 * List of all supported STUN methods.
 * RFC 5389 defines only one method(Bining). All other methods have been defined by TURN (rfc5766 section 13).
**/
typedef enum tnet_stun_method_e {
    tnet_stun_method_binding = 0x0001, /**< RFC 5389 - Binding method: 0b000000000001 */

    tnet_stun_method_allocate = 0x0003,  /**< rfc5766 - Allocate          (only request/response semantics defined) */
    tnet_stun_method_refresh = 0x0004,  /**< rfc5766 - Refresh           (only request/response semantics defined) */
    tnet_stun_method_send = 0x0006,  /**< rfc5766 - Send              (only indication semantics defined) */
    tnet_stun_method_data = 0x0007,  /**< rfc5766 - Data              (only indication semantics defined) */
    tnet_stun_method_createpermission = 0x0008,  /**< rfc5766 - CreatePermission  (only request/response semantics defined */
    tnet_stun_method_channelbind = 0x0009,  /**< rfc5766 - ChannelBind       (only request/response semantics defined) */
    tnet_stun_method_connect = 0x000a,/**< rfc6062 - Connect */
    tnet_stun_method_connectionbind = 0x000b,/**< rfc6062 - ConnectionBind */
    tnet_stun_method_connectionattempt = 0x000c,/**< rfc6062 - ConnectionAttempt */
}
tnet_stun_method_t;

/**@ingroup tnet_stun_group
*/
typedef enum tnet_stun_mask_e {
    tnet_stun_mask_request = 0x0000,
    tnet_stun_mask_indication = 0x0010,
    tnet_stun_mask_success = 0x0100,
    tnet_stun_mask_error = 0x0110
}
tnet_stun_mask_t;

/**@ingroup tnet_stun_group
 * STUN IP family as per RFC 5389 subclause 15.1.
**/
typedef enum tnet_stun_address_family_e {
    tnet_stun_address_family_ipv4 = 0x01,
    tnet_stun_address_family_ipv6 = 0x02
} tnet_stun_address_family_t;

// rfc5766 - 14.7.  REQUESTED-TRANSPORT
typedef enum tnet_turn_transport_e {
    tnet_turn_transport_udp = 17,
    tnet_turn_transport_tcp = 6
}
tnet_turn_transport_t;

// RFC 5389  - 15.6.  ERROR-CODE
#define kStunErrorClassTryAlternate						3
#define kStunErrorNumberTryAlternate					0
#define kStunErrorPhraseTryAlternate					"Try Alternate"
#define kStunErrorClassBadRequest						4
#define kStunErrorNumberBadRequest						0
#define kStunErrorPhraseBadRequest						"Bad Request"
#define kStunErrorClassUnauthorized						4
#define kStunErrorNumberUnauthorized					1
#define kStunErrorPhraseUnauthorized					"Unauthorized"
#define kStunErrorClassUnknownAttribute					4
#define kStunErrorNumberUnknownAttribute				20
#define kStunErrorPhraseUnknownAttribute				"Unknown Attribute"
#define kStunErrorClassStaleNonce						4
#define kStunErrorNumberStaleNonce						38
#define kStunErrorPhraseStaleNonce						"Stale Nonce"
#define kStunErrorClassServerError						5
#define kStunErrorNumberServerError						0
#define kStunErrorPhraseServerError						"Server Error"
// rfc5766 - 15.  New STUN Error Response Codes
#define kStunErrorClassForbidden						4
#define kStunErrorNumberForbidden						3
#define kStunErrorPhraseForbidden						"Forbidden"
#define kStunErrorClassAllocationMismatch				4
#define kStunErrorNumberAllocationMismatch				37
#define kStunErrorPhraseAllocationMismatch				"Allocation Mismatch"
#define kStunErrorClassWrongCredentials					4
#define kStunErrorNumberWrongCredentials				42
#define kStunErrorPhraseWrongCredentials				"Wrong Credentials"
#define kStunErrorClassUnsupportedTransportProtocol		4
#define kStunErrorNumberUnsupportedTransportProtocol	42
#define kStunErrorPhraseUnsupportedTransportProtocol	"Unsupported Transport Protocol"
#define kStunErrorClassAllocationQuotaReached			4
#define kStunErrorNumberAllocationQuotaReached			86
#define kStunErrorPhraseAllocationQuotaReached			"Allocation Quota Reached"
#define kStunErrorClassInsufficientCapacity				5
#define kStunErrorNumberInsufficientCapacity			8
#define kStunErrorPhraseInsufficientCapacity			"Insufficient Capacity"


/**@ingroup tnet_stun_group
 * STUN attr types as per RFC 5389 subclause 18.2.
**/
typedef enum tnet_stun_attr_type_e {
    /* === RFC 5389 - Comprehension-required range (0x0000-0x7FFF) */
    tnet_stun_attr_type_reserved = 0x0000,				/**< (Reserved) */
    tnet_stun_attr_type_mapped_address = 0x0001,			/**< http://tools.ietf.org/html/rfc5389#page-32 */
    tnet_stun_attr_type_response_address = 0x0002,		/**< (Reserved; was RESPONSE-ADDRESS) */
    tnet_stun_attr_type_change_address = 0x0003,			/**< (Reserved; was CHANGE-ADDRESS) */
    tnet_stun_attr_type_source_address = 0x0004,			/**< (Reserved; was SOURCE-ADDRESS) */
    tnet_stun_attr_type_changed_address = 0x0005,			/**< (Reserved; was CHANGED-ADDRESS) */
    tnet_stun_attr_type_username = 0x0006,				/**< http://tools.ietf.org/html/rfc5389#page-34 */
    tnet_stun_attr_type_password = 0x0007,				/**< (Reserved; was PASSWORD) */
    tnet_stun_attr_type_message_integrity = 0x0008,		/**< http://tools.ietf.org/html/rfc5389#page-34 */
    tnet_stun_attr_type_error_code = 0x0009,				/**< http://tools.ietf.org/html/rfc5389#page-36 */
    tnet_stun_attr_type_unknown_attrs = 0x000A,			/**< http://tools.ietf.org/html/rfc5389#page-38 */
    tnet_stun_attr_type_reflected_from = 0x000B,			/**< (Reserved; was REFLECTED-FROM) */
    tnet_stun_attr_type_realm = 0x0014,					/**< http://tools.ietf.org/html/rfc5389#page-38 */
    tnet_stun_attr_type_nonce = 0x0015,					/**< http://tools.ietf.org/html/rfc5389#page-38 */
    tnet_stun_attr_type_xor_mapped_address = 0x0020,		/**< http://tools.ietf.org/html/rfc5389#page-33 */

    /* === RFC 5389 - Comprehension-optional range (0x8000-0xFFFF) */
    tnet_stun_attr_type_software = 0x8022,				/**< http://tools.ietf.org/html/rfc5389#page-39 */
    tnet_stun_attr_type_alternate_server = 0x8023,		/**< http://tools.ietf.org/html/rfc5389#page-39 */
    tnet_stun_attr_type_fingerprint = 0x8028,				/**< http://tools.ietf.org/html/rfc5389#page-36 */

    /* === rfc5766 */
    tnet_stun_attr_type_channel_number = 0x000C,			/**< rfc5766 -  CHANNEL-NUMBER */
    tnet_stun_attr_type_lifetime = 0x000D,					/**< rfc5766 -  LIFETIME */
    tnet_stun_attr_type_reserved2 = 0x0010,				/**< rfc5766 -  Reserved (was BANDWIDTH) */
    tnet_stun_attr_type_xor_peer_address = 0x0012,			/**< rfc5766 -  XOR-PEER-ADDRESS */
    tnet_stun_attr_type_data = 0x0013,						/**< rfc5766 -  DATA */
    tnet_stun_attr_type_xor_relayed_address = 0x0016,		/**< rfc5766 -  XOR-RELAYED-ADDRESS */
    tnet_stun_attr_type_even_port = 0x0018,				/**< rfc5766 -  EVEN-PORT */
    tnet_stun_attr_type_requested_transport = 0x0019,		/**< rfc5766 -  REQUESTED-TRANSPORT */
    tnet_stun_attr_type_dont_fragment = 0x001A,			/**< rfc5766 -  DONT-FRAGMENT */
    tnet_stun_attr_type_reserved3 = 0x0021,				/**< rfc5766 -  Reserved (was TIMER-VAL) */
    tnet_stun_attr_type_reservation_token = 0x0022,		/**< rfc5766 -  RESERVATION-TOKEN */

    /* RFC 5245 */
    tnet_stun_attr_type_ice_priority = 0x0024, /**< 21.2. STUN Attributes */
    tnet_stun_attr_type_ice_use_candidate = 0x0025, /**< 21.2. STUN Attributes */
    tnet_stun_attr_type_ice_controlled = 0x8029, /**< 21.2. STUN Attributes */
    tnet_stun_attr_type_ice_controlling = 0x802A, /**< 21.2. STUN Attributes */

    /* rfc6062 */
    tnet_stun_attr_type_connection_id = 0x002a, /**< 6.2.  New STUN Attributes */
} tnet_stun_attr_type_t;


/**@ingroup tnet_stun_group
* List of all supported STUN message types.
*/
typedef enum tnet_stun_pkt_type_e {
    /*	RFC 5389 - 6.  STUN Message Structure

    	The message type defines the message class (request, success
    	response, failure response, or indication) and the message method
    	(the primary function) of the STUN message.  Although there are four
    	message classes, there are only two types of transactions in STUN:
    	request/response transactions (which consist of a request message and
    	a response message) and indication transactions (which consist of a
    	single indication message).  Response classes are split into error
    	and success responses to aid in quickly processing the STUN message.

    	The message type field is decomposed further into the following
    	structure:

    	0                 1
        2  3  4 5 6 7 8 9 0 1 2 3 4 5
       +--+--+-+-+-+-+-+-+-+-+-+-+-+-+
       |M |M |M|M|M|C|M|M|M|C|M|M|M|M|
       |11|10|9|8|7|1|6|5|4|0|3|2|1|0|
       +--+--+-+-+-+-+-+-+-+-+-+-+-+-+
    */
    tnet_stun_pkt_type_binding_request = (tnet_stun_method_binding | tnet_stun_mask_request),
    tnet_stun_pkt_type_binding_indication = (tnet_stun_method_binding | tnet_stun_mask_indication),
    tnet_stun_pkt_type_binding_success_response = (tnet_stun_method_binding | tnet_stun_mask_success),
    tnet_stun_pkt_type_binding_error_response = (tnet_stun_method_binding | tnet_stun_mask_error),

    tnet_stun_pkt_type_allocate_request = (tnet_stun_method_allocate | tnet_stun_mask_request),
    tnet_stun_pkt_type_allocate_indication = (tnet_stun_method_allocate | tnet_stun_mask_indication),
    tnet_stun_pkt_type_allocate_success_response = (tnet_stun_method_allocate | tnet_stun_mask_success),
    tnet_stun_pkt_type_allocate_error_response = (tnet_stun_method_allocate | tnet_stun_mask_error),

    tnet_stun_pkt_type_refresh_request = (tnet_stun_method_refresh | tnet_stun_mask_request),
    tnet_stun_pkt_type_refresh_indication = (tnet_stun_method_refresh | tnet_stun_mask_indication),
    tnet_stun_pkt_type_refresh_success_response = (tnet_stun_method_refresh | tnet_stun_mask_success),
    tnet_stun_pkt_type_refresh_error_response = (tnet_stun_method_refresh | tnet_stun_mask_error),

    tnet_stun_pkt_type_send_indication = (tnet_stun_method_send | tnet_stun_mask_indication),

    tnet_stun_pkt_type_data_indication = (tnet_stun_method_data | tnet_stun_mask_indication),

    tnet_stun_pkt_type_createpermission_request = (tnet_stun_method_createpermission | tnet_stun_mask_request),
    tnet_stun_pkt_type_createpermission_indication = (tnet_stun_method_createpermission | tnet_stun_mask_indication),
    tnet_stun_pkt_type_createpermission_success_response = (tnet_stun_method_createpermission | tnet_stun_mask_success),
    tnet_stun_pkt_type_createpermission_error_response = (tnet_stun_method_createpermission | tnet_stun_mask_error),

    tnet_stun_pkt_type_channelbind_request = (tnet_stun_method_channelbind | tnet_stun_mask_request),
    tnet_stun_pkt_type_channelbind_indication = (tnet_stun_method_channelbind | tnet_stun_mask_indication),
    tnet_stun_pkt_type_channelbind_success_response = (tnet_stun_method_channelbind | tnet_stun_mask_success),
    tnet_stun_pkt_type_channelbind_error_response = (tnet_stun_method_channelbind | tnet_stun_mask_error),

    tnet_stun_pkt_type_connect_request = (tnet_stun_method_connect | tnet_stun_mask_request),
    tnet_stun_pkt_type_connect_indication = (tnet_stun_method_connect | tnet_stun_mask_indication),
    tnet_stun_pkt_type_connect_success_response = (tnet_stun_method_connect | tnet_stun_mask_success),
    tnet_stun_pkt_type_connect_error_response = (tnet_stun_method_connect | tnet_stun_mask_error),

    tnet_stun_pkt_type_connectionbind_request = (tnet_stun_method_connectionbind | tnet_stun_mask_request),
    tnet_stun_pkt_type_connectionbind_indication = (tnet_stun_method_connectionbind | tnet_stun_mask_indication),
    tnet_stun_pkt_type_connectionbind_success_response = (tnet_stun_method_connectionbind | tnet_stun_mask_success),
    tnet_stun_pkt_type_connectionbind_error_response = (tnet_stun_method_connectionbind | tnet_stun_mask_error),

    tnet_stun_pkt_type_connectionattempt_indication = (tnet_stun_method_connectionattempt | tnet_stun_mask_indication),
}
tnet_stun_pkt_type_t;

/*	RFC 5389 - 7.2.1.  Sending over UDP
		A client SHOULD retransmit a STUN request message starting with an
		interval of RTO ("Retransmission TimeOut"), doubling after each
		retransmission.

		e.g. 0 ms, 500 ms, 1500 ms, 3500 ms, 7500ms, 15500 ms, and 31500 ms
	*/
#define kStunUdpRetransmitTimoutMinInMs 500
#define kStunUdpRetransmitTimoutMaxInMs 31500

// rfc5766 - 2.2.  Allocations
#if !defined(kTurnAllocationTimeOutInSec)
#	define kTurnAllocationTimeOutInSec 600 /* 10min */ // FIXME
#endif /* kTurnAllocationTimeOutInSec */

// rfc5766 - 2.3.  Permissions
#if !defined(kTurnPermissionTimeOutInSec)
#	define kTurnPermissionTimeOutInSec 300 /* 5min */
#endif /* kTurnPermissionTimeOutInSec */

// rfc5766 - 2.5.  Channels
#if !defined(kTurnChannelBindingTimeOutInSec)
#	define kTurnChannelBindingTimeOutInSec 600 /* 10min */
#endif /* kTurnChannelBindingTimeOutInSec */

// rfc5766 - 11.4.  The ChannelData Message
#if !defined(kStunChannelDataHdrSizeInOctets)
#	define kStunChannelDataHdrSizeInOctets 4
#endif /* kStunChannelDataHdrSizeInOctets */

// Not part of the standard
typedef enum tnet_stun_state_e {
    tnet_stun_state_none,
    tnet_stun_state_trying,
    tnet_stun_state_ok,
    tnet_stun_state_nok
} tnet_stun_state_t;

TNET_END_DECLS

#endif /* TNET_STUN_TYPES_H */
