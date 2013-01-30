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

/**@file tcomp_compressor.h
 * @brief  SigComp Deflate compressor(Ghost).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */
#include "tcomp_deflatedata.h"

#include "tsk_binaryutils.h"
#include "tsk_debug.h"

#include <string.h>

/*
	|<----------------------------DMS--------------------------------->|
    |<-----SigComp message---->|<------------UDVM memory size--------->|
    +-+----------+-------------+-----+----------+----------------------+
    | | bytecode |  comp msg   |     | bytecode | circular buffer      |
    +-+----------+-------------+-----+----------+----------------------+
     ^                            ^
     |                            |
    SigComp header          Low bytes of UDVM

I suppose we would like to compress this message "libsigcomp":
	GHOST STRUCTURE:
	0x02bb	--> Circular Buffer size (DEFLATE_UDVM_CIRCULAR_START_INDEX = 699)
	0x2000	--> DMS (8192)
	0x0005	--> See 'libsigcomp/asm/deflate.asm' (MULTILOAD param number five)
	0x02c5	--> 709	(DEFLATE_UDVM_CIRCULAR_START_INDEX + strlen("libsigcomp"))
	......	--> Remaining MULTILOAD words
	0x0682	--> 1666 (HASH_LEN = SMS+8)
	0x067a	--> 1658 (SMS)
	0x1902	--> [CPB+DMS+SMS]+[SigComp version]
	------------------------------------------------------------
	0x0003	--> DEFLATE_NO_DICT
	......	--> Dictionnary words
	0x0000	--> FIXME
	0x0000	--> FIXME
	......	--> Deflate byte code (DeflateCompressor::deflate_bytecode)
	......	--> no compressed message
*/


#define GHOST_CB_START_INDEX				(0)
#define GHOST_DMS_INDEX						(GHOST_CB_START_INDEX + 2)
#define GHOST_0x0005_INDEX					(GHOST_DMS_INDEX + 2)
#define GHOST_CB_END_INDEX					(GHOST_0x0005_INDEX + 2)
#define GHOST_HASH_LEN_INDEX				(GHOST_CB_END_INDEX + 2 + 236)
#define GHOST_SMS_INDEX						(GHOST_HASH_LEN_INDEX + 2)
#define GHOST_CPB_DMS_SMS_INDEX				(GHOST_SMS_INDEX + 2)
#define GHOST_VERSION_INDEX					(GHOST_CPB_DMS_SMS_INDEX + 1)


#define GHOST_BYTECODE1_SIZE				(GHOST_VERSION_INDEX + 1)


static const char* DeflateData_deflate_bytecode1_ghost =
{
	"\xff\xff"	// Circular buffer Size (CBS)
	"\xff\xff"	// State Memory Size (DMS)
	"\x00\x05"	// See 'libsigcomp/asm/deflate.asm'
	"\xff\xff"	// (CBS + strlen(input))
	
	"\x00\x00\x00\x03\x00\x00\x00\x04\x00\x00\x00\x05\x00\x00\x00\x06\x00\x00\x00\x07\x00\x00\x00\x08\x00\x00\x00\x09\x00\x00"
	"\x00\x0a\x00\x01\x00\x0b\x00\x01\x00\x0d\x00\x01\x00\x0f\x00\x01\x00\x11\x00\x02\x00\x13\x00\x02\x00\x17\x00\x02\x00\x1b"
	"\x00\x02\x00\x1f\x00\x03\x00\x23\x00\x03\x00\x2b\x00\x03\x00\x33\x00\x03\x00\x3b\x00\x04\x00\x43\x00\x04\x00\x53\x00\x04"
	"\x00\x63\x00\x04\x00\x73\x00\x05\x00\x83\x00\x05\x00\xa3\x00\x05\x00\xc3\x00\x05\x00\xe3\x00\x00\x01\x02\x00\x00\x00\x01"
	"\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00\x04\x00\x01\x00\x05\x00\x01\x00\x07\x00\x02\x00\x09\x00\x02\x00\x0d\x00\x03"
	"\x00\x11\x00\x03\x00\x19\x00\x04\x00\x21\x00\x04\x00\x31\x00\x05\x00\x41\x00\x05\x00\x61\x00\x06\x00\x81\x00\x06\x00\xc1"
	"\x00\x07\x01\x01\x00\x07\x01\x81\x00\x08\x02\x01\x00\x08\x03\x01\x00\x09\x04\x01\x00\x09\x06\x01\x00\x0a\x08\x01\x00\x0a"
	"\x0c\x01\x00\x0b\x10\x01\x00\x0b\x18\x01\x00\x0c\x20\x01\x00\x0c\x30\x01\x00\x0d\x40\x01\x00\x0d\x60\x01"

	"\xff\xff"	//	HASH_LEN
	"\xff\xff"	//	SMS
	"\xff"		//	[CPB+DMS+SMS]
	"\xff"		//	SigComp Version
};


