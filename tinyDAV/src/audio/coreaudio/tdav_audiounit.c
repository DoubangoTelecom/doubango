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
#include "tinydav/audio/coreaudio/tdav_audiounit.h"

#if HAVE_COREAUDIO_AUDIO_UNIT

#include "tinydav/tdav_apple.h"

#include "tsk_string.h"
#include "tsk_list.h"
#include "tsk_safeobj.h"
#include "tsk_debug.h"

#if TARGET_OS_IPHONE
static UInt32 kOne = 1;
static UInt32 kZero = 0;
#endif /* TARGET_OS_IPHONE */

#if TARGET_OS_IPHONE
#if TARGET_IPHONE_SIMULATOR // VoiceProcessingIO will give unexpected result on the simulator when using iOS 5
#define kDoubangoAudioUnitSubType	kAudioUnitSubType_RemoteIO
#else // Echo cancellation, AGC, ...
#define kDoubangoAudioUnitSubType	kAudioUnitSubType_VoiceProcessingIO
#endif
#elif TARGET_OS_MAC
#define kDoubangoAudioUnitSubType	kAudioUnitSubType_HALOutput
#else
#error "Unknown target"
#endif

#undef kInputBus
#define kInputBus 1
#undef kOutputBus
#define kOutputBus 0

typedef struct tdav_audiounit_instance_s {
    TSK_DECLARE_OBJECT;
    uint64_t session_id;
    uint32_t frame_duration;
    AudioComponentInstance audioUnit;
    struct {
        unsigned consumer:1;
        unsigned producer:1;
    } ready;
    unsigned started:1;
    unsigned interrupted:1;

    TSK_DECLARE_SAFEOBJ;

}
tdav_audiounit_instance_t;
TINYDAV_GEXTERN const tsk_object_def_t *tdav_audiounit_instance_def_t;
typedef tsk_list_t tdav_audiounit_instances_L_t;


static AudioComponent __audioSystem = tsk_null;
static tdav_audiounit_instances_L_t* __audioUnitInstances = tsk_null;

static int _tdav_audiounit_handle_signal_xxx_ready(tdav_audiounit_handle_t* self, tsk_bool_t consumer)
{
    tdav_audiounit_instance_t* inst = (tdav_audiounit_instance_t*)self;
    if(!inst || !inst->audioUnit) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(inst);

    if (consumer) {
        inst->ready.consumer = tsk_true;
    }
    else {
        inst->ready.producer = tsk_true;
    }

    OSStatus status;

    // For iOS we are using full-duplex AudioUnit and we wait for both consumer and producer to be prepared
#if TARGET_OS_IPHONE
    if(inst->ready.consumer && inst->ready.producer)
#endif
    {
        status = AudioUnitInitialize(inst->audioUnit);
        if(status != noErr) {
            TSK_DEBUG_ERROR("AudioUnitInitialize failed with status =%ld", (signed long)status);
            tsk_safeobj_unlock(inst);
            return -2;
        }
    }

    tsk_safeobj_unlock(inst);
    return 0;
}

tdav_audiounit_handle_t* tdav_audiounit_handle_create(uint64_t session_id)
{
    tdav_audiounit_instance_t* inst = tsk_null;

    // create audio unit component
    if(!__audioSystem) {
        AudioComponentDescription audioDescription;
        audioDescription.componentType = kAudioUnitType_Output;
        audioDescription.componentSubType = kDoubangoAudioUnitSubType;
        audioDescription.componentManufacturer = kAudioUnitManufacturer_Apple;
        audioDescription.componentFlags = 0;
        audioDescription.componentFlagsMask = 0;
        if((__audioSystem = AudioComponentFindNext(NULL, &audioDescription))) {
            // leave blank
        }
        else {
            TSK_DEBUG_ERROR("Failed to find new audio component");
            goto done;
        }

    }
    // create list used to hold instances
    if(!__audioUnitInstances && !(__audioUnitInstances = tsk_list_create())) {
        TSK_DEBUG_ERROR("Failed to create new list");
        goto done;
    }

    //= lock the list
    tsk_list_lock(__audioUnitInstances);

    // For iOS we are using full-duplex AudioUnit and to keep it unique for both
    // the consumer and producer we use the session id.
#if TARGET_OS_IPHONE
    // find the instance from the list
    const tsk_list_item_t* item;
    tsk_list_foreach(item,__audioUnitInstances) {
        if(((tdav_audiounit_instance_t*)item->data)->session_id == session_id) {
            inst = tsk_object_ref(item->data);
            goto done;
        }
    }
#endif

    // create instance object and put it into the list
    if((inst = tsk_object_new(tdav_audiounit_instance_def_t))) {
        OSStatus status = noErr;
        tdav_audiounit_instance_t* _inst;

        // create new instance
        if((status= AudioComponentInstanceNew(__audioSystem, &inst->audioUnit)) != noErr) {
            TSK_DEBUG_ERROR("AudioComponentInstanceNew() failed with status=%ld", (signed long)status);
            TSK_OBJECT_SAFE_FREE(inst);
            goto done;
        }
        _inst = inst, _inst->session_id = session_id;
        tsk_list_push_back_data(__audioUnitInstances, (void**)&_inst);
    }

done:
    //= unlock the list
    tsk_list_unlock(__audioUnitInstances);
    return (tdav_audiounit_handle_t*)inst;
}

