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
#include "internals/DSCaptureGraph.h"
#include "internals/DSUtils.h"
#include "internals/DSCaptureUtils.h"

#include "tsk_debug.h"

#include <iostream>

using namespace std;

#ifdef _WIN32_WCE
DSCaptureGraph::DSCaptureGraph(DSISampleGrabberCB* callback, HRESULT *hr)
    : DSBaseCaptureGraph(callback, hr)
#else
DSCaptureGraph::DSCaptureGraph(ISampleGrabberCB* callback, HRESULT *hr)
    : DSBaseCaptureGraph(callback, hr)
#endif
{
    this->grabberCallback = callback;

    this->captureFormat = NULL;
    this->captureGraphBuilder = NULL;
    this->graphBuilder = NULL;

    this->sourceFilter = NULL;
    this->sampleGrabberFilter = NULL;

#ifdef _WIN32_WCE
    this->colorConvertor565 = NULL;
#else
    this->frameRateFilter = NULL;
#endif

    this->nullRendererFilter = NULL;
    this->grabberController = NULL;
    this->mediaController = NULL;
    this->mediaEventController = NULL;

    this->streamConfiguration = NULL;

    this->running = FALSE;
    this->paused = FALSE;
    this->deviceId = "";

    *hr = this->createCaptureGraph();
}

DSCaptureGraph::~DSCaptureGraph()
{
    SAFE_RELEASE(this->streamConfiguration);

    SAFE_RELEASE(this->mediaEventController);
    SAFE_RELEASE(this->mediaController);
    SAFE_RELEASE(this->grabberController);

#if defined(_WIN32_WCE)
    SAFE_RELEASE(this->colorConvertor565);
#else
#endif

    SAFE_RELEASE(this->nullRendererFilter);
    SAFE_RELEASE(this->sampleGrabberFilter);
    SAFE_RELEASE(this->sourceFilter);

    SAFE_RELEASE(this->graphBuilder);
    SAFE_RELEASE(this->captureGraphBuilder);
}

HRESULT DSCaptureGraph::setSource(const std::string &devicePath)
{
    HRESULT hr = E_FAIL;

    if (this->sourceFilter) {
        this->graphBuilder->RemoveFilter(this->sourceFilter);
    }

    SAFE_RELEASE(this->streamConfiguration);
    SAFE_RELEASE(this->sourceFilter);

    // Create the filter
    this->deviceId = devicePath;
    hr = createSourceFilter(&this->deviceId, &this->sourceFilter);

    if (this->sourceFilter) {
        // Gets the supported formats
        this->supportedFormats.clear();
        getSupportedFormats(this->sourceFilter, &this->supportedFormats);

        // Query for video stream config
        hr = this->captureGraphBuilder->FindInterface(
                 &PIN_CATEGORY_CAPTURE,
                 &MEDIATYPE_Video,
                 this->sourceFilter,
                 IID_IAMStreamConfig,
                 reinterpret_cast<void**>(&this->streamConfiguration));

        hr = this->graphBuilder->AddFilter(this->sourceFilter, FILTER_WEBCAM);
    }

    return hr;
}

HRESULT DSCaptureGraph::setParameters(DSCaptureFormat *format, int framerate)
{
    HRESULT hr = E_FAIL;
    AM_MEDIA_TYPE *mediaType = NULL;

    if (!this->streamConfiguration) {
        goto bail;
    }

    hr = this->streamConfiguration->GetFormat(&mediaType);
    if (FAILED(hr)) {
        goto bail;
    }

    VIDEOINFOHEADER* vih = reinterpret_cast<VIDEOINFOHEADER*>(mediaType->pbFormat);
    BITMAPINFOHEADER* bih = &vih->bmiHeader;

    int w = format->getWidth();
    int h = format->getHeight();

    bool wN = (bih->biWidth<0);
    bool hN = (bih->biHeight<0);

    // DIBS are DWORD aligned
    int data_size = h * ((w * bih->biBitCount + 31) / 32) * 4;

    bih->biSize = sizeof(BITMAPINFOHEADER);
    bih->biWidth = w*(wN?-1:1);
    bih->biHeight = h*(hN?-1:1);
    bih->biSizeImage = data_size;

    //vih->dwBitRate = framerate * data_size;
    //vih->AvgTimePerFrame = SECONDS_TO_100NS(framerate);

    mediaType->cbFormat = sizeof(VIDEOINFOHEADER);
    //mediaType->lSampleSize = data_size;
    mediaType->subtype = format->getChroma();

    hr = this->streamConfiguration->SetFormat(mediaType);
    if (FAILED(hr)) {
        goto bail;
    }

#if defined(_WIN32_WCE)
    hr = this->grabberController->SetFps((int) DS_SECONDS_FROM_100NS(vih->AvgTimePerFrame)/*format->getFramerate()*/, framerate);
    if (FAILED(hr)) {
        goto bail;
    }
    hr = this->grabberController->SetSize(w,h);
#else
    // Set fps using tdshow filter
    hr = this->frameRateFilter->SetFps((int) ((float)vih->AvgTimePerFrame/10000.f)/*format->getFramerate()*/, framerate);
#endif
    if (FAILED(hr)) {
        goto bail;
    }

    this->captureFormat = format;

bail:
    DeleteMediaType(mediaType);

    return hr;
}

