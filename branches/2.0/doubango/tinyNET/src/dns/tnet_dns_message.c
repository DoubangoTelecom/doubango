/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango[dot]org>
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
/**@file tnet_dns_message.h
 * @brief DNS Message holding RRs (RFC 1035).
 *
 * @author Mamadou Diop <diopmamadou(at)doubango[dot]org>
 *

 */
#include "tnet_dns_message.h"

#include "../tnet_utils.h"
#include "../tnet_endianness.h"

#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_debug.h"

#include <string.h>

/**@ingroup tnet_dns_group
* Creates new DNS message.
* @sa tnet_dns_message_create_null.
*/
tnet_dns_message_t* tnet_dns_message_create(const char* qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype, tsk_bool_t isquery)
{
	return tsk_object_new(tnet_dns_message_def_t, qname, qclass, qtype, isquery);
}

/**@ingroup tnet_dns_group
* Creates new DNS message.
* @sa tnet_dns_message_create.
*/
tnet_dns_message_t* tnet_dns_message_create_null()
{
	return tnet_dns_message_create(tsk_null, qclass_any, qtype_any, tsk_false);
}

/**@ingroup tnet_dns_group
* Creates new DNS response message.
* @sa tnet_dns_query_create.
*/
tnet_dns_response_t* tnet_dns_response_create(const char* qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype)
{
	return tnet_dns_message_create(qname, qclass, qtype, tsk_false);
}

/**@ingroup tnet_dns_group
* Creates new DNS query message.
* @sa tnet_dns_response_create.
*/
tnet_dns_query_t* tnet_dns_query_create(const char* qname, tnet_dns_qclass_t qclass, tnet_dns_qtype_t qtype)
{
	return tnet_dns_message_create(qname, qclass, qtype, tsk_true);
}

/**@ingroup tnet_dns_group
* Serializes a DNS message in binary data.
* @param message The DNS message to seriablize.
* @retval The binary buffer containong the message if succeed. Otherwise, @a tsk_null is returned.
* @sa tnet_dns_message_deserialize.
*/
tsk_buffer_t* tnet_dns_message_serialize(const tnet_dns_message_t *message)
{
	tsk_buffer_t* output = tsk_null;
	uint16_t _2bytes;
	tsk_list_item_t *item;

	/* Check message validity */
	if(!message){
		goto bail;
	}
	
	/* Creates empty buffer */
	output = tsk_buffer_create_null();

	/* ==============================
	*	HEADER
	*/
	//tsk_buffer_append(output, &(message->Header), sizeof(message->Header));
	
	/* ID */
	_2bytes = tnet_ntohs(message->Header.ID);
	tsk_buffer_append(output, &(_2bytes), 2);
	/* |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   | */
	{
		uint16_t temp, _2bytes = 0;

		temp = message->Header.QR, temp <<= 15;
		_2bytes |= temp;

		temp = message->Header.OPCODE, temp <<= 11;
		_2bytes |= temp;

		temp = message->Header.AA, temp <<= 10;
		_2bytes |= temp;

		temp = message->Header.TC, temp <<= 9;
		_2bytes |= temp;

		temp = message->Header.RD, temp <<= 8;
		_2bytes |= temp;

		temp = message->Header.RA, temp <<= 7;
		_2bytes |= temp;

		temp = message->Header.Z, temp <<= 4;
		_2bytes |= temp;

		temp = message->Header.RCODE, temp <<= 4;
		_2bytes |= temp;

		_2bytes = tnet_ntohs(_2bytes);
		tsk_buffer_append(output, &(_2bytes), 2);
	}
	/* QDCOUNT */
	_2bytes = tnet_ntohs(message->Header.QDCOUNT);
	tsk_buffer_append(output, &(_2bytes), 2);
	/* ANCOUNT */
	_2bytes = tnet_ntohs(message->Header.ANCOUNT);
	tsk_buffer_append(output, &(_2bytes), 2);
	/* NSCOUNT */
	_2bytes = tnet_ntohs(message->Header.NSCOUNT);
	tsk_buffer_append(output, &(_2bytes), 2);
	/* ARCOUNT */
	_2bytes = tnet_ntohs(message->Header.ARCOUNT);
	tsk_buffer_append(output, &(_2bytes), 2);


	/* ==============================
	*	QUESTION
	*/
	if(TNET_DNS_MESSAGE_IS_QUERY(message))
	{
		/* QNAME */
		tnet_dns_rr_qname_serialize(message->Question.QNAME, output);
		/* QTYPE */
		_2bytes = tnet_ntohs(message->Question.QTYPE);
		tsk_buffer_append(output, &(_2bytes), 2);
		/* QCLASS */
		_2bytes = tnet_ntohs(message->Question.QCLASS);
		tsk_buffer_append(output, &(_2bytes), 2);
	}

	/* ==============================
	*	ANSWERS
	*/
	tsk_list_foreach(item, message->Answers)
	{
		tnet_dns_rr_serialize((tnet_dns_rr_t *)item->data, output);
	}

	/* ==============================
	*	AUTHORITIES
	*/
	tsk_list_foreach(item, message->Authorities)
	{
		tnet_dns_rr_serialize((tnet_dns_rr_t *)item->data, output);
	}

	/* ==============================
	*	ADDITIONALS
	*/
	tsk_list_foreach(item, message->Additionals)
	{
		tnet_dns_rr_serialize((tnet_dns_rr_t *)item->data, output);
	}
	

bail:
	return output;
}

