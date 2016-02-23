/*
* Copyright (C) 2010-2011 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou(at)doubango.org>
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

/**@file tdav_consumer_dsound.c
 * @brief Microsoft DirectSound consumer.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 */
#include "tinydav/audio/directsound/tdav_consumer_dsound.h"

#if HAVE_DSOUND_H

#if defined(_MSC_VER)
#	pragma comment(lib, "dsound.lib")
// #	pragma comment(lib, "dxguid.lib")
#endif

#include "tsk_string.h"
#include "tsk_thread.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <initguid.h>
#include <dsound.h>

extern void tdav_win32_print_error(const char* func, HRESULT hr);

#if !defined(TDAV_DSOUND_CONSUMER_NOTIF_POS_COUNT)
#	define TDAV_DSOUND_CONSUMER_NOTIF_POS_COUNT		20
#endif /* TDAV_DSOUND_CONSUMER_NOTIF_POS_COUNT */

typedef struct tdav_consumer_dsound_s {
    TDAV_DECLARE_CONSUMER_AUDIO;

    tsk_bool_t started;
    tsk_size_t bytes_per_notif_size;
    uint8_t* bytes_per_notif_ptr;
    tsk_thread_handle_t* tid[1];

    LPDIRECTSOUND device;
    LPDIRECTSOUNDBUFFER primaryBuffer;
    LPDIRECTSOUNDBUFFER secondaryBuffer;
    HANDLE notifEvents[TDAV_DSOUND_CONSUMER_NOTIF_POS_COUNT];
}
tdav_consumer_dsound_t;

static _inline int32_t __convert_volume(int32_t volume)
{
    static const int32_t __step = (DSBVOLUME_MAX - DSBVOLUME_MIN) / 100;
    return (volume * __step) + DSBVOLUME_MIN;
}

static void* TSK_STDCALL _tdav_consumer_dsound_playback_thread(void *param)
{
    tdav_consumer_dsound_t* dsound = (tdav_consumer_dsound_t*)param;

    HRESULT hr;
    LPVOID lpvAudio1, lpvAudio2;
    DWORD dwBytesAudio1, dwBytesAudio2, dwEvent;
    static const DWORD dwWriteCursor = 0;
    tsk_size_t out_size;

    TSK_DEBUG_INFO("_tdav_consumer_dsound_playback_thread -- START");

    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

    while (dsound->started) {
        dwEvent = WaitForMultipleObjects(TDAV_DSOUND_CONSUMER_NOTIF_POS_COUNT, dsound->notifEvents, FALSE, INFINITE);
        if (!dsound->started) {
            break;
        }

        // lock
        hr = IDirectSoundBuffer_Lock(
                 dsound->secondaryBuffer,
                 dwWriteCursor/* Ignored because of DSBLOCK_FROMWRITECURSOR */,
                 (DWORD)dsound->bytes_per_notif_size,
                 &lpvAudio1, &dwBytesAudio1,
                 &lpvAudio2, &dwBytesAudio2,
                 DSBLOCK_FROMWRITECURSOR);
        if (hr != DS_OK) {
            tdav_win32_print_error("IDirectSoundBuffer_Lock", hr);
            goto next;
        }

        out_size = tdav_consumer_audio_get(TDAV_CONSUMER_AUDIO(dsound), dsound->bytes_per_notif_ptr, dsound->bytes_per_notif_size);
        if (out_size < dsound->bytes_per_notif_size) {
            // fill with silence
            memset(&dsound->bytes_per_notif_ptr[out_size], 0, (dsound->bytes_per_notif_size - out_size));
        }
        if ((dwBytesAudio1 + dwBytesAudio2) == dsound->bytes_per_notif_size) {
            memcpy(lpvAudio1, dsound->bytes_per_notif_ptr, dwBytesAudio1);
            if (lpvAudio2 && dwBytesAudio2) {
                memcpy(lpvAudio2, &dsound->bytes_per_notif_ptr[dwBytesAudio1], dwBytesAudio2);
            }
        }
        else {
            TSK_DEBUG_ERROR("Not expected: %d+%d#%d", dwBytesAudio1, dwBytesAudio2, dsound->bytes_per_notif_size);
        }
#if 0
        memset(lpvAudio1, rand(), dwBytesAudio1);
#endif
        // unlock
        if ((hr = IDirectSoundBuffer_Unlock(dsound->secondaryBuffer, lpvAudio1, dwBytesAudio1, lpvAudio2, dwBytesAudio2)) != DS_OK) {
            tdav_win32_print_error("IDirectSoundBuffer_UnLock", hr);
            goto next;
        }
next:
        tdav_consumer_audio_tick(TDAV_CONSUMER_AUDIO(dsound));
    }

    TSK_DEBUG_INFO("_tdav_consumer_dsound_playback_thread -- STOP");


    return tsk_null;
}

