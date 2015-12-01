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
// MS Audio Resampler DSP: http://msdn.microsoft.com/en-us/library/windows/desktop/ff819070(v=vs.85).aspx
#include "plugin_audio_dsp_utils.h"

#include "tinymedia/tmedia_resampler.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

#include <initguid.h>
#include <Wmcodecdsp.h>
#include <Mftransform.h>
#include <Mfapi.h>
#include <Mferror.h>

#if !defined(PLUGIN_AUDIO_DSP_RESAMPLER_MAX_QUALITY)
#	define PLUGIN_AUDIO_DSP_RESAMPLER_MAX_QUALITY 60 /* [1 - 60]: http://msdn.microsoft.com/en-us/library/windows/desktop/ff819449(v=vs.85).aspx */
#endif

static const UINT32 g_nBitsPerSample = 16;
static HRESULT ProcessOutput(struct plugin_audio_dsp_resampler_s *resampler, IMFSample **ppSample);

typedef struct plugin_audio_dsp_resampler_s
{
	TMEDIA_DECLARE_RESAMPLER;

	bool bOpened;

	tsk_size_t in_size_samples;
	tsk_size_t in_size_bytes;
	tsk_size_t out_size_samples;
	tsk_size_t out_size_bytes;
	uint32_t in_channels;
	uint32_t out_channels;
	
	LONGLONG rtStart;
    UINT64 rtDuration;
	
	IMFTransform* pMFT;
	IMFSample *pSampleIn;
	IMFSample *pSampleOut;
}
plugin_audio_dsp_resampler_t;

// Doubango engine uses quality from [1 - 10].
static int plugin_audio_dsp_resampler_open(tmedia_resampler_t* self, uint32_t in_freq, uint32_t out_freq, uint32_t frame_duration, uint32_t in_channels, uint32_t out_channels, uint32_t quality)
{
	plugin_audio_dsp_resampler_t *resampler = (plugin_audio_dsp_resampler_t *)self;

	IMFMediaType* pTypeIn = NULL;
	IMFMediaType* pTypeOut = NULL;
	IWMResamplerProps* pProps = NULL;
	HRESULT hr = S_OK;
	
	if(in_channels != 1 && in_channels != 2)
	{
		TSK_DEBUG_ERROR("%d not valid as input channel", in_channels);
		CHECK_HR(hr = E_INVALIDARG);
	}
	if(out_channels != 1 && out_channels != 2)
	{
		TSK_DEBUG_ERROR("%d not valid as output channel", out_channels);
		CHECK_HR(hr = E_INVALIDARG);
	}

	if(resampler->bOpened)
	{
		TSK_DEBUG_ERROR("Resampler already opened");
		CHECK_HR(hr = E_FAIL);
	}

	resampler->in_size_samples  = ((in_freq * frame_duration) / 1000) << (in_channels == 2 ? 1 : 0);
	resampler->out_size_samples = ((out_freq * frame_duration) / 1000) << (out_channels == 2 ? 1 : 0);
	resampler->in_channels = in_channels;
	resampler->out_channels = out_channels;

	resampler->in_size_bytes = (resampler->in_size_samples * (g_nBitsPerSample >> 3));
	resampler->out_size_bytes = (resampler->out_size_samples * (g_nBitsPerSample >> 3));

	resampler->rtStart = 0;
	resampler->rtDuration = PLUGIN_AUDIO_DSP_MILLIS_TO_100NS(frame_duration); // milliseconds -> 100ns

	CHECK_HR(hr = CoCreateInstance(CLSID_CResamplerMediaObject, NULL, 
				CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&resampler->pMFT)));

	CHECK_HR(hr = AudioDSPUtils::CreatePCMAudioType(in_freq, g_nBitsPerSample, in_channels, &pTypeIn));
	CHECK_HR(hr = AudioDSPUtils::CreatePCMAudioType(out_freq, g_nBitsPerSample, out_channels, &pTypeOut));

	CHECK_HR(hr = resampler->pMFT->SetInputType(0, pTypeIn, 0));
	CHECK_HR(hr = resampler->pMFT->SetOutputType(0, pTypeOut, 0));

	CHECK_HR(hr = resampler->pMFT->QueryInterface(IID_PPV_ARGS(&pProps)));
    CHECK_HR(hr = pProps->SetHalfFilterLength((quality * PLUGIN_AUDIO_DSP_RESAMPLER_MAX_QUALITY) / 10)); // [1 - 10] -> [1 - 60]

	CHECK_HR(hr = resampler->pMFT->ProcessMessage(MFT_MESSAGE_COMMAND_FLUSH, NULL));
    CHECK_HR(hr = resampler->pMFT->ProcessMessage(MFT_MESSAGE_NOTIFY_BEGIN_STREAMING, NULL));
    CHECK_HR(hr = resampler->pMFT->ProcessMessage(MFT_MESSAGE_NOTIFY_START_OF_STREAM, NULL));

