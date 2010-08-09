/*
* Copyright (C) 2009-2010 Mamadou Diop.
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

/**@file tnet_stun_attribute.c
 * @brief STUN2(RFC 5389) attribute parser.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_stun_attribute.h"

#include "tnet_stun.h"

#include "../tnet_types.h"
#include "../tnet_endianness.h"

#include "../turn/tnet_turn_attribute.h"

#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_debug.h"

#include <string.h>


/**@ingroup tnet_stun_group
* RFC 5389 - 15.  STUN Attributes 
* Creates new @ref tnet_stun_attribute_def_t object.
*/
tnet_stun_attribute_t* tnet_stun_attribute_create()
{
	return tsk_object_new(tnet_stun_attribute_def_t);
}

/**@ingroup tnet_stun_group
* RFC 5389 - 15.1.  MAPPED-ADDRESS.
* Creates new @ref tnet_stun_attribute_mapped_addr_t object.
*/
tnet_stun_attribute_mapped_addr_t* tnet_stun_attribute_mapped_address_create(const void* payload, tsk_size_t payload_size)
{
	return tsk_object_new(tnet_stun_attribute_mapped_addr_def_t, payload, payload_size);
}

/**@ingroup tnet_stun_group
* RFC 5389 - 15.2.  XOR-MAPPED-ADDRESS. 
* Creates new @ref tnet_stun_attribute_xmapped_addr_t object.
*/
tnet_stun_attribute_xmapped_addr_t* tnet_stun_attribute_xmapped_address_create(const void* payload, tsk_size_t payload_size)
{
	return tsk_object_new(tnet_stun_attribute_xmapped_addr_def_t, payload, payload_size);
}

/**@ingroup tnet_stun_group
* RFC 5389 - 15.3.  USERNAME.
* Creates new @ref tnet_stun_attribute_username_t object.
*/
tnet_stun_attribute_username_t* tnet_stun_attribute_username_create(const void* payload, tsk_size_t payload_size)
{
	return tsk_object_new(tnet_stun_attribute_username_def_t, payload, payload_size);
}

/**@ingroup tnet_stun_group
* RFC 5389 - 15.4.  MESSAGE-INTEGRITY.
* Creates new @ref tnet_stun_attribute_integrity_t object.
*/
tnet_stun_attribute_integrity_t* tnet_stun_attribute_integrity_create(const void* payload, tsk_size_t payload_size)
{
	return tsk_object_new(tnet_stun_attribute_integrity_def_t, payload, payload_size);
}

/**@ingroup tnet_stun_group
* RFC 5389 - 15.5.  FINGERPRINT.
* Creates new @ref tnet_stun_attribute_fingerprint_t object.
*/
tnet_stun_attribute_fingerprint_t* tnet_stun_attribute_fingerprint_create(uint32_t fingerprint)
{
	return tsk_object_new(tnet_stun_attribute_fingerprint_def_t, fingerprint);
}

/**@ingroup tnet_stun_group
* RFC 5389 - 15.6.  ERROR-CODE
* Creates new @ref tnet_stun_attribute_errorcode_t object.
*/
tnet_stun_attribute_errorcode_t* tnet_stun_attribute_errorcode_create(const void* payload, tsk_size_t payload_size)
{
	return tsk_object_new(tnet_stun_attribute_errorcode_def_t, payload, payload_size);
}

/**@ingroup tnet_stun_group
* RFC 5389 - 15.7.  REALM.
* Creates new @ref tnet_stun_attribute_realm_t object.
*/
tnet_stun_attribute_realm_t* tnet_stun_attribute_realm_create(const void* payload, tsk_size_t payload_size)
{
	return tsk_object_new(tnet_stun_attribute_realm_def_t, payload, payload_size);
}

/**@ingroup tnet_stun_group
* RFC 5389 - 15.8.  NONCE.
* Creates new @ref tnet_stun_attribute_nonce_t object.
*/
tnet_stun_attribute_nonce_t* tnet_stun_attribute_nonce_create(const void* payload, tsk_size_t payload_size)
{
	return tsk_object_new(tnet_stun_attribute_nonce_def_t, payload, payload_size);
}