#define GHOST_STATE_ADDRESS				64
#define GHOST_STATE_INSTRUCTION			492
#define GHOST_STATE_MIN_ACCESS_LEN		6
#define GHOST_STATE_RETENTION_PRIORITY	0

/**Creates a Ghost state.
*/
void tcomp_deflatedata_createGhost(tcomp_deflatedata_t *deflatedata, uint32_t state_length, tcomp_params_t *params)
{
	uint8_t *ghostvalue_ptr = 0;
#define GHOSTVALUE_AT(position) (ghostvalue_ptr + position)

	if(!deflatedata){
		TSK_DEBUG_ERROR("NULL defalte data.");
		return;
	}

	if(deflatedata->ghostState){
		TSK_DEBUG_ERROR("The defalte data already have a ghost state. This MUST never happen.");
		return;
	}

	tsk_safeobj_lock(deflatedata);
	
	deflatedata->ghostState = tcomp_state_create(state_length, GHOST_STATE_ADDRESS, GHOST_STATE_INSTRUCTION, GHOST_STATE_MIN_ACCESS_LEN, GHOST_STATE_RETENTION_PRIORITY);
	tcomp_buffer_allocBuff(deflatedata->ghostState->value, state_length);
	ghostvalue_ptr = tcomp_buffer_getBuffer(deflatedata->ghostState->value);

	memcpy(ghostvalue_ptr, DeflateData_deflate_bytecode1_ghost, GHOST_BYTECODE1_SIZE);
	
	TSK_BINARY_SET_2BYTES( GHOSTVALUE_AT(GHOST_CB_START_INDEX), DEFLATE_UDVM_CIRCULAR_START_INDEX );
	TSK_BINARY_SET_2BYTES( GHOSTVALUE_AT(GHOST_DMS_INDEX), state_length+GHOST_STATE_ADDRESS );
	TSK_BINARY_SET_2BYTES( GHOSTVALUE_AT(GHOST_CB_END_INDEX), (DEFLATE_UDVM_CIRCULAR_START_INDEX + 0/*input_size*/) );
	// ......
	TSK_BINARY_SET_2BYTES( GHOSTVALUE_AT(GHOST_HASH_LEN_INDEX), (state_length+8) );
	TSK_BINARY_SET_2BYTES( GHOSTVALUE_AT(GHOST_SMS_INDEX), state_length );
	TSK_BINARY_SET_2BYTES( GHOSTVALUE_AT(GHOST_CPB_DMS_SMS_INDEX), tcomp_params_getParameters(params) ); // [cpb+dms+sms]+[version]

	// ------------------------------------------------------------

#define GHOST_DICT_CODE_INDEX			(GHOST_BYTECODE1_SIZE)
#define GHOST_DICT_WORDS_INDEX			(GHOST_DICT_CODE_INDEX + 2)
#if USE_DICTS_FOR_COMPRESSION
#	define GHOST_FIXME2_INDEX			(GHOST_DICT_WORDS_INDEX + 4/*Sip dict id len*/)
#else
#	define GHOST_FIXME2_INDEX			(GHOST_DICT_WORDS_INDEX + 0)
#endif
#define GHOST_DEFLATE_BYTECODE_INDEX	(GHOST_FIXME2_INDEX + 4)
#define GHOST_INPUT_INDEX				(GHOST_DEFLATE_BYTECODE_INDEX + DEFLATE_BYTECODE_LEN)	

#if USE_DICTS_FOR_COMPRESSION
	BINARY_SET_2BYTES( ghostState->getStateValue()->getBuffer(GHOST_DICT_CODE_INDEX), DEFLATE_FIXME_DICT );
	if( DEFLATE_FIXME_DICT == DEFLATE_NO_DICT ){
		// Nothing to append
	}else{
		if(DEFLATE_FIXME_DICT == DEFLATE_SIP_DICT_ONLY){
			::memmove( ghostState->getStateValue()->getBuffer(GHOST_DICT_WORDS_INDEX), RFC3485_DICTIONARY_SIP_IDENTIFIER, 4 );
		}else{
			assert(1==0);
		}
	}
#endif
	memcpy( GHOSTVALUE_AT(GHOST_DEFLATE_BYTECODE_INDEX), (const char*)DEFLATEDATA_DEFLATE_BYTECODE, DEFLATE_BYTECODE_LEN );

	tsk_safeobj_unlock(deflatedata);

#undef GHOSTVALUE_AT
}

