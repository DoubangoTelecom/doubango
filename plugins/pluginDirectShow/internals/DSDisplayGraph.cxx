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
#if defined(VMR9)
#define DIRECT3D_VERSION 0x0900
#endif

#include "internals/DSDisplayGraph.h"
#include "internals/DSUtils.h"
#include "internals/DSOutputFilter.h"

#include "tsk_debug.h"

#include <iostream>

using namespace std;

DSDisplayGraph::DSDisplayGraph(HRESULT *hr)
{
	this->running = FALSE;
	this->paused = FALSE;
	this->fps = 15;
	
	this->graphBuilder = NULL;

	this->sourceFilter = NULL;
	this->colorspaceConverterFilter = NULL;
	this->videoRendererFilter = NULL;

	this->mediaController = NULL;
	this->mediaEvent = NULL;
	this->videoWindow = NULL;

#if defined(VMR) ||defined(VMR9) || defined(VMR9_WINDOWLESS)
	this->mixerBitmap = NULL;
	this->filterConfig = NULL;
#endif

#if defined(VMR9_WINDOWLESS)
	this->windowlessControl = NULL;
#endif

	*hr = this->createDisplayGraph();
	if (FAILED(*hr)) return;

	*hr = this->connect();
	if (FAILED(*hr)) return;
}

DSDisplayGraph::~DSDisplayGraph()
{
	this->disconnect();

#if defined(VMR9_WINDOWLESS)
	SAFE_RELEASE(this->windowlessControl);
#endif

#if defined(VMR) ||defined(VMR9) || defined(VMR9_WINDOWLESS)
	SAFE_RELEASE(this->filterConfig);
	SAFE_RELEASE(this->mixerBitmap);
#endif

	SAFE_RELEASE(this->videoWindow);
	SAFE_RELEASE(this->mediaEvent);
	SAFE_RELEASE(this->mediaController);

	SAFE_RELEASE(this->colorspaceConverterFilter);
	SAFE_RELEASE(this->videoRendererFilter);
	//SAFE_RELEASE(this->sourceFilter);

	SAFE_RELEASE(this->graphBuilder);
}

void DSDisplayGraph::setDisplayFps(int fps_)
{
	this->fps = fps_;
	if(this->sourceFilter){
		this->sourceFilter->setFps(fps_);
	}
}

bool DSDisplayGraph::getImageFormat(UINT &width, UINT &height)
{
	if(this->sourceFilter){
		return this->sourceFilter->getImageFormat(width, height);
	}
	return false;
}

bool DSDisplayGraph::setImageFormat(UINT width, UINT height)
{
	bool ret = true;
	if(this->sourceFilter){
		UINT w=width, h = height;
		if(this->sourceFilter->getImageFormat(w, h)){
			if(w!= width || h!=height){ // Image format has changed
				bool reconnect = this->connected; // IMPORTANT: Must reconnect all elements
				HRESULT hr;
				if(reconnect){
					if((hr = this->disconnect()) != S_OK){
						return false;
					}
				}
				ret = (this->sourceFilter->setImageFormat(width, height) == S_OK);
				if(reconnect){
					if((hr = this->connect())){
						return false;
					}
				}
			}
		}
	}
	return ret;
}

HRESULT DSDisplayGraph::connect()
{
	HRESULT hr;

	if((hr = ConnectFilters(this->graphBuilder, this->sourceFilter, this->colorspaceConverterFilter)) != S_OK){
		TSK_DEBUG_ERROR("Failed to connect sourcefilter with the colorspace");
		return hr;
	}
	if((hr = ConnectFilters(this->graphBuilder, this->colorspaceConverterFilter, this->videoRendererFilter)) != S_OK){
		TSK_DEBUG_ERROR("Failed to connect colorspace with the videorenderer");
		return hr;
	}

	this->connected = true;
	return S_OK;
}

HRESULT DSDisplayGraph::disconnect()
{
	HRESULT hr;

	if((hr = DisconnectFilters(this->graphBuilder, this->sourceFilter, this->colorspaceConverterFilter)) != S_OK){
		TSK_DEBUG_ERROR("Failed to disconnect sourcefilter with the colorspace");
		return hr;
	}
	if((hr = DisconnectFilters(this->graphBuilder, this->colorspaceConverterFilter, this->videoRendererFilter)) != S_OK){
		TSK_DEBUG_ERROR("Failed to connect colorspace with the videorenderer");
		return hr;
	}

	this->connected = false;
	return S_OK;
}

HRESULT DSDisplayGraph::start()
{
	HRESULT hr;
	this->running = true;
	this->sourceFilter->reset();

	hr = this->mediaController->Run();
	if (!SUCCEEDED(hr)){
		TSK_DEBUG_ERROR("DSDisplayGraph::mediaController->Run() has failed with %ld", hr);
	}
	return hr;
}

HRESULT DSDisplayGraph::pause()
{
	HRESULT hr = S_OK;
	if(isRunning() && !isPaused()){
		hr = this->mediaController->Pause();
		if(SUCCEEDED(hr)){
			this->paused = true;
		}
	}
	return hr;
}

HRESULT DSDisplayGraph::stop()
{
	HRESULT hr;

	hr = this->mediaController->Pause();
	if (hr == S_FALSE){
		TSK_DEBUG_ERROR("DSDisplayGraph::mediaController->Pause() has failed with %ld. Waiting for transition.", hr);
		FILTER_STATE pfs;
		hr = this->mediaController->GetState(2500, (OAFilterState*) &pfs);
	}

	hr = this->mediaController->Stop();
	if (!SUCCEEDED(hr)){
		TSK_DEBUG_ERROR("DSDisplayGraph::mediaController->Stop() has failed with %ld", hr);
	}

	this->running = false;
	this->paused = false;

	return hr;
}