/**@ingroup tnet_stun_group
* RFC 5389 - 15.9.  UNKNOWN-ATTRIBUTES.
* Creates new @ref tnet_stun_attribute_unknowns_t object.
*/
tnet_stun_attribute_unknowns_t* tnet_stun_attribute_unknowns_create(const void* payload, tsk_size_t payload_size)
{
	return tsk_object_new(tnet_stun_attribute_unknowns_def_t, payload, payload_size);
}

/**@ingroup tnet_stun_group
* RFC 5389 - 15.10.  SOFTWARE.
* Creates new @ref tnet_stun_attribute_software_t object.
*/
tnet_stun_attribute_software_t* tnet_stun_attribute_software_create(const void* payload, tsk_size_t payload_size)
{
	return tsk_object_new(tnet_stun_attribute_software_def_t, payload, payload_size);
}

/**@ingroup tnet_stun_group
* RFC 5389 - 15.11.  ALTERNATE-SERVER.
* Creates new @ref tnet_stun_attribute_altserver_t object.
*/
tnet_stun_attribute_altserver_t* tnet_stun_attribute_altserver_create(const void* payload, tsk_size_t payload_size)
{
	return tsk_object_new(tnet_stun_attribute_altserver_def_t, payload, payload_size);
}

/**@ingroup tnet_stun_group
* Creates @ref tnet_stun_attribute_t from raw buffer.
* @param data Raw buffer from which to create the STUN attribute.*
* @param size The size of the eaw buffer.
* @retval @ref tnet_stun_attribute_t object if succeed and NULL other wise.
*/
tnet_stun_attribute_t* tnet_stun_attribute_deserialize(const void* data, tsk_size_t size)
{
	tnet_stun_attribute_t *attribute = 0;
	const uint8_t* dataPtr = data;

	tnet_stun_attribute_type_t type = (tnet_stun_attribute_type_t)tnet_ntohs_2(dataPtr);
	uint16_t length = tnet_ntohs_2(&dataPtr[2]);

	/* Check validity */
	if(!data || size<=4/* Type(2-bytes) plus Length (2-bytes) */)
	{
		return 0;
	}

	dataPtr += (2 /* Type */+ 2/* Length */);

	/* Attribute Value
	*/
	
	switch(type)
	{
	/* RFC 5389 - 15.1.  MAPPED-ADDRESS */
	case stun_mapped_address:
		{
			attribute = (tnet_stun_attribute_t *)tnet_stun_attribute_mapped_address_create(dataPtr, length);
			break;
		}

	/* RFC 5389 -  15.2.  XOR-MAPPED-ADDRESS*/
	case stun_xor_mapped_address:
		{
			attribute = (tnet_stun_attribute_t *)tnet_stun_attribute_xmapped_address_create(dataPtr, length);
			break;
		}

	/* RFC 5389 -  15.3.  USERNAME*/
	case stun_username:
		{
			attribute = (tnet_stun_attribute_t *)tnet_stun_attribute_username_create(dataPtr, length);
			break;
		}


	/* RFC 5389 -  MESSAGE-INTEGRITY*/
	case stun_message_integrity:
		{
			if(length == TSK_SHA1_DIGEST_SIZE){
				attribute = (tnet_stun_attribute_t *)tnet_stun_attribute_integrity_create(dataPtr, length);
			}
			break;
		}

		/* RFC 5389 -  15.5.  FINGERPRINT*/
	case stun_fingerprint:
		{
			uint32_t fingerprint = tnet_htonl_2(dataPtr);
			attribute = (tnet_stun_attribute_t *)tnet_stun_attribute_fingerprint_create(fingerprint);
			break;
		}

	/* RFC 5389 -  15.6.  ERROR-CODE*/
	case stun_error_code:
		{
			attribute = (tnet_stun_attribute_t *)tnet_stun_attribute_errorcode_create(dataPtr, length);
			break;
		}

	/* RFC 5389 -  15.7.  REALM*/
	case stun_realm:
		{
			attribute = (tnet_stun_attribute_t *)tnet_stun_attribute_realm_create(dataPtr, length);
			break;
		}

	/* RFC 5389 -  15.8.  NONCE*/
	case stun_nonce:
		{
			attribute = (tnet_stun_attribute_t *)tnet_stun_attribute_nonce_create(dataPtr, length);
			break;
		}

	/* RFC 5389 -  15.9.  UNKNOWN-ATTRIBUTES*/
	case stun_unknown_attributes:
		{
			TSK_DEBUG_ERROR("DESERIALIZE:UNKNOWN-ATTRIBUTES ==> NOT IMPLEMENTED");
			attribute = tnet_stun_attribute_create();
			break;
		}

	/*	RFC 5389 - 15.10.  SOFTWARE */
	case stun_software:
		{
			attribute = (tnet_stun_attribute_t *)tnet_stun_attribute_software_create(dataPtr, length);
			break;
		}

	/*	RFC 5389 - 15.11.  ALTERNATE-SERVER */
	case stun_alternate_server:
		{
			attribute = (tnet_stun_attribute_t *)tnet_stun_attribute_altserver_create(dataPtr, length);
			break;
		}

	/* draft-ietf-behave-turn-16 subclause 14 */
	case stun_channel_number:
	case stun_lifetime:
	case stun_reserved2:
	case stun_xor_peer_address:
	case stun_data:
	case stun_xor_relayed_address:
	case stun_even_port:
	case stun_requested_transport:
	case stun_dont_fragment:
	case stun_reserved3:
	case stun_reservation_token:
		{
			attribute = tnet_turn_attribute_deserialize(type, length, dataPtr, length);
			break;
		}

	default:
		//TSK_DEBUG_WARN("==> NOT IMPLEMENTED");
		break;
	}

	if(!attribute){
		/* Create default */
		attribute = tnet_stun_attribute_create();
	}
	
	/* Set common values (Do I need this ==> already set by the constructor). */	
	attribute->type = type;
	attribute->length = length;

	return attribute;
}

