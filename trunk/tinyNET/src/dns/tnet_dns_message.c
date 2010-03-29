/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)yahoo.fr>
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
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tnet_dns_message.h"

#include "../tnet_utils.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_debug.h"

#include <string.h>

/**@ingroup tnet_dns_group
* Serializes a STUN message in binary data.
* @param message The STUN message to seriablize.
* @retval The binary data representing the message if succeed.
* Otherwise, NULL is returned.
* @sa tnet_dns_message_deserialize.
*/
tsk_buffer_t* tnet_dns_message_serialize(const tnet_dns_message_t *message)
{
	tsk_buffer_t* output = 0;
	uint16_t _2bytes;
	tsk_list_item_t *item;

	/* Check message validity */
	if(!message)
	{
		goto bail;
	}

	output = TSK_BUFFER_CREATE_NULL();

	/* ==============================
	*	HEADER
	*/
	//tsk_buffer_append(output, &(message->Header), sizeof(message->Header));
	
	/* ID */
	_2bytes = ntohs(message->Header.ID);
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

		_2bytes = ntohs(_2bytes);
		tsk_buffer_append(output, &(_2bytes), 2);
	}
	/* QDCOUNT */
	_2bytes = ntohs(message->Header.QDCOUNT);
	tsk_buffer_append(output, &(_2bytes), 2);
	/* ANCOUNT */
	_2bytes = ntohs(message->Header.ANCOUNT);
	tsk_buffer_append(output, &(_2bytes), 2);
	/* NSCOUNT */
	_2bytes = ntohs(message->Header.NSCOUNT);
	tsk_buffer_append(output, &(_2bytes), 2);
	/* ARCOUNT */
	_2bytes = ntohs(message->Header.ARCOUNT);
	tsk_buffer_append(output, &(_2bytes), 2);


	/* ==============================
	*	QUESTION
	*/
	if(TNET_DNS_MESSAGE_IS_QUERY(message))
	{
		/* QNAME */
		tnet_dns_rr_qname_serialize(message->Question.QNAME, output);
		/* QTYPE */
		_2bytes = ntohs(message->Question.QTYPE);
		tsk_buffer_append(output, &(_2bytes), 2);
		/* QCLASS */
		_2bytes = ntohs(message->Question.QCLASS);
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
tnet_dns_message_t* tnet_dns_message_deserialize(const uint8_t *data, size_t size)
{
	tnet_dns_message_t *message = 0;
	uint8_t *dataPtr, *dataEnd, *dataStart;
	uint16_t i;
	size_t offset = 0;

	if(!data || !size)
	{
		goto bail;
	}

	dataPtr = (uint8_t*)data;
	dataStart = dataPtr;
	dataEnd = (dataStart + size);

	message = TNET_DNS_MESSAGE_CREATE_NULL();

	/* ==============================
	*	HEADER
	*/
	/* ID */
	message->Header.ID = ntohs(*((uint16_t*)dataPtr));
	dataPtr += 2;
	/* |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   | */
	{
		uint16_t flags = ntohs(*((uint16_t*)dataPtr));

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
	message->Header.QDCOUNT = ntohs(*((uint16_t*)dataPtr));
	dataPtr += 2;
	/* ANCOUNT */
	message->Header.ANCOUNT = ntohs(*((uint16_t*)dataPtr));
	dataPtr += 2;
	/* NSCOUNT */
	message->Header.NSCOUNT = ntohs(*((uint16_t*)dataPtr));
	dataPtr += 2;
	/* ARCOUNT */
	message->Header.ARCOUNT = ntohs(*((uint16_t*)dataPtr));
	dataPtr += 2;

	/* === Queries 
	*/
	offset = (dataPtr - dataStart);
	for(i=0; i<message->Header.QDCOUNT; i++)
	{
		/* Do not need to parse queries in the response ==> silently ignore */
		char* name = 0;
		tnet_dns_rr_qname_deserialize(dataStart, (dataEnd-dataPtr), &name, &offset); /* QNAME */
		dataPtr+=offset;
		dataPtr+=4, offset+=4; /* QTYPE + QCLASS */
		TSK_FREE(name);
	}
	dataPtr = (dataStart + offset); /* TODO: remove ==> obly for debug tests */

	/* === Answers 
	*/
	offset = (dataPtr - dataStart);
	for(i=0; i<message->Header.ANCOUNT; i++)
	{
		tnet_dns_rr_t* rr = tnet_dns_rr_deserialize(dataStart, (dataEnd-dataPtr), &offset);
		if(rr)
		{
			if(!message->Answers)
			{
				message->Answers = TSK_LIST_CREATE();
			}
			tsk_list_push_descending_data(message->Answers, (void**)&rr); /* Push filtered (Useful for NAPTR and SRV records). */
		}
	}
	dataPtr = (dataStart + offset);

	/* === Authorities 
	*/
	offset = (dataPtr - dataStart);
	for(i=0; i<message->Header.NSCOUNT; i++)
	{
		tnet_dns_rr_t* rr = tnet_dns_rr_deserialize(dataStart, (dataEnd-dataPtr), &offset);
		if(rr)
		{
			if(!message->Authorities)
			{
				message->Authorities = TSK_LIST_CREATE();
			}
			tsk_list_push_back_data(message->Authorities, (void**)&rr);
		}
	}
	dataPtr = (dataStart + offset);

	/* === Additionals 
	*/
	offset = (dataPtr - dataStart);
	for(i=0; i<message->Header.ARCOUNT; i++)
	{
		tnet_dns_rr_t* rr = tnet_dns_rr_deserialize(dataStart, (dataEnd-dataPtr), &offset);
		if(rr)
		{
			if(!message->Additionals)
			{
				message->Additionals = TSK_LIST_CREATE();
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
static void* tnet_dns_message_create(void * self, va_list * app)
{
	tnet_dns_message_t *message = self;
	if(message)
	{
		static uint16_t __dnsmessage_unique_id = 0;

		const char* qname = va_arg(*app, const char*);
		tnet_dns_qclass_t qclass = va_arg(*app, tnet_dns_qclass_t);
		tnet_dns_qtype_t qtype = va_arg(*app, tnet_dns_qtype_t);
		unsigned isquery = va_arg(*app, unsigned);

		/* Create random ID. */
		message->Header.ID = ++__dnsmessage_unique_id;
		
		/* QR field ==> query (0) - response (1) */
		message->Header.QR = isquery ? 0 : 1;
		
		if(isquery)
		{
			/* QDCOUNT field ==> at least one question */
			message->Header.QDCOUNT = 1;
		}
		
		if(qname)
		{
			message->Question.QNAME = tsk_strdup(qname);
			message->Question.QTYPE = qtype;
			message->Question.QCLASS = qclass;
		}
	}
	return self;
}

static void* tnet_dns_message_destroy(void * self) 
{ 
	tnet_dns_message_t *message = self;
	if(message)
	{
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
	tnet_dns_message_create,
	tnet_dns_message_destroy,
	0,
};
const void *tnet_dns_message_def_t = &tnet_dns_message_def_s;
