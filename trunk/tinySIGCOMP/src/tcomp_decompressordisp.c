/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
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

/**@file tcomp_decompressordisp.c
 * @brief  Entity that receives SigComp messages, invokes a UDVM, and forwards the resulting decompressed messages to the application.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tcomp_decompressordisp.h"
#include "tcomp_message.h"
#include "tcomp_udvm.h"

#include "tsk_debug.h"

#include <assert.h>
#include <string.h>

#define TCOMP_MAX_STREAM_BUFFER_SIZE		65535
#define TCOMP_NACK_SUPPORTED(dispatcher)	(dispatcher->stateHandler->sigcomp_parameters->SigComp_version >= 0x02)

/**@defgroup tcomp_decompressordisp_group SIGCOMP decompressor dispatcher.
* Entity that receives SigComp messages, invokes a UDVM, and forwards the resulting decompressed messages to the application.
*/


/**@ingroup tcomp_decompressordisp_group
*/
static int pred_find_streambuffer_by_id(const tsk_list_item_t *item, const void *id)
{
	if(item && item->data)
	{
		tcomp_stream_buffer_t *streambuffer = item->data;
		uint64_t res = (streambuffer->id - *((uint64_t*)id));
		return res > 0 ? (int)1 : (res < 0 ? (int)-1 : (int)0);
	}
	return -1;
}


/**@ingroup tcomp_decompressordisp_group
*/
int tcomp_decompressordisp_decompress(tcomp_decompressordisp_t *dispatcher, const void* input_ptr, size_t input_size, tcomp_result_t *lpResult)
{
	int ret = 1;
	uint64_t streamId = 0;
	const tsk_list_item_t *item_const;

	if(!dispatcher)
	{
		TSK_DEBUG_ERROR("NULL sigcomp decompressor dispatcher.");
		return 0;
	}

	/*
	* Check if transport type changed.
	*/
	if(lpResult->isStreamBased)
	{
		if(!dispatcher->streamBuffers)
		{
			dispatcher->streamBuffers = TSK_LIST_CREATE();
		}

		streamId = lpResult->streamId;
		ret =   tcomp_decompressordisp_appendStream(dispatcher, input_ptr, input_size, streamId);
		if(!ret)
		{
			TSK_DEBUG_ERROR("Failed to append new stream buffer.");
			return 0;
		}
	}
	
	if(lpResult->isStreamBased)
	{
		size_t size = 0;
		uint16_t discard_count = 0;
		tcomp_stream_buffer_t *lpBuffer;

		item_const = tsk_list_find_item_by_pred(dispatcher->streamBuffers, pred_find_streambuffer_by_id, &streamId);
		if(!item_const || !(lpBuffer = item_const->data))
		{
			TSK_DEBUG_ERROR("Failed to find stream buffer by id %llu.", streamId);
			return 0;
		}

		if(ret && tcomp_decompressordisp_getNextStreamMsg(dispatcher, streamId, &discard_count, &size))
		{
			ret &= tcomp_decompressordisp_internalDecompress(dispatcher, tcomp_buffer_getBuffer(lpBuffer->buffer), size, &lpResult);

			/* remove buffer and discard */
			tcomp_buffer_discardLastBytes(lpBuffer->buffer, discard_count);
			ret &= tcomp_buffer_removeBuff(lpBuffer->buffer, 0, size);
		}
		if(discard_count)
		{
			tcomp_buffer_discardLastBytes(lpBuffer->buffer, discard_count);
		}
		if(size)
		{
			//ret&= lpBuffer->removeBuff(0, (size));
		}
	}
	else
	{
		ret &= tcomp_decompressordisp_internalDecompress(dispatcher, input_ptr, input_size, &lpResult);
	}
	
	return ret;
}

