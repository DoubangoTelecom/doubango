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

/**@file tdav_producer_dsound.c
 * @brief Microsoft DirectSound producer.
 *
 * @author Mamadou Diop <diopmamadou(at)doubango.org>
 *

 */
#include "tinydav/audio/directsound/tdav_producer_dsound.h"

#if HAVE_DSOUND_H

#if defined(_MSC_VER)
#	pragma comment(lib, "dsound.lib")
#	pragma comment(lib, "dxguid.lib")
#endif

#if !defined(SEND_SILENCE_ON_MUTE)
#	if METROPOLIS
#		define SEND_SILENCE_ON_MUTE	1
#	else
#		define SEND_SILENCE_ON_MUTE	0
#	endif
#endif /* SEND_SILENCE_ON_MUTE */

#include "tsk_string.h"
#include "tsk_thread.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <initguid.h>
#include <dsound.h>

extern void tdav_win32_print_error(const char* func, HRESULT hr);

#if !defined(TDAV_DSOUND_PRODUCER_NOTIF_POS_COUNT)
#	define TDAV_DSOUND_PRODUCER_NOTIF_POS_COUNT		10
#endif /* TDAV_DSOUND_PRODUCER_NOTIF_POS_COUNT */

typedef struct tdav_producer_dsound_s
{
	TDAV_DECLARE_PRODUCER_AUDIO;

	tsk_bool_t started;
	tsk_bool_t mute;
	tsk_size_t bytes_per_notif_size;
	tsk_thread_handle_t* tid[1];

	LPDIRECTSOUNDCAPTURE device;
	LPDIRECTSOUNDCAPTUREBUFFER captureBuffer;
	HANDLE notifEvents[TDAV_DSOUND_PRODUCER_NOTIF_POS_COUNT];
}
tdav_producer_dsound_t;

static void* TSK_STDCALL _tdav_producer_dsound_record_thread(void *param)
{
	tdav_producer_dsound_t* dsound = (tdav_producer_dsound_t*)param; 

	HRESULT hr;
	LPVOID lpvAudio1, lpvAudio2;
	DWORD dwBytesAudio1, dwBytesAudio2, dwEvent;

	TSK_DEBUG_INFO("__record_thread -- START");

	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

	while(dsound->started){
		dwEvent = WaitForMultipleObjects(TDAV_DSOUND_PRODUCER_NOTIF_POS_COUNT, dsound->notifEvents, FALSE, INFINITE);
		if(!dsound->started){
			break;
		}
		
		// lock
		if((hr = IDirectSoundCaptureBuffer_Lock(dsound->captureBuffer, (dwEvent * dsound->bytes_per_notif_size), dsound->bytes_per_notif_size, &lpvAudio1, &dwBytesAudio1, &lpvAudio2, &dwBytesAudio2, 0)) != DS_OK){
			tdav_win32_print_error("IDirectSoundCaptureBuffer_Lock", hr);
			continue;
		}

		if(TMEDIA_PRODUCER(dsound)->enc_cb.callback){
#if SEND_SILENCE_ON_MUTE
			if(dsound->mute){
				memset(lpvAudio1, 0, dwBytesAudio1);
				if(lpvAudio2){
					memset(lpvAudio2, 0, dwBytesAudio2);
				}
			}
#endif
			TMEDIA_PRODUCER(dsound)->enc_cb.callback(TMEDIA_PRODUCER(dsound)->enc_cb.callback_data, lpvAudio1, dwBytesAudio1);
			if(lpvAudio2){
				TMEDIA_PRODUCER(dsound)->enc_cb.callback(TMEDIA_PRODUCER(dsound)->enc_cb.callback_data, lpvAudio2, dwBytesAudio2);
			}
		}

		// unlock
		if((hr = IDirectSoundCaptureBuffer_Unlock(dsound->captureBuffer, lpvAudio1, dwBytesAudio1, lpvAudio2, dwBytesAudio2)) != DS_OK){
			tdav_win32_print_error("IDirectSoundCaptureBuffer_Unlock", hr);
			continue;
		}		
	}

	TSK_DEBUG_INFO("__record_thread -- STOP");
	

	return tsk_null;
}

static int _tdav_producer_dsound_unprepare(tdav_producer_dsound_t* dsound)
{
	if(dsound){
		tsk_size_t i;
		if(dsound->captureBuffer){
			IDirectSoundCaptureBuffer_Release(dsound->captureBuffer);
			dsound->captureBuffer = NULL;
		}
		if(dsound->device){
			IDirectSoundCapture_Release(dsound->device);
			dsound->device = NULL;
		}
		for(i = 0; i<sizeof(dsound->notifEvents)/sizeof(dsound->notifEvents[0]); i++){
			if(dsound->notifEvents[i]){
				CloseHandle(dsound->notifEvents[i]);
				dsound->notifEvents[i] = NULL;
			}
		}
	}
	return 0;
}




