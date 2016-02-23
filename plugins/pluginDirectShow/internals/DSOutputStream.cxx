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
#include "internals/DSOutputStream.h"
#include "internals/DSOutputFilter.h"
#include "internals/DSUtils.h"

#include <iostream>

#include "tsk_memory.h"

using namespace std;

#define DEFAULT_FPS	15

#define MEMCPY_WORKAROUND 1

// Overlay
#define OVERLAY 0
#define OVERLAY_TEXT TEXT("Press ESC to exit full screen mode")
#define OVERLAY_DURATION 3 // in seconds

DSOutputStream::DSOutputStream(HRESULT *phr, DSOutputFilter *pParent, LPCWSTR pPinName)
    : CSourceStream(_T("DSOutputStream"), phr, pParent, pPinName)
{
#if !(defined(_WIN32_WCE) && defined(_DEBUG))
    CAutoLock cAutoLock(m_pFilter->pStateLock());
#endif

    this->buffer = NULL;
    this->buffer_size = NULL;

    this->frameNumber = 0;
    this->frameLength = (1000)/DEFAULT_FPS;
    this->fps = DEFAULT_FPS;

    this->width = 352;
    this->height = 288;

    this->overlay = false;

    this->paintBuffer = NULL;
    this->paintDC = NULL;
    this->hDibSection = NULL;
    this->hObject = NULL;

    this->mutex = tsk_mutex_create();
}

DSOutputStream::~DSOutputStream()
{
    TSK_FREE(this->buffer);
    tsk_mutex_destroy(&this->mutex);
    // TODO : Is there anything to free ???
}

void DSOutputStream::setFps(int fps_)
{
    this->fps = fps_;
    this->frameLength = (1000)/this->fps;
}

void DSOutputStream::showOverlay(int value)
{
    if (value == 0) {
        this->overlay = false;
    }
    this->overlay = (value > 0);
}

HRESULT DSOutputStream::setImageFormat(UINT width, UINT height)
{
    if ((this->width == width) && (this->height == height)) {
        return S_FALSE;
    }

    this->width = width;
    this->height = height;

    this->frameNumber = 0;

    return S_OK;
}

bool DSOutputStream::getImageFormat(UINT &width, UINT &height)
{
    width = this->width;
    height = this->height;
    return true;
}

HRESULT DSOutputStream::GetMediaType(CMediaType *pMediaType)
{
    HRESULT hr = S_OK;
#if !(defined(_WIN32_WCE) && defined(_DEBUG))
    CAutoLock lock(m_pFilter->pStateLock());
#endif

    ZeroMemory(pMediaType, sizeof(CMediaType));

    VIDEOINFO *pvi = (VIDEOINFO *)pMediaType->AllocFormatBuffer(sizeof(VIDEOINFO));
    if (NULL == pvi) {
        return E_OUTOFMEMORY;
    }

    ZeroMemory(pvi, sizeof(VIDEOINFO));

    pvi->bmiHeader.biCompression = BI_RGB;
    pvi->bmiHeader.biBitCount = 24;
    pvi->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
    pvi->bmiHeader.biWidth			= this->width;
    pvi->bmiHeader.biHeight			= this->height;
    pvi->bmiHeader.biPlanes			= 1;
    pvi->bmiHeader.biSizeImage		= GetBitmapSize(&pvi->bmiHeader);
    pvi->bmiHeader.biClrImportant	= 0;

    // Frame rate
    pvi->AvgTimePerFrame			= DS_MILLIS_TO_100NS(1000/this->fps);

    SetRectEmpty(&(pvi->rcSource));	// we want the whole image area rendered.
    SetRectEmpty(&(pvi->rcTarget));	// no particular destination rectangle

    pMediaType->SetType(&MEDIATYPE_Video);
    pMediaType->SetFormatType(&FORMAT_VideoInfo);
    pMediaType->SetTemporalCompression(FALSE);

    pMediaType->SetSubtype(&MEDIASUBTYPE_RGB24);
    pMediaType->SetSampleSize(pvi->bmiHeader.biSizeImage);

    bitmapInfo.bmiHeader = pvi->bmiHeader;

    return hr;
}

HRESULT DSOutputStream::DecideBufferSize(IMemAllocator *pMemAlloc, ALLOCATOR_PROPERTIES *pProperties)
{
    CheckPointer(pMemAlloc, E_POINTER);
    CheckPointer(pProperties, E_POINTER);

#if !(defined(_WIN32_WCE) && defined(_DEBUG))
    CAutoLock cAutoLock(m_pFilter->pStateLock());
#endif

    HRESULT hr = NOERROR;

    VIDEOINFO *pvi = (VIDEOINFO *) m_mt.Format();
    pProperties->cBuffers = 1;
    pProperties->cbBuffer = pvi->bmiHeader.biSizeImage;

    // Ask the allocator to reserve us some sample memory. NOTE: the function
    // can succeed (return NOERROR) but still not have allocated the
    // memory that we requested, so we must check we got whatever we wanted.
    ALLOCATOR_PROPERTIES Actual;
    hr = pMemAlloc->SetProperties(pProperties,&Actual);
    if(FAILED(hr)) {
        return hr;
    }

    // Is this allocator unsuitable?
    if(Actual.cbBuffer < pProperties->cbBuffer) {
        return E_FAIL;
    }

    // Make sure that we have only 1 buffer (we erase the ball in the
    // old buffer to save having to zero a 200k+ buffer every time
    // we draw a frame)
    return NOERROR;
}

