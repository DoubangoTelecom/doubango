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

/**@file tcomp_message.c
 * @brief  SIGCOMP message as per RFC 3320 subclause 7.
 * A message sent from the compressor dispatcher to the decompressordispatcher.  In case of a message-based transport such as UDP, a
 * SigComp message corresponds to exactly one datagram.  For a stream-based transport such as TCP, the SigComp messages are
 * separated by reserved delimiters.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */
#include "tcomp_message.h"
#include "tcomp_nack_codes.h"

#include "tsk_memory.h"
#include "tsk_debug.h"
#include "tsk_binaryutils.h"
#include "tsk_sha1.h"

#include <string.h>

#define MIN_LEN 2
#define HEADER_GET_LEN(message)		(message->headerSigComp & 0x03)
#define HEADER_GET_T(message)		(message->headerSigComp & 0x04)
#define HEADER_IS_VALID(message)	(message->headerSigComp >= 0xf8)

#define HEADER_GET_DEST_VALUE(destination) ( sigcomp_encoding_destination[destination] )
#define HEADER_GET_STATE_LENGTH(length) ( sigcomp_encoding_partial_id_length[length] )

static void initFeedbackItem(tcomp_message_t *message, uint8_t** start_ptr);
static void initStateId(tcomp_message_t *message, uint8_t** start_ptr, uint8_t state_len);
static void initStateful(tcomp_message_t *message, uint8_t** start_ptr, uint8_t* end_ptr);
static void initStateless(tcomp_message_t *message, uint8_t** start_ptr, uint8_t* end_ptr, int32_t *nack_code);
static void initNack(tcomp_message_t *message, uint8_t** start_ptr, uint8_t* end_ptr, int32_t* nack_code);

/* 
Creates new SigComp message.
*/
tcomp_message_t* tcomp_message_create(const void* input_ptr, tsk_size_t input_size, tsk_bool_t stream, int32_t* nack_code)
{
	tcomp_message_t *message;

	if(!nack_code){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}

	if(!input_ptr){
		TSK_DEBUG_ERROR("Invalid parameter");
		*nack_code = NACK_INTERNAL_ERROR;
		return tsk_null;
	}

	if(input_size < MIN_LEN){
		TSK_DEBUG_ERROR("MESSAGE_TOO_SHORT");
		*nack_code = NACK_MESSAGE_TOO_SHORT;
		return tsk_null;
	}
	
	if((message = tsk_object_new(tcomp_message_def_t))){
		uint8_t *dummy_ptr, *end_ptr;
		uint8_t state_len;

		message->startPtr = input_ptr;
		message->stateId = tcomp_buffer_create_null();
		message->remaining_sigcomp_buffer = tcomp_buffer_create_null();
		message->uploaded_UDVM_buffer = tcomp_buffer_create_null();
		message->ret_feedback_buffer= tcomp_buffer_create_null();
		
		message->isNack = 0;
		dummy_ptr = ((uint8_t*)input_ptr);
		end_ptr = (dummy_ptr + input_size);

		// 
		message->totalSize = input_size;
		message->stream_based = stream;
		message->bytecodes_destination = 0;

		/* Get sigcomp header */
		message->headerSigComp = *dummy_ptr;
		dummy_ptr++;

		/* Check message validity --> magic code (11111)? */
		message->isOK = HEADER_IS_VALID(message);
		if(!message->isOK){
			TSK_DEBUG_ERROR("SigComp Message not valid (magic code mismatch)");
			*nack_code = NACK_INTERNAL_ERROR;
			goto bail;
		}
		
		/* Feedback item */
		if((HEADER_GET_T(message)!=0)){
			initFeedbackItem(message, &dummy_ptr);
			if(!message->isOK){
				goto bail;
			}
		}

		/*
		* If the len field is non-zero, then the SigComp message contains a state identifier 
		* to access a state item at the receiving endpoint.
		*/
		state_len = HEADER_GET_STATE_LENGTH( HEADER_GET_LEN(message) );
		if(state_len){
			initStateId(message, &dummy_ptr, state_len);
			initStateful(message, &dummy_ptr, end_ptr);
			TSK_DEBUG_INFO("SigComp - Decompressing stateful message with state id =");
			tcomp_buffer_print(message->stateId);
		}
		else
		{
			if( !*dummy_ptr && !(*(dummy_ptr+1)&0xf0) ){
				// "code_len" field of zero --> it's a nack
				initNack(message, &dummy_ptr, end_ptr, nack_code);
			}
			else{
				initStateless(message, &dummy_ptr, end_ptr, nack_code);
			}
		}

		/*
		* The fields (RFC 3320 section 7) except for the "remaining SigComp message" are referred to
		* as the "SigComp header" (note that this may include the uploaded UDVM bytecode).
		*/
		if(message->isOK){
			message->header_size = ( message->totalSize - tcomp_buffer_getSize(message->remaining_sigcomp_buffer));
		}
	}
	else{
		TSK_DEBUG_ERROR("Failed to create new SigComp message");
	}

bail:
	if(message && !message->isOK){
		TSK_OBJECT_SAFE_FREE(message);
	}

	return message;
}