#if defined(_WIN32_WCE)
#	include "internals/wince/DSNullFilter.h"
#endif

HRESULT DSCaptureGraph::connect()
{
    HRESULT hr;

    if (!this->sourceFilter) {
        TSK_DEBUG_ERROR("Invalid source filter");
        return E_FAIL;
    }

    if (!this->captureFormat) {
        TSK_DEBUG_ERROR("Invalid capture format");
        return E_FAIL;
    }

    if (!this->graphBuilder) {
        TSK_DEBUG_ERROR("Invalid grash builder");
        return E_FAIL;
    }

    if (this->captureFormat->isRGB()) {
#if defined(_WIN32_WCE)
        hr = ConnectFilters(this->graphBuilder, this->sourceFilter, this->colorConvertor565)		;
        if(FAILED(hr)) {
            TSK_DEBUG_ERROR("ConnectFilters failed");
            return hr;
        }
        hr = ConnectFilters(this->graphBuilder, this->colorConvertor565, this->sampleGrabberFilter)	;
        if(FAILED(hr)) {
            TSK_DEBUG_ERROR("ConnectFilters failed");
            return hr;
        }
        hr = ConnectFilters(this->graphBuilder, this->sampleGrabberFilter, this->nullRendererFilter);
        if(FAILED(hr)) {
            TSK_DEBUG_ERROR("ConnectFilters failed");
            return hr;
        }
#else
        hr = ConnectFilters(this->graphBuilder, this->sourceFilter, this->frameRateFilter);
        if(FAILED(hr)) {
            TSK_DEBUG_ERROR("ConnectFilters failed");
            return hr;
        }
        hr = ConnectFilters(this->graphBuilder, this->frameRateFilter, this->sampleGrabberFilter);
        if(FAILED(hr)) {
            TSK_DEBUG_ERROR("ConnectFilters failed");
            return hr;
        }
        hr = ConnectFilters(this->graphBuilder, this->sampleGrabberFilter, this->nullRendererFilter);
        if(FAILED(hr)) {
            TSK_DEBUG_ERROR("ConnectFilters failed");
            return hr;
        }
#endif
    }
    else {
#if defined(_WIN32_WCE)
        hr = ConnectFilters(this->graphBuilder, this->sourceFilter, this->colorConvertor565)		;
        if(FAILED(hr)) {
            return hr;
        }
        hr = ConnectFilters(this->graphBuilder, this->colorConvertor565, this->sampleGrabberFilter)	;
        if(FAILED(hr)) {
            return hr;
        }
        hr = ConnectFilters(this->graphBuilder, this->sampleGrabberFilter, this->nullRendererFilter);
        if(FAILED(hr)) {
            return hr;
        }
#else
        // No convertor needed
        // AVI Decompressor Filter is automatically by the Filter Graph Manager when needed
        hr = ConnectFilters(this->graphBuilder, this->sourceFilter, this->frameRateFilter);
        if(FAILED(hr)) {
            TSK_DEBUG_ERROR("ConnectFilters failed");
            return hr;
        }
        hr = ConnectFilters(this->graphBuilder, this->frameRateFilter, this->sampleGrabberFilter);
        if(FAILED(hr)) {
            TSK_DEBUG_ERROR("ConnectFilters failed");
            return hr;
        }
        hr = ConnectFilters(this->graphBuilder, this->sampleGrabberFilter, this->nullRendererFilter);
        if(FAILED(hr)) {
            TSK_DEBUG_ERROR("ConnectFilters failed");
            return hr;
        }
#endif
    }

    return hr;
}

