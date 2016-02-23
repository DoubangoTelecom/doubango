/*
* Copyright (C) 2010-2015 Mamadou DIOP
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

/**@file tdav_consumer_waveapi.c
 * @brief Audio Consumer for Win32 and WinCE platforms.
 *
 */
#include "tinydav/audio/waveapi/tdav_consumer_waveapi.h"

#if HAVE_WAVE_API

#include "tsk_thread.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#define TDAV_WAVEAPI_CONSUMER_ERROR_BUFF_COUNT	0xFF

#define tdav_consumer_waveapi_set	tsk_null

static void print_last_error(MMRESULT mmrError, const char* func)
{
    static char buffer_err[TDAV_WAVEAPI_CONSUMER_ERROR_BUFF_COUNT];

    waveOutGetErrorTextA(mmrError, buffer_err, sizeof(buffer_err));
    TSK_DEBUG_ERROR("%s() error: %s", func, buffer_err);
}

static int free_wavehdr(tdav_consumer_waveapi_t* consumer, tsk_size_t index)
{
    if(!consumer || index >= sizeof(consumer->hWaveHeaders)/sizeof(LPWAVEHDR)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    TSK_FREE(consumer->hWaveHeaders[index]->lpData);
    TSK_FREE(consumer->hWaveHeaders[index]);

    return 0;
}

static int create_wavehdr(tdav_consumer_waveapi_t* consumer, tsk_size_t index)
{
    if(!consumer || index >= sizeof(consumer->hWaveHeaders)/sizeof(LPWAVEHDR)) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(consumer->hWaveHeaders[index]) {
        free_wavehdr(consumer, index);
    }

    consumer->hWaveHeaders[index] = tsk_calloc(1, sizeof(WAVEHDR));
    consumer->hWaveHeaders[index]->lpData = tsk_calloc(1, consumer->bytes_per_notif);
    consumer->hWaveHeaders[index]->dwBufferLength = (DWORD)consumer->bytes_per_notif;
    consumer->hWaveHeaders[index]->dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP;
    consumer->hWaveHeaders[index]->dwLoops = 0x01;
    consumer->hWaveHeaders[index]->dwUser = index;

    return 0;
}

static int write_wavehdr(tdav_consumer_waveapi_t* consumer, tsk_size_t index)
{
    MMRESULT result;

    if(!consumer || !consumer->hWaveHeaders[index] || !consumer->hWaveOut) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    result = waveOutPrepareHeader(consumer->hWaveOut, consumer->hWaveHeaders[index], sizeof(WAVEHDR));
    if(result != MMSYSERR_NOERROR) {
        print_last_error(result, "waveOutPrepareHeader");
        return -2;
    }

    result = waveOutWrite(consumer->hWaveOut, consumer->hWaveHeaders[index], sizeof(WAVEHDR));
    if(result != MMSYSERR_NOERROR) {
        print_last_error(result, "waveOutWrite");
        return -3;
    }

    return 0;
}

static int play_wavehdr(tdav_consumer_waveapi_t* consumer, LPWAVEHDR lpHdr)
{
    MMRESULT result;
    tsk_size_t out_size;

    if(!consumer || !lpHdr || !consumer->hWaveOut) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    result = waveOutUnprepareHeader(consumer->hWaveOut, lpHdr, sizeof(WAVEHDR));
    if(result != MMSYSERR_NOERROR) {
        print_last_error(result, "waveOutUnprepareHeader");
        return -2;
    }

    //
    //
    //	Fill lpHdr->Data with decoded data
    //
    //
    if((out_size = tdav_consumer_audio_get(TDAV_CONSUMER_AUDIO(consumer), lpHdr->lpData, lpHdr->dwBufferLength))) {
        //memcpy(lpHdr->lpData, data, lpHdr->dwBufferLength);
        //TSK_FREE(data);
    }
    else {
        /* Put silence */
        memset(lpHdr->lpData, 0, lpHdr->dwBufferLength);
    }

    if(!consumer->started) {
        return 0;
    }

    result = waveOutPrepareHeader(consumer->hWaveOut, lpHdr, sizeof(WAVEHDR));
    if(result != MMSYSERR_NOERROR) {
        print_last_error(result, "waveOutPrepareHeader");
        return -3;
    }

    result = waveOutWrite(consumer->hWaveOut, lpHdr, sizeof(WAVEHDR));
    if(result != MMSYSERR_NOERROR) {
        print_last_error(result, "waveOutWrite");
        return -4;
    }

    return 0;
}

static void* TSK_STDCALL __playback_thread(void *param)
{
    tdav_consumer_waveapi_t* consumer = (tdav_consumer_waveapi_t*)param;
    DWORD dwEvent;
    tsk_size_t i;

    TSK_DEBUG_INFO("__playback_thread -- START");

    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

    for(;;) {
        dwEvent = WaitForMultipleObjects(2, consumer->events, FALSE, INFINITE);

        if (dwEvent == 1) {
            break;
        }

        else if (dwEvent == 0) {
            EnterCriticalSection(&consumer->cs);
            for(i = 0; i< sizeof(consumer->hWaveHeaders)/sizeof(LPWAVEHDR); i++) {
                if(consumer->hWaveHeaders[i] && (consumer->hWaveHeaders[i]->dwFlags & WHDR_DONE)) {
                    play_wavehdr(consumer, consumer->hWaveHeaders[i]);
                }
            }
            LeaveCriticalSection(&consumer->cs);
        }
    }

    TSK_DEBUG_INFO("__playback_thread -- STOP");


    return tsk_null;
}








/* ============ Media Consumer Interface ================= */
int tdav_consumer_waveapi_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
    tdav_consumer_waveapi_t* consumer = (tdav_consumer_waveapi_t*)self;
    tsk_size_t i;

    if(!consumer || !codec && codec->plugin) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    TMEDIA_CONSUMER(consumer)->audio.ptime = TMEDIA_CODEC_PTIME_AUDIO_DECODING(codec);
    TMEDIA_CONSUMER(consumer)->audio.in.channels = TMEDIA_CODEC_CHANNELS_AUDIO_DECODING(codec);
    TMEDIA_CONSUMER(consumer)->audio.in.rate = TMEDIA_CODEC_RATE_DECODING(codec);

    /* codec should have ptime */


    /* Format */
    ZeroMemory(&consumer->wfx, sizeof(WAVEFORMATEX));
    consumer->wfx.wFormatTag = WAVE_FORMAT_PCM;
    consumer->wfx.nChannels = TMEDIA_CONSUMER(consumer)->audio.in.channels;
    consumer->wfx.nSamplesPerSec = TMEDIA_CONSUMER(consumer)->audio.out.rate ? TMEDIA_CONSUMER(consumer)->audio.out.rate : TMEDIA_CONSUMER(consumer)->audio.in.rate;
    consumer->wfx.wBitsPerSample = TMEDIA_CONSUMER(consumer)->audio.bits_per_sample;
    consumer->wfx.nBlockAlign = (consumer->wfx.nChannels * consumer->wfx.wBitsPerSample/8);
    consumer->wfx.nAvgBytesPerSec = (consumer->wfx.nSamplesPerSec * consumer->wfx.nBlockAlign);

    /* Average bytes (count) for each notification */
    consumer->bytes_per_notif = ((consumer->wfx.nAvgBytesPerSec * TMEDIA_CONSUMER(consumer)->audio.ptime)/1000);

    /* create buffers */
    for(i = 0; i< sizeof(consumer->hWaveHeaders)/sizeof(consumer->hWaveHeaders[0]); i++) {
        create_wavehdr(consumer, i);
    }

    return 0;
}