bool DSDisplayGraph::isRunning()
{
	return this->running;
}

bool DSDisplayGraph::isPaused()
{
	return this->paused;
}

void DSDisplayGraph::handleFrame(const void* data, int w, int h)
{
	HRESULT hr;
	
	if(!this->sourceFilter){
		TSK_DEBUG_ERROR("Invalid parameter");
		return;
	}

	if(!data || !this->running){
		this->sourceFilter->setBuffer(NULL, (w*h*3));
		return;
	}
	
	hr = this->sourceFilter->setImageFormat(w, h);
	if (hr == S_OK){
		this->stop();

		this->disconnect();
		this->connect();

		this->start();
	}

	this->sourceFilter->setBuffer((void*)data, (w*h*3));
}

HRESULT DSDisplayGraph::createDisplayGraph()
{
	HRESULT hr;

	// Create the graph builder
	hr = COCREATE(CLSID_FilterGraph, IID_IGraphBuilder, this->graphBuilder);
	if(FAILED(hr)) return hr;


	// Create my custom filter
	LPUNKNOWN pUnk = NULL;
	this->sourceFilter = new DSOutputFilter(pUnk, &hr /*, this*/);
	if(FAILED(hr) || this->sourceFilter == NULL) return hr;

	// Create the color space convertor filter
	hr = COCREATE(CLSID_Colour, IID_IBaseFilter, this->colorspaceConverterFilter);
	if(FAILED(hr)) return hr;

#if defined(VMR)
	// Create the video mixing renderer based on Direct X
	hr = COCREATE(CLSID_VideoMixingRenderer, IID_IBaseFilter, this->videoRendererFilter);
	if(FAILED(hr)) return hr;
#elif defined(VMR9) || defined(VMR9_WINDOWLESS)
	// Create the video mixing renderer based on Direct X 9.0
	hr = COCREATE(CLSID_VideoMixingRenderer9, IID_IBaseFilter, this->videoRendererFilter);
	if(FAILED(hr)) return hr;
#else
	// Create the video renderer
	hr = COCREATE(CLSID_VideoRenderer, IID_IBaseFilter, this->videoRendererFilter);
	if(FAILED(hr)) return hr;
#endif


	// Add dource filter to the graph
	hr = this->graphBuilder->AddFilter(this->sourceFilter, FILTER_OUTPUT);
	if(FAILED(hr)) return hr;

	// Add the color space convertor to the graph
	hr = this->graphBuilder->AddFilter(this->colorspaceConverterFilter, FILTER_COLORSPACE_CONVERTOR);
	if(FAILED(hr)) return hr;

	// Add video renderer to the graph
	hr = this->graphBuilder->AddFilter(this->videoRendererFilter, FILTER_VIDEO_RENDERER);
	if(FAILED(hr)) return hr;


	// Find media control
	hr = QUERY(this->graphBuilder, IID_IMediaControl, this->mediaController);
	if(FAILED(hr)) return hr;

	// Find media event
	hr = QUERY(this->graphBuilder, IID_IMediaEventEx, this->mediaEvent);
	if(FAILED(hr)) return hr;
	// hr = this->mediaEvent->SetNotifyFlags(AM_MEDIAEVENT_NONOTIFY);


#if defined(VMR)
	// Find the bitmap mixer (Direct X)
	hr = QUERY(this->videoRendererFilter, IID_IVMRMixerBitmap, this->mixerBitmap);
	if(FAILED(hr)) return hr;

	// Find the bitmap configurer (Direct X)
	hr = QUERY(this->videoRendererFilter, IID_IVMRFilterConfig, this->filterConfig);
	if(FAILED(hr)) return hr;

	// Set the number of streams (Direct X)
	hr = this->filterConfig->SetNumberOfStreams(1);
	if(FAILED(hr)) return hr;
#elif defined(VMR9) || defined(VMR9_WINDOWLESS)
	// Find the bitmap mixer (Direct X 9.0)
	hr = QUERY(this->videoRendererFilter, IID_IVMRMixerBitmap9, this->mixerBitmap);
	if(FAILED(hr)) return hr;

	// Find the bitmap configurer (Direct X 9.0)
	hr = QUERY(this->videoRendererFilter, IID_IVMRFilterConfig9, this->filterConfig);
	if(FAILED(hr)) return hr;

	// Set the number of streams (Direct X 9.0)
	hr = this->filterConfig->SetNumberOfStreams(1);
	if(FAILED(hr)) return hr;
#endif

#if defined(VMR9_WINDOWLESS)
	// Set the rendering mode (Direct X 9.0)
	hr = this->filterConfig->SetRenderingMode(VMR9Mode_Windowless);
	if(FAILED(hr)) return hr;

	// Find the windowless control (Direct X 9.0)
	hr = QUERY(this->videoRendererFilter, IID_IVMRWindowlessControl9, this->windowlessControl);
	if(FAILED(hr)) return hr;
#else
	// Find IVideoWindow interface
	hr = QUERY(this->graphBuilder, IID_IVideoWindow, this->videoWindow);
	if(FAILED(hr)) return  hr;
#endif

	return hr;
}
