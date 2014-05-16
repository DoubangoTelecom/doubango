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

#ifndef TDAV_APPLE_H
#define TDAV_APPLE_H

#if TDAV_UNDER_APPLE

#include <AudioToolbox/AudioToolbox.h>

#include "tsk_debug.h"

#if TARGET_OS_IPHONE
// deprecated in iOS7+
static void _AudioSessionInterruptionListener(void * inClientData, UInt32 inInterruptionState)
{
    switch(inInterruptionState) {
        case kAudioSessionBeginInterruption:
            {
                TSK_DEBUG_INFO("_AudioSessionInterruptionListener:kAudioSessionBeginInterruption");
                break;
            }
        case kAudioSessionEndInterruption:
            {
                TSK_DEBUG_INFO("_AudioSessionInterruptionListener:kAudioSessionEndInterruption");
                break;
            }
        default:
            {
                TSK_DEBUG_INFO("_AudioSessionInterruptionListener:%u", (unsigned int)inInterruptionState);
                break;
            }
    }
}
#endif

static int tdav_apple_init()
{
	// initialize audio session
#if TARGET_OS_IPHONE
	OSStatus status;
	status = AudioSessionInitialize(NULL, NULL, _AudioSessionInterruptionListener, NULL);
	if(status){
		TSK_DEBUG_ERROR("AudioSessionInitialize() failed with status code=%d", (int32_t)status);
		return -1;
	}
	
	// enable record/playback
	UInt32 audioCategory = kAudioSessionCategory_PlayAndRecord;
	status = AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(audioCategory), &audioCategory);
	if(status){
		TSK_DEBUG_ERROR("AudioSessionSetProperty(kAudioSessionProperty_AudioCategory) failed with status code=%d", (int32_t)status);
		return -2;
	}
	
	// allow mixing
	UInt32 allowMixing = true;
	status = AudioSessionSetProperty(kAudioSessionProperty_OverrideCategoryMixWithOthers, sizeof(allowMixing), &allowMixing);
	if(status){
		TSK_DEBUG_ERROR("AudioSessionSetProperty(kAudioSessionProperty_OverrideCategoryMixWithOthers) failed with status code=%d", (int32_t)status);
		return -3;
	}
	
	// enable audio session
	status = AudioSessionSetActive(true);
	if(status){
		TSK_DEBUG_ERROR("AudioSessionSetActive(true) failed with status code=%d", (int32_t)status);
		return -4;
	}
#endif
	return 0;
}

static int tdav_apple_deinit()
{
	// maybe other code use the session
	// OSStatus status = AudioSessionSetActive(false);
	return 0;
}

#endif /* TDAV_UNDER_APPLE */

#endif /* TDAV_APPLE_H */