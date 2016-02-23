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
#ifndef DOUBANGO_AUDIO_WEBRTC_TRANSPORT_H
#define DOUBANGO_AUDIO_WEBRTC_TRANSPORT_H

#include "audio_webrtc_config.h"

#include <webrtc/audio_device.h>

class AudioTransportImpl: public webrtc::AudioTransport
{
public:
    virtual WebRtc_Word32
    RecordedDataIsAvailable(const void* audioSamples,
                            const WebRtc_UWord32 nSamples,
                            const WebRtc_UWord8 nBytesPerSample,
                            const WebRtc_UWord8 nChannels,
                            const WebRtc_UWord32 samplesPerSec,
                            const WebRtc_UWord32 totalDelayMS,
                            const WebRtc_Word32 clockDrift,
                            const WebRtc_UWord32 currentMicLevel,
                            WebRtc_UWord32& newMicLevel);

    virtual WebRtc_Word32 NeedMorePlayData(const WebRtc_UWord32 nSamples,
                                           const WebRtc_UWord8 nBytesPerSample,
                                           const WebRtc_UWord8 nChannels,
                                           const WebRtc_UWord32 samplesPerSec,
                                           void* audioSamples,
                                           WebRtc_UWord32& nSamplesOut);

    AudioTransportImpl(webrtc::AudioDeviceModule* audioDevice);
    ~AudioTransportImpl();

public:
    void SetFullDuplex(bool enable);
    void SetSpeakerVolume(bool enable) {
        _speakerVolume = enable;
    }
    ;
    void SetSpeakerMute(bool enable) {
        _speakerMute = enable;
    }
    ;
    void SetMicrophoneMute(bool enable) {
        _microphoneMute = enable;
    }
    ;
    void SetMicrophoneVolume(bool enable) {
        _microphoneVolume = enable;
    }
    ;
    void SetMicrophoneBoost(bool enable) {
        _microphoneBoost = enable;
    }
    ;
    void SetLoopbackMeasurements(bool enable) {
        _loopBackMeasurements = enable;
    }
    ;
    void SetMicrophoneAGC(bool enable) {
        _microphoneAGC = enable;
    }
    ;

    void SetConsumer(const struct audio_consumer_webrtc_s* consumer) {
        _consumer = consumer;
    }
    ;

    void SetProducer(const struct audio_producer_webrtc_s* producer) {
        _producer = producer;
    }
    ;

private:
    webrtc::AudioDeviceModule* _audioDevice;
    const struct audio_consumer_webrtc_s* _consumer; // mut be const and must not take reference
    const struct audio_producer_webrtc_s* _producer; // mut be const and must not take reference

    bool _fullDuplex;
    bool _speakerVolume;
    bool _speakerMute;
    bool _microphoneVolume;
    bool _microphoneMute;
    bool _microphoneBoost;
    bool _microphoneAGC;
    bool _loopBackMeasurements;
};


#endif /* DOUBANGO_AUDIO_WEBRTC_TRANSPORT_H */
