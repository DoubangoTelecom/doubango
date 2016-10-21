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

#include <mach/mach.h>
#import <sys/sysctl.h>

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_thread.h"
#include "tsk_debug.h"

#define kRingPacketCount 10

static int tdav_producer_audiounit_pause(tmedia_producer_t* self);
static int tdav_producer_audiounit_resume(tmedia_producer_t* self);
static int tdav_producer_audiounit_init(tmedia_producer_t* self);
static int tdav_producer_audiounit_deinit(tmedia_producer_t* self);

static OSStatus __handle_input_buffer(void *inRefCon,
                                      AudioUnitRenderActionFlags *ioActionFlags,
                                      const AudioTimeStamp *inTimeStamp,
                                      UInt32 inBusNumber,
                                      UInt32 inNumberFrames,
                                      AudioBufferList *ioData)
{
    OSStatus status = noErr;
    tdav_producer_audiounit_t* producer = (tdav_producer_audiounit_t*)inRefCon;

    // holder
    AudioBuffer buffer;
    buffer.mData =  tsk_null;
    buffer.mDataByteSize = 0;
    buffer.mNumberChannels = TMEDIA_PRODUCER(producer)->audio.channels;

    // list of holders
    AudioBufferList buffers;
    buffers.mNumberBuffers = 1;
    buffers.mBuffers[0] = buffer;

    // render to get frames from the system
    status = AudioUnitRender(tdav_audiounit_handle_get_instance(producer->audioUnitHandle),
                             ioActionFlags,
                             inTimeStamp,
                             inBusNumber,
                             inNumberFrames,
                             &buffers);
    if(status == 0) {
        // must not be done on async thread: doing it gives bad audio quality when audio+video call is done with CPU consuming codec (e.g. speex or g729)
        speex_buffer_write(producer->ring.buffer, buffers.mBuffers[0].mData, buffers.mBuffers[0].mDataByteSize);
        int avail = speex_buffer_get_available(producer->ring.buffer);
        while (producer->started && avail >= producer->ring.chunck.size) {
            avail -= speex_buffer_read(producer->ring.buffer, (void*)producer->ring.chunck.buffer, (int)producer->ring.chunck.size);
            TMEDIA_PRODUCER(producer)->enc_cb.callback(TMEDIA_PRODUCER(producer)->enc_cb.callback_data,
                    producer->ring.chunck.buffer, producer->ring.chunck.size);
        }
    }

    return status;
}

/* ============ Media Producer Interface ================= */
int tdav_producer_audiounit_set(tmedia_producer_t* self, const tmedia_param_t* param)
{
    tdav_producer_audiounit_t* producer = (tdav_producer_audiounit_t*)self;
    if(param->plugin_type == tmedia_ppt_producer) {
        if(param->value_type == tmedia_pvt_int32) {
            if (tsk_striequals(param->key, "mute")) {
                producer->muted = TSK_TO_INT32((uint8_t*)param->value);
                return tdav_audiounit_handle_mute(((tdav_producer_audiounit_t*)self)->audioUnitHandle, producer->muted);
            }
            else if (tsk_striequals(param->key, "interrupt")) {
                int32_t interrupt = *((uint8_t*)param->value) ? 1 : 0;
                return tdav_audiounit_handle_interrupt(producer->audioUnitHandle, interrupt);
            }
            else if (tsk_striequals(param->key, "pause") || tsk_striequals(param->key, "hold")) {
                int32_t pause = *((uint8_t*)param->value) ? 1 : 0;
                return pause ? tdav_producer_audiounit_pause(self) : tdav_producer_audiounit_resume(self);
            }
        }
    }
    return tdav_producer_audio_set(TDAV_PRODUCER_AUDIO(self), param);
}

static int tdav_producer_audiounit_prepare(tmedia_producer_t* self, const tmedia_codec_t* codec)
{
    tdav_producer_audiounit_t* producer = (tdav_producer_audiounit_t*)self;
    if (!producer || !codec || !codec->plugin) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    
    TMEDIA_PRODUCER(producer)->audio.channels = TMEDIA_CODEC_CHANNELS_AUDIO_ENCODING(codec);
    TMEDIA_PRODUCER(producer)->audio.rate = TMEDIA_CODEC_RATE_ENCODING(codec);
    TMEDIA_PRODUCER(producer)->audio.ptime = TMEDIA_CODEC_PTIME_AUDIO_ENCODING(codec);
    
    TSK_DEBUG_INFO("AudioUnit producer prepared(channels=%d, rate=%d, ptime=%d)",
                   (int)TMEDIA_PRODUCER(producer)->audio.channels,
                   (int)TMEDIA_PRODUCER(producer)->audio.rate,
                   (int)TMEDIA_PRODUCER(producer)->audio.ptime);
    return 0;
}