/**@ingroup tnet_stun_group
* Serializes a @ref tnet_stun_attribute_t objet in binary format.
* @param attribute The STUN attribute to serialize.
* @param output The output binary buffer.
* @retval Zero if succeed and non-zero error code otherwise.
*/
int tnet_stun_attribute_serialize(const tnet_stun_attribute_t* attribute, tsk_buffer_t *output)
{
	if(!attribute || !output){
		return -1;
	}

	/* Attribute Type 
	*/
	{
		uint16_t type = tnet_htons(attribute->type);
		tsk_buffer_append(output, &(type), 2);
	}
	
	/* Attribute Length
	*/
	{
		uint16_t length = tnet_htons(attribute->length);
		tsk_buffer_append(output, &(length), 2);
	}
	
	/* Attribute Value
	*/
	
	switch(attribute->type){
	/* RFC 5389 - 15.1.  MAPPED-ADDRESS */
	case stun_mapped_address:
		{
			TSK_DEBUG_ERROR("NOT IMPLEMENTED");
			return -3;
		}

	/* RFC 5389 -  15.2.  XOR-MAPPED-ADDRESS*/
	case stun_xor_mapped_address:
		{
			TSK_DEBUG_ERROR("NOT IMPLEMENTED");
			return -3;
		}

	/* RFC 5389 -  15.3.  USERNAME*/
	case stun_username:
		{
			tnet_stun_attribute_username_t *username = (tnet_stun_attribute_username_t*)attribute;
			tsk_buffer_append(output, username->value, tsk_strlen(username->value));
			return 0;
		}


	/* RFC 5389 -  MESSAGE-INTEGRITY*/
	case stun_message_integrity:
		{
			tnet_stun_attribute_integrity_t *integrity = (tnet_stun_attribute_integrity_t*)attribute;
			tsk_buffer_append(output, integrity->sha1digest, TSK_SHA1_DIGEST_SIZE);
			return 0;
		}

		/* RFC 5389 -  15.5.  FINGERPRINT*/
	case stun_fingerprint:
		{
			uint32_t fingerprint = /*tnet_htonl*/(((tnet_stun_attribute_fingerprint_t*)attribute)->value);
			tsk_buffer_append(output, &fingerprint, 4);
			return 0;
		}

	/* RFC 5389 -  15.6.  ERROR-CODE*/
	case stun_error_code:
		{
			TSK_DEBUG_ERROR("NOT IMPLEMENTED");
			return -3;
		}

	/* RFC 5389 -  15.7.  REALM*/
	case stun_realm:
		{
			tnet_stun_attribute_realm_t *realm = (tnet_stun_attribute_realm_t*)attribute;
			tsk_buffer_append(output, realm->value, tsk_strlen(realm->value));
			return 0;
		}

	/* RFC 5389 -  15.8.  NONCE*/
	case stun_nonce:
		{
			tnet_stun_attribute_nonce_t *nonce = (tnet_stun_attribute_nonce_t*)attribute;
			tsk_buffer_append(output, nonce->value, tsk_strlen(nonce->value));
			return 0;
		}

	/* RFC 5389 -  15.9.  UNKNOWN-ATTRIBUTES*/
	case stun_unknown_attributes:
		{
			TSK_DEBUG_ERROR("NOT IMPLEMENTED");
			return -3;
		}

	/*	RFC 5389 - 15.10.  SOFTWARE */
	case stun_software:
		{
			tnet_stun_attribute_software_t *software = (tnet_stun_attribute_software_t*)attribute;
			tsk_buffer_append(output, software->value, tsk_strlen(software->value));
			return 0;
		}

	/*	RFC 5389 - 15.11.  ALTERNATE-SERVER */
	case stun_alternate_server:
		{
			TSK_DEBUG_ERROR("NOT IMPLEMENTED");
			return -3;
		}
	/* draft-ietf-behave-turn-16 - */
	case stun_channel_number:
	case stun_lifetime:
	case stun_reserved2:
	case stun_xor_peer_address:
	case stun_data:
	case stun_xor_relayed_address:
	case stun_even_port:
	case stun_requested_transport:
	case stun_dont_fragment:
	case stun_reserved3:
	case stun_reservation_token:
		{
			return tnet_turn_attribute_serialize(attribute, output);
		}

	default:
		return -2;
	}
}

