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
#include "plugin_audio_dsp_utils.h"
#include "tsk_debug.h"

#include <uuids.h>
#include <Dmo.h>
#include <Mfapi.h>
#include <assert.h>

bool AudioDSPUtils::g_bStarted = false;

HRESULT AudioDSPUtils::Startup()
{
	if(!g_bStarted)
	{
		HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
		if(SUCCEEDED(hr) || hr == 0x80010106) // 0x80010106 when called from managed code (e.g. Boghe) - More info: http://support.microsoft.com/kb/824480
		{
			hr = MFStartup(MF_VERSION);
		}
		g_bStarted = SUCCEEDED(hr);
		return hr;
	}
	return S_OK;
}

HRESULT AudioDSPUtils::Shutdown()
{
	return S_OK;
}

HRESULT AudioDSPUtils::CreatePCMAudioType(
    UINT32 sampleRate,        // Samples per second
    UINT32 bitsPerSample,     // Bits per sample
    UINT32 cChannels,         // Number of channels
    IMFMediaType **ppType     // Receives a pointer to the media type.
    )
{
    HRESULT hr = S_OK;

    IMFMediaType *pType = NULL;

    // Calculate derived values.
    UINT32 blockAlign = cChannels * (bitsPerSample >> 3);
    UINT32 bytesPerSecond = blockAlign * sampleRate;

    // Create the empty media type.
	CHECK_HR(hr = MFCreateMediaType(&pType));

    // Set attributes on the type.
    CHECK_HR(hr = pType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio));
   
	CHECK_HR(hr = pType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM));
    
	CHECK_HR(hr = pType->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, cChannels));
    
	CHECK_HR(hr = pType->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, sampleRate));
       
	CHECK_HR(hr = pType->SetUINT32(MF_MT_AUDIO_BLOCK_ALIGNMENT, blockAlign));
    
	CHECK_HR(hr = pType->SetUINT32(MF_MT_AUDIO_AVG_BYTES_PER_SECOND, bytesPerSecond));
    
	CHECK_HR(hr = pType->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, bitsPerSample));
    
	CHECK_HR(hr = pType->SetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, TRUE));

	CHECK_HR(hr = pType->SetUINT32(MF_MT_FIXED_SIZE_SAMPLES, TRUE));
    
    *ppType = pType;
    (*ppType)->AddRef();

bail:
    SafeRelease(&pType);
    return hr;
}

HRESULT AudioDSPUtils::CreateMediaSample(
	DWORD cbData, // Maximum buffer size
	IMFSample **ppSample // Receives the sample
)
{
	HRESULT hr = S_OK;

	if(!ppSample)
	{
		CHECK_HR(hr = E_POINTER);
	}

    IMFSample *pSample = NULL;
    IMFMediaBuffer *pBuffer = NULL;

    CHECK_HR(hr = MFCreateSample(&pSample));    
    CHECK_HR(hr = MFCreateMemoryBuffer(cbData, &pBuffer));    
    CHECK_HR(hr = pSample->AddBuffer(pBuffer));
    
    *ppSample = pSample;
    (*ppSample)->AddRef();

bail:
    SafeRelease(&pSample);
    SafeRelease(&pBuffer);
    return hr;
}

HRESULT AudioDSPUtils::MoInitMediaType(
		UINT32 sampleRate,        // Samples per second
		UINT32 bitsPerSample,     // Bits per sample
		UINT32 cChannels,         // Number of channels
		DMO_MEDIA_TYPE *pType     // The media type to initialize. Must be freed using MoFreeMediaType.
		)
{
	HRESULT hr = S_OK;
	WAVEFORMATEX *pWAV = NULL;

	if(!pType)
	{
		CHECK_HR(hr = E_POINTER);
	}

	pType->majortype = MEDIATYPE_Audio;
	pType->subtype = MEDIASUBTYPE_PCM;
	pType->lSampleSize = 0;
	pType->bFixedSizeSamples = TRUE;
	pType->bTemporalCompression = FALSE;
	pType->formattype = FORMAT_WaveFormatEx;

	CHECK_HR(hr = ::MoInitMediaType(pType, sizeof(WAVEFORMATEX)));

	pWAV = (WAVEFORMATEX*)pType->pbFormat;
    pWAV->wFormatTag = WAVE_FORMAT_PCM;
    pWAV->nChannels = 1;
    pWAV->nSamplesPerSec = sampleRate;
	pWAV->nBlockAlign = cChannels * (bitsPerSample >> 3);
    pWAV->nAvgBytesPerSec = pWAV->nBlockAlign * pWAV->nSamplesPerSec;
    pWAV->wBitsPerSample = bitsPerSample;
    pWAV->cbSize = 0;

bail:
	return hr;
}