/* ============ Media Producer Interface ================= */
static int tdav_producer_dsound_set(tmedia_producer_t* self, const tmedia_param_t* param)
{	
	tdav_producer_dsound_t* dsound = (tdav_producer_dsound_t*)self;
	if(param->plugin_type == tmedia_ppt_producer){
		if(param->value_type == tmedia_pvt_int32){
			if(tsk_striequals(param->key, "volume")){
				return 0;
			}
			else if(tsk_striequals(param->key, "mute")){
				dsound->mute = (TSK_TO_INT32((uint8_t*)param->value) != 0);
#if !SEND_SILENCE_ON_MUTE
				if(dsound->started){
					if(dsound->mute){
						IDirectSoundCaptureBuffer_Stop(dsound->captureBuffer);
					}
					else{
						IDirectSoundCaptureBuffer_Start(dsound->captureBuffer, DSBPLAY_LOOPING);
					}
				}
#endif
				return 0;
			}
		}
	}
	return tdav_producer_audio_set(TDAV_PRODUCER_AUDIO(self), param);
}
static int tdav_producer_dsound_prepare(tmedia_producer_t* self, const tmedia_codec_t* codec)
{
	HRESULT hr;

	WAVEFORMATEX wfx = {0};
	DSCBUFFERDESC dsbd = {0};

	tdav_producer_dsound_t* dsound = (tdav_producer_dsound_t*)self;

	if(!dsound || !codec){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(dsound->device || dsound->captureBuffer){
		TSK_DEBUG_ERROR("Producer already prepared");
		return -2;
	}

	TMEDIA_PRODUCER(dsound)->audio.channels = TMEDIA_CODEC_CHANNELS_AUDIO_ENCODING(codec);
	TMEDIA_PRODUCER(dsound)->audio.rate = TMEDIA_CODEC_RATE_ENCODING(codec);
	TMEDIA_PRODUCER(dsound)->audio.ptime = TMEDIA_CODEC_PTIME_AUDIO_ENCODING(codec);

	/* Create capture device */
	if((hr = DirectSoundCaptureCreate(NULL, &dsound->device, NULL) != DS_OK)){
		tdav_win32_print_error("DirectSoundCaptureCreate", hr);
		return -3;
	}

	/* Creates the capture buffer */
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = TMEDIA_PRODUCER(dsound)->audio.channels;
	wfx.nSamplesPerSec = TMEDIA_PRODUCER(dsound)->audio.rate;
	wfx.wBitsPerSample = TMEDIA_PRODUCER(dsound)->audio.bits_per_sample;
	wfx.nBlockAlign = (wfx.nChannels * wfx.wBitsPerSample/8);
	wfx.nAvgBytesPerSec = (wfx.nSamplesPerSec * wfx.nBlockAlign);

	/* Average bytes (count) for each notification */
	dsound->bytes_per_notif_size = ((wfx.nAvgBytesPerSec * TMEDIA_PRODUCER(dsound)->audio.ptime)/1000);

	dsbd.dwSize = sizeof(DSCBUFFERDESC);
	dsbd.dwBufferBytes = (TDAV_DSOUND_PRODUCER_NOTIF_POS_COUNT * dsound->bytes_per_notif_size);
	dsbd.lpwfxFormat = &wfx;

	if((hr = IDirectSoundCapture_CreateCaptureBuffer(dsound->device, &dsbd, &dsound->captureBuffer, NULL)) != DS_OK){
		tdav_win32_print_error("IDirectSoundCapture_CreateCaptureBuffer", hr);
		return -4;
	}	

	return 0;
}

static int tdav_producer_dsound_start(tmedia_producer_t* self)
{
	tdav_producer_dsound_t* dsound = (tdav_producer_dsound_t*)self;

	tsk_size_t i;
	HRESULT hr;
	LPDIRECTSOUNDNOTIFY lpDSBNotify;
	DSBPOSITIONNOTIFY pPosNotify[TDAV_DSOUND_PRODUCER_NOTIF_POS_COUNT] = {0};

	if(!dsound){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!dsound->device || !dsound->captureBuffer){
		TSK_DEBUG_ERROR("Producer not prepared");
		return -2;
	}

	if(dsound->started){
		return 0;
	}

	if((hr = IDirectSoundCaptureBuffer_QueryInterface(dsound->captureBuffer, &IID_IDirectSoundNotify, (LPVOID*)&lpDSBNotify)) != DS_OK){
		tdav_win32_print_error("IDirectSoundCaptureBuffer_QueryInterface", hr);
		return -3;
	}

	/* Events associated to notification points */
	for(i = 0; i<TDAV_DSOUND_PRODUCER_NOTIF_POS_COUNT; i++){
		dsound->notifEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		pPosNotify[i].dwOffset = (dsound->bytes_per_notif_size * i) + (dsound->bytes_per_notif_size >> 1);
		pPosNotify[i].hEventNotify = dsound->notifEvents[i];
	}
	if((hr = IDirectSoundNotify_SetNotificationPositions(lpDSBNotify, TDAV_DSOUND_PRODUCER_NOTIF_POS_COUNT, pPosNotify)) != DS_OK){
		IDirectSoundNotify_Release(lpDSBNotify);
		tdav_win32_print_error("IDirectSoundBuffer_QueryInterface", hr);
		return -4;
	}
	
	if((hr = IDirectSoundNotify_Release(lpDSBNotify))){
		tdav_win32_print_error("IDirectSoundNotify_Release", hr);
	}

	/* Start the buffer */
	if((hr = IDirectSoundCaptureBuffer_Start(dsound->captureBuffer, DSBPLAY_LOOPING)) != DS_OK){
		tdav_win32_print_error("IDirectSoundCaptureBuffer_Start", hr);
		return -5;
	}

	/* start the reader thread */
	dsound->started = tsk_true;
	tsk_thread_create(&dsound->tid[0], _tdav_producer_dsound_record_thread, dsound);

	return 0;
}

static int tdav_producer_dsound_pause(tmedia_producer_t* self)
{
	return 0;
}

static int tdav_producer_dsound_stop(tmedia_producer_t* self)
{
	tdav_producer_dsound_t* dsound = (tdav_producer_dsound_t*)self;

	HRESULT hr;

	if(!self){
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	if(!dsound->started){
		return 0;
	}

	// should be done here
	dsound->started = tsk_false;

#if !SEND_SILENCE_ON_MUTE
	if(dsound->mute && dsound->notifEvents[0]){
		// thread is paused -> raise event now that "started" is equal to false
		SetEvent(dsound->notifEvents[0]);
	}		
#endif

	// stop thread
	if(dsound->tid[0]){
		tsk_thread_join(&(dsound->tid[0]));
	}

	if((hr = IDirectSoundCaptureBuffer_Stop(dsound->captureBuffer)) != DS_OK){
		tdav_win32_print_error("IDirectSoundCaptureBuffer_Stop", hr);
	}

	// unprepare
	// will be prepared again before next start()
	_tdav_producer_dsound_unprepare(dsound);

	return 0;
}


//
//	WaveAPI producer object definition
//
/* constructor */
static tsk_object_t* tdav_producer_dsound_ctor(tsk_object_t * self, va_list * app)
{
	tdav_producer_dsound_t *producer = self;
	if(producer){
		/* init base */
		tdav_producer_audio_init(TDAV_PRODUCER_AUDIO(producer));
		/* init self */
		
	}
	return self;
}
/* destructor */
static tsk_object_t* tdav_producer_dsound_dtor(tsk_object_t * self)
{ 
	tdav_producer_dsound_t *dsound = self;
	if(dsound){
		/* stop */
		if(dsound->started){
			tdav_producer_dsound_stop(self);
		}

		/* deinit base */
		tdav_producer_audio_deinit(TDAV_PRODUCER_AUDIO(dsound));
		/* deinit self */
		_tdav_producer_dsound_unprepare(dsound);
	}

	return self;
}
/* object definition */
static const tsk_object_def_t tdav_producer_dsound_def_s = 
{
	sizeof(tdav_producer_dsound_t),
	tdav_producer_dsound_ctor, 
	tdav_producer_dsound_dtor,
	tdav_producer_audio_cmp, 
};
/* plugin definition*/
static const tmedia_producer_plugin_def_t tdav_producer_dsound_plugin_def_s = 
{
	&tdav_producer_dsound_def_s,
	
	tmedia_audio,
	"Microsoft DirectSound producer",
	
	tdav_producer_dsound_set,
	tdav_producer_dsound_prepare,
	tdav_producer_dsound_start,
	tdav_producer_dsound_pause,
	tdav_producer_dsound_stop
};
const tmedia_producer_plugin_def_t *tdav_producer_dsound_plugin_def_t = &tdav_producer_dsound_plugin_def_s;


#endif /* HAVE_DSOUND_H */