/*Copyright (C) 2013 Mamadou Diop
* Copyright (C) 2013 Doubango Telecom <http://www.doubango.org>
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
/**@file plugin_wasapi_producer_audio.cxx
* @brief Microsoft Windows Audio Session API (WASAPI) producer.
* http://msdn.microsoft.com/en-us/library/windows/desktop/dd316551(v=vs.85).aspx
*/
#include "plugin_wasapi_utils.h"

#include "tinydav/audio/tdav_producer_audio.h"

#include "tsk_thread.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_debug.h"

#include <windows.h>
#include <audioclient.h>
#if PLUGIN_WASAPI_UNDER_WINDOWS_PHONE
#	include <phoneaudioclient.h>
#else
#	include <Mmdeviceapi.h>
#endif
#include <initguid.h>

#include <speex/speex_buffer.h>

static const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
static const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
static const IID IID_IAudioClient = __uuidof(IAudioClient);
static const IID IID_IAudioCaptureClient = __uuidof(IAudioCaptureClient);

#if !defined(PLUGIN_WASAPI_PRODUCER_NOTIF_POS_COUNT)
#       define PLUGIN_WASAPI_PRODUCER_NOTIF_POS_COUNT             10
#endif
#define WASAPI_MILLIS_TO_100NS(MILLIS) (((LONGLONG)(MILLIS)) * 10000ui64)
#define WASAPI_100NS_TO_MILLIS(NANOS) (((LONGLONG)(NANOS)) / 10000ui64)

struct plugin_wasapi_producer_audio_s;

class AudioCapture
{
public:
    AudioCapture();
    virtual ~AudioCapture();

    int Prepare(struct plugin_wasapi_producer_audio_s* wasapi, const tmedia_codec_t* codec);
    int UnPrepare();
    int Start();
    int Stop();
    int Pause();

private:
    static void* TSK_STDCALL AsyncThread(void *pArg);

private:
    tsk_mutex_handle_t* m_hMutex;
#if PLUGIN_WASAPI_UNDER_WINDOWS_PHONE
    IAudioClient2* m_pDevice;
#else
    IAudioClient* m_pDevice;
#endif
    IAudioCaptureClient* m_pClient;
    HANDLE m_hCaptureEvent;
    HANDLE m_hShutdownEvent;
    tsk_thread_handle_t* m_ppTread[1];
    INT32 m_nBytesPerNotif;
    INT32 m_nSourceFrameSizeInBytes;

    struct {
        tmedia_producer_enc_cb_f fn;
        const void* pcData;
    } m_callback;

    struct {
        struct {
            void* buffer;
            tsk_size_t size;
        } chunck;
        SpeexBuffer* buffer;
        tsk_size_t size;
    } m_ring;
    bool m_bStarted;
    bool m_bPrepared;
    bool m_bPaused;
};

typedef struct plugin_wasapi_producer_audio_s {
    TDAV_DECLARE_PRODUCER_AUDIO;
    AudioCapture* pAudioCapture;
}
plugin_wasapi_producer_audio_t;


/* ============ Media Producer Interface ================= */
static int plugin_wasapi_producer_audio_set(tmedia_producer_t* self, const tmedia_param_t* param)
{
    plugin_wasapi_producer_audio_t* wasapi = (plugin_wasapi_producer_audio_t*)self;
    if(param->plugin_type == tmedia_ppt_producer) {
        if(param->value_type == tmedia_pvt_int32) {
            if(tsk_striequals(param->key, "volume")) {
                return 0;
            }
            else if(tsk_striequals(param->key, "mute")) {
                //wasapi->mute = (TSK_TO_INT32((uint8_t*)param->value) != 0);
#if !FIXME_SEND_SILENCE_ON_MUTE
                //if(wasapi->started){
                //      if(wasapi->mute){
                //IDirectSoundCaptureBuffer_Stop(wasapi->captureBuffer);
                //      }
                //      else{
                //IDirectSoundCaptureBuffer_Start(wasapi->captureBuffer, DSBPLAY_LOOPING);
                //      }
                //}
#endif
                return 0;
            }
        }
    }
    return tdav_producer_audio_set(TDAV_PRODUCER_AUDIO(self), param);
}