static int tdav_producer_audiounit_start(tmedia_producer_t* self)
{
    tdav_producer_audiounit_t* producer = (tdav_producer_audiounit_t*)self;

    if (!producer) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (producer->paused) {
        producer->paused = tsk_false;
        return tsk_false;
    }

    int ret;
    if (producer->started) {
        TSK_DEBUG_WARN("Already started");
        return 0;
    }
    else {
        // Initialize the consumer if not already done
        if (!producer->ready) {
            ret = tdav_producer_audiounit_init(self);
            if (ret) {
                tdav_producer_audiounit_deinit(self);
                TSK_DEBUG_ERROR("tdav_producer_audiounit_init failed with error code=%d", ret);
                return ret;
            }
        }
        // Start handle
        ret = tdav_audiounit_handle_start(producer->audioUnitHandle);
        if(ret) {
            TSK_DEBUG_ERROR("tdav_audiounit_handle_start failed with error code=%d", ret);
            return ret;
        }
    }
    producer->started = tsk_true;

    // apply parameters (because could be lost when the producer is restarted -handle recreated-)
    ret = tdav_audiounit_handle_mute(producer->audioUnitHandle, producer->muted);

    TSK_DEBUG_INFO("AudioUnit producer started");
    return 0;
}

static int tdav_producer_audiounit_pause(tmedia_producer_t* self)
{
    tdav_producer_audiounit_t* producer = (tdav_producer_audiounit_t*)self;
    if (!producer) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (!producer->paused) {
        tdav_producer_audiounit_deinit(self);
        producer->paused = tsk_true;
    }
    
    TSK_DEBUG_INFO("AudioUnit producer paused");
    return 0;
}

static int tdav_producer_audiounit_resume(tmedia_producer_t* self)
{
    tdav_producer_audiounit_t* producer = (tdav_producer_audiounit_t*)self;
    if (!producer) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    if (producer->paused) {
        producer->paused = tsk_false; // *must* be  before start()
        if (!producer->started) {
            int ret = tdav_producer_audiounit_start(self);
            if (ret) {
                TSK_DEBUG_ERROR("Failed to stop the consumer");
                return ret;
            }
        }
    }
    
    TSK_DEBUG_INFO("AudioUnit producer resumed");
    return 0;
}

static int tdav_producer_audiounit_stop(tmedia_producer_t* self)
{
    int ret = tdav_producer_audiounit_deinit(self);
    if (ret) {
        TSK_DEBUG_ERROR("Failed to stop the consumer");
        return ret;
    }
    TSK_DEBUG_INFO("AudioUnit producer stoppped");
    return 0;
}