HRESULT DSOutputStream::OnThreadCreate()
{
#if OVERLAY
    hDibSection = CreateDIBSection(NULL, (BITMAPINFO *) &bitmapInfo, DIB_RGB_COLORS, &paintBuffer, NULL, 0);

    HDC hDC = GetDC(NULL);
    paintDC = CreateCompatibleDC(hDC);
    SetMapMode(paintDC, GetMapMode(hDC));
    SetBkMode(paintDC, TRANSPARENT);
    SetTextColor(paintDC, RGB(255,255,255));

    hObject = SelectObject(paintDC, hDibSection);
#endif

    return CSourceStream::OnThreadCreate();
}

HRESULT DSOutputStream::OnThreadDestroy()
{
#if OVERLAY
    if (paintDC) {
        DeleteDC(paintDC);
    }
    if (hObject) {
        DeleteObject(hObject);
    }

    if (paintBuffer) {
        //delete[] paintBuffer; // will be done
        //paintBuffer = NULL;
    }
#endif
    return CSourceStream::OnThreadDestroy();
}

inline HRESULT DSOutputStream::DrawOverLay(void *pBuffer, long lSize)
{
    // called only #if OVERLAY
    CopyMemory(paintBuffer, pBuffer, lSize);

    // Draw the current frame
#ifdef _WIN32_WCE

#else
    if( !TextOut( paintDC, 0, 0, OVERLAY_TEXT, (int)_tcslen( OVERLAY_TEXT ) ) ) {
        return E_FAIL;
    }
#endif

    CopyMemory(pBuffer, paintBuffer, lSize);

    return S_OK;
}

static __inline void TransfertBuffer(void* src, void* dest, long lSize)
{
    __try {
#if MEMCPY_WORKAROUND
        //#ifdef _WIN32_WCE
        memmove(dest, src, lSize);
        /*#else
        		unsigned char * pDst = (unsigned char *) dest;

        		if(src){
        			unsigned char const * pSrc = (unsigned char const *) src;
        			for( register int i=0; ((i< lSize) && src); i++) *pDst++ = *pSrc++;
        		}else{
        			for( register int i=0; i< lSize; i++) *pDst++ = 0;
        		}
        #endif*/
#else
        CopyMemory(dest, src, lSize); //BUGGY
#endif
    }
    __except(EXCEPTION_ACCESS_VIOLATION == GetExceptionCode()) {
        //ZeroMemory(dest, sizeof(void*));
    }
}

HRESULT DSOutputStream::FillBuffer(IMediaSample *pSample)
{
    CheckPointer(pSample, E_POINTER);
#if !(defined(_WIN32_WCE) && defined(_DEBUG))
    CAutoLock lock(m_pFilter->pStateLock());
#endif

    HRESULT hr;
    BYTE *pBuffer = NULL;
    long lSize, lDataSize;

    hr = pSample->GetPointer(&pBuffer);
    if (SUCCEEDED(hr)) {
        lDataSize = lSize = pSample->GetSize();

        // Check that we're still using video
        //ASSERT(m_mt.formattype == FORMAT_VideoInfo);

        if (this->buffer) {
#if OVERLAY
            if (this->overlay) {
                DrawOverLay(this->buffer, lSize);
            }
#endif
            // Why try do not work, see: http://msdn2.microsoft.com/en-us/library/xwtb73ad(vs.80).aspx
            this->lockBuffer();
            lDataSize = TSK_MIN(lSize, this->buffer_size);
            TransfertBuffer(this->buffer, (void*)pBuffer, lDataSize);
            this->unlockBuffer();
        }
        else {
            // Avoid caching last image
            memset((void*)pBuffer, NULL, lSize);
        }

        REFERENCE_TIME rtStart = DS_MILLIS_TO_100NS(this->frameNumber * this->frameLength);
        REFERENCE_TIME rtStop  = rtStart + DS_MILLIS_TO_100NS(this->frameLength);

        this->frameNumber++;

        pSample->SetTime(&rtStart, &rtStop);
        //pSample->SetMediaTime(&rtStart, &rtStop);
        pSample->SetActualDataLength(lDataSize);
        pSample->SetPreroll(FALSE);
        pSample->SetDiscontinuity(FALSE);
    }

    // Set TRUE on every sample for uncompressed frames (KEYFRAME)
    pSample->SetSyncPoint(TRUE);

    return S_OK;
}