AudioComponentInstance tdav_audiounit_handle_get_instance(tdav_audiounit_handle_t* self)
{
    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return tsk_null;
    }
    return ((tdav_audiounit_instance_t*)self)->audioUnit;
}

int tdav_audiounit_handle_signal_consumer_ready(tdav_audiounit_handle_t* self)
{
    return _tdav_audiounit_handle_signal_xxx_ready(self, tsk_true);
}

int tdav_audiounit_handle_signal_producer_ready(tdav_audiounit_handle_t* self)
{
    return _tdav_audiounit_handle_signal_xxx_ready(self, tsk_false);
}

int tdav_audiounit_handle_start(tdav_audiounit_handle_t* self)
{
    tdav_audiounit_instance_t* inst = (tdav_audiounit_instance_t*)self;
    OSStatus status = noErr;
    if(!inst || !inst->audioUnit) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(inst);
    if (inst->ready.consumer && inst->ready.producer) {
        status = (OSStatus)tdav_apple_enable_audio();
        if (status == noErr) {
            if ((!inst->started || inst->interrupted) && (status = AudioOutputUnitStart(inst->audioUnit))) {
                TSK_DEBUG_ERROR("AudioOutputUnitStart failed with status=%ld", (signed long)status);
            }
        }
        else {
            TSK_DEBUG_ERROR("tdav_apple_enable_audio() failed with status=%ld", (signed long)status);
        }
        inst->started = (status == noErr) ? tsk_true : tsk_false;
        if (inst->started) {
            inst->interrupted = 0;
        }
    }
    else {
        TSK_DEBUG_INFO("AudioUnit producer(%d) or consumer(%d) not ready yet ...delaying start", (int)inst->ready.producer, (int)inst->ready.consumer);
    }
    tsk_safeobj_unlock(inst);
    return status ? -2 : 0;
}

uint32_t tdav_audiounit_handle_get_frame_duration(tdav_audiounit_handle_t* self)
{
    if (self) {
        return ((tdav_audiounit_instance_t*)self)->frame_duration;
    }
    return 0;
}

int tdav_audiounit_handle_configure(tdav_audiounit_handle_t* self, tsk_bool_t consumer, uint32_t ptime, AudioStreamBasicDescription* audioFormat)
{
    OSStatus status = noErr;
    tdav_audiounit_instance_t* inst = (tdav_audiounit_instance_t*)self;

    if(!inst || !audioFormat) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

#if TARGET_OS_IPHONE
    // set preferred buffer size
    Float32 preferredBufferSize = ((Float32)ptime / 1000.f); // in seconds
    UInt32 size = sizeof(preferredBufferSize);
    status = AudioSessionSetProperty(kAudioSessionProperty_PreferredHardwareIOBufferDuration, sizeof(preferredBufferSize), &preferredBufferSize);
    if(status != noErr) {
        TSK_DEBUG_ERROR("AudioSessionSetProperty(kAudioSessionProperty_PreferredHardwareIOBufferDuration) failed with status=%d", (int)status);
        TSK_OBJECT_SAFE_FREE(inst);
        goto done;
    }
    status = AudioSessionGetProperty(kAudioSessionProperty_CurrentHardwareIOBufferDuration, &size, &preferredBufferSize);
    if(status == noErr) {
        inst->frame_duration = (preferredBufferSize * 1000);
        TSK_DEBUG_INFO("Frame duration=%d", inst->frame_duration);
    }
    else {
        TSK_DEBUG_ERROR("AudioSessionGetProperty(kAudioSessionProperty_CurrentHardwareIOBufferDuration, %f) failed", preferredBufferSize);
    }


    UInt32 audioCategory = kAudioSessionCategory_PlayAndRecord;
    status = AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(audioCategory), &audioCategory);
    if(status != noErr) {
        TSK_DEBUG_ERROR("AudioSessionSetProperty(kAudioSessionProperty_AudioCategory) failed with status code=%d", (int)status);
        goto done;
    }

#elif TARGET_OS_MAC
#if 1
    // set preferred buffer size
    UInt32 preferredBufferSize = ((ptime * audioFormat->mSampleRate)/1000); // in bytes
    UInt32 size = sizeof(preferredBufferSize);
    status = AudioUnitSetProperty(inst->audioUnit, kAudioDevicePropertyBufferFrameSize, kAudioUnitScope_Global, 0, &preferredBufferSize, size);
    if(status != noErr) {
        TSK_DEBUG_ERROR("AudioUnitSetProperty(kAudioOutputUnitProperty_SetInputCallback) failed with status=%ld", (signed long)status);
    }
    status = AudioUnitGetProperty(inst->audioUnit, kAudioDevicePropertyBufferFrameSize, kAudioUnitScope_Global, 0, &preferredBufferSize, &size);
    if(status == noErr) {
        inst->frame_duration = ((preferredBufferSize * 1000)/audioFormat->mSampleRate);
        TSK_DEBUG_INFO("Frame duration=%d", inst->frame_duration);
    }
    else {
        TSK_DEBUG_ERROR("AudioUnitGetProperty(kAudioDevicePropertyBufferFrameSize, %lu) failed", (unsigned long)preferredBufferSize);
    }
