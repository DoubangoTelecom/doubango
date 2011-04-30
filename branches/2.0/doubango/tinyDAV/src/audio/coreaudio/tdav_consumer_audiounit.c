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

#include "tsk_debug.h"

static OSStatus __handle_output_buffer(void *inRefCon, 
								 AudioUnitRenderActionFlags *ioActionFlags, 
								 const AudioTimeStamp *inTimeStamp, 
								 UInt32 inBusNumber, 
								 UInt32 inNumberFrames, 
								 AudioBufferList *ioData) {
    return noErr;
}

/* ============ Media Consumer Interface ================= */
#define tdav_consumer_audiounit_set tsk_null

static int tdav_consumer_audiounit_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
	static UInt32 flagOne = 1;
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
	if(!(consumer->audioUnitHandle = tdav_audiounit_handle_create(TMEDIA_CONSUMER(consumer)->session_id))){
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
		AudioStreamBasicDescription audioFormat;
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
	TSK_DEBUG_ERROR("Not implemented");
	return -1;
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
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_consumer_audiounit_dtor(tsk_object_t * self)
{ 
	tdav_consumer_audiounit_t *consumer = self;
	if(consumer){
		// Stop the consumer if not done
		if(consumer->started){
			tdav_consumer_audiounit_stop(self);
		}
		// destroy handle
		if(consumer->audioUnitHandle){
			tdav_audiounit_handle_destroy(&consumer->audioUnitHandle);
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
