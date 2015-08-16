/* Copyright (C) 2013 Mamadou DIOP
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
// MS Voice Capture DSP: http://msdn.microsoft.com/en-us/library/windows/desktop/ff819492(v=vs.85).aspx
// Features:
// - Acoustic echo cancellation (AEC)
// - Microphone array processing
// - Noise suppression
// - Automatic gain control
// - Voice activity detection
#include "plugin_audio_dsp_utils.h"
#include "plugin_audio_dsp_mediabuffer.h"

#include "tinymedia/tmedia_denoise.h"
#include "tinymedia/tmedia_defaults.h"

#include "tsk_string.h"
#include "tsk_memory.h"
#include "tsk_debug.h"

#include <Wmcodecdsp.h>
#include <Dmo.h>

static const UINT32 g_nMicrophoneStreamIndex = 0;
static const UINT32 g_nSpeakerStreamIndex = 0;

static const UINT32 g_nBitsPerSample = 16;
static const UINT32 g_nChannles = 1; // FIXME
static const UINT32 g_nFrameDuration = 20; // FIXME

/** Speex denoiser*/
typedef struct plugin_audio_dsp_denoise_s
{
	TMEDIA_DECLARE_DENOISE;
	
	bool bOpened;

	LONGLONG rtStart;
    UINT64 rtDuration;

	uint32_t echo_tail;
	tsk_size_t playback_size_samples;
	tsk_size_t playback_size_bytes;
	tsk_size_t playback_channels;
	tsk_size_t record_size_samples;
	tsk_size_t record_size_bytes;
	tsk_size_t record_channels;

	IMediaObject* pInst;
	IMediaBuffer *pBufferIn;
	IMediaBuffer *pBufferOut;
}
plugin_audio_dsp_denoise_t;

static int plugin_audio_dsp_denoise_set(tmedia_denoise_t* _self, const tmedia_param_t* param)
{
	plugin_audio_dsp_denoise_t *self = (plugin_audio_dsp_denoise_t *)_self;
	if(!self || !param)
	{
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}
	
	if(param->value_type == tmedia_pvt_int32)
	{
		if(tsk_striequals(param->key, "echo-tail"))
		{
			_self->echo_tail = *((int32_t*)param->value);
			TSK_DEBUG_INFO("ms_voice_dsp_set_echo_tail(%d)", _self->echo_tail);
			if(self->pInst)
			{
				IPropertyStore* pPropStore = NULL;
				HRESULT hr = self->pInst->QueryInterface(IID_PPV_ARGS(&pPropStore));
				if(SUCCEEDED(hr))
				{
					DMO_MEDIA_TYPE mt = {0};
					PROPVARIANT var = {0};
					var.vt = VT_UI4;
					var.ulVal = _self->echo_tail;
					hr = pPropStore->SetValue(MFPKEY_WMAAECMA_FEATR_ECHO_LENGTH , var);
				}
				SafeRelease(&pPropStore);
			}
			return 0;
		}
	}
	return -1;
}

