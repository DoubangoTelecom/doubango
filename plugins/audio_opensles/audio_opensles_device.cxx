/*  Copyright (C) 2012 Doubango Telecom <http://www.doubango.org>. All Rights Reserved.
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "audio_opensles_device.h"

#include <stdio.h>
#include <string.h>

#define CHECK_TRUE(_bool, _text) { if(!_bool){ AUDIO_OPENSLES_DEBUG_ERROR(_text); return -1; } }
#define CHECK_FALSE(_bool, _text) { if(_bool){ AUDIO_OPENSLES_DEBUG_ERROR(_text); return -1; } }
#define CHECK_PLAYOUT_INITIALIZED() CHECK_TRUE(m_bPlayoutInitialized, "Playout not initialized")
#define CHECK_PLAYOUT_NOT_INITIALIZED() CHECK_FALSE(m_bPlayoutInitialized, "Playout initialized")
#define CHECK_RECORDING_INITIALIZED() CHECK_TRUE(m_bRecordingInitialized, "Recording not initialized")
#define CHECK_RECORDING_NOT_INITIALIZED() CHECK_FALSE(m_bRecordingInitialized, "Recording initialized")
#define CHECK_MICROPHONE_INITIALIZED() CHECK_TRUE(m_bMicrophoneInitialized, "Microphone not initialized")
#define CHECK_MICROPHONE_NOT_INITIALIZED() CHECK_FALSE(m_bMicrophoneInitialized, "Microphone initialized")

#if AUDIO_OPENSLES_UNDER_ANDROID
static inline SLuint32 SL_SAMPLING_RATE(int RATE_INT)
{
    switch(RATE_INT) {
    case 8000:
        return SL_SAMPLINGRATE_8;
    case 11025:
        return SL_SAMPLINGRATE_11_025;
    default:
    case 16000:
        return SL_SAMPLINGRATE_16;
    case 22050:
        return SL_SAMPLINGRATE_22_05;
    case 24000:
        return SL_SAMPLINGRATE_24;
    case 32000:
        return SL_SAMPLINGRATE_32;
    case 44100:
        return SL_SAMPLINGRATE_44_1;
    case 64000:
        return SL_SAMPLINGRATE_64;
    case 88200:
        return SL_SAMPLINGRATE_88_2;
    case 96000:
        return SL_SAMPLINGRATE_96;
    case 192000:
        return SL_SAMPLINGRATE_192;
    }
}
#endif

SLAudioDevice::SLAudioDevice(const SLAudioDeviceCallback* pCallback):
#if AUDIO_OPENSLES_UNDER_ANDROID
    m_slEngineObject(NULL),
    m_slPlayer(NULL),
    m_slEngine(NULL),
    m_slPlayerPlay(NULL),
    m_slPlayerSimpleBufferQueue(NULL),
    m_slOutputMixObject(NULL),
    m_slSpeakerVolume(NULL),
    m_slRecorder(NULL),
    m_slRecorderRecord(NULL),
    m_slAudioIODeviceCapabilities(NULL),
    m_slRecorderSimpleBufferQueue(NULL),
    m_slMicVolume(NULL),
    _playQueueSeq(0),
    _recCurrentSeq(0),
    _recBufferTotalSize(0),
    _recQueueSeq(0),
#endif
    m_nMicDeviceId(0),
    m_pCallback(pCallback),
    m_bInitialized(false),
    m_bSpeakerInitialized(false),
    m_bSpeakerOn(false),
    m_bPlayoutInitialized(false),
    m_bRecordingInitialized(false),
    m_bMicrophoneInitialized(false),
    m_bStereoPlayout(false),
    m_bStereoRecording(false),
    m_nPlayoutSampleRate(PLAYOUT_SAMPLE_RATE),
    m_nRecordingSampleRate(RECORDING_SAMPLE_RATE),
    m_nRecordingBufferSize(RECORDING_BUFFER_SIZE),
    m_nPlayoutBufferSize(PLAYOUT_BUFFER_SIZE),
    m_bPlaying(false),
    m_bRecording(false),
    m_nSpeakerVolume(0),
    m_nMinSpeakerVolume(0),
    m_nMaxSpeakerVolume(0)
{
#if AUDIO_OPENSLES_UNDER_ANDROID
    memset(_playQueueBuffer, 0, sizeof(_playQueueBuffer));
    memset(_recQueueBuffer, 0, sizeof(_recQueueBuffer));
    memset(_recBuffer, 0, sizeof(_recBuffer));
    memset(_recLength, 0, sizeof(_recLength));
    memset(_recSeqNumber, 0, sizeof(_recSeqNumber));
#endif
}

SLAudioDevice::~SLAudioDevice()
{

}

int SLAudioDevice::SetCallback(const SLAudioDeviceCallback* pCallback)
{
    m_pCallback = pCallback;
    return 0;
}

int SLAudioDevice::Init()
{
    CHECK_FALSE(m_bInitialized, "Already initialized");

    AUDIO_OPENSLES_DEBUG_INFO("SLAudioDevice::Init()");

#if AUDIO_OPENSLES_UNDER_ANDROID
    SLresult slResult;

    SLEngineOption EngineOption[] = {
        { (SLuint32) SL_ENGINEOPTION_THREADSAFE, (SLuint32) SL_BOOLEAN_TRUE },
    };
    slResult = slCreateEngine(&m_slEngineObject, 1, EngineOption, 0, NULL, NULL);
    if (slResult != SL_RESULT_SUCCESS) {
        AUDIO_OPENSLES_DEBUG_ERROR("Failed to create Engine with error code = %d", slResult);
        return -1;
    }
    if ((slResult = (*m_slEngineObject)->Realize(m_slEngineObject, SL_BOOLEAN_FALSE)) != SL_RESULT_SUCCESS) {
        AUDIO_OPENSLES_DEBUG_ERROR("Failed to Realize SL Engine with erro code = %d", slResult);
        return -1;
    }
    if ((slResult = (*m_slEngineObject)->GetInterface(m_slEngineObject, SL_IID_ENGINE, (void*) &m_slEngine)) != SL_RESULT_SUCCESS) {
        AUDIO_OPENSLES_DEBUG_ERROR("Failed to get SL Engine interface with error code = %d", slResult);
        return -1;
    }
#endif /* AUDIO_OPENSLES_UNDER_ANDROID */

    m_bInitialized = true;
    AUDIO_OPENSLES_DEBUG_INFO("SL engine initialized");
    return 0;
}