/**@ingroup tnet_stun_group
* Pads a STUN attribute to align it on 4 octets.
* @param attribute The STUN attribute to pad.
* @param output The output buffer into which to put zeros.
*/
void tnet_stun_attribute_pad(const tnet_stun_attribute_t* attribute, tsk_buffer_t *output)
{
	if(attribute->length%4){
		static uint32_t zeros = 0x00000000;
		tsk_buffer_append(output, &zeros, 4-(attribute->length%4));
	}
}




//=================================================================================================
//	[[RFC 5389 - 15.  STUN Attributes]] object definition
//
static tsk_object_t* tnet_stun_attribute_ctor(tsk_object_t * self, va_list * app)
{
	tnet_stun_attribute_t *attribute = self;
	if(attribute){
	}
	return self;
}

static tsk_object_t* tnet_stun_attribute_dtor(tsk_object_t * self) 
{ 
	tnet_stun_attribute_t *attribute = self;
	if(attribute){
	}
	return self;
}

static const tsk_object_def_t tnet_stun_attribute_def_s =
{
	sizeof(tnet_stun_attribute_t),
	tnet_stun_attribute_ctor,
	tnet_stun_attribute_dtor,
	tsk_null,
};
const tsk_object_def_t *tnet_stun_attribute_def_t = &tnet_stun_attribute_def_s;


//=================================================================================================
//	[[RFC 5389 - 15.1.  MAPPED-ADDRESS]] object definition
//
static tsk_object_t* tnet_stun_attribute_mapped_addr_ctor(tsk_object_t * self, va_list * app)
{
	tnet_stun_attribute_mapped_addr_t *attribute = self;
	if(attribute){
		const void *payload = va_arg(*app, const void*);
		tsk_size_t payload_size = va_arg(*app, tsk_size_t);

		if(payload && payload_size){
			const uint8_t *payloadPtr = (const uint8_t*)payload;
			payloadPtr += 1; /* Ignore first 8bits */

			TNET_STUN_ATTRIBUTE(attribute)->type = stun_mapped_address;
			TNET_STUN_ATTRIBUTE(attribute)->length = payload_size;

			attribute->family = (tnet_stun_addr_family_t) (*(payloadPtr++));
			attribute->port = tnet_ntohs_2(payloadPtr);
			payloadPtr+=2;

			{	/*=== Compute IP address */
				tsk_size_t addr_size = (attribute->family == stun_ipv6) ? 16 : (attribute->family == stun_ipv4 ? 4 : 0);
				if(addr_size){	
					tsk_size_t i;

					for(i=0; i<addr_size; i+=4){
						// ntohl() not needed : byte per byte to avoid endianness problem
						attribute->address[i] = payloadPtr[0],
							attribute->address[i+1] = payloadPtr[1],
							attribute->address[i+2] = payloadPtr[2],
							attribute->address[i+3] = payloadPtr[3];
						payloadPtr+=4;
					}
				}
				else{
					TSK_DEBUG_ERROR("UNKNOWN FAMILY [%u].", attribute->family);
				}
			}
		}
	}
	return self;
}