/**@ingroup tnet_dns_group
* Deserialize a STUN message from binary data.
* @param data A pointer to the binary data.
* @param size The size of the bnary data buffer.
* @retval @ref tnet_dns_message_t object if succeed or NULL otherwise.
* @sa @ref tnet_dns_message_serialize.
*/
tnet_dns_message_t* tnet_dns_message_deserialize(const uint8_t *data, tsk_size_t size)
{
	tnet_dns_message_t *message = 0;
	uint8_t *dataPtr, *dataEnd, *dataStart;
	uint16_t i;
	tsk_size_t offset = 0;

	if(!data || !size){
		goto bail;
	}

	dataPtr = (uint8_t*)data;
	dataStart = dataPtr;
	dataEnd = (dataStart + size);

	message = tnet_dns_message_create_null();

	/* === HEADER ===*/
	/* ID */
	message->Header.ID = tnet_ntohs_2(dataPtr);
	dataPtr += 2;
	/* |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   | */
	{
		uint16_t flags = tnet_ntohs_2(dataPtr);

		message->Header.QR = (flags >> 15);
		message->Header.OPCODE = ((flags >> 11) & 0x000F);
		message->Header.AA = ((flags >> 10) & 0x0001);
		message->Header.TC = ((flags >> 9) & 0x0001);
		message->Header.RD = ((flags >> 8) & 0x0001);
		message->Header.RA = ((flags >> 7) & 0x0001);
		message->Header.Z = ((flags >> 4) & 0x0007);
		message->Header.RCODE = (flags & 0x000F);

		dataPtr += 2;
	}
	/* QDCOUNT */
	message->Header.QDCOUNT = tnet_ntohs_2(dataPtr);
	dataPtr += 2;
	/* ANCOUNT */
	message->Header.ANCOUNT = tnet_ntohs_2(dataPtr);
	dataPtr += 2;
	/* NSCOUNT */
	message->Header.NSCOUNT = tnet_ntohs_2(dataPtr);
	dataPtr += 2;
	/* ARCOUNT */
	message->Header.ARCOUNT = tnet_ntohs_2(dataPtr);
	dataPtr += 2;

	/* === Queries ===*/
	offset = (dataPtr - dataStart);
	for(i=0; i<message->Header.QDCOUNT; i++)
	{
		/* Do not need to parse queries in the response ==> silently ignore */
		char* name = 0;
		tnet_dns_rr_qname_deserialize(dataStart, &name, &offset); /* QNAME */
		dataPtr+=offset;
		dataPtr+=4, offset+=4; /* QTYPE + QCLASS */
		TSK_FREE(name);
	}
	dataPtr = (dataStart + offset); /* TODO: remove ==> obly for debug tests */

	/* === Answers ===*/
	offset = (dataPtr - dataStart);
	for(i=0; i<message->Header.ANCOUNT; i++)
	{
		tnet_dns_rr_t* rr = tnet_dns_rr_deserialize(dataStart, (dataEnd-dataPtr), &offset);
		if(rr){
			if(!message->Answers){
				message->Answers = tsk_list_create();
			}
			/* Push in descending order (useful for NAPTR and SRV records). */
			tsk_list_push_descending_data(message->Answers, (void**)&rr);
		}
	}
	dataPtr = (dataStart + offset);

	/* === Authorities ===*/
	offset = (dataPtr - dataStart);
	for(i=0; i<message->Header.NSCOUNT; i++)
	{
		tnet_dns_rr_t* rr = tnet_dns_rr_deserialize(dataStart, (dataEnd-dataPtr), &offset);
		if(rr){
			if(!message->Authorities){
				message->Authorities = tsk_list_create();
			}
			tsk_list_push_back_data(message->Authorities, (void**)&rr);
		}
	}
	dataPtr = (dataStart + offset);

	/* === Additionals ===*/
	offset = (dataPtr - dataStart);
	for(i=0; i<message->Header.ARCOUNT; i++)
	{
		tnet_dns_rr_t* rr = tnet_dns_rr_deserialize(dataStart, (dataEnd-dataPtr), &offset);
		if(rr){
			if(!message->Additionals){
				message->Additionals = tsk_list_create();
			}
			tsk_list_push_back_data(message->Additionals, (void**)&rr);
		}
	}
	dataPtr = (dataStart + offset);
	

bail:
	return message;
}