static int tdav_producer_audiounit_init(tmedia_producer_t* self)
{
    static UInt32 flagOne = 1;
    UInt32 param;
    // static UInt32 flagZero = 0;
#define kInputBus  1
    
    tdav_producer_audiounit_t* producer = (tdav_producer_audiounit_t*)self;
    OSStatus status = noErr;
    AudioStreamBasicDescription audioFormat;
    AudioStreamBasicDescription	deviceFormat;
    
    if (!producer) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    
    if (producer->ready) {
        TSK_DEBUG_INFO("AudioUnit consumer already initialized");
        return 0;
    }
    
    if (!producer->audioUnitHandle) {
        if(!(producer->audioUnitHandle = tdav_audiounit_handle_create(TMEDIA_PRODUCER(producer)->session_id))) {
            TSK_DEBUG_ERROR("Failed to get audio unit instance for session with id=%lld", TMEDIA_PRODUCER(producer)->session_id);
            return -3;
        }
    }
    
    // enable
    status = AudioUnitSetProperty(tdav_audiounit_handle_get_instance(producer->audioUnitHandle),
                                  kAudioOutputUnitProperty_EnableIO,
                                  kAudioUnitScope_Input,
                                  kInputBus,
                                  &flagOne,
                                  sizeof(flagOne));
    if (status != noErr) {
        TSK_DEBUG_ERROR("AudioUnitSetProperty(kAudioOutputUnitProperty_EnableIO) failed with status=%ld", (signed long)status);
        return -4;
    }
    else {
#if !TARGET_OS_IPHONE // strange: TARGET_OS_MAC is equal to '1' on Smulator
        // disable output
        param = 0;
        status = AudioUnitSetProperty(tdav_audiounit_handle_get_instance(producer->audioUnitHandle),
                                      kAudioOutputUnitProperty_EnableIO,
                                      kAudioUnitScope_Output,
                                      0,
                                      &param,
                                      sizeof(UInt32));
        if(status != noErr) {
            TSK_DEBUG_ERROR("AudioUnitSetProperty(kAudioOutputUnitProperty_EnableIO) failed with status=%ld", (signed long)status);
            return -4;
        }
        
        // set default audio device
        param = sizeof(AudioDeviceID);
        AudioDeviceID inputDeviceID;
        status = AudioHardwareGetProperty(kAudioHardwarePropertyDefaultInputDevice, &param, &inputDeviceID);
        if(status != noErr) {
            TSK_DEBUG_ERROR("AudioHardwareGetProperty(kAudioHardwarePropertyDefaultInputDevice) failed with status=%ld", (signed long)status);
            return -4;
        }
        
        // set the current device to the default input unit
        status = AudioUnitSetProperty(tdav_audiounit_handle_get_instance(producer->audioUnitHandle),
                                      kAudioOutputUnitProperty_CurrentDevice,
                                      kAudioUnitScope_Output,
                                      0,
                                      &inputDeviceID,
                                      sizeof(AudioDeviceID));
        if(status != noErr) {
            TSK_DEBUG_ERROR("AudioUnitSetProperty(kAudioOutputUnitProperty_CurrentDevice) failed with status=%ld", (signed long)status);
            return -4;
        }
#endif /* TARGET_OS_MAC */
        
        TSK_DEBUG_INFO("AudioUnit producer: channels=%d, rate=%d, ptime=%d",
                       TMEDIA_PRODUCER(producer)->audio.channels,
                       TMEDIA_PRODUCER(producer)->audio.rate,
                       TMEDIA_PRODUCER(producer)->audio.ptime);
        
        // get device format
        param = sizeof(AudioStreamBasicDescription);
        status = AudioUnitGetProperty(tdav_audiounit_handle_get_instance(producer->audioUnitHandle),
                                      kAudioUnitProperty_StreamFormat,
                                      kAudioUnitScope_Input,
                                      kInputBus,
                                      &deviceFormat, &param);
        if(status == noErr && deviceFormat.mSampleRate) {
#if TARGET_OS_IPHONE
            // iOS support 8Khz, 16kHz and 32kHz => do not override the sampleRate
#elif TARGET_OS_MAC
            // For example, iSight supports only 48kHz
            TMEDIA_PRODUCER(producer)->audio.rate = deviceFormat.mSampleRate;
#endif
        }
        
        // set format
        audioFormat.mSampleRate = TMEDIA_PRODUCER(producer)->audio.rate;
        audioFormat.mFormatID = kAudioFormatLinearPCM;
        audioFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked | kAudioFormatFlagIsNonInterleaved;
        audioFormat.mChannelsPerFrame = TMEDIA_PRODUCER(producer)->audio.channels;
        audioFormat.mFramesPerPacket = 1;
        audioFormat.mBitsPerChannel = TMEDIA_PRODUCER(producer)->audio.bits_per_sample;
        audioFormat.mBytesPerPacket = audioFormat.mBitsPerChannel / 8 * audioFormat.mChannelsPerFrame;
        audioFormat.mBytesPerFrame = audioFormat.mBytesPerPacket;
        audioFormat.mReserved = 0;
        if(audioFormat.mFormatID == kAudioFormatLinearPCM && audioFormat.mChannelsPerFrame  == 1) {
            audioFormat.mFormatFlags &= ~kLinearPCMFormatFlagIsNonInterleaved;
        }
        status = AudioUnitSetProperty(tdav_audiounit_handle_get_instance(producer->audioUnitHandle),
                                      kAudioUnitProperty_StreamFormat,
                                      kAudioUnitScope_Output,
                                      kInputBus,
                                      &audioFormat,
                                      sizeof(audioFormat));
        if(status) {
            TSK_DEBUG_ERROR("AudioUnitSetProperty(kAudioUnitProperty_StreamFormat) failed with status=%ld", (signed long)status);
            return -5;
        }
        else {
            
            // configure
            if (tdav_audiounit_handle_configure(producer->audioUnitHandle, tsk_false, TMEDIA_PRODUCER(producer)->audio.ptime, &audioFormat)) {
                TSK_DEBUG_ERROR("tdav_audiounit_handle_set_rate(%d) failed", TMEDIA_PRODUCER(producer)->audio.rate);
                return -4;
            }
            
            // set callback function
            AURenderCallbackStruct callback;
            callback.inputProc = __handle_input_buffer;
            callback.inputProcRefCon = producer;
            status = AudioUnitSetProperty(tdav_audiounit_handle_get_instance(producer->audioUnitHandle),
                                          kAudioOutputUnitProperty_SetInputCallback,
                                          kAudioUnitScope_Output,
                                          kInputBus,
                                          &callback,
                                          sizeof(callback));
            if(status) {
                TSK_DEBUG_ERROR("AudioUnitSetProperty(kAudioOutputUnitProperty_SetInputCallback) failed with status=%ld", (signed long)status);
                return -6;
            }
            else {
                // disbale buffer allocation as we will provide ours
                //status = AudioUnitSetProperty(tdav_audiounit_handle_get_instance(producer->audioUnitHandle),
                //							  kAudioUnitProperty_ShouldAllocateBuffer,
                //							  kAudioUnitScope_Output,
                //							  kInputBus,
                //							  &flagZero,
                //							  sizeof(flagZero));
                
                producer->ring.chunck.size = (TMEDIA_PRODUCER(producer)->audio.ptime * audioFormat.mSampleRate * audioFormat.mBytesPerFrame) / 1000;
                // allocate our chunck buffer
                if(!(producer->ring.chunck.buffer = tsk_realloc(producer->ring.chunck.buffer, producer->ring.chunck.size))) {
                    TSK_DEBUG_ERROR("Failed to allocate new buffer");
                    return -7;
                }
                // create ringbuffer
                producer->ring.size = kRingPacketCount * producer->ring.chunck.size;
                if(!producer->ring.buffer) {
                    producer->ring.buffer = speex_buffer_init((int)producer->ring.size);
                }
                else {
                    int ret;
                    if((ret = speex_buffer_resize(producer->ring.buffer, (int)producer->ring.size)) < 0) {
                        TSK_DEBUG_ERROR("speex_buffer_resize(%d) failed with error code=%d", (int)producer->ring.size, ret);
                        return ret;
                    }
                }
                if(!producer->ring.buffer) {
                    TSK_DEBUG_ERROR("Failed to create a new ring buffer with size = %d", (int)producer->ring.size);
                    return -9;
                }
            }
            
        }
    }
    
    if (tdav_audiounit_handle_signal_producer_ready(producer->audioUnitHandle)) {
        TSK_DEBUG_ERROR("tdav_audiounit_handle_signal_producer_ready failed");
        return -10;
    }
    
    producer->ready = tsk_true;
    
    TSK_DEBUG_INFO("AudioUnit producer initialized");
    return 0;
}