static int _tdav_consumer_dsound_unprepare(tdav_consumer_dsound_t *dsound)
{
    if(dsound) {
        tsk_size_t i;
        if(dsound->primaryBuffer) {
            IDirectSoundBuffer_Release(dsound->primaryBuffer);
            dsound->primaryBuffer = NULL;
        }
        if(dsound->secondaryBuffer) {
            IDirectSoundBuffer_Release(dsound->secondaryBuffer);
            dsound->secondaryBuffer = NULL;
        }
        if(dsound->device) {
            IDirectSound_Release(dsound->device);
            dsound->device = NULL;
        }
        for(i = 0; i<sizeof(dsound->notifEvents)/sizeof(dsound->notifEvents[0]); i++) {
            if(dsound->notifEvents[i]) {
                CloseHandle(dsound->notifEvents[i]);
                dsound->notifEvents[i] = NULL;
            }
        }
    }
    return 0;
}



/* ============ Media Consumer Interface ================= */
static int tdav_consumer_dsound_set(tmedia_consumer_t* self, const tmedia_param_t* param)
{
    tdav_consumer_dsound_t* dsound = (tdav_consumer_dsound_t*)self;
    int ret = tdav_consumer_audio_set(TDAV_CONSUMER_AUDIO(self), param);

    if(ret == 0) {
        if(dsound->secondaryBuffer && tsk_striequals(param->key, "volume")) {
            if(IDirectSoundBuffer_SetVolume(dsound->secondaryBuffer, __convert_volume(TMEDIA_CONSUMER(self)->audio.volume)) != DS_OK) {
                TSK_DEBUG_ERROR("IDirectSoundBuffer_SetVolume() failed");
                ret = -1;
            }
        }
    }

    return ret;
}

static int tdav_consumer_dsound_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
    HRESULT hr;
    HWND hWnd;

    WAVEFORMATEX wfx = {0};
    DSBUFFERDESC dsbd = {0};

    tdav_consumer_dsound_t* dsound = (tdav_consumer_dsound_t*)self;

    if(!dsound) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(dsound->device || dsound->primaryBuffer || dsound->secondaryBuffer) {
        TSK_DEBUG_ERROR("Consumer already prepared");
        return -2;
    }

    TMEDIA_CONSUMER(dsound)->audio.ptime = TMEDIA_CODEC_PTIME_AUDIO_DECODING(codec);
    TMEDIA_CONSUMER(dsound)->audio.in.channels = TMEDIA_CODEC_CHANNELS_AUDIO_DECODING(codec);
    TMEDIA_CONSUMER(dsound)->audio.in.rate = TMEDIA_CODEC_RATE_DECODING(codec);

#if 0
    TMEDIA_CONSUMER(dsound)->audio.out.rate = 48000;
    TMEDIA_CONSUMER(dsound)->audio.out.channels = 2;
