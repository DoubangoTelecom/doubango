/*Copyright (C) 2013 Mamadou DIOP
* Copyright (C) 2013-2014 Doubango Telecom <http://www.doubango.org>
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
/**@file tdav_consumer_wasapi.cxx
 * @brief Microsoft Windows Audio Session API (WASAPI) consumer.
 * http://msdn.microsoft.com/en-us/library/windows/desktop/dd316551(v=vs.85).aspx
 */
#include "tinydav/audio/wasapi/tdav_consumer_wasapi.h"

#if HAVE_WASAPI

#include "tinydav/audio/tdav_consumer_audio.h"

#include "tsk_thread.h"
#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_condwait.h"
#include "tsk_debug.h"

#include <windows.h>
#include <audioclient.h>
#include <phoneaudioclient.h>

#include <speex/speex_buffer.h>

#if !defined(TDAV_WASAPI_CONSUMER_NOTIF_POS_COUNT)
#	define TDAV_WASAPI_CONSUMER_NOTIF_POS_COUNT		4
#endif
#define WASAPI_MILLIS_TO_100NS(MILLIS) (((LONGLONG)(MILLIS)) * 10000ui64)
#define WASAPI_100NS_TO_MILLIS(NANOS) (((LONGLONG)(NANOS)) / 10000ui64)

#define WASAPI_DEBUG_INFO(FMT, ...) TSK_DEBUG_INFO("[WASAPI Consumer] " FMT, ##__VA_ARGS__)
#define WASAPI_DEBUG_WARN(FMT, ...) TSK_DEBUG_WARN("[WASAPI Consumer] " FMT, ##__VA_ARGS__)
#define WASAPI_DEBUG_ERROR(FMT, ...) TSK_DEBUG_ERROR("[WASAPI Consumer] " FMT, ##__VA_ARGS__)
#define WASAPI_DEBUG_FATAL(FMT, ...) TSK_DEBUG_FATAL("[WASAPI Consumer] " FMT, ##__VA_ARGS__)

struct tdav_consumer_wasapi_s;

namespace Doubango
{
namespace VoIP
{
ref class AudioRender sealed
{
public:
    virtual ~AudioRender();
internal:
    AudioRender();

    int Prepare(struct tdav_consumer_wasapi_s* wasapi, const tmedia_codec_t* codec);
    int UnPrepare();
    int Start();
    int Stop();
    int Pause();
    int Consume(const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr);
private:
    tsk_size_t Read(void* data, tsk_size_t size);
    void AsyncThread(Windows::Foundation::IAsyncAction^ operation);

private:
    tsk_mutex_handle_t* m_hMutex;
    const struct tdav_consumer_wasapi_s* m_pWrappedConsumer; // Must not take ref() otherwise dtor() will be never called (circular reference)
    IAudioClient2* m_pDevice;
    IAudioRenderClient* m_pClient;
    HANDLE m_hEvent;
    Windows::Foundation::IAsyncAction^ m_pAsyncThread;
    INT32 m_nBytesPerNotif;
    INT32 m_nSourceFrameSizeInBytes;
    UINT32 m_nMaxFrameCount;
    UINT32 m_nPtime;

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
}
}

typedef struct tdav_consumer_wasapi_s {
    TDAV_DECLARE_CONSUMER_AUDIO;

    Doubango::VoIP::AudioRender ^AudioRender;
}
tdav_consumer_wasapi_t;

extern "C" void tdav_win32_print_error(const char* func, HRESULT hr);


/* ============ Media consumer Interface ================= */

static int tdav_consumer_wasapi_set(tmedia_consumer_t* self, const tmedia_param_t* param)
{
    return tdav_consumer_audio_set(TDAV_CONSUMER_AUDIO(self), param);
}

static int tdav_consumer_wasapi_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
    tdav_consumer_wasapi_t* wasapi = (tdav_consumer_wasapi_t*)self;