int tdav_consumer_waveapi_start(tmedia_consumer_t* self)
{
    tdav_consumer_waveapi_t* consumer = (tdav_consumer_waveapi_t*)self;
    MMRESULT result;
    tsk_size_t i;

    if(!consumer) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(consumer->started || consumer->hWaveOut) {
        TSK_DEBUG_WARN("Consumer already started");
        return 0;
    }

    /* create events */
    if(!consumer->events[0]) {
        consumer->events[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
    }
    if(!consumer->events[1]) {
        consumer->events[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
    }

    /* open */
    result = waveOutOpen((HWAVEOUT *)&consumer->hWaveOut, WAVE_MAPPER, &consumer->wfx, (DWORD)consumer->events[0], (DWORD_PTR)consumer, CALLBACK_EVENT);
    if(result != MMSYSERR_NOERROR) {
        print_last_error(result, "waveOutOpen");
        return -2;
    }

    /* write */
    for(i = 0; i< sizeof(consumer->hWaveHeaders)/sizeof(consumer->hWaveHeaders[0]); i++) {
        write_wavehdr(consumer, i);
    }

    /* start thread */
    consumer->started = tsk_true;
    tsk_thread_create(&consumer->tid[0], __playback_thread, consumer);

    return 0;
}

int tdav_consumer_waveapi_consume(tmedia_consumer_t* self, const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
    tdav_consumer_waveapi_t* consumer = (tdav_consumer_waveapi_t*)self;

    if(!consumer || !buffer || !size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    /* buffer is already decoded */
    return tdav_consumer_audio_put(TDAV_CONSUMER_AUDIO(consumer), buffer, size, proto_hdr);
}

int tdav_consumer_waveapi_pause(tmedia_consumer_t* self)
{
    tdav_consumer_waveapi_t* consumer = (tdav_consumer_waveapi_t*)self;

    if(!consumer) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    return 0;
}

int tdav_consumer_waveapi_stop(tmedia_consumer_t* self)
{
    tdav_consumer_waveapi_t* consumer = (tdav_consumer_waveapi_t*)self;
    MMRESULT result;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(!consumer->started) {
        TSK_DEBUG_WARN("Consumer not started");
        return 0;
    }

    /* stop thread */
    if(consumer->tid[0]) {
        SetEvent(consumer->events[1]);
        tsk_thread_join(&(consumer->tid[0]));
    }

    /* should be done here */
    consumer->started = tsk_false;

    if(consumer->hWaveOut && ((result = waveOutReset(consumer->hWaveOut)) != MMSYSERR_NOERROR)) {
        print_last_error(result, "waveOutReset");
    }

    return 0;
}


//
//	WaveAPI consumer object definition
//
/* constructor */
static tsk_object_t* tdav_consumer_waveapi_ctor(tsk_object_t * self, va_list * app)
{
    tdav_consumer_waveapi_t *consumer = self;
    if(consumer) {
        /* init base */
        tdav_consumer_audio_init(TDAV_CONSUMER_AUDIO(consumer));
        /* init self */
        InitializeCriticalSection(&consumer->cs);
    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_consumer_waveapi_dtor(tsk_object_t * self)
{
    tdav_consumer_waveapi_t *consumer = self;
    if(consumer) {
        tsk_size_t i;

        /* stop */
        if(consumer->started) {
            tdav_consumer_waveapi_stop(self);
        }

        /* deinit base */
        tdav_consumer_audio_deinit(TDAV_CONSUMER_AUDIO(consumer));
        /* deinit self */
        for(i = 0; i< sizeof(consumer->hWaveHeaders)/sizeof(LPWAVEHDR); i++) {
            free_wavehdr(consumer, i);
        }
        if(consumer->hWaveOut) {
            waveOutClose(consumer->hWaveOut);
        }
        if(consumer->events[0]) {
            CloseHandle(consumer->events[0]);
        }
        if(consumer->events[1]) {
            CloseHandle(consumer->events[1]);
        }
        DeleteCriticalSection(&consumer->cs);
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_consumer_waveapi_def_s = {
    sizeof(tdav_consumer_waveapi_t),
    tdav_consumer_waveapi_ctor,
    tdav_consumer_waveapi_dtor,
    tdav_consumer_audio_cmp,
};
/* plugin definition*/
static const tmedia_consumer_plugin_def_t tdav_consumer_waveapi_plugin_def_s = {
    &tdav_consumer_waveapi_def_s,

    tmedia_audio,
    "Microsoft WaveAPI consumer",

    tdav_consumer_waveapi_set,
    tdav_consumer_waveapi_prepare,
    tdav_consumer_waveapi_start,
    tdav_consumer_waveapi_consume,
    tdav_consumer_waveapi_pause,
    tdav_consumer_waveapi_stop
};
const tmedia_consumer_plugin_def_t *tdav_consumer_waveapi_plugin_def_t = &tdav_consumer_waveapi_plugin_def_s;

#endif /* HAVE_WAVE_API */