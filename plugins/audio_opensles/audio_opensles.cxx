/* Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>
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
*/
#include "audio_opensles.h"

#include "audio_opensles_consumer.h"
#include "audio_opensles_producer.h"
#include "audio_opensles_device.h"
#include "audio_opensles_device_impl.h"

#include "tinymedia/tmedia_consumer.h"
#include "tinymedia/tmedia_producer.h"

#include "tsk_list.h"
#include "tsk_safeobj.h"
#include "tsk_debug.h"

typedef enum PLUGIN_INDEX_E {
	PLUGIN_INDEX_AUDIO_CONSUMER,
	PLUGIN_INDEX_AUDIO_PRODUCER,
	PLUGIN_INDEX_COUNT
}
PLUGIN_INDEX_T;


int __plugin_get_def_count()
{
	return PLUGIN_INDEX_COUNT;
}

tsk_plugin_def_type_t __plugin_get_def_type_at(int index)
{
	switch (index) {
	case PLUGIN_INDEX_AUDIO_CONSUMER:
		return tsk_plugin_def_type_consumer;
	case PLUGIN_INDEX_AUDIO_PRODUCER:
		return tsk_plugin_def_type_producer;
	default: {
		AUDIO_OPENSLES_DEBUG_ERROR("No plugin at index %d", index);
		return tsk_plugin_def_type_none;
	}
	}
}

tsk_plugin_def_media_type_t	__plugin_get_def_media_type_at(int index)
{
	switch (index) {
	case PLUGIN_INDEX_AUDIO_CONSUMER:
	case PLUGIN_INDEX_AUDIO_PRODUCER: {
		return tsk_plugin_def_media_type_audio;
	}
	default: {
		AUDIO_OPENSLES_DEBUG_ERROR("No plugin at index %d", index);
		return tsk_plugin_def_media_type_none;
	}
	}
}

tsk_plugin_def_ptr_const_t __plugin_get_def_at(int index)
{
	switch (index) {
	case PLUGIN_INDEX_AUDIO_CONSUMER: {
		return audio_consumer_opensles_plugin_def_t;
	}
	case PLUGIN_INDEX_AUDIO_PRODUCER: {
		return audio_producer_opensles_plugin_def_t;
	}
	default: {
		AUDIO_OPENSLES_DEBUG_ERROR("No plugin at index %d", index);
		return tsk_null;
	}
	}
}

//
// SLES AudioInstance
//

typedef struct audio_opensles_instance_s {
	TSK_DECLARE_OBJECT;

	uint64_t sessionId;

	bool isStarted;

	bool isConsumerPrepared;
	bool isConsumerStarted;
	bool isProducerPrepared;
	bool isProducerStarted;

	bool isSpeakerAvailable;
	bool isPlayoutAvailable;
	bool isRecordingAvailable;

	SLAudioDevice* device;
	SLAudioDeviceCallbackImpl* callback;

	TSK_DECLARE_SAFEOBJ;
}
audio_opensles_instance_t;
typedef tsk_list_t audio_opensles_instances_L_t;

static audio_opensles_instances_L_t* __audioInstances = tsk_null;

static tsk_object_t* audio_opensles_instance_ctor(tsk_object_t * self, va_list * app)
{
	audio_opensles_instance_t* audioInstance = (audio_opensles_instance_t*)self;
	if (audioInstance) {
		tsk_safeobj_init(audioInstance);
	}
	return self;
}
static tsk_object_t* audio_opensles_instance_dtor(tsk_object_t * self)
{
	AUDIO_OPENSLES_DEBUG_INFO("Audio Instance destroyed");
	audio_opensles_instance_t* audioInstance = (audio_opensles_instance_t*)self;
	if (audioInstance) {
		tsk_safeobj_lock(audioInstance);
		if (audioInstance->device) {
			audioInstance->device->SetCallback(NULL);
			audioInstance->device->Terminate();
			delete audioInstance->device;
			audioInstance->device = tsk_null;
		}
		if (audioInstance->callback) {
			delete audioInstance->callback;
			audioInstance->callback = tsk_null;
		}
		tsk_safeobj_unlock(audioInstance);

		tsk_safeobj_deinit(audioInstance);
	}
	return self;
}
static int audio_opensles_instance_cmp(const tsk_object_t *_ai1, const tsk_object_t *_ai2)
{
	return (int)((uintptr_t)_ai1 - (uintptr_t)_ai2);
}
static const tsk_object_def_t audio_opensles_instance_def_s = {
	sizeof(audio_opensles_instance_t),
	audio_opensles_instance_ctor,
	audio_opensles_instance_dtor,
	audio_opensles_instance_cmp,
};
const tsk_object_def_t *audio_opensles_instance_def_t = &audio_opensles_instance_def_s;