static int plugin_wasapi_producer_audio_prepare(tmedia_producer_t* self, const tmedia_codec_t* codec)
{
    plugin_wasapi_producer_audio_t* wasapi = (plugin_wasapi_producer_audio_t*)self;

    if(!wasapi || !codec || !wasapi->pAudioCapture) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* codec should have ptime */
    TMEDIA_PRODUCER(wasapi)->audio.channels = TMEDIA_CODEC_CHANNELS_AUDIO_ENCODING(codec);
    TMEDIA_PRODUCER(wasapi)->audio.rate = TMEDIA_CODEC_RATE_ENCODING(codec);
    TMEDIA_PRODUCER(wasapi)->audio.ptime = TMEDIA_CODEC_PTIME_AUDIO_ENCODING(codec);

    TSK_DEBUG_INFO("WASAPI producer: channels=%d, rate=%d, ptime=%d",
                   TMEDIA_PRODUCER(wasapi)->audio.channels,
                   TMEDIA_PRODUCER(wasapi)->audio.rate,
                   TMEDIA_PRODUCER(wasapi)->audio.ptime);

    return wasapi->pAudioCapture->Prepare(wasapi, codec);
}

static int plugin_wasapi_producer_audio_start(tmedia_producer_t* self)
{
    plugin_wasapi_producer_audio_t* wasapi = (plugin_wasapi_producer_audio_t*)self;

    TSK_DEBUG_INFO("plugin_wasapi_producer_audio_start()");

    if(!wasapi || !wasapi->pAudioCapture) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    return wasapi->pAudioCapture->Start();
}

static int plugin_wasapi_producer_audio_pause(tmedia_producer_t* self)
{
    plugin_wasapi_producer_audio_t* wasapi = (plugin_wasapi_producer_audio_t*)self;

    if(!wasapi || !wasapi->pAudioCapture) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    return wasapi->pAudioCapture->Pause();
}

static int plugin_wasapi_producer_audio_stop(tmedia_producer_t* self)
{
    plugin_wasapi_producer_audio_t* wasapi = (plugin_wasapi_producer_audio_t*)self;

    TSK_DEBUG_INFO("plugin_wasapi_producer_audio_stop()");

    if(!wasapi || !wasapi->pAudioCapture) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    return wasapi->pAudioCapture->Stop();
}







AudioCapture::AudioCapture()
    : m_pDevice(NULL)
    , m_hMutex(NULL)
    , m_pClient(NULL)
    , m_hCaptureEvent(NULL)
    , m_hShutdownEvent(NULL)
    , m_nBytesPerNotif(0)
    , m_nSourceFrameSizeInBytes(0)
    , m_bStarted(false)
    , m_bPrepared(false)
    , m_bPaused(false)
{
    m_ppTread[0] = NULL;
    memset(&m_ring, 0, sizeof(m_ring));

    m_callback.fn = NULL, m_callback.pcData = NULL;

    if(!(m_hMutex = tsk_mutex_create())) {
        TSK_DEBUG_ERROR("Failed to create mutex");
    }
}

AudioCapture::~AudioCapture()
{
    Stop();
    UnPrepare();

    tsk_mutex_destroy(&m_hMutex);
}

