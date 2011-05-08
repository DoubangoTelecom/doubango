/*
 * Copyright (C) 2010-2011 Mamadou Diop.
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
#include "tinydav/audio/coreaudio/tdav_consumer_audiounit.h"

// http://developer.apple.com/library/ios/#documentation/MusicAudio/Conceptual/AudioUnitHostingGuide_iOS/Introduction/Introduction.html%23//apple_ref/doc/uid/TP40009492-CH1-SW1

#if HAVE_COREAUDIO_AUDIO_UNIT

// AudioUnit already contains denoiser
#if HAVE_SPEEX_DSP && (!defined(HAVE_SPEEX_DENOISE) || HAVE_SPEEX_DENOISE)
#	error "AudioUnit already contain denoiser => Disable it"
#endif

#undef DISABLE_JITTER_BUFFER
#define DISABLE_JITTER_BUFFER	0 // FIXME: Find why there is too delay when we use speex jitter buffer

#include "tsk_debug.h"
#include "tsk_memory.h"

#define kNoDataError		-1
#define kRingPacketCount	+5

static tsk_size_t tdav_consumer_audiounit_get(tdav_consumer_audiounit_t* self, void* data, tsk_size_t size);

static OSStatus __handle_output_buffer(void *inRefCon, 
								 AudioUnitRenderActionFlags *ioActionFlags, 
								 const AudioTimeStamp *inTimeStamp, 
								 UInt32 inBusNumber, 
								 UInt32 inNumberFrames, 
								 AudioBufferList *ioData) {
	OSStatus status = noErr;
	// tsk_size_t out_size;
	tdav_consumer_audiounit_t* consumer = (tdav_consumer_audiounit_t* )inRefCon;
	
	if(!consumer->started || consumer->paused){
		goto done;
	}
	
	if(!ioData){
		TSK_DEBUG_ERROR("Invalid argument");
		status = kNoDataError;
		goto done;
	}
	// read from jitter buffer and fill ioData buffers
	for(int i=0; i<ioData->mNumberBuffers; i++){
		/* int ret = */ tdav_consumer_audiounit_get(consumer, ioData->mBuffers[i].mData, ioData->mBuffers[i].mDataByteSize);
	}
	
done:	
    return status;
}

static tsk_size_t tdav_consumer_audiounit_get(tdav_consumer_audiounit_t* self, void* data, tsk_size_t size)
{
	tsk_ssize_t retSize = 0;
#if DISABLE_JITTER_BUFFER
	retSize = speex_buffer_read(self->ring.buffer, data, size);
	if(retSize < size){
		memset(((uint8_t*)data)+retSize, 0, (size - retSize));
	}
#else
	self->ring.leftBytes += size;
	while (self->ring.leftBytes >= self->ring.chunck.size) {
		self->ring.leftBytes -= self->ring.chunck.size;
		retSize =  (tsk_ssize_t)tdav_consumer_audio_get(TDAV_CONSUMER_AUDIO(self), self->ring.chunck.buffer, self->ring.chunck.size);
		tdav_consumer_audio_tick(TDAV_CONSUMER_AUDIO(self));
		speex_buffer_write(self->ring.buffer, self->ring.chunck.buffer, retSize);
	}
	retSize = speex_buffer_read(self->ring.buffer, data, size);
	if(retSize < size){
		memset(((uint8_t*)data)+retSize, 0, (size - retSize));
	}
#endif

	return retSize;
}

/* ============ Media Consumer Interface ================= */
#define tdav_consumer_audiounit_set tsk_null

static int tdav_consumer_audiounit_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
	static UInt32 flagOne = 1;
	AudioStreamBasicDescription audioFormat;
