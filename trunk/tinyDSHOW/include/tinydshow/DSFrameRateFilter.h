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
#ifndef DSFrameRateFilter_H
#define DSFrameRateFilter_H

#include <tinydshow_config.h>

#include <streams.h>
#include <math.h>

// TODO: do it only once
#if !defined(TDSHOW_DEFINE_GUID) && !defined(_WIN32_WCE)
#define TDSHOW_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        EXTERN_C const GUID DECLSPEC_SELECTANY name \
                = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
#elif !defined(TDSHOW_DEFINE_GUID) && defined(_WIN32_WCE)
#define TDSHOW_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        EXTERN_C const GUID __declspec(selectany) name \
                = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
#endif

// {7F9F08CF-139F-40b2-A283-01C4EC26A452}
TDSHOW_DEFINE_GUID(CLSID_DSFrameRateFilter, 
0x7f9f08cf, 0x139f, 0x40b2, 0xa2, 0x83, 0x1, 0xc4, 0xec, 0x26, 0xa4, 0x52);

class DSFrameRateFilter : public CTransInPlaceFilter
{
public:
	DSFrameRateFilter(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr);
	~DSFrameRateFilter(void);

public:
	HRESULT Transform(IMediaSample *pSample);
    HRESULT CheckInputType(const CMediaType* mtIn);

public:
	/**
	* \def		SetFps
	* \brief	fps1 define source .
	*/
	HRESULT SetFps(int inputFps, int outputFps);

    static CUnknown *WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *phr);
    DECLARE_IUNKNOWN;

#ifdef _WIN32_WCE
	STDMETHODIMP_(ULONG) NonDelegatingRelease();
#endif

private:
	int m_progress;
	int m_inputFps, m_outputFps;
	bool m_bProcessFrame;
	REFERENCE_TIME m_rtFrameLength;		// UNITS/fps
	LONGLONG m_iFrameNumber;
};

#endif ////DSFrameRateFilter_H