int AudioCapture::Prepare(plugin_wasapi_producer_audio_t* wasapi, const tmedia_codec_t* codec)
{
    HRESULT hr = S_OK;
    int ret = 0;
    WAVEFORMATEX wfx = {0};
#if PLUGIN_WASAPI_UNDER_WINDOWS_PHONE
    AudioClientProperties properties = {0};
#endif
    IMMDeviceEnumerator *pEnumerator = NULL;
    LPCWSTR pwstrCaptureId = NULL;
    IMMDevice *pDevice = NULL;

    tsk_mutex_lock(m_hMutex);

    if(m_bPrepared) {
        TSK_DEBUG_INFO("#WASAPI: Audio producer already prepared");
        goto bail;
    }

    if(!wasapi || !codec) {
        TSK_DEBUG_ERROR("Invalid parameter");
        CHECK_HR(hr = E_FAIL);
    }

    if(m_pDevice || m_pClient) {
        TSK_DEBUG_ERROR("Producer already prepared");
        CHECK_HR(hr = E_FAIL);
    }
#if PLUGIN_WASAPI_UNDER_WINDOWS_PHONE
    pwstrCaptureId = GetDefaultAudioCaptureId(AudioDeviceRole::Communications);
    if (NULL == pwstrCaptureId) {
        PLUGIN_WASAPI_ERROR("GetDefaultAudioCaptureId", HRESULT_FROM_WIN32(GetLastError()));
        CHECK_HR(hr = E_FAIL);
    }
    CHECK_HR(hr = ActivateAudioInterface(pwstrCaptureId, __uuidof(IAudioClient2), (void**)&m_pDevice));

    // Win8 or WP8 only
    properties.cbSize = sizeof AudioClientProperties;
    properties.eCategory = AudioCategory_Communications;
    CHECK_HR(hr = m_pDevice->SetClientProperties(&properties));
#else
    CHECK_HR(hr = CoCreateInstance(
                      CLSID_MMDeviceEnumerator, NULL,
                      CLSCTX_ALL, IID_IMMDeviceEnumerator,
                      (void**)&pEnumerator));

    CHECK_HR(hr = pEnumerator->GetDefaultAudioEndpoint(
                      eCapture, eCommunications, &pDevice));

    CHECK_HR(hr = pDevice->Activate(
                      IID_IAudioClient, CLSCTX_ALL,
                      NULL, (void**)&m_pDevice));
#endif


    /* Set best format */
    {
        wfx.wFormatTag = WAVE_FORMAT_PCM;
        wfx.nChannels = TMEDIA_PRODUCER(wasapi)->audio.channels;
        wfx.nSamplesPerSec = TMEDIA_PRODUCER(wasapi)->audio.rate;
        wfx.wBitsPerSample = TMEDIA_PRODUCER(wasapi)->audio.bits_per_sample;
        wfx.nBlockAlign = (wfx.nChannels * wfx.wBitsPerSample/8);
        wfx.nAvgBytesPerSec = (wfx.nSamplesPerSec * wfx.nBlockAlign);

        PWAVEFORMATEX pwfxClosestMatch = NULL;
        hr = m_pDevice->IsFormatSupported(AUDCLNT_SHAREMODE_SHARED, &wfx, &pwfxClosestMatch);
        if(hr != S_OK && hr != S_FALSE) {
            CHECK_HR(hr);
        }

        if(hr == S_FALSE) {
            if(!pwfxClosestMatch) {
                CHECK_HR(hr = E_OUTOFMEMORY);
            }
            wfx.nChannels = pwfxClosestMatch->nChannels;
            wfx.nSamplesPerSec = pwfxClosestMatch->nSamplesPerSec;
#if 0
            wfx.wBitsPerSample = pwfxClosestMatch->wBitsPerSample;
#endif
            wfx.nBlockAlign       = wfx.nChannels * (wfx.wBitsPerSample / 8);
            wfx.nAvgBytesPerSec   = wfx.nSamplesPerSec * wfx.nBlockAlign;
            // Request resampler
            TMEDIA_PRODUCER(wasapi)->audio.rate = (uint32_t)wfx.nSamplesPerSec;
            TMEDIA_PRODUCER(wasapi)->audio.bits_per_sample = (uint8_t)wfx.wBitsPerSample;
            TMEDIA_PRODUCER(wasapi)->audio.channels = (uint8_t)wfx.nChannels;

            TSK_DEBUG_INFO("Audio device format fallback: rate=%d, bps=%d, channels=%d", wfx.nSamplesPerSec, wfx.wBitsPerSample, wfx.nChannels);
        }
        if(pwfxClosestMatch) {
            CoTaskMemFree(pwfxClosestMatch);
        }
    }

    m_nSourceFrameSizeInBytes = (wfx.wBitsPerSample >> 3) * wfx.nChannels;
    m_nBytesPerNotif = ((wfx.nAvgBytesPerSec * TMEDIA_PRODUCER(wasapi)->audio.ptime)/1000) * wfx.nChannels;

    // Initialize
    CHECK_HR(hr = m_pDevice->Initialize(
                      AUDCLNT_SHAREMODE_SHARED,
                      AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
                      (PLUGIN_WASAPI_PRODUCER_NOTIF_POS_COUNT * WASAPI_MILLIS_TO_100NS(TMEDIA_PRODUCER(wasapi)->audio.ptime)),
                      0,
                      &wfx,
                      NULL));

    REFERENCE_TIME DefaultDevicePeriod, MinimumDevicePeriod;
    CHECK_HR(hr = m_pDevice->GetDevicePeriod(&DefaultDevicePeriod, &MinimumDevicePeriod));
    TSK_DEBUG_INFO("#WASAPI(Capture): DefaultDevicePeriod=%lld ms, MinimumDevicePeriod=%lldms", WASAPI_100NS_TO_MILLIS(DefaultDevicePeriod), WASAPI_100NS_TO_MILLIS(MinimumDevicePeriod));

    if(!m_hCaptureEvent) {
        if(!(m_hCaptureEvent = CreateEventEx(NULL, NULL, 0, EVENT_ALL_ACCESS))) {
            PLUGIN_WASAPI_ERROR(HRESULT_FROM_WIN32(GetLastError()));
            CHECK_HR(hr = E_FAIL);
        }
    }
    if(!m_hShutdownEvent) {
        if(!(m_hShutdownEvent = CreateEventEx(NULL, NULL, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS))) {
            PLUGIN_WASAPI_ERROR(HRESULT_FROM_WIN32(GetLastError()));
            CHECK_HR(hr = E_FAIL);
        }
    }

    CHECK_HR(hr = m_pDevice->SetEventHandle(m_hCaptureEvent));

    CHECK_HR(hr = m_pDevice->GetService(__uuidof(IAudioCaptureClient), (void**)&m_pClient));

    int packetperbuffer = (1000 / TMEDIA_PRODUCER(wasapi)->audio.ptime);
    m_ring.chunck.size = (wfx.nSamplesPerSec * (wfx.wBitsPerSample >> 3) / packetperbuffer) * wfx.nChannels;
    TSK_DEBUG_INFO("#WASAPI: Audio producer ring chunk size = %u", m_ring.chunck.size);
    // allocate our chunck buffer
    if(!(m_ring.chunck.buffer = tsk_realloc(m_ring.chunck.buffer, m_ring.chunck.size))) {
        TSK_DEBUG_ERROR("Failed to allocate new buffer");
        CHECK_HR(hr = E_OUTOFMEMORY);
    }
    // create ringbuffer
    m_ring.size = PLUGIN_WASAPI_PRODUCER_NOTIF_POS_COUNT * m_ring.chunck.size;
    TSK_DEBUG_INFO("#WASAPI: Audio producer ring size = %u", m_ring.size);
    if(!m_ring.buffer) {
        m_ring.buffer = speex_buffer_init(m_ring.size);
    }
    else {
        int sret;
        if((sret = speex_buffer_resize(m_ring.buffer, m_ring.size)) < 0) {
            TSK_DEBUG_ERROR("speex_buffer_resize(%d) failed with error code=%d", m_ring.size, sret);
            CHECK_HR(hr = E_OUTOFMEMORY);
        }
    }
    if(!m_ring.buffer) {
        TSK_DEBUG_ERROR("Failed to create a new ring buffer with size = %d", m_ring.size);
        CHECK_HR(hr = E_OUTOFMEMORY);
    }

    m_callback.fn = TMEDIA_PRODUCER(wasapi)->enc_cb.callback;
    m_callback.pcData = TMEDIA_PRODUCER(wasapi)->enc_cb.callback_data;

bail:
    ret = SUCCEEDED(hr) ? 0 : -1;
    if (pwstrCaptureId) {
        CoTaskMemFree((LPVOID)pwstrCaptureId);
    }
    if(ret != 0) {
        UnPrepare();
    }
    m_bPrepared = (ret == 0);

    tsk_mutex_unlock(m_hMutex);

    SafeRelease(&pEnumerator);
    SafeRelease(&pDevice);

    return ret;
}