audio_opensles_instance_handle_t* audio_opensles_instance_create(uint64_t sessionId)
{
	audio_opensles_instance_t* audioInstance = tsk_null;

	// create list used to hold instances
	if (!__audioInstances && !(__audioInstances = tsk_list_create())) {
		AUDIO_OPENSLES_DEBUG_ERROR("Failed to create new list");
		return tsk_null;
	}

	//= lock the list
	tsk_list_lock(__audioInstances);

	// find the instance from the list
	const tsk_list_item_t* item;
	tsk_list_foreach(item, __audioInstances) {
		if (((audio_opensles_instance_t*)item->data)->sessionId == sessionId) {
			audioInstance = (audio_opensles_instance_t*)tsk_object_ref(item->data);
			break;
		}
	}

	if (!audioInstance) {
		audio_opensles_instance_t* _audioInstance;
		if (!(_audioInstance = (audio_opensles_instance_t*)tsk_object_new(&audio_opensles_instance_def_s))) {
			AUDIO_OPENSLES_DEBUG_ERROR("Failed to create new audio instance");
			goto done;
		}

		if (!(_audioInstance->device = new SLAudioDevice())) {
			AUDIO_OPENSLES_DEBUG_ERROR("Failed to create audio device");
			TSK_OBJECT_SAFE_FREE(_audioInstance);
			goto done;
		}

		if (!(_audioInstance->callback = new SLAudioDeviceCallbackImpl())) {
			AUDIO_OPENSLES_DEBUG_ERROR("Failed to create audio transport");
			TSK_OBJECT_SAFE_FREE(_audioInstance);
			goto done;
		}
		if ((_audioInstance->device->SetCallback(_audioInstance->callback))) {
			AUDIO_OPENSLES_DEBUG_ERROR("AudioDeviceModule::RegisterAudioCallback() failed");
			TSK_OBJECT_SAFE_FREE(_audioInstance);
			goto done;
		}

		if ((_audioInstance->device->Init())) {
			AUDIO_OPENSLES_DEBUG_ERROR("AudioDeviceModule::Init() failed");
			TSK_OBJECT_SAFE_FREE(_audioInstance);
			goto done;
		}

		_audioInstance->sessionId = sessionId;
		audioInstance = _audioInstance;
		tsk_list_push_back_data(__audioInstances, (void**)&_audioInstance);
	}

done:
	//= unlock the list
	tsk_list_unlock(__audioInstances);

	return audioInstance;
}