    if (!wasapi || !codec || !wasapi->AudioRender) {
        WASAPI_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    TMEDIA_CONSUMER(wasapi)->audio.ptime = TMEDIA_CODEC_PTIME_AUDIO_DECODING(codec);
    TMEDIA_CONSUMER(wasapi)->audio.in.channels = TMEDIA_CODEC_CHANNELS_AUDIO_DECODING(codec);
    TMEDIA_CONSUMER(wasapi)->audio.in.rate = TMEDIA_CODEC_RATE_DECODING(codec);

    WASAPI_DEBUG_INFO("in.channels=%d, out.channles=%d, in.rate=%d, out.rate=%d, ptime=%d",
                      TMEDIA_CONSUMER(wasapi)->audio.in.channels,
                      TMEDIA_CONSUMER(wasapi)->audio.out.channels,
                      TMEDIA_CONSUMER(wasapi)->audio.in.rate,
                      TMEDIA_CONSUMER(wasapi)->audio.out.rate,
                      TMEDIA_CONSUMER(wasapi)->audio.ptime);

    return wasapi->AudioRender->Prepare(wasapi, codec);
}

static int tdav_consumer_wasapi_start(tmedia_consumer_t* self)
{
    tdav_consumer_wasapi_t* wasapi = (tdav_consumer_wasapi_t*)self;

    WASAPI_DEBUG_INFO("tdav_consumer_wasapi_start()");

    if (!wasapi || !wasapi->AudioRender) {
        WASAPI_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    return wasapi->AudioRender->Start();
}


static int tdav_consumer_wasapi_consume(tmedia_consumer_t* self, const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
    tdav_consumer_wasapi_t* wasapi = (tdav_consumer_wasapi_t*)self;
    if (!wasapi || !wasapi->AudioRender || !buffer || !size) {
        WASAPI_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    return wasapi->AudioRender->Consume(buffer, size, proto_hdr);
}

static int tdav_consumer_wasapi_pause(tmedia_consumer_t* self)
{
    tdav_consumer_wasapi_t* wasapi = (tdav_consumer_wasapi_t*)self;

    if (!wasapi || !wasapi->AudioRender) {
        WASAPI_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    return wasapi->AudioRender->Pause();
}

static int tdav_consumer_wasapi_stop(tmedia_consumer_t* self)
{
    tdav_consumer_wasapi_t* wasapi = (tdav_consumer_wasapi_t*)self;

    WASAPI_DEBUG_INFO("tdav_consumer_wasapi_stop()");

    if (!wasapi || !wasapi->AudioRender) {
        WASAPI_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    return wasapi->AudioRender->Stop();
}







Doubango::VoIP::AudioRender::AudioRender()
    : m_pDevice(nullptr)
    , m_hMutex(nullptr)
    , m_pClient(nullptr)
    , m_hEvent(nullptr)
    , m_pAsyncThread(nullptr)
    , m_pWrappedConsumer(nullptr)
    , m_nBytesPerNotif(0)
    , m_nSourceFrameSizeInBytes(0)
    , m_nMaxFrameCount(0)
    , m_nPtime(0)
    , m_bStarted(false)
    , m_bPrepared(false)
    , m_bPaused(false)
{
    memset(&m_ring, 0, sizeof(m_ring));

    if (!(m_hMutex = tsk_mutex_create())) {
        throw ref new Platform::FailureException(L"Failed to create mutex");
    }
}

Doubango::VoIP::AudioRender::~AudioRender()
{
    Stop();
    UnPrepare();

    tsk_mutex_destroy(&m_hMutex);
}

int Doubango::VoIP::AudioRender::Prepare(tdav_consumer_wasapi_t* wasapi, const tmedia_codec_t* codec)
{
    HRESULT hr = E_FAIL;
    int ret = 0;
    WAVEFORMATEX wfx = {0};
    AudioClientProperties properties = {0};
    LPCWSTR pwstrRenderId = nullptr;

#define WASAPI_SET_ERROR(code) ret = (code); goto bail;

    tsk_mutex_lock(m_hMutex);

    if (m_bPrepared) {
        WASAPI_DEBUG_INFO("Already prepared");
        goto bail;
    }

    if (!wasapi || !codec) {
        WASAPI_DEBUG_ERROR("Invalid parameter");
        WASAPI_SET_ERROR(-1);
    }

    if (m_pDevice || m_pClient) {
        WASAPI_DEBUG_ERROR("consumer already prepared");
        WASAPI_SET_ERROR(-2);
    }

    pwstrRenderId = GetDefaultAudioRenderId(AudioDeviceRole::Communications);

    if (NULL == pwstrRenderId) {
        tdav_win32_print_error("GetDefaultAudioRenderId", HRESULT_FROM_WIN32(GetLastError()));
        WASAPI_SET_ERROR(-3);
    }

    hr = ActivateAudioInterface(pwstrRenderId, __uuidof(IAudioClient2), (void**)&m_pDevice);
    if (!SUCCEEDED(hr)) {
        tdav_win32_print_error("ActivateAudioInterface", HRESULT_FROM_WIN32(GetLastError()));
        WASAPI_SET_ERROR(-4);
    }

    if (SUCCEEDED(hr)) {
        properties.cbSize = sizeof AudioClientProperties;
        properties.eCategory = AudioCategory_Communications;
        hr = m_pDevice->SetClientProperties(&properties);
        if (!SUCCEEDED(hr)) {
            tdav_win32_print_error("SetClientProperties", HRESULT_FROM_WIN32(GetLastError()));
            WASAPI_SET_ERROR(-5);
        }
    }
    else {
        tdav_win32_print_error("ActivateAudioInterface", HRESULT_FROM_WIN32(GetLastError()));
        WASAPI_SET_ERROR(-6);
    }

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
        if (hr != S_OK && hr != S_FALSE) {
            tdav_win32_print_error("IsFormatSupported", HRESULT_FROM_WIN32(GetLastError()));
            WASAPI_SET_ERROR(-8);
        }

        if (hr == S_FALSE) {
            if (!pwfxClosestMatch) {
                WASAPI_DEBUG_ERROR("malloc(%d) failed", sizeof(WAVEFORMATEX));
                WASAPI_SET_ERROR(-7);
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

            WASAPI_DEBUG_INFO("Audio device format fallback: rate=%d, bps=%d, channels=%d", wfx.nSamplesPerSec, wfx.wBitsPerSample, wfx.nChannels);
        }
        if (pwfxClosestMatch) {
            CoTaskMemFree(pwfxClosestMatch);
        }
    }

    m_nSourceFrameSizeInBytes = (wfx.wBitsPerSample >> 3) * wfx.nChannels;
    m_nBytesPerNotif = ((wfx.nAvgBytesPerSec * TMEDIA_CONSUMER(wasapi)->audio.ptime) / 1000);

    // Initialize
    hr = m_pDevice->Initialize(
             AUDCLNT_SHAREMODE_SHARED,
             AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
             WASAPI_MILLIS_TO_100NS(TDAV_WASAPI_CONSUMER_NOTIF_POS_COUNT * TMEDIA_CONSUMER(wasapi)->audio.ptime),
             0,
             &wfx,
             NULL);
    if (!SUCCEEDED(hr)) {
        tdav_win32_print_error("#WASAPI: Render::Initialize", hr);
        WASAPI_SET_ERROR(-9);
    }

    REFERENCE_TIME DefaultDevicePeriod, MinimumDevicePeriod;
    hr = m_pDevice->GetDevicePeriod(&DefaultDevicePeriod, &MinimumDevicePeriod);
    if (!SUCCEEDED(hr)) {
        tdav_win32_print_error("GetDevicePeriod", hr);
        WASAPI_SET_ERROR(-10);
    }
    hr = m_pDevice->GetBufferSize(&m_nMaxFrameCount);
    if (!SUCCEEDED(hr)) {
        tdav_win32_print_error("GetBufferSize", hr);
        WASAPI_SET_ERROR(-10);
    }

    WASAPI_DEBUG_INFO("#WASAPI (Playback): BufferSize=%u, DefaultDevicePeriod=%lld ms, MinimumDevicePeriod=%lldms", m_nMaxFrameCount, WASAPI_100NS_TO_MILLIS(DefaultDevicePeriod), WASAPI_100NS_TO_MILLIS(MinimumDevicePeriod));

    if (!m_hEvent) {
        if (!(m_hEvent = CreateEventEx(NULL, NULL, 0, EVENT_MODIFY_STATE | SYNCHRONIZE))) {
            tdav_win32_print_error("CreateEventEx(EVENT_MODIFY_STATE | SYNCHRONIZE)", HRESULT_FROM_WIN32(GetLastError()));
            WASAPI_SET_ERROR(-11);
        }
    }

    hr = m_pDevice->SetEventHandle(m_hEvent);
    if (!SUCCEEDED(hr)) {
        tdav_win32_print_error("SetEventHandle", hr);
        WASAPI_SET_ERROR(-12);
    }

    hr = m_pDevice->GetService(__uuidof(IAudioRenderClient), (void**)&m_pClient);
    if (!SUCCEEDED(hr)) {
        tdav_win32_print_error("GetService", hr);
        WASAPI_SET_ERROR(-14);
    }

    m_ring.chunck.size = (TMEDIA_CONSUMER(wasapi)->audio.ptime * TMEDIA_CONSUMER(wasapi)->audio.out.rate * ((TMEDIA_CONSUMER(wasapi)->audio.bits_per_sample >> 3) * TMEDIA_CONSUMER(wasapi)->audio.out.channels)) / 1000;
    m_ring.size = TDAV_WASAPI_CONSUMER_NOTIF_POS_COUNT * m_ring.chunck.size;
    if (!(m_ring.chunck.buffer = tsk_realloc(m_ring.chunck.buffer, m_ring.chunck.size))) {
        m_ring.size = 0;
        WASAPI_DEBUG_ERROR("Failed to allocate new buffer");
        WASAPI_SET_ERROR(-15);
    }
    if (!m_ring.buffer) {
        m_ring.buffer = speex_buffer_init(m_ring.size);
    }
    else {
        int sret;
        if ((sret = speex_buffer_resize(m_ring.buffer, m_ring.size)) < 0) {
            WASAPI_DEBUG_ERROR("speex_buffer_resize(%d) failed with error code=%d", m_ring.size, sret);
            WASAPI_SET_ERROR(-16);
        }
    }
    if (!m_ring.buffer) {
        WASAPI_DEBUG_ERROR("Failed to create a new ring buffer with size = %d", m_ring.size);
        WASAPI_SET_ERROR(-17);
    }

bail:
    if (pwstrRenderId) {
        CoTaskMemFree((LPVOID)pwstrRenderId);
    }
    if (ret != 0) {
        UnPrepare();
    }

    if ((m_bPrepared = (ret == 0))) {
        m_pWrappedConsumer = wasapi;
        m_nPtime = TMEDIA_CONSUMER(wasapi)->audio.ptime;
    }

    tsk_mutex_unlock(m_hMutex);

    return ret;
}

int Doubango::VoIP::AudioRender::UnPrepare()
{
    tsk_mutex_lock(m_hMutex);

    if (m_hEvent) {
        CloseHandle(m_hEvent), m_hEvent = nullptr;
    }
    if (m_pDevice) {
        m_pDevice->Release(), m_pDevice = nullptr;
    }
    if (m_pClient) {
        m_pClient->Release(), m_pClient = nullptr;
    }

    TSK_FREE(m_ring.chunck.buffer);
    if (m_ring.buffer) {
        speex_buffer_destroy(m_ring.buffer);
        m_ring.buffer = nullptr;
    }

    m_pWrappedConsumer = nullptr;

    m_bPrepared = false;

    tsk_mutex_unlock(m_hMutex);

    return 0;
}

int Doubango::VoIP::AudioRender::Start()
{
    tsk_mutex_lock(m_hMutex);

    if (m_bStarted) {
        WASAPI_DEBUG_INFO("already started");
        goto bail;
    }
    if (!m_bPrepared) {
        WASAPI_DEBUG_ERROR("not prepared");
        goto bail;
    }

    m_pAsyncThread = Windows::System::Threading::ThreadPool::RunAsync(ref new Windows::System::Threading::WorkItemHandler(this, &Doubango::VoIP::AudioRender::AsyncThread),
                     Windows::System::Threading::WorkItemPriority::High,
                     Windows::System::Threading::WorkItemOptions::TimeSliced);

    if ((m_bStarted = (m_pAsyncThread != nullptr))) {
        HRESULT hr = m_pDevice->Start();
        if(!SUCCEEDED(hr)) {
            tdav_win32_print_error("Device::Start", hr);
            Stop();
        }
        m_bPaused = false;
    }

bail:
    tsk_mutex_unlock(m_hMutex);

    return (m_bStarted ? 0 : -2);
}

int Doubango::VoIP::AudioRender::Stop()
{
    m_bStarted = false;

    tsk_mutex_lock(m_hMutex);

    if (m_hEvent) {
        SetEvent(m_hEvent);
    }

    if (m_pAsyncThread) {
        m_pAsyncThread->Cancel();
        m_pAsyncThread->Close();
        m_pAsyncThread = nullptr;
    }

    if (m_pDevice) {
        m_pDevice->Stop();
    }

    // will be prepared again before next start()
    UnPrepare();

    tsk_mutex_unlock(m_hMutex);

    return 0;
}

int Doubango::VoIP::AudioRender::Pause()
{
    m_bPaused = true;

    return 0;
}

int Doubango::VoIP::AudioRender::Consume(const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
    int ret;
    // tsk_mutex_lock(m_hMutex);
    ret = tdav_consumer_audio_put(TDAV_CONSUMER_AUDIO(m_pWrappedConsumer), buffer, size, proto_hdr); // thread-safe
    // tsk_mutex_unlock(m_hMutex);
    return ret;
}

tsk_size_t Doubango::VoIP::AudioRender::Read(void* data, tsk_size_t size)
{
    tsk_ssize_t retSize = 0, availSize;

    m_ring.leftBytes += size;
    while (m_ring.leftBytes >= (tsk_ssize_t)m_ring.chunck.size) {
        m_ring.leftBytes -= m_ring.chunck.size;
        retSize =  (tsk_ssize_t)tdav_consumer_audio_get(TDAV_CONSUMER_AUDIO(m_pWrappedConsumer), m_ring.chunck.buffer, m_ring.chunck.size);
        tdav_consumer_audio_tick(TDAV_CONSUMER_AUDIO(m_pWrappedConsumer));
        speex_buffer_write(m_ring.buffer, m_ring.chunck.buffer, retSize);
    }
    // IMPORTANT: looks like there is a bug in speex: continously trying to read more than avail
    // many times can corrupt the buffer. At least on OS X 1.5
#if 0
    if (speex_buffer_get_available(m_ring.buffer) >= (tsk_ssize_t)size) {
        retSize = speex_buffer_read(m_ring.buffer, data, size);
    }
    else {
        memset(data, 0, size);
    }
#else
    availSize = speex_buffer_get_available(m_ring.buffer);
    if (availSize == 0) {
        memset(data, 0, size);
    }
    else {
        retSize = speex_buffer_read(m_ring.buffer, data, min(availSize, (tsk_ssize_t)size));
        if (availSize < (tsk_ssize_t)size) {
            memset(((uint8_t*)data) + availSize, 0, (size - availSize));
        }
    }

#endif

    return retSize;
}

void Doubango::VoIP::AudioRender::AsyncThread(Windows::Foundation::IAsyncAction^ operation)
{
    HRESULT hr = S_OK;
    INT32 nFramesToWrite;
    UINT32 nPadding, nRead;
    DWORD retval;

    WASAPI_DEBUG_INFO("#WASAPI: __playback_thread -- START");

#define BREAK_WHILE tsk_mutex_unlock(m_hMutex); break;

    while (m_bStarted && SUCCEEDED(hr)) {
        retval = WaitForSingleObjectEx(m_hEvent, /*m_nPtime*/INFINITE, FALSE);

        tsk_mutex_lock(m_hMutex);

        if (!m_bStarted) {
            BREAK_WHILE;
        }

        if (retval == WAIT_OBJECT_0) {
            hr = m_pDevice->GetCurrentPadding(&nPadding);
            if (SUCCEEDED(hr)) {
                BYTE* pRenderBuffer = NULL;
                nFramesToWrite =  m_nMaxFrameCount - nPadding;

                if (nFramesToWrite > 0) {
                    hr = m_pClient->GetBuffer(nFramesToWrite, &pRenderBuffer);
                    if (SUCCEEDED(hr)) {
                        nRead = Read(pRenderBuffer, (nFramesToWrite * m_nSourceFrameSizeInBytes));

                        // Release the buffer
                        hr = m_pClient->ReleaseBuffer(nFramesToWrite, (nRead == 0) ? AUDCLNT_BUFFERFLAGS_SILENT : 0);
                    }
                }
            }
        }

        tsk_mutex_unlock(m_hMutex);
    }// end-of-while

    if (!SUCCEEDED(hr)) {
        tdav_win32_print_error("AsyncThread: ", hr);
    }


    WASAPI_DEBUG_INFO("__playback_thread(%s) -- STOP", (SUCCEEDED(hr) && retval == WAIT_OBJECT_0) ? "OK" : "NOK");
}







//
//	WaveAPI consumer object definition
//
/* constructor */
static tsk_object_t* tdav_consumer_wasapi_ctor(tsk_object_t * self, va_list * app)
{
    tdav_consumer_wasapi_t *wasapi = (tdav_consumer_wasapi_t*)self;
    if (wasapi) {
        /* init base */
        tdav_consumer_audio_init(TDAV_CONSUMER_AUDIO(wasapi));
        /* init self */

        wasapi->AudioRender = ref new Doubango::VoIP::AudioRender();
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_consumer_wasapi_dtor(tsk_object_t * self)
{
    tdav_consumer_wasapi_t *wasapi = (tdav_consumer_wasapi_t*)self;
    if (wasapi) {
        /* stop */
        tdav_consumer_wasapi_stop((tmedia_consumer_t*)self);
        /* deinit base */
        tdav_consumer_audio_deinit(TDAV_CONSUMER_AUDIO(wasapi));
        /* deinit self */
        if (wasapi->AudioRender) {
            delete wasapi->AudioRender;
            wasapi->AudioRender = nullptr;
        }
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_consumer_wasapi_def_s = {
    sizeof(tdav_consumer_wasapi_t),
    tdav_consumer_wasapi_ctor,
    tdav_consumer_wasapi_dtor,
    tdav_consumer_audio_cmp,
};
/* plugin definition*/
static const tmedia_consumer_plugin_def_t tdav_consumer_wasapi_plugin_def_s = {
    &tdav_consumer_wasapi_def_s,

    tmedia_audio,
    "Microsoft Windows Audio Session API (WASAPI) consumer",

    tdav_consumer_wasapi_set,
    tdav_consumer_wasapi_prepare,
    tdav_consumer_wasapi_start,
    tdav_consumer_wasapi_consume,
    tdav_consumer_wasapi_pause,
    tdav_consumer_wasapi_stop
};
const tmedia_consumer_plugin_def_t *tdav_consumer_wasapi_plugin_def_t = &tdav_consumer_wasapi_plugin_def_s;




#endif /* HAVE_WASAPI */
