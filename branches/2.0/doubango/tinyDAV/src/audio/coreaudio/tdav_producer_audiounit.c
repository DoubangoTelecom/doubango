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
#include "tinydav/audio/coreaudio/tdav_producer_audiounit.h"

// http://developer.apple.com/library/ios/#documentation/MusicAudio/Conceptual/AudioUnitHostingGuide_iOS/Introduction/Introduction.html%23//apple_ref/doc/uid/TP40009492-CH1-SW1

#if HAVE_COREAUDIO_AUDIO_UNIT

#include "tsk_debug.h"

static OSStatus __handle_input_buffer(void *inRefCon, 
                                  AudioUnitRenderActionFlags *ioActionFlags, 
                                  const AudioTimeStamp *inTimeStamp, 
                                  UInt32 inBusNumber, 
                                  UInt32 inNumberFrames, 
                                  AudioBufferList *ioData) {
    return noErr;
}

/* ============ Media Producer Interface ================= */
#define tdav_producer_audiounit_set tsk_null

static int tdav_producer_audiounit_prepare(tmedia_producer_t* self, const tmedia_codec_t* codec)
{
	static UInt32 flagOne = 1;
	static UInt32 flagZero = 0;
#define kInputBus  1
	
	tdav_producer_audiounit_t* producer = (tdav_producer_audiounit_t*)self;
	OSStatus status;
	
	if(!producer || !codec || !codec->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(producer->audioUnitHandle){
		TSK_DEBUG_ERROR("Already propared");
		return -2;
	}
	if(!(producer->audioUnitHandle = tdav_audiounit_handle_create(TMEDIA_PRODUCER(producer)->session_id))){
		TSK_DEBUG_ERROR("Failed to get audio unit instance for session with id=%lld", TMEDIA_PRODUCER(producer)->session_id);
		return -3;
	}
	
	// enable
	status = AudioUnitSetProperty(tdav_audiounit_handle_get_instance(producer->audioUnitHandle), 
								  kAudioOutputUnitProperty_EnableIO, 
								  kAudioUnitScope_Input, 
								  kInputBus,
								  &flagOne, 
								  sizeof(flagOne));
	if(status){
		TSK_DEBUG_ERROR("AudioUnitSetProperty(kAudioOutputUnitProperty_EnableIO) failed with status=%d", (int32_t)status);
		return -4;
	}
	else {
		TMEDIA_PRODUCER(producer)->audio.channels = codec->plugin->audio.channels;
		TMEDIA_PRODUCER(producer)->audio.rate = codec->plugin->rate;
		TMEDIA_PRODUCER(producer)->audio.ptime = codec->plugin->audio.ptime;
		/* codec should have ptime */
		
		// set format
		AudioStreamBasicDescription audioFormat;
		audioFormat.mSampleRate = TMEDIA_PRODUCER(producer)->audio.rate;
		audioFormat.mFormatID = kAudioFormatLinearPCM;
		audioFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
		audioFormat.mChannelsPerFrame = TMEDIA_PRODUCER(producer)->audio.channels;
		audioFormat.mFramesPerPacket = 1;
		audioFormat.mBitsPerChannel = TMEDIA_PRODUCER(producer)->audio.bits_per_sample;
		audioFormat.mBytesPerPacket = audioFormat.mBitsPerChannel / 8 * audioFormat.mChannelsPerFrame;
		audioFormat.mBytesPerFrame = audioFormat.mBytesPerPacket;
		audioFormat.mReserved = 0;
		status = AudioUnitSetProperty(tdav_audiounit_handle_get_instance(producer->audioUnitHandle), 
									  kAudioUnitProperty_StreamFormat, 
									  kAudioUnitScope_Output, 
									  kInputBus, 
									  &audioFormat, 
								sizeof(audioFormat));
		if(status){
			TSK_DEBUG_ERROR("AudioUnitSetProperty(kAudioUnitProperty_StreamFormat) failed with status=%d", (int32_t)status);
			return -5;
		}
		else {
			// set callback function
			AURenderCallbackStruct callback;
			callback.inputProc = __handle_input_buffer;
			callback.inputProcRefCon = producer;
			status = AudioUnitSetProperty(tdav_audiounit_handle_get_instance(producer->audioUnitHandle), 
										  kAudioOutputUnitProperty_SetInputCallback, 
										  kAudioUnitScope_Global, 
										  kInputBus, 
										  &callback, 
										  sizeof(callback));
			if(status){
				TSK_DEBUG_ERROR("AudioUnitSetProperty(kAudioOutputUnitProperty_SetInputCallback) failed with status=%d", (int32_t)status);
				return -6;
			}
			else {
				// disbale buffer allocation as we will provide ours
				status = AudioUnitSetProperty(tdav_audiounit_handle_get_instance(producer->audioUnitHandle), 
											  kAudioUnitProperty_ShouldAllocateBuffer,
											  kAudioUnitScope_Output, 
											  kInputBus,
											  &flagZero, 
											  sizeof(flagZero));
			}

		}
	}

	TSK_DEBUG_INFO("AudioUnit producer prepared");
	return tdav_audiounit_handle_signal_producer_prepared(producer->audioUnitHandle);;
}

static int tdav_producer_audiounit_start(tmedia_producer_t* self)
{
	tdav_producer_audiounit_t* producer = (tdav_producer_audiounit_t*)self;
	
    if(!producer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(producer->paused){
		producer->paused = tsk_false;
		return tsk_false;
	}
	if(producer->started){
		TSK_DEBUG_WARN("Already started");
		return 0;
	}
	else {
		int ret = tdav_audiounit_handle_start(producer->audioUnitHandle);
		if(ret){
			TSK_DEBUG_ERROR("tdav_audiounit_handle_start failed with error code=%d", ret);
			return ret;
		}
	}
	producer->started = tsk_true;
	TSK_DEBUG_INFO("AudioUnit producer started");
	return 0;
}

static int tdav_producer_audiounit_pause(tmedia_producer_t* self)
{
    tdav_producer_audiounit_t* producer = (tdav_producer_audiounit_t*)self;
    if(!producer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	producer->paused = tsk_true;
	TSK_DEBUG_INFO("AudioUnit producer paused");
	return 0;
}

static int tdav_producer_audiounit_stop(tmedia_producer_t* self)
{
    tdav_producer_audiounit_t* producer = (tdav_producer_audiounit_t*)self;
	
    if(!producer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	if(!producer->started){
		TSK_DEBUG_WARN("Not started started");
		return 0;
	}
	else {
		int ret = tdav_audiounit_handle_stop(producer->audioUnitHandle);
		if(ret){
			TSK_DEBUG_ERROR("tdav_audiounit_handle_stop failed with error code=%d", ret);
			return ret;
		}
	}	
	producer->started = tsk_false;
	TSK_DEBUG_INFO("AudioUnit producer stoppped");
	return 0;	
}


//
//	CoreAudio producer object definition
//
/* constructor */
static tsk_object_t* tdav_producer_audiounit_ctor(tsk_object_t * self, va_list * app)
{
	tdav_producer_audiounit_t *producer = self;
	if(producer){
		/* init base */
		tdav_producer_audio_init(TDAV_PRODUCER_AUDIO(producer));
		/* init self */  
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_producer_audiounit_dtor(tsk_object_t * self)
{ 
	tdav_producer_audiounit_t *producer = self;
	if(producer){
		// Stop the producer if not done
		if(producer->started){
			tdav_producer_audiounit_stop(self);
		}
		
		// Free all buffers and dispose the queue
        if (producer->audioUnitHandle) {
			tdav_audiounit_handle_destroy(&producer->audioUnitHandle);
        }
        
		/* deinit base */
		tdav_producer_audio_deinit(TDAV_PRODUCER_AUDIO(producer));
	}
	
	return self;
}
/* object definition */
static const tsk_object_def_t tdav_producer_audiounit_def_s = 
{
	sizeof(tdav_producer_audiounit_t),
	tdav_producer_audiounit_ctor, 
	tdav_producer_audiounit_dtor,
	tdav_producer_audio_cmp, 
};
/* plugin definition*/
static const tmedia_producer_plugin_def_t tdav_producer_audiounit_plugin_def_s = 
{
	&tdav_producer_audiounit_def_s,
	
	tmedia_audio,
	"Apple CoreAudio producer (AudioUnit)",
	
	tdav_producer_audiounit_set,
	tdav_producer_audiounit_prepare,
	tdav_producer_audiounit_start,
	tdav_producer_audiounit_pause,
	tdav_producer_audiounit_stop
};
const tmedia_producer_plugin_def_t *tdav_producer_audiounit_plugin_def_t = &tdav_producer_audiounit_plugin_def_s;


#endif /* HAVE_COREAUDIO_AUDIO_UNIT */