int audio_opensles_instance_prepare_consumer(audio_opensles_instance_handle_t* _self, tmedia_consumer_t** _consumer)
{
	audio_opensles_instance_t* self = (audio_opensles_instance_t*)_self;
	const struct audio_consumer_opensles_s* consumer = (const struct audio_consumer_opensles_s*)*_consumer;
	if (!self || !self->device || !self->callback || !_consumer || !*_consumer) {
		AUDIO_OPENSLES_DEBUG_ERROR("invalid parameter");
		return -1;
	}

	if (self->isConsumerPrepared) {
		AUDIO_OPENSLES_DEBUG_WARN("Consumer already prepared");
		return 0;
	}

	int ret;
	bool _bool;

	tsk_safeobj_lock(self);

	self->callback->SetConsumer(consumer);

	if ((ret = self->device->SpeakerIsAvailable(&_bool))) {
		AUDIO_OPENSLES_DEBUG_ERROR("SpeakerIsAvailable() failed with error code=%d", ret);
	}
	else {
		if (!_bool) {
			AUDIO_OPENSLES_DEBUG_ERROR("SpeakerIsAvailable() returned false");
		}
		self->isSpeakerAvailable = _bool;
	}

	if ((ret = self->device->InitSpeaker())) {
		AUDIO_OPENSLES_DEBUG_ERROR("InitSpeaker() failed with error code=%d", ret);
	}
	else if ((ret = self->device->SetSpeakerOn(audio_consumer_opensles_is_speakerOn(consumer)))) {
		AUDIO_OPENSLES_DEBUG_ERROR("SetSpeakerOn() failed with error code=%d", ret);
	}

	if ((ret = self->device->PlayoutIsAvailable(&_bool))) {
		AUDIO_OPENSLES_DEBUG_ERROR("PlayoutIsAvailable() failed with error code =%d", ret);
	}
	else {
		if (!_bool) {
			AUDIO_OPENSLES_DEBUG_ERROR("PlayoutIsAvailable() returned false");
		}
		self->isPlayoutAvailable = _bool;
	}

	if ((ret = self->device->SetStereoPlayout(((*_consumer)->audio.in.channels == 2)))) {
		AUDIO_OPENSLES_DEBUG_ERROR("SetStereoPlayout(%d==2) failed with error code=%d", (*_consumer)->audio.in.channels, ret);
	}

	//if((ret = self->device->SetPlayoutBuffer(AudioDeviceModule::kFixedBufferSize, (*_consumer)->audio.ptime))){
	//	AUDIO_OPENSLES_DEBUG_ERROR("SetPlayoutBuffer(%d ms) failed with error code=%d", (*_consumer)->audio.ptime, ret);
	//}
	// always request 10ms buffers
	if ((ret = self->device->SetPlayoutBuffer(10))) {
		AUDIO_OPENSLES_DEBUG_ERROR("SetPlayoutBuffer(%d ms) failed with error code=%d", 10, ret);
	}

	int playoutSampleRate = (*_consumer)->audio.out.rate ? (*_consumer)->audio.out.rate : (*_consumer)->audio.in.rate;
	if ((ret = self->device->SetPlayoutSampleRate(playoutSampleRate))) {
		AUDIO_OPENSLES_DEBUG_ERROR("SetPlayoutSampleRate(%d) failed with error code=%d", playoutSampleRate, ret);
	}

	if ((ret = self->device->InitPlayout())) {
		AUDIO_OPENSLES_DEBUG_ERROR("AudioDeviceModule::InitPlayout() failed with error code = %d", ret);
		goto done;
	}

	// init output parameters
	if ((ret = self->device->StereoPlayout(&_bool))) {
		AUDIO_OPENSLES_DEBUG_ERROR("StereoPlayout() failed with error code=%d", ret);
	}
	else {
		(*_consumer)->audio.out.channels = (_bool ? 2 : 1);
	}
	if ((ret = self->device->PlayoutSampleRate(&playoutSampleRate))) {
		AUDIO_OPENSLES_DEBUG_ERROR("PlayoutSampleRate() failed with error code=%d", ret);
	}
	else {
		(*_consumer)->audio.out.rate = playoutSampleRate;
	}

done:
	tsk_safeobj_unlock(self);

	self->isConsumerPrepared = (ret == 0);

	return ret;
}