#endif

#endif

done:
    return (status == noErr) ? 0 : -2;
}

int tdav_audiounit_handle_mute(tdav_audiounit_handle_t* self, tsk_bool_t mute)
{
    tdav_audiounit_instance_t* inst = (tdav_audiounit_instance_t*)self;
    if(!inst || !inst->audioUnit) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
#if TARGET_OS_IPHONE
    OSStatus status = noErr;
    status = AudioUnitSetProperty(inst->audioUnit, kAUVoiceIOProperty_MuteOutput,
                                  kAudioUnitScope_Output, kOutputBus, mute ? &kOne : &kZero, mute ? sizeof(kOne) : sizeof(kZero));

    return (status == noErr) ? 0 : -2;
#else
    return 0;
#endif
}

int tdav_audiounit_handle_interrupt(tdav_audiounit_handle_t* self, tsk_bool_t interrupt)
{
    tdav_audiounit_instance_t* inst = (tdav_audiounit_instance_t*)self;
    if (!inst) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    OSStatus status = noErr;
    if (inst->interrupted != interrupt && inst->started) {
        if (interrupt) {
            status = AudioOutputUnitStop(inst->audioUnit);
            if (status != noErr) {
                TSK_DEBUG_ERROR("AudioOutputUnitStop failed with status=%ld", (signed long)status);
                goto bail;
            }
        }
        else {
#if TARGET_OS_IPHONE
            status = (OSStatus)tdav_apple_enable_audio();
            if (status != noErr) {
                TSK_DEBUG_ERROR("AudioSessionSetActive failed with status=%ld", (signed long)status);
                goto bail;
            }
#endif
            status = AudioOutputUnitStart(inst->audioUnit);
            if (status != noErr) {
                TSK_DEBUG_ERROR("AudioOutputUnitStart failed with status=%ld", (signed long)status);
                goto bail;
            }
        }
    }
    inst->interrupted = interrupt ? 1: 0;
bail:
    return (status != noErr) ? -2 : 0;
}

int tdav_audiounit_handle_stop(tdav_audiounit_handle_t* self)
{
    tdav_audiounit_instance_t* inst = (tdav_audiounit_instance_t*)self;
    OSStatus status = noErr;
    if (!inst || (inst->started && !inst->audioUnit)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    tsk_safeobj_lock(inst);
    if (inst->started && (status = AudioOutputUnitStop(inst->audioUnit))) {
        TSK_DEBUG_ERROR("AudioOutputUnitStop failed with status=%ld", (signed long)status);
    }
    inst->started = (status == noErr ? tsk_false : tsk_true);
    inst->ready.consumer = tsk_false;
    inst->ready.producer = tsk_false;
    tsk_safeobj_unlock(inst);
    return (status != noErr) ? -2 : 0;
}

int tdav_audiounit_handle_destroy(tdav_audiounit_handle_t** self)
{
    if (!self || !*self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    tsk_list_lock(__audioUnitInstances);
    if (tsk_object_get_refcount(*self)==1) {
        tsk_list_remove_item_by_data(__audioUnitInstances, *self);
    }
    else {
        tsk_object_unref(*self);
    }
    tsk_list_unlock(__audioUnitInstances);
    *self = tsk_null;
    return 0;
}

//
//	Object definition for and AudioUnit instance
//
static tsk_object_t* tdav_audiounit_instance_ctor(tsk_object_t * self, va_list * app)
{
    tdav_audiounit_instance_t* inst = self;
    if(inst) {
        tsk_safeobj_init(inst);
    }
    return self;
}
static tsk_object_t* tdav_audiounit_instance_dtor(tsk_object_t * self)
{
    tdav_audiounit_instance_t* inst = self;
    if(inst) {
        tsk_safeobj_lock(inst);
        if(inst->audioUnit) {
            AudioUnitUninitialize(inst->audioUnit);
            AudioComponentInstanceDispose(inst->audioUnit);
            inst->audioUnit = tsk_null;
        }
        tsk_safeobj_unlock(inst);

        tsk_safeobj_deinit(inst);
        TSK_DEBUG_INFO("*** AudioUnit Instance destroyed ***");
    }
    return self;
}
static int tdav_audiounit_instance_cmp(const tsk_object_t *_ai1, const tsk_object_t *_ai2)
{
    return (int)(_ai1 - _ai2);
}
static const tsk_object_def_t tdav_audiounit_instance_def_s = {
    sizeof(tdav_audiounit_instance_t),
    tdav_audiounit_instance_ctor,
    tdav_audiounit_instance_dtor,
    tdav_audiounit_instance_cmp,
};
const tsk_object_def_t *tdav_audiounit_instance_def_t = &tdav_audiounit_instance_def_s;



#endif /* HAVE_COREAUDIO_AUDIO_UNIT */