#define kOutputBus  0
	
	tdav_consumer_audiounit_t* consumer = (tdav_consumer_audiounit_t*)self;
	OSStatus status;
	
	if(!consumer || !codec || !codec->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(consumer->audioUnitHandle){
		TSK_DEBUG_ERROR("Already propared");
		return -2;
	}
	if(!(consumer->audioUnitHandle = tdav_audiounit_handle_create(TMEDIA_CONSUMER(consumer)->session_id, codec->plugin->audio.ptime))){
		TSK_DEBUG_ERROR("Failed to get audio unit instance for session with id=%lld", TMEDIA_CONSUMER(consumer)->session_id);
		return -3;
	}
	
	// enable
	status = AudioUnitSetProperty(tdav_audiounit_handle_get_instance(consumer->audioUnitHandle), 
								  kAudioOutputUnitProperty_EnableIO, 
								  kAudioUnitScope_Output, 
								  kOutputBus,
								  &flagOne, 
								  sizeof(flagOne));
	if(status){
		TSK_DEBUG_ERROR("AudioUnitSetProperty(kAudioOutputUnitProperty_EnableIO) failed with status=%d", (int32_t)status);
		return -4;
	}
	else {
		TMEDIA_CONSUMER(consumer)->audio.ptime = codec->plugin->audio.ptime;
		TMEDIA_CONSUMER(consumer)->audio.in.channels = codec->plugin->audio.channels;
		TMEDIA_CONSUMER(consumer)->audio.in.rate = codec->plugin->rate;
		
		// set format
		audioFormat.mSampleRate = TMEDIA_CONSUMER(consumer)->audio.out.rate ? TMEDIA_CONSUMER(consumer)->audio.out.rate : TMEDIA_CONSUMER(consumer)->audio.in.rate;
		audioFormat.mFormatID = kAudioFormatLinearPCM;
		audioFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
		audioFormat.mChannelsPerFrame = TMEDIA_CONSUMER(consumer)->audio.in.channels;
		audioFormat.mFramesPerPacket = 1;
		audioFormat.mBitsPerChannel = TMEDIA_CONSUMER(consumer)->audio.bits_per_sample;
		audioFormat.mBytesPerPacket = audioFormat.mBitsPerChannel / 8 * audioFormat.mChannelsPerFrame;
		audioFormat.mBytesPerFrame = audioFormat.mBytesPerPacket;
		audioFormat.mReserved = 0;
		status = AudioUnitSetProperty(tdav_audiounit_handle_get_instance(consumer->audioUnitHandle), 
									  kAudioUnitProperty_StreamFormat, 
									  kAudioUnitScope_Input, 
									  kOutputBus, 
									  &audioFormat, 
									  sizeof(audioFormat));
		
		if(status){
			TSK_DEBUG_ERROR("AudioUnitSetProperty(kAudioUnitProperty_StreamFormat) failed with status=%d", (int32_t)status);
			return -5;
		}
		else {
			// set callback function
			AURenderCallbackStruct callback;
			callback.inputProc = __handle_output_buffer;
			callback.inputProcRefCon = consumer;
			status = AudioUnitSetProperty(tdav_audiounit_handle_get_instance(consumer->audioUnitHandle), 
										  kAudioUnitProperty_SetRenderCallback, 
										  kAudioUnitScope_Global, 
										  kOutputBus,
										  &callback, 
										  sizeof(callback));
			if(status){
				TSK_DEBUG_ERROR("AudioUnitSetProperty(kAudioOutputUnitProperty_SetInputCallback) failed with status=%d", (int32_t)status);
				return -6;
			}
		}
	}
	
	// allocate the chunck buffer and create the ring
	int packetperbuffer = (1000 / TMEDIA_CONSUMER(consumer)->audio.ptime);
	consumer->ring.chunck.size = audioFormat.mSampleRate * audioFormat.mBytesPerFrame / packetperbuffer;
	consumer->ring.size = kRingPacketCount * consumer->ring.chunck.size;
	if(!(consumer->ring.chunck.buffer = tsk_realloc(consumer->ring.chunck.buffer, consumer->ring.chunck.size))){
		TSK_DEBUG_ERROR("Failed to allocate new buffer");
		return -7;
	}
	if(!consumer->ring.buffer){
		consumer->ring.buffer = speex_buffer_init(consumer->ring.size);
	}
	else {
		int ret;
		if((ret = speex_buffer_resize(consumer->ring.buffer, consumer->ring.size))){
			TSK_DEBUG_ERROR("speex_buffer_resize(%d) failed with error code=%d", consumer->ring.size, ret);
			return ret;
		}
	}
	if(!consumer->ring.buffer){
		TSK_DEBUG_ERROR("Failed to create a new ring buffer with size = %d", consumer->ring.size);
		return -8;
	}
	if(!(consumer->ring.mutex = tsk_mutex_create_2(tsk_false))){
		TSK_DEBUG_ERROR("Failed to create mutex");
		return -9;
	}

	// set maximum frames per slice as buffer size
	//UInt32 numFrames = (UInt32)consumer->ring.chunck.size;
	//status = AudioUnitSetProperty(tdav_audiounit_handle_get_instance(consumer->audioUnitHandle), 
	//							  kAudioUnitProperty_MaximumFramesPerSlice,
	//							  kAudioUnitScope_Global, 
	//							  0, 
	//							  &numFrames, 
	//							  sizeof(numFrames));
	//if(status){
	//	TSK_DEBUG_ERROR("AudioUnitSetProperty(kAudioUnitProperty_MaximumFramesPerSlice, %u) failed with status=%d", (unsigned)numFrames, (int32_t)status);
	//	return -6;
	//}
	
	TSK_DEBUG_INFO("AudioUnit producer prepared");
    return tdav_audiounit_handle_signal_consumer_prepared(consumer->audioUnitHandle);
}

