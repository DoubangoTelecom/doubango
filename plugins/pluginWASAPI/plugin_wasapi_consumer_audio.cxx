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
/**@file plugin_wasapi_consumer_audio.cxx
* @brief Microsoft Windows Audio Session API (WASAPI) consumer.
* http://msdn.microsoft.com/en-us/library/windows/desktop/dd316551(v=vs.85).aspx
*/
#include "plugin_wasapi_utils.h"

#include "tinydav/audio/tdav_consumer_audio.h"

#include "tsk_thread.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_condwait.h"
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

#if !defined(PLUGIN_WASAPI_CONSUMER_NOTIF_POS_COUNT)
#       define PLUGIN_WASAPI_CONSUMER_NOTIF_POS_COUNT             13
#endif
#define WASAPI_MILLIS_TO_100NS(MILLIS) (((LONGLONG)(MILLIS)) * 10000ui64)
#define WASAPI_100NS_TO_MILLIS(NANOS) (((LONGLONG)(NANOS)) / 10000ui64)

struct plugin_wasapi_consumer_audio_s;

class AudioRender sealed
{
public:
    AudioRender();
    virtual ~AudioRender();

    int Prepare(struct plugin_wasapi_consumer_audio_s* wasapi, const tmedia_codec_t* codec);
    int UnPrepare();
    int Start();
    int Stop();
    int Pause();
    int Consume(const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr);
private:
    tsk_size_t Read(void* data, tsk_size_t size);
    static void* TSK_STDCALL AsyncThread(void *pArg);

private:
    tsk_mutex_handle_t* m_hMutex;
    const struct plugin_wasapi_consumer_audio_s* m_pWrappedConsumer; // Must not take ref() otherwise dtor() will be never called (circular reference)
#if PLUGIN_WASAPI_UNDER_WINDOWS_PHONE
    IAudioClient2* m_pDevice;
#else
    IAudioClient* m_pDevice;
#endif
    IAudioRenderClient* m_pClient;
    tsk_condwait_handle_t* m_hCondWait;
    tsk_thread_handle_t* m_ppTread[1];
    INT32 m_nBytesPerNotif;
    INT32 m_nSourceFrameSizeInBytes;
    UINT32 m_nMaxFrameCount;
    UINT32 m_nPtime;
    UINT32 m_nChannels;

    struct {
        struct {
            void* buffer;
            tsk_size_t size;
        } chunck;
        tsk_ssize_t leftBytes;
        SpeexBuffer* buffer;
        tsk_size_t size;
    } m_ring;

    bool m_bStarted;
    bool m_bPrepared;
    bool m_bPaused;
};

typedef struct plugin_wasapi_consumer_audio_s {
    TDAV_DECLARE_CONSUMER_AUDIO;

    AudioRender* pAudioRender;
}
plugin_wasapi_consumer_audio_t;


/* ============ Media consumer Interface ================= */

static int plugin_wasapi_consumer_audio_set(tmedia_consumer_t* self, const tmedia_param_t* param)
{
    return tdav_consumer_audio_set(TDAV_CONSUMER_AUDIO(self), param);
}

static int plugin_wasapi_consumer_audio_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
    plugin_wasapi_consumer_audio_t* wasapi = (plugin_wasapi_consumer_audio_t*)self;

    if(!wasapi || !codec || !wasapi->pAudioRender) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    TMEDIA_CONSUMER(wasapi)->audio.ptime = TMEDIA_CODEC_PTIME_AUDIO_DECODING(codec);
    TMEDIA_CONSUMER(wasapi)->audio.in.channels = TMEDIA_CODEC_CHANNELS_AUDIO_DECODING(codec);
    TMEDIA_CONSUMER(wasapi)->audio.in.rate = TMEDIA_CODEC_RATE_DECODING(codec);

    TSK_DEBUG_INFO("WASAPI consumer: in.channels=%d, out.channles=%d, in.rate=%d, out.rate=%d, ptime=%d",
                   TMEDIA_CONSUMER(wasapi)->audio.in.channels,
                   TMEDIA_CONSUMER(wasapi)->audio.out.channels,
                   TMEDIA_CONSUMER(wasapi)->audio.in.rate,
                   TMEDIA_CONSUMER(wasapi)->audio.out.rate,
                   TMEDIA_CONSUMER(wasapi)->audio.ptime);

    return wasapi->pAudioRender->Prepare(wasapi, codec);
}