#endif

    /* Create sound device */
    if((hr = DirectSoundCreate(NULL, &dsound->device, NULL) != DS_OK)) {
        tdav_win32_print_error("DirectSoundCreate", hr);
        return -3;
    }

    /* Set CooperativeLevel */
    if((hWnd = GetForegroundWindow()) || (hWnd = GetDesktopWindow()) || (hWnd = GetConsoleWindow())) {
        if((hr = IDirectSound_SetCooperativeLevel(dsound->device, hWnd, DSSCL_PRIORITY)) != DS_OK) {
            tdav_win32_print_error("IDirectSound_SetCooperativeLevel", hr);
            return -2;
        }
    }

    /* Creates the primary buffer and apply format */
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = TMEDIA_CONSUMER(dsound)->audio.out.channels ? TMEDIA_CONSUMER(dsound)->audio.out.channels : TMEDIA_CONSUMER(dsound)->audio.in.channels;
    wfx.nSamplesPerSec = TMEDIA_CONSUMER(dsound)->audio.out.rate ? TMEDIA_CONSUMER(dsound)->audio.out.rate : TMEDIA_CONSUMER(dsound)->audio.in.rate;
    wfx.wBitsPerSample = TMEDIA_CONSUMER(dsound)->audio.bits_per_sample;
    wfx.nBlockAlign = (wfx.nChannels * wfx.wBitsPerSample/8);
    wfx.nAvgBytesPerSec = (wfx.nSamplesPerSec * wfx.nBlockAlign);

    /* Average bytes (count) for each notification */
    dsound->bytes_per_notif_size = ((wfx.nAvgBytesPerSec * TMEDIA_CONSUMER(dsound)->audio.ptime)/1000);
    if(!(dsound->bytes_per_notif_ptr = tsk_realloc(dsound->bytes_per_notif_ptr, dsound->bytes_per_notif_size))) {
        TSK_DEBUG_ERROR("Failed to allocate buffer with size = %u", dsound->bytes_per_notif_size);
        return -3;
    }

    dsbd.dwSize = sizeof(DSBUFFERDESC);
    dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER;
    dsbd.dwBufferBytes = 0;
    dsbd.lpwfxFormat = NULL;

    if((hr = IDirectSound_CreateSoundBuffer(dsound->device, &dsbd, &dsound->primaryBuffer, NULL)) != DS_OK) {
        tdav_win32_print_error("IDirectSound_CreateSoundBuffer", hr);
        return -4;
    }
    if((hr = IDirectSoundBuffer_SetFormat(dsound->primaryBuffer, &wfx)) != DS_OK) {
        tdav_win32_print_error("IDirectSoundBuffer_SetFormat", hr);
        return -5;
    }

    /* Creates the secondary buffer and apply format */
    dsbd.dwFlags = (DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLVOLUME);
    dsbd.dwBufferBytes = (DWORD)(TDAV_DSOUND_CONSUMER_NOTIF_POS_COUNT * dsound->bytes_per_notif_size);
    dsbd.lpwfxFormat = &wfx;

    if((hr = IDirectSound_CreateSoundBuffer(dsound->device, &dsbd, &dsound->secondaryBuffer, NULL)) != DS_OK) {
        tdav_win32_print_error("IDirectSound_CreateSoundBuffer", hr);
        return -6;
    }

    /* Set Volume */
    if(IDirectSoundBuffer_SetVolume(dsound->secondaryBuffer, __convert_volume(TMEDIA_CONSUMER(self)->audio.volume)) != DS_OK) {
        TSK_DEBUG_ERROR("IDirectSoundBuffer_SetVolume() failed");
    }

    return 0;
}

static int tdav_consumer_dsound_start(tmedia_consumer_t* self)
{
    tdav_consumer_dsound_t* dsound = (tdav_consumer_dsound_t*)self;

    tsk_size_t i;
    HRESULT hr;
    LPDIRECTSOUNDNOTIFY lpDSBNotify;
    DSBPOSITIONNOTIFY pPosNotify[TDAV_DSOUND_CONSUMER_NOTIF_POS_COUNT] = {0};

    static DWORD dwMajorVersion = -1;

    // Get OS version
    if(dwMajorVersion == -1) {
        OSVERSIONINFO osvi;
        ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        GetVersionEx(&osvi);
        dwMajorVersion = osvi.dwMajorVersion;
    }

    if(!dsound) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(!dsound->device || !dsound->primaryBuffer || !dsound->secondaryBuffer) {
        TSK_DEBUG_ERROR("Consumer not prepared");
        return -2;
    }

    if(dsound->started) {
        return 0;
    }

    if((hr = IDirectSoundBuffer_QueryInterface(dsound->secondaryBuffer, &IID_IDirectSoundNotify, (LPVOID*)&lpDSBNotify)) != DS_OK) {
        tdav_win32_print_error("IDirectSoundBuffer_QueryInterface", hr);
        return -3;
    }

    /* Events associated to notification points */
    for(i = 0; i<TDAV_DSOUND_CONSUMER_NOTIF_POS_COUNT; i++) {
        dsound->notifEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
        // set notification point offset at the start of the buffer for Windows Vista and later and at the half of the buffer of XP and before
        pPosNotify[i].dwOffset = (DWORD)((dsound->bytes_per_notif_size * i) + (dwMajorVersion > 5 ? (dsound->bytes_per_notif_size >> 1) : 1));
        pPosNotify[i].hEventNotify = dsound->notifEvents[i];
    }
    if((hr = IDirectSoundNotify_SetNotificationPositions(lpDSBNotify, TDAV_DSOUND_CONSUMER_NOTIF_POS_COUNT, pPosNotify)) != DS_OK) {
        IDirectSoundNotify_Release(lpDSBNotify);
        tdav_win32_print_error("IDirectSoundBuffer_QueryInterface", hr);
        return -4;
    }

    if((hr = IDirectSoundNotify_Release(lpDSBNotify))) {
        tdav_win32_print_error("IDirectSoundNotify_Release", hr);
    }

    /* Start the buffer */
    if((hr = IDirectSoundBuffer_Play(dsound->secondaryBuffer, 0, 0, DSBPLAY_LOOPING)) != DS_OK) {
        tdav_win32_print_error("IDirectSoundNotify_Release", hr);
        return -5;
    }

    /* start the reader thread */
    dsound->started = tsk_true;
    tsk_thread_create(&dsound->tid[0], _tdav_consumer_dsound_playback_thread, dsound);

    return 0;
}

