/*
* Copyright (C) 2010-2011 Mamadou DIOP.
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
#include "internals/DSOutputFilter.h"
#include "internals/DSOutputStream.h"
#include "internals/DSUtils.h"

#include "tsk_memory.h"

DSOutputFilter::DSOutputFilter(LPUNKNOWN pUnk, HRESULT *phr)
    : CSource(_T("TDSHOW_OUTPUT"), pUnk, CLSID_TdshowOutputFilter)
{
#if !(defined(_WIN32_WCE) && defined(_DEBUG))
    CAutoLock cAutoLock(&m_cStateLock);
#endif

    // Add one source stream (output pin)!
    this->outputStream = new DSOutputStream(phr, this, _T("Out"));
}

DSOutputFilter::~DSOutputFilter()
{
    //SAFE_RELEASE(this->outputStream);
}

void DSOutputFilter::setBuffer(void *pointer, int size)
{
    this->outputStream->lockBuffer();
    if(pointer && size) {
        if(this->outputStream->buffer_size != size) {
            if((this->outputStream->buffer = tsk_realloc(this->outputStream->buffer, size))) {
                this->outputStream->buffer_size = size;
            }
            else {
                goto done;
            }
        }
        memcpy(this->outputStream->buffer, pointer, size);
    }
done:
    this->outputStream->unlockBuffer();
}

void DSOutputFilter::getMediaType(AM_MEDIA_TYPE* &pmt)
{
    //if(pmt)
    //{
    //	memcpy(pmt, &this->outputStream->pmt, sizeof(AM_MEDIA_TYPE));
    //}
}

HRESULT DSOutputFilter::setMediaType(const AM_MEDIA_TYPE* pmt)
{
    return this->ReconnectPin(this->outputStream, pmt);
}

HRESULT DSOutputFilter::setImageFormat(UINT width, UINT height)
{
    return this->outputStream->setImageFormat(width, height);
}

bool DSOutputFilter::getImageFormat(UINT &width, UINT &height)
{
    if(this->outputStream) {
        return this->outputStream->getImageFormat(width, height);
    }
    return false;
}

void DSOutputFilter::setFps(int fps_)
{
    this->outputStream->setFps(fps_);
}

void DSOutputFilter::showOverlay(int value)
{
    this->outputStream->showOverlay(value);
}

void DSOutputFilter::reset()
{
    this->outputStream->frameNumber = 0;
    this->outputStream->lockBuffer();
    this->outputStream->buffer = NULL;
    this->outputStream->buffer_size = 0;
    this->outputStream->unlockBuffer();
}

#ifdef _WIN32_WCE
STDMETHODIMP_(ULONG) DSOutputFilter::NonDelegatingRelease()
{
    if(InterlockedDecrement(&m_cRef) == 0) {
        delete this;
        return 0;
    }
    return m_cRef;
}
#endif
