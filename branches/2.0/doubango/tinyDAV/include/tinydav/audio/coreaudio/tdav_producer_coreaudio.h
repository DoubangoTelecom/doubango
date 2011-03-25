/**@file tdav_consumer_coreaudio.h
 * @brief Audio Producer for MacOSX and iOS platforms.
 *
 * @author Laurent Etiemble <laurent.etiemble(at)gmail.com>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 letiemble
 */
#ifndef TINYDAV_PRODUCER_COREAUDIO_H
#define TINYDAV_PRODUCER_COREAUDIO_H

#include "tinydav_config.h"

#if HAVE_COREAUDIO

#include <AudioToolbox/AudioToolbox.h>
#include "tinydav/audio/tdav_producer_audio.h"

TDAV_BEGIN_DECLS

#define CoreAudioRecordBuffers 3

typedef struct tdav_producer_coreaudio_s
{
	TDAV_DECLARE_PRODUCER_AUDIO;
	
	tsk_bool_t started;
	
    AudioStreamBasicDescription description;
    AudioQueueRef queue;
    AudioQueueBufferRef buffers[CoreAudioRecordBuffers];
    
    tsk_size_t buffer_size;
}
tdav_producer_coreaudio_t;

TINYDAV_GEXTERN const tmedia_producer_plugin_def_t *tdav_producer_coreaudio_plugin_def_t;

TDAV_END_DECLS

#endif /* HAVE_COREAUDIO */

#endif /* TINYDAV_PRODUCER_COREAUDIO_H */
