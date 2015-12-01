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

/**@file tdav_producer_audioqueue.c
 * @brief Audio Producer for MacOSX and iOS platforms using AudioQueue.
 *
 * @authors 
 *	- Laurent Etiemble <laurent.etiemble(at)gmail.com>
 *	- Mamadou Diop <diopmamadou(at)doubango(dot)org>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 letiemble
 */
#include "tinydav/audio/coreaudio/tdav_producer_audioqueue.h"


// http://developer.apple.com/library/mac/#documentation/MusicAudio/Reference/AudioQueueReference/Reference/reference.html

#if HAVE_COREAUDIO_AUDIO_QUEUE

#include "tsk_string.h"
#include "tsk_thread.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

static void __handle_input_buffer (void *userdata, AudioQueueRef queue, AudioQueueBufferRef buffer, const AudioTimeStamp *start_time, UInt32 number_packet_descriptions, const AudioStreamPacketDescription *packet_descriptions ) {
	tdav_producer_audioqueue_t* producer = (tdav_producer_audioqueue_t*)userdata;
    
    if (!producer->started) {
        return;
    }
	
	// Alert the session that there is new data to send
	if(TMEDIA_PRODUCER(producer)->enc_cb.callback) {
		TMEDIA_PRODUCER(producer)->enc_cb.callback(TMEDIA_PRODUCER(producer)->enc_cb.callback_data, buffer->mAudioData, buffer->mAudioDataByteSize);
	}
    
    // Re-enqueue the buffer
    AudioQueueEnqueueBuffer(producer->queue, buffer, 0, NULL);
}

/* ============ Media Producer Interface ================= */
#define tdav_producer_audioqueue_set tsk_null

static int tdav_producer_audioqueue_prepare(tmedia_producer_t* self, const tmedia_codec_t* codec)
{
    OSStatus ret;
	tsk_size_t i;
	tdav_producer_audioqueue_t* producer = (tdav_producer_audioqueue_t*)self;
	
	if(!producer || !codec && codec->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	TMEDIA_PRODUCER(producer)->audio.channels = TMEDIA_CODEC_CHANNELS_AUDIO_ENCODING(codec);
	TMEDIA_PRODUCER(producer)->audio.rate = TMEDIA_CODEC_RATE_ENCODING(codec);
	TMEDIA_PRODUCER(producer)->audio.ptime = TMEDIA_CODEC_PTIME_AUDIO_ENCODING(codec);
	/* codec should have ptime */
	
	
	// Set audio category
#if TARGET_OS_IPHONE
	UInt32 category = kAudioSessionCategory_PlayAndRecord;
	AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(category), &category);
#endif
    // Create the audio stream description
    AudioStreamBasicDescription *description = &(producer->description);
    description->mSampleRate = TMEDIA_PRODUCER(producer)->audio.rate;
    description->mFormatID = kAudioFormatLinearPCM;
    description->mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
    description->mChannelsPerFrame = TMEDIA_PRODUCER(producer)->audio.channels;
    description->mFramesPerPacket = 1;
    description->mBitsPerChannel = TMEDIA_PRODUCER(producer)->audio.bits_per_sample;
    description->mBytesPerPacket = description->mBitsPerChannel / 8 * description->mChannelsPerFrame;
    description->mBytesPerFrame = description->mBytesPerPacket;
    description->mReserved = 0;
    
    int packetperbuffer = 1000 / TMEDIA_PRODUCER(producer)->audio.ptime;
    producer->buffer_size = description->mSampleRate * description->mBytesPerFrame / packetperbuffer;
    
    // Create the record audio queue
    ret = AudioQueueNewInput(&(producer->description),
							 __handle_input_buffer,
							 producer,
							 NULL, 
							 kCFRunLoopCommonModes,
							 0,
							 &(producer->queue));
    
    for(i = 0; i < CoreAudioRecordBuffers; i++) {
        // Create the buffer for the queue
        ret = AudioQueueAllocateBuffer(producer->queue, producer->buffer_size, &(producer->buffers[i]));
        if (ret) {
            break;
        }
        
        // Clear the data
        memset(producer->buffers[i]->mAudioData, 0, producer->buffer_size);
        producer->buffers[i]->mAudioDataByteSize = producer->buffer_size;
        
        // Enqueue the buffer
        ret = AudioQueueEnqueueBuffer(producer->queue, producer->buffers[i], 0, NULL);
        if (ret) {
            break;
        }
    }
	
	return 0;
}