static int tdav_consumer_dsound_consume(tmedia_consumer_t* self, const void* buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
    tdav_consumer_dsound_t* dsound = (tdav_consumer_dsound_t*)self;

    if(!dsound || !buffer || !size) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }
    /* buffer is already decoded */
    return tdav_consumer_audio_put(TDAV_CONSUMER_AUDIO(dsound), buffer, size, proto_hdr);
}

static int tdav_consumer_dsound_pause(tmedia_consumer_t* self)
{
    return 0;
}

static int tdav_consumer_dsound_stop(tmedia_consumer_t* self)
{
    tdav_consumer_dsound_t* dsound = (tdav_consumer_dsound_t*)self;

    HRESULT hr;

    if(!self) {
        TSK_DEBUG_ERROR("Invalid parameter");
        return -1;
    }

    if(!dsound->started) {
        return 0;
    }

    /* should be done here */
    dsound->started = tsk_false;

    /* stop thread */
    if(dsound->tid[0]) {
        tsk_thread_join(&(dsound->tid[0]));
    }

    if((hr = IDirectSoundBuffer_Stop(dsound->secondaryBuffer)) != DS_OK) {
        tdav_win32_print_error("IDirectSoundBuffer_Stop", hr);
    }
    if((hr = IDirectSoundBuffer_SetCurrentPosition(dsound->secondaryBuffer, 0)) != DS_OK) {
        tdav_win32_print_error("IDirectSoundBuffer_SetCurrentPosition", hr);
    }

    // unprepare
    // will be prepared again before calling next start()
    _tdav_consumer_dsound_unprepare(dsound);

    return 0;
}


//
//	WaveAPI consumer object definition
//
/* constructor */
static tsk_object_t* tdav_consumer_dsound_ctor(tsk_object_t * self, va_list * app)
{
    tdav_consumer_dsound_t *consumer = self;
    if(consumer) {
        /* init base */
        tdav_consumer_audio_init(TDAV_CONSUMER_AUDIO(consumer));
        /* init self */

    }
    return self;
}
/* destructor */
static tsk_object_t* tdav_consumer_dsound_dtor(tsk_object_t * self)
{
    tdav_consumer_dsound_t *dsound = self;
    if(dsound) {
        /* stop */
        if(dsound->started) {
            tdav_consumer_dsound_stop(self);
        }

        /* deinit base */
        tdav_consumer_audio_deinit(TDAV_CONSUMER_AUDIO(dsound));
        /* deinit self */
        _tdav_consumer_dsound_unprepare(dsound);
        TSK_FREE(dsound->bytes_per_notif_ptr);
    }

    return self;
}
/* object definition */
static const tsk_object_def_t tdav_consumer_dsound_def_s = {
    sizeof(tdav_consumer_dsound_t),
    tdav_consumer_dsound_ctor,
    tdav_consumer_dsound_dtor,
    tdav_consumer_audio_cmp,
};
/* plugin definition*/
static const tmedia_consumer_plugin_def_t tdav_consumer_dsound_plugin_def_s = {
    &tdav_consumer_dsound_def_s,

    tmedia_audio,
    "Microsoft DirectSound consumer",

    tdav_consumer_dsound_set,
    tdav_consumer_dsound_prepare,
    tdav_consumer_dsound_start,
    tdav_consumer_dsound_consume,
    tdav_consumer_dsound_pause,
    tdav_consumer_dsound_stop
};
const tmedia_consumer_plugin_def_t *tdav_consumer_dsound_plugin_def_t = &tdav_consumer_dsound_plugin_def_s;


#endif /* HAVE_DSOUND_H */