static tsk_object_t* tnet_stun_attribute_mapped_addr_dtor(tsk_object_t * self) 
{ 
	tnet_stun_attribute_mapped_addr_t *attribute = self;
	if(attribute){
	}
	return self;
}

static const tsk_object_def_t tnet_stun_attribute_mapped_addr_def_s =
{
	sizeof(tnet_stun_attribute_mapped_addr_t),
	tnet_stun_attribute_mapped_addr_ctor,
	tnet_stun_attribute_mapped_addr_dtor,
	tsk_null,
};
const tsk_object_def_t *tnet_stun_attribute_mapped_addr_def_t = &tnet_stun_attribute_mapped_addr_def_s;

//=================================================================================================
//	[[RFC 5389 - 15.2.  XOR-MAPPED-ADDRESS]] object definition
//
static tsk_object_t* tnet_stun_attribute_xmapped_addr_ctor(tsk_object_t * self, va_list * app)
{
	tnet_stun_attribute_xmapped_addr_t *attribute = self;
	if(attribute){
		const void *payload = va_arg(*app, const void*);
		tsk_size_t payload_size = va_arg(*app, tsk_size_t);

		if(payload && payload_size){
			const uint8_t *payloadPtr = (const uint8_t*)payload;
			payloadPtr += 1; /* Ignore first 8bits */

			TNET_STUN_ATTRIBUTE(attribute)->type = stun_xor_mapped_address;
			TNET_STUN_ATTRIBUTE(attribute)->length = payload_size;
			
			attribute->family = (tnet_stun_addr_family_t)(*(payloadPtr++));

			/*	RFC 5389 - 15.2.  XOR-MAPPED-ADDRESS
				X-Port is computed by taking the mapped port in host byte order,
				XOR'ing it with the most significant 16 bits of the magic cookie, and
				then the converting the result to network byte order.
			*/
			attribute->xport = tnet_ntohs_2(payloadPtr);
			attribute->xport ^= 0x2112;
			payloadPtr+=2;
			
			
			{	/*=== Compute IP address */

				/*	RFC 5389 - 15.2.  XOR-MAPPED-ADDRESS
					If the IP address family is IPv4, X-Address is computed by taking the mapped IP
					address in host byte order, XOR'ing it with the magic cookie, and
					converting the result to network byte order.
				*/
				tsk_size_t addr_size = (attribute->family == stun_ipv6) ? 16 : (attribute->family == stun_ipv4 ? 4 : 0);
				if(addr_size){	
					tsk_size_t i;
					uint32_t addr;

					for(i=0; i<addr_size; i+=4){
						addr = tnet_ntohl(tnet_ntohl_2(payloadPtr) ^ TNET_STUN_MAGIC_COOKIE);
						memcpy(&attribute->xaddress[i], &addr, 4);
						payloadPtr+=4;
					}
				}
				else{
					TSK_DEBUG_ERROR("UNKNOWN FAMILY [%u].", attribute->family);
				}
			}
		}
		
	}
	return self;
}

static tsk_object_t* tnet_stun_attribute_xmapped_addr_dtor(tsk_object_t * self) 
{ 
	tnet_stun_attribute_xmapped_addr_t *attribute = self;
	if(attribute){
	}
	return self;
}

static const tsk_object_def_t tnet_stun_attribute_xmapped_addr_def_s =
{
	sizeof(tnet_stun_attribute_xmapped_addr_t),
	tnet_stun_attribute_xmapped_addr_ctor,
	tnet_stun_attribute_xmapped_addr_dtor,
	tsk_null,
};
const tsk_object_def_t *tnet_stun_attribute_xmapped_addr_def_t = &tnet_stun_attribute_xmapped_addr_def_s;