bool SLAudioDevice::Initialized()
{
    return m_bInitialized;
}

int SLAudioDevice::SpeakerIsAvailable(bool *pAvailable)
{
    CHECK_TRUE(m_bInitialized, "Not initialized");

    if(!pAvailable) {
        AUDIO_OPENSLES_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    *pAvailable = true;
    return 0;
}

int SLAudioDevice::InitSpeaker()
{
    AUDIO_OPENSLES_DEBUG_INFO("SLAudioDevice::InitSpeaker()");

    CHECK_TRUE(m_bInitialized, "Not initialized");

    if(m_bSpeakerInitialized) {
        return 0;
    }

    m_bSpeakerInitialized = true;
    return 0;
}

int SLAudioDevice::SetMaxSpeakerVolume(int nMaxSpeakerVolume)
{
    CHECK_TRUE(m_bSpeakerInitialized, "Speaker not initialized");
    AUDIO_OPENSLES_DEBUG_INFO("SetMaxSpeakerVolume(%d)", nMaxSpeakerVolume);
    m_nMaxSpeakerVolume = nMaxSpeakerVolume;
    return 0;
}

int SLAudioDevice::SetMinSpeakerVolume(int nMinSpeakerVolume)
{
    CHECK_TRUE(m_bSpeakerInitialized, "Speaker not initialized");
    AUDIO_OPENSLES_DEBUG_INFO("SetMinSpeakerVolume(%d)", nMinSpeakerVolume);
    m_nMinSpeakerVolume = nMinSpeakerVolume;
    return 0;
}

int SLAudioDevice::SetSpeakerVolume(int nSpeakerVolume)
{
    CHECK_TRUE(m_bSpeakerInitialized, "Speaker not initialized");
    AUDIO_OPENSLES_DEBUG_INFO("SetSpeakerVolume(%d)", nSpeakerVolume);
    m_nSpeakerVolume = nSpeakerVolume;
    return 0;
}

int SLAudioDevice::SetSpeakerOn(bool bSpeakerOn)
{
    CHECK_TRUE(m_bSpeakerInitialized, "Speaker not initialized");

    AUDIO_OPENSLES_DEBUG_INFO("SetSpeakerOn(%s -> %s)", (m_bSpeakerOn ? "true" : "false"), (bSpeakerOn ? "true" : "false"));
    int ret = 0;
    bool oldValue = m_bSpeakerOn;
    m_bSpeakerOn = bSpeakerOn; // update value beacause use in PlayoutApplyNewConfig();
    if(m_bPlayoutInitialized && (oldValue != bSpeakerOn)) {
        ret = PlayoutApplyNewConfig();
    }

    if(ret != 0) {
        m_bSpeakerOn = oldValue;
    }

    return ret;
}

int SLAudioDevice::PlayoutIsAvailable(bool *pAvailable)
{
    CHECK_TRUE(m_bInitialized, "Not initialized");
    CHECK_PLAYOUT_NOT_INITIALIZED();

    if(!pAvailable) {
        AUDIO_OPENSLES_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    *pAvailable = true;
    return 0;
}

int SLAudioDevice::SetStereoPlayout(bool bEnabled)
{
    CHECK_TRUE(m_bInitialized, "Not initialized");
    CHECK_PLAYOUT_NOT_INITIALIZED();
    m_bStereoPlayout = bEnabled;
    return 0;
}

int SLAudioDevice::SetPlayoutBuffer(int nPlayoutBufferSize)
{
    CHECK_TRUE(m_bInitialized, "Not initialized");
    CHECK_PLAYOUT_NOT_INITIALIZED();

    if(PLAYOUT_BUFFER_SIZE != nPlayoutBufferSize) {
        AUDIO_OPENSLES_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    m_nPlayoutBufferSize = nPlayoutBufferSize;
    return 0;
}

int SLAudioDevice::SetPlayoutSampleRate(int nPlayoutSampleRate)
{
    CHECK_TRUE(m_bInitialized, "Not initialized");
    CHECK_PLAYOUT_NOT_INITIALIZED();

    AUDIO_OPENSLES_DEBUG_INFO("SetPlayoutSampleRate(%d)", nPlayoutSampleRate);

    switch(nPlayoutSampleRate) {
    case 8000:
    case 11025:
    case 16000:
    case 22050:
    case 24000:
    case 32000:
    case 44100:
    case 64000:
    case 88200:
    case 96000:
    case 192000: {
        m_nPlayoutSampleRate = nPlayoutSampleRate;
        return 0;
    }
    default: {
        AUDIO_OPENSLES_DEBUG_ERROR("%d not valid sampling rate", nPlayoutSampleRate);
        return -1;
    }
    }
}

int SLAudioDevice::InitPlayout()
{
    CHECK_TRUE(m_bInitialized, "Not initialized");

    AUDIO_OPENSLES_DEBUG_INFO("SLAudioDevice::InitPlayout()");

    if(m_bPlayoutInitialized) {
        return 0;
    }

    if (m_bPlaying) {
        AUDIO_OPENSLES_DEBUG_ERROR("Playout already started");
        return -1;
    }

    // Initialize the speaker
    if (InitSpeaker()) {
        AUDIO_OPENSLES_DEBUG_ERROR("InitSpeaker() failed");
    }

#if AUDIO_OPENSLES_UNDER_ANDROID

    if (m_slEngineObject == NULL || m_slEngine == NULL) {
        AUDIO_OPENSLES_DEBUG_ERROR("SLObject or Engiine is NULL");
        return -1;
    }

    SLresult slResult;
    SLDataFormat_PCM pcm;
    SLDataSource audioSource;
    SLDataLocator_AndroidSimpleBufferQueue simpleBufferQueue;
    SLDataSink audioSink;
    SLDataLocator_OutputMix locator_outputmix;

    // Create Output Mix object to be used by player
    SLInterfaceID ids[N_MAX_INTERFACES];
    SLboolean req[N_MAX_INTERFACES];
    for (unsigned int i = 0; i < N_MAX_INTERFACES; i++) {
        ids[i] = SL_IID_NULL;
        req[i] = SL_BOOLEAN_FALSE;
    }
    ids[0] = SL_IID_ENVIRONMENTALREVERB;

    if ((slResult = (*m_slEngine)->CreateOutputMix(m_slEngine, &m_slOutputMixObject, 1, ids, req)) != SL_RESULT_SUCCESS) {
        AUDIO_OPENSLES_DEBUG_ERROR("CreateOutputMix() for playout failed with error code = %d", slResult);
        return -1;
    }

    if ((slResult = (*m_slOutputMixObject)->Realize(m_slOutputMixObject, SL_BOOLEAN_FALSE)) != SL_RESULT_SUCCESS) {
        AUDIO_OPENSLES_DEBUG_ERROR("Failed to realize SL Output Mix object for playout with error code = %d", slResult);
        return -1;
    }

    simpleBufferQueue.locatorType = SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;
    simpleBufferQueue.numBuffers = N_PLAY_QUEUE_BUFFERS;

    pcm.formatType = SL_DATAFORMAT_PCM;
    pcm.numChannels = m_bStereoPlayout ? 2 : 1;
    pcm.samplesPerSec = SL_SAMPLING_RATE(m_nPlayoutSampleRate);
    pcm.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
    pcm.containerSize = SL_PCMSAMPLEFORMAT_FIXED_16;
    pcm.channelMask = m_bStereoRecording ? (SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT) : SL_SPEAKER_FRONT_CENTER;
    pcm.endianness = SL_BYTEORDER_LITTLEENDIAN;

    audioSource.pFormat = (void *) &pcm;
    audioSource.pLocator = (void *) &simpleBufferQueue;

    locator_outputmix.locatorType = SL_DATALOCATOR_OUTPUTMIX;
    locator_outputmix.outputMix = m_slOutputMixObject;
    audioSink.pLocator = (void *) &locator_outputmix;
    audioSink.pFormat = NULL;

    ids[0] = SL_IID_ANDROIDSIMPLEBUFFERQUEUE;
    ids[1] = SL_IID_EFFECTSEND;
    ids[2] = SL_IID_ANDROIDCONFIGURATION;
    ids[3] = SL_IID_VOLUME;
    req[0] = SL_BOOLEAN_TRUE;
    req[1] = SL_BOOLEAN_TRUE;
    req[2] = SL_BOOLEAN_TRUE;
    req[3] = SL_BOOLEAN_TRUE;

    // Create the player
    if ((slResult = (*m_slEngine)->CreateAudioPlayer(m_slEngine, &m_slPlayer, &audioSource, &audioSink, 3, ids, req)) != SL_RESULT_SUCCESS) {
        AUDIO_OPENSLES_DEBUG_ERROR("Failed to create Audio Player with error code = %d", slResult);
        return -1;
    }

    // set stream type
    if(!m_bSpeakerOn) { // only set if speaker OFF, otherwise default is ON. "SL_ANDROID_STREAM_MEDIA" doen't look to work on all devices
        static SLAndroidConfigurationItf _playerStreamConfig;
        if((slResult = (*m_slPlayer)->GetInterface(m_slPlayer, SL_IID_ANDROIDCONFIGURATION, &_playerStreamConfig)) != SL_RESULT_SUCCESS) {
            AUDIO_OPENSLES_DEBUG_ERROR("Failed to get player configuration with error code = %d", slResult);
            return -1;
        }
        else {
            static SLint32 _playerStreamType = m_bSpeakerOn ? SL_ANDROID_STREAM_MEDIA : SL_ANDROID_STREAM_VOICE;
            static SLint32 _playerStreamTypeSize = sizeof(SLint32);
            AUDIO_OPENSLES_DEBUG_INFO("_playerStreamType=%d", _playerStreamType);
            if((slResult = (*_playerStreamConfig)->SetConfiguration(_playerStreamConfig, SL_ANDROID_KEY_STREAM_TYPE, &_playerStreamType, _playerStreamTypeSize))) {
                AUDIO_OPENSLES_DEBUG_ERROR("Failed to set player stream type with error code = %d", slResult);
                return -2;
            }
        }
    }

    // Realizing the player in synchronous mode
    if ((slResult = (*m_slPlayer)->Realize(m_slPlayer, SL_BOOLEAN_FALSE)) != SL_RESULT_SUCCESS) {
        AUDIO_OPENSLES_DEBUG_ERROR("Failed to realize the player with error code = %d", slResult);
        return -1;
    }
    // Get seek and play interfaces
    if ((slResult = (*m_slPlayer)->GetInterface(m_slPlayer, SL_IID_PLAY, (void*) &m_slPlayerPlay)) != SL_RESULT_SUCCESS) {
        AUDIO_OPENSLES_DEBUG_ERROR("Failed to get Player interface with error code = %d", slResult);
        return -1;
    }
    if ((slResult = (*m_slPlayer)->GetInterface(m_slPlayer, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, (void*) &m_slPlayerSimpleBufferQueue)) != SL_RESULT_SUCCESS) {
        AUDIO_OPENSLES_DEBUG_ERROR("Failed to get Player Simple Buffer Queue interface with error code = %d", slResult);
        return -1;
    }

    // Setup to receive buffer queue event callbacks
    if ((slResult = (*m_slPlayerSimpleBufferQueue)->RegisterCallback(m_slPlayerSimpleBufferQueue, PlayerSimpleBufferQueueCallback, this)) != SL_RESULT_SUCCESS) {
        AUDIO_OPENSLES_DEBUG_ERROR("Failed to register Player Callback");
        return -1;
    }
#endif /* AUDIO_OPENSLES_UNDER_ANDROID */

    m_bPlayoutInitialized = true;

    AUDIO_OPENSLES_DEBUG_INFO("Playout initialized");

    return 0;
}

int SLAudioDevice::StereoPlayout(bool *pEnabled)
{
    CHECK_TRUE(m_bInitialized, "Not initialized");
    CHECK_PLAYOUT_INITIALIZED();

    if(!pEnabled) {
        AUDIO_OPENSLES_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    *pEnabled = m_bStereoPlayout;
    return 0;
}

int SLAudioDevice::PlayoutSampleRate(int *pPlayoutSampleRate)
{
    CHECK_TRUE(m_bInitialized, "Not initialized");
    CHECK_PLAYOUT_INITIALIZED();

    if(!pPlayoutSampleRate) {
        AUDIO_OPENSLES_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    *pPlayoutSampleRate = m_nPlayoutSampleRate;
    return 0;
}

int SLAudioDevice::StartPlayout()
{
    AUDIO_OPENSLES_DEBUG_INFO("SLAudioDevice::StartPlayout()");

    CHECK_TRUE(m_bInitialized, "Not initialized");
    CHECK_PLAYOUT_INITIALIZED();

    if (m_bPlaying) {
        return 0;
    }

#if AUDIO_OPENSLES_UNDER_ANDROID
    if (m_slPlayerPlay == NULL) {
        AUDIO_OPENSLES_DEBUG_ERROR("PlayItf is NULL");
        return -1;
    }
    if (m_slPlayerSimpleBufferQueue == NULL) {
        AUDIO_OPENSLES_DEBUG_ERROR("PlayerSimpleBufferQueue is NULL");
        return -1;
    }

    _recQueueSeq = 0;

    SLresult slResult;
    /* Enqueue a set of zero buffers to get the ball rolling */
    uint32_t nSample10ms = m_nPlayoutSampleRate / 100;
    uint8_t playBuffer[nSample10ms << BYTES_PER_SAMPLE_LOG2];
    uint32_t noSamplesOut(0);
    {
        // get data from jitter buffer
        noSamplesOut = SLAudioDevice::PullPlayoutData(playBuffer, nSample10ms);
        if(noSamplesOut != nSample10ms) {
            AUDIO_OPENSLES_DEBUG_WARN("%d not expected as samples output count value", noSamplesOut);
            noSamplesOut = nSample10ms;
            memset(_playQueueBuffer[_playQueueSeq], 0, (noSamplesOut << BYTES_PER_SAMPLE_LOG2));
        }
        else {
            memcpy(_playQueueBuffer[_playQueueSeq], playBuffer, (noSamplesOut << BYTES_PER_SAMPLE_LOG2));
        }

        // write the buffer data we into the device
        if ((slResult = (*m_slPlayerSimpleBufferQueue)->Enqueue(m_slPlayerSimpleBufferQueue, (void*) _playQueueBuffer[_playQueueSeq], (noSamplesOut << 1))) != SL_RESULT_SUCCESS) {
            AUDIO_OPENSLES_DEBUG_ERROR("Player simpler buffer queue Enqueue failed with error code = %d and noSamplesOut = %d", slResult, noSamplesOut);
        }
        _playQueueSeq = (_playQueueSeq + 1) % N_PLAY_QUEUE_BUFFERS;
    }

    // Play the PCM samples using a buffer queue
    m_bPlaying = true;
    if ((slResult = (*m_slPlayerPlay)->SetPlayState(m_slPlayerPlay, SL_PLAYSTATE_PLAYING)) != SL_RESULT_SUCCESS) {
        AUDIO_OPENSLES_DEBUG_ERROR("Failed to start playout with error code = %d", slResult);
        m_bPlaying = false;
        return -1;
    }
#else
    m_bPlaying = true;
#endif /* AUDIO_OPENSLES_UNDER_ANDROID */

    AUDIO_OPENSLES_DEBUG_INFO("Payout started - rate=%d", m_nPlayoutSampleRate);

    return 0;
}

bool SLAudioDevice::Playing()
{
    return m_bPlaying;
}

int SLAudioDevice::StopPlayout()
{
    AUDIO_OPENSLES_DEBUG_INFO("SLAudioDevice::StopPlayout()");

    if(!m_bPlaying) {
        return 0;
    }
#if AUDIO_OPENSLES_UNDER_ANDROID
    if ((m_slPlayerPlay != NULL) && (m_slOutputMixObject != NULL) && (m_slPlayer != NULL)) {
        SLresult slResult;

        if ((slResult = (*m_slPlayerPlay)->SetPlayState(m_slPlayerPlay, SL_PLAYSTATE_STOPPED)) != SL_RESULT_SUCCESS) {
            AUDIO_OPENSLES_DEBUG_ERROR("Failed to stop playout with error code = %d", slResult);
            return -1;
        }

        if ((slResult = (*m_slPlayerSimpleBufferQueue)->Clear(m_slPlayerSimpleBufferQueue)) != SL_RESULT_SUCCESS) {
            AUDIO_OPENSLES_DEBUG_ERROR("Failed to clear recorder buffer queue");
            return -1;
        }

        // Destroy the player
        (*m_slPlayer)->Destroy(m_slPlayer);
        // Destroy Output Mix object
        (*m_slOutputMixObject)->Destroy(m_slOutputMixObject);
        m_slPlayer = NULL;
        m_slPlayerPlay = NULL;
        m_slPlayerSimpleBufferQueue = NULL;
        m_slOutputMixObject = NULL;
    }
#endif

    AUDIO_OPENSLES_DEBUG_INFO("Playout stopped");
    m_bPlayoutInitialized = false;
    m_bPlaying = false;
    return 0;
}

int SLAudioDevice::RecordingIsAvailable(bool *pAvailable)
{
    CHECK_TRUE(m_bInitialized, "Device not initialized");
    CHECK_RECORDING_NOT_INITIALIZED();

    if(!pAvailable) {
        AUDIO_OPENSLES_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    *pAvailable = true;
    return 0;
}

int SLAudioDevice::MicrophoneIsAvailable(bool *pAvailable)
{
    CHECK_TRUE(m_bInitialized, "Device not initialized");
    CHECK_RECORDING_NOT_INITIALIZED();

    if(!pAvailable) {
        AUDIO_OPENSLES_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    *pAvailable = true;
    return 0;
}

int SLAudioDevice::InitMicrophone()
{
    AUDIO_OPENSLES_DEBUG_INFO("SLAudioDevice::InitMicrophone()");
    CHECK_TRUE(m_bInitialized, "Device not initialized");

    if(m_bMicrophoneInitialized) {
        return 0;
    }

    m_bMicrophoneInitialized = true;
    return 0;
}

int SLAudioDevice::SetMicrophoneVolume(int nMicrophoneVolume)
{
    CHECK_MICROPHONE_INITIALIZED();

    AUDIO_OPENSLES_DEBUG_INFO("SetMicrophoneVolume(%d)", nMicrophoneVolume);

#if AUDIO_OPENSLES_UNDER_ANDROID
    if (m_slMicVolume == NULL) {
        SLresult slResult;
        if ((slResult = (*m_slEngineObject)->GetInterface(m_slEngineObject, SL_IID_DEVICEVOLUME, (void*) &m_slMicVolume)) != SL_RESULT_SUCCESS) {
            AUDIO_OPENSLES_DEBUG_ERROR("Failed to get 'SL_IID_DEVICEVOLUME' interface with error code = %d", slResult);
            return -1;
        }
    }

    if (m_slMicVolume != NULL) {
        SLresult slResult;
        int vol(0);
        vol = ((nMicrophoneVolume * (m_nMaxSpeakerVolume - m_nMinSpeakerVolume) + (int) (255 / 2)) / (255)) + m_nMinSpeakerVolume;
        if ((slResult = (*m_slMicVolume)->SetVolume(m_slMicVolume, m_nMicDeviceId, vol)) != SL_RESULT_SUCCESS) {
            AUDIO_OPENSLES_DEBUG_ERROR("SetVolume() failed with error code = %d", slResult);
            return -1;
        }
    }
#endif /* AUDIO_OPENSLES_UNDER_ANDROID */

    return 0;
}

int SLAudioDevice::SetStereoRecording(bool bEnabled)
{
    CHECK_TRUE(m_bInitialized, "Not initialized");
    CHECK_RECORDING_NOT_INITIALIZED();
    AUDIO_OPENSLES_DEBUG_INFO("SetStereoRecording(%s)", bEnabled ? "True" : "False");
    m_bStereoRecording = bEnabled;
    return 0;
}

int SLAudioDevice::SetRecordingSampleRate(int nRecordingSampleRate)
{
    CHECK_TRUE(m_bInitialized, "Not initialized");
    CHECK_RECORDING_NOT_INITIALIZED();

    AUDIO_OPENSLES_DEBUG_INFO("SetRecordingSampleRate(%d)", nRecordingSampleRate);

    switch(nRecordingSampleRate) {
    case 8000:
    case 11025:
    case 16000:
    case 22050:
    case 24000:
    case 32000:
    case 44100:
    case 64000:
    case 88200:
    case 96000:
    case 192000: {
        m_nRecordingSampleRate = nRecordingSampleRate;
        return 0;
    }
    default: {
        AUDIO_OPENSLES_DEBUG_ERROR("%d not valid sampling rate", nRecordingSampleRate);
        return -1;
    }
    }
}

int SLAudioDevice::InitRecording()
{
    CHECK_TRUE(m_bInitialized, "Not initialized");

    AUDIO_OPENSLES_DEBUG_INFO("SLAudioDevice::InitRecording()");

    if (m_bRecording) {
        AUDIO_OPENSLES_DEBUG_ERROR("Recording already started");
        return -1;
    }

    if (m_bRecordingInitialized) {
        return 0;
    }

    // Initialize the microphone
    if (InitMicrophone() == -1) {
        AUDIO_OPENSLES_DEBUG_ERROR("InitMicrophone() failed");
    }

#if AUDIO_OPENSLES_UNDER_ANDROID

    if (m_slEngineObject == NULL || m_slEngine == NULL) {
        AUDIO_OPENSLES_DEBUG_ERROR("Recording object is NULL");
        return -1;
    }

    SLresult slResult;
    SLDataSource audioSource;
    SLDataLocator_IODevice micLocator;
    SLDataSink audioSink;
    SLDataFormat_PCM pcm;
    SLDataLocator_AndroidSimpleBufferQueue simpleBufferQueue;

    // Setup the data source structure
    micLocator.locatorType = SL_DATALOCATOR_IODEVICE;
    micLocator.deviceType = SL_IODEVICE_AUDIOINPUT;
    micLocator.deviceID = SL_DEFAULTDEVICEID_AUDIOINPUT; //micDeviceID;
    micLocator.device = NULL;
    audioSource.pLocator = (void *) &micLocator;
    audioSource.pFormat = NULL;

    /* Setup the data source structure for the buffer queue */
    simpleBufferQueue.locatorType = SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;
    simpleBufferQueue.numBuffers = N_REC_QUEUE_BUFFERS;
    /* Setup the format of the content in the buffer queue */
    pcm.formatType = SL_DATAFORMAT_PCM;
    pcm.numChannels = 1;
    // _samplingRateIn is initialized in initSampleRate()
    pcm.samplesPerSec = SL_SAMPLING_RATE(m_nRecordingSampleRate);
    pcm.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
    pcm.containerSize = 16;
    pcm.channelMask = SL_SPEAKER_FRONT_CENTER;
    pcm.endianness = SL_BYTEORDER_LITTLEENDIAN;
    audioSink.pFormat = (void *) &pcm;
    audioSink.pLocator = (void *) &simpleBufferQueue;

    const SLInterfaceID id[2] = { SL_IID_ANDROIDSIMPLEBUFFERQUEUE, SL_IID_ANDROIDCONFIGURATION };
    const SLboolean req[2] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };
    slResult = (*m_slEngine)->CreateAudioRecorder(m_slEngine, &m_slRecorder, &audioSource, &audioSink, 2, id, req);
    if (slResult != SL_RESULT_SUCCESS) {
        AUDIO_OPENSLES_DEBUG_ERROR("Failed to create Recorder with error code = %d", slResult);
        return -1;
    }

    // Set stream type
    SLAndroidConfigurationItf slRecorderConfig;
    SLint32 slStreamType = SL_ANDROID_RECORDING_PRESET_GENERIC;
    slResult = (*m_slRecorder)->GetInterface(m_slRecorder, SL_IID_ANDROIDCONFIGURATION, &slRecorderConfig);
    if(slResult != SL_RESULT_SUCCESS) {
        AUDIO_OPENSLES_DEBUG_ERROR("GetInterface(SL_IID_ANDROIDCONFIGURATION) failed with error code = %d", slResult);
        return -1;
    }
    AUDIO_OPENSLES_DEBUG_INFO("Recording stream type = %d", slStreamType);
    slResult = (*slRecorderConfig)->SetConfiguration(slRecorderConfig, SL_ANDROID_KEY_RECORDING_PRESET, &slStreamType, sizeof(SLint32));
    if(slResult != SL_RESULT_SUCCESS) {
        AUDIO_OPENSLES_DEBUG_ERROR("SetConfiguration(SL_ANDROID_KEY_RECORDING_PRESET) failed with error code = %d", slResult);
        return -1;
    }


    // Realizing the recorder in synchronous mode.
    slResult = (*m_slRecorder)->Realize(m_slRecorder, SL_BOOLEAN_FALSE);
    if (slResult != SL_RESULT_SUCCESS) {
        AUDIO_OPENSLES_DEBUG_ERROR("Failed to realize Recorder with error code = %d", slResult);
        return -1;
    }

    // Get the RECORD interface - it is an implicit interface
    slResult = (*m_slRecorder)->GetInterface(m_slRecorder, SL_IID_RECORD, (void*) &m_slRecorderRecord);
    if (slResult != SL_RESULT_SUCCESS) {
        AUDIO_OPENSLES_DEBUG_ERROR("Failed to get Recorder interface with error code = %d", slResult);
        return -1;
    }

    // Get the simpleBufferQueue interface
    slResult = (*m_slRecorder)->GetInterface(m_slRecorder, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, (void*) &m_slRecorderSimpleBufferQueue);
    if (slResult != SL_RESULT_SUCCESS) {
        AUDIO_OPENSLES_DEBUG_ERROR("Failed to get Recorder Simple Buffer Queue with error code = %d", slResult);
        return -1;
    }

    // Setup to receive buffer queue event callbacks
    slResult = (*m_slRecorderSimpleBufferQueue)->RegisterCallback(m_slRecorderSimpleBufferQueue, RecorderSimpleBufferQueueCallback, this);
    if (slResult != SL_RESULT_SUCCESS) {
        AUDIO_OPENSLES_DEBUG_ERROR("Failed to register Recorder Callback with error code = %d", slResult);
        return -1;
    }

#endif /* AUDIO_OPENSLES_UNDER_ANDROID */

    AUDIO_OPENSLES_DEBUG_INFO("Recording initialized");

    m_bRecordingInitialized = true;

    return 0;
}

int SLAudioDevice::StereoRecording(bool *pEnabled)
{
    CHECK_TRUE(m_bInitialized, "Not initialized");
    CHECK_RECORDING_INITIALIZED();

    if(!pEnabled) {
        AUDIO_OPENSLES_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    *pEnabled = m_bStereoRecording;
    return 0;
}

int SLAudioDevice::RecordingSampleRate(int *pRecordingSampleRate)
{
    CHECK_TRUE(m_bInitialized, "Not initialized");
    CHECK_RECORDING_INITIALIZED();

    if(!pRecordingSampleRate) {
        AUDIO_OPENSLES_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    *pRecordingSampleRate = m_nRecordingSampleRate;
    return 0;
}

int SLAudioDevice::StartRecording()
{
    AUDIO_OPENSLES_DEBUG_INFO("SLAudioDevice::StartRecording()");

    CHECK_TRUE(m_bInitialized, "Not initialized");
    CHECK_RECORDING_INITIALIZED();

    if (m_bRecording) {
        return 0;
    }

#if AUDIO_OPENSLES_UNDER_ANDROID

    if (m_slRecorderRecord == NULL) {
        AUDIO_OPENSLES_DEBUG_ERROR("RecordITF is NULL");
        return -1;
    }

    if (m_slRecorderSimpleBufferQueue == NULL) {
        AUDIO_OPENSLES_DEBUG_ERROR("Recorder Simple Buffer Queue is NULL");
        return -1;
    }

    // Reset recording buffer
    memset(_recQueueBuffer, 0, sizeof(_recQueueBuffer)); // empty the queue
    _recQueueSeq = 0;

    memset(_recBuffer, 0, sizeof(_recBuffer));
    memset(_recLength, 0, sizeof(_recLength));
    memset(_recSeqNumber, 0, sizeof(_recSeqNumber));

    // Enqueue N_REC_QUEUE_BUFFERS -1 zero buffers to get the ball rolling
    // find out how it behaves when the sample rate is 44100
    SLresult slResult;
    int nSample10ms = m_nRecordingSampleRate / 100;
    for (int i = 0; i < (N_REC_QUEUE_BUFFERS - 1); i++) {
        // We assign 10ms buffer to each queue, size given in bytes.
        slResult = (*m_slRecorderSimpleBufferQueue)->Enqueue(m_slRecorderSimpleBufferQueue, (void*) _recQueueBuffer[_recQueueSeq], (nSample10ms << BYTES_PER_SAMPLE_LOG2));
        if (slResult != SL_RESULT_SUCCESS) {
            AUDIO_OPENSLES_DEBUG_ERROR("Failed to Enqueue Empty Buffer to recorder with error code = %d", slResult);
            return -1;
        }
        _recQueueSeq++;
    }
    // Record the audio
    m_bRecording = true;
    slResult = (*m_slRecorderRecord)->SetRecordState(m_slRecorderRecord, SL_RECORDSTATE_RECORDING);
    if (slResult != SL_RESULT_SUCCESS) {
        AUDIO_OPENSLES_DEBUG_ERROR("Failed to start recording with error code = %d", slResult);
        m_bRecording = false;
        return -1;
    }
#else
    m_bRecording = true;
#endif /* AUDIO_OPENSLES_UNDER_ANDROID */

    AUDIO_OPENSLES_DEBUG_INFO("Recording started - rate = %d", m_nRecordingSampleRate);

    return 0;
}

bool SLAudioDevice::Recording()
{
    return m_bRecording;
}

int SLAudioDevice::StopRecording()
{
    AUDIO_OPENSLES_DEBUG_INFO("SLAudioDevice::StopRecording()");
    if (!m_bRecording) {
        return 0;
    }
#if AUDIO_OPENSLES_UNDER_ANDROID
    if ((m_slRecorderRecord != NULL) && (m_slRecorder != NULL)) {
        SLresult slResult = (*m_slRecorderRecord)->SetRecordState(m_slRecorderRecord, SL_RECORDSTATE_STOPPED);
        if (slResult != SL_RESULT_SUCCESS) {
            AUDIO_OPENSLES_DEBUG_ERROR("Failed to stop recording with error code = %d", slResult);
            return -1;
        }
        slResult = (*m_slRecorderSimpleBufferQueue)->Clear(m_slRecorderSimpleBufferQueue);
        if (slResult != SL_RESULT_SUCCESS) {
            AUDIO_OPENSLES_DEBUG_ERROR("Failed to clear recorder buffer queue with error code = %d", slResult);
            return -1;
        }

        // Destroy the recorder object
        (*m_slRecorder)->Destroy(m_slRecorder);
        m_slRecorder = NULL;
        m_slRecorderRecord = NULL;
        m_slRecorderRecord = NULL;
    }
#endif

    AUDIO_OPENSLES_DEBUG_INFO("Recording stopped");
    m_bRecording = false;
    m_bRecordingInitialized = false;
    return 0;
}

int SLAudioDevice::Terminate()
{
    if (!m_bInitialized) {
        return 0;
    }

    if(Recording()) {
        StopRecording();
    }

    if(Playing()) {
        StopPlayout();
    }

#if AUDIO_OPENSLES_UNDER_ANDROID

    if(m_slPlayer) {
        (*m_slPlayer)->Destroy(m_slPlayer);
        m_slPlayer = NULL;
        m_slPlayerPlay = NULL;
        m_slPlayerSimpleBufferQueue = NULL;
    }

    if(m_slRecorder) {
        (*m_slRecorder)->Destroy(m_slRecorder);
        m_slRecorder = NULL;
        m_slRecorderRecord = NULL;
        m_slRecorderSimpleBufferQueue = NULL;
        m_slAudioIODeviceCapabilities = NULL;
    }

    if(m_slOutputMixObject) {
        (*m_slOutputMixObject)->Destroy(m_slOutputMixObject);
        m_slOutputMixObject = NULL;
    }

    if (m_slEngineObject) {
        (*m_slEngineObject)->Destroy(m_slEngineObject);
        m_slEngineObject = NULL;
        m_slEngine = NULL;
    }
#endif

    m_bSpeakerInitialized = false;
    m_bPlayoutInitialized = false;
    m_bRecordingInitialized = false;
    m_bInitialized = false;

    return 0;
}

int SLAudioDevice::PlayoutApplyNewConfig()
{
    AUDIO_OPENSLES_DEBUG_INFO("SLAudioDevice::PlayoutApplyNewConfig()");
#if AUDIO_OPENSLES_UNDER_ANDROID
    if(m_slPlayer) {
        SLresult slResult;
        int ret;
        bool wasPlaying = Playing();

        if(wasPlaying) {
            if ((ret = StopPlayout())) {
                AUDIO_OPENSLES_DEBUG_ERROR("Failed to stop playout for reconf");
                return ret;
            }
            if((ret = InitPlayout())) {
                AUDIO_OPENSLES_DEBUG_ERROR("Failed to init() playout after reconf");
                return ret;
            }
        }

        if(wasPlaying) {
            if((ret = StartPlayout())) {
                AUDIO_OPENSLES_DEBUG_ERROR("Failed to start() playout after reconf");
                return ret;
            }
        }
    }
#endif /* AUDIO_OPENSLES_UNDER_ANDROID */
    return 0;
}



uint32_t SLAudioDevice::PullPlayoutData(void* pAudioSamples, const uint32_t nSamples)
{
    if(!pAudioSamples || !nSamples) {
        AUDIO_OPENSLES_DEBUG_ERROR("PullPlayoutData() - Invalid parameter");
        return 0;
    }

    if(!m_pCallback) {
        memset(pAudioSamples, 0, (nSamples << BYTES_PER_SAMPLE_LOG2));
        return nSamples;
    }

    uint32_t nSamplesOut = 0;
    const_cast<SLAudioDeviceCallback*>(m_pCallback)->NeedMorePlayData(nSamples,
            BYTES_PER_SAMPLE,
            m_bStereoPlayout ? 2 : 1,
            m_nPlayoutSampleRate,
            pAudioSamples,
            nSamplesOut);
    return nSamplesOut;
}

void SLAudioDevice::PushRecordingData(void* pAudioSamples, const uint32_t nSamples)
{
    if(!pAudioSamples || !nSamples) {
        AUDIO_OPENSLES_DEBUG_ERROR("PushRecordingData() - Invalid parameter");
        return;
    }

    if(m_pCallback) {
        const_cast<SLAudioDeviceCallback*>(m_pCallback)->RecordedDataIsAvailable(pAudioSamples,
                nSamples,
                BYTES_PER_SAMPLE,
                m_bStereoRecording ? 2 : 1,
                m_nRecordingSampleRate);
    }
}

#if AUDIO_OPENSLES_UNDER_ANDROID

void SLAudioDevice::PlayerSimpleBufferQueueCallback(SLAndroidSimpleBufferQueueItf queueItf, void *pContext)
{
    SLAudioDevice* This = static_cast<SLAudioDevice*> (pContext);

    // AUDIO_OPENSLES_DEBUG_INFO("PlayerSimpleBufferQueueCallback(playing=%s, _playQueueSeq=%d)", (This->m_bPlaying ? "true" : "false"), This->_playQueueSeq);

    if (This->m_bPlaying && (This->_playQueueSeq < N_PLAY_QUEUE_BUFFERS)) {
        unsigned int noSamp10ms = This->m_nPlayoutSampleRate / 100;
        uint8_t playBuffer[noSamp10ms << BYTES_PER_SAMPLE_LOG2];
        uint32_t noSamplesOut = This->PullPlayoutData(playBuffer, noSamp10ms);

        if (noSamp10ms != noSamplesOut) {
            if(noSamplesOut) { // (noSamplesOut==0) -> jitter buffer cannot provide data
                AUDIO_OPENSLES_DEBUG_ERROR("noSamp10ms (%u) != noSamplesOut (%d)", noSamp10ms, noSamplesOut);
            }
            noSamplesOut = noSamp10ms;
            memset(This->_playQueueBuffer[This->_playQueueSeq], 0, (noSamplesOut << BYTES_PER_SAMPLE_LOG2));
        }
        else {
            memcpy(This->_playQueueBuffer[This->_playQueueSeq], playBuffer, (noSamplesOut << BYTES_PER_SAMPLE_LOG2));
        }

        SLresult slResult = (*This->m_slPlayerSimpleBufferQueue)->Enqueue(This->m_slPlayerSimpleBufferQueue, This->_playQueueBuffer[This->_playQueueSeq], (noSamplesOut << BYTES_PER_SAMPLE_LOG2));
        if (slResult != SL_RESULT_SUCCESS) {
            AUDIO_OPENSLES_DEBUG_ERROR("Player simpler buffer queue Enqueue failed, noSamplesOut=%d, ret=%d", noSamplesOut, slResult);
            return;
        }
        // update the play buffer sequency
        This->_playQueueSeq = (This->_playQueueSeq + 1) % N_PLAY_QUEUE_BUFFERS;
    }
}

void SLAudioDevice::RecorderSimpleBufferQueueCallback(SLAndroidSimpleBufferQueueItf queueItf, void *pContext)
{
    // AUDIO_OPENSLES_DEBUG_INFO("RecorderSimpleBufferQueueCallback()");

    SLAudioDevice* This = static_cast<SLAudioDevice*> (pContext);

    if (This->m_bRecording) {
        const unsigned int noSamp10ms = This->m_nRecordingSampleRate / 100;

#if 1 // not using async thread
        // push data
        This->PushRecordingData(This->_recQueueBuffer[0], noSamp10ms);
        // enqueue new buffer
        SLresult slResult = (*This->m_slRecorderSimpleBufferQueue)->Enqueue(
                                This->m_slRecorderSimpleBufferQueue,
                                (void*) This->_recQueueBuffer[0],
                                (noSamp10ms << BYTES_PER_SAMPLE_LOG2));
        if (slResult != SL_RESULT_SUCCESS) {
            AUDIO_OPENSLES_DEBUG_WARN("Failed to enqueue recording buffer with error code = %d", slResult);
            return;
        }
#else
        unsigned int dataPos = 0;
        uint16_t bufPos = 0;
        int16_t insertPos = -1;
        unsigned int nCopy = 0; // Number of samples to copy

        while (dataPos < noSamp10ms) {
            // Loop over all recording buffers or until we find the partially
            // full buffer
            // First choice is to insert into partially full buffer,
            // second choice is to insert into empty buffer
            bufPos = 0;
            insertPos = -1;
            nCopy = 0;
            while (bufPos < N_REC_BUFFERS) {
                if ((This->_recLength[bufPos] > 0) && (This->_recLength[bufPos] < noSamp10ms)) {
                    // Found the partially full buffer
                    insertPos = static_cast<int16_t> (bufPos);
                    bufPos = N_REC_BUFFERS; // Don't need to search more
                }
                else if ((-1 == insertPos) && (0 == This->_recLength[bufPos])) {
                    // Found an empty buffer
                    insertPos = static_cast<int16_t> (bufPos);
                }
                ++bufPos;
            }

            if (insertPos > -1) {
                // We found a non-full buffer, copy data from the buffer queue
                // o recBuffer
                unsigned int dataToCopy = noSamp10ms - dataPos;
                unsigned int currentRecLen = _recLength[insertPos];
                unsigned int roomInBuffer = noSamp10ms - currentRecLen;
                nCopy = (dataToCopy < roomInBuffer ? dataToCopy : roomInBuffer);
                memcpy(&This->_recBuffer[insertPos][currentRecLen], &This->_recQueueBuffer[This->_recQueueSeq][dataPos], nCopy * sizeof(short));
                if (0 == currentRecLen) {
                    _recSeqNumber[insertPos] = This->_recCurrentSeq;
                    ++_recCurrentSeq;
                }
                This->_recBufferTotalSize += nCopy;
                // Has to be done last to avoid interrupt problems
                // between threads
                This->_recLength[insertPos] += nCopy;
                dataPos += nCopy;
            }
            else {
                // Didn't find a non-full buffer
                AUDIO_OPENSLES_DEBUG_WARN("Could not insert into recording buffer");
                dataPos = noSamp10ms; // Don't try to insert more
            }
        }

        // clean the queue buffer
        // Start with empty buffer
        memset(This->_recQueueBuffer[This->_recQueueSeq], 0, (REC_BUF_SIZE_IN_SAMPLES << BYTES_PER_SAMPLE_LOG2));
        // write the empty buffer to the queue
        SLresult slResult = (*This->m_slRecorderSimpleBufferQueue)->Enqueue(
                                This->m_slRecorderSimpleBufferQueue,
                                (void*) This->_recQueueBuffer[This->_recQueueSeq],
                                (noSamp10ms << BYTES_PER_SAMPLE_LOG2));
        if (slResult != SL_RESULT_SUCCESS) {
            AUDIO_OPENSLES_DEBUG_WARN("Failed to enqueue recording buffer with error code = %d", slResult);
            return;
        }

        // update the rec queue seq
        This->_recQueueSeq = (This->_recQueueSeq + 1) % N_REC_QUEUE_BUFFERS;

        // alert thread
        // TODO
#endif
    }
}

#endif /* AUDIO_OPENSLES_UNDER_ANDROID */
