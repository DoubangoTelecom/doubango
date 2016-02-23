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
/**@file tdav_producer_wasapi.cxx
 * @brief Microsoft Windows Audio Session API (WASAPI) producer.
 * http://msdn.microsoft.com/en-us/library/windows/desktop/dd316551(v=vs.85).aspx
 */
#include "tinydav/audio/wasapi/tdav_producer_wasapi.h"

#if HAVE_WASAPI

#include "tinydav/audio/tdav_producer_audio.h"

#include "tsk_memory.h"
#include "tsk_string.h"
#include "tsk_debug.h"

#include <windows.h>
#include <audioclient.h>
#include <phoneaudioclient.h>

#include <speex/speex_buffer.h>

#if !defined(TDAV_WASAPI_PRODUCER_NOTIF_POS_COUNT)
#	define TDAV_WASAPI_PRODUCER_NOTIF_POS_COUNT		10
#endif
#define WASAPI_MILLIS_TO_100NS(MILLIS) (((LONGLONG)(MILLIS)) * 10000ui64)
#define WASAPI_100NS_TO_MILLIS(NANOS) (((LONGLONG)(NANOS)) / 10000ui64)

#define WASAPI_DEBUG_INFO(FMT, ...) TSK_DEBUG_INFO("[WASAPI Producer] " FMT, ##__VA_ARGS__)
#define WASAPI_DEBUG_WARN(FMT, ...) TSK_DEBUG_WARN("[WASAPI Producer] " FMT, ##__VA_ARGS__)
#define WASAPI_DEBUG_ERROR(FMT, ...) TSK_DEBUG_ERROR("[WASAPI Producer] " FMT, ##__VA_ARGS__)
#define WASAPI_DEBUG_FATAL(FMT, ...) TSK_DEBUG_FATAL("[WASAPI Producer] " FMT, ##__VA_ARGS__)

struct tdav_producer_wasapi_s;