//=================================================================================================
//	[[RFC 5389 - 15.3.  USERNAME]] object definition
//
static tsk_object_t* tnet_stun_attribute_username_ctor(tsk_object_t * self, va_list * app)
{
	tnet_stun_attribute_username_t *attribute = self;
	if(attribute){
		const void *payload = va_arg(*app, const void*);
		tsk_size_t payload_size = va_arg(*app, tsk_size_t);

		attribute->value = tsk_strndup(payload, payload_size);

		TNET_STUN_ATTRIBUTE(attribute)->type = stun_username;
		TNET_STUN_ATTRIBUTE(attribute)->length = payload_size;
	}
	return self;
}

static tsk_object_t* tnet_stun_attribute_username_dtor(tsk_object_t * self) 
{ 
	tnet_stun_attribute_username_t *attribute = self;
	if(attribute){
		TSK_FREE(attribute->value);
	}
	return self;
}

static const tsk_object_def_t tnet_stun_attribute_username_def_s =
{
	sizeof(tnet_stun_attribute_username_t),
	tnet_stun_attribute_username_ctor,
	tnet_stun_attribute_username_dtor,
	tsk_null,
};
const tsk_object_def_t *tnet_stun_attribute_username_def_t = &tnet_stun_attribute_username_def_s;


//=================================================================================================
//	[[RFC 5389 - 15.4.  MESSAGE-INTEGRITY]] object definition
//
static tsk_object_t* tnet_stun_attribute_integrity_ctor(tsk_object_t * self, va_list * app)
{
	tnet_stun_attribute_integrity_t *attribute = self;
	if(attribute){
		const void *payload = va_arg(*app, const void*);
		tsk_size_t payload_size = va_arg(*app, tsk_size_t);
		
		if(payload_size == TSK_SHA1_DIGEST_SIZE){
			memcpy(attribute->sha1digest, payload, TSK_SHA1_DIGEST_SIZE);

			TNET_STUN_ATTRIBUTE(attribute)->type = stun_message_integrity;
			TNET_STUN_ATTRIBUTE(attribute)->length = TSK_SHA1_DIGEST_SIZE;
		}
	}
	return self;
}

static tsk_object_t* tnet_stun_attribute_integrity_dtor(tsk_object_t * self) 
{ 
	tnet_stun_attribute_integrity_t *attribute = self;
	if(attribute){
	}
	return self;
}

static const tsk_object_def_t tnet_stun_attribute_integrity_def_s =
{
	sizeof(tnet_stun_attribute_integrity_t),
	tnet_stun_attribute_integrity_ctor,
	tnet_stun_attribute_integrity_dtor,
	tsk_null,
};
const tsk_object_def_t *tnet_stun_attribute_integrity_def_t = &tnet_stun_attribute_integrity_def_s;


//=================================================================================================
//	[[RFC 5389 - 15.5.  FINGERPRINT]] object definition
//
static tsk_object_t* tnet_stun_attribute_fingerprint_ctor(tsk_object_t * self, va_list * app)
{
	tnet_stun_attribute_fingerprint_t *attribute = self;
	if(attribute){
		attribute->value = va_arg(*app, uint32_t);
		
		TNET_STUN_ATTRIBUTE(attribute)->type = stun_fingerprint;
		TNET_STUN_ATTRIBUTE(attribute)->length = 4;
	}
	return self;
}

static tsk_object_t* tnet_stun_attribute_fingerprint_dtor(tsk_object_t * self) 
{ 
	tnet_stun_attribute_fingerprint_t *attribute = self;
	if(attribute){
	}
	return self;
}

static const tsk_object_def_t tnet_stun_attribute_fingerprint_def_s =
{
	sizeof(tnet_stun_attribute_fingerprint_t),
	tnet_stun_attribute_fingerprint_ctor,
	tnet_stun_attribute_fingerprint_dtor,
	tsk_null,
};
const tsk_object_def_t *tnet_stun_attribute_fingerprint_def_t = &tnet_stun_attribute_fingerprint_def_s;