static int plugin_audio_dsp_denoise_open(tmedia_denoise_t* self, uint32_t record_frame_size_samples, uint32_t record_sampling_rate, uint32_t playback_frame_size_samples, uint32_t playback_sampling_rate)
{
	plugin_audio_dsp_denoise_t *denoiser = (plugin_audio_dsp_denoise_t *)self;

	HRESULT hr = S_OK;
	DMO_MEDIA_TYPE mt = {0};
	PROPVARIANT var = {0};
	IPropertyStore* pPropStore = NULL;

	TSK_DEBUG_INFO("[MS Voice Capture DSP] AEC_ENABLED=%d ECHO_TAIL=%d,\nAGC_ENABLED=%d,\nNOISE_SUPP_ENABLED=%d,\nVAD_ENABLED=%d",
		self->echo_supp_enabled, self->echo_tail,
		self->agc_enabled,
		self->noise_supp_enabled,
		self->vad_enabled
		);

	if(denoiser->bOpened)
	{
		TSK_DEBUG_ERROR("Denoiser already opened");
		CHECK_HR(hr = E_FAIL);
	}
	
	CHECK_HR(hr = AudioDSPUtils::MoInitMediaType(
		record_sampling_rate,
		g_nBitsPerSample,
		g_nChannles,
		&mt));

	CHECK_HR(hr = CoCreateInstance(CLSID_CWMAudioAEC, NULL, 
				CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&denoiser->pInst)));
	CHECK_HR(hr = denoiser->pInst->QueryInterface(IID_PPV_ARGS(&pPropStore)));

	// If the input format does not match the output format, the DMO automatically performs sample-rate conversion.
	CHECK_HR(hr = denoiser->pInst->SetInputType(0, &mt, 0));
	CHECK_HR(hr = denoiser->pInst->SetOutputType(0, &mt, 0));

	// Enables the application to override the default settings on various properties of the Voice Capture DSP
	// http://msdn.microsoft.com/en-us/library/windows/desktop/ff819422(v=vs.85).aspx
	var.vt = VT_BOOL;
	var.boolVal = VARIANT_TRUE;
	CHECK_HR(hr = pPropStore->SetValue(MFPKEY_WMAAECMA_FEATURE_MODE, var));

	// Switch to filter mode: http://msdn.microsoft.com/en-us/library/windows/desktop/ff819410(v=vs.85).aspx
	var.vt = VT_BOOL;
	var.boolVal = VARIANT_FALSE; /* VARIANT_FALSE: Filter, VARIANT_TRUE: Source */
	CHECK_HR(hr = pPropStore->SetValue(MFPKEY_WMAAECMA_DMO_SOURCE_MODE, var));

	// Enable AEC
	if(self->echo_supp_enabled)
	{
		// Enable AEC: http://msdn.microsoft.com/en-us/library/windows/desktop/ff819427(v=vs.85).aspx
		var.vt = VT_I4;
		var.lVal = SINGLE_CHANNEL_AEC;
		CHECK_HR(hr = pPropStore->SetValue(MFPKEY_WMAAECMA_SYSTEM_MODE, var));

		// Echo Tail (milliseconds): http://msdn.microsoft.com/en-us/library/windows/desktop/ff819414(v=vs.85).aspx
		if(!self->echo_tail)
		{
			self->echo_tail = tmedia_defaults_get_echo_tail();
		}
		var.vt = VT_I4;
		var.lVal = self->echo_tail ? self->echo_tail : 256;
		CHECK_HR(hr = pPropStore->SetValue(MFPKEY_WMAAECMA_FEATR_ECHO_LENGTH, var));
	}	

	// Automatic Gain Control (AGC): http://msdn.microsoft.com/en-us/library/windows/desktop/ff819412(v=vs.85).aspx
	var.vt = VT_BOOL;
	var.boolVal = self->agc_enabled ? VARIANT_TRUE : VARIANT_FALSE;
	CHECK_HR(hr = pPropStore->SetValue(MFPKEY_WMAAECMA_FEATR_AGC, var));

	// Noise suppression (NS): http://msdn.microsoft.com/en-us/library/windows/desktop/ff819420(v=vs.85).aspx
	var.vt = VT_I4;
	var.lVal = self->noise_supp_enabled ? 1 : 0;
	CHECK_HR(hr = pPropStore->SetValue(MFPKEY_WMAAECMA_FEATR_NS, var));

	// Automatic Gain Control (AGC): http://msdn.microsoft.com/en-us/library/windows/desktop/ff819412(v=vs.85).aspx
	var.vt = VT_BOOL;
	var.boolVal = self->agc_enabled ? VARIANT_TRUE : VARIANT_FALSE;
	CHECK_HR(hr = pPropStore->SetValue(MFPKEY_WMAAECMA_FEATR_AGC, var));

	// Voice Activity Detection (VAD): http://msdn.microsoft.com/en-us/library/windows/desktop/ff819421(v=vs.85).aspx
	var.vt = VT_I4;
	var.lVal = self->vad_enabled ? AEC_VAD_FOR_SILENCE_SUPPRESSION : AEC_VAD_DISABLED;
	CHECK_HR(hr = pPropStore->SetValue(MFPKEY_WMAAECMA_FEATR_VAD, var));

	// Recommended to allocate resources
	CHECK_HR(hr = denoiser->pInst->AllocateStreamingResources()); // FIXME: returns E_FAIL

	denoiser->record_channels = g_nChannles;
	denoiser->record_size_samples  = ((record_sampling_rate * g_nFrameDuration) / 1000) << (denoiser->record_channels == 2 ? 1 : 0);
	denoiser->record_size_bytes = (denoiser->record_size_samples * (g_nBitsPerSample >> 3));

	denoiser->playback_channels = g_nChannles;
	denoiser->playback_size_samples  = ((playback_sampling_rate * g_nFrameDuration) / 1000) << (denoiser->playback_channels == 2 ? 1 : 0);
	denoiser->playback_size_bytes = (denoiser->playback_size_samples * (g_nBitsPerSample >> 3));

	denoiser->rtStart = 0;
	denoiser->rtDuration = PLUGIN_AUDIO_DSP_MILLIS_TO_100NS(g_nFrameDuration); // milliseconds -> 100ns