int AudioCapture::UnPrepare()
{
    tsk_mutex_lock(m_hMutex);

    if(m_hCaptureEvent) {
        CloseHandle(m_hCaptureEvent), m_hCaptureEvent = NULL;
    }
    if(m_hShutdownEvent) {
        CloseHandle(m_hShutdownEvent), m_hShutdownEvent = NULL;
    }
    if(m_pDevice) {
        m_pDevice->Release(), m_pDevice = NULL;
    }
    if(m_pClient) {
        m_pClient->Release(), m_pClient = NULL;
    }

    TSK_FREE(m_ring.chunck.buffer);
    if(m_ring.buffer) {
        speex_buffer_destroy(m_ring.buffer);
        m_ring.buffer = NULL;
    }

    m_callback.fn = NULL;
    m_callback.pcData = NULL;

    m_bPrepared = false;

    tsk_mutex_unlock(m_hMutex);

    return 0;
}

int AudioCapture::Start()
{
    tsk_mutex_lock(m_hMutex);

    if(m_bStarted) {
        TSK_DEBUG_INFO("#WASAPI: Audio producer already started");
        goto bail;
    }
    if(!m_bPrepared) {
        TSK_DEBUG_ERROR("Audio producer not prepared");
        goto bail;
    }

    m_bStarted = true;
    if(!m_ppTread[0] && tsk_thread_create(m_ppTread, AudioCapture::AsyncThread, this) != 0) {
        m_bStarted = false;
        goto bail;
    }

    HRESULT hr = m_pDevice->Start();
    if(!SUCCEEDED(hr)) {
        PLUGIN_WASAPI_ERROR(hr);
        Stop();
    }
    m_bPaused = false;

bail:
    tsk_mutex_unlock(m_hMutex);

    return (m_bStarted ? 0 : -2);
}