//=================================================================================================
//	[[RFC 5389 - 15.6.  ERROR-CODE]] object definition
//
static tsk_object_t* tnet_stun_attribute_errorcode_ctor(tsk_object_t * self, va_list * app)
{
	tnet_stun_attribute_errorcode_t *attribute = self;
	if(attribute){
		const uint8_t *payload = (const uint8_t*)va_arg(*app, const void*);
		tsk_size_t payload_size = va_arg(*app, tsk_size_t);

		if(payload_size >4){
			uint32_t code = tnet_htonl_2(payload);
			payload += 4;

			attribute->_class = code >>8;
			attribute->number = (code & 0xFF);
			attribute->reason_phrase = tsk_strndup((const char*)payload, (payload_size-4));
		}
		
		TNET_STUN_ATTRIBUTE(attribute)->type = stun_error_code;
		TNET_STUN_ATTRIBUTE(attribute)->length = payload_size;
	}
	return self;
}

static tsk_object_t* tnet_stun_attribute_errorcode_dtor(tsk_object_t * self) 
{ 
	tnet_stun_attribute_errorcode_t *attribute = self;
	if(attribute){
		TSK_FREE(attribute->reason_phrase);
	}
	return self;
}

static const tsk_object_def_t tnet_stun_attribute_errorcode_def_s =
{
	sizeof(tnet_stun_attribute_errorcode_t),
	tnet_stun_attribute_errorcode_ctor,
	tnet_stun_attribute_errorcode_dtor,
	tsk_null,
};
const tsk_object_def_t *tnet_stun_attribute_errorcode_def_t = &tnet_stun_attribute_errorcode_def_s;


//=================================================================================================
//	[[RFC 5389 - 15.7.  REALM]] object definition
//
static tsk_object_t* tnet_stun_attribute_realm_ctor(tsk_object_t * self, va_list * app)
{
	tnet_stun_attribute_realm_t *attribute = self;
	if(attribute){
		const void *payload = va_arg(*app, const void*);
		tsk_size_t payload_size = va_arg(*app, tsk_size_t);

		attribute->value = tsk_strndup(payload, payload_size);

		TNET_STUN_ATTRIBUTE(attribute)->type = stun_realm;
		TNET_STUN_ATTRIBUTE(attribute)->length = payload_size;
	}
	return self;
}

static tsk_object_t* tnet_stun_attribute_realm_dtor(tsk_object_t * self) 
{ 
	tnet_stun_attribute_realm_t *attribute = self;
	if(attribute){
		TSK_FREE(attribute->value);
	}
	return self;
}

static const tsk_object_def_t tnet_stun_attribute_realm_def_s =
{
	sizeof(tnet_stun_attribute_realm_t),
	tnet_stun_attribute_realm_ctor,
	tnet_stun_attribute_realm_dtor,
	tsk_null,
};
const tsk_object_def_t *tnet_stun_attribute_realm_def_t = &tnet_stun_attribute_realm_def_s;


//=================================================================================================
//	[[RFC 5389 - 15.8.  NONCE]] object definition
//
static tsk_object_t* tnet_stun_attribute_nonce_ctor(tsk_object_t * self, va_list * app)
{
	tnet_stun_attribute_nonce_t *attribute = self;
	if(attribute){
		const void *payload = va_arg(*app, const void*);
		tsk_size_t payload_size = va_arg(*app, tsk_size_t);

		attribute->value = tsk_strndup(payload, payload_size);

		TNET_STUN_ATTRIBUTE(attribute)->type = stun_nonce;
		TNET_STUN_ATTRIBUTE(attribute)->length = payload_size;
	}
	return self;
}

static tsk_object_t* tnet_stun_attribute_nonce_dtor(tsk_object_t * self) 
{ 
	tnet_stun_attribute_nonce_t *attribute = self;
	if(attribute){
		TSK_FREE(attribute->value);
	}
	return self;
}

static const tsk_object_def_t tnet_stun_attribute_nonce_def_s =
{
	sizeof(tnet_stun_attribute_nonce_t),
	tnet_stun_attribute_nonce_ctor,
	tnet_stun_attribute_nonce_dtor,
	tsk_null,
};
const tsk_object_def_t *tnet_stun_attribute_nonce_def_t = &tnet_stun_attribute_nonce_def_s;


//=================================================================================================
//	[[RFC 5389 - 15.9.  UNKNOWN-ATTRIBUTES]] object definition
//
static tsk_object_t* tnet_stun_attribute_unknowns_ctor(tsk_object_t * self, va_list * app)
{
	tnet_stun_attribute_unknowns_t *attribute = self;
	if(attribute){
		//--const void *payload = va_arg(*app, const void*);
		//--tsk_size_t payload_size = va_arg(*app, tsk_size_t);

		TNET_STUN_ATTRIBUTE(attribute)->type = stun_unknown_attributes;
		attribute->value = tsk_buffer_create_null();
	}
	return self;
}