/*
Iniatizes the feedback item field.
*/
static void initFeedbackItem(tcomp_message_t *message, uint8_t** start_ptr)
{
	/*
     0   1   2   3   4   5   6   7       0   1   2   3   4   5   6   7
   +---+---+---+---+---+---+---+---+   +---+---+---+---+---+---+---+---+
   | 0 |  returned_feedback_field  |   | 1 | returned_feedback_length  |
   +---+---+---+---+---+---+---+---+   +---+---+---+---+---+---+---+---+
                                       |                               |
                                       :    returned_feedback_field    :
                                       |                               |
                                       +---+---+---+---+---+---+---+---+
	*/
	if((**start_ptr) <= 128){
		tcomp_buffer_referenceBuff(message->ret_feedback_buffer, *start_ptr, 1);
		(void)(*start_ptr++);
	}
	else{
		tcomp_buffer_referenceBuff(message->ret_feedback_buffer, *start_ptr, 1+(**start_ptr&0x7f));
		*start_ptr += tcomp_buffer_getSize(message->ret_feedback_buffer);
	}
}

/*
Initializes the state identifier field.
*/
static void initStateId(tcomp_message_t *message, uint8_t** start_ptr, uint8_t state_len)
{
	tcomp_buffer_referenceBuff(message->stateId, *start_ptr, state_len);
	*start_ptr += state_len;
}

/*
Initializes a stateful SigComp message.
*/
static void initStateful(tcomp_message_t *message, uint8_t** start_ptr, uint8_t* end_ptr)
{
	/*
   +---+---+---+---+---+---+---+---+
   |                               |
   :   partial state identifier    :
   |                               |
   +---+---+---+---+---+---+---+---+
   |                               |
   :   remaining SigComp message   :
   |                               |
   +---+---+---+---+---+---+---+---+
	*/
	message->isOK &= (*start_ptr<=end_ptr);
	if(message->isOK){
		tcomp_buffer_referenceBuff(message->remaining_sigcomp_buffer, *start_ptr, 
							((end_ptr-*start_ptr)));
	}
}
	
/*
Initializes a stateless SigComp message.
*/
static void initStateless(tcomp_message_t *message, uint8_t** start_ptr, uint8_t* end_ptr, int32_t *nack_code)
{
	int has_bytecode = (HEADER_GET_LEN(message) == 0); // No state ==> message contains udvm bytecode
	message->isOK &= has_bytecode;
	if(!message->isOK) return;

	/*
  +---+---+---+---+---+---+---+---+
  |           code_len            |
  +---+---+---+---+---+---+---+---+
  |   code_len    |  destination  |
  +---+---+---+---+---+---+---+---+
  |                               |
  :    uploaded UDVM bytecode     :
  |                               |
  +---+---+---+---+---+---+---+---+
  |                               |
  :   remaining SigComp message   :
  |                               |
  +---+---+---+---+---+---+---+---+
  */
	{
		uint32_t code_len1, bytecodes_len;
		uint8_t code_len2, destination, *bytecodes_uploaded_udvm, *remaining_SigComp_message;

		uint8_t* dummy_ptr = ((uint8_t*)*start_ptr);

		/* Code_len --> 12bits [8+4] */
		code_len1 = *dummy_ptr; dummy_ptr++; /* skip first code_len 8bits */
		code_len2 = (*dummy_ptr) & 0xf0; /* code_len 4 remaining bits */
		destination = (*dummy_ptr) & 0x0f; /* 4bits after code_len */
		dummy_ptr++; /* skip code_len 4bits + destination 4bits ==> 1-byte */

		/* Get bytecodes length (12bits) */
		bytecodes_len = ( (code_len1<<4)|(code_len2>>4) );
		
		/* Starting memory address (code destination address). In UDVM. */
		message->bytecodes_destination = HEADER_GET_DEST_VALUE(destination); 
		if((message->bytecodes_destination < 128) || (message->bytecodes_destination > 1024)){
			TSK_DEBUG_ERROR("INVALID_CODE_LOCATION");
			*nack_code = NACK_INVALID_CODE_LOCATION;
			message->isOK = 0;
			return;
		}

		/* Uploaded UDVM pointer */
		bytecodes_uploaded_udvm = dummy_ptr; /* SigComp header, feedback_item, code_len and destination have been skipped */
		
		 /* Skip uploaded udvm */
		dummy_ptr += bytecodes_len;
		
		/* remaining SigComp message */
		remaining_SigComp_message = dummy_ptr;

		/* check that remaining sigcomp message is valide */
		if( !(message->isOK &= (remaining_SigComp_message <= end_ptr )) ){
			TSK_DEBUG_ERROR("MESSAGE_TOO_SHORT");
			*nack_code = NACK_MESSAGE_TOO_SHORT;
			return;
		}

		//
		// Setting buffers
		//
		tcomp_buffer_referenceBuff(message->uploaded_UDVM_buffer, bytecodes_uploaded_udvm, bytecodes_len);
		tcomp_buffer_referenceBuff(message->remaining_sigcomp_buffer, remaining_SigComp_message, ((end_ptr-remaining_SigComp_message)));
	}
}