int audio_opensles_instance_prepare_producer(audio_opensles_instance_handle_t* _self, tmedia_producer_t** _producer)
{
	audio_opensles_instance_t* self = (audio_opensles_instance_t*)_self;
	if (!self || !self->device || !self->callback || !_producer || !*_producer) {
		AUDIO_OPENSLES_DEBUG_ERROR("invalid parameter");
		return -1;
	}

	if (self->isProducerPrepared) {
		AUDIO_OPENSLES_DEBUG_WARN("Producer already prepared");
		return 0;
	}

	int ret;
	bool _bool;

	tsk_safeobj_lock(self);

	self->callback->SetProducer((const struct audio_producer_opensles_s*)*_producer);

	if ((ret = self->device->RecordingIsAvailable(&_bool))) {
		AUDIO_OPENSLES_DEBUG_ERROR("RecordingIsAvailable() failed with error code =%d", ret);
	}
	else {
		if (!_bool) {
			AUDIO_OPENSLES_DEBUG_ERROR("RecordingIsAvailable() returned false");
		}
		self->isRecordingAvailable = _bool;
	}

	if ((ret = self->device->MicrophoneIsAvailable(&_bool))) {
		AUDIO_OPENSLES_DEBUG_ERROR("MicrophoneIsAvailable() failed with error code =%d", ret);
	}
	else {
		if (!_bool) {
			AUDIO_OPENSLES_DEBUG_ERROR("MicrophoneIsAvailable() returned false");
		}
		else {
			if ((ret = self->device->InitMicrophone())) {
				AUDIO_OPENSLES_DEBUG_ERROR("InitMicrophone() failed with error code =%d", ret);
			}
		}
	}

	if ((ret = self->device->SetStereoRecording(((*_producer)->audio.channels == 2)))) {
		AUDIO_OPENSLES_DEBUG_ERROR("SetStereoRecording(%d==2) failed with error code=%d", (*_producer)->audio.channels, ret);
	}

	int recordingSampleRate = (*_producer)->audio.rate;
	if ((ret = self->device->SetRecordingSampleRate(recordingSampleRate))) {
		AUDIO_OPENSLES_DEBUG_ERROR("SetRecordingSampleRate(%d) failed with error code=%d", recordingSampleRate, ret);
	}

	if ((ret = self->device->InitRecording())) {
		AUDIO_OPENSLES_DEBUG_ERROR("AudioDeviceModule::InitRecording() failed with error code = %d", ret);
		goto done;
	}

	// init output parameters
	if ((ret = self->device->StereoRecording(&_bool))) {
		AUDIO_OPENSLES_DEBUG_ERROR("StereoRecording() failed with error code=%d", ret);
	}
	else {
		(*_producer)->audio.channels = (_bool ? 2 : 1);
	}
	if ((ret = self->device->RecordingSampleRate(&recordingSampleRate))) {
		AUDIO_OPENSLES_DEBUG_ERROR("RecordingSampleRate() failed with error code=%d", ret);
	}
	else {
		(*_producer)->audio.rate = recordingSampleRate;
	}

done:
	tsk_safeobj_unlock(self);

	self->isProducerPrepared = (ret == 0);

	return ret;
}

