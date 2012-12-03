/* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
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
*/
#include "audio_webrtc_producer.h"
#include "audio_webrtc.h"

#include "tinydav/audio/tdav_producer_audio.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

typedef struct audio_producer_webrtc_s
{
	TDAV_DECLARE_PRODUCER_AUDIO;

	bool isMuted;
	audio_webrtc_instance_handle_t* audioInstHandle;
	struct{
		void* ptr;
		int size;
		int index;
	} buffer;
}
audio_producer_webrtc_t;

int audio_producer_webrtc_handle_data_10ms(const audio_producer_webrtc_t* _self, const void* audioSamples, int nSamples, int nBytesPerSample, int samplesPerSec, int nChannels)
{
	if(!_self || !audioSamples || !nSamples){
		DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if((nSamples != (samplesPerSec / 100))){
		DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR("Not producing 10ms samples (nSamples=%d, samplesPerSec=%d)", nSamples, samplesPerSec);
		return -2;
	}
	if((nBytesPerSample != (TMEDIA_PRODUCER(_self)->audio.bits_per_sample >> 3))){
		DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR("%d not valid bytes/samples", nBytesPerSample);
		return -3;
	}
	if((nChannels != TMEDIA_PRODUCER(_self)->audio.channels)){
		DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR("%d not the expected number of channels", nChannels);
		return -4;
	}

	int nSamplesInBits = (nSamples * nBytesPerSample);
	if(_self->buffer.index + nSamplesInBits > _self->buffer.size){
		DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR("Buffer overflow");
		return -5;
	}

	audio_producer_webrtc_t* self = const_cast<audio_producer_webrtc_t*>(_self);

	memcpy((((uint8_t*)self->buffer.ptr) + self->buffer.index), audioSamples, nSamplesInBits);
	self->buffer.index += nSamplesInBits;

	if(self->buffer.index == self->buffer.size){
		self->buffer.index = 0;
		if(TMEDIA_PRODUCER(self)->enc_cb.callback){
			if(self->isMuted){
				memset(self->buffer.ptr, 0, self->buffer.size);
			}
			TMEDIA_PRODUCER(self)->enc_cb.callback(TMEDIA_PRODUCER(self)->enc_cb.callback_data, self->buffer.ptr, self->buffer.size);
		}
	}

	return 0;
}


/* ============ Media Producer Interface ================= */
static int audio_producer_webrtc_set(tmedia_producer_t* _self, const tmedia_param_t* param)
{	
	audio_producer_webrtc_t* self = (audio_producer_webrtc_t*)_self;
	if(param->plugin_type == tmedia_ppt_producer){
		if(param->value_type == tmedia_pvt_int32){
			if(tsk_striequals(param->key, "mute")){
				self->isMuted = (TSK_TO_INT32((uint8_t*)param->value) != 0);
				return 0;
			}
		}
	}
	return tdav_producer_audio_set(TDAV_PRODUCER_AUDIO(self), param);
}
static int audio_producer_webrtc_prepare(tmedia_producer_t* _self, const tmedia_codec_t* codec)
{
	audio_producer_webrtc_t* self = (audio_producer_webrtc_t*)_self;
	if(!self || !codec){
		DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	// create audio instance
	if(!(self->audioInstHandle = audio_webrtc_instance_create(TMEDIA_PRODUCER(self)->session_id))){
		DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR("Failed to create audio instance handle");
		return -2;
	}

	// check that ptime is mutiple of 10
	if((codec->plugin->audio.ptime % 10)){
		DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR("ptime=%d not multiple of 10", codec->plugin->audio.ptime);
		return -3;
	}

	// init input parameters from the codec
	TMEDIA_PRODUCER(self)->audio.channels = codec->plugin->audio.channels;
	TMEDIA_PRODUCER(self)->audio.rate = codec->plugin->rate;
	TMEDIA_PRODUCER(self)->audio.ptime = codec->plugin->audio.ptime;

	// prepare playout device and update output parameters
	int ret;
	ret = audio_webrtc_instance_prepare_producer(self->audioInstHandle, &_self);

	// now that the producer is prepared we can initialize internal buffer using device caps
	if(ret == 0){
		// allocate buffer
		int xsize = ((TMEDIA_PRODUCER(self)->audio.ptime * TMEDIA_PRODUCER(self)->audio.rate) / 1000) * (TMEDIA_PRODUCER(self)->audio.bits_per_sample >> 3);
		if(!(self->buffer.ptr = tsk_realloc(self->buffer.ptr, xsize))){
			DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR("Failed to allocate buffer with size = %d", xsize);
			self->buffer.size = 0;
			return -1;
		}
		self->buffer.size = xsize;
		self->buffer.index = 0;
	}
	return ret;
}

static int audio_producer_webrtc_start(tmedia_producer_t* _self)
{
	audio_producer_webrtc_t* self = (audio_producer_webrtc_t*)_self;
	if(!self){
		DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	return audio_webrtc_instance_start_producer(self->audioInstHandle);
}

static int audio_producer_webrtc_pause(tmedia_producer_t* self)
{
	return 0;
}

static int audio_producer_webrtc_stop(tmedia_producer_t* _self)
{
	audio_producer_webrtc_t* self = (audio_producer_webrtc_t*)_self;
	if(!self){
		DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	return audio_webrtc_instance_stop_producer(self->audioInstHandle);
}


//
//	WebRTC audio producer object definition
//
/* constructor */
static tsk_object_t* audio_producer_webrtc_ctor(tsk_object_t *_self, va_list * app)
{
	audio_producer_webrtc_t *self = (audio_producer_webrtc_t *)_self;
	if(self){
		/* init base */
		tdav_producer_audio_init(TDAV_PRODUCER_AUDIO(self));
		/* init self */
		
	}
	return self;
}
/* destructor */
static tsk_object_t* audio_producer_webrtc_dtor(tsk_object_t *_self)
{ 
	audio_producer_webrtc_t *self = (audio_producer_webrtc_t *)_self;
	if(self){
		/* stop */
		audio_producer_webrtc_stop(TMEDIA_PRODUCER(self));
		/* deinit self */
		if(self->audioInstHandle){
			audio_webrtc_instance_destroy(&self->audioInstHandle);
		}
		TSK_FREE(self->buffer.ptr);

		/* deinit base */
		tdav_producer_audio_deinit(TDAV_PRODUCER_AUDIO(self));
	}

	return self;
}
/* object definition */
static const tsk_object_def_t audio_producer_webrtc_def_s = 
{
	sizeof(audio_producer_webrtc_t),
	audio_producer_webrtc_ctor, 
	audio_producer_webrtc_dtor,
	tdav_producer_audio_cmp, 
};
/* plugin definition*/
static const tmedia_producer_plugin_def_t audio_producer_webrtc_plugin_def_s = 
{
	&audio_producer_webrtc_def_s,
	
	tmedia_audio,
	"WebRTC audio producer",
	
	audio_producer_webrtc_set,
	audio_producer_webrtc_prepare,
	audio_producer_webrtc_start,
	audio_producer_webrtc_pause,
	audio_producer_webrtc_stop
};
const tmedia_producer_plugin_def_t *audio_producer_webrtc_plugin_def_t = &audio_producer_webrtc_plugin_def_s;