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

/**@file tdav_consumer_audio.c
 * @brief Base class for all Audio consumers.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinydav/audio/tdav_consumer_audio.h"

#include "tinyrtp/rtp/trtp_rtp_header.h"

#include "tsk_memory.h"
#include "tsk_time.h"
#include "tsk_debug.h"

#if TSK_UNDER_WINDOWS
#	include <Winsock2.h> // timeval
#elif defined(__SYMBIAN32__)
#	include <_timeval.h> 
#else
#	include <sys/time.h>
#endif

#define TDAV_BITS_PER_SAMPLE_DEFAULT	16
#define TDAV_CHANNELS_DEFAULT			2
#define TDAV_RATE_DEFAULT				8000
#define TDAV_PTIME_DEFAULT				20

#define TDAV_10MS						10
#define TDAV_10MS_FRAME_SIZE(self)		(((self)->rate * TDAV_10MS)/1000)
#define TDAV_PTIME_FRAME_SIZE(self)		(((self)->rate * (self)->ptime)/1000)

/** Initialize audio consumer */
int tdav_consumer_audio_init(tdav_consumer_audio_t* self)
{
	int ret;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	/* base */
	if((ret = tmedia_consumer_init(TMEDIA_CONSUMER(self)))){
		return ret;
	}

	/* self (should be update by prepare() by using the codec's info)*/
	self->bits_per_sample = TDAV_BITS_PER_SAMPLE_DEFAULT;
	self->channels = TDAV_CHANNELS_DEFAULT;
	self->rate = TDAV_RATE_DEFAULT;
	self->ptime = TDAV_PTIME_DEFAULT;

	/* self:jitterbuffer */
	if(!self->jb.jbuffer){
		self->jb.jbuffer = jb_new();
		self->jb.jcodec = JB_CODEC_OTHER; // FIXME: e.g. JB_CODEC_G711x
	}

	tsk_safeobj_init(self);

	return 0;
}

/**
* Generic function to compare two consumers.
* @param consumer1 The first consumer to compare.
* @param consumer2 The second consumer to compare.
* @retval Returns an integral value indicating the relationship between the two consumers:
* <0 : @a consumer1 less than @a consumer2.<br>
* 0  : @a consumer1 identical to @a consumer2.<br>
* >0 : @a consumer1 greater than @a consumer2.<br>
*/
int tdav_consumer_audio_cmp(const tsk_object_t* consumer1, const tsk_object_t* consumer2)
{	
	return (TDAV_CONSUMER_AUDIO(consumer1) - TDAV_CONSUMER_AUDIO(consumer2));
}