static int tdav_producer_audioqueue_start(tmedia_producer_t* self)
{
    OSStatus ret;
	tdav_producer_audioqueue_t* producer = (tdav_producer_audioqueue_t*)self;
	
	if(!producer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	if(producer->started){
		TSK_DEBUG_WARN("Producer already started");
		return 0;
	}
    
	producer->started = tsk_true;
    ret = AudioQueueStart(producer->queue, NULL);
    
	return ret;
}

static int tdav_producer_audioqueue_pause(tmedia_producer_t* self)
{
    OSStatus ret;
	tdav_producer_audioqueue_t* producer = (tdav_producer_audioqueue_t*)self;
	
	if(!producer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
    
    ret = AudioQueuePause(producer->queue);
    
	return ret;
}

static int tdav_producer_audioqueue_stop(tmedia_producer_t* self)
{
    OSStatus ret;
	tdav_producer_audioqueue_t* producer = (tdav_producer_audioqueue_t*)self;
	
	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	if(!producer->started){
		TSK_DEBUG_WARN("Producer not started");
		return 0;
	}
    
	producer->started = tsk_false;
    ret = AudioQueueStop(producer->queue, false);
    
	return ret;
}


//
//	CoreAudio producer object definition
//
/* constructor */
static tsk_object_t* tdav_producer_audioqueue_ctor(tsk_object_t * self, va_list * app)
{
	tdav_producer_audioqueue_t *producer = self;
	if(producer){
		/* init base */
		tdav_producer_audio_init(TDAV_PRODUCER_AUDIO(producer));
		/* init self */
        // TODO        
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_producer_audioqueue_dtor(tsk_object_t * self)
{ 
	tdav_producer_audioqueue_t *producer = self;
	if(producer){
		// Stop the producer if not done
		if(producer->started){
			tdav_producer_audioqueue_stop(self);
		}
		
		// Free all buffers and dispose the queue
        if (producer->queue) {
			tsk_size_t i;
			
			for(i=0; i<CoreAudioRecordBuffers; i++){
				AudioQueueFreeBuffer(producer->queue, producer->buffers[i]);
			}
            AudioQueueDispose(producer->queue, true);
        }
        
		/* deinit base */
		tdav_producer_audio_deinit(TDAV_PRODUCER_AUDIO(producer));
	}
	
	return self;
}
/* object definition */
static const tsk_object_def_t tdav_producer_audioqueue_def_s = 
{
	sizeof(tdav_producer_audioqueue_t),
	tdav_producer_audioqueue_ctor, 
	tdav_producer_audioqueue_dtor,
	tdav_producer_audio_cmp, 
};
/* plugin definition*/
static const tmedia_producer_plugin_def_t tdav_producer_audioqueue_plugin_def_s = 
{
	&tdav_producer_audioqueue_def_s,
	
	tmedia_audio,
	"Apple CoreAudio producer (AudioQueue)",
	
	tdav_producer_audioqueue_set,
	tdav_producer_audioqueue_prepare,
	tdav_producer_audioqueue_start,
	tdav_producer_audioqueue_pause,
	tdav_producer_audioqueue_stop
};
const tmedia_producer_plugin_def_t *tdav_producer_audioqueue_plugin_def_t = &tdav_producer_audioqueue_plugin_def_s;

#endif /* HAVE_COREAUDIO_AUDIO_QUEUE */