static int plugin_wasapi_consumer_audio_start(tmedia_consumer_t* self)
{
    plugin_wasapi_consumer_audio_t* wasapi = (plugin_wasapi_consumer_audio_t*)self;

    TSK_DEBUG_INFO("plugin_wasapi_consumer_audio_start()");

    if(!wasapi || !wasapi->pAudioRender) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    return wasapi->pAudioRender->Start();
}


static int plugin_wasapi_consumer_audio_consume(tmedia_consumer_t* self, const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
    plugin_wasapi_consumer_audio_t* wasapi = (plugin_wasapi_consumer_audio_t*)self;
    if(!wasapi || !wasapi->pAudioRender || !buffer || !size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    return wasapi->pAudioRender->Consume(buffer, size, proto_hdr);
}

static int plugin_wasapi_consumer_audio_pause(tmedia_consumer_t* self)
{
    plugin_wasapi_consumer_audio_t* wasapi = (plugin_wasapi_consumer_audio_t*)self;

    if(!wasapi || !wasapi->pAudioRender) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    return wasapi->pAudioRender->Pause();
}

static int plugin_wasapi_consumer_audio_stop(tmedia_consumer_t* self)
{
    plugin_wasapi_consumer_audio_t* wasapi = (plugin_wasapi_consumer_audio_t*)self;

    TSK_DEBUG_INFO("plugin_wasapi_consumer_audio_stop()");

    if(!wasapi || !wasapi->pAudioRender) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    return wasapi->pAudioRender->Stop();
}







AudioRender::AudioRender()
    : m_pDevice(NULL)
    , m_hMutex(NULL)
    , m_pClient(NULL)
    , m_hCondWait(NULL)
    , m_pWrappedConsumer(NULL)
    , m_nBytesPerNotif(0)
    , m_nSourceFrameSizeInBytes(0)
    , m_nMaxFrameCount(0)
    , m_nPtime(0)
    , m_nChannels(1)
    , m_bStarted(false)
    , m_bPrepared(false)
    , m_bPaused(false)
{
    m_ppTread[0] = NULL;
    memset(&m_ring, 0, sizeof(m_ring));

    if(!(m_hMutex = tsk_mutex_create())) {
        TSK_DEBUG_ERROR("Failed to create mutex");
    }
}

AudioRender::~AudioRender()
{
    Stop();
    UnPrepare();

    tsk_mutex_destroy(&m_hMutex);
}

int AudioRender::Prepare(plugin_wasapi_consumer_audio_t* wasapi, const tmedia_codec_t* codec)
{
    HRESULT hr = E_FAIL;
    int ret = 0;
    WAVEFORMATEX wfx = {0};
#if PLUGIN_WASAPI_UNDER_WINDOWS_PHONE
    AudioClientProperties properties = {0};
#endif
    LPCWSTR pwstrRenderId = NULL;
    IMMDeviceEnumerator *pEnumerator = NULL;
    IMMDevice *pDevice = NULL;

    tsk_mutex_lock(m_hMutex);

    if(m_bPrepared) {
        TSK_DEBUG_INFO("#WASAPI: Audio consumer already prepared");
        goto bail;
    }

    if(!wasapi || !codec) {
        TSK_DEBUG_ERROR("Invalid parameter");
        CHECK_HR(hr = E_FAIL);
    }

    if(m_pDevice || m_pClient) {
        TSK_DEBUG_ERROR("consumer already prepared");
        CHECK_HR(hr = E_FAIL);
    }
#if PLUGIN_WASAPI_UNDER_WINDOWS_PHONE
    pwstrRenderId = GetDefaultAudioRenderId(AudioDeviceRole::Communications);

    if (NULL == pwstrRenderId) {
        PLUGIN_WASAPI_ERROR("GetDefaultAudioRenderId", HRESULT_FROM_WIN32(GetLastError()));
        CHECK_HR(hr = E_FAIL);
    }

    CHECK_HR(hr = ActivateAudioInterface(pwstrRenderId, __uuidof(IAudioClient2), (void**)&m_pDevice));

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
                      eRender, eCommunications, &pDevice));

    CHECK_HR(hr = pDevice->Activate(
                      IID_IAudioClient, CLSCTX_ALL,
                      NULL, (void**)&m_pDevice));