/* put data (bytes not shorts) into the jitter buffer (consumers always have ptime of 20ms) */
int tdav_consumer_audio_put(tdav_consumer_audio_t* self, void** data, tsk_size_t size, const tsk_object_t* proto_hdr)
{
	const trtp_rtp_header_t* rtp_hdr = (const trtp_rtp_header_t*)proto_hdr;
	int i, _10ms_size_shorts, _10ms_size_bytes;
	long now, ts;
	short* _10ms_buf; // 10ms frame

	if(!self || !data || !*data || !self->jb.jbuffer || !rtp_hdr){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* synchronize the reference timestamp */
	if(!self->jb.ref_timestamp){
		struct timeval tv;
		long ts = (rtp_hdr->timestamp/(self->rate/1000));
		tsk_gettimeofday(&tv, tsk_null);
		tv.tv_sec -= (ts / self->rate);
		tv.tv_usec -= (ts % self->rate) * 125;
		if((tv.tv_usec -= (tv.tv_usec % (TDAV_10MS * 10000))) <0){
			tv.tv_usec += 1000000;
			tv.tv_sec -= 1;
		}
		self->jb.ref_timestamp = tsk_time_get_ms(&tv);

		switch(rtp_hdr->payload_type){
			// FIXME: TMEDIA_CODEC_FORMAT_* are "char*" just define int values to avoid char comparison
			case 8: /*TMEDIA_CODEC_FORMAT_G711a*/
			case 0: /* TMEDIA_CODEC_FORMAT_G711u */
				self->jb.jcodec = JB_CODEC_G711x;
				break;
			case 18: /* TMEDIA_CODEC_FORMAT_G729 */
				self->jb.jcodec = JB_CODEC_G729A;
				break;
			case 3: /* TMEDIA_CODEC_FORMAT_GSM */
				self->jb.jcodec = JB_CODEC_GSM_EFR;
				break;

			default:
				self->jb.jcodec = JB_CODEC_OTHER;
				break;
		}
	}
	
	tsk_safeobj_lock(self);
	// split as several 10ms frames
	now = (long) (tsk_time_now()-self->jb.ref_timestamp);
	ts = (long)(rtp_hdr->timestamp/(self->rate/1000));
	_10ms_size_shorts = TDAV_10MS_FRAME_SIZE(self);
	_10ms_size_bytes = _10ms_size_shorts * sizeof(short);
	for(i=0; i<(int)(size/_10ms_size_bytes);i++){
		if((_10ms_buf = tsk_calloc(_10ms_size_shorts, sizeof(short)))){
			memcpy(_10ms_buf, &((uint8_t*)*data)[i*_10ms_size_bytes], _10ms_size_bytes);
			jb_put(self->jb.jbuffer, _10ms_buf, JB_TYPE_VOICE, TDAV_10MS, ts, now, self->jb.jcodec);
			_10ms_buf = tsk_null;
		}
		ts += TDAV_10MS;
	}
	tsk_safeobj_unlock(self);

	return 0;
}

/* get data drom the jitter buffer (consumers should always have ptime of 20ms) */
void* tdav_consumer_audio_get(tdav_consumer_audio_t* self)
{
	void* data = tsk_null;
	int jret;

	int i, _10ms_count, _10ms_size_bytes, _10ms_size_shorts, data_size = 0;
	long now;
	short* _10ms_buf = tsk_null;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return tsk_null;
	}
	
	_10ms_size_shorts = TDAV_10MS_FRAME_SIZE(self);
	_10ms_size_bytes = (_10ms_size_shorts * sizeof(short));
	_10ms_count = (TDAV_PTIME_FRAME_SIZE(self)/_10ms_size_shorts);
	now = (long) (tsk_time_now()-self->jb.ref_timestamp);

	tsk_safeobj_lock(self);
	for(i=0; i<_10ms_count; i++){

		jret = jb_get(self->jb.jbuffer, (void**)&_10ms_buf, now, TDAV_10MS);
		

		switch(jret){
			case JB_OK:
			case JB_INTERP:
			case JB_EMPTY:
			case JB_NOFRAME:
			case JB_NOJB:
					if(data){
						if((data = tsk_realloc(data, (data_size + _10ms_size_bytes)))){
							if(_10ms_buf && (jret == JB_OK)){
								/* copy data */
								memcpy(&((uint8_t*)data)[data_size], _10ms_buf, _10ms_size_bytes);
							}
							else{
								/* copy silence */
								memset(&((uint8_t*)data)[data_size], 0, _10ms_size_bytes);
							}
							data_size += _10ms_size_bytes;
						}
						else{ /* realloc failed */
							data_size = 0;
						}

						if(jret == JB_INTERP){
							jb_reset_all(self->jb.jbuffer);
						}
					}
					else{
						data = _10ms_buf, _10ms_buf = tsk_null;
						data_size = _10ms_size_bytes;
					}
					break;
			
			default:
				break;
		}
		TSK_FREE(_10ms_buf);
	}
	tsk_safeobj_unlock(self);

	return data;
}

/** Reset jitterbuffer */
int tdav_consumer_audio_reset(tdav_consumer_audio_t* self){
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(self);
	if(self->jb.jbuffer){
		jb_reset_all(self->jb.jbuffer);
	}
	tsk_safeobj_unlock(self);

	return 0;
}

/* tsk_safeobj_lock(self); */
/* tsk_safeobj_unlock(self); */

/** DeInitialize audio consumer */
int tdav_consumer_audio_deinit(tdav_consumer_audio_t* self)
{
	int ret;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	/* base */
	if((ret = tmedia_consumer_deinit(TMEDIA_CONSUMER(self)))){
		/* return ret; */
	}

	/* self */
	if(self->jb.jbuffer){
		jb_destroy(self->jb.jbuffer);
	}

	tsk_safeobj_deinit(self);

	return 0;
}

