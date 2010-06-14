/**@file tdav_consumer_coreaudio.h
 * @brief Audio Consumer for MacOSX and iOS platforms.
 *
 * @author Laurent Etiemble <laurent.etiemble(at)gmail.com>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 letiemble
 */
#ifndef TINYDAV_CONSUMER_COREAUDIO_H
#define TINYDAV_CONSUMER_COREAUDIO_H

#include "tinydav_config.h"

#if HAVE_COREAUDIO

#include <AudioToolbox/AudioToolbox.h>
#include "tinydav/audio/tdav_consumer_audio.h"

TDAV_BEGIN_DECLS

#define kNumberBuffers 3

typedef struct tdav_consumer_coreaudio_s
{
	TDAV_DECLARE_CONSUMER_AUDIO;
    
	tsk_bool_t started;
    
    AudioStreamBasicDescription description;
    AudioQueueRef queue;
    AudioQueueBufferRef buffers[kNumberBuffers];
    
    tsk_size_t buffer_size;
}
tdav_consumer_coreaudio_t;

TINYDAV_GEXTERN const tmedia_consumer_plugin_def_t *tmedia_consumer_coreaudio_plugin_def_t;

TDAV_END_DECLS

#endif /* HAVE_COREAUDIO */

#endif /* TINYDAV_CONSUMER_COREAUDIO_H */