int AudioCapture::Stop()
{
    m_bStarted = false;

    tsk_mutex_lock(m_hMutex);

    if (m_hShutdownEvent) {
        SetEvent(m_hShutdownEvent);
    }

    if (m_ppTread[0]) {
        tsk_thread_join(m_ppTread);
    }

    if(m_pDevice) {
        m_pDevice->Stop();
    }

    // will be prepared again before next start()
    UnPrepare();

    tsk_mutex_unlock(m_hMutex);

    return 0;
}

int AudioCapture::Pause()
{
    tsk_mutex_lock(m_hMutex);

    m_bPaused = true;

    tsk_mutex_unlock(m_hMutex);

    return 0;
}

void* TSK_STDCALL AudioCapture::AsyncThread(void *pArg)
{
    HRESULT hr = S_OK;
    BYTE* pbData = NULL;
    UINT32 nFrames = 0;
    DWORD dwFlags = 0;
    UINT32 incomingBufferSize;
    INT32 avail;
    UINT32 nNextPacketSize;
    AudioCapture* This = (AudioCapture*)pArg;

    HANDLE eventHandles[] = {
        This->m_hCaptureEvent,        // WAIT_OBJECT0
        This->m_hShutdownEvent                // WAIT_OBJECT1
    };

    TSK_DEBUG_INFO("#WASAPI: __record_thread -- START");

#define BREAK_WHILE tsk_mutex_unlock(This->m_hMutex); break;

    while(This->m_bStarted && SUCCEEDED(hr)) {
        DWORD waitResult = WaitForMultipleObjectsEx(SIZEOF_ARRAY(eventHandles), eventHandles, FALSE, INFINITE, FALSE);

        tsk_mutex_lock(This->m_hMutex);

        if(!This->m_bStarted) {
            BREAK_WHILE;
        }

        if(waitResult == WAIT_OBJECT_0 && This->m_callback.fn) {
            hr = This->m_pClient->GetNextPacketSize(&nNextPacketSize);
            while(SUCCEEDED(hr) && nNextPacketSize >0) {
                hr = This->m_pClient->GetBuffer(&pbData, &nFrames, &dwFlags, NULL, NULL);
                if(SUCCEEDED(hr) && pbData && nFrames) {
                    if((dwFlags & AUDCLNT_BUFFERFLAGS_SILENT) != AUDCLNT_BUFFERFLAGS_SILENT) {
                        incomingBufferSize = nFrames * This->m_nSourceFrameSizeInBytes;
                        speex_buffer_write(This->m_ring.buffer, pbData, incomingBufferSize);
                        avail = speex_buffer_get_available(This->m_ring.buffer);
                        while (This->m_bStarted && avail >= (INT32)This->m_ring.chunck.size) {
                            avail -= speex_buffer_read(This->m_ring.buffer, This->m_ring.chunck.buffer, This->m_ring.chunck.size);
#if 0
                            {
                                static FILE* f = fopen("./wasapi_producer.raw", "w+");
                                fwrite(This->m_ring.chunck.buffer, 1, This->m_ring.chunck.size, f);
                            }
#endif
                            This->m_callback.fn(This->m_callback.pcData, This->m_ring.chunck.buffer, This->m_ring.chunck.size);
                        }
                    }

                    if (SUCCEEDED(hr)) {
                        hr = This->m_pClient->ReleaseBuffer(nFrames);
                    }
                    if (SUCCEEDED(hr)) {
                        hr = This->m_pClient->GetNextPacketSize(&nNextPacketSize);
                    }
                }
            }
        }
        else if(waitResult != WAIT_OBJECT_0) {
            BREAK_WHILE;
        }

        tsk_mutex_unlock(This->m_hMutex);
    }// end-of-while

    if (!SUCCEEDED(hr)) {
        PLUGIN_WASAPI_ERROR(hr);
    }

    TSK_DEBUG_INFO("WASAPI: __record_thread(%s) -- STOP", SUCCEEDED(hr) ? "OK": "NOK");

    return NULL;
}