HRESULT DSCaptureGraph::disconnect()
{
    HRESULT hr;

    if (!this->sourceFilter) {
        return E_FAIL;
    }

    if (!this->captureFormat) {
        return E_FAIL;
    }

    if (this->captureFormat->isRGB()) {
#if defined(_WIN32_WCE)
        hr = DisconnectFilters(this->graphBuilder, this->sourceFilter, this->colorConvertor565);
        hr = DisconnectFilters(this->graphBuilder, this->colorConvertor565, this->sampleGrabberFilter);
        hr = DisconnectFilters(this->graphBuilder, this->sampleGrabberFilter, this->nullRendererFilter);
#else
        hr = DisconnectFilters(this->graphBuilder, this->sourceFilter, this->frameRateFilter);
        hr = DisconnectFilters(this->graphBuilder, this->frameRateFilter, this->sampleGrabberFilter);
        hr = DisconnectFilters(this->graphBuilder, this->sampleGrabberFilter, this->nullRendererFilter);
#endif
    }
    else {
#if defined(_WIN32_WCE)
        hr = DisconnectFilters(this->graphBuilder, this->sourceFilter, this->colorConvertor565);
        if(FAILED(hr)) {
            return hr;
        }
        hr = DisconnectFilters(this->graphBuilder, this->colorConvertor565, this->sampleGrabberFilter);
        if(FAILED(hr)) {
            return hr;
        }
        hr = DisconnectFilters(this->graphBuilder, this->sampleGrabberFilter, this->nullRendererFilter);
        if(FAILED(hr)) {
            return hr;
        }
#else
        hr = DisconnectFilters(this->graphBuilder, this->sourceFilter, this->frameRateFilter);
        hr = DisconnectFilters(this->graphBuilder, this->frameRateFilter, this->sampleGrabberFilter);
        hr = DisconnectFilters(this->graphBuilder, this->sampleGrabberFilter, this->nullRendererFilter);
#endif
    }

    return hr;
}

HRESULT DSCaptureGraph::start()
{
    HRESULT hr;

    if (isRunning() && !isPaused()) {
        return S_OK;
    }

    //this->mediaController->Stop();

    hr = this->mediaController ? this->mediaController->Run() : E_POINTER;
    /*if (hr == S_FALSE)
    {
    	cerr << "DSCaptureGraph::mediaController->Start() has failed with " << hr << ". Waiting for transition." << endl;
    	FILTER_STATE pfs;
    	hr = this->mediaController->GetState(2500, (OAFilterState*) &pfs);
    	hr = this->mediaController->Run();
    }*/

    if (!SUCCEEDED(hr)) {
#if defined(_WIN32_WCE)
        MessageBox(NULL, _T("Starting DirectShow Graph Failed"), _T("Failure"), MB_OK);
        //assert(1==15);
#endif
        TSK_DEBUG_ERROR("DSCaptureGraph::mediaController->Run() has failed with %ld", hr);
        return hr;
    }
    this->running = true;
    return hr;
}

HRESULT DSCaptureGraph::pause()
{
    HRESULT hr = S_OK;
    if (isRunning()) {
        hr = this->mediaController->Pause();
        if (SUCCEEDED(hr)) {
            this->paused = TRUE;
        }
    }
    return hr;
}

HRESULT DSCaptureGraph::stop()
{
    HRESULT hr;
#if 0 // Must not
    hr = this->mediaController->Pause();
    if (hr == S_FALSE) {
        TSK_DEBUG_ERROR("DSCaptureGraph::mediaController->Pause() has failed with %ld. Waiting for transition.", hr);
        FILTER_STATE pfs;
        hr = this->mediaController->GetState(2500, (OAFilterState*) &pfs);
    }
#endif
    hr = this->mediaController->Stop();
    if (!SUCCEEDED(hr)) {
        TSK_DEBUG_ERROR("DSCaptureGraph::mediaController->Stop() has failed with %ld", hr);
    }
    this->running = false;
    this->paused = false;
    return hr;
}

bool DSCaptureGraph::isRunning()
{
    return this->running;
}

bool DSCaptureGraph::isPaused()
{
    return this->paused;
}

HRESULT DSCaptureGraph::getConnectedMediaType(AM_MEDIA_TYPE *mediaType)
{
#if defined(_WIN32_WCE)
    memmove(mediaType, &this->grabberController->GetMediaType(), sizeof(AM_MEDIA_TYPE));
    return S_OK;
#else
    return this->grabberController->GetConnectedMediaType(mediaType);
#endif
}

