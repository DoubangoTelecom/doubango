/* Copyright (C) 2011-2013 Doubango Telecom <http://www.doubango.org>
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
#ifndef PLUGIN_DSHOW_DSFRAMERATEFILTER_H
#define PLUGIN_DSHOW_DSFRAMERATEFILTER_H

#include "plugin_dshow_config.h"

#include <streams.h>
#include <math.h>

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

    /*STDMETHODIMP_(ULONG) NonDelegatingRelease()
    {
    	if(InterlockedDecrement(&m_cRef) == 0)
    	{
    		delete this;
    		return 0;
    	}
    	return m_cRef;
    }*/

private:
    int m_progress;
    int m_inputFps, m_outputFps;
    bool m_bProcessFrame;
    REFERENCE_TIME m_rtFrameLength;		// UNITS/fps
    LONGLONG m_iFrameNumber;
};

#endif ////DSFrameRateFilter_H