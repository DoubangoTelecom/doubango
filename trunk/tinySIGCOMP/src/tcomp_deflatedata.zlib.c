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

/**@file tcomp_compressor.h
 * @brief  Deflate compressor data.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tcomp_deflatedata.h"
#include "tsk_debug.h"

/**@ingroup tcomp_deflatedata_group
*/
int tcomp_deflateStream_end(tcomp_deflateStream_t *stream)
{
	if(!stream)
	{
		TSK_DEBUG_ERROR("NULL defalte stream.");
		return 0;
	}

	return deflateEnd(&(stream->zs));
}

/**@ingroup tcomp_deflatedata_group
*/
int tcomp_deflateStream_copy(tcomp_deflateStream_t *stream, tcomp_deflateStream_t *source)
{
	if(!stream)
	{
		TSK_DEBUG_ERROR("NULL defalte stream.");
		return 0;
	}

	return deflateCopy(&(stream->zs), &(source->zs));
}

/**@ingroup tcomp_deflatedata_group
*/
int tcomp_deflatedata_zInit(tcomp_deflatedata_t *deflatedata)
{
	if(!deflatedata)
	{
		TSK_DEBUG_ERROR("NULL defalte data.");
		return 0;
	}

	/* Already initialized? */
	if(deflatedata->initialized) 
	{
		return 1;
	}

	/* allocate deflate state */
    deflatedata->stream_1.zs.zalloc = deflatedata->stream_acked.zs.zalloc = Z_NULL;
    deflatedata->stream_1.zs.zfree = deflatedata->stream_acked.zs.zfree = Z_NULL;
    deflatedata->stream_1.zs.opaque = deflatedata->stream_acked.zs.opaque = Z_NULL;
#ifndef __SYMBIAN32__
	deflatedata->stream_1.zs.data_type = deflatedata->stream_acked.zs.data_type = Z_TEXT;
#endif

    //bool ret  =  (deflateInit(&this->zStream, this->zLevel) == Z_OK);
	if( deflateInit2(&deflatedata->stream_1.zs, deflatedata->zLevel, Z_DEFLATED, -deflatedata->zWindowBits, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY) != Z_OK 
		|| deflateInit2(&deflatedata->stream_acked.zs, deflatedata->zLevel, Z_DEFLATED, -deflatedata->zWindowBits, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY) != Z_OK )
	{
		return 0;
	}
#if USE_DICTS_FOR_COMPRESSION
	if( deflateSetDictionary(this->stream_1.zs, (const Bytef*)RFC3485_DICTIONARY_SIP_VALUE, RFC3485_DICTIONARY_SIP_VALUE_LENGTH) != Z_OK 
		|| deflateSetDictionary(this->stream_acked.zs, (const Bytef*)RFC3485_DICTIONARY_SIP_VALUE, RFC3485_DICTIONARY_SIP_VALUE_LENGTH) != Z_OK )
	{
		return false;
	}
#endif

	deflatedata->stream_1.stateful = deflatedata->stream_acked.stateful = 0;
	deflatedata->stream_1.dataWaitingAck = deflatedata->stream_acked.dataWaitingAck = 0;
	deflatedata->initialized = 1;

	return 1;
}

/**@ingroup tcomp_deflatedata_group
*/
int tcomp_deflatedata_zUnInit(tcomp_deflatedata_t *deflatedata)
{
	if(!deflatedata)
	{
		TSK_DEBUG_ERROR("NULL defalte data.");
		return 0;
	}

	if(deflatedata->initialized)
	{
		deflatedata->initialized = 0;
		
		deflatedata->stream_1.dataWaitingAck = deflatedata->stream_acked.dataWaitingAck = 0;
		deflatedata->stream_1.stateful = deflatedata->stream_acked.stateful = 0;

		return (tcomp_deflateStream_end(&deflatedata->stream_1) != Z_STREAM_ERROR) && (tcomp_deflateStream_end(&deflatedata->stream_acked) != Z_STREAM_ERROR);
	}
	return 1;
}

/**@ingroup tcomp_deflatedata_group
*/
int tcomp_deflatedata_zReset(tcomp_deflatedata_t *deflatedata)
{
	int ret;

	if(!deflatedata)
	{
		TSK_DEBUG_ERROR("NULL defalte data.");
		return 0;
	}

	ret = deflatedata->initialized ? tcomp_deflatedata_zUnInit(deflatedata) : 1; 
	ret &= tcomp_deflatedata_zInit(deflatedata);
	
	return ret;
}

/**@ingroup tcomp_deflatedata_group
*/
int tcomp_deflatedata_zCompress(tcomp_deflatedata_t *deflatedata, const void* in, size_t inLen, void* out, size_t* outLen, int *stateChanged)
{
	int ret = 0;
/*
	Two streams [1] and [2]
	
	* ZINIT/ZUNINIT/ZRESET
		XXX([1])
		XXX([2])
	
	* COMPRESSION
		[1]->END()
		[1]<-COPY-[2]
		[1]->COMPRESS()
	
	* ACK
		[2]->END()
		[2]<-COPY-[1]
		updateGhost([1])
*/
	if(!deflatedata)
	{
		TSK_DEBUG_ERROR("NULL defalte data.");
		return 0;
	}

	tsk_safeobj_lock(deflatedata);
	
	/* Initialized? */
	if(!deflatedata->initialized)
	{
		if(!tcomp_deflatedata_zInit(deflatedata))
		{
			TSK_DEBUG_ERROR("Failed to initialize zlib resources..");
			tsk_safeobj_unlock(deflatedata);
			return 0;
		}
	}
	
#if USE_ONLY_ACKED_STATES
	if(!deflatedata->stream_acked.dataWaitingAck)
	{
		deflatedata->stream_acked.dataWaitingAck = 1;
		*stateChanged = 1;
	}
	else
	{
		*stateChanged = 0;
	}

	/* END() + COPY() ==> use acked state */
	tcomp_deflateStream_end(&(deflatedata->stream_1));
	tcomp_deflateStream_copy(&(deflatedata->stream_1), &(deflatedata->stream_acked));
	
#else
	*stateChanged = 1;
#endif
	
	// IN
	deflatedata->stream_1.zs.next_in = (Bytef*)in;
    deflatedata->stream_1.zs.avail_in = (uInt)inLen;
	
	// OUT
	deflatedata->stream_1.zs.next_out = (Bytef*)out;
    deflatedata->stream_1.zs.avail_out = (uInt)*outLen;
	
	ret = deflate(&(deflatedata->stream_1.zs), Z_SYNC_FLUSH);
	
	*outLen -= deflatedata->stream_1.zs.avail_out;
	
	tsk_safeobj_unlock(deflatedata);
	
	return (ret == Z_OK);
}

/**@ingroup tcomp_deflatedata_group
*/
int tcomp_deflatedata_zGetWindowBits(tcomp_deflatedata_t *deflatedata)
{
	if(!deflatedata)
	{
		TSK_DEBUG_ERROR("NULL defalte data.");
		return 0;
	}

	return deflatedata->zWindowBits;
}

/**@ingroup tcomp_deflatedata_group
*/
void tcomp_deflatedata_zSetWindowBits(tcomp_deflatedata_t *deflatedata, int windowSize)
{
	if(!deflatedata)
	{
		TSK_DEBUG_ERROR("NULL defalte data.");
		return;
	}

	deflatedata->zWindowBits = windowSize;
}