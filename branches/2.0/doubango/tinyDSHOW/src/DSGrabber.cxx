/*
* Copyright (C) 2010-2011 Mamadou Diop.
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
#include <tinydshow/DSGrabber.h>
#include <tinydshow/DSDisplay.h>
#include <tinydshow/DSUtils.h>
#include <tinydshow/DSCaptureUtils.h>
#include <tinydshow/Resizer.h>

#include <tinydshow/DSUtils.h>

#include "tsk_debug.h"

using namespace std;

DSGrabber::DSGrabber(HRESULT *hr)
: mutex_buffer(NULL), preview(NULL)
{
#ifdef _WIN32_WCE
	this->graph = new DSCaptureGraph(this, hr);
#else
	this->graph = new DSCaptureGraph(this, hr);
	if(!FAILED(*hr)){
		this->preview = new DSDisplay(hr);
	}
#endif
	if (FAILED(*hr)){
		TSK_DEBUG_ERROR("Failed to create Grabber");
		return;
	}

	// Init the bitmap info header with default values
	memset(&(this->bitmapInfo), 0, sizeof(BITMAPINFOHEADER));
	this->bitmapInfo.biSize = sizeof(BITMAPINFOHEADER);
	this->bitmapInfo.biWidth = 352;
	this->bitmapInfo.biHeight = 288;
	this->bitmapInfo.biPlanes = 1;
	this->bitmapInfo.biBitCount = 24; 
	this->bitmapInfo.biCompression = 0;
	this->bitmapInfo.biXPelsPerMeter = 0;
	this->bitmapInfo.biYPelsPerMeter = 0;
	this->bitmapInfo.biClrUsed = 0;
	this->bitmapInfo.biClrImportant = 0;

	this->plugin_cb = NULL;
	this->buffer = NULL;
	this->mutex_buffer = tsk_mutex_create();
}

DSGrabber::~DSGrabber()
{
	SAFE_DELETE_PTR ( this->graph );
	SAFE_DELETE_PTR ( this->preview );
	SAFE_DELETE_ARRAY ( this->buffer  );
	tsk_mutex_destroy(&this->mutex_buffer);
}

void DSGrabber::setCaptureDevice(const std::string &devicePath)
{
	this->graph->setSource(devicePath);
}

void DSGrabber::setCallback(tmedia_producer_enc_cb_f callback, const void* callback_data)
{
	this->plugin_cb = callback;
	this->plugin_cb_data = callback_data;
}

void DSGrabber::start()
{
	if (!this->graph->isRunning()){
		first_buffer = true;

		if(this->preview){
			this->preview->start();
		}
		this->graph->connect();
		this->graph->start();
	}
}
void DSGrabber::stop()
{
	if (this->graph->isRunning()){
		if(this->preview){
			this->preview->stop();
		}
		this->graph->stop();
		this->graph->disconnect();
	}
}

bool DSGrabber::setCaptureParameters(int w, int h, int f)
{
	tsk_mutex_lock(this->mutex_buffer);

	// Store the framerate
	this->fps = f;
	this->width = w;
	this->height = h;

	// Store the required dimensions
	this->bitmapInfo.biWidth = this->width;
	this->bitmapInfo.biHeight = this->height;
	this->bitmapInfo.biBitCount = 24;
	this->bitmapInfo.biSizeImage = (this->width * this->height * 3);

	// Change the intermediate buffer
	SAFE_DELETE_ARRAY ( this->buffer  );
	this->buffer = new BYTE[this->bitmapInfo.biSizeImage];
	memset(this->buffer,0,this->bitmapInfo.biSizeImage);

	// Find closest matching format to drive the source filter
	DSCaptureFormat *fmt = NULL;
	int score = 0;
	std::vector<DSCaptureFormat> *formats = this->graph->getFormats();
	std::vector<DSCaptureFormat>::iterator iter;
	std::vector<DSCaptureFormat>::iterator last = formats->end();
	for(iter = formats->begin(); iter != last; iter++){
		int value = (*iter).getMatchScore(this->width, this->height);
		if (value > score || !fmt){
			score = value;
			fmt = &(*iter);
		}
	}

	// Setup source filter in the graph
	HRESULT hr = this->graph->setParameters(fmt, this->fps);
	// Set preview parameters
	if(this->preview){
		this->preview->setFps(this->fps);
		this->preview->setSize(this->width, this->height);
	}

	tsk_mutex_unlock(this->mutex_buffer);

	return SUCCEEDED(hr);
}

bool DSGrabber::setCaptureParameters(int format, int f)
{
	int w, h;
	// Get size from the format
	VIDEOFORMAT_TO_SIZE(format, w, h);
	return this->setCaptureParameters(w, h, f);
}

int DSGrabber::getFramerate()
{
	return this->fps;
}

HRESULT DSGrabber::SampleCB(double SampleTime, IMediaSample *pSample)
{
	return S_OK;
}

HRESULT DSGrabber::BufferCB(double SampleTime, BYTE *pBuffer, long BufferLen)
{	
	HRESULT hr;

	tsk_mutex_lock(this->mutex_buffer);

	AM_MEDIA_TYPE mediaType;
	hr = this->graph->getConnectedMediaType(&mediaType);
	if (FAILED(hr) || !this->buffer){
		return hr;
	}

	if(first_buffer){
		first_buffer = false;

		tsk_mutex_unlock(this->mutex_buffer);
		return hr;
	}

	// Examine the format block.
	if ((mediaType.formattype == FORMAT_VideoInfo) && (mediaType.cbFormat >= sizeof(VIDEOINFOHEADER)) && (mediaType.pbFormat != NULL) ) 
	{
		VIDEOINFOHEADER *pVih = reinterpret_cast<VIDEOINFOHEADER *>(mediaType.pbFormat);
		BITMAPINFOHEADER* bih = &pVih->bmiHeader;

		//int framerate = pVih->AvgTimePerFrame;
		if( (bih->biHeight  == this->bitmapInfo.biHeight) && (bih->biWidth  == this->bitmapInfo.biWidth) && (bih->biBitCount  == this->bitmapInfo.biBitCount) )
		{
			memmove(this->buffer, pBuffer, this->bitmapInfo.biSizeImage);
		}
		else
		{
			ResizeRGB(
				bih,
				(const unsigned char *) pBuffer,
				&this->bitmapInfo,
				(unsigned char *) this->buffer,
				this->width,
				this->height);
		}

		// for the network
		if(this->plugin_cb){
			this->plugin_cb(this->plugin_cb_data, this->buffer, (this->width*this->height*3));
		}

		// for the preview
		if(this->preview){
			this->preview->handleVideoFrame(this->buffer, this->width, this->height);
		}
	}

	// Free the format
#ifdef _WIN32_WCE
	// Nothing had been allocated
#else
	FreeMediaType(mediaType);
#endif

	tsk_mutex_unlock(this->mutex_buffer);

	return hr;
}

HRESULT DSGrabber::QueryInterface(REFIID iid, LPVOID *ppv)
{
#ifdef _WIN32_WCE
	assert(1==0);
#else
	if( iid == IID_ISampleGrabberCB || iid == IID_IUnknown )
	{ 
		*ppv = (void *) static_cast<ISampleGrabberCB*>(this);
		return NOERROR;
	} 
#endif
	return E_NOINTERFACE;
}

ULONG DSGrabber::AddRef()
{
	return 2;
}

ULONG DSGrabber::Release()
{
	return 1;
}