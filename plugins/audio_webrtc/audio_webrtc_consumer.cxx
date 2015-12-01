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
#include "audio_webrtc_consumer.h"
#include "audio_webrtc.h"

#include "tinydav/audio/tdav_consumer_audio.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

typedef struct audio_consumer_webrtc_s
{
	TDAV_DECLARE_CONSUMER_AUDIO;
	audio_webrtc_instance_handle_t* audioInstHandle;
	struct{
		void* ptr;
		bool isFull;
		int size;
		int index;
	} buffer;
}
audio_consumer_webrtc_t;

int audio_consumer_webrtc_get_data_10ms(const audio_consumer_webrtc_t* _self, void* audioSamples, int nSamples, int nBytesPerSample, int nChannels, int samplesPerSec, uint32_t &nSamplesOut)
{
	nSamplesOut = 0;
	if(!_self || !audioSamples || !nSamples){
		DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if((nSamples != (samplesPerSec / 100))){
		DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR("Not producing 10ms samples (nSamples=%d, samplesPerSec=%d)", nSamples, samplesPerSec);
		return -2;
	}
	if((nBytesPerSample != (TMEDIA_CONSUMER(_self)->audio.bits_per_sample >> 3))){
		DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR("%d not valid bytes/samples", nBytesPerSample);
		return -3;
	}
	if((nChannels != TMEDIA_CONSUMER(_self)->audio.out.channels)){
		DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR("%d not the expected number of channels", nChannels);
		return -4;
	}

	audio_consumer_webrtc_t* self = const_cast<audio_consumer_webrtc_t*>(_self);

	if(self->buffer.index == self->buffer.size){
		tdav_consumer_audio_tick(TDAV_CONSUMER_AUDIO(self));
		self->buffer.index = 0;
		if((tdav_consumer_audio_get(TDAV_CONSUMER_AUDIO(self), self->buffer.ptr, self->buffer.size)) != self->buffer.size){
			nSamplesOut = 0;
			return 0;
		}
	}
	
	int nSamplesInBits = (nSamples * nBytesPerSample);
	if(_self->buffer.index + nSamplesInBits <= _self->buffer.size){
		memcpy(audioSamples, (((uint8_t*)self->buffer.ptr) + self->buffer.index), nSamplesInBits);
	}
	self->buffer.index += nSamplesInBits;
	TSK_CLAMP(0, self->buffer.index, self->buffer.size);
	nSamplesOut = nSamples;
	
	return 0;
}


/* ============ Media Consumer Interface ================= */
static int audio_consumer_webrtc_set(tmedia_consumer_t* self, const tmedia_param_t* param)
{
	audio_consumer_webrtc_t* webrtc = (audio_consumer_webrtc_t*)self;
	int ret = tdav_consumer_audio_set(TDAV_CONSUMER_AUDIO(self), param);

	if(ret == 0){
		if(tsk_striequals(param->key, "volume")){
			
		}
	}

	return ret;
}

static int audio_consumer_webrtc_prepare(tmedia_consumer_t* _self, const tmedia_codec_t* codec)
{
	audio_consumer_webrtc_t* self = (audio_consumer_webrtc_t*)_self;
	if(!self){
		DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	// create audio instance
	if(!(self->audioInstHandle = audio_webrtc_instance_create(TMEDIA_CONSUMER(self)->session_id))){
		DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR("Failed to create audio instance handle");
		return -1;
	}

	// initialize input parameters from the codec information
	TMEDIA_CONSUMER(self)->audio.ptime = codec->plugin->audio.ptime;
	TMEDIA_CONSUMER(self)->audio.in.channels = codec->plugin->audio.channels;
	TMEDIA_CONSUMER(self)->audio.in.rate = codec->plugin->rate;
	
	// prepare playout device and update output parameters
	int ret = audio_webrtc_instance_prepare_consumer(self->audioInstHandle, &_self);

	// now that the producer is prepared we can initialize internal buffer using device caps
	if(ret == 0){
		// allocate buffer
		int xsize = ((TMEDIA_CONSUMER(self)->audio.ptime * TMEDIA_CONSUMER(self)->audio.out.rate) / 1000) * (TMEDIA_CONSUMER(self)->audio.bits_per_sample >> 3);
		if(!(self->buffer.ptr = tsk_realloc(self->buffer.ptr, xsize))){
			DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR("Failed to allocate buffer with size = %d", xsize);
			self->buffer.size = 0;
			return -1;
		}
		memset(self->buffer.ptr, 0, xsize);
		self->buffer.size = xsize;
		self->buffer.index = 0;
		self->buffer.isFull = false;
	}
	return ret;
}

static int audio_consumer_webrtc_start(tmedia_consumer_t* _self)
{
	audio_consumer_webrtc_t* self = (audio_consumer_webrtc_t*)_self;
	if(!self){
		DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	return audio_webrtc_instance_start_consumer(self->audioInstHandle);
}


static int audio_consumer_webrtc_consume(tmedia_consumer_t* _self, const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
	audio_consumer_webrtc_t* self = (audio_consumer_webrtc_t*)_self;
	if(!self || !buffer || !size){
		DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR("1Invalid parameter");
		return -1;
	}	
	/* buffer is already decoded */
	return tdav_consumer_audio_put(TDAV_CONSUMER_AUDIO(self), buffer, size, proto_hdr);
}

static int audio_consumer_webrtc_pause(tmedia_consumer_t* self)
{
	return 0;
}

static int audio_consumer_webrtc_stop(tmedia_consumer_t* _self)
{
	audio_consumer_webrtc_t* self = (audio_consumer_webrtc_t*)_self;
	if(!self){
		DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	return audio_webrtc_instance_stop_consumer(self->audioInstHandle);
}


//
//	WebRTC audio consumer object definition
//
/* constructor */
static tsk_object_t* audio_consumer_webrtc_ctor(tsk_object_t *_self, va_list * app)
{
	audio_consumer_webrtc_t *self = (audio_consumer_webrtc_t *)_self;
	if(self){
		/* init base */
		tdav_consumer_audio_init(TDAV_CONSUMER_AUDIO(self));
		/* init self */
		
	}
	return self;
}
/* destructor */
static tsk_object_t* audio_consumer_webrtc_dtor(tsk_object_t *_self)
{ 
	audio_consumer_webrtc_t *self = (audio_consumer_webrtc_t *)_self;
	if(self){
		/* stop */
		audio_consumer_webrtc_stop(TMEDIA_CONSUMER(self));
		/* deinit self */
		if(self->audioInstHandle){
			audio_webrtc_instance_destroy(&self->audioInstHandle);
		}
		TSK_FREE(self->buffer.ptr);
		/* deinit base */
		tdav_consumer_audio_deinit(TDAV_CONSUMER_AUDIO(self));
	}

	return self;
}
/* object definition */
static const tsk_object_def_t audio_consumer_webrtc_def_s = 
{
	sizeof(audio_consumer_webrtc_t),
	audio_consumer_webrtc_ctor, 
	audio_consumer_webrtc_dtor,
	tdav_consumer_audio_cmp, 
};
/* plugin definition*/
static const tmedia_consumer_plugin_def_t audio_consumer_webrtc_plugin_def_s = 
{
	&audio_consumer_webrtc_def_s,
	
	tmedia_audio,
	"WebRTC audio consumer",
	
	audio_consumer_webrtc_set,
	audio_consumer_webrtc_prepare,
	audio_consumer_webrtc_start,
	audio_consumer_webrtc_consume,
	audio_consumer_webrtc_pause,
	audio_consumer_webrtc_stop
};
const tmedia_consumer_plugin_def_t *audio_consumer_webrtc_plugin_def_t = &audio_consumer_webrtc_plugin_def_s;