//=================================================================================================
//	[[DNS MESSAGE]] object definition
//
static tsk_object_t* tnet_dns_message_ctor(tsk_object_t * self, va_list * app)
{
	tnet_dns_message_t *message = self;
	if(message){
		static uint16_t __dnsmessage_unique_id = 0;

		const char* qname = va_arg(*app, const char*);
		tnet_dns_qclass_t qclass = va_arg(*app, tnet_dns_qclass_t);
		tnet_dns_qtype_t qtype = va_arg(*app, tnet_dns_qtype_t);
		tsk_bool_t isquery = va_arg(*app, tsk_bool_t);

		/* Create random ID. */
		message->Header.ID = ++__dnsmessage_unique_id;
		
		/* QR field ==> query (0) - response (1) */
		message->Header.QR = isquery ? 0 : 1;
		
		if(isquery){
			/* QDCOUNT field ==> at least one question */
			message->Header.QDCOUNT = 1;
		}
		
		if(qname){
			message->Question.QNAME = tsk_strdup(qname);
			message->Question.QTYPE = qtype;
			message->Question.QCLASS = qclass;
		}
	}
	return self;
}

static tsk_object_t* tnet_dns_message_dtor(tsk_object_t * self) 
{ 
	tnet_dns_message_t *message = self;
	if(message){
		TSK_FREE(message->Question.QNAME);
		
		TSK_OBJECT_SAFE_FREE(message->Answers);
		TSK_OBJECT_SAFE_FREE(message->Authorities);
		TSK_OBJECT_SAFE_FREE(message->Additionals);
	}
	return self;
}

static const tsk_object_def_t tnet_dns_message_def_s =
{
	sizeof(tnet_dns_message_t),
	tnet_dns_message_ctor,
	tnet_dns_message_dtor,
	tsk_null,
};
const tsk_object_def_t *tnet_dns_message_def_t = &tnet_dns_message_def_s;
