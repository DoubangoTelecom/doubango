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
 * @brief  SiComp Deflate compressor (zlib).
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *

 */
#include "tcomp_deflatedata.h"
#include "tsk_debug.h"


tsk_bool_t tcomp_deflateStream_end(tcomp_deflateStream_t *stream)
{
    if(!stream) {
        TSK_DEBUG_ERROR("NULL defalte stream.");
        return tsk_false;
    }

    return deflateEnd(&(stream->zs));
}

tsk_bool_t tcomp_deflateStream_copy(tcomp_deflateStream_t *stream, tcomp_deflateStream_t *source)
{
    if(!stream) {
        TSK_DEBUG_ERROR("NULL defalte stream.");
        return tsk_false;
    }

    return deflateCopy(&(stream->zs), &(source->zs));
}

tsk_bool_t tcomp_deflatedata_zInit(tcomp_deflatedata_t *deflatedata)
{
    if(!deflatedata) {
        TSK_DEBUG_ERROR("NULL defalte data.");
        return tsk_false;
    }

    /* Already initialized? */
    if(deflatedata->isInitialized) {
        return tsk_true;
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
            || deflateInit2(&deflatedata->stream_acked.zs, deflatedata->zLevel, Z_DEFLATED, -deflatedata->zWindowBits, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY) != Z_OK ) {
        return tsk_false;
    }
#if USE_DICTS_FOR_COMPRESSION
    if( deflateSetDictionary(this->stream_1.zs, (const Bytef*)RFC3485_DICTIONARY_SIP_VALUE, RFC3485_DICTIONARY_SIP_VALUE_LENGTH) != Z_OK
            || deflateSetDictionary(this->stream_acked.zs, (const Bytef*)RFC3485_DICTIONARY_SIP_VALUE, RFC3485_DICTIONARY_SIP_VALUE_LENGTH) != Z_OK ) {
        return false;
    }
#endif

    deflatedata->stream_1.stateful = deflatedata->stream_acked.stateful = 0;
    deflatedata->stream_1.dataWaitingAck = deflatedata->stream_acked.dataWaitingAck = 0;
    deflatedata->isInitialized = tsk_true;

    return tsk_true;
}

tsk_bool_t tcomp_deflatedata_zUnInit(tcomp_deflatedata_t *deflatedata)
{
    if(!deflatedata) {
        TSK_DEBUG_ERROR("NULL defalte data.");
        return tsk_false;
    }

    if(deflatedata->isInitialized) {
        deflatedata->isInitialized = tsk_false;

        deflatedata->stream_1.dataWaitingAck = deflatedata->stream_acked.dataWaitingAck = 0;
        deflatedata->stream_1.stateful = deflatedata->stream_acked.stateful = 0;

        return (tcomp_deflateStream_end(&deflatedata->stream_1) != Z_STREAM_ERROR) && (tcomp_deflateStream_end(&deflatedata->stream_acked) != Z_STREAM_ERROR);
    }
    return tsk_true;
}

tsk_bool_t tcomp_deflatedata_zReset(tcomp_deflatedata_t *deflatedata)
{
    tsk_bool_t ret;

    if(!deflatedata) {
        TSK_DEBUG_ERROR("NULL defalte data.");
        return tsk_false;
    }

    ret = deflatedata->isInitialized ? tcomp_deflatedata_zUnInit(deflatedata) : tsk_true;
    ret &= tcomp_deflatedata_zInit(deflatedata);

    return ret;
}

tsk_bool_t tcomp_deflatedata_zCompress(tcomp_deflatedata_t *deflatedata, const void* in, tsk_size_t inLen, void* out, tsk_size_t* outLen, tsk_bool_t *stateChanged)
{
    int ret = tsk_false;
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
    if(!deflatedata) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_false;
    }

    tsk_safeobj_lock(deflatedata);

    /* Initialized? */
    if(!deflatedata->isInitialized) {
        if(!tcomp_deflatedata_zInit(deflatedata)) {
            TSK_DEBUG_ERROR("Failed to initialize zlib resources..");
            tsk_safeobj_unlock(deflatedata);
            return tsk_false;
        }
    }

    if(deflatedata->useOnlyACKedStates) {
        if(!deflatedata->stream_acked.dataWaitingAck) {
            deflatedata->stream_acked.dataWaitingAck = 1;
            *stateChanged = tsk_true;
        }
        else {
            *stateChanged = tsk_false;
        }

        /* END() + COPY() ==> use acked state */
        tcomp_deflateStream_end(&(deflatedata->stream_1));
        tcomp_deflateStream_copy(&(deflatedata->stream_1), &(deflatedata->stream_acked));
    }
    else {
        *stateChanged = tsk_true;
    }

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

int tcomp_deflatedata_zGetWindowBits(tcomp_deflatedata_t *deflatedata)
{
    if(!deflatedata) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return 0;
    }

    return deflatedata->zWindowBits;
}

void tcomp_deflatedata_zSetWindowBits(tcomp_deflatedata_t *deflatedata, int windowSize)
{
    if(!deflatedata) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return;
    }

    deflatedata->zWindowBits = windowSize;
}