/*
Initializes a NACK message as per RFC 4077.
*/
static void initNack(tcomp_message_t *message, uint8_t** start_ptr, uint8_t* end_ptr, int32_t* nack_code)
{
	/*
	+---+---+---+---+---+---+---+---+
	|         code_len = 0          |
	+---+---+---+---+---+---+---+---+
	| code_len = 0  |  version = 1  |
	+---+---+---+---+---+---+---+---+
	|          Reason Code          |
	+---+---+---+---+---+---+---+---+
	|  OPCODE of failed instruction |
	+---+---+---+---+---+---+---+---+
	|   PC of failed instruction    |
	|                               |
	+---+---+---+---+---+---+---+---+
	|                               |
	: SHA-1 Hash of failed message  :
	|                               |
	+---+---+---+---+---+---+---+---+
	|                               |
	:         Error Details         :
	|                               |
	+---+---+---+---+---+---+---+---+*/

	uint8_t* dummy_ptr;
	message->isNack = 1;
	if( (end_ptr - *start_ptr)<25 ){
		*nack_code = NACK_MESSAGE_TOO_SHORT;
		TSK_DEBUG_ERROR("MESSAGE_TOO_SHORT");
		message->isOK = 0;
		return;
	}

	dummy_ptr = ((uint8_t*)*start_ptr);
	dummy_ptr++; /* skip first code_len byte */
	if(!(message->isOK = (*dummy_ptr++ == NACK_VERSION))) {
		return;
	}

	if(!message->nack_info){
		message->nack_info = tcomp_nackinfo_create();
	}

	message->nack_info->reasonCode = *dummy_ptr++;
	message->nack_info->opcode = *dummy_ptr++;
	message->nack_info->pc = TSK_BINARY_GET_2BYTES(dummy_ptr); dummy_ptr+=2;
	memcpy(message->nack_info->sha1, dummy_ptr, TSK_SHA1_DIGEST_SIZE); dummy_ptr += TSK_SHA1_DIGEST_SIZE;
	if(dummy_ptr < end_ptr){
		/* Has error details */
		tcomp_buffer_appendBuff(message->nack_info->details, dummy_ptr, (end_ptr-dummy_ptr));
	}
}





//========================================================
//	SigComp message object definition
//

static tsk_object_t* tcomp_message_ctor(tsk_object_t *self, va_list * app)
{
	tcomp_message_t *message = self;

	if(message){
	}

	return self;
}

static tsk_object_t* tcomp_message_dtor(tsk_object_t *self)
{
	tcomp_message_t *message = self;

	if(message){
		TSK_OBJECT_SAFE_FREE(message->stateId);
		TSK_OBJECT_SAFE_FREE(message->remaining_sigcomp_buffer);
		TSK_OBJECT_SAFE_FREE(message->uploaded_UDVM_buffer);
		TSK_OBJECT_SAFE_FREE(message->ret_feedback_buffer);
		TSK_OBJECT_SAFE_FREE(message->nack_info);
	}
	else{
		TSK_DEBUG_WARN("NULL SigComp message.");
	}

	return self;
}

static const tsk_object_def_t tcomp_message_def_s = 
{
	sizeof(tcomp_message_t),
	tcomp_message_ctor, 
	tcomp_message_dtor,
	tsk_null
};
const tsk_object_def_t* tcomp_message_def_t = &tcomp_message_def_s;