/**@ingroup tcomp_decompressordisp_group
*/
int tcomp_decompressordisp_getNextMessage(tcomp_decompressordisp_t *dispatcher, tcomp_result_t *lpResult)
{
	int ret = 1;
	size_t size=0;
	uint16_t discard_count = 0;
	uint64_t streamId;
	tcomp_stream_buffer_t *lpBuffer;
	const tsk_list_item_t *item_const;

	if(!dispatcher)
	{
		TSK_DEBUG_ERROR("NULL sigcomp decompressor dispatcher.");
		return 0;
	}

	streamId = lpResult->streamId;

	item_const = tsk_list_find_item_by_pred(dispatcher->streamBuffers, pred_find_streambuffer_by_id, &streamId);
	if(!item_const || !(lpBuffer = item_const->data))
	{
		TSK_DEBUG_ERROR("Failed to find stream buffer by id %llu.", streamId);
		return 0;
	}
	
	if(ret && tcomp_decompressordisp_getNextStreamMsg(dispatcher, streamId, &discard_count, &size))
	{
		ret &= tcomp_decompressordisp_internalDecompress(dispatcher, tcomp_buffer_getBuffer(lpBuffer->buffer), size, &lpResult);

		/* remove buffer and discard */
		tcomp_buffer_discardLastBytes(lpBuffer->buffer, discard_count);
		ret &= tcomp_buffer_removeBuff(lpBuffer->buffer, 0, size);
	}
	else ret = 0; /* Is it right? */

	if(discard_count)
	{
		tcomp_buffer_discardLastBytes(lpBuffer->buffer, discard_count);
	}
	return ret;
}

/**@ingroup tcomp_decompressordisp_group
*/
int tcomp_decompressordisp_internalDecompress(tcomp_decompressordisp_t *dispatcher, const void* input_ptr, const size_t input_size, tcomp_result_t **lpResult)
{
	tcomp_message_t *sigCompMessage = 0;
	tcomp_udvm_t *sigCompUDVM = 0;
	int ret = 0;

	if(!dispatcher)
	{
		TSK_DEBUG_ERROR("NULL sigcomp decompressor dispatcher.");
		goto bail;
	}		

	sigCompMessage = TCOMP_MESSAGE_CREATE(input_ptr, input_size, (*lpResult)->isStreamBased);
	if(!sigCompMessage || !sigCompMessage->isOK)
	{
		TSK_DEBUG_ERROR("Failed to create new sigcomp message.");
		goto bail;
	}
	else if(sigCompMessage->isNack && TCOMP_NACK_SUPPORTED(dispatcher))
	{
		/* Remote party send us a NACK --> handle it */
		tcomp_statehandler_handleNack((tcomp_statehandler_t*)dispatcher->stateHandler, (const tcomp_nackinfo_t*)sigCompMessage->nack_info);
		(*lpResult)->isNack = 1;
		
		goto bail;
	}

	/* Create new UDVM entity for each SigComp message */
	sigCompUDVM = TCOMP_UDVM_CREATE(sigCompMessage, dispatcher->stateHandler, *lpResult);
	
	/* Decompress message */
	ret = tcomp_udvm_decompress(sigCompUDVM);

	/* decompression failed --> returns nack if supported */
	if(!ret)
	{
		/* Decompression failed --> return NACK message to the application layer */
		(*lpResult)->isNack = TCOMP_NACK_SUPPORTED(dispatcher);
	}
	
bail:
	/* Delete Message */
	TSK_OBJECT_SAFE_FREE(sigCompMessage);

	/* Delete UDVM entity */
	TSK_OBJECT_SAFE_FREE(sigCompUDVM);

	return ret;
}

/**@ingroup tcomp_decompressordisp_group
*/
int tcomp_decompressordisp_appendStream(tcomp_decompressordisp_t *dispatcher, const void* input_ptr, size_t input_size, uint64_t streamId)
{
	tcomp_stream_buffer_t* lpBuffer = 0;
	const tsk_list_item_t *item_const;

	if(!dispatcher)
	{
		TSK_DEBUG_ERROR("NULL sigcomp decompressor dispatcher.");
		return 0;
	}

	item_const = tsk_list_find_item_by_pred(dispatcher->streamBuffers, pred_find_streambuffer_by_id, &streamId);
	if(!item_const || !(lpBuffer = item_const->data))
	{
		/* First time we get this stream ID */
		tcomp_buffer_handle_t *newbuf = TCOMP_STREAM_BUFFER_CREATE(streamId);
		if(newbuf)
		{
			lpBuffer = newbuf;
			lpBuffer->buffer = TCOMP_BUFFER_CREATE();
			tsk_list_push_back_data(dispatcher->streamBuffers, ((void**) &newbuf));
		}
		else
		{
			TSK_DEBUG_ERROR("Failed to create new stream buffer.");
			return 0;
		}
	}
	
	/*  Check if buffer is too large */
	if(lpBuffer->buffer && (tcomp_buffer_getSize(lpBuffer->buffer) + input_size) > TCOMP_MAX_STREAM_BUFFER_SIZE)
	{
		tcomp_buffer_freeBuff(lpBuffer->buffer);
		return 0;
	}

	/* append new buffer */
	if(!tcomp_buffer_appendBuff(lpBuffer->buffer, input_ptr, input_size))
	{
		TSK_DEBUG_ERROR("Failed to append new buffer.");
		tcomp_buffer_freeBuff(lpBuffer->buffer);
		return 0;
	}
	
	return 1;
}