bail:
	resampler->bOpened = SUCCEEDED(hr);
	if(!resampler->bOpened)
	{
		SafeRelease(&resampler->pMFT);
	}
	SafeRelease(&pTypeIn);
	SafeRelease(&pTypeOut);
	SafeRelease(&pProps);
	return resampler->bOpened ? 0 : -1;
}

static tsk_size_t plugin_audio_dsp_resampler_process(tmedia_resampler_t* self, const uint16_t* in_data, tsk_size_t in_size, uint16_t* out_data, tsk_size_t out_size)
{
	plugin_audio_dsp_resampler_t *resampler = (plugin_audio_dsp_resampler_t *)self;

	HRESULT hr = S_OK;
	tsk_size_t retSize = 0;
	
	if(!resampler || !out_data)
	{
		CHECK_HR(hr = E_POINTER);
	}

	if(!resampler->bOpened)
	{
		TSK_DEBUG_ERROR("Resampler not opened");
		CHECK_HR(hr = E_FAIL);
	}

	if(in_size != resampler->in_size_samples)
	{
		TSK_DEBUG_ERROR("Input data has wrong size");
		CHECK_HR(hr = E_FAIL);
	}

	if(out_size < resampler->out_size_samples)
	{
		TSK_DEBUG_ERROR("Output data is too short");
		CHECK_HR(hr = E_FAIL);
	}

	IMFMediaBuffer* pBufferIn = NULL;
	IMFMediaBuffer* pBufferOut = NULL;
	IMFSample *pSampleOut = NULL;
	BYTE* pBufferPtr = NULL;

	if(!resampler->pSampleIn)
	{
		CHECK_HR(hr = AudioDSPUtils::CreateMediaSample(resampler->in_size_bytes, &resampler->pSampleIn));
		hr = resampler->pSampleIn->GetBufferByIndex(0, &pBufferIn);
		if(FAILED(hr))
		{
			SafeRelease(&resampler->pSampleIn);
			CHECK_HR(hr);
		}
	}
	else
	{
		DWORD dwMaxLength = 0;
		CHECK_HR(hr = resampler->pSampleIn->GetBufferByIndex(0, &pBufferIn));
		CHECK_HR(hr = pBufferIn->GetMaxLength(&dwMaxLength));
		if(dwMaxLength < resampler->in_size_bytes)
		{
			CHECK_HR(hr = resampler->pSampleIn->RemoveAllBuffers());
			SafeRelease(&pBufferIn);
			CHECK_HR(hr = MFCreateMemoryBuffer(resampler->in_size_bytes, &pBufferIn));    
			CHECK_HR(hr = resampler->pSampleIn->AddBuffer(pBufferIn));
		}
	}

	CHECK_HR(hr = pBufferIn->Lock(&pBufferPtr, NULL, NULL));
	memcpy(pBufferPtr, in_data, resampler->in_size_bytes);
	CHECK_HR(hr = pBufferIn->Unlock());
	CHECK_HR(hr = pBufferIn->SetCurrentLength(resampler->in_size_bytes));

	CHECK_HR(hr = resampler->pSampleIn->SetSampleDuration(resampler->rtDuration));
	CHECK_HR(hr = resampler->pSampleIn->SetSampleTime(resampler->rtStart));
	
	// Process input
	hr = resampler->pMFT->ProcessInput(0, resampler->pSampleIn, 0);
	if(hr == MF_E_NOTACCEPTING)
	{
		hr = S_OK;
	}
	CHECK_HR(hr);

	resampler->rtStart += resampler->rtDuration;

	// Process output
	CHECK_HR(hr = ProcessOutput(resampler, &pSampleOut));
	if(pSampleOut)
	{
		CHECK_HR(hr = pSampleOut->GetBufferByIndex(0, &pBufferOut));

		BYTE* pBufferOutPtr = NULL;
		DWORD dwDataLength = 0;
		CHECK_HR(hr = pBufferOut->GetCurrentLength(&dwDataLength));
		//if(dwDataLength == resampler->out_size_bytes)
		{
			CHECK_HR(hr = pBufferOut->Lock(&pBufferOutPtr, NULL, NULL));
			{
				memcpy(out_data, pBufferOutPtr, TSK_MIN(dwDataLength, resampler->out_size_bytes));
				if(dwDataLength < resampler->out_size_bytes)
				{
					TSK_DEBUG_INFO("[MS Resampler DSP] Output too short filling with silence");
					memset(&((uint8_t*)out_data)[dwDataLength], 0, (resampler->out_size_bytes - dwDataLength));
				}
				retSize = (tsk_size_t)resampler->out_size_bytes;
			}
			CHECK_HR(hr = pBufferOut->Unlock());
		}
	}
	
	
bail:
	SafeRelease(&pBufferIn);
	SafeRelease(&pBufferOut);
	SafeRelease(&pSampleOut);
	
	return retSize;
}