static tsk_object_t* tnet_stun_attribute_unknowns_dtor(tsk_object_t * self) 
{ 
	tnet_stun_attribute_unknowns_t *attribute = self;
	if(attribute){
		TSK_OBJECT_SAFE_FREE(attribute->value);
	}
	return self;
}

static const tsk_object_def_t tnet_stun_attribute_unknowns_def_s =
{
	sizeof(tnet_stun_attribute_unknowns_t),
	tnet_stun_attribute_unknowns_ctor,
	tnet_stun_attribute_unknowns_dtor,
	tsk_null,
};
const tsk_object_def_t *tnet_stun_attribute_unknowns_def_t = &tnet_stun_attribute_unknowns_def_s;

//=================================================================================================
//	[[RFC 5389 - 15.10.  SOFTWARE]] object definition
//
static tsk_object_t* tnet_stun_attribute_software_ctor(tsk_object_t * self, va_list * app)
{
	tnet_stun_attribute_software_t *attribute = self;
	if(attribute){
		const void *payload = va_arg(*app, const void*);
		tsk_size_t payload_size = va_arg(*app, tsk_size_t);

		TNET_STUN_ATTRIBUTE(attribute)->type = stun_software;

		attribute->value = tsk_strndup(payload, payload_size);
		TNET_STUN_ATTRIBUTE(attribute)->length = tsk_strlen(attribute->value);
	}
	return self;
}

static tsk_object_t* tnet_stun_attribute_software_dtor(tsk_object_t * self) 
{ 
	tnet_stun_attribute_software_t *attribute = self;
	if(attribute){
		TSK_FREE(attribute->value);
	}
	return self;
}

static const tsk_object_def_t tnet_stun_attribute_software_def_s =
{
	sizeof(tnet_stun_attribute_software_t),
	tnet_stun_attribute_software_ctor,
	tnet_stun_attribute_software_dtor,
	tsk_null,
};
const tsk_object_def_t *tnet_stun_attribute_software_def_t = &tnet_stun_attribute_software_def_s;

//=================================================================================================
//	[[RFC 5389 - 15.11.  ALTERNATE-SERVER]] object definition
//
static tsk_object_t* tnet_stun_attribute_altserver_ctor(tsk_object_t * self, va_list * app)
{
	tnet_stun_attribute_altserver_t *attribute = self;
	if(attribute){
		const void *payload = va_arg(*app, const void*);
		tsk_size_t payload_size = va_arg(*app, tsk_size_t);

		const uint8_t *payloadPtr = (const uint8_t*)payload;
		payloadPtr += 1; /* Ignore first 8bits */

		TNET_STUN_ATTRIBUTE(attribute)->type = stun_alternate_server;
		TNET_STUN_ATTRIBUTE(attribute)->length = payload_size;

		attribute->family = (tnet_stun_addr_family_t) (*(payloadPtr++));
		attribute->port = tnet_ntohs_2(payloadPtr);
		payloadPtr+=2;

		if(attribute->family == stun_ipv4){
			uint32_t addr = tnet_htonl_2(payloadPtr);
			memcpy(attribute->server, &addr, 4);
			payloadPtr+=4;
		}
		else if(attribute->family == stun_ipv6){
			TSK_DEBUG_ERROR("IPv6 not supported yet.");
		}
		else{
			TSK_DEBUG_ERROR("UNKNOWN FAMILY.");
		}
	}
	return self;
}

static tsk_object_t* tnet_stun_attribute_altserver_dtor(tsk_object_t * self) 
{ 
	tnet_stun_attribute_altserver_t *attribute = self;
	if(attribute){
	}
	return self;
}

static const tsk_object_def_t tnet_stun_attribute_altserver_def_s =
{
	sizeof(tnet_stun_attribute_altserver_t),
	tnet_stun_attribute_altserver_ctor,
	tnet_stun_attribute_altserver_dtor,
	tsk_null,
};
const tsk_object_def_t *tnet_stun_attribute_altserver_def_t = &tnet_stun_attribute_altserver_def_s;