namespace Doubango
{
namespace VoIP
{
ref class AudioCapture sealed
{
public:
    virtual ~AudioCapture();
internal:
    AudioCapture();

    int Prepare(struct tdav_producer_wasapi_s* wasapi, const tmedia_codec_t* codec);
    int UnPrepare();
    int Start();
    int Stop();
    int Pause();

private:
    void AsyncThread(Windows::Foundation::IAsyncAction^ operation);

private:
    tsk_mutex_handle_t* m_hMutex;
    IAudioClient2* m_pDevice;
    IAudioCaptureClient* m_pClient;
    HANDLE m_hCaptureEvent;
    HANDLE m_hShutdownEvent;
    Windows::Foundation::IAsyncAction^ m_pAsyncThread;
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
}
}

typedef struct tdav_producer_wasapi_s {
    TDAV_DECLARE_PRODUCER_AUDIO;

    Doubango::VoIP::AudioCapture ^audioCapture;
}
tdav_producer_wasapi_t;

extern "C" void tdav_win32_print_error(const char* func, HRESULT hr);


/* ============ Media Producer Interface ================= */
static int tdav_producer_wasapi_set(tmedia_producer_t* self, const tmedia_param_t* param)
{
    tdav_producer_wasapi_t* wasapi = (tdav_producer_wasapi_t*)self;
    if (param->plugin_type == tmedia_ppt_producer) {
        if (param->value_type == tmedia_pvt_int32) {
            if (tsk_striequals(param->key, "volume")) {
                return 0;
            }
            else if (tsk_striequals(param->key, "mute")) {
                //wasapi->mute = (TSK_TO_INT32((uint8_t*)param->value) != 0);
#if !FIXME_SEND_SILENCE_ON_MUTE
                //if(wasapi->started){
                //	if(wasapi->mute){
                //IDirectSoundCaptureBuffer_Stop(wasapi->captureBuffer);
                //	}
                //	else{
                //IDirectSoundCaptureBuffer_Start(wasapi->captureBuffer, DSBPLAY_LOOPING);
                //	}
                //}
#endif
                return 0;
            }
        }
    }
    return tdav_producer_audio_set(TDAV_PRODUCER_AUDIO(self), param);
}



static int tdav_producer_wasapi_prepare(tmedia_producer_t* self, const tmedia_codec_t* codec)
{
    tdav_producer_wasapi_t* wasapi = (tdav_producer_wasapi_t*)self;

    if(!wasapi || !codec || !wasapi->audioCapture) {
        WASAPI_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    /* codec should have ptime */
    TMEDIA_PRODUCER(wasapi)->audio.channels = TMEDIA_CODEC_CHANNELS_AUDIO_ENCODING(codec);
    TMEDIA_PRODUCER(wasapi)->audio.rate = TMEDIA_CODEC_RATE_ENCODING(codec);
    TMEDIA_PRODUCER(wasapi)->audio.ptime = TMEDIA_CODEC_PTIME_AUDIO_ENCODING(codec);

    WASAPI_DEBUG_INFO("channels=%d, rate=%d, ptime=%d",
                      TMEDIA_PRODUCER(wasapi)->audio.channels,
                      TMEDIA_PRODUCER(wasapi)->audio.rate,
                      TMEDIA_PRODUCER(wasapi)->audio.ptime);

    return wasapi->audioCapture->Prepare(wasapi, codec);
}

static int tdav_producer_wasapi_start(tmedia_producer_t* self)
{
    tdav_producer_wasapi_t* wasapi = (tdav_producer_wasapi_t*)self;

    WASAPI_DEBUG_INFO("tdav_producer_wasapi_start()");

    if(!wasapi || !wasapi->audioCapture) {
        WASAPI_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    return wasapi->audioCapture->Start();
}

static int tdav_producer_wasapi_pause(tmedia_producer_t* self)
{
    tdav_producer_wasapi_t* wasapi = (tdav_producer_wasapi_t*)self;

    if(!wasapi || !wasapi->audioCapture) {
        WASAPI_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    return wasapi->audioCapture->Pause();
}

static int tdav_producer_wasapi_stop(tmedia_producer_t* self)
{
    tdav_producer_wasapi_t* wasapi = (tdav_producer_wasapi_t*)self;

    WASAPI_DEBUG_INFO("tdav_producer_wasapi_stop()");

    if(!wasapi || !wasapi->audioCapture) {
        WASAPI_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    return wasapi->audioCapture->Stop();
}







Doubango::VoIP::AudioCapture::AudioCapture()
    : m_pDevice(nullptr)
    , m_hMutex(nullptr)
    , m_pClient(nullptr)
    , m_hCaptureEvent(nullptr)
    , m_hShutdownEvent(nullptr)
    , m_pAsyncThread(nullptr)
    , m_nBytesPerNotif(0)
    , m_nSourceFrameSizeInBytes(0)
    , m_bStarted(false)
    , m_bPrepared(false)
    , m_bPaused(false)
{
    m_callback.fn = nullptr, m_callback.pcData = nullptr;
    memset(&m_ring, 0, sizeof(m_ring));

    if(!(m_hMutex = tsk_mutex_create())) {
        throw ref new Platform::FailureException(L"Failed to create mutex");
    }
}

Doubango::VoIP::AudioCapture::~AudioCapture()
{
    Stop();
    UnPrepare();

    tsk_mutex_destroy(&m_hMutex);
}

int Doubango::VoIP::AudioCapture::Prepare(tdav_producer_wasapi_t* wasapi, const tmedia_codec_t* codec)
{
    HRESULT hr = E_FAIL;
    int ret = 0;
    WAVEFORMATEX wfx = {0};
    AudioClientProperties properties = {0};
    LPCWSTR pwstrCaptureId = nullptr;

#define WASAPI_SET_ERROR(code) ret = (code); goto bail;

    tsk_mutex_lock(m_hMutex);

    if(m_bPrepared) {
        WASAPI_DEBUG_INFO("#WASAPI: Audio producer already prepared");
        goto bail;
    }

    if(!wasapi || !codec) {
        WASAPI_DEBUG_ERROR("Invalid parameter");
        WASAPI_SET_ERROR(-1);
    }

    if(m_pDevice || m_pClient) {
        WASAPI_DEBUG_ERROR("Producer already prepared");
        WASAPI_SET_ERROR(-2);
    }

    pwstrCaptureId = GetDefaultAudioCaptureId(AudioDeviceRole::Communications);

    if (NULL == pwstrCaptureId) {
        tdav_win32_print_error("GetDefaultAudioCaptureId", HRESULT_FROM_WIN32(GetLastError()));
        WASAPI_SET_ERROR(-3);
    }

    hr = ActivateAudioInterface(pwstrCaptureId, __uuidof(IAudioClient2), (void**)&m_pDevice);
    if(!SUCCEEDED(hr)) {
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
        wfx.nChannels = TMEDIA_PRODUCER(wasapi)->audio.channels;
        wfx.nSamplesPerSec = TMEDIA_PRODUCER(wasapi)->audio.rate;
        wfx.wBitsPerSample = TMEDIA_PRODUCER(wasapi)->audio.bits_per_sample;
        wfx.nBlockAlign = (wfx.nChannels * wfx.wBitsPerSample/8);
        wfx.nAvgBytesPerSec = (wfx.nSamplesPerSec * wfx.nBlockAlign);

        PWAVEFORMATEX pwfxClosestMatch = NULL;
        hr = m_pDevice->IsFormatSupported(AUDCLNT_SHAREMODE_SHARED, &wfx, &pwfxClosestMatch);
        if(hr != S_OK && hr != S_FALSE) {
            tdav_win32_print_error("IsFormatSupported", HRESULT_FROM_WIN32(GetLastError()));
            WASAPI_SET_ERROR(-8);
        }

        if(hr == S_FALSE) {
            if(!pwfxClosestMatch) {
                WASAPI_DEBUG_ERROR("malloc(%d) failed", sizeof(WAVEFORMATEX));
                WASAPI_SET_ERROR(-7);
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

            WASAPI_DEBUG_INFO("Audio device format fallback: rate=%d, bps=%d, channels=%d", wfx.nSamplesPerSec, wfx.wBitsPerSample, wfx.nChannels);
        }
        if(pwfxClosestMatch) {
            CoTaskMemFree(pwfxClosestMatch);
        }
    }

    m_nSourceFrameSizeInBytes = (wfx.wBitsPerSample >> 3) * wfx.nChannels;
    m_nBytesPerNotif = ((wfx.nAvgBytesPerSec * TMEDIA_PRODUCER(wasapi)->audio.ptime)/1000);

    // Initialize
    hr = m_pDevice->Initialize(
             AUDCLNT_SHAREMODE_SHARED,
             AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
             (TDAV_WASAPI_PRODUCER_NOTIF_POS_COUNT * WASAPI_MILLIS_TO_100NS(TMEDIA_PRODUCER(wasapi)->audio.ptime)),
             0,
             &wfx,
             NULL);
    if (!SUCCEEDED(hr)) {
        tdav_win32_print_error("#WASAPI: Capture::SetClientProperties", hr);
        WASAPI_SET_ERROR(-9);
    }

    REFERENCE_TIME DefaultDevicePeriod, MinimumDevicePeriod;
    hr = m_pDevice->GetDevicePeriod(&DefaultDevicePeriod, &MinimumDevicePeriod);
    if (!SUCCEEDED(hr)) {
        tdav_win32_print_error("GetDevicePeriod", hr);
        WASAPI_SET_ERROR(-10);
    }
    WASAPI_DEBUG_INFO("#WASAPI(Capture): DefaultDevicePeriod=%lld ms, MinimumDevicePeriod=%lldms", WASAPI_100NS_TO_MILLIS(DefaultDevicePeriod), WASAPI_100NS_TO_MILLIS(MinimumDevicePeriod));

    if(!m_hCaptureEvent) {
        if(!(m_hCaptureEvent = CreateEventEx(NULL, NULL, 0, EVENT_ALL_ACCESS))) {
            tdav_win32_print_error("CreateEventEx(Capture)", HRESULT_FROM_WIN32(GetLastError()));
            WASAPI_SET_ERROR(-11);
        }
    }
    if(!m_hShutdownEvent) {
        if(!(m_hShutdownEvent = CreateEventEx(NULL, NULL, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS))) {
            tdav_win32_print_error("CreateEventEx(Shutdown)", HRESULT_FROM_WIN32(GetLastError()));
            WASAPI_SET_ERROR(-12);
        }
    }

    hr = m_pDevice->SetEventHandle(m_hCaptureEvent);
    if (!SUCCEEDED(hr)) {
        tdav_win32_print_error("SetEventHandle", hr);
        WASAPI_SET_ERROR(-13);
    }

    hr = m_pDevice->GetService(__uuidof(IAudioCaptureClient), (void**)&m_pClient);
    if (!SUCCEEDED(hr)) {
        tdav_win32_print_error("GetService", hr);
        WASAPI_SET_ERROR(-14);
    }

    int packetperbuffer = (1000 / TMEDIA_PRODUCER(wasapi)->audio.ptime);
    m_ring.chunck.size = wfx.nSamplesPerSec * (wfx.wBitsPerSample >> 3) / packetperbuffer;
    WASAPI_DEBUG_INFO("#WASAPI: Audio producer ring chunk size = %u", m_ring.chunck.size);
    // allocate our chunck buffer
    if(!(m_ring.chunck.buffer = tsk_realloc(m_ring.chunck.buffer, m_ring.chunck.size))) {
        WASAPI_DEBUG_ERROR("Failed to allocate new buffer");
        WASAPI_SET_ERROR(-15);
    }
    // create ringbuffer
    m_ring.size = TDAV_WASAPI_PRODUCER_NOTIF_POS_COUNT * m_ring.chunck.size;
    WASAPI_DEBUG_INFO("#WASAPI: Audio producer ring size = %u", m_ring.size);
    if(!m_ring.buffer) {
        m_ring.buffer = speex_buffer_init(m_ring.size);
    }
    else {
        int sret;
        if((sret = speex_buffer_resize(m_ring.buffer, m_ring.size)) < 0) {
            WASAPI_DEBUG_ERROR("speex_buffer_resize(%d) failed with error code=%d", m_ring.size, sret);
            WASAPI_SET_ERROR(-16);
        }
    }
    if(!m_ring.buffer) {
        WASAPI_DEBUG_ERROR("Failed to create a new ring buffer with size = %d", m_ring.size);
        WASAPI_SET_ERROR(-17);
    }

    m_callback.fn = TMEDIA_PRODUCER(wasapi)->enc_cb.callback;
    m_callback.pcData = TMEDIA_PRODUCER(wasapi)->enc_cb.callback_data;

bail:
    if (pwstrCaptureId) {
        CoTaskMemFree((LPVOID)pwstrCaptureId);
    }
    if(ret != 0) {
        UnPrepare();
    }
    m_bPrepared = (ret == 0);

    tsk_mutex_unlock(m_hMutex);

    return ret;
}

int Doubango::VoIP::AudioCapture::UnPrepare()
{
    tsk_mutex_lock(m_hMutex);

    if(m_hCaptureEvent) {
        CloseHandle(m_hCaptureEvent), m_hCaptureEvent = nullptr;
    }
    if(m_hShutdownEvent) {
        CloseHandle(m_hShutdownEvent), m_hShutdownEvent = nullptr;
    }
    if(m_pDevice) {
        m_pDevice->Release(), m_pDevice = nullptr;
    }
    if(m_pClient) {
        m_pClient->Release(), m_pClient = nullptr;
    }

    TSK_FREE(m_ring.chunck.buffer);
    if(m_ring.buffer) {
        speex_buffer_destroy(m_ring.buffer);
        m_ring.buffer = nullptr;
    }

    m_callback.fn = nullptr;
    m_callback.pcData = nullptr;

    m_bPrepared = false;

    tsk_mutex_unlock(m_hMutex);

    return 0;
}

int Doubango::VoIP::AudioCapture::Start()
{
    tsk_mutex_lock(m_hMutex);

    if(m_bStarted) {
        WASAPI_DEBUG_INFO("#WASAPI: Audio producer already started");
        goto bail;
    }
    if(!m_bPrepared) {
        WASAPI_DEBUG_ERROR("Audio producer not prepared");
        goto bail;
    }

    m_pAsyncThread = Windows::System::Threading::ThreadPool::RunAsync(ref new Windows::System::Threading::WorkItemHandler(this, &Doubango::VoIP::AudioCapture::AsyncThread),
                     Windows::System::Threading::WorkItemPriority::High,
                     Windows::System::Threading::WorkItemOptions::TimeSliced);

    if((m_bStarted = (m_pAsyncThread != nullptr))) {
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

int Doubango::VoIP::AudioCapture::Stop()
{
    m_bStarted = false;

    tsk_mutex_lock(m_hMutex);

    if (m_hShutdownEvent) {
        SetEvent(m_hShutdownEvent);
    }

    if (m_pAsyncThread) {
        m_pAsyncThread->Cancel();
        m_pAsyncThread->Close();
        m_pAsyncThread = nullptr;
    }

    if(m_pDevice) {
        m_pDevice->Stop();
    }

    // will be prepared again before next start()
    UnPrepare();

    tsk_mutex_unlock(m_hMutex);

    return 0;
}

int Doubango::VoIP::AudioCapture::Pause()
{
    tsk_mutex_lock(m_hMutex);

    m_bPaused = true;

    tsk_mutex_unlock(m_hMutex);

    return 0;
}

void Doubango::VoIP::AudioCapture::AsyncThread(Windows::Foundation::IAsyncAction^ operation)
{
    HRESULT hr = S_OK;
    BYTE* pbData = nullptr;
    UINT32 nFrames = 0;
    DWORD dwFlags = 0;
    UINT32 incomingBufferSize;
    INT32 avail;
    UINT32 nNextPacketSize;

    HANDLE eventHandles[] = {
        m_hCaptureEvent,        // WAIT_OBJECT0
        m_hShutdownEvent		// WAIT_OBJECT1
    };

    WASAPI_DEBUG_INFO("#WASAPI: __record_thread -- START");

#define BREAK_WHILE tsk_mutex_unlock(m_hMutex); break;

    while(m_bStarted && SUCCEEDED(hr)) {
        DWORD waitResult = WaitForMultipleObjectsEx(SIZEOF_ARRAY(eventHandles), eventHandles, FALSE, INFINITE, FALSE);

        tsk_mutex_lock(m_hMutex);

        if(!m_bStarted) {
            BREAK_WHILE;
        }

        if(waitResult == WAIT_OBJECT_0 && m_callback.fn) {
            hr = m_pClient->GetNextPacketSize(&nNextPacketSize);
            while(SUCCEEDED(hr) && nNextPacketSize >0) {
                hr = m_pClient->GetBuffer(&pbData, &nFrames, &dwFlags, nullptr, nullptr);
                if(SUCCEEDED(hr) && pbData && nFrames) {
                    if((dwFlags & AUDCLNT_BUFFERFLAGS_SILENT) != AUDCLNT_BUFFERFLAGS_SILENT) {
                        incomingBufferSize = nFrames * m_nSourceFrameSizeInBytes;
                        speex_buffer_write(m_ring.buffer, pbData, incomingBufferSize);
                        avail = speex_buffer_get_available(m_ring.buffer);
                        while (m_bStarted && avail >= (INT32)m_ring.chunck.size) {
                            avail -= speex_buffer_read(m_ring.buffer, m_ring.chunck.buffer, m_ring.chunck.size);
                            m_callback.fn(m_callback.pcData, m_ring.chunck.buffer, m_ring.chunck.size);
                        }
                    }

                    if (SUCCEEDED(hr)) {
                        hr = m_pClient->ReleaseBuffer(nFrames);
                    }
                    if (SUCCEEDED(hr)) {
                        hr = m_pClient->GetNextPacketSize(&nNextPacketSize);
                    }
                }
            }
        }
        else if(waitResult != WAIT_OBJECT_0) {
            BREAK_WHILE;
        }

        tsk_mutex_unlock(m_hMutex);
    }// end-of-while

    if (!SUCCEEDED(hr)) {
        tdav_win32_print_error("AsyncThread: ", hr);
    }


    WASAPI_DEBUG_INFO("WASAPI: __record_thread(%s) -- STOP", SUCCEEDED(hr) ? "OK": "NOK");
}







//
//	WaveAPI producer object definition
//
/* constructor */
static tsk_object_t* tdav_producer_wasapi_ctor(tsk_object_t * self, va_list * app)
{
    tdav_producer_wasapi_t *wasapi = (tdav_producer_wasapi_t*)self;
    if(wasapi) {
        /* init base */
        tdav_producer_audio_init(TDAV_PRODUCER_AUDIO(wasapi));
        /* init self */

        wasapi->audioCapture = ref new Doubango::VoIP::AudioCapture();
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_producer_wasapi_dtor(tsk_object_t * self)
{
    tdav_producer_wasapi_t *wasapi = (tdav_producer_wasapi_t*)self;
    if(wasapi) {
        /* stop */
        tdav_producer_wasapi_stop((tmedia_producer_t*)self);
        /* deinit base */
        tdav_producer_audio_deinit(TDAV_PRODUCER_AUDIO(wasapi));
        /* deinit self */
        if(wasapi->audioCapture) {
            delete wasapi->audioCapture;
            wasapi->audioCapture = nullptr;
        }
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_producer_wasapi_def_s = {
    sizeof(tdav_producer_wasapi_t),
    tdav_producer_wasapi_ctor,
    tdav_producer_wasapi_dtor,
    tdav_producer_audio_cmp,
};
/* plugin definition*/
static const tmedia_producer_plugin_def_t tdav_producer_wasapi_plugin_def_s = {
    &tdav_producer_wasapi_def_s,

    tmedia_audio,
    "Microsoft Windows Audio Session API (WASAPI) producer",

    tdav_producer_wasapi_set,
    tdav_producer_wasapi_prepare,
    tdav_producer_wasapi_start,
    tdav_producer_wasapi_pause,
    tdav_producer_wasapi_stop
};
const tmedia_producer_plugin_def_t *tdav_producer_wasapi_plugin_def_t = &tdav_producer_wasapi_plugin_def_s;




#endif /* HAVE_WASAPI */
