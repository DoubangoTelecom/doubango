/*
* Copyright (C) 2009-2010 Mamadou Diop.
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
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tinydav/audio/directsound/tdav_consumer_dsound.h"

#if HAVE_DSOUND_H

#if defined(_MSC_VER)
#	pragma comment(lib, "dsound.lib")
#	pragma comment(lib, "dxguid.lib")
#endif

#include "tinydav/tdav_win32.h"

#include "tsk_thread.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <initguid.h>

#define tdav_consumer_dsound_set tsk_null

static void *__playback_thread(void *param)
{
	tdav_consumer_dsound_t* dsound = (tdav_consumer_dsound_t*)param; 

	HRESULT hr;
	LPVOID lpvAudio1, lpvAudio2;
	DWORD dwBytesAudio1, dwBytesAudio2;

	void* data;
	int index;

	TSK_DEBUG_INFO("__playback_thread -- START");

	SetPriorityClass(GetCurrentThread(), REALTIME_PRIORITY_CLASS);

	for(;;){
		DWORD dwEvent = WaitForMultipleObjects(sizeof(dsound->notifEvents)/sizeof(HANDLE), dsound->notifEvents, FALSE, INFINITE);

		if(!dsound->started){
			break;
		}
		else {
			tsk_size_t out_size = 0;
			data = tdav_consumer_audio_get(TDAV_CONSUMER_AUDIO(dsound), &out_size);
			index = (dwEvent == (TDAV_DSOUNS_CONSUMER_NOTIF_POS_COUNT-1)) ? 0 : (dwEvent + 1);
			
			// lock
			if((hr = IDirectSoundBuffer_Lock(dsound->secondaryBuffer, (index * dsound->bytes_per_notif), dsound->bytes_per_notif, &lpvAudio1, &dwBytesAudio1, &lpvAudio2, &dwBytesAudio2, 0)) != DS_OK){
				tdav_win32_print_error("IDirectSoundBuffer_Lock", hr);
				goto next;
			}

			if(data){
				// copy data to dsound buffers
				memcpy(lpvAudio1, data, TSK_MIN(dwBytesAudio1, out_size));
				if(lpvAudio2){
					memcpy(lpvAudio2, ((LPBYTE*)data) + dwBytesAudio1, dwBytesAudio2);
				}
			}
			else{
				// Put silence
				memset(lpvAudio1, 0, dwBytesAudio1);
				if(lpvAudio2){
					memset(lpvAudio2, 0, dwBytesAudio2);
				}
			}

			// unlock
			if((hr = IDirectSoundBuffer_Unlock(dsound->secondaryBuffer, lpvAudio1, dwBytesAudio1, lpvAudio2, dwBytesAudio2)) != DS_OK){
				tdav_win32_print_error("IDirectSoundBuffer_UnLock", hr);
				goto next;
			}
next:
			TSK_FREE(data);
		}
	}

	TSK_DEBUG_INFO("__playback_thread -- STOP");
	

	return tsk_null;
}




/* ============ Media Consumer Interface ================= */
int tdav_consumer_dsound_prepare(tmedia_consumer_t* self, const tmedia_codec_t* codec)
{
	HRESULT hr;
	HWND hWnd;

	WAVEFORMATEX wfx = {0};
	DSBUFFERDESC dsbd = {0};

	tdav_consumer_dsound_t* dsound = (tdav_consumer_dsound_t*)self;

	if(!dsound){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(dsound->device || dsound->primaryBuffer || dsound->secondaryBuffer){
		TSK_DEBUG_ERROR("Consumer already prepared");
		return -2;
	}

	TDAV_CONSUMER_AUDIO(dsound)->channels = codec->plugin->audio.channels;
	TDAV_CONSUMER_AUDIO(dsound)->rate = codec->plugin->rate;

	/* Create sound device */
	if((hr = DirectSoundCreate(NULL, &dsound->device, NULL) != DS_OK)){
		tdav_win32_print_error("DirectSoundCreate", hr);
		return -3;
	}

	/* Set CooperativeLevel */
	if((hWnd = GetConsoleWindow()) || (hWnd = GetDesktopWindow()) || (hWnd = GetForegroundWindow())){
		if((hr = IDirectSound_SetCooperativeLevel(dsound->device, hWnd, DSSCL_PRIORITY)) != DS_OK){
			tdav_win32_print_error("IDirectSound_SetCooperativeLevel", hr);
		}
	}

	/* Creates the primary buffer and apply format */
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = TDAV_CONSUMER_AUDIO(dsound)->channels;
	wfx.nSamplesPerSec = TDAV_CONSUMER_AUDIO(dsound)->rate;
	wfx.wBitsPerSample = TDAV_CONSUMER_AUDIO(dsound)->bits_per_sample;
	wfx.nBlockAlign = (wfx.nChannels * wfx.wBitsPerSample/8);
	wfx.nAvgBytesPerSec = (wfx.nSamplesPerSec * wfx.nBlockAlign);

	/* Average bytes (count) for each notification */
	dsound->bytes_per_notif = ((wfx.nAvgBytesPerSec * TDAV_CONSUMER_AUDIO(dsound)->ptime)/1000);

	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER;
	dsbd.dwBufferBytes = 0;
	dsbd.lpwfxFormat = NULL;

	if((hr = IDirectSound_CreateSoundBuffer(dsound->device, &dsbd, &dsound->primaryBuffer, NULL)) != DS_OK){
		tdav_win32_print_error("IDirectSound_CreateSoundBuffer", hr);
		return -4;
	}
	if((hr = IDirectSoundBuffer_SetFormat(dsound->primaryBuffer, &wfx)) != DS_OK){
		tdav_win32_print_error("IDirectSoundBuffer_SetFormat", hr);
		return -5;
	}

	/* Creates the secondary buffer and apply format */
	dsbd.dwFlags = (DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GLOBALFOCUS);
	dsbd.dwBufferBytes = (TDAV_DSOUNS_CONSUMER_NOTIF_POS_COUNT * dsound->bytes_per_notif);
	dsbd.lpwfxFormat = &wfx;

	if((hr = IDirectSound_CreateSoundBuffer(dsound->device, &dsbd, &dsound->secondaryBuffer, NULL)) != DS_OK){
		tdav_win32_print_error("IDirectSound_CreateSoundBuffer", hr);
		return -6;
	}
	

	return 0;
}

int tdav_consumer_dsound_start(tmedia_consumer_t* self)
{
	tdav_consumer_dsound_t* dsound = (tdav_consumer_dsound_t*)self;

	tsk_size_t i;
	HRESULT hr;
	LPDIRECTSOUNDNOTIFY lpDSBNotify;
	DSBPOSITIONNOTIFY pPosNotify[TDAV_DSOUNS_CONSUMER_NOTIF_POS_COUNT] = {0};

	if(!dsound){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!dsound->device || !dsound->primaryBuffer || !dsound->secondaryBuffer){
		TSK_DEBUG_ERROR("Consumer not prepared");
		return -2;
	}

	if(dsound->started){
		TSK_DEBUG_WARN("Consumer already started");
		return 0;
	}

	if((hr = IDirectSoundBuffer_QueryInterface(dsound->secondaryBuffer, &IID_IDirectSoundNotify, (LPVOID*)&lpDSBNotify)) != DS_OK){
		tdav_win32_print_error("IDirectSoundBuffer_QueryInterface", hr);
		return -3;
	}

	/* Events associated to notification points */
	for(i = 0; i<sizeof(dsound->notifEvents)/sizeof(HANDLE); i++){
		dsound->notifEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		pPosNotify[i].dwOffset = ((dsound->bytes_per_notif * i) + dsound->bytes_per_notif) - 1;
		pPosNotify[i].hEventNotify = dsound->notifEvents[i];
	}
	if((hr = IDirectSoundNotify_SetNotificationPositions(lpDSBNotify, TDAV_DSOUNS_CONSUMER_NOTIF_POS_COUNT, pPosNotify)) != DS_OK){
		IDirectSoundNotify_Release(lpDSBNotify);
		tdav_win32_print_error("IDirectSoundBuffer_QueryInterface", hr);
		return -4;
	}
	
	if((hr = IDirectSoundNotify_Release(lpDSBNotify))){
		tdav_win32_print_error("IDirectSoundNotify_Release", hr);
	}
	
	/* start the reader thread */
	tsk_thread_create(&dsound->tid[0], __playback_thread, dsound);

	/* Start the buffer */
	if((hr = IDirectSoundBuffer_Play(dsound->secondaryBuffer,0, 0, DSBPLAY_LOOPING)) != DS_OK){
		tdav_win32_print_error("IDirectSoundNotify_Release", hr);
		return -5;
	}

	dsound->started = tsk_true;

	return 0;
}

int tdav_consumer_dsound_consume(tmedia_consumer_t* self, void** buffer, tsk_size_t size, const tsk_object_t* proto_hdr)
{
	tdav_consumer_dsound_t* dsound = (tdav_consumer_dsound_t*)self;

	if(!dsound || !buffer || !*buffer || !size){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	/* buffer is already decoded */
	return tdav_consumer_audio_put(TDAV_CONSUMER_AUDIO(dsound), buffer, size, proto_hdr);
}

int tdav_consumer_dsound_pause(tmedia_consumer_t* self)
{
	return 0;
}

int tdav_consumer_dsound_stop(tmedia_consumer_t* self)
{
	tdav_consumer_dsound_t* dsound = (tdav_consumer_dsound_t*)self;

	HRESULT hr;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!dsound->started){
		TSK_DEBUG_WARN("Consumer not started");
		return 0;
	}

	/* should be done here */
	dsound->started = tsk_false;

	/* stop thread */
	if(dsound->tid[0]){
		tsk_thread_join(&(dsound->tid[0]));
	}

	if((hr = IDirectSoundBuffer_Stop(dsound->secondaryBuffer)) != DS_OK){
		tdav_win32_print_error("IDirectSoundBuffer_Stop", hr);
	}
	if((hr = IDirectSoundBuffer_SetCurrentPosition(dsound->secondaryBuffer, 0)) != DS_OK){
		tdav_win32_print_error("IDirectSoundBuffer_SetCurrentPosition", hr);
	}

	return 0;
}


//
//	WaveAPI consumer object definition
//
/* constructor */
static tsk_object_t* tdav_consumer_dsound_ctor(tsk_object_t * self, va_list * app)
{
	tdav_consumer_dsound_t *consumer = self;
	if(consumer){
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
	if(dsound){
		tsk_size_t i;

		/* stop */
		if(dsound->started){
			tdav_consumer_dsound_stop(self);
		}

		/* deinit base */
		tdav_consumer_audio_deinit(TDAV_CONSUMER_AUDIO(dsound));
		/* deinit self */
		// Delete secondary buffer
		if(dsound->primaryBuffer){
			IDirectSoundBuffer_Release(dsound->primaryBuffer);
		}
		if(dsound->secondaryBuffer){
			IDirectSoundBuffer_Release(dsound->secondaryBuffer);
		}
		if(dsound->device){
			IDirectSound_Release(dsound->device);
		}
		for(i = 0; i<sizeof(dsound->notifEvents)/sizeof(HANDLE); i++){
			if(dsound->notifEvents[i]){
				CloseHandle(dsound->notifEvents[i]);
			}
		}
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_consumer_dsound_def_s = 
{
	sizeof(tdav_consumer_dsound_t),
	tdav_consumer_dsound_ctor, 
	tdav_consumer_dsound_dtor,
	tdav_consumer_audio_cmp, 
};
/* plugin definition*/
static const tmedia_consumer_plugin_def_t tdav_consumer_dsound_plugin_def_s = 
{
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