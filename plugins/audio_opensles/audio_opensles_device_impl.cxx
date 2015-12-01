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
#include "audio_opensles_device_impl.h"

#include "audio_opensles_producer.h"
#include "audio_opensles_consumer.h"

SLAudioDeviceCallbackImpl::SLAudioDeviceCallbackImpl():
SLAudioDeviceCallback()
{
}

SLAudioDeviceCallbackImpl::~SLAudioDeviceCallbackImpl()
{
}

int32_t SLAudioDeviceCallbackImpl::RecordedDataIsAvailable(const void* audioSamples,
                                        const uint32_t nSamples,
                                        const uint8_t nBytesPerSample,
                                        const uint8_t nChannels,
                                        const uint32_t samplesPerSec)
{
	if(!m_pProducer){
		AUDIO_OPENSLES_DEBUG_WARN("No wrapped producer");
		return 0;
	}
	return audio_producer_opensles_handle_data_10ms(m_pProducer, audioSamples, nSamples, nBytesPerSample, samplesPerSec, nChannels);
}


int32_t SLAudioDeviceCallbackImpl::NeedMorePlayData(const uint32_t nSamples,
                                 const uint8_t nBytesPerSample,
                                 const uint8_t nChannels,
                                 const uint32_t samplesPerSec,
                                 void* audioSamples,
                                 uint32_t& nSamplesOut)
{
	if(!m_pConsumer){
		AUDIO_OPENSLES_DEBUG_WARN("No wrapped consumer");
		return 0;
	}
	return audio_consumer_opensles_get_data_10ms(m_pConsumer, audioSamples, nSamples, nBytesPerSample, nChannels, samplesPerSec, nSamplesOut);
}