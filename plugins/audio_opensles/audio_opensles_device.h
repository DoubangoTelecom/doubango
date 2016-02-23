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
#include "audio_opensles_config.h"

#ifndef _DOUBANGO_AUDIO_OPENSLES_SLDEVICE_H
#define _DOUBANGO_AUDIO_OPENSLES_SLDEVICE_H

#if AUDIO_OPENSLES_UNDER_ANDROID
#	include <SLES/OpenSLES.h>
#	include <SLES/OpenSLES_Android.h>
#	include <SLES/OpenSLES_AndroidConfiguration.h>
#endif

#define BYTES_PER_SAMPLE 2
#define BYTES_PER_SAMPLE_LOG2 1

#define PLAYOUT_BUFFER_SIZE	10 // millis
#define PLAYOUT_SAMPLE_RATE	16000
#define RECORDING_BUFFER_SIZE	10
#define RECORDING_SAMPLE_RATE	16000

// max buffer size = 10 ms @ 48 kHz
#define REC_BUF_SIZE_IN_SAMPLES 480
#define PLAY_BUF_SIZE_IN_SAMPLES 480

#define N_MAX_INTERFACES 4
#define N_PLAY_QUEUE_BUFFERS 5
#define N_REC_QUEUE_BUFFERS 5
#define N_REC_BUFFERS 20

class SLAudioDeviceCallback
{
public:
    virtual int32_t RecordedDataIsAvailable(const void* audioSamples,
                                            const uint32_t nSamples,
                                            const uint8_t nBytesPerSample,
                                            const uint8_t nChannels,
                                            const uint32_t samplesPerSec) = 0;

    virtual int32_t NeedMorePlayData(const uint32_t nSamples,
                                     const uint8_t nBytesPerSample,
                                     const uint8_t nChannels,
                                     const uint32_t samplesPerSec,
                                     void* audioSamples,
                                     uint32_t& nSamplesOut) = 0;

protected:
    virtual ~SLAudioDeviceCallback() {}
};

class SLAudioDevice
{
public:
    SLAudioDevice(const SLAudioDeviceCallback* pCallback = NULL);
    virtual ~SLAudioDevice();

public:
    int SetCallback(const SLAudioDeviceCallback* pCallback);
    int Init();
    bool Initialized();
    int SpeakerIsAvailable(bool *pAvailable);
    int InitSpeaker();
    int SetMaxSpeakerVolume(int nMaxSpeakerVolume);
    int SetMinSpeakerVolume(int nMinSpeakerVolume);
    int SetSpeakerVolume(int nSpeakerVolume);
    int SetSpeakerOn(bool bSpeakerOn);
    int PlayoutIsAvailable(bool *pAvailable);
    int SetStereoPlayout(bool bEnabled);
    int SetPlayoutBuffer(int nPlayoutBufferSize);
    int SetPlayoutSampleRate(int nPlayoutSampleRate);
    int InitPlayout();
    int StereoPlayout(bool *pEnabled);
    int PlayoutSampleRate(int *pPlayoutSampleRate);
    int StartPlayout();
    bool Playing();
    int StopPlayout();
    int RecordingIsAvailable(bool *pAvailable);
    int MicrophoneIsAvailable(bool *pAvailable);
    int InitMicrophone();
    int SetMicrophoneVolume(int nMicrophoneVolume);
    int SetStereoRecording(bool bEnabled);
    int SetRecordingSampleRate(int nRecordingSampleRate);
    int InitRecording();
    int StereoRecording(bool *pEnabled);
    int RecordingSampleRate(int *pRecordingSampleRate);
    int StartRecording();
    bool Recording();
    int StopRecording();
    int Terminate();

private:
    int PlayoutApplyNewConfig();
    uint32_t PullPlayoutData(void* pAudioSamples, const uint32_t nSamples);
    void PushRecordingData(void* pAudioSamples, const uint32_t nSamples);
#if AUDIO_OPENSLES_UNDER_ANDROID
    static void PlayerSimpleBufferQueueCallback(SLAndroidSimpleBufferQueueItf queueItf, void *pContext);
    static void RecorderSimpleBufferQueueCallback(SLAndroidSimpleBufferQueueItf queueItf, void *pContext);
#endif

private:
    const SLAudioDeviceCallback* m_pCallback;
    int m_nMicDeviceId;
    bool m_bInitialized;
    bool m_bSpeakerInitialized;
    bool m_bPlayoutInitialized;
    bool m_bRecordingInitialized;
    bool m_bMicrophoneInitialized;
    bool m_bSpeakerOn;
    bool m_bStereoPlayout;
    bool m_bStereoRecording;
    int m_nPlayoutBufferSize;
    int m_nRecordingBufferSize;
    int m_nPlayoutSampleRate;
    int m_nRecordingSampleRate;
    bool m_bPlaying;
    bool m_bRecording;
    int m_nSpeakerVolume;
    int m_nMaxSpeakerVolume;
    int m_nMinSpeakerVolume;

#if AUDIO_OPENSLES_UNDER_ANDROID
    // audio unit
    SLObjectItf m_slEngineObject;

    // playout device
    SLObjectItf m_slPlayer;
    SLEngineItf m_slEngine;
    SLPlayItf m_slPlayerPlay;
    SLAndroidSimpleBufferQueueItf m_slPlayerSimpleBufferQueue;
    SLObjectItf m_slOutputMixObject;
    SLVolumeItf m_slSpeakerVolume;

    // recording device
    SLObjectItf m_slRecorder;
    SLRecordItf m_slRecorderRecord;
    SLAudioIODeviceCapabilitiesItf m_slAudioIODeviceCapabilities;
    SLAndroidSimpleBufferQueueItf m_slRecorderSimpleBufferQueue;
    SLDeviceVolumeItf m_slMicVolume;

    int _recQueueSeq;

    // Playout buffer
    uint8_t _playQueueBuffer[N_PLAY_QUEUE_BUFFERS][PLAY_BUF_SIZE_IN_SAMPLES << BYTES_PER_SAMPLE_LOG2];
    int _playQueueSeq;
    // Recording buffer
    uint8_t _recQueueBuffer[N_REC_QUEUE_BUFFERS][REC_BUF_SIZE_IN_SAMPLES << BYTES_PER_SAMPLE_LOG2];
    uint8_t _recBuffer[N_REC_BUFFERS][REC_BUF_SIZE_IN_SAMPLES << BYTES_PER_SAMPLE_LOG2];
    int _recLength[N_REC_BUFFERS];
    int _recSeqNumber[N_REC_BUFFERS];
    int _recCurrentSeq;
    // Current total size all data in buffers, used for delay estimate
    int _recBufferTotalSize;
#endif
};

#endif /* _DOUBANGO_AUDIO_OPENSLES_SLDEVICE_H */