bail:
	denoiser->bOpened = SUCCEEDED(hr);

	MoFreeMediaType(&mt);
	SafeRelease(&pPropStore);

	return denoiser->bOpened ? 0 : -1;
}

// playback = "stream 1"
// /!\Thread safety: could be called at the same time as plugin_audio_dsp_denoise_process_record()
static int plugin_audio_dsp_denoise_echo_playback(tmedia_denoise_t* self, const void* echo_frame, uint32_t echo_frame_size_bytes)
{
	plugin_audio_dsp_denoise_t *denoiser = (plugin_audio_dsp_denoise_t *)self;

	HRESULT hr = S_OK;

	if(!self || !echo_frame || !echo_frame_size_bytes)
	{
		CHECK_HR(hr = E_POINTER);
	}

	if(!denoiser->bOpened)
	{
		TSK_DEBUG_ERROR("Denoiser not opened");
		CHECK_HR(hr = E_FAIL);
	}

	if(denoiser->record_size_bytes != echo_frame_size_bytes)
	{
		TSK_DEBUG_ERROR("Size mismatch: %u<>%u", denoiser->record_size_bytes, echo_frame_size_bytes);
		CHECK_HR(hr = E_INVALIDARG);
	}

	
	
bail:
	return SUCCEEDED(hr) ? 0 : -1;
}


// record = "stream 0"
// /!\Thread safety: could be called at the same time as plugin_audio_dsp_denoise_echo_playback()
static int plugin_audio_dsp_denoise_process_record(tmedia_denoise_t* self, void* audio_frame, uint32_t audio_frame_size_bytes, tsk_bool_t* silence_or_noise)
{
	plugin_audio_dsp_denoise_t *denoiser = (plugin_audio_dsp_denoise_t *)self;

	HRESULT hr = S_OK;
	BYTE* pBufferInPtr = NULL;
	DWORD dwBufferInSize = 0;

	if(!self || !audio_frame || !audio_frame_size_bytes || !silence_or_noise)
	{
		CHECK_HR(hr = E_POINTER);
	}

	if(!denoiser->bOpened)
	{
		TSK_DEBUG_ERROR("Denoiser not opened");
		CHECK_HR(hr = E_FAIL);
	}

	if(denoiser->record_size_bytes != audio_frame_size_bytes)
	{
		TSK_DEBUG_ERROR("Size mismatch: %u<>%u", denoiser->record_size_bytes, audio_frame_size_bytes);
		CHECK_HR(hr = E_INVALIDARG);
	}

	if(!denoiser->pBufferIn)
	{
		CHECK_HR(hr = AudioDSPMediaBuffer::Create(denoiser->record_size_bytes, &denoiser->pBufferIn));
	}
	else
	{
		DWORD dwMaxLength = 0;
		CHECK_HR(hr = denoiser->pBufferIn->GetMaxLength(&dwMaxLength));
		if(dwMaxLength < denoiser->record_size_bytes)
		{
			SafeRelease(&denoiser->pBufferIn);
			CHECK_HR(hr = AudioDSPMediaBuffer::Create(denoiser->record_size_bytes, &denoiser->pBufferIn));
		}
	}
	
	// Get memory pointer to the input buffer
	CHECK_HR(hr = denoiser->pBufferIn->GetBufferAndLength(&pBufferInPtr, NULL));
	// Copy data
	dwBufferInSize = TSK_MIN(audio_frame_size_bytes, denoiser->record_size_bytes);
	memcpy(pBufferInPtr, audio_frame, dwBufferInSize);
	CHECK_HR(hr = denoiser->pBufferIn->SetLength(dwBufferInSize));

	// Process input
	hr = denoiser->pInst->ProcessInput(
		g_nMicrophoneStreamIndex, 
		denoiser->pBufferIn, 
		(/*DMO_INPUT_DATA_BUFFERF_TIME | DMO_INPUT_DATA_BUFFERF_TIMELENGTH*/0),
		denoiser->rtStart, 
		denoiser->rtDuration);
	if(hr == DMO_E_NOTACCEPTING)
	{
		hr = S_OK;
	}
	CHECK_HR(hr);

	denoiser->rtStart += denoiser->rtDuration;


bail:
	return SUCCEEDED(hr) ? 0 : -1;
}

