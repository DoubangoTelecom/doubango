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
#ifndef PLUGIN_AUDIO_DSP_UTILS_H
#define PLUGIN_AUDIO_DSP_UTILS_H

#include "plugin_audio_dsp_config.h"

#include <Windows.h>
#include <mfidl.h>

#undef CHECK_HR
// In CHECK_HR(x) When (x) is a function it will be executed twice when used in "TSK_DEBUG_ERROR(x)" and "If(x)"
#define CHECK_HR(x) { HRESULT __hr__ = (x); if (FAILED(__hr__)) { TSK_DEBUG_ERROR("Operation Failed (%08x)", __hr__); goto bail; } }

#if !defined(PLUGIN_AUDIO_DSP_MILLIS_TO_100NS)
#	define PLUGIN_AUDIO_DSP_MILLIS_TO_100NS(MILLIS) (((LONGLONG)(MILLIS)) * 10000ui64)
#endif

#undef SafeRelease
#define SafeRelease(ppT) \
{ \
    if (*ppT) \
    { \
        (*ppT)->Release(); \
        *ppT = NULL; \
    } \
} 

class AudioDSPUtils
{
public:
	static HRESULT Startup();
	static HRESULT Shutdown();
	static HRESULT CreatePCMAudioType(
		UINT32 sampleRate,        // Samples per second
		UINT32 bitsPerSample,     // Bits per sample
		UINT32 cChannels,         // Number of channels
		IMFMediaType **ppType     // Receives a pointer to the media type.
    );
	static HRESULT CreateMediaSample(
		DWORD cbData, // Maximum buffer size
		IMFSample **ppSample // Receives the sample
	);
	static HRESULT MoInitMediaType(
		UINT32 sampleRate,        // Samples per second
		UINT32 bitsPerSample,     // Bits per sample
		UINT32 cChannels,         // Number of channels
		DMO_MEDIA_TYPE *pType     // The media type to initialize. Must be freed using MoFreeMediaType.
		);

private:
	static bool g_bStarted;
};

#endif /* PLUGIN_AUDIO_DSP_UTILS_H */

