/**@file tdav_producer_coreaudio.c
 * @brief Audio Producer for MacOSX and iOS platforms.
 *
 * @author Laurent Etiemble <laurent.etiemble(at)gmail.com>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 letiemble
 */
#include "tinydav/audio/coreaudio/tdav_producer_coreaudio.h"

#if HAVE_COREAUDIO

#include "tsk_thread.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

void __handle_input_buffer (void *userdata, AudioQueueRef queue, AudioQueueBufferRef buffer, const AudioTimeStamp *start_time, UInt32 number_packet_descriptions, const AudioStreamPacketDescription *packet_descriptions ) {
    OSStatus ret;
	tdav_producer_coreaudio_t* producer = (tdav_producer_coreaudio_t*)userdata;
    
    if (!producer->started) {
        return;
    }
                           
	// Alert the session that there is new data to send
	if(TMEDIA_PRODUCER(producer)->callback) {
		TMEDIA_PRODUCER(producer)->callback(TMEDIA_PRODUCER(producer)->callback_data, buffer->mAudioData, buffer->mAudioDataByteSize);
	}
    
    // Re-enqueue the buffer
    ret = AudioQueueEnqueueBuffer(producer->queue, buffer, 0, NULL);
}

/* ============ Media Producer Interface ================= */
int tdav_producer_coreaudio_prepare(tmedia_producer_t* self, const tmedia_codec_t* codec)
{
    OSStatus ret;
	tsk_size_t i;
	tdav_producer_coreaudio_t* producer = (tdav_producer_coreaudio_t*)self;

	if(!producer || !codec && codec->plugin){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	TDAV_PRODUCER_AUDIO(producer)->channels = codec->plugin->audio.channels;
	TDAV_PRODUCER_AUDIO(producer)->rate = codec->plugin->rate;
	/* codec should have ptime */
	
    // Create the audio stream description
    AudioStreamBasicDescription *description = &(producer->description);
    description->mSampleRate = TDAV_PRODUCER_AUDIO(producer)->rate;
    description->mFormatID = kAudioFormatLinearPCM;
    description->mFormatFlags = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
    description->mChannelsPerFrame = TDAV_PRODUCER_AUDIO(producer)->channels;
    description->mFramesPerPacket = 1;
    description->mBitsPerChannel = TDAV_PRODUCER_AUDIO(producer)->bits_per_sample;
    description->mBytesPerPacket = description->mBitsPerChannel / 8 * description->mChannelsPerFrame;
    description->mBytesPerFrame = description->mBytesPerPacket;
    description->mReserved = 0;
    
    int packetperbuffer = 1000 / TDAV_PRODUCER_AUDIO(producer)->ptime;
    producer->buffer_size = description->mSampleRate * description->mBytesPerFrame / packetperbuffer;
    
    // Create the record audio queue
    ret = AudioQueueNewInput(&(producer->description),
                              __handle_input_buffer,
                              producer,
                              NULL, 
                              NULL,
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

int tdav_producer_coreaudio_start(tmedia_producer_t* self)
{
    OSStatus ret;
	tdav_producer_coreaudio_t* producer = (tdav_producer_coreaudio_t*)self;

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

int tdav_producer_coreaudio_pause(tmedia_producer_t* self)
{
    OSStatus ret;
	tdav_producer_coreaudio_t* producer = (tdav_producer_coreaudio_t*)self;

	if(!producer){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
    
    ret = AudioQueuePause(producer->queue);
    
	return ret;
}

int tdav_producer_coreaudio_stop(tmedia_producer_t* self)
{
    OSStatus ret;
	tdav_producer_coreaudio_t* producer = (tdav_producer_coreaudio_t*)self;

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
static tsk_object_t* tdav_producer_coreaudio_ctor(tsk_object_t * self, va_list * app)
{
	tdav_producer_coreaudio_t *producer = self;
	if(producer){
		/* init base */
		tdav_producer_audio_init(TDAV_PRODUCER_AUDIO(producer));
		/* init self */
        // TODO        
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_producer_coreaudio_dtor(tsk_object_t * self)
{ 
	tdav_producer_coreaudio_t *producer = self;
	if(producer){
        if (producer->queue) {
            AudioQueueDispose(producer->queue, true);
        }
        
		/* deinit base */
		tdav_producer_audio_deinit(TDAV_PRODUCER_AUDIO(producer));
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_producer_coreaudio_def_s = 
{
	sizeof(tdav_producer_coreaudio_t),
	tdav_producer_coreaudio_ctor, 
	tdav_producer_coreaudio_dtor,
	tdav_producer_audio_cmp, 
};
/* plugin definition*/
static const tmedia_producer_plugin_def_t tdav_producer_coreaudio_plugin_def_s = 
{
	&tdav_producer_coreaudio_def_s,
	
	tmedia_audio,
	"Apple CoreAudio producer",
	
	tdav_producer_coreaudio_prepare,
	tdav_producer_coreaudio_start,
	tdav_producer_coreaudio_pause,
	tdav_producer_coreaudio_stop
};
const tmedia_producer_plugin_def_t *tdav_producer_coreaudio_plugin_def_t = &tdav_producer_coreaudio_plugin_def_s;

#endif /* HAVE_COREAUDIO */
