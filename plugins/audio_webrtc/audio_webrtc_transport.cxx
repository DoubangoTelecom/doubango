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
#include "audio_webrtc_transport.h"
#include "audio_webrtc_producer.h"
#include "audio_webrtc_consumer.h"

#include "tsk_debug.h"

using namespace webrtc;

AudioTransportImpl::AudioTransportImpl(AudioDeviceModule* audioDevice) :
    _audioDevice(audioDevice),
    _fullDuplex(false),
    _speakerVolume(false),
    _speakerMute(false),
    _microphoneVolume(false),
    _microphoneMute(false),
    _microphoneBoost(false),
    _microphoneAGC(false),
    _loopBackMeasurements(false),
	_consumer(tsk_null),
	_producer(tsk_null)
{
    
}

AudioTransportImpl::~AudioTransportImpl()
{
    
}

void AudioTransportImpl::SetFullDuplex(bool enable)
{
    _fullDuplex = enable;
}

WebRtc_Word32 AudioTransportImpl::RecordedDataIsAvailable(
    const void* audioSamples,
    const WebRtc_UWord32 nSamples,
    const WebRtc_UWord8 nBytesPerSample,
    const WebRtc_UWord8 nChannels,
    const WebRtc_UWord32 samplesPerSec,
    const WebRtc_UWord32 totalDelayMS,
    const WebRtc_Word32 clockDrift,
    const WebRtc_UWord32 currentMicLevel,
    WebRtc_UWord32& newMicLevel)
{
	if(!_producer){
		DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR("No wrapped producer");
		return 0;
	}
	return audio_producer_webrtc_handle_data_10ms(_producer, audioSamples, nSamples, nBytesPerSample, samplesPerSec, nChannels);
}


WebRtc_Word32 AudioTransportImpl::NeedMorePlayData(
    const WebRtc_UWord32 nSamples,
    const WebRtc_UWord8 nBytesPerSample,
    const WebRtc_UWord8 nChannels,
    const WebRtc_UWord32 samplesPerSec,
    void* audioSamples,
    WebRtc_UWord32& nSamplesOut)
{
	if(!_consumer){
		DOUBANGO_AUDIO_WEBRTC_DEBUG_ERROR("No wrapped consumer");
		return 0;
	}
	return audio_consumer_webrtc_get_data_10ms(_consumer, audioSamples, nSamples, nBytesPerSample, nChannels, samplesPerSec, nSamplesOut);
}