static int tdav_consumer_audiounit_start(tmedia_consumer_t* self)
{
	tdav_consumer_audiounit_t* consumer = (tdav_consumer_audiounit_t*)self;
	
    if(!consumer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(consumer->paused){
		consumer->paused = tsk_false;
		return tsk_false;
	}
	if(consumer->started){
		TSK_DEBUG_WARN("Already started");
		return 0;
	}
	else {
		int ret = tdav_audiounit_handle_start(consumer->audioUnitHandle);
		if(ret){
			TSK_DEBUG_ERROR("tdav_audiounit_handle_start failed with error code=%d", ret);
			return ret;
		}
	}
	consumer->started = tsk_true;
	TSK_DEBUG_INFO("AudioUnit consumer started");
	return 0;
}

static int tdav_consumer_audiounit_consume(tmedia_consumer_t* self, const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{	
	tdav_consumer_audiounit_t* consumer = (tdav_consumer_audiounit_t*)self;
	if(!consumer || !buffer || !size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
#if DISABLE_JITTER_BUFFER
	{
		if(consumer->ring.buffer){
			tsk_mutex_lock(consumer->ring.mutex);
			speex_buffer_write(consumer->ring.buffer, (void*)buffer, size);
			tsk_mutex_unlock(consumer->ring.mutex);
			return 0;
		}
		return -2;
	}
#else
	{
		return tdav_consumer_audio_put(TDAV_CONSUMER_AUDIO(consumer), buffer, size, proto_hdr);
	}
#endif
}

static int tdav_consumer_audiounit_pause(tmedia_consumer_t* self)
{
	tdav_consumer_audiounit_t* consumer = (tdav_consumer_audiounit_t*)self;
    if(!consumer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	consumer->paused = tsk_true;
	TSK_DEBUG_INFO("AudioUnit producer paused");
	return 0;
}

static int tdav_consumer_audiounit_stop(tmedia_consumer_t* self)
{
	tdav_consumer_audiounit_t* consumer = (tdav_consumer_audiounit_t*)self;
	
    if(!consumer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(!consumer->started){
		TSK_DEBUG_WARN("Not started started");
		return 0;
	}
	else {
		int ret = tdav_audiounit_handle_stop(consumer->audioUnitHandle);
		if(ret){
			TSK_DEBUG_ERROR("tdav_audiounit_handle_stop failed with error code=%d", ret);
			return ret;
		}
	}	
	consumer->started = tsk_false;
	TSK_DEBUG_INFO("AudioUnit consumer stoppped");
	return 0;	
	
}

//
//	coreaudio consumer (AudioUnit) object definition
//
/* constructor */
static tsk_object_t* tdav_consumer_audiounit_ctor(tsk_object_t * self, va_list * app)
{
	tdav_consumer_audiounit_t *consumer = self;
	if(consumer){
		/* init base */
		tdav_consumer_audio_init(TDAV_CONSUMER_AUDIO(consumer));
		/* init self */
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_consumer_audiounit_dtor(tsk_object_t * self)
{ 
	tdav_consumer_audiounit_t *consumer = self;
	if(consumer){
		/* deinit self */
		// Stop the consumer if not done
		if(consumer->started){
			tdav_consumer_audiounit_stop(self);
		}
		// destroy handle
		if(consumer->audioUnitHandle){
			tdav_audiounit_handle_destroy(&consumer->audioUnitHandle);
		}
		TSK_FREE(consumer->ring.chunck.buffer);
		if(consumer->ring.buffer){
			speex_buffer_destroy(consumer->ring.buffer);
		}
		if(consumer->ring.mutex){
			tsk_mutex_destroy(&consumer->ring.mutex);
		}
        
		/* deinit base */
		tdav_consumer_audio_deinit(TDAV_CONSUMER_AUDIO(consumer));
	}
	
	return self;
}

/* object definition */
static const tsk_object_def_t tdav_consumer_audiounit_def_s = 
{
	sizeof(tdav_consumer_audiounit_t),
	tdav_consumer_audiounit_ctor, 
	tdav_consumer_audiounit_dtor,
	tdav_consumer_audio_cmp, 
};

/* plugin definition*/
static const tmedia_consumer_plugin_def_t tdav_consumer_audiounit_plugin_def_s = 
{
	&tdav_consumer_audiounit_def_s,
	
	tmedia_audio,
	"Apple CoreAudio consumer(AudioUnit)",
	
	tdav_consumer_audiounit_set,
	tdav_consumer_audiounit_prepare,
	tdav_consumer_audiounit_start,
	tdav_consumer_audiounit_consume,
	tdav_consumer_audiounit_pause,
	tdav_consumer_audiounit_stop
};

const tmedia_consumer_plugin_def_t *tdav_consumer_audiounit_plugin_def_t = &tdav_consumer_audiounit_plugin_def_s;

#endif /* HAVE_COREAUDIO_AUDIO_UNIT */