/**Acknowledge a Ghost state.
*/
void tcomp_deflatedata_ackGhost(tcomp_compressordata_t *data, const tcomp_buffer_handle_t *stateid)
{
	tcomp_deflatedata_t *deflatedata = data;
	if(!deflatedata){
		TSK_DEBUG_ERROR("NULL defalte data.");
		return;
	}

	tsk_safeobj_lock(deflatedata);
	
	if(deflatedata->ghostState){
		/* Update ghost */
		if(tcomp_buffer_startsWith(deflatedata->ghostState->identifier, stateid)){
			/* END() + COPY() */
			tcomp_deflateStream_end(&(deflatedata->stream_acked));
			tcomp_deflateStream_copy(&(deflatedata->stream_acked), &(deflatedata->stream_1));
			deflatedata->stream_acked.stateful = 1;
			deflatedata->stream_acked.dataWaitingAck = 0;
			TSK_DEBUG_INFO("SigComp - ACKed State id=");
			tcomp_buffer_print(stateid);
		}
	}

	tsk_safeobj_unlock(deflatedata);
}

/**Updates a Ghost state.
*/
void tcomp_deflatedata_updateGhost(tcomp_deflatedata_t *deflatedata, const uint8_t *input_ptr, tsk_size_t input_size)
{
	uint32_t i;
	uint8_t *ghostvalue_ptr = 0;

#define GHOSTVALUE_AT(position) (ghostvalue_ptr + position)
		
	if(!deflatedata){
		TSK_DEBUG_ERROR("NULL defalte data.");
		return;
	}

	if(!deflatedata->ghostState){
		TSK_DEBUG_ERROR("NULL ghost state.");
		return;
	}

	tsk_safeobj_lock(deflatedata);

	ghostvalue_ptr = tcomp_buffer_getBuffer(deflatedata->ghostState->value);

#define ZBUFF_LEN	(0x0001 << deflatedata->zWindowBits)
	for(i = 0; i < input_size; i++){
#if 0
		*GHOSTVALUE_AT(GHOST_INPUT_INDEX + ghost_copy_offset) = 0x00;
#else
		*GHOSTVALUE_AT(GHOST_INPUT_INDEX + deflatedata->ghost_copy_offset) = input_ptr[i];
#endif
		deflatedata->ghost_copy_offset = (deflatedata->ghost_copy_offset + 1) % ZBUFF_LEN;
	}

	/* Update Circular Buffer Index */
	TSK_BINARY_SET_2BYTES( GHOSTVALUE_AT(GHOST_CB_END_INDEX), (DEFLATE_UDVM_CIRCULAR_START_INDEX + deflatedata->ghost_copy_offset) );
	
	/* Compute State Identifier (20 bytes) */
	tcomp_state_makeValid(deflatedata->ghostState);
	// new state identifer not acked yet 
	if(deflatedata->useOnlyACKedStates){
		deflatedata->stream_acked.stateful = 0;
		deflatedata->stream_acked.dataWaitingAck = 1;
	}

	TSK_DEBUG_INFO("SigComp - Making Ghost state valid with id = ");
	tcomp_buffer_print(deflatedata->ghostState->identifier);
	
	tsk_safeobj_unlock(deflatedata);

#undef GHOSTVALUE_AT
}


uint32_t* tcomp_deflatedata_getGhostCopyOffset(tcomp_deflatedata_t *deflatedata)
{
	if(!deflatedata){
		TSK_DEBUG_ERROR("NULL defalte data.");
		return 0;
	}

	return &(deflatedata->ghost_copy_offset);
}

void tcomp_deflatedata_freeGhostState(tcomp_compressordata_t *data)
{
	tcomp_deflatedata_t *deflatedata = data;
	if(!deflatedata){
		TSK_DEBUG_ERROR("NULL defalte data.");
		return;
	}

	TSK_OBJECT_SAFE_FREE(deflatedata->ghostState);
}