static int tdav_producer_audiounit_deinit(tmedia_producer_t* self)
{
    tdav_producer_audiounit_t* producer = (tdav_producer_audiounit_t*)self;
    if (!producer) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    // Stop
    if (producer->started) {
        int ret = tdav_audiounit_handle_stop(producer->audioUnitHandle);
        if (ret) {
            TSK_DEBUG_ERROR("tdav_audiounit_handle_stop failed with error code=%d", ret);
        }
        producer->started = tsk_false;
    }
    // Destroy handle (will be re-created by the next start)
#if TARGET_OS_IPHONE
    //https://devforums.apple.com/thread/118595
    if (producer->audioUnitHandle) {
        tdav_audiounit_handle_destroy(&producer->audioUnitHandle);
    }
#endif
    producer->ready = tsk_false;
    producer->paused = tsk_false;
    
    TSK_DEBUG_INFO("AudioUnit producer deinitialized");
    
    return 0;
}


//
//	CoreAudio producer object definition
//
/* constructor */
static tsk_object_t* tdav_producer_audiounit_ctor(tsk_object_t * self, va_list * app)
{
    tdav_producer_audiounit_t *producer = self;
    if(producer) {
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
    if(producer) {
        tdav_producer_audiounit_deinit(TMEDIA_PRODUCER(self));
        TSK_FREE(producer->ring.chunck.buffer);
        if(producer->ring.buffer) {
            speex_buffer_destroy(producer->ring.buffer);
        }
        /* deinit base */
        tdav_producer_audio_deinit(TDAV_PRODUCER_AUDIO(producer));

        TSK_DEBUG_INFO("*** AudioUnit Producer destroyed ***");
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_producer_audiounit_def_s = {
    sizeof(tdav_producer_audiounit_t),
    tdav_producer_audiounit_ctor,
    tdav_producer_audiounit_dtor,
    tdav_producer_audio_cmp,
};
/* plugin definition*/
static const tmedia_producer_plugin_def_t tdav_producer_audiounit_plugin_def_s = {
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