/**@ingroup tcomp_decompressordisp_group
*/
int tcomp_decompressordisp_getNextStreamMsg(tcomp_decompressordisp_t *dispatcher, uint64_t streamId, uint16_t *discard_count, size_t *size)
{
	tcomp_stream_buffer_t *lpBuffer;
	const tsk_list_item_t *item_const;

	uint8_t quote_count = 0;
	uint8_t* start;
	uint8_t* end;

	if(!dispatcher)
	{
		TSK_DEBUG_ERROR("NULL sigcomp decompressor dispatcher.");
		return 0;
	}

	/*
	* RFC 3320 - 4.2.1.  Decompressor Dispatcher Strategies [strategie 1]
	*/
	item_const = tsk_list_find_item_by_pred(dispatcher->streamBuffers, pred_find_streambuffer_by_id, &streamId);
	if(!item_const || !(lpBuffer = item_const->data))
	{
		TSK_DEBUG_ERROR("Failed to find stream buffer by id %llu.", streamId);
		return 0;
	}
	
	*size = 0;
	*discard_count = 0;

	quote_count = 0;
	start = tcomp_buffer_getBuffer(lpBuffer->buffer);
	end = (start + tcomp_buffer_getSize(lpBuffer->buffer));

	while(start<end){
		if(*start==0xff)
		{
			start++;
			if(*start==0xff)
			{ /* end message */
				if(*size) return 1;
				else /* message is empty --> delete this empty message(length=2) */
				{ 
					start--;
					memcpy(start, (start+2), (end-start));
					(*discard_count)+=2;
					end-=2;
					continue; 
				}
			}

			quote_count = *start;
			memcpy((start), (start+1), (end-start));
			end--;
			(*discard_count)++;
			start+=(quote_count);
			(*size)+=(1+quote_count);
		}else { start++; (*size)++; }
	}

	return 0;
}










//========================================================
//	SigComp decompressor dispatcher object definition
//

/**@ingroup tcomp_decompressordisp_group
*/
static void* tcomp_decompressordisp_create(void * self, va_list * app)
{
	tcomp_decompressordisp_t *decompressordisp = self;
	if(decompressordisp)
	{
		decompressordisp->stateHandler = va_arg(*app, const tcomp_statehandler_t*);

		/* Initialize safeobject */
		tsk_safeobj_init(decompressordisp);
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new decompressor dispatcher.");
	}

	return self;
}

/**@ingroup tcomp_decompressordisp_group
*/
static void* tcomp_decompressordisp_destroy(void *self)
{
	tcomp_decompressordisp_t *decompressordisp = self;
	if(decompressordisp)
	{
		/* Deinitialize safeobject */
		tsk_safeobj_deinit(decompressordisp);

		TSK_OBJECT_SAFE_FREE(decompressordisp->streamBuffers);
	}
	else TSK_DEBUG_ERROR("Null dispatcher.");
	
	return self;
}

static const tsk_object_def_t tcomp_decompressordisp_def_s = 
{
	sizeof(tcomp_decompressordisp_t),
	tcomp_decompressordisp_create,
	tcomp_decompressordisp_destroy,
	0
};
const void *tcomp_decompressordisp_def_t = &tcomp_decompressordisp_def_s;




//========================================================
//	SigComp stream buffer object definition
//

/**@ingroup tcomp_decompressordisp_group
*/
static void* tcomp_stream_buffer_create(void * self, va_list * app)
{
	tcomp_stream_buffer_t *stream_buffer = self;
	if(stream_buffer)
	{
		stream_buffer->id = va_arg(*app, uint64_t);
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new stream buffer.");
	}

	return self;
}

/**@ingroup tcomp_decompressordisp_group
*/
static void* tcomp_stream_buffer_destroy(void *self)
{
	tcomp_stream_buffer_t *stream_buffer = self;
	if(stream_buffer)
	{
		TSK_OBJECT_SAFE_FREE(stream_buffer->buffer);
	}
	else TSK_DEBUG_ERROR("Null stream buffer.");
	
	return self;
}

static const tsk_object_def_t tcomp_stream_buffer_def_s = 
{
	sizeof(tcomp_stream_buffer_t),
	tcomp_stream_buffer_create,
	tcomp_stream_buffer_destroy,
	0
};
const void *tcomp_stream_buffer_def_t = &tcomp_stream_buffer_def_s;
