#if !defined(_WIN32_WCE)
/* Copyright (C) 2014 Mamadou DIOP
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
#include <streams.h>
#include "internals/DSScreenCastGraph.h"
#include "internals/DSPushSource.h"
#include "internals/DSUtils.h"
#include "internals/DSCaptureUtils.h"

#include "tsk_debug.h"

#include <iostream>

using namespace std;

DSScreenCastGraph::DSScreenCastGraph(ISampleGrabberCB* callback, HRESULT *hr)
    : DSBaseCaptureGraph(callback, hr)
{
    this->grabberCallback = callback;

    this->captureFormat = NULL;
    this->captureGraphBuilder = NULL;
    this->graphBuilder = NULL;

    this->sourceFilter = NULL;
    this->sampleGrabberFilter = NULL;

    this->nullRendererFilter = NULL;
    this->grabberController = NULL;
    this->mediaController = NULL;
    this->mediaEventController = NULL;

    this->running = FALSE;
    this->paused = FALSE;

    *hr = this->createCaptureGraph();
}

DSScreenCastGraph::~DSScreenCastGraph()
{
    SAFE_RELEASE(this->mediaEventController);
    SAFE_RELEASE(this->mediaController);
    SAFE_RELEASE(this->grabberController);

    SAFE_RELEASE(this->nullRendererFilter);
    SAFE_RELEASE(this->sampleGrabberFilter);

    SAFE_RELEASE(this->graphBuilder);
    SAFE_RELEASE(this->captureGraphBuilder);

    SAFE_RELEASE(this->sourceFilter);
}

HRESULT DSScreenCastGraph::setParameters(DSCaptureFormat *format, int framerate)
{
    return S_OK;
}

#ifdef _WIN32_WCE
#	include <tinydshow/wce/DSInxbNullFilter.h>
#endif

HRESULT DSScreenCastGraph::connect()
{
    HRESULT hr;

    if (!this->sourceFilter) {
        TSK_DEBUG_ERROR("Invalid source filter");
        return E_FAIL;
    }
#if 0
    if (!this->captureFormat) {
        TSK_DEBUG_ERROR("Invalid capture format");
        return E_FAIL;
    }
#endif

    hr = ConnectFilters(this->graphBuilder, this->sourceFilter, this->sampleGrabberFilter);
    if(FAILED(hr)) {
        TSK_DEBUG_ERROR("ConnectFilters failed");
        return hr;
    }
    hr = ConnectFilters(this->graphBuilder, this->sampleGrabberFilter, this->nullRendererFilter);
    if(FAILED(hr)) {
        TSK_DEBUG_ERROR("ConnectFilters failed");
        return hr;
    }

    return hr;
}

HRESULT DSScreenCastGraph::disconnect()
{
    HRESULT hr;

    if (!this->sourceFilter) {
        return E_FAIL;
    }
#if 0
    if (!this->captureFormat) {
        return E_FAIL;
    }
#endif

    hr = DisconnectFilters(this->graphBuilder, this->sourceFilter, this->sampleGrabberFilter);
    hr = DisconnectFilters(this->graphBuilder, this->sampleGrabberFilter, this->nullRendererFilter);

    return hr;
}

HRESULT DSScreenCastGraph::start()
{
    HRESULT hr;

    if (isRunning() && !isPaused()) {
        return S_OK;
    }

    hr = this->mediaController->Run();
    if (!SUCCEEDED(hr)) {
        TSK_DEBUG_ERROR("DSScreenCastGraph::mediaController->Run() has failed with %ld", hr);
        return hr;
    }
    this->running = true;
    return hr;
}

HRESULT DSScreenCastGraph::pause()
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

HRESULT DSScreenCastGraph::stop()
{
    if (!this->running) {
        return S_OK;
    }

    HRESULT hr;
    hr = this->mediaController->Stop();
    if (FAILED(hr)) {
        TSK_DEBUG_ERROR("DSScreenCastGraph::mediaController->Stop() has failed with %ld", hr);
    }
    this->running = false;
    this->paused = false;
    return hr;
}

bool DSScreenCastGraph::isRunning()
{
    return this->running;
}

bool DSScreenCastGraph::isPaused()
{
    return this->paused;
}

HRESULT DSScreenCastGraph::getConnectedMediaType(AM_MEDIA_TYPE *mediaType)
{
    return this->grabberController->GetConnectedMediaType(mediaType);
}

HRESULT DSScreenCastGraph::createCaptureGraph()
{
    HRESULT hr;

    // Create capture graph builder
    hr = COCREATE(CLSID_CaptureGraphBuilder2, IID_ICaptureGraphBuilder2, this->captureGraphBuilder);
    if(FAILED(hr)) {
        return hr;
    }

    // Create the graph builder
    hr = COCREATE(CLSID_FilterGraph, IID_IGraphBuilder, this->graphBuilder);
    if(FAILED(hr)) {
        return hr;
    }

    // Initialize the Capture Graph Builder.
    hr = this->captureGraphBuilder->SetFiltergraph(this->graphBuilder);
    if(FAILED(hr)) {
        return hr;
    }

    // Create source filter
    LPUNKNOWN pUnk = NULL;
    this->sourceFilter = (CPushSourceDesktop*)CPushSourceDesktop::CreateInstance(pUnk, &hr);
    if(FAILED(hr)) {
        return hr;
    }
    this->sourceFilter->AddRef();

    // Create the sample grabber filter
    hr = COCREATE(CLSID_SampleGrabber, IID_IBaseFilter, this->sampleGrabberFilter);
    if(FAILED(hr)) {
        return hr;
    }

    // Create the NULL renderer
    hr = COCREATE(CLSID_NullRenderer, IID_IBaseFilter, this->nullRendererFilter);
    if(FAILED(hr)) {
        return hr;
    }

    // Add source filter to the graph
    hr = this->graphBuilder->AddFilter(this->sourceFilter, FILTER_SCREENCAST);
    if(FAILED(hr)) {
        return hr;
    }

    // Add sample grabber to the graph
    hr = this->graphBuilder->AddFilter(this->sampleGrabberFilter, FITLER_SAMPLE_GRABBER);
    if(FAILED(hr)) {
        return hr;
    }

    // Add null renderer to the graph
    hr = this->graphBuilder->AddFilter(this->nullRendererFilter, FILTER_NULL_RENDERER);
    if(FAILED(hr)) {
        return hr;
    }

    // Find media control
    hr = QUERY(this->graphBuilder, IID_IMediaControl, this->mediaController);
    if(FAILED(hr)) {
        return hr;
    }

    // Create the sample grabber controller
    hr = QUERY(this->sampleGrabberFilter, IID_ISampleGrabber, this->grabberController);
    if(FAILED(hr)) {
        return hr;
    }

    // Set the sample grabber media type (RGB24)
    // TODO : CHECK
    AM_MEDIA_TYPE mt;
    ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
    mt.majortype = MEDIATYPE_Video;
    mt.subtype = MEDIASUBTYPE_RGB24;
    mt.formattype = FORMAT_VideoInfo;

    hr = this->grabberController->SetMediaType(&mt);
    if(FAILED(hr)) {
        return hr;
    }

    // Set sample grabber media type
    this->grabberController->SetOneShot(FALSE);
    this->grabberController->SetBufferSamples(FALSE);

    hr = this->grabberController->SetCallback(this->grabberCallback, 1);
    if(FAILED(hr)) {
        return hr;
    }

    return hr;
}

#endif /* _WIN32_WCE */