//
//      WaveAPI producer object definition
//
/* constructor */
static tsk_object_t* plugin_wasapi_producer_audio_ctor(tsk_object_t * self, va_list * app)
{
    plugin_wasapi_producer_audio_t *wasapi = (plugin_wasapi_producer_audio_t*)self;
    if(wasapi) {
        WASAPIUtils::Startup();

        /* init base */
        tdav_producer_audio_init(TDAV_PRODUCER_AUDIO(wasapi));
        /* init self */

        wasapi->pAudioCapture = new AudioCapture();
        if(!wasapi->pAudioCapture) {
            TSK_DEBUG_ERROR("Failed to create Audio capture device");
            return tsk_null;
        }
    }
    return self;
}
/* destructor */
static tsk_object_t* plugin_wasapi_producer_audio_dtor(tsk_object_t * self)
{
    plugin_wasapi_producer_audio_t *wasapi = (plugin_wasapi_producer_audio_t*)self;
    if(wasapi) {
        /* stop */
        plugin_wasapi_producer_audio_stop((tmedia_producer_t*)self);
        /* deinit base */
        tdav_producer_audio_deinit(TDAV_PRODUCER_AUDIO(wasapi));
        /* deinit self */
        if(wasapi->pAudioCapture) {
            delete wasapi->pAudioCapture;
            wasapi->pAudioCapture = NULL;
        }
    }

    return self;
}
/* object definition */
static const tsk_object_def_t plugin_wasapi_producer_audio_def_s = {
    sizeof(plugin_wasapi_producer_audio_t),
    plugin_wasapi_producer_audio_ctor,
    plugin_wasapi_producer_audio_dtor,
    tdav_producer_audio_cmp,
};
/* plugin definition*/
static const tmedia_producer_plugin_def_t plugin_wasapi_producer_audio_plugin_def_s = {
    &plugin_wasapi_producer_audio_def_s,

    tmedia_audio,
    "Microsoft Windows Audio Session API (WASAPI) producer",

    plugin_wasapi_producer_audio_set,
    plugin_wasapi_producer_audio_prepare,
    plugin_wasapi_producer_audio_start,
    plugin_wasapi_producer_audio_pause,
    plugin_wasapi_producer_audio_stop
};
const tmedia_producer_plugin_def_t *plugin_wasapi_producer_audio_plugin_def_t = &plugin_wasapi_producer_audio_plugin_def_s;
