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
#include "internals/DSFrameRateFilter.h"

#include <iostream>
#include <string>

using namespace std;

#define FPS_INPUT  30
#define FPS_OUTPUT 5

// {7F9F08CF-139F-40b2-A283-01C4EC26A452}
TDSHOW_DEFINE_GUID(CLSID_DSFrameRateFilter,
                   0x7f9f08cf, 0x139f, 0x40b2, 0xa2, 0x83, 0x1, 0xc4, 0xec, 0x26, 0xa4, 0x52);

DSFrameRateFilter::DSFrameRateFilter(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr)
    :CTransInPlaceFilter (tszName, punk, CLSID_DSFrameRateFilter, phr)
{
    this->m_rtFrameLength = (10000000)/FPS_OUTPUT;

    this->m_inputFps = FPS_INPUT;
    this->m_outputFps = FPS_OUTPUT;

    this->m_iFrameNumber = 0;
    this->m_progress = 0;
    this->m_bProcessFrame = true;
}

DSFrameRateFilter::~DSFrameRateFilter()
{
}

HRESULT DSFrameRateFilter::SetFps(int inputFps, int outputFps)
{
    if(inputFps <= 0 || outputFps <= 0) {
        return E_FAIL;
    }

    // Stop prcessing
    this->m_bProcessFrame = false;

    if (inputFps < outputFps) {
        this->m_inputFps = this->m_outputFps = inputFps;
    }
    else {
        this->m_outputFps = outputFps;
        this->m_inputFps = inputFps;
    }

    // Restart processing
    this->m_iFrameNumber = 0;
    this->m_progress = 0;
    this->m_bProcessFrame = true;

    return S_OK;
}

HRESULT DSFrameRateFilter::Transform(IMediaSample *pSample)
{
    if(!this->m_bProcessFrame) {
        return S_FALSE;
    }

    CheckPointer(pSample, E_POINTER);

    HRESULT hr = S_OK;
    HRESULT ret = S_FALSE;

    pSample->SetTime(NULL, NULL);

    // Drop frame?
    if (this->m_iFrameNumber == 0) {
        ret = S_OK;
    }
    else if (this->m_progress >= this->m_inputFps) {
        this->m_progress -= this->m_inputFps;
        ret = S_OK;
    }

    // Mark frame as accepted
    if (ret == S_OK) {
        // Set TRUE on every sample for uncompressed frames
        pSample->SetSyncPoint(TRUE);
    }

    this->m_progress += this->m_outputFps;
    this->m_iFrameNumber++;

    return ret;
}

HRESULT DSFrameRateFilter::CheckInputType(const CMediaType* mtIn)
{
    return S_OK;
}

//Implement CreateInstance for your filter object. Typically, CreateInstance calls the constructor of your filter clas
CUnknown * WINAPI DSFrameRateFilter::CreateInstance(LPUNKNOWN punk, HRESULT *phr)
{
    DSFrameRateFilter *pNewObject = new DSFrameRateFilter(_T("Tdshow DirectShow Framerate Limiter Filter."), punk, phr );
    if (pNewObject == NULL) {
        *phr = E_OUTOFMEMORY;
    }
    return pNewObject;
}