int audio_opensles_instance_start_consumer(audio_opensles_instance_handle_t* _self)
{
	audio_opensles_instance_t* self = (audio_opensles_instance_t*)_self;
	if (!self || !self->device || !self->callback) {
		AUDIO_OPENSLES_DEBUG_ERROR("invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(self);
	if (!self->isConsumerPrepared) {
		AUDIO_OPENSLES_DEBUG_ERROR("Consumer not prepared");
		goto done;
	}

	if (self->isConsumerStarted) {
		AUDIO_OPENSLES_DEBUG_WARN("Consumer already started");
		goto done;
	}

	if (self->isPlayoutAvailable) {
		int ret;
		if ((ret = self->device->StartPlayout())) {
			AUDIO_OPENSLES_DEBUG_ERROR("StartPlayout() failed with error code = %d", ret);
		}

		self->isConsumerStarted = self->device->Playing();
		AUDIO_OPENSLES_DEBUG_INFO("isPlaying=%s", (self->isConsumerPrepared ? "true" : "false"));
	}

done:
	tsk_safeobj_unlock(self);
	return (self->isConsumerStarted ? 0 : -1);
}

int audio_opensles_instance_start_producer(audio_opensles_instance_handle_t* _self)
{
	audio_opensles_instance_t* self = (audio_opensles_instance_t*)_self;
	if (!self || !self->device || !self->callback) {
		AUDIO_OPENSLES_DEBUG_ERROR("invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(self);
	if (!self->isProducerPrepared) {
		AUDIO_OPENSLES_DEBUG_ERROR("Producer not prepared");
		goto done;
	}

	if (self->isProducerStarted) {
		AUDIO_OPENSLES_DEBUG_WARN("Consumer already started");
		goto done;
	}

	if (self->isRecordingAvailable) {
		int ret;
		if ((ret = self->device->StartRecording())) {
			AUDIO_OPENSLES_DEBUG_ERROR("StartRecording() failed with error code = %d", ret);
		}

		self->isProducerStarted = self->device->Recording();
		AUDIO_OPENSLES_DEBUG_INFO("isRecording=%s", (self->isProducerStarted ? "true" : "false"));
	}

done:
	tsk_safeobj_unlock(self);
	return (self->isProducerStarted ? 0 : -1);
}

int audio_opensles_instance_stop_consumer(audio_opensles_instance_handle_t* _self)
{
	audio_opensles_instance_t* self = (audio_opensles_instance_t*)_self;
	if (!self || !self->device || !self->callback) {
		AUDIO_OPENSLES_DEBUG_ERROR("invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(self);

	if (!self->isConsumerStarted) {
		goto done;
	}

	int ret;
	if ((ret = self->device->StopPlayout())) {
		AUDIO_OPENSLES_DEBUG_ERROR("StopPlayout() failed with error code = %d", ret);
	}
	else {
		self->isConsumerStarted = self->device->Playing();
		self->isConsumerPrepared = false;
	}

done:
	tsk_safeobj_unlock(self);
	return (self->isConsumerStarted ? -1 : 0);
}

int audio_opensles_instance_stop_producer(audio_opensles_instance_handle_t* _self)
{
	audio_opensles_instance_t* self = (audio_opensles_instance_t*)_self;
	if (!self || !self->device || !self->callback) {
		AUDIO_OPENSLES_DEBUG_ERROR("invalid parameter");
		return -1;
	}

	tsk_safeobj_lock(self);

	if (!self->isProducerStarted) {
		goto done;
	}

	int ret;
	if ((ret = self->device->StopRecording())) {
		AUDIO_OPENSLES_DEBUG_ERROR("StopRecording() failed with error code = %d", ret);
	}
	else {
		self->isProducerStarted = self->device->Recording();
		self->isProducerPrepared = false;
	}

done:
	tsk_safeobj_unlock(self);
	return (self->isProducerStarted ? -1 : 0);
}

int audio_opensles_instance_set_speakerOn(audio_opensles_instance_handle_t* _self, bool speakerOn)
{
	audio_opensles_instance_t* self = (audio_opensles_instance_t*)_self;
	if (!self || !self->device) {
		AUDIO_OPENSLES_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return self->device->SetSpeakerOn(speakerOn);
}

int audio_opensles_instance_set_microphone_volume(audio_opensles_instance_handle_t* _self, int32_t volume)
{
	audio_opensles_instance_t* self = (audio_opensles_instance_t*)_self;
	if (!self || !self->device) {
		AUDIO_OPENSLES_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	return self->device->SetMicrophoneVolume(volume);
}

int audio_opensles_instance_destroy(audio_opensles_instance_handle_t** _self)
{
	if (!_self || !*_self) {
		AUDIO_OPENSLES_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	tsk_list_lock(__audioInstances);
	if (tsk_object_get_refcount(*_self) == 1) {
		tsk_list_remove_item_by_data(__audioInstances, *_self);
	}
	else {
		tsk_object_unref(*_self);
	}
	tsk_list_unlock(__audioInstances);
	*_self = tsk_null;
	return 0;
}