#endif




    /* Set best format */
    {
        wfx.wFormatTag = WAVE_FORMAT_PCM;
        wfx.nChannels = TMEDIA_CONSUMER(wasapi)->audio.in.channels;
        wfx.nSamplesPerSec = TMEDIA_CONSUMER(wasapi)->audio.in.rate;
        wfx.wBitsPerSample = TMEDIA_CONSUMER(wasapi)->audio.bits_per_sample;
        wfx.nBlockAlign = (wfx.nChannels * wfx.wBitsPerSample/8);
        wfx.nAvgBytesPerSec = (wfx.nSamplesPerSec * wfx.nBlockAlign);

        PWAVEFORMATEX pwfxClosestMatch = NULL;
        hr = m_pDevice->IsFormatSupported(AUDCLNT_SHAREMODE_SHARED, &wfx, &pwfxClosestMatch);
        if(hr != S_OK && hr != S_FALSE) {
            PLUGIN_WASAPI_ERROR(HRESULT_FROM_WIN32(GetLastError()));
            CHECK_HR(hr = E_FAIL);
        }

        if(hr == S_FALSE) {
            if(!pwfxClosestMatch) {
                TSK_DEBUG_ERROR("malloc(%d) failed", sizeof(WAVEFORMATEX));
                CHECK_HR(hr = E_OUTOFMEMORY);
            }

            wfx.nSamplesPerSec = pwfxClosestMatch->nSamplesPerSec;
            wfx.nChannels = pwfxClosestMatch->nChannels;
#if 0
            wfx.wBitsPerSample = pwfxClosestMatch->wBitsPerSample;
#endif
            wfx.nBlockAlign       = wfx.nChannels * (wfx.wBitsPerSample / 8);
            wfx.nAvgBytesPerSec   = wfx.nSamplesPerSec * wfx.nBlockAlign;
            // Request resampler
            TMEDIA_CONSUMER(wasapi)->audio.out.rate = (uint32_t)wfx.nSamplesPerSec;
            TMEDIA_CONSUMER(wasapi)->audio.bits_per_sample = (uint8_t)wfx.wBitsPerSample;
            TMEDIA_CONSUMER(wasapi)->audio.out.channels = (uint8_t)wfx.nChannels;

            TSK_DEBUG_INFO("Audio device format fallback: rate=%d, bps=%d, channels=%d", wfx.nSamplesPerSec, wfx.wBitsPerSample, wfx.nChannels);
        }
        if(pwfxClosestMatch) {
            CoTaskMemFree(pwfxClosestMatch);
        }
    }

    m_nSourceFrameSizeInBytes = (wfx.wBitsPerSample >> 3) * wfx.nChannels;
    m_nBytesPerNotif = ((wfx.nAvgBytesPerSec * TMEDIA_CONSUMER(wasapi)->audio.ptime)/1000) * wfx.nChannels;

    // Initialize
    CHECK_HR(hr = m_pDevice->Initialize(
                      AUDCLNT_SHAREMODE_SHARED,
                      0x00000000,
                      (PLUGIN_WASAPI_CONSUMER_NOTIF_POS_COUNT * WASAPI_MILLIS_TO_100NS(TMEDIA_CONSUMER(wasapi)->audio.ptime)) ,
                      0,
                      &wfx,
                      NULL));

    REFERENCE_TIME DefaultDevicePeriod, MinimumDevicePeriod;
    CHECK_HR(hr = m_pDevice->GetDevicePeriod(&DefaultDevicePeriod, &MinimumDevicePeriod));

    CHECK_HR(hr = m_pDevice->GetBufferSize(&m_nMaxFrameCount));
    TSK_DEBUG_INFO("#WASAPI (Playback): BufferSize=%u, DefaultDevicePeriod=%lld ms, MinimumDevicePeriod=%lldms", m_nMaxFrameCount, WASAPI_100NS_TO_MILLIS(DefaultDevicePeriod), WASAPI_100NS_TO_MILLIS(MinimumDevicePeriod));

    if(!m_hCondWait) {
        if(!(m_hCondWait = tsk_condwait_create())) {
            PLUGIN_WASAPI_ERROR(HRESULT_FROM_WIN32(GetLastError()));
            CHECK_HR(hr = E_FAIL);
        }
    }

    CHECK_HR(hr = m_pDevice->GetService(__uuidof(IAudioRenderClient), (void**)&m_pClient));

    int packetperbuffer = (1000 / TMEDIA_CONSUMER(wasapi)->audio.ptime);
    m_ring.chunck.size = (wfx.nSamplesPerSec * (wfx.wBitsPerSample >> 3) / packetperbuffer) * wfx.nChannels;
    m_ring.size = PLUGIN_WASAPI_CONSUMER_NOTIF_POS_COUNT * m_ring.chunck.size;
    if(!(m_ring.chunck.buffer = tsk_realloc(m_ring.chunck.buffer, m_ring.chunck.size))) {
        m_ring.size = 0;
        TSK_DEBUG_ERROR("Failed to allocate new buffer");
        CHECK_HR(hr = E_OUTOFMEMORY);
    }
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