HRESULT DSCaptureGraph::createCaptureGraph()
{
    HRESULT hr;

#if defined(_WIN32_WCE)
    // Create capture graph builder
    CHECK_HR(hr = COCREATE(CLSID_CaptureGraphBuilder, IID_ICaptureGraphBuilder2, this->captureGraphBuilder));
    CHECK_HR(hr = COCREATE(CLSID_FilterGraph, IID_IGraphBuilder, this->graphBuilder));
    CHECK_HR(hr = this->captureGraphBuilder->SetFiltergraph(this->graphBuilder));

    // Create filters
    LPUNKNOWN pUnk1 = NULL, pUnk2 = NULL;
    CHECK_HR(hr = COCREATE(CLSID_Colour, IID_IBaseFilter, this->colorConvertor565));
    this->sampleGrabberFilter = new DSSampleGrabber(FITLER_SAMPLE_GRABBER, pUnk1, &hr);
    CHECK_HR(hr);
    this->nullRendererFilter = new DSNullFilter(/*FILTER_NULL_RENDERER,*/ pUnk2, &hr);
    CHECK_HR(hr);
    this->grabberController = (DSSampleGrabber*)(this->sampleGrabberFilter);
    if (!this->grabberController) {
        CHECK_HR(E_FAIL);
    }

    // Add Filters
    CHECK_HR(hr = this->graphBuilder->AddFilter(this->colorConvertor565, FILTER_COLOR_CONVERTOR_565));
    CHECK_HR(hr = this->graphBuilder->AddFilter(this->sampleGrabberFilter, FITLER_SAMPLE_GRABBER));
    CHECK_HR(hr = this->graphBuilder->AddFilter(this->nullRendererFilter, FILTER_NULL_RENDERER));

    // Find media control
    CHECK_HR(hr = QUERY(this->graphBuilder, IID_IMediaControl, this->mediaController));

    // Set callback
    CHECK_HR(hr = this->grabberController->SetCallback(this->grabberCallback));
#else
    // Create capture graph builder
    CHECK_HR(hr = COCREATE(CLSID_CaptureGraphBuilder2, IID_ICaptureGraphBuilder2, this->captureGraphBuilder));

    // Create the graph builder
    CHECK_HR(hr = COCREATE(CLSID_FilterGraph, IID_IGraphBuilder, this->graphBuilder));

    // Initialize the Capture Graph Builder.
    CHECK_HR(hr = this->captureGraphBuilder->SetFiltergraph(this->graphBuilder));

    // Create the sample grabber filter
    CHECK_HR(hr = COCREATE(CLSID_SampleGrabber, IID_IBaseFilter, this->sampleGrabberFilter));

    // Create tdshow filter
    LPUNKNOWN pUnk = NULL;
    this->frameRateFilter = new DSFrameRateFilter(FILTER_FRAMERATE, pUnk, &hr);
    CHECK_HR(hr);
    if (!this->frameRateFilter == NULL) {
        CHECK_HR(E_FAIL);
    }

    // Create the NULL renderer
    CHECK_HR(hr = COCREATE(CLSID_NullRenderer, IID_IBaseFilter, this->nullRendererFilter));

    // Add sample grabber to the graph
    CHECK_HR(hr = this->graphBuilder->AddFilter(this->sampleGrabberFilter, FITLER_SAMPLE_GRABBER));

    // Add null renderer to the graph
    CHECK_HR(hr = this->graphBuilder->AddFilter(this->nullRendererFilter, FILTER_NULL_RENDERER));

    // Add tdshow filter
    CHECK_HR(hr = this->graphBuilder->AddFilter(this->frameRateFilter, FILTER_FRAMERATE));

    // Find media control
    CHECK_HR(hr = QUERY(this->graphBuilder, IID_IMediaControl, this->mediaController));

    // Create the sample grabber
    CHECK_HR(hr = QUERY(this->sampleGrabberFilter, IID_ISampleGrabber, this->grabberController));

    // Set the sample grabber media type (RGB24)
    // TODO : CHECK
    AM_MEDIA_TYPE mt;
    ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
    mt.majortype = MEDIATYPE_Video;
    mt.subtype = MEDIASUBTYPE_RGB24;
    mt.formattype = FORMAT_VideoInfo;

    CHECK_HR(hr = this->grabberController->SetMediaType(&mt));

    // Set sample grabber media type
    this->grabberController->SetOneShot(FALSE);
    this->grabberController->SetBufferSamples(FALSE);

    CHECK_HR(hr = this->grabberController->SetCallback(this->grabberCallback, 1));
#endif

bail:
    return hr;
}