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
#ifndef _DOUBANGO_AUDIO_OPENSLES_SLDEVICE_IMPL_H
#define _DOUBANGO_AUDIO_OPENSLES_SLDEVICE_IMPL_H

#include "audio_opensles_config.h"
#include "audio_opensles_device.h"

class SLAudioDeviceCallbackImpl : public SLAudioDeviceCallback
{
public:
	SLAudioDeviceCallbackImpl();
	virtual ~SLAudioDeviceCallbackImpl();

	virtual int32_t RecordedDataIsAvailable(const void* audioSamples,
                                            const uint32_t nSamples,
                                            const uint8_t nBytesPerSample,
                                            const uint8_t nChannels,
                                            const uint32_t samplesPerSec);   

    virtual int32_t NeedMorePlayData(const uint32_t nSamples,
                                     const uint8_t nBytesPerSample,
                                     const uint8_t nChannels,
                                     const uint32_t samplesPerSec,
                                     void* audioSamples,
                                     uint32_t& nSamplesOut);

	inline void SetConsumer(const struct audio_consumer_opensles_s* pConsumer){ m_pConsumer = pConsumer; }
	inline void SetProducer(const struct audio_producer_opensles_s* pProducer){ m_pProducer = pProducer; }

private:
	const struct audio_consumer_opensles_s* m_pConsumer; // mut be const and must not take reference
	const struct audio_producer_opensles_s* m_pProducer; // mut be const and must not take reference
};

#endif /* _DOUBANGO_AUDIO_OPENSLES_SLDEVICE_IMPL_H */