static int plugin_audio_dsp_resampler_close(tmedia_resampler_t* self)
{
	plugin_audio_dsp_resampler_t *resampler = (plugin_audio_dsp_resampler_t *)self;

	HRESULT hr = S_OK;

	if(resampler->pMFT)
	{
		hr = resampler->pMFT->ProcessMessage(MFT_MESSAGE_NOTIFY_END_OF_STREAM, NULL);
	}

	SafeRelease(&resampler->pMFT);
	SafeRelease(&resampler->pSampleIn);
	SafeRelease(&resampler->pSampleOut);

	resampler->bOpened = false;
	
	return 0;
}

static HRESULT ProcessOutput(plugin_audio_dsp_resampler_t *resampler, IMFSample **ppSample)
{
	*ppSample = NULL;

    IMFMediaBuffer* pBufferOut = NULL;

    DWORD dwStatus;

	HRESULT hr = S_OK;

    MFT_OUTPUT_DATA_BUFFER mftOutputData = { 0 };

	if(!resampler || !ppSample)
	{
		CHECK_HR(hr = E_POINTER);
	}

	if(!resampler->pSampleOut)
	{
		CHECK_HR(hr = AudioDSPUtils::CreateMediaSample(resampler->out_size_bytes, &resampler->pSampleOut));
		hr = resampler->pSampleOut->GetBufferByIndex(0, &pBufferOut);
		if(FAILED(hr))
		{
			SafeRelease(&resampler->pSampleOut);
			CHECK_HR(hr);
		}
	}
	else
	{
		DWORD dwMaxLength = 0;
		CHECK_HR(hr = resampler->pSampleOut->GetBufferByIndex(0, &pBufferOut));
		CHECK_HR(hr = pBufferOut->GetMaxLength(&dwMaxLength));
		if(dwMaxLength < resampler->out_size_bytes)
		{
			CHECK_HR(hr = resampler->pSampleOut->RemoveAllBuffers());
			SafeRelease(&pBufferOut);
			CHECK_HR(hr = MFCreateMemoryBuffer(resampler->out_size_bytes, &pBufferOut));    
			CHECK_HR(hr = resampler->pSampleOut->AddBuffer(pBufferOut));
		}
	}

	CHECK_HR(hr = pBufferOut->SetCurrentLength(0));
 
    //Set the output sample
    mftOutputData.pSample = resampler->pSampleOut;
    //Set the output id
    mftOutputData.dwStreamID = 0;

    //Generate the output sample
    hr = resampler->pMFT->ProcessOutput(0, 1, &mftOutputData, &dwStatus);
    if (hr == MF_E_TRANSFORM_NEED_MORE_INPUT)
    {
        hr = S_OK;
        goto bail;
    }

    // TODO: Handle MF_E_TRANSFORM_STREAM_CHANGE

    if (FAILED(hr))
    {
        goto bail;
    }

    *ppSample = resampler->pSampleOut;
    (*ppSample)->AddRef();

bail:
    SafeRelease(&pBufferOut);
    return hr;
}



//
//	Speex resamplerr Plugin definition
//

/* constructor */
static tsk_object_t* plugin_audio_dsp_resampler_ctor(tsk_object_t * self, va_list * app)
{
	plugin_audio_dsp_resampler_t *resampler = (plugin_audio_dsp_resampler_t *)self;
	if(resampler)
	{
		AudioDSPUtils::Startup();

		/* init base */
		tmedia_resampler_init(TMEDIA_RESAMPLER(resampler));
		/* init self */
	}
	return self;
}
/* destructor */
static tsk_object_t* plugin_audio_dsp_resampler_dtor(tsk_object_t * self)
{ 
	plugin_audio_dsp_resampler_t *resampler = (plugin_audio_dsp_resampler_t *)self;
	if(resampler)
	{
		/* deinit base */
		tmedia_resampler_deinit(TMEDIA_RESAMPLER(resampler));
		/* deinit self */
		// tmedia_resampler_deinit() already closed the resampler and freed the resources...but do it again
		SafeRelease(&resampler->pMFT);
		SafeRelease(&resampler->pSampleIn);
		SafeRelease(&resampler->pSampleOut);

		TSK_DEBUG_INFO("*** MS Audio Resampler DSP (plugin) destroyed ***");
	}

	return self;
}
/* object definition */
static const tsk_object_def_t plugin_audio_dsp_resampler_def_s = 
{
	sizeof(plugin_audio_dsp_resampler_t),
	plugin_audio_dsp_resampler_ctor, 
	plugin_audio_dsp_resampler_dtor,
	tsk_null, 
};
/* plugin definition*/
static const tmedia_resampler_plugin_def_t plugin_audio_dsp_resampler_plugin_def_s = 
{
	&plugin_audio_dsp_resampler_def_s,
	
	"MS Audio Resampler DSP", /* http://msdn.microsoft.com/en-us/library/windows/desktop/ff819070(v=vs.85).aspx */
	
	plugin_audio_dsp_resampler_open,
	plugin_audio_dsp_resampler_process,
	plugin_audio_dsp_resampler_close,
};
const tmedia_resampler_plugin_def_t *plugin_audio_dsp_resampler_plugin_def_t = &plugin_audio_dsp_resampler_plugin_def_s;