bail:
    ret = SUCCEEDED(hr) ? 0 : -1;
    if (pwstrRenderId) {
        CoTaskMemFree((LPVOID)pwstrRenderId);
    }
    if(ret != 0) {
        UnPrepare();
    }

    if((m_bPrepared = (ret == 0))) {
        m_pWrappedConsumer = wasapi;
        m_nPtime = TMEDIA_CONSUMER(wasapi)->audio.ptime;
        m_nChannels = TMEDIA_CONSUMER(wasapi)->audio.out.channels;
    }

    tsk_mutex_unlock(m_hMutex);

    SafeRelease(&pEnumerator);
    SafeRelease(&pDevice);

    return ret;
}

int AudioRender::UnPrepare()
{
    tsk_mutex_lock(m_hMutex);

    if(m_hCondWait) {
        tsk_condwait_destroy(&m_hCondWait);
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

    m_pWrappedConsumer = NULL;

    m_bPrepared = false;

    tsk_mutex_unlock(m_hMutex);

    return 0;
}

int AudioRender::Start()
{
    tsk_mutex_lock(m_hMutex);

    if(m_bStarted) {
        TSK_DEBUG_INFO("#WASAPI: Audio consumer already started");
        goto bail;
    }
    if(!m_bPrepared) {
        TSK_DEBUG_ERROR("Audio consumer not prepared");
        goto bail;
    }

    m_bStarted = true;
    if(!m_ppTread[0] && tsk_thread_create(m_ppTread, AudioRender::AsyncThread, this) != 0) {
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

int AudioRender::Stop()
{
    m_bStarted = false;

    tsk_mutex_lock(m_hMutex);

    if (m_hCondWait) {
        tsk_condwait_broadcast(m_hCondWait);
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

int AudioRender::Pause()
{
    m_bPaused = true;

    return 0;
}

int AudioRender::Consume(const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
    return tdav_consumer_audio_put(TDAV_CONSUMER_AUDIO(m_pWrappedConsumer), buffer, size, proto_hdr);
}

tsk_size_t AudioRender::Read(void* data, tsk_size_t size)
{
    tsk_ssize_t retSize = 0;

    m_ring.leftBytes += size;
    while (m_ring.leftBytes >= (tsk_ssize_t)m_ring.chunck.size) {
        m_ring.leftBytes -= m_ring.chunck.size;
        retSize =  (tsk_ssize_t)tdav_consumer_audio_get(TDAV_CONSUMER_AUDIO(m_pWrappedConsumer), m_ring.chunck.buffer, m_ring.chunck.size);
        tdav_consumer_audio_tick(TDAV_CONSUMER_AUDIO(m_pWrappedConsumer));
        speex_buffer_write(m_ring.buffer, m_ring.chunck.buffer, retSize);
    }
    // IMPORTANT: looks like there is a bug in speex: continously trying to read more than avail
    // many times can corrupt the buffer. At least on OS X 1.5
    int avail = speex_buffer_get_available(m_ring.buffer);
    //if(speex_buffer_get_available(m_ring.buffer) >= (tsk_ssize_t)size)
    //{
    retSize = speex_buffer_read(m_ring.buffer, data, TSK_MIN((int)size,avail));
    //}
    //else
    //{
    //memset(data, 0, size);
    //}

    return retSize;
}

void* TSK_STDCALL AudioRender::AsyncThread(void *pArg)
{
    HRESULT hr = S_OK;
    INT32 nFramesToWrite;
    UINT32 nPadding, nRead;
    int waitResult = 0;
    AudioRender* This = (AudioRender*)pArg;

    TSK_DEBUG_INFO("#WASAPI: __playback_thread -- START");

#define BREAK_WHILE tsk_mutex_unlock(This->m_hMutex); break;

    while(This->m_bStarted && SUCCEEDED(hr)) {
        waitResult = tsk_condwait_timedwait(This->m_hCondWait, This->m_nPtime);

        tsk_mutex_lock(This->m_hMutex);

        if(!This->m_bStarted) {
            BREAK_WHILE;
        }

        if(waitResult == 0) {
            hr = This->m_pDevice->GetCurrentPadding(&nPadding);
            if (SUCCEEDED(hr)) {
                BYTE* pRenderBuffer = NULL;
                nFramesToWrite = This->m_nMaxFrameCount - nPadding;

                if (nFramesToWrite > 0) {
                    hr = This->m_pClient->GetBuffer(nFramesToWrite, &pRenderBuffer);
                    if (SUCCEEDED(hr)) {
                        nRead = This->Read(pRenderBuffer, (nFramesToWrite * This->m_nSourceFrameSizeInBytes));

                        // Release the buffer
                        hr = This->m_pClient->ReleaseBuffer((nRead / This->m_nSourceFrameSizeInBytes), (nRead == 0) ? AUDCLNT_BUFFERFLAGS_SILENT: 0);
                    }
                }
            }
        }
        else {
            BREAK_WHILE;
        }

        tsk_mutex_lock(This->m_hMutex);
    }// end-of-while

    if (!SUCCEEDED(hr)) {
        PLUGIN_WASAPI_ERROR(hr);
    }

    TSK_DEBUG_INFO("WASAPI: __playback_thread(%s) -- STOP", (SUCCEEDED(hr) && waitResult == 0) ? "OK": "NOK");

    return NULL;
}







//
//      WaveAPI consumer object definition
//
/* constructor */
static tsk_object_t* plugin_wasapi_consumer_audio_ctor(tsk_object_t * self, va_list * app)
{
    plugin_wasapi_consumer_audio_t *wasapi = (plugin_wasapi_consumer_audio_t*)self;
    if(wasapi) {
        WASAPIUtils::Startup();

        /* init base */
        tdav_consumer_audio_init(TDAV_CONSUMER_AUDIO(wasapi));
        /* init self */

        wasapi->pAudioRender = new AudioRender();
        if(!wasapi->pAudioRender) {
            TSK_DEBUG_ERROR("Failed to create renderer");
            return tsk_null;
        }
    }
    return self;
}
/* destructor */
static tsk_object_t* plugin_wasapi_consumer_audio_dtor(tsk_object_t * self)
{
    plugin_wasapi_consumer_audio_t *wasapi = (plugin_wasapi_consumer_audio_t*)self;
    if(wasapi) {
        /* stop */
        plugin_wasapi_consumer_audio_stop((tmedia_consumer_t*)self);
        /* deinit base */
        tdav_consumer_audio_deinit(TDAV_CONSUMER_AUDIO(wasapi));
        /* deinit self */
        if(wasapi->pAudioRender) {
            delete wasapi->pAudioRender;
            wasapi->pAudioRender = NULL;
        }
    }

    return self;
}
/* object definition */
static const tsk_object_def_t plugin_wasapi_consumer_audio_def_s = {
    sizeof(plugin_wasapi_consumer_audio_t),
    plugin_wasapi_consumer_audio_ctor,
    plugin_wasapi_consumer_audio_dtor,
    tdav_consumer_audio_cmp,
};
/* plugin definition*/
static const tmedia_consumer_plugin_def_t plugin_wasapi_consumer_audio_plugin_def_s = {
    &plugin_wasapi_consumer_audio_def_s,

    tmedia_audio,
    "Microsoft Windows Audio Session API (WASAPI) consumer",

    plugin_wasapi_consumer_audio_set,
    plugin_wasapi_consumer_audio_prepare,
    plugin_wasapi_consumer_audio_start,
    plugin_wasapi_consumer_audio_consume,
    plugin_wasapi_consumer_audio_pause,
    plugin_wasapi_consumer_audio_stop
};
const tmedia_consumer_plugin_def_t *plugin_wasapi_consumer_audio_plugin_def_t = &plugin_wasapi_consumer_audio_plugin_def_s;