static int plugin_audio_dsp_denoise_process_playback(tmedia_denoise_t* self, void* audio_frame, uint32_t audio_frame_size_bytes)
{
	plugin_audio_dsp_denoise_t *denoiser = (plugin_audio_dsp_denoise_t *)self;
	
	(void)(denoiser);

	// Not mandatory to denoise audio before playback.
	// All Doubango clients support noise suppression.
	return 0;
}

static int plugin_audio_dsp_denoise_close(tmedia_denoise_t* self)
{
	plugin_audio_dsp_denoise_t *denoiser = (plugin_audio_dsp_denoise_t *)self;	

	if(!self)
	{
		TSK_DEBUG_ERROR("Invalid parameter");
		return -1;
	}

	denoiser->bOpened = false;
	SafeRelease(&denoiser->pBufferIn);
	SafeRelease(&denoiser->pBufferOut);
	SafeRelease(&denoiser->pInst);
	return 0;
}



//
//	MS Voice Capture DSP Plugin definition
//

/* constructor */
static tsk_object_t* plugin_audio_dsp_denoise_ctor(tsk_object_t * self, va_list * app)
{
	plugin_audio_dsp_denoise_t *denoise = (plugin_audio_dsp_denoise_t*)self;
	if(denoise)
	{
		AudioDSPUtils::Startup();

		/* init base */
		tmedia_denoise_init(TMEDIA_DENOISE(denoise));
		/* init self */
	}
	return self;
}
/* destructor */
static tsk_object_t* plugin_audio_dsp_denoise_dtor(tsk_object_t * self)
{ 
	plugin_audio_dsp_denoise_t *denoiser = (plugin_audio_dsp_denoise_t*)self;
	if(denoiser)
	{
		/* deinit base */
		tmedia_denoise_deinit(TMEDIA_DENOISE(denoiser));
		/* deinit self */
		SafeRelease(&denoiser->pBufferIn);
		SafeRelease(&denoiser->pBufferOut);
		SafeRelease(&denoiser->pInst);

		TSK_DEBUG_INFO("*** MS Voice Capture DSP destroyed ***");
	}

	return self;
}
/* object definition */
static const tsk_object_def_t plugin_audio_dsp_denoise_def_s = 
{
	sizeof(plugin_audio_dsp_denoise_t),
	plugin_audio_dsp_denoise_ctor, 
	plugin_audio_dsp_denoise_dtor,
	tsk_null, 
};
/* plugin definition*/
static const tmedia_denoise_plugin_def_t plugin_audio_dsp_denoise_plugin_def_s = 
{
	&plugin_audio_dsp_denoise_def_s,

	"MS Voice Capture DSP", /* http://msdn.microsoft.com/en-us/library/windows/desktop/ff819492(v=vs.85).aspx */

	plugin_audio_dsp_denoise_set,
	plugin_audio_dsp_denoise_open,
	plugin_audio_dsp_denoise_echo_playback,
	plugin_audio_dsp_denoise_process_record,
	plugin_audio_dsp_denoise_process_playback,
	plugin_audio_dsp_denoise_close,
};
const tmedia_denoise_plugin_def_t *plugin_audio_dsp_denoise_plugin_def_t = &plugin_audio_dsp_denoise_plugin_def_s;
