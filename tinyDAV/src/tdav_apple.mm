/*
 * Copyright (C) 2014-2015 Mamadou DIOP.
 *
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
#include "tinydav/tdav_apple.h"

#if TDAV_UNDER_APPLE

#if TDAV_UNDER_IPHONE || TDAV_UNDER_IPHONE_SIMULATOR
#import <UIKit/UIKit.h>
#import <AudioToolbox/AudioToolbox.h>
#import <AVFoundation/AVFoundation.h>
#endif

#include "tsk_debug.h"

static tsk_bool_t g_apple_initialized = tsk_false;
static tsk_bool_t g_apple_audio_enabled = tsk_false;

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

int tdav_apple_init()
{
    if (g_apple_initialized) {
        return 0;
    }
    // initialize audio session
#if TARGET_OS_IPHONE
    if ([[[UIDevice currentDevice] systemVersion] doubleValue] >= 7.0) {
        // Listening to interruption must be done in your AppDelegate:
        // https://code.google.com/p/idoubs/source/browse/branches/2.0/ios-idoubs/Classes/idoubs2AppDelegate.mm?r=264#433
        
        /* Set the audio session category to allow for playback/recording and mixing */
        NSError *setCategoryError = nil;
        BOOL setCategorySuccess = [[AVAudioSession sharedInstance]
                                   setCategory:AVAudioSessionCategoryPlayAndRecord
                                   withOptions: AVAudioSessionCategoryOptionMixWithOthers
                                   error:&setCategoryError];
        
        if (setCategorySuccess == NO) {
            TSK_DEBUG_ERROR("Failed to set audio catrgory. Error code=%ld", (long)[setCategoryError code]);
            return -1;
        }
    }
    else {
        OSStatus status;
        status = AudioSessionInitialize(NULL, NULL, _AudioSessionInterruptionListener, NULL);
        if (status) {
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
    }
#endif
    g_apple_initialized = tsk_true;
    return 0;
}

int tdav_apple_enable_audio()
{
    if (g_apple_audio_enabled) {
        return 0;
    }
    int ret = 0;
    if (!g_apple_initialized) {
        if ((ret = tdav_apple_init())) {
            return ret;
        }
    }
#if TARGET_OS_IPHONE
    if ([[[UIDevice currentDevice] systemVersion] doubleValue] >= 7.0) {
        NSError *activationError = nil;
        BOOL activationResult = [[AVAudioSession sharedInstance] setActive: YES error: &activationError];
        
        if (activationResult == NO) {
            NSInteger code = [activationError code];
            if (code == AVAudioSessionErrorInsufficientPriority || [[AVAudioSession sharedInstance] isOtherAudioPlaying]) {
                TSK_DEBUG_WARN("Delaying audio initialization because another app is using it");
                return 0; // application is interrupted -> wait for notification -> not error
            }
            else {
                TSK_DEBUG_ERROR("AVAudioSession.setActive(YES) faile with error code: %ld", (long)code);
                return -1;
            }
        }
    }
    else {
        // enable audio session
        OSStatus status = AudioSessionSetActive(true);
        if (status) {
            TSK_DEBUG_ERROR("AudioSessionSetActive(true) failed with status code=%d", (int32_t)status);
            ret = -1;
        }
    }
#endif /* TARGET_OS_IPHONE */
    g_apple_audio_enabled = (ret == 0) ? tsk_true : tsk_false;
    return ret;
}

int tdav_apple_deinit()
{
    // maybe other code use the session
    // OSStatus status = AudioSessionSetActive(false);
    return 0;
}

#endif /* TDAV_UNDER_APPLE */

