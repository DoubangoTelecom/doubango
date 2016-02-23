/*
* Copyright (C) 2010-2015 Mamadou DIOP.
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
*
*/

/**@file tdav_producer_waveapi.c
 * @brief Audio Producer for Win32 and WinCE platforms.
 */
#include "tinydav/audio/waveapi/tdav_producer_waveapi.h"

#if HAVE_WAVE_API

#include "tsk_thread.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#define TDAV_WAVEAPI_PRODUCER_ERROR_BUFF_COUNT	0xFF

#define tdav_producer_waveapi_set	tsk_null

static void print_last_error(MMRESULT mmrError, const char* func)
{
    static char buffer_err[TDAV_WAVEAPI_PRODUCER_ERROR_BUFF_COUNT];

    waveInGetErrorTextA(mmrError, buffer_err, sizeof(buffer_err));
    TSK_DEBUG_ERROR("%s() error: %s", func, buffer_err);
}

static int free_wavehdr(tdav_producer_waveapi_t* producer, tsk_size_t index)
{
    if(!producer || index >= sizeof(producer->hWaveHeaders)/sizeof(LPWAVEHDR)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    TSK_FREE(producer->hWaveHeaders[index]->lpData);
    TSK_FREE(producer->hWaveHeaders[index]);

    return 0;
}

static int create_wavehdr(tdav_producer_waveapi_t* producer, tsk_size_t index)
{
    if(!producer || index >= sizeof(producer->hWaveHeaders)/sizeof(LPWAVEHDR)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(producer->hWaveHeaders[index]) {
        free_wavehdr(producer, index);
    }

    producer->hWaveHeaders[index] = tsk_calloc(1, sizeof(WAVEHDR));
    producer->hWaveHeaders[index]->lpData = tsk_calloc(1, producer->bytes_per_notif);
    producer->hWaveHeaders[index]->dwBufferLength = (DWORD)producer->bytes_per_notif;
    producer->hWaveHeaders[index]->dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP;
    producer->hWaveHeaders[index]->dwLoops = 0x01;
    producer->hWaveHeaders[index]->dwUser = index;

    return 0;
}

static int add_wavehdr(tdav_producer_waveapi_t* producer, tsk_size_t index)
{
    MMRESULT result;

    if(!producer || !producer->hWaveHeaders[index] || !producer->hWaveIn) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    result = waveInPrepareHeader(producer->hWaveIn, producer->hWaveHeaders[index], sizeof(WAVEHDR));
    if(result != MMSYSERR_NOERROR) {
        print_last_error(result, "waveInPrepareHeader");
        return -2;
    }

    result = waveInAddBuffer(producer->hWaveIn, producer->hWaveHeaders[index], sizeof(WAVEHDR));
    if(result != MMSYSERR_NOERROR) {
        print_last_error(result, "waveInAddBuffer");
        return -3;
    }

    return 0;
}

static int record_wavehdr(tdav_producer_waveapi_t* producer, LPWAVEHDR lpHdr)
{
    MMRESULT result;

    if(!producer || !lpHdr || !producer->hWaveIn) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    //
    // Alert the session that there is new data to send over the network
    //
    if(TMEDIA_PRODUCER(producer)->enc_cb.callback) {
#if 0
        {
            static FILE* f = NULL;
            if(!f) {
                f = fopen("./waveapi_producer.raw", "w+");
            }
            fwrite(lpHdr->lpData, 1, lpHdr->dwBytesRecorded, f);
        }
#endif
        TMEDIA_PRODUCER(producer)->enc_cb.callback(TMEDIA_PRODUCER(producer)->enc_cb.callback_data, lpHdr->lpData, lpHdr->dwBytesRecorded);
    }

    if(!producer->started) {
        return 0;
    }

    result = waveInUnprepareHeader(producer->hWaveIn, lpHdr, sizeof(WAVEHDR));
    if(result != MMSYSERR_NOERROR) {
        print_last_error(result, "waveInUnprepareHeader");
        return -2;
    }

    result = waveInPrepareHeader(producer->hWaveIn, lpHdr, sizeof(WAVEHDR));
    if(result != MMSYSERR_NOERROR) {
        print_last_error(result, "waveInPrepareHeader");
        return -3;
    }

    result = waveInAddBuffer(producer->hWaveIn, lpHdr, sizeof(WAVEHDR));
    if(result != MMSYSERR_NOERROR) {
        print_last_error(result, "waveInAddBuffer");
        return -4;
    }

    return 0;
}

static void* TSK_STDCALL __record_thread(void *param)
{
    tdav_producer_waveapi_t* producer = (tdav_producer_waveapi_t*)param;
    DWORD dwEvent;
    tsk_size_t i;

    TSK_DEBUG_INFO("__record_thread -- START");

    // SetPriorityClass(GetCurrentThread(), REALTIME_PRIORITY_CLASS);

    for(;;) {
        dwEvent = WaitForMultipleObjects(2, producer->events, FALSE, INFINITE);

        if (dwEvent == 1) {
            break;
        }

        else if (dwEvent == 0) {
            EnterCriticalSection(&producer->cs);
            for(i = 0; i< sizeof(producer->hWaveHeaders)/sizeof(producer->hWaveHeaders[0]); i++) {
                if(producer->hWaveHeaders[i] && (producer->hWaveHeaders[i]->dwFlags & WHDR_DONE)) {
                    record_wavehdr(producer, producer->hWaveHeaders[i]);
                }
            }
            LeaveCriticalSection(&producer->cs);
        }
    }

    TSK_DEBUG_INFO("__record_thread() -- STOP");


    return tsk_null;
}








/* ============ Media Producer Interface ================= */
int tdav_producer_waveapi_prepare(tmedia_producer_t* self, const tmedia_codec_t* codec)
{
    tdav_producer_waveapi_t* producer = (tdav_producer_waveapi_t*)self;
    tsk_size_t i;

    if(!producer || !codec && codec->plugin) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    TMEDIA_PRODUCER(producer)->audio.channels = TMEDIA_CODEC_CHANNELS_AUDIO_ENCODING(codec);
    TMEDIA_PRODUCER(producer)->audio.rate = TMEDIA_CODEC_RATE_ENCODING(codec);
    TMEDIA_PRODUCER(producer)->audio.ptime = TMEDIA_CODEC_PTIME_AUDIO_ENCODING(codec);
    /* codec should have ptime */


    /* Format */
    ZeroMemory(&producer->wfx, sizeof(WAVEFORMATEX));
    producer->wfx.wFormatTag = WAVE_FORMAT_PCM;
    producer->wfx.nChannels = TMEDIA_PRODUCER(producer)->audio.channels;
    producer->wfx.nSamplesPerSec = TMEDIA_PRODUCER(producer)->audio.rate;
    producer->wfx.wBitsPerSample = TMEDIA_PRODUCER(producer)->audio.bits_per_sample;
    producer->wfx.nBlockAlign = (producer->wfx.nChannels * producer->wfx.wBitsPerSample/8);
    producer->wfx.nAvgBytesPerSec = (producer->wfx.nSamplesPerSec * producer->wfx.nBlockAlign);

    /* Average bytes (count) for each notification */
    producer->bytes_per_notif = ((producer->wfx.nAvgBytesPerSec * TMEDIA_PRODUCER(producer)->audio.ptime)/1000);

    /* create buffers */
    for(i = 0; i< sizeof(producer->hWaveHeaders)/sizeof(producer->hWaveHeaders[0]); i++) {
        create_wavehdr(producer, i);
    }

    return 0;
}

int tdav_producer_waveapi_start(tmedia_producer_t* self)
{
    tdav_producer_waveapi_t* producer = (tdav_producer_waveapi_t*)self;
    MMRESULT result;
    tsk_size_t i;

    if(!producer) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(producer->started || producer->hWaveIn) {
        TSK_DEBUG_WARN("Producer already started");
        return 0;
    }

    /* create events */
    if(!producer->events[0]) {
        producer->events[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
    }
    if(!producer->events[1]) {
        producer->events[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
    }

    /* open */
    result = waveInOpen((HWAVEIN *)&producer->hWaveIn, /*WAVE_MAPPER*/0, &producer->wfx, (DWORD)producer->events[0], (DWORD_PTR)producer, CALLBACK_EVENT);
    if(result != MMSYSERR_NOERROR) {
        print_last_error(result, "waveInOpen");
        return -2;
    }

    /* start */
    result = waveInStart(producer->hWaveIn);
    if(result != MMSYSERR_NOERROR) {
        print_last_error(result, "waveInStart");
        return -2;
    }

    /* write */
    for(i = 0; i< sizeof(producer->hWaveHeaders)/sizeof(LPWAVEHDR); i++) {
        add_wavehdr(producer, i);
    }

    /* start thread */
    producer->started = tsk_true;
    tsk_thread_create(&producer->tid[0], __record_thread, producer);

    return 0;
}

int tdav_producer_waveapi_pause(tmedia_producer_t* self)
{
    tdav_producer_waveapi_t* producer = (tdav_producer_waveapi_t*)self;

    if(!producer) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    return 0;
}

int tdav_producer_waveapi_stop(tmedia_producer_t* self)
{
    tdav_producer_waveapi_t* producer = (tdav_producer_waveapi_t*)self;
    MMRESULT result;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(!producer->started) {
        TSK_DEBUG_WARN("Producer not started");
        return 0;
    }

    /* stop thread */
    if(producer->tid[0]) {
        SetEvent(producer->events[1]);
        tsk_thread_join(&(producer->tid[0]));
    }

    /* should be done here */
    producer->started = tsk_false;

    if(producer->hWaveIn && (((result = waveInReset(producer->hWaveIn)) != MMSYSERR_NOERROR) || ((result = waveInClose(producer->hWaveIn)) != MMSYSERR_NOERROR))) {
        print_last_error(result, "waveInReset/waveInClose");
    }

    return 0;
}


//
//	WaveAPI producer object definition
//
/* constructor */
static tsk_object_t* tdav_producer_waveapi_ctor(tsk_object_t * self, va_list * app)
{
    tdav_producer_waveapi_t *producer = self;
    if(producer) {
        /* init base */
        tdav_producer_audio_init(TDAV_PRODUCER_AUDIO(producer));
        /* init self */
        InitializeCriticalSection(&producer->cs);
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_producer_waveapi_dtor(tsk_object_t * self)
{
    tdav_producer_waveapi_t *producer = self;
    if(producer) {
        tsk_size_t i;

        /* stop */
        if(producer->started) {
            tdav_producer_waveapi_stop(self);
        }

        /* deinit base */
        tdav_producer_audio_deinit(TDAV_PRODUCER_AUDIO(producer));
        /* deinit self */
        for(i = 0; i< sizeof(producer->hWaveHeaders)/sizeof(LPWAVEHDR); i++) {
            free_wavehdr(producer, i);
        }
        if(producer->hWaveIn) {
            waveInClose(producer->hWaveIn);
        }
        if(producer->events[0]) {
            CloseHandle(producer->events[0]);
        }
        if(producer->events[1]) {
            CloseHandle(producer->events[1]);
        }
        DeleteCriticalSection(&producer->cs);
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_producer_waveapi_def_s = {
    sizeof(tdav_producer_waveapi_t),
    tdav_producer_waveapi_ctor,
    tdav_producer_waveapi_dtor,
    tdav_producer_audio_cmp,
};
/* plugin definition*/
static const tmedia_producer_plugin_def_t tdav_producer_waveapi_plugin_def_s = {
    &tdav_producer_waveapi_def_s,

    tmedia_audio,
    "Microsoft WaveAPI producer",

    tdav_producer_waveapi_set,
    tdav_producer_waveapi_prepare,
    tdav_producer_waveapi_start,
    tdav_producer_waveapi_pause,
    tdav_producer_waveapi_stop
};
const tmedia_producer_plugin_def_t *tdav_producer_waveapi_plugin_def_t = &tdav_producer_waveapi_plugin_def_s;

#endif /* HAVE_WAVE_API */