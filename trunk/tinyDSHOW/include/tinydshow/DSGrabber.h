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
#if !defined(DIRECTSHOW_GRABBER_H)/* && !defined(_WIN32_WCE)*/
#define DIRECTSHOW_GRABBER_H

#include <tinydshow_config.h>

#include <tinydshow/DSCaptureGraph.h>
#include <tinydshow/VideoFrame.h>

#include "tinymedia/tmedia_producer.h"

#include "tsk_mutex.h"

class DSDisplay;

#ifdef _WIN32_WCE
#	include <tinydshow/wce/InxbISampleGrabberCB.h>
#endif


class DSGrabber : public
#ifdef _WIN32_WCE
	InxbISampleGrabberCB
#else
	ISampleGrabberCB
#endif
{
public:
	DSGrabber(HRESULT *hr);
	virtual ~DSGrabber();

	void setCallback(tmedia_producer_cb_f callback, const void* callback_data);
	void setCaptureDevice(const std::string &devicePath);

	virtual void start();
	virtual void stop();

	virtual bool setCaptureParameters(int format, int f);

	virtual VIDEOFORMAT getCaptureFormat();
	virtual int getFramerate();

	virtual HRESULT STDMETHODCALLTYPE SampleCB(double SampleTime, IMediaSample *pSample);
    virtual HRESULT STDMETHODCALLTYPE BufferCB(double SampleTime, BYTE *pBuffer, long BufferLen);

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();

	DSDisplay *preview;

private:
	int					width;
	int					height;
	int					fps;

	DSCaptureGraph		*graph;

	//VideoFrame			*currentFrame;
	BITMAPINFOHEADER	bitmapInfo;
	BYTE				*buffer;

	tsk_mutex_handle_t	*mutex_buffer;

	bool				first_buffer;

	const void* plugin_cb_data;
	tmedia_producer_cb_f plugin_cb;
};

